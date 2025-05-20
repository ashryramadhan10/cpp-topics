#include <stdio.h>
#include <stdlib.h>

int main() {

    void* mem = malloc(10);
    char* byte_ptr = (char*)mem;
    byte_ptr[0] = 0xFF;

    printf("%c\n", byte_ptr[0]);

    int val = 0x12345678;
    char* bytes = (char*)&val;
    for (int i = 0; i < sizeof(int); i++) {
        printf("Byte %d: %02X\n", i, (unsigned char)bytes[i]);
    }

    free(mem);
    return 0;
}