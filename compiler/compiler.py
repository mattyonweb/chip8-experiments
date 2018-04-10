import opcodes as opc

with open("test.chip8", "r") as f:
    raw_lines = map(str.strip, f.readlines())

src_lines = list()
for l in raw_lines:
    if l != "":
        src_lines.append(l)

def return_sections(stripped_lines):
    try:
        idx_data = stripped_lines.index(".data:")
    except:
        idx_data = -1
        
    try:
        idx_code = stripped_lines.index(".code:")
    except:
        raise LookupError("Can't find .code tag")

    if idx_data == -1:
        data = stripped_lines[idx_data+1:idx_code]
    else:
        data = []

    code = stripped_lines[idx_code+1:]

    return data, code
    
data, code = return_sections(src_lines)
out = list()

def tokenize_line(line):
    in_tokens = line.replace(", ", " ").replace(",", " ").split()
    out_tokens = [ in_tokens[0] ]
    
    for t in in_tokens[1:]:
        if t in opc.registers:
            out_tokens.append(t.upper())
        elif "-" in t:
            out_tokens.append(t)
        else:
            if len(t) <= 2:          #numero o labels corta
                try:
                    out_tokens.append("{:02X}".format(int(t, 16))) #ATTENZIONE: questo significa che non ci
                                #possono essere label di 1/2/3 chars che
                                #possono corrispondere a un numero hex!!
                except ValueError:
                    out_tokens.append(t)
            else:
                out_tokens.append(t)
                 
    return out_tokens

def translate_tokens(opcode, args):
    if len(args) == 1:
        if args[0][0].upper() == "V":
            return [args[0][1].upper()]
        else:
            return args[0]
    else:
        f = lambda x: x[1].upper() if x[0].upper() == "V" else x
        return opcode.format(*map(f, args))   
        

def intermediate_translation(tokens):
    out_ = str()
    
    if ":" in tokens[0]:
        return tokens[0]
        
    if tokens[0] not in opc.opcodes:
        print(tokens)
        raise LookupError("NON SI TROVA QUESTA FACCIA DA CULO")
        
    translation = opc.opcodes[tokens[0]]
    
    if tokens[0] == "skip_eq" or tokens[0] == "skip_ne":
        if tokens[1] in opc.registers and not (tokens[2] in opc.registers):
            out_ = translation[0].format(tokens[1][1], tokens[2])
        elif tokens[1] in opc.registers and tokens[2] in opc.registers:
            out_ = translation[1].format(tokens[1][1], tokens[2][1])
        else:
            raise LookupError("SKIP_EQ - Unknown format: " + l)
            
    elif tokens[0] not in "li,add,mv,sub_curry".split(","):
        out_ = "".join(translate_tokens(translation, tokens[1:]))
    
    return out_
    
def translate_line(line):
    tokens = tokenize_line(line)
    return intermediate_translation(tokens)
# ~ out = list()
# ~ for l in code:
    # ~ tokens = tokenize_line(l)
    # ~ trans = intermediate_translation(tokens)
    # ~ out.append(trans) if trans != "" else None
    
# ~ print("\n".join(out))
            
########################################## TESTS #######################
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

a = "xor v0,v1"
assert(translate_line(a) == "8013")
a = "rand vF, 3"
assert(translate_line(a) == "CF03")
a = "key_eq VF, 9"
assert(translate_line(a) == "EF9E")
a = "clear"
assert(translate_line(a) == "00E0")
