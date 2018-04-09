
#include <stdio.h>

int main() {
    unsigned char a = 0x40;
    unsigned char c,d,u;
    
    c = 100 * ((int)a / 100);
    d = 10  * (((int)a - c) / 10);
    u = a - c - d;
    d = (unsigned char)(d / 10);
    c = (unsigned char)(c / 100);
    
    printf("%d\n", a);
    printf("%d, %d, %d", c, d, u);
    
}
