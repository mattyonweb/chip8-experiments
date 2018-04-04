#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <ncurses.h>
#include "emu.h"


int main() {
    Chip8 chip8 = malloc(sizeof(struct machine));
    
    FILE * source = fopen("SIERPINSKY", "rb");

    fseek(source, 0L, SEEK_END);
    int size = ftell(source);
    rewind(source);

    for (int i=0; i<size; i += 2) {
        unsigned char b1 = fgetc(source), b2 = fgetc(source);
        chip8->memory[512 + i]     = b1;
        chip8->memory[512 + i + 1] = b2;
    }
    chip8->pc = 512;
    chip8->sp = 0;
    for (int i=0; i<16; i++)
        chip8->registers[i] = 0;
    
    memdump(chip8);
    if (!DEBUG)
        createWindow();
    
    while (1) {
    //~ for (int i=0; i<16; i++) {
        timeout(INPUTTIME);
        execute(chip8, DEBUG);
        if(chip8->dt > 0) chip8->dt--;
        usleep(CLOCK);
        drawScreen(chip8);
    }
    monitordump(chip8);
}
