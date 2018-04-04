li		VE, 05
li		VB, 06
li		I, V30C
add		PC, PC, V0
addi	PC, PC, V17A
skip_eq	VA, 40
skip_eq	VB, 12
li		VD, 1f
j		0dc
jal		2f6
li		V1, 00
j		2d0
add		V0, V0, 08
li		V0, 40
j		5f0
skip_eq	V0, 00
skip_eq	V4, c6
li		V7, 1e
li		I, V30E
add		V1, V1, a3
draw	VC, VD, 1
add		VC, VC, fe
add		VC, VC, 02
skip_eq	VF, 8c
draw	VC, VD, 1
add_curry	V6, V6, V8
skip_ne	V4, V6
and		V6, V0
j		f87
skip_ne	V7, 1f
li		I, VC46
li		V8, 01
skip_eq	VF, 68
skip_ne	V7, 00
skip_eq	VF, 01
li		I, VA47
j		2aa
skip_eq	VF, 00
li		I, VA60
mv		ST, V0
li		V0, 61
and		V0, V1
li		V0, fe
add		V5, V5, 01
j		2de
skip_ne	V6, 69
mv		V0, V6
rand	V5j		2ca
skip_eq	VF, 01
skip_eq	VF, 01
skip_eq	VF, 01
mv		ST, V0
mv		V0, VE
draw	V0, V1, 1
j		230
draw	VE, V7, 8
skip_ne	V8, fe
add		V8, V8, 01
li		V0, 04
j		869
j		270
j		4f5
multiload 0-2
jal		963
li		V4, 00
skip_ne	V5, 73
skip_ne	V5, 00
