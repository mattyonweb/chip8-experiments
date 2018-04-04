#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <ncurses.h>

#define CLOCK       8000
#define INPUTTIME   8000
#define DEBUG       1

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

int execute (Chip8 c, int debug);
void memdump(Chip8 c);
void monitordump(Chip8 c);
unsigned char keyTranslate(unsigned char c);
void draw(Chip8 chip8, unsigned char x, unsigned char y, unsigned char n);
void drawScreen(Chip8 chip8);

WINDOW * createWindow() {
    initscr(); //inizializza ncurses; necessario
    WINDOW * w = newwin(32, 64, 0, 0);
    timeout(INPUTTIME); //quanto tempo aspetta per un input prima di dare ERR
    noecho();   //non mostra l'input da tastiera
    nodelay(stdscr, TRUE);  //getch() diventa non-blocking dopo INPUTTIME secondi tipo
    cbreak();   //boh?!
    return w;
}

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

void memdump(Chip8 chip8) {
    FILE* fout = fopen("memdump", "w");
    
    for (int i=0; i<4096; i+=2)
        fprintf(fout, "%03X\t%04X\n", i, (chip8->memory[i] << 8) | chip8->memory[i+1]);
    fclose(fout);
}

void monitordump(Chip8 chip8) {
    FILE * fout = fopen("mondump", "w");
    for (int i=0; i<32*64; i++) {
        fprintf(fout, "%d", chip8->monitor[i]);
        if (i % 64 == 0)
            fprintf(fout, "\n");
    }
    fclose(fout);
}

int execute(Chip8 chip8, int debug) {
    unsigned char   updatePc = 1;
    unsigned short  instruction = (chip8->memory[chip8->pc]) << 8 |
                                   chip8->memory[chip8->pc+1];
    unsigned char   b1 = (chip8->memory[chip8->pc] & 0xF0) >> 4,
                    b2 = chip8->memory[chip8->pc] & 0xF,
                    b3 = (chip8->memory[chip8->pc+1] & 0xF0) >> 4,
                    b4 = chip8->memory[chip8->pc+1] & 0xF;
    unsigned char   lastTwo   = (b3 << 4) | b4;
    unsigned char   ch;

    if (debug) { printf("%04X\t%X%X%X%X\t", chip8->pc, b1,b2,b3,b4);}
    
    switch (b1) {
        case 0:
            if (instruction == 0x00E0) {
                if (debug) { printf("[CLEAR_DISPLAY]\n");}
                for (int i=0; i<64*32; i++)
                    chip8->monitor[i]=0;
            }
            
            else if (instruction == 0x00EE) { //00EE
                if (debug) { printf("[RET]\t$SP=%d\n", chip8->sp);}
                chip8->pc = chip8->stack[--(chip8->sp)];
                updatePc = 0;
            }
            
            break;
            
        case 1:
            if (debug) { printf("[GOTO] %03X\n", instruction & 0x0FFF);}
            chip8->pc = instruction & 0x0FFF;
            updatePc = 0;
            break;

        case 2:
            if (debug) { printf("[FUNCALL]\n");}
            chip8->stack[chip8->sp] = chip8->pc + 2;
            chip8->sp++;
            chip8->pc = instruction & 0x0FFF;
            updatePc = 0;
            break;

        case 3:
            if (debug) { printf("[SKIP_EQ]\tSKIP: ");}
            if (chip8->registers[b2] == lastTwo) {
                chip8->pc += 2; 
                if (debug) { printf("TRUE\t%02X==%02X\n", chip8->registers[b2], lastTwo);}
            }
            else
                if (debug) { printf("FALSE\t%02X!=%02X\n", chip8->registers[b2], lastTwo);}
            break;
            
        case 4:
            if (debug) { printf("[SKIP_NE]\tSKIP: ");}
            if (chip8->registers[b2] != (lastTwo)) {
                chip8->pc += 2; 
                if (debug) { printf("TRUE\t%02X!=%02X\n", chip8->registers[b2], lastTwo);}
            }
            else
                if (debug) { printf("FALSE\t%02X==%02X\n", chip8->registers[b2], lastTwo);}
            break;

        case 5:
            if (debug) { printf("[SKIP_RegE]\tSKIP: ");}
            if (chip8->registers[b2] == chip8->registers[b3]) {
                chip8->pc += 2; 
                if (debug) { printf("TRUE\t%02X==%02X\n", chip8->registers[b2], chip8->registers[b3]);}
            } 
            else
                if (debug) { printf("FALSE\t%02X!=%02X\n", chip8->registers[b2], chip8->registers[b3]);}
            break;
            
        case 9:
            if (debug) { printf("[SKIP_RegNE]\tSKIP: ");}
            if (chip8->registers[b2] != chip8->registers[b3]) {
                chip8->pc += 2; 
                if (debug) { printf("TRUE\t%02X!=%02X\n", chip8->registers[b2], chip8->registers[b3]);}
            } 
            else
                if (debug) { printf("FALSE\t%02X==%02X\n", chip8->registers[b2], chip8->registers[b3]);}
            break;

        case 6:
            if (debug) { printf("[LI]\t");}
            chip8->registers[b2] = lastTwo;
            if (debug) { printf("V%X = 0x%x\n", b2, lastTwo);}
            break;
            
        case 7:
            if (debug) { printf("[ADD_EQ]\t");}
            chip8->registers[b2] += lastTwo;
            if (debug) { printf("V%X += 0x%x\n", b2, lastTwo);}
            break;

        case 8:
            switch (b4) {
                case 0:
                    if (debug) { printf("[COPY]\tV%X <- V%X\n", b2, b3);}
                    chip8->registers[b2] = chip8->registers[b3];
                    break;

                case 1:
                    if (debug) { printf("[OR]\tV%X |= V%X\n", b2, b3);}
                    chip8->registers[b2] = chip8->registers[b2] | chip8->registers[b3];
                    break;

                case 2:
                    if (debug) { printf("[AND]\tV%X &= V%X\n", b2, b3);}
                    chip8->registers[b2] = chip8->registers[b2] & chip8->registers[b3];
                    break;

                case 3:
                    if (debug) { printf("[XOR]\tV%X ^= V%X\n", b2, b3);}
                    chip8->registers[b2] = chip8->registers[b2] ^ chip8->registers[b3];
                    break;

                case 4:
                    if (debug) { printf("[ADD_C]\tV%X += V%X (c)\n", b2, b3);}
                    if (chip8->registers[b2] + chip8->registers[b3] > 255)
                        chip8->registers[0xF] = 1;
                    else 
                        chip8->registers[0xF] = 0;
                    chip8->registers[b2] += chip8->registers[b3]; //corretto (tiene i 8 bit pù bassi)
                    break;
                    
                case 5:
                    if (debug) { printf("[SUB_C]\tV%X -= V%X\t", b2, b3);}
                    if (chip8->registers[b2] - chip8->registers[b3] < 0) {
                        chip8->registers[0xF] = 1;
                        printf("CARRY\n");
                    }
                    else { 
                        chip8->registers[0xF] = 0;
                        printf("NOCARRY\n");
                    }
                    chip8->registers[b2] -= chip8->registers[b3];
                    break;

                case 6:
                    if (debug) { printf("[SHIFTR]\tV%X, V%X\n", b2, b3);}
                    chip8->registers[0xF] = chip8->registers[b3] & 0x01; //GIUSTO?!
                    chip8->registers[b3] = chip8->registers[b3] >> 1;
                    chip8->registers[b2] = chip8->registers[b3];
                    break;

                case 7:
                    if (debug) { printf("[SUB_C]\tV%X = V%X - V%X (c)\n", b2, b3, b2);}
                    if (chip8->registers[b3] - chip8->registers[b2] < 0)
                        chip8->registers[0xF] = 1;
                    else 
                        chip8->registers[0xF] = 0;
                    chip8->registers[b2] = chip8->registers[b3] - chip8->registers[b2];
                    break;

                case 0xE:
                    if (debug) { printf("[SHIFTL]\tV%X, V%X\n", b2, b3);}
                    chip8->registers[0xF] = (chip8->registers[b3]&0b10000000)>>7; //GIUSTO?!
                    chip8->registers[b3] = chip8->registers[b3] << 1;
                    chip8->registers[b2] = chip8->registers[b3];
                    break;
                                        
                default:
                    if (debug) { printf("error\n");}
                    break;
            }
            break;

        case 0xA:
            if (debug) { printf("[SETI]\tI = %03x\n", (instruction & 0x0fff));}
            chip8->I = instruction & 0x0fff;
            break;

        case 0xB:
            if (debug) { printf("[JUMP]\tPC = %03x\n", instruction & 0x0fff);}
            chip8->pc = chip8->registers[0] + (instruction & 0x0fff);
            updatePc = 0;
            break;
        
        case 0xC:
            chip8->registers[b2] = (rand()%255) & (lastTwo);
            if (debug) { printf("[RAND]\tV%X = rand()\n", b2);}
            break;
        
        case 0xD:
            if (debug) { printf("[DRAW]\t(%d,%d)\n", chip8->registers[b2], chip8->registers[b3]); }
            draw(chip8, chip8->registers[b2], chip8->registers[b3], b4);
            break;
            
        case 0xE:
            if (b3 == 9 && b4 == 0xE) {   
                ch = getch();
                if (debug) {printf("[KEYEQ]\tV%x=%x\t%x\n", b2, chip8->registers[b2], ch);}
                if (keyTranslate(ch) == chip8->registers[b2])
                    chip8->pc += 2;
                break;
            }
            else if (b3 == 0xA && b4 == 1) {
                ch = getch();
                if (debug){printf("[KEYNE]\tV%x!=%x\t%x\n", b2, chip8->registers[b2], ch);}
                if (keyTranslate(ch) != chip8->registers[b2])
                    chip8->pc += 2;
                break;
            }
            break;
            
        case 0xF:
            switch( (b3<<4) | b4) {
                case 0x07:
                    if (debug) { printf("[Vx=DT]\n");}
                    chip8->registers[b2] = chip8->dt;
                    break;

                case 0x15:
                    if (debug) { printf("[DTIME]\n");}
                    chip8->dt = chip8->registers[b2];
                    break;

                case 0x18:
                    if (debug) { printf("[SREG]\n");}
                    chip8->st = chip8->registers[b2];
                    break;
                
                case 0x29:
                    printf("DIOCANE\n");            //TODO
                    break;
                     
                case 0x1E:
                    if (debug) { printf("[IADD]\n");}
                    chip8->I += chip8->registers[b2];
                    break;

                case 0x55:
                    if (debug) { printf("[MULTSTORE]\n");}
                    for (int i=0; i<=b2; i++) {
                        chip8->memory[chip8->I] = chip8->registers[i]; //I si aggiorna???
                        chip8->I++;
                    }
                    break;

                case 0x65:
                    if (debug) { printf("[MULTLOAD]\n");}
                    for (int i=0; i<=b2; i++) {
                        chip8->registers[i] = chip8->memory[chip8->I];
                        chip8->I++;
                    }
                    break;

                case 0x0A:
                    if (debug) { printf("[GETKEY]\n");}
                    chip8->registers[b2] = keyTranslate(getchar()); //qualcosa non mi torna
                    break;

                case 0x33:
                    if (debug) { printf("[BCD]\n");}
                    chip8->memory[chip8->I+0] = 100 * ((int)chip8->registers[b2] / 100);
                    chip8->memory[chip8->I+1] = 10  * (((int)chip8->registers[b2] - chip8->memory[chip8->I+0]) / 10);
                    chip8->memory[chip8->I+2] = chip8->registers[b2] - chip8->memory[chip8->I+0] - chip8->memory[chip8->I+1];
                    chip8->memory[chip8->I+1] = (unsigned char)(chip8->memory[chip8->I+1] / 10);
                    chip8->memory[chip8->I+0] = (unsigned char)(chip8->memory[chip8->I+0] / 100);
                    break;
                    
                default:
                    break;
            }
            break;

        default:
            if (debug) { printf("[UNKNOWN]\n");}
            break;
    }
    
    if (updatePc)
        chip8->pc += 2;
    else
        if (debug) { printf("\t\t(non updato)\n");}
    return 0;

}

unsigned char keyTranslate(unsigned char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return (c - 'a') + 10;
    if (c >= 'A' && c <= 'F')
        return (c - 'A') + 10;
    return 255;
}

void draw(Chip8 chip8, unsigned char x, unsigned char y, unsigned char n) {
    int hasFlipped = 0;
    
    for (int i=0; i<n; i++) {   //per ogni riga dello spirito
        int pos = x + 64 * (y + i);
        char bits = chip8->memory[chip8->I + i];    //riga nesima dello spirito
        if (DEBUG) printf("\t\t\t\t");
        
        for (int b=0; b<8; b++) {
            unsigned char newVal = bits >> (7 - b) & 0x01;
            if (chip8->monitor[pos] == 1 && newVal == 1) {
                chip8->registers[0xF] = 1;
                hasFlipped = 1;
            }
            chip8->monitor[pos] ^= newVal;
            if (DEBUG) printf("%d", newVal);
            pos++;
        }
        if (DEBUG) printf("hasFlipped = %d\n", hasFlipped);
        if (!hasFlipped) chip8->registers[0xF] = 0;
    }
}
        
void drawScreen(Chip8 chip8) {
    clear();
    for (int i=0; i<64*32; i++) 
        if (chip8->monitor[i] == 0)
            mvprintw(i / 64, i % 64, " ");
        else
            mvprintw(i / 64, i % 64, "x");
    refresh();
}
