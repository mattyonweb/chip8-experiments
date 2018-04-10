import opcodes as opc
import translation as trans
import linker as lk


def read_source(filename):
    with open(filename, "r") as f:
        raw_lines = map(str.strip, f.readlines())

    src_lines = list(filter(lambda l: l!="", raw_lines))
    return src_lines
    
def return_sections(stripped_lines):
    """ Ritorna due liste in cui sono presenti, rispettivamente, le 
    linee sui dati e le linee di codice. """
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

def return_spirit_table(lines):
    """ Ritorna dizionario nomeSpirito : str(idx) (già base 16) """
    spiriti_table  = {}
    spiriti_values = []
    
    first_idx_free = 512 + 2 * len(list(filter(lambda l: ":" not in l, out)))
    
    for spirit in data:
        name, values = spirit.replace(": ", " : ").replace(" ", "").split(":")
        bytes_str    = values.replace(", ", ",").split(",")
        bytes_       = list(map(lambda b: "{:02X}".format(int(b,16)), bytes_str))
        
        spiriti_table[name] = "{:03X}".format(int(str(first_idx_free)))
        first_idx_free     += len(bytes_)
        spiriti_values     += bytes_
    
    spiriti_values = list(map(lambda b: int(b, 16), spiriti_values))
    return (spiriti_table, spiriti_values)
    

spirit_table, spirit_values = return_spirit_table(out)
code_translated = lk.linker(out, spirit_table)

def chunk_code(two_bytes_codes):
    bytes_ = []
    for i in range(0, len(code_translated)):
        bytes_.append(two_bytes_codes[i][:2])
        bytes_.append(two_bytes_codes[i][2:4])
    return list(map(lambda b: int(b, 16), bytes_))
    
bytecode = chunk_code(code_translated)

output = bytearray(bytecode + spirit_values)
with open("../roms/MYMAZE", "wb") as f:
    f.write(output)
            
