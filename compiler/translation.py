import opcodes as opc

def tokenize_line(line):
    """ Tokeniza una riga di codice """
    in_tokens = line.replace(", ", " ").replace(",", " ").split()
    out_tokens = [ in_tokens[0] ]
    
    for t in in_tokens[1:]:
        if t in opc.registers:
            out_tokens.append(t.upper())
        elif "-" in t:
            out_tokens.append(t)
        else:
            if len(t) <= 3:          #numero o labels corta
                try:
                    out_tokens.append("{:02X}".format(int(t, 16))) #ATTENZIONE: questo significa che non ci
                                #possono essere label di 1/2/3 chars che
                                #possono corrispondere a un numero hex!!
                except ValueError:
                    out_tokens.append(t)
            else:
                out_tokens.append(t)
                 
    return out_tokens

def translate_registers(args):
    """ Un map() che traduce i registri nei rispettivi codici hex e 
    lascia intatto tutto il resto. """
    f = lambda x: x[1].upper() if x[0].upper() == "V" else x
    return map(f, args)
        

def intermediate_translation(tokens):
    """ Dati i tokens di una riga, ritorna una stringa che è la 
    rappresentazione intermedia del codice, aka: le label per i jump 
    rimangono intatte (in realtà tutte le label rimangono intatte) tutto
    il resto viene tradotto. """
    inst, args = tokens[0], tokens[1:]
    
    if ":" in inst:
        return inst
        
    if inst not in opc.opcodes:
        print(inst, tokens)
        raise LookupError("NON SI TROVA QUESTA FACCIA DA CULO")
        
    translation = opc.opcodes[inst] #eg. "4{}{}5", ("1{}", "8{}{}")
    
    if inst == "skip_eq" or inst == "skip_ne":
        if tokens[1] in opc.registers and not (tokens[2] in opc.registers):
            return translation[0].format(tokens[1][1], tokens[2])
        elif tokens[1] in opc.registers and tokens[2] in opc.registers:
            return translation[1].format(tokens[1][1], tokens[2][1])
        else:
            raise LookupError("SKIP_EQ - Unknown format: " + l)
            
    elif inst == "li":
        if args[0] == "I":
            try:
                address = "{:03X}".format(int(args[1], 16))
            except ValueError:
                address = args[1]
            return translation[1].format(address)
        else:
            return translation[0].format(*translate_registers(args))
    
    elif inst == "add":
        if args[0] == "I":
            return translation[1].format(*translate_registers(args[1:]))
        if args[1] in opc.registers:
            return translation[2].format(*translate_registers(args))
        return translation[0].format(*translate_registers(args))
    
    elif inst == "mv":
        if args[0].upper() == "DT":
            return translation[2].format(*translate_registers(args[1:]))
        if args[0].upper() == "ST":
            return translation[3].format(*translate_registers(args[1:]))
        if args[1].upper() == "DT":
            return translation[1].format(*translate_registers(args[:1]))
        return translation[0].format(*translate_registers(args))
    
    elif inst == "sub_curry":
        if args[0] == args[1]:
            return translation[0].format(*translate_registers(args[1:]))
        else:
            return translation[1].format(*translate_registers(args[:2]))
    
    elif inst == "multistore":
        return translation.format(args[0].split("-")[1].upper())
    elif inst == "multiload":
        return translation.format(args[0].split("-")[1].upper())
    
    elif inst == "draw":
        return translation.format(args[0][1].upper(), args[1][1].upper(), args[2][1].upper())
        
    else:
        return translation.format(*translate_registers(args))  
    
def translate_line(line):
    tokens = tokenize_line(line)
    return intermediate_translation(tokens)

########################################## TESTS #######################
### TEST1 - Vediamo se tokenizza bene 
a = "li v0, 5"
assert(tokenize_line(a) == ["li", "V0", "05"])
a = "j label2"
assert(tokenize_line(a) == ["j", "label2"])
a = "mv v0, Vf"
assert(tokenize_line(a) == ["mv", "V0", "VF"])
a = "multiload 0-5"
assert(tokenize_line(a) == ["multiload", "0-5"])
a = "start:"
assert(tokenize_line(a) == ["start:"])

### TEST2 - Che cossa fa translate_register(args)
a = ["li", "v0", "5"]
assert(list(translate_registers(a[1:])) == ["0", "5"])
a = ["add", "vf", "vc"]
assert(list(translate_registers(a[1:])) == ["F", "C"])

### TEST3 - Vediamo se traduce bene le traduzioni univoche
a = "xor v0,v1"
assert(translate_line(a) == "8013")
a = "rand vF, 3"
assert(translate_line(a) == "CF03")
a = "key_eq vf"
assert(translate_line(a) == "EF9E")
a = "clear"
assert(translate_line(a) == "00E0")
a = "bcd v1"
assert(translate_line(a) == "F133")
a = "draw v0, v1, 5"
assert(translate_line(a) == "D015")

### TEST4 - Ora quelli multi univochi
### LI
a = "li v0, ff"
assert(translate_line(a) == "60FF")
a = "li vF, 6"
assert(translate_line(a) == "6F06")
a = "li I, 2fe"
assert(translate_line(a) == "A2FE")
a = "li I, 0"
assert(translate_line(a) == "A000")

### ADD
a = "add v0, ef"
assert(translate_line(a) == "70EF")
a = "add v0, 0"
assert(translate_line(a) == "7000")
a = "add v5, vf"
assert(translate_line(a) == "85F4")
a = "add I, V4"
assert(translate_line(a) == "F41E")
a = "add I,vf"
assert(translate_line(a) == "FF1E")

### MV
a = "mv v0, vf"
assert(translate_line(a) == "80F0")
a = "mv v0, dt"
assert(translate_line(a) == "F007")
a = "mv dt, v0"
assert(translate_line(a) == "F015")
a = "mv st, v0"
assert(translate_line(a) == "F018")
a = "add I,vf"

## SUB_CURRY
a = "sub_curry v0, v0, vf"
assert(translate_line(a) == "80F5")
a = "sub_curry v0, vf, v0"
assert(translate_line(a) == "80F7")
a = "sub_curry v0, v5, vf"  #AATTENZIONE!! SEMANTICAMENTE SCORRETTO MA COMPILA!
assert(translate_line(a) == "8057")

### MULTI
a = "multistore 0-f"
assert(translate_line(a) == "FF55")
a = "multiload 0-f"
assert(translate_line(a) == "FF65")
