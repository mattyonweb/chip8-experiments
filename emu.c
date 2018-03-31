#include <stdio.h>
#include <stdlib.h>

typedef struct machine {
    unsigned char memory[4096];
    unsigned char registers[16];
    unsigned short I;
    unsigned short pc;
    unsigned char monitor[64 * 32];

    unsigned short stack[16];
    unsigned char sp;
} * Chip8;

int execute (Chip8 c);
void memdump(Chip8 c);

int main() {
    Chip8 chip8 = malloc(sizeof(struct machine));
    
    FILE * source = fopen("TETRIS", "rb");

    fseek(source, 0L, SEEK_END);
    int size = ftell(source);
    rewind(source);

    for (int i=0; i<size; i += 2) {
        unsigned char b1 = fgetc(source), b2 = fgetc(source);
        //unsigned short instruction = b1 << 8 | b2;
        //printf("%04X\n", instruction);
        chip8->memory[512 + i]     = b1;
        chip8->memory[512 + i + 1] = b2;
    }
    chip8->pc = 512;
    chip8->sp = 0;
    for (int i=0; i<16; i++)
        chip8->registers[i] = 0;
    
    memdump(chip8);
    for (int x=0; x<200; x++)
       execute(chip8);
}

void memdump(Chip8 chip8) {
    FILE* fout = fopen("memdump", "w");
    
    for (int i=0; i<4096; i+=2)
        fprintf(fout, "%03X\t%04X\n", i, (chip8->memory[i] << 8) | chip8->memory[i+1]);
    fclose(fout);
}

int execute(Chip8 chip8) {
    unsigned char   updatePc = 1;
    unsigned short  instruction = (chip8->memory[chip8->pc]) << 8 |
                                   chip8->memory[chip8->pc+1];
    unsigned char   b1 = (chip8->memory[chip8->pc] & 0xF0) >> 4,
                    b2 = chip8->memory[chip8->pc] & 0xF,
                    b3 = (chip8->memory[chip8->pc+1] & 0xF0) >> 4,
                    b4 = chip8->memory[chip8->pc+1] & 0xF;
                    
    printf("0x%03X\t%01X%01X%01X%01X\t", chip8->pc,b1,b2,b3,b4);

    switch ((instruction & 0xF000) >> 12) {
        case 0:
            if (instruction == 0x00E0) {
                printf("[CLEAR_DISPLAY]\n");
                for (int i=0; i<64*32; i++)
                    chip8->monitor[i]=0;
            }
            
            else if (instruction == 0x00EE) { //00EE
                printf("[RET]\t$SP=%d\n", chip8->sp);
                chip8->pc = chip8->stack[--(chip8->sp)];
                updatePc = 0;
            }

            else
                printf("[UNKNOWN]\n");

            break;
            
        case 1:
            printf("[GOTO] %03X\n", instruction & 0x0FFF);
            chip8->pc = instruction & 0x0FFF;
            updatePc = 0;
            break;

        case 2:
            printf("[FUNCALL]\n");
            chip8->stack[chip8->sp] = chip8->pc + 2;
            chip8->sp++;
            chip8->pc = instruction & 0x0FFF;
            updatePc = 0;
            break;

        case 3:
            printf("[SKIP_EQ]\tSKIP: ");
            if (chip8->registers[ (instruction&0x0f00)>>8 ] == (instruction&0x00ff)) {
                chip8->pc += 2;
                printf("TRUE\t%02X==%02X\n", chip8->registers[ (instruction&0x0f00)>>8 ], instruction&0x00ff);
            }
            else
                printf("FALSE\t%02X!=%02X\n", chip8->registers[ (instruction&0x0f00)>>8 ], instruction&0x00ff);
            break;
            
        case 4:
            printf("[SKIP_NE]\tSKIP: ");
            if (chip8->registers[ (instruction&0x0f00)>>8 ] != (instruction&0x00ff)) {
                chip8->pc += 2;
                printf("TRUE\t%02X!=%02X\n", chip8->registers[ (instruction&0x0f00)>>8 ], instruction&0x00ff);
            }
            else
                printf("FALSE\t%02X==%02X\n", chip8->registers[ (instruction&0x0f00)>>8 ], instruction&0x00ff);
            break;

        case 5:
            printf("[SKIP_RegE]\tSKIP: ");
            if (chip8->registers[ (instruction&0x0f00)>>8 ] == chip8->registers[(instruction&0x00f0)>>4]) {
                chip8->pc += 2;
                printf("TRUE\tV%X==V%X (%x==%x)\n", chip8->registers[ (instruction&0x0f00)>>8 ], chip8->registers[(instruction&0x00f0)>>4]);
            }
            else
                printf("FALSE\t%02X!=%02X\n", chip8->registers[ (instruction&0x0f00)>>8 ], chip8->registers[(instruction&0x00f0)>>4]);
            break;

        case 6:
            printf("[MOVE]\t");
            chip8->registers[ (instruction&0x0f00)>>8 ] = instruction&0x00ff;
            printf("V%X = 0x%x\n", (instruction&0x0f00)>>8, instruction&0x00ff);
            break;
            
        case 7:
            printf("[ADD_EQ]\t");
            chip8->registers[ (instruction&0x0f00)>>8 ] += instruction&0x00ff;
            printf("V%X += 0x%x\n", (instruction&0x0f00)>>8, instruction&0x00ff);
            break;

        case 0xC:
            printf("[RAND]\t");
            chip8->registers[(instruction&0x0f00)>>8] = (rand()%255) & (instruction&0xFF);
            printf("V%X = rand()\n", (instruction&0x0f00)>>8);
            break;
            
        default:
            printf("[NOP]\n");
            break;
    }
    
    if (updatePc)
        chip8->pc += 2;
    else
        printf("\t\t(non updato)\n");
    return 0;

}
