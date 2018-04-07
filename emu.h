#ifndef EMU_H
#define EMU_H

#define CLOCK       5000
#define INPUTTIME   25
#define DEBUG       0

typedef struct machine {
    unsigned char memory[4096];
    unsigned char registers[16];
    unsigned char monitor[64 * 32];
    
    unsigned short I;
    unsigned short pc;
    unsigned char  dt;
    unsigned char  st;

    unsigned short stack[16];
    unsigned char sp;    
} * Chip8;

WINDOW * createWindow();
Chip8 initChip8(char* filename);
int execute (Chip8 c, int debug, FILE* asmout);
void memdump(Chip8 c);
void monitordump(Chip8 c);
void disassemble(Chip8 chip8);
unsigned char keyTranslate(unsigned char c);
void draw(Chip8 chip8, unsigned char x, unsigned char y, unsigned char n);
void drawScreen(Chip8 chip8);
void machinedump(Chip8 chip8, unsigned char regNum);
#endif
