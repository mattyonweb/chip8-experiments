lines = open("emu.c", "r").readlines()

with open("emu.out", "w") as f:
    for line in lines:
        if "if (asmf)" in line:
            line = line.replace("{", "{ fprintf(asmf, \"%04x\t\", chip8->pc); ")
        f.write(line)
