#include <iostream>
#include <iomanip>
#include <limits>
#include <cfloat>

/*
The output should look like this:
=====================================================================
                   char:                      -128 to 127
          unsigned char:                         0 to 255
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
                  short:                    -32768 to 32767
         unsigned short:                         0 to 65535
                wchar_t:                         0 to 65535
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
                    int:               -2147483648 to 2147483647
           unsigned int:                         0 to 4294967295
               long int:               -2147483648 to 2147483647
      unsigned long int:                         0 to 4294967295
                  float:               1.17549e-38 to 3.40282e+38
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          long long int:      -9223372036854775808 to 9223372036854775807
 unsigned long long int:                         0 to 18446744073709551615
                 double:              2.22507e-308 to 1.79769e+308
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            long double:              3.3621e-4932 to 1.18973e+4932
=====================================================================
*/

int main(int argc, char* argv[]) {

    int space_size = 35;

    // 1 bytes
    std::cout << std::setw(25) << "char: " << std::setw(25) << CHAR_MIN << " to " << CHAR_MAX << std::endl;
    std::cout << std::setw(25) << "unsigned char: " << std::setw(25) << "0" << " to " << UCHAR_MAX << std::endl;

    for (int i = 0; i < space_size; i++) {std::cout << "- ";}
    std::cout << std::endl;

    // 2 bytes
    std::cout << std::setw(25) << "short: " << std::setw(25) << SHRT_MIN << " to " << SHRT_MAX << std::endl;
    std::cout << std::setw(25) << "unsigned short: " << std::setw(25) << "0" << " to " << USHRT_MAX << std::endl;

    std::cout << std::setw(25) << "wchar_t: " << std::setw(25) << WCHAR_MIN << " to " << WCHAR_MAX << std::endl;

    for (int i = 0; i < space_size; i++) {std::cout << "- ";}
    std::cout << std::endl;

    // 4 bytes
    std::cout << std::setw(25) << "int: " << std::setw(25) << INT_MIN << " to " << INT_MAX << std::endl;
    std::cout << std::setw(25) << "unsigned int: " << std::setw(25) << "0" << " to " << UINT_MAX << std::endl;

    std::cout << std::setw(25) << "long int: " << std::setw(25) << LONG_MIN << " to " << LONG_MAX << std::endl;
    std::cout << std::setw(25) << "unsigned long int: " << std::setw(25) << "0" << " to " << ULONG_MAX << std::endl;

    std::cout << std::setw(25) << "float: " << std::setw(25) << FLT_MIN << " to " << FLT_MAX << std::endl;

    for (int i = 0; i < space_size; i++) {std::cout << "- ";}
    std::cout << std::endl;

    // 8 bytes
    std::cout << std::setw(25) << "long long int: " << std::setw(25) << LLONG_MIN << " to " << LLONG_MAX << std::endl;
    std::cout << std::setw(25) << "unsigned long long int: " << std::setw(25) << "0" << " to " << ULLONG_MAX << std::endl;

    std::cout << std::setw(25) << "double: " << std::setw(25) << DBL_MIN << " to " << DBL_MAX << std::endl;

    for (int i = 0; i < space_size; i++) {std::cout << "- ";}
    std::cout << std::endl;

    // 16 bytes
    std::cout << std::setw(25) << "long double: " << std::setw(25) << LDBL_MIN << " to " << LDBL_MAX << std::endl;

    return 0;
}