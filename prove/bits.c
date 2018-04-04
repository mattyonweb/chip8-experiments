#include <stdio.h>

int main() {
    for (int i=0; i<10; i++) {
        unsigned char c = (rand()%255) & 0x03;
        printf("%d\n", c);
    }
}
