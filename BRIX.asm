0200	li		VE, 05
0202	li		V5, 00
0204	li		VB, 06      ;spiazzamento sulle y
0206	li		VA, 00      ;spiazzamento sulle x
0208	li		I, 30c      ;indirizzo spirito tavoletta da rompere?
020a	draw	VA, VB, 1   ;disegna tavoletta
020c	add		VA, VA, 04  ;sposta di 4 sulle x
020e	skip_eq	VA, 40      ;se non ho ancora disegnato 10 tavolette
0210	j		208         ;disegnane un'altra
0212	add		VB, VB, 02  ;altrimenti: spostati in giù sulle y di 2
0214	skip_eq	VB, 12      ;se non ho ancora disegnato 6 file di tavolette
0216	j		206         ;disegnane un'altra
0218	li		VC, 20      ;altrimenti: VC = y della racchetta (32)?
021a	li		VD, 1f
021c	li		I, 310
021e	draw	VC, VD, 1
0220	jal		2f6
0222	li		V0, 00
0224	li		V1, 00
0226	li		I, 312
0228	draw	V0, V1, 1
022a	add		V0, V0, 08
022c	li		I, 30e
022e	draw	V0, V1, 1
0230	li		V0, 40
0232	mv		DT, V0
0234	mv		V0, DT
0236	skip_eq	V0, 00
0238	j		234
023a	rand	V6
023c	li		V7, 1e
023e	li		V8, 01
0240	li		V9, ff
0242	li		I, 30e
0244	draw	V6, V7, 1
0246	li		I, 310
0248	draw	VC, VD, 1
024a	li		V0, 04
024c	keyne	V0
024e	add		VC, VC, fe
0250	li		V0, 06
0252	keyne	V0
0254	add		VC, VC, 02
0256	li		V0, 3f
0258	and		VC, V0
025a	draw	VC, VD, 1
025c	li		I, 30e
025e	draw	V6, V7, 1
0260	add_curry	V6, V6, V8
0262	add_curry	V7, V7, V9
0264	li		V0, 3f
0266	and		V6, V0
0268	li		V1, 1f
026a	and		V7, V1
026c	skip_ne	V7, 1f
026e	j		2ac
0270	skip_ne	V6, 00
0272	li		V8, 01
0274	skip_ne	V6, 3f
0276	li		V8, ff
0278	skip_ne	V7, 00
027a	li		V9, 01
027c	draw	V6, V7, 1
027e	skip_eq	VF, 01
0280	j		2aa
0282	skip_ne	V7, 1f
0284	j		2aa
0286	li		V0, 05
0288	sub_curry	V0, V0, V7
028a	skip_eq	VF, 00
028c	j		2aa
028e	li		V0, 01
0290	mv		ST, V0
0292	mv		V0, V6
0294	li		V1, fc
0296	and		V0, V1
0298	li		I, 30c
029a	draw	V0, V7, 1
029c	li		V0, fe
029e	xor		V9, V0
02a0	jal		2f6
02a2	add		V5, V5, 01
02a4	jal		2f6
02a6	skip_ne	V5, 60
02a8	j		2de
02aa	j		246
02ac	li		V9, ff
02ae	mv		V0, V6
02b0	sub_curry	V0, V0, VC
02b2	skip_eq	VF, 01
02b4	j		2ca
02b6	li		V1, 02
02b8	sub_curry	V0, V0, V1
02ba	skip_eq	VF, 01
02bc	j		2e0
02be	sub_curry	V0, V0, V1
02c0	skip_eq	VF, 01
02c2	j		2ee
02c4	sub_curry	V0, V0, V1
02c6	skip_eq	VF, 01
02c8	j		2e8
02ca	li		V0, 20
02cc	mv		ST, V0
02ce	li		I, 30e
02d0	add		VE, VE, ff
02d2	mv		V0, VE
02d4	add_curry	V0, V0, V0
02d6	li		V1, 00
02d8	draw	V0, V1, 1
02da	skip_eq	VE, 00
02dc	j		230
02de	j		2de
02e0	add		V8, V8, ff
02e2	skip_ne	V8, fe
02e4	li		V8, ff
02e6	j		2ee
02e8	add		V8, V8, 01
02ea	skip_ne	V8, 02
02ec	li		V8, 01
02ee	li		V0, 04
02f0	mv		ST, V0
02f2	li		V9, ff
02f4	j		270
02f6	li		I, 314
02f8	bcd		V5
02fa	multiload 0-2
02fc	non implementata
02fe	li		V3, 37
0300	li		V4, 00
0302	draw	V3, V4, 5
0304	add		V3, V3, 05
0306	non implementata
0308	draw	V3, V4, 5
030a	ret
030c	ISTRUZIONE SCONOSCIUTA E000
030e	mv		V0, V0
0310	ISTRUZIONE SCONOSCIUTA FC00
0312	li		I, a00
