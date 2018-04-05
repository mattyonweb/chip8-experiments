
#include <stdio.h>

int main() {
    unsigned char a = 0x00,
                  b = 0xEE;
                  
    unsigned char   b1 = (a & 0xF0) >> 4,
                        b2 = a & 0xF,
                        b3 = (b & 0xF0) >> 4,
                        b4 = b & 0xF;
    
    printf("%02x%02x\n", a, b);
    printf("%x%x%x%x\n", b1,b2,b3,b4);
}
