import opcodes as opc
import translation as trans
import linker as lk

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
for l in code:
    translated = trans.translate_line(l)
    out.append(translated) if translated != "" else None
    
print(lk.linker(out))
# ~ print("\n".join(out))
            
