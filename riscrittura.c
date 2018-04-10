int execute(Chip8 chip8, int debug, FILE* asmf) {
    unsigned char   updatePc = 1;
    unsigned char   needToDraw = 0;
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
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "clear\n"); break; }
                for (int i=0; i<64*32; i++)
                    chip8->monitor[i]=0;
            }
            
            else if (instruction == 0x00EE) { //00EE
                if (debug) { printf("[RET]\t$SP=%d\n", chip8->sp);}
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ret\n"); break; }
                chip8->pc = chip8->stack[--(chip8->sp)];
                updatePc = 0;
            }
            
            else {
                if (debug) { printf("[ATTENZIONE! ISTRUZIONE NON ESISTENTE]\n"); }
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ISTRUZIONE SCONOSCIUTA %04X\n", instruction); break; }
            }
            break;

        case 0xA:
            if (debug) { printf("[SETI]\tI = %03x\n", (instruction & 0x0fff));}
            if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "li\t\tI, %x\n", instruction & 0x0fff); break; }
            chip8->I = instruction & 0x0fff;
            break;

        case 0xB:
            if (debug) { printf("[JUMP]\tPC = %03x\n", instruction & 0x0fff);}
            //~ if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "add\t\tPC, PC, V0\naddi\tPC, PC, %x\n", instruction & 0x0fff); break; }
            if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "joff\t%x\n", instruction & 0x0fff); break; }
            chip8->pc = chip8->registers[0] + (instruction & 0x0fff);
            updatePc = 0;
            break;
        
        case 0xC:
            chip8->registers[b2] = (rand()%255) & (lastTwo);
            if (debug) { printf("[RAND]\tV%X = rand()\n", b2);}
            if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "rand\tV%X\n", b2); break; }
            break;
        
        case 0xD:
            if (debug) { printf("[DRAW]\t(%d,%d)\n", chip8->registers[b2], chip8->registers[b3]); }
            if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "draw\tV%X, V%X, %d\n", b2, b3, b4); break; }
            draw(chip8, chip8->registers[b2], chip8->registers[b3], b4);
            needToDraw = 1;
            break;
            
        case 0xE:
            if (b3 == 9 && b4 == 0xE) {   
                ch = getch();
                if (debug) {printf("[KEYEQ]\tV%x=%x\t%x\n", b2, chip8->registers[b2], ch);}
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "key_eq\tV%X\n", b2); break; }
                if (keyTranslate(ch) == chip8->registers[b2])
                    chip8->pc += 2;
                break;
            }
            else if (b3 == 0xA && b4 == 1) {
                ch = getch();
                if (debug){printf("[KEYNE]\tV%x!=%x\t%x\n", b2, chip8->registers[b2], ch);}
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "keyne\tV%X\n", b2); break; }
                if (keyTranslate(ch) != chip8->registers[b2])
                    chip8->pc += 2;
                break;
            }
            else {
                if (debug) { printf("[ATTENZIONE! ISTRUZIONE NON ESISTENTE]\n"); }
                if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ISTRUZIONE SCONOSCIUTA %04X\n", instruction); break; }
                break;
            }
            break;
            
        case 0xF:
            switch( (b3<<4) | b4) {
                case 0x07:
                    if (debug) { printf("[Vx=DT]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "mv\t\tV%X, DT\n", b2); break; }
                    chip8->registers[b2] = chip8->dt;
                    break;

                case 0x15:
                    if (debug) { printf("[DTIME]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "mv\t\tDT, V%X\n", b2); break; }
                    chip8->dt = chip8->registers[b2];
                    break;

                case 0x18:
                    if (debug) { printf("[SREG]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "mv\t\tST, V%X\n", b2); break; }
                    chip8->st = chip8->registers[b2];
                    break;
                
                case 0x29:
                    if (debug) { printf("[LOADCHAR]\n"); }
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "load_charset V%02X\n", b2); break; }
                    chip8->I = chip8->registers[b2]*5;
                    break;
                     
                case 0x1E:
                    if (debug) { printf("[IADD]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "add\t\tI, I, V%X\n", b2); break; }
                    chip8->I += chip8->registers[b2];
                    break;

                case 0x55:
                    if (debug) { printf("[MULTSTORE]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "multistore 0-%X\n", b2); break; }
                    for (int i=0; i<=b2; i++) {
                        chip8->memory[chip8->I] = chip8->registers[i]; //I si aggiorna???
                        chip8->I++;
                    }
                    break;

                case 0x65:
                    if (debug) { printf("[MULTLOAD]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "multiload 0-%X\n", b2); break; }
                    for (int i=0; i<=b2; i++) {
                        chip8->registers[i] = chip8->memory[chip8->I];
                        chip8->I++;
                    }
                    break;

                case 0x0A:
                    if (debug) { printf("[GETKEY]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "waitkey\n", b2); break; }
                    chip8->registers[b2] = keyTranslate(getchar()); //qualcosa non mi torna
                    break;

                case 0x33:
                    if (debug) { printf("[BCD]\n");}
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "bcd\t\tV%X\n", b2); break; }
                    chip8->memory[chip8->I+0] = 100 * ((int)chip8->registers[b2] / 100);
                    chip8->memory[chip8->I+1] = 10  * (((int)chip8->registers[b2] - chip8->memory[chip8->I+0]) / 10);
                    chip8->memory[chip8->I+2] = chip8->registers[b2] - chip8->memory[chip8->I+0] - chip8->memory[chip8->I+1];
                    chip8->memory[chip8->I+1] = (unsigned char)(chip8->memory[chip8->I+1] / 10);
                    chip8->memory[chip8->I+0] = (unsigned char)(chip8->memory[chip8->I+0] / 100);
                    break;
                    
                default:
                    if (debug) { printf("[ATTENZIONE! ISTRUZIONE NON ESISTENTE]\n"); }
                    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ISTRUZIONE SCONOSCIUTA %04X\n", instruction); break; }
                    break;
            }
            break;

        default:
            if (debug) { printf("[ATTENZIONE! ISTRUZIONE NON ESISTENTE]\n"); }
            if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ISTRUZIONE SCONOSCIUTA %04X\n", instruction); break; }
            break;
    }
    
    if (updatePc)
        chip8->pc += 2;
    else
        if (debug) { printf("\t\t(non updato)\n");}
    return 0;

}

////////////////////////////////////////////////////////////////////////
void jump(short instruction) {
    if (debug) { printf("[GOTO] %03X\n", instruction & 0x0FFF);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "j\t\t%03x\n", instruction & 0x0FFF); return; }
    chip8->pc = instruction & 0x0FFF;
    updatePc = 0;
    return;
}

void jal(short instruction) {
    if (debug) { printf("[FUNCALL]\n");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "jal\t\t%03x\n", instruction & 0x0FFF); return; }
    chip8->stack[chip8->sp] = chip8->pc + 2;
    chip8->sp++;
    chip8->pc = instruction & 0x0FFF;
    updatePc = 0;
    return;
}

void skipEqImm(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char lastTwo = chip8->memory[chip8->pc+1];
    
    if (debug) { printf("[SKIP_EQ]\tSKIP: ");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "skip_eq\tV%X, %02x\n", b2, lastTwo); return; }
    if (chip8->registers[b2] == lastTwo) {
        chip8->pc += 2; 
        if (debug) { printf("TRUE\t%02X==%02X\n", chip8->registers[b2], lastTwo);}
    }
    else
        if (debug) { printf("FALSE\t%02X!=%02X\n", chip8->registers[b2], lastTwo);}
    return;
}

void skipNeImm(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char lastTwo = chip8->memory[chip8->pc+1];
    
    if (debug) { printf("[SKIP_NE]\tSKIP: ");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "skip_ne\tV%X, %02x\n", b2, lastTwo); return; }
    if (chip8->registers[b2] != (lastTwo)) {
        chip8->pc += 2; 
        if (debug) { printf("TRUE\t%02X!=%02X\n", chip8->registers[b2], lastTwo);}
    }
    else
        if (debug) { printf("FALSE\t%02X==%02X\n", chip8->registers[b2], lastTwo);}
    return;
}

void skipEqReg(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char b3 = chip8->memory[chip8->pc+1] & 0xF0;
    if (debug) { printf("[SKIP_RegE]\tSKIP: ");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "skip_eq\tV%X, V%X\n", b2, b3); return; }
    if (chip8->registers[b2] == chip8->registers[b3]) {
        chip8->pc += 2; 
        if (debug) { printf("TRUE\t%02X==%02X\n", chip8->registers[b2], chip8->registers[b3]);}
    } 
    else
        if (debug) { printf("FALSE\t%02X!=%02X\n", chip8->registers[b2], chip8->registers[b3]);}
    return;
}

void skipNeReg(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char b3 = chip8->memory[chip8->pc+1] & 0xF0;
    if (debug) { printf("[SKIP_RegNE]\tSKIP: ");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "skip_ne\tV%X, V%X\n", b2, b3); return; }
    if (chip8->registers[b2] != chip8->registers[b3]) {
        chip8->pc += 2; 
        if (debug) { printf("TRUE\t%02X!=%02X\n", chip8->registers[b2], chip8->registers[b3]);}
    } 
    else
        if (debug) { printf("FALSE\t%02X==%02X\n", chip8->registers[b2], chip8->registers[b3]);}
    return;
}

void li(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char lastTwo = chip8->memory[chip8->pc+1];
    
    if (debug) { printf("[LI]\t");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "li\t\tV%X, %02x\n", b2, lastTwo); return; }
    chip8->registers[b2] = lastTwo;
    if (debug) { printf("V%X = 0x%x\n", b2, lastTwo);}
    return;
}

void addi(short instruction) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF;
    unsigned char lastTwo = chip8->memory[chip8->pc+1];
    
    if (debug) { printf("[ADD_EQ]\t");}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "add\t\tV%X, %02x\n", b2, b2, lastTwo); return; }
    chip8->registers[b2] += lastTwo;
    if (debug) { printf("V%X += 0x%x\n", b2, lastTwo);}
    return;
}
////////////////////////////////////////////////////////////////////////

void error() {
    if (debug) { printf("[ATTENZIONE! ISTRUZIONE NON ESISTENTE]\n"); }
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "ISTRUZIONE SCONOSCIUTA %04X\n", instruction); break; }
    break;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//////////////////// BLOCCO ARITMETICO /////////////////////////////////
////////////////////////////////////////////////////////////////////////

void cp(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[COPY]\tV%X <- V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "mv\t\tV%X, V%X\n", b2, b3); return; }
    chip8->registers[b2] = chip8->registers[b3];
}

void or(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[OR]\tV%X |= V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "or\t\tV%X, V%X\n", b2, b3); return; }
    chip8->registers[b2] = chip8->registers[b2] | chip8->registers[b3];
}

void and(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[AND]\tV%X &= V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "and\t\tV%X, V%X\n", b2, b3); }
    chip8->registers[b2] = chip8->registers[b2] & chip8->registers[b3];
}

void xor(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[XOR]\tV%X ^= V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "xor\t\tV%X, V%X\n", b2, b3); return; }
    chip8->registers[b2] = chip8->registers[b2] ^ chip8->registers[b3];
    return;
}

void addC(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[ADD_C]\tV%X += V%X (c)\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "add\t\tV%X, V%X\n", b2, b2, b3); return; }
    if (chip8->registers[b2] + chip8->registers[b3] > 255)
        chip8->registers[0xF] = 1;
    else 
        chip8->registers[0xF] = 0;
    chip8->registers[b2] += chip8->registers[b3]; //corretto (tiene i 8 bit pù bassi)
    return;
}

void subC(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[SUB_C]\tV%X -= V%X\t", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "sub_curry\tV%X, V%X, V%X\n", b2, b2, b3); return; }
    if (chip8->registers[b2] - chip8->registers[b3] > 0) {
        chip8->registers[0xF] = 1;
        if (debug) printf("CARRY\n");
    }
    else { 
        chip8->registers[0xF] = 0;
        if (debug) printf("NOCARRY\n");
    }
    chip8->registers[b2] -= chip8->registers[b3];
    return;
}

void sr(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[SHIFTR]\tV%X, V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "sr\t\tV%X\n", b2); return; }
    chip8->registers[0xF] = chip8->registers[b3] & 0x01; //GIUSTO?!
    chip8->registers[b3] = chip8->registers[b3] >> 1;
    chip8->registers[b2] = chip8->registers[b3];
    return;
}

void subCIn(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[SUB_C]\tV%X = V%X - V%X (c)\n", b2, b3, b2);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "sub_curry\tV%X, V%X, V%X\n", b2, b3, b2); return; }
    if (chip8->registers[b3] - chip8->registers[b2] > 0)
        chip8->registers[0xF] = 1;
    else 
        chip8->registers[0xF] = 0;
    chip8->registers[b2] = chip8->registers[b3] - chip8->registers[b2];
    return;
}

void sl(unsigned char b1, unsigned char b2) {
    if (debug) { printf("[SHIFTL]\tV%X, V%X\n", b2, b3);}
    if (asmf) { fprintf(asmf, "%04x	", chip8->pc);  fprintf(asmf, "sl\t\tV%X\n", b2); return; }
    chip8->registers[0xF] = (chip8->registers[b3]&0b10000000)>>7; //GIUSTO?!
    chip8->registers[b3] = chip8->registers[b3] << 1;
    chip8->registers[b2] = chip8->registers[b3];
    return;
}

void (*ArithmeticTable[8])() = {
    cp,     or,     and,    xor
    addC,   subC,   sr,     subCInv,
    error,  error,  error,  error,
    error,  error,  sl,     error   };
    
void cpuARITM(short opcode) {
    unsigned char b2 = chip8->memory[chip8->pc] & 0xF,
    unsigned char b3 = (chip8->memory[chip8->pc+1] & 0xF0) >> 4,
    ArithmeticTable[opcode & F](b2, b3);
}
////////////////////////////////////////////////////////////////////////
///////////////// FINE BLOCCO ARITMETICO ///////////////////////////////
////////////////////////////////////////////////////////////////////////


void (*MainTable[16])() = {
    cpuZERO,    jump,       jal,    skipEqImm,
    skipNeImm,  skipEqReg,  li,     addi,
    cpuARITM,   skipNeReg,  liI,    joff,
    random,     draw,       cpuKEY, cpuMISCELL };
