#include <stdio.h>

int main() {
    unsigned char c = 0xB7;
    unsigned char d = 0x80;
    
    //~ unsigned char e = c + d;
    printf("%d", (d & 0b10000000) >> 7);
}
