def return_table(lines, spirits_table):
    table = spirits_table
    
    i = 512
    
    for l in lines:
        if ":" in l:
            table[l[:-1]] = "{:03X}".format(i)
            continue
        i += 2
    
    return table

def replacer(lines, table):
    text = "\n".join(lines)
    
    for lbl, idx in table.items():  
        text = text.replace(lbl + ":\n", "")
        text = text.replace(lbl, idx)
    
    return text.split("\n")

def linker(lines, spirits_table={}):
    """ Ritorna una lista """
    table = return_table(lines, spirits_table)
    return replacer(lines, table)
