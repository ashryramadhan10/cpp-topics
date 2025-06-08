#include  <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    int val = 0x12345678;
    char* bytes = (char*)&val;
    for (int i = 0; i < sizeof(int); i++) {
        printf("Byte %d: %02X\n", i, (unsigned char)bytes[i]);
    }

    return 0;
}