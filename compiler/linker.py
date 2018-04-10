def return_table(lines):
    table = {}
    i = 200
    
    for l in lines:
        if ":" in l:
            table[l[:-1]] = i
            continue
        i += 2
    
    return table

def replacer(lines, table):
    text = "\n".join(lines)
    
    for lbl, idx in table.items():
        text = text.replace(lbl + ":\n", "")
        text = text.replace(lbl, str(idx))
    
    return text.split("\n")

def linker(lines):
    table = return_table(lines)
    return replacer(lines, table)
