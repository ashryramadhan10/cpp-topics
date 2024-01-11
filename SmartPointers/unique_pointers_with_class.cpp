#include <memory>

class Device {
};

class Settings {
    std::unique_ptr<Device> device;
public:
    Settings(std::unique_ptr<Device> d) {
        device = std::move(d);
    }

    Device* getDevice() {
        return device.get();
    }
};

int main() {
    std::unique_ptr<Device> device(new Device());
    Settings settings(std::move(device));
    // ...
    Device *myDevice = settings.getDevice();
    // do something with myDevice...
}