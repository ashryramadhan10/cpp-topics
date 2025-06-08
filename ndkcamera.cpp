// ndkcamera_no_rotation.cpp

#include "ndkcamera.h"

#include <string>
#include <android/log.h>
#include <opencv2/core/core.hpp>

#include "mat.h"

// -----------------------------------------------------------------
// Basic camera callback definitions (unchanged)
static void onDisconnected(void* context, ACameraDevice* device)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onDisconnected %p", device);
}

static void onError(void* context, ACameraDevice* device, int error)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onError %p %d", device, error);
}

static void onImageAvailable(void* context, AImageReader* reader)
{
    AImage* image = nullptr;
    media_status_t status = AImageReader_acquireLatestImage(reader, &image);
    if (status != AMEDIA_OK)
    {
        // error
        return;
    }

    int32_t format;
    AImage_getFormat(image, &format);
    // assert format == AIMAGE_FORMAT_YUV_420_888

    int32_t width = 0;
    int32_t height = 0;
    AImage_getWidth(image, &width);
    AImage_getHeight(image, &height);

    int32_t y_pixelStride = 0, u_pixelStride = 0, v_pixelStride = 0;
    AImage_getPlanePixelStride(image, 0, &y_pixelStride);
    AImage_getPlanePixelStride(image, 1, &u_pixelStride);
    AImage_getPlanePixelStride(image, 2, &v_pixelStride);

    int32_t y_rowStride = 0, u_rowStride = 0, v_rowStride = 0;
    AImage_getPlaneRowStride(image, 0, &y_rowStride);
    AImage_getPlaneRowStride(image, 1, &u_rowStride);
    AImage_getPlaneRowStride(image, 2, &v_rowStride);

    uint8_t* y_data = nullptr;
    uint8_t* u_data = nullptr;
    uint8_t* v_data = nullptr;
    int y_len = 0, u_len = 0, v_len = 0;
    AImage_getPlaneData(image, 0, &y_data, &y_len);
    AImage_getPlaneData(image, 1, &u_data, &u_len);
    AImage_getPlaneData(image, 2, &v_data, &v_len);

    // If the layout is already NV21
    if (u_data == v_data + 1
        && v_data == y_data + width * height
        && y_pixelStride == 1 && u_pixelStride == 2 && v_pixelStride == 2
        && y_rowStride == width && u_rowStride == width && v_rowStride == width)
    {
        // Use directly
        ((NdkCamera*)context)->on_image((unsigned char*)y_data, (int)width, (int)height);
    }
    else
    {
        // Construct NV21 manually
        unsigned char* nv21 = new unsigned char[width * height + width * height / 2];

        // Copy Y
        {
            unsigned char* yptr = nv21;
            for (int row = 0; row < height; row++)
            {
                const unsigned char* row_ptr = y_data + y_rowStride * row;
                for (int col = 0; col < width; col++)
                {
                    *yptr++ = *row_ptr;
                    row_ptr += y_pixelStride;
                }
            }
        }
        // Copy UV
        {
            unsigned char* uvptr = nv21 + width * height;
            for (int row = 0; row < height / 2; row++)
            {
                const unsigned char* v_row_ptr = v_data + v_rowStride * row;
                const unsigned char* u_row_ptr = u_data + u_rowStride * row;
                for (int col = 0; col < width / 2; col++)
                {
                    *uvptr++ = *v_row_ptr;
                    *uvptr++ = *u_row_ptr;
                    v_row_ptr += v_pixelStride;
                    u_row_ptr += u_pixelStride;
                }
            }
        }

        ((NdkCamera*)context)->on_image((unsigned char*)nv21, (int)width, (int)height);
        delete[] nv21;
    }

    AImage_delete(image);
}

// Session callbacks
static void onSessionActive(void* context, ACameraCaptureSession *session)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onSessionActive %p", session);
}

static void onSessionReady(void* context, ACameraCaptureSession *session)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onSessionReady %p", session);
}

static void onSessionClosed(void* context, ACameraCaptureSession *session)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onSessionClosed %p", session);
}

void onCaptureFailed(void* context, ACameraCaptureSession* session, ACaptureRequest* request, ACameraCaptureFailure* failure)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onCaptureFailed %p %p %p", session, request, failure);
}

void onCaptureSequenceCompleted(void* context, ACameraCaptureSession* session, int sequenceId, int64_t frameNumber)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onCaptureSequenceCompleted %p %d %ld", session, sequenceId, frameNumber);
}

void onCaptureSequenceAborted(void* context, ACameraCaptureSession* session, int sequenceId)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "onCaptureSequenceAborted %p %d", session, sequenceId);
}

void onCaptureCompleted(void* context, ACameraCaptureSession* session, ACaptureRequest* request, const ACameraMetadata* result)
{
    // optional debug
}

// -----------------------------------------------------------------
// NdkCamera Implementation
NdkCamera::NdkCamera()
{
    camera_facing = 0;
    camera_orientation = 0;

    camera_manager = nullptr;
    camera_device = nullptr;
    image_reader = nullptr;
    image_reader_surface = nullptr;
    image_reader_target = nullptr;
    capture_request = nullptr;
    capture_session_output_container = nullptr;
    capture_session_output = nullptr;
    capture_session = nullptr;

    // Create an AImageReader (640x480 as example)
    {
        AImageReader_new(640, 480, AIMAGE_FORMAT_YUV_420_888, /*maxImages*/2, &image_reader);

        AImageReader_ImageListener listener;
        listener.context = this;
        listener.onImageAvailable = onImageAvailable;

        AImageReader_setImageListener(image_reader, &listener);

        AImageReader_getWindow(image_reader, &image_reader_surface);
        ANativeWindow_acquire(image_reader_surface);
    }
}

NdkCamera::~NdkCamera()
{
    close();

    if (image_reader)
    {
        AImageReader_delete(image_reader);
        image_reader = nullptr;
    }

    if (image_reader_surface)
    {
        ANativeWindow_release(image_reader_surface);
        image_reader_surface = nullptr;
    }
}

int NdkCamera::open(int _camera_facing)
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "open");

    camera_facing = _camera_facing;
    camera_manager = ACameraManager_create();

    // Find camera (front or back)
    std::string camera_id;
    {
        ACameraIdList* camera_id_list = nullptr;
        ACameraManager_getCameraIdList(camera_manager, &camera_id_list);

        for (int i = 0; i < camera_id_list->numCameras; ++i)
        {
            const char* id = camera_id_list->cameraIds[i];
            ACameraMetadata* camera_metadata = nullptr;
            ACameraManager_getCameraCharacteristics(camera_manager, id, &camera_metadata);

            acamera_metadata_enum_android_lens_facing_t facing = ACAMERA_LENS_FACING_FRONT;
            {
                ACameraMetadata_const_entry e = { 0 };
                ACameraMetadata_getConstEntry(camera_metadata, ACAMERA_LENS_FACING, &e);
                facing = (acamera_metadata_enum_android_lens_facing_t)e.data.u8[0];
            }

            if (camera_facing == 0 && facing != ACAMERA_LENS_FACING_FRONT)
            {
                ACameraMetadata_free(camera_metadata);
                continue;
            }
            if (camera_facing == 1 && facing != ACAMERA_LENS_FACING_BACK)
            {
                ACameraMetadata_free(camera_metadata);
                continue;
            }

            camera_id = id;

            // We read orientation but do NOT use it
            {
                ACameraMetadata_const_entry e = { 0 };
                ACameraMetadata_getConstEntry(camera_metadata, ACAMERA_SENSOR_ORIENTATION, &e);
                camera_orientation = (int)e.data.i32[0];
            }

            ACameraMetadata_free(camera_metadata);
            break;
        }

        ACameraManager_deleteCameraIdList(camera_id_list);
    }

    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "open %s orientation=%d", camera_id.c_str(), camera_orientation);

    // Open camera
    {
        ACameraDevice_StateCallbacks camera_device_state_callbacks;
        camera_device_state_callbacks.context = this;
        camera_device_state_callbacks.onDisconnected = onDisconnected;
        camera_device_state_callbacks.onError = onError;

        ACameraManager_openCamera(camera_manager, camera_id.c_str(), &camera_device_state_callbacks, &camera_device);
    }

    // Create capture request
    {
        ACameraDevice_createCaptureRequest(camera_device, TEMPLATE_PREVIEW, &capture_request);

        ACameraOutputTarget_create(image_reader_surface, &image_reader_target);
        ACaptureRequest_addTarget(capture_request, image_reader_target);
    }

    // Create capture session
    {
        ACameraCaptureSession_stateCallbacks camera_capture_session_state_callbacks;
        camera_capture_session_state_callbacks.context = this;
        camera_capture_session_state_callbacks.onActive = onSessionActive;
        camera_capture_session_state_callbacks.onReady = onSessionReady;
        camera_capture_session_state_callbacks.onClosed = onSessionClosed;

        ACaptureSessionOutputContainer_create(&capture_session_output_container);

        ACaptureSessionOutput_create(image_reader_surface, &capture_session_output);
        ACaptureSessionOutputContainer_add(capture_session_output_container, capture_session_output);

        ACameraDevice_createCaptureSession(camera_device,
                                           capture_session_output_container,
                                           &camera_capture_session_state_callbacks,
                                           &capture_session);

        ACameraCaptureSession_captureCallbacks camera_capture_session_capture_callbacks;
        camera_capture_session_capture_callbacks.context = this;
        camera_capture_session_capture_callbacks.onCaptureStarted = nullptr;
        camera_capture_session_capture_callbacks.onCaptureProgressed = nullptr;
        camera_capture_session_capture_callbacks.onCaptureCompleted = onCaptureCompleted;
        camera_capture_session_capture_callbacks.onCaptureFailed = onCaptureFailed;
        camera_capture_session_capture_callbacks.onCaptureSequenceCompleted = onCaptureSequenceCompleted;
        camera_capture_session_capture_callbacks.onCaptureSequenceAborted = onCaptureSequenceAborted;
        camera_capture_session_capture_callbacks.onCaptureBufferLost = nullptr;

        ACameraCaptureSession_setRepeatingRequest(capture_session,
                                                  &camera_capture_session_capture_callbacks,
                                                  1, &capture_request, nullptr);
    }

    return 0;
}

void NdkCamera::close()
{
    __android_log_print(ANDROID_LOG_WARN, "NdkCamera", "close");

    if (capture_session)
    {
        ACameraCaptureSession_stopRepeating(capture_session);
        ACameraCaptureSession_close(capture_session);
        capture_session = nullptr;
    }
    if (camera_device)
    {
        ACameraDevice_close(camera_device);
        camera_device = nullptr;
    }
    if (capture_session_output_container)
    {
        ACaptureSessionOutputContainer_free(capture_session_output_container);
        capture_session_output_container = nullptr;
    }
    if (capture_session_output)
    {
        ACaptureSessionOutput_free(capture_session_output);
        capture_session_output = nullptr;
    }
    if (capture_request)
    {
        ACaptureRequest_free(capture_request);
        capture_request = nullptr;
    }
    if (image_reader_target)
    {
        ACameraOutputTarget_free(image_reader_target);
        image_reader_target = nullptr;
    }
    if (camera_manager)
    {
        ACameraManager_delete(camera_manager);
        camera_manager = nullptr;
    }
}

void NdkCamera::on_image(const cv::Mat& rgb) const
{
    // This is just a virtual callback you can implement in a subclass
}

// -----------------------------------------------------------------
// The CRITICAL part: We do NO rotation in this function
// We simply convert NV21 -> RGB and pass it along unchanged
void NdkCamera::on_image(const unsigned char* nv21, int nv21_width, int nv21_height) const
{
    // 1) Convert NV21 to RGB with no rotation
    cv::Mat rgb(nv21_height, nv21_width, CV_8UC3);
    ncnn::yuv420sp2rgb(nv21, nv21_width, nv21_height, rgb.data);

    // 2) Use a callback
    on_image(rgb);
}

// -----------------------------------------------------------------
// NdkCameraWindow Implementation (subclass of NdkCamera)
static const int NDKCAMERAWINDOW_ID = 233;

NdkCameraWindow::NdkCameraWindow() : NdkCamera()
{
    sensor_manager = nullptr;
    sensor_event_queue = nullptr;
    accelerometer_sensor = nullptr;
    win = nullptr;

    accelerometer_orientation = 0;
    // We do NOT rely on these for orientation anymore
    // You can remove them entirely if you wish
}

NdkCameraWindow::~NdkCameraWindow()
{
    // Cleanup
    if (accelerometer_sensor)
    {
        ASensorEventQueue_disableSensor(sensor_event_queue, accelerometer_sensor);
        accelerometer_sensor = nullptr;
    }
    if (sensor_event_queue)
    {
        ASensorManager_destroyEventQueue(sensor_manager, sensor_event_queue);
        sensor_event_queue = nullptr;
    }
    if (win)
    {
        ANativeWindow_release(win);
        win = nullptr;
    }
}

void NdkCameraWindow::set_window(ANativeWindow* _win)
{
    if (win)
    {
        ANativeWindow_release(win);
    }
    win = _win;
    ANativeWindow_acquire(win);
}

// This callback is where you can do detection/drawing
void NdkCameraWindow::on_image_render(cv::Mat& rgb) const
{
    // e.g. do YOLO detection, draw boxes, etc.
}

// We override on_image(...) to display the frame with NO rotation/cropping
void NdkCameraWindow::on_image(const unsigned char* nv21, int nv21_width, int nv21_height) const
{
    // NO accelerometer usage - removed all sensor logic
    // Just do a plain NV21->RGB
    cv::Mat rgb(nv21_height, nv21_width, CV_8UC3);
    ncnn::yuv420sp2rgb(nv21, nv21_width, nv21_height, rgb.data);

    // Optional: pass to detection or rendering
    on_image_render(rgb);

    // Now post 'rgb' to window as-is
    ANativeWindow_setBuffersGeometry(win, nv21_width, nv21_height,
                                     AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);

    ANativeWindow_Buffer buf;
    ANativeWindow_lock(win, &buf, NULL);

    if (buf.format == AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM ||
        buf.format == AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM)
    {
        for (int y = 0; y < nv21_height; y++)
        {
            const unsigned char* src = rgb.ptr<const unsigned char>(y);
            unsigned char* dst = (unsigned char*)buf.bits + buf.stride * 4 * y;

            for (int x = 0; x < nv21_width; x++)
            {
                dst[0] = src[0]; // R
                dst[1] = src[1]; // G
                dst[2] = src[2]; // B
                dst[3] = 255;    // A
                src += 3;
                dst += 4;
            }
        }
    }

    ANativeWindow_unlockAndPost(win);
}
