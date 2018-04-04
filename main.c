#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "emu.h"

void prompt(Chip8 chip8);

int main() {
    Chip8 chip8 = initChip8("TETRIS");
    
    if (DEBUG) memdump(chip8);
    if (!DEBUG)
        createWindow();
    
    while (1) {
        execute(chip8, DEBUG);
        if (DEBUG) 
            prompt(chip8);
        //~ else
            //~ drawScreen(chip8);
        if(chip8->dt > 0)
            chip8->dt--;
        usleep(CLOCK);
    }
}

void prompt(Chip8 chip8) {
    int end = 0;
    
    char* s = calloc(2, sizeof(char));
    
    while (!end) {
        printf(">");
        scanf("%s", s);
        
        switch(s[0]) {
            case 'v':
                machinedump(chip8, keyTranslate(s[1]));
                break;
            case 'c':
                end = 1;
                break;
            default:
                break;
        }
        
        printf("\n");
    }
    free(s);
}

