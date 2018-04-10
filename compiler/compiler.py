import opcodes as opc
import translation as trans
import linker as lk


def read_source(filename):
    with open(filename, "r") as f:
        raw_lines = map(str.strip, f.readlines())

    src_lines = list(filter(lambda l: l!="", raw_lines))
    return src_lines
    
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
        data = []
    else:
        data = stripped_lines[idx_data+1:idx_code]

    code = stripped_lines[idx_code+1:]

    return data, code
   
src_lines = read_source("maze.chip8")
data, code = return_sections(src_lines)

out = list()
for l in code:
    translated = trans.translate_line(l)
    out.append(translated)

first_idx_free = 512 + 2 * len(list(filter(lambda l: ":" not in l, out)))
print(out)
print(len(out))
print("{:03X}".format(first_idx_free))

spiriti_table = {}
spiriti_value = []
for s in data:
    name, values = s.replace(": ", " : ").replace(" ", "").split(":")
    bytes_str    = values.replace(", ", ",").split(",")
    bytes_       = list(map(lambda b: "{:02X}".format(int(b,16)), bytes_str))
    
    spiriti_table[name] = "{:03X}".format(int(str(first_idx_free)))
    first_idx_free     += len(bytes_)
    spiriti_value      += bytes_ 
    

code_translated = lk.linker(out, spiriti_table)

bytes_ = []
for i in range(0, len(code_translated)):
    bytes_.append(code_translated[i][:2])
    bytes_.append(code_translated[i][2:4])
print(spiriti_value)
print(bytes_ + spiriti_value)

ints = map(lambda b: int(b, 16), bytes_ + spiriti_value)
output = bytearray(ints)
with open("../roms/MYMAZE", "wb") as f:
    f.write(output)
# ~ print("\n".join(out))
            
