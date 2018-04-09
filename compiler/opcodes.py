opcodes = {
"clear"     : "00e0",
"ret"       : "00ee",
"j"         : "1{}",
"jal"       : "2{}",
"skip_eq"   : ("3{}{}", "5{}{}"),   #registro-imm | registro-registro
"skip_ne"   : ("4{}{}", "9{}{}"),   #registro-imm | registro-registro
"li"        : ("6{}{}", "a{}"),     #li normale   | li I, val
"add"       : ("7{}{}{}", "b{}",    #add normale  | add pc, pc, v0
               "f{}1e"),            #add I, val
"mv"        : ("8{}{}0", "f{}07",   #mv normale   | mv VX, dt
               "f{}15",  "f{}18",), #mv dt, VX    | mv st, VX
"or"        : "8{}{}1",
"and"       : "8{}{}2",
"xor"       : "8{}{}3",
"add_curry" : "8{}{}4",
"sub_curry" : ("8{}{}5", "8{}{}7"),
"sr"        : "8{}{}6",
"sl"        : "8{}{}e",
"joff"      : "b{}",
"rand"      : "c{}{}",
"draw"      : "d{}{}{}",
"key_eq"    : "e{}9e",
"key_ne"    : "e{}a1",
"load_charset": "f{}29",
"multistore": "f{}55",
"multiload" : "f{}65",
"waitkey"   : "f{}0a",
"bcd"       : "f{}33" }

registers = list()
for i in range(16):
    registers.append("V" + hex(i)[2:].upper())
    registers.append("V" + hex(i)[2:])
    registers.append("v" + hex(i)[2:].upper())
    registers.append("v" + hex(i)[2:])
