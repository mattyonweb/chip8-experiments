opcodes = {
"clear"     : "00E0",
"ret"       : "00EE",
"j"         : "1{}",
"jal"       : "2{}",
"skip_eq"   : ("3{}{}", "5{}{}0"),   #registro-imm | registro-registro
"skip_ne"   : ("4{}{}", "9{}{}0"),   #registro-imm | registro-registro
"li"        : ("6{}{}", "A{}"),     #li normale   | li I, val
"add"       : ("7{}{}{}", "B{}",    #add normale  | add pc, pc, v0
               "f{}1E"),            #add I, val
"mv"        : ("8{}{}0", "F{}07",   #mv normale   | mv VX, dt
               "F{}15",  "F{}18",), #mv dt, VX    | mv st, VX
"or"        : "8{}{}1",
"and"       : "8{}{}2",
"xor"       : "8{}{}3",
"add_curry" : "8{}{}4",
"sub_curry" : ("8{}{}5", "8{}{}7"),
"sr"        : "8{}{}6",
"sl"        : "8{}{}E",
"joff"      : "B{}",
"rand"      : "C{}{}",
"draw"      : "D{}{}{}",
"key_eq"    : "E{}9E",
"key_ne"    : "E{}A1",
"load_charset": "F{}29",
"multistore": "F{}55",
"multiload" : "F{}65",
"waitkey"   : "F{}0A",
"bcd"       : "F{}33" }

registers = list()
for i in range(16):
    registers.append("V" + hex(i)[2:].upper())
    registers.append("V" + hex(i)[2:])
    registers.append("v" + hex(i)[2:].upper())
    registers.append("v" + hex(i)[2:])
