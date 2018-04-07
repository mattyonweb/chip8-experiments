0200	li		I, 2b4
0202	jal		3e6
0204	jal		2b6
0206	add		V0, V0, 01
0208	draw	V0, V1, 1
020a	skip_eq	V0, 25
020c	j		206
020e	add		V1, V1, ff
0210	draw	V0, V1, 1
0212	li		V0, 1a
0214	draw	V0, V1, 1
0216	li		V0, 25
0218	skip_eq	V1, 00
021a	j		20e
021c	rand	V4
021e	skip_ne	V4, 70
0220	j		21c
0222	rand	V3
0224	li		V0, 1e
0226	li		V1, 03
0228	jal		25c
022a	mv		DT, V5
022c	draw	V0, V1, 4
022e	skip_eq	VF, 01
0230	j		23c
0232	draw	V0, V1, 4
0234	add		V1, V1, ff
0236	draw	V0, V1, 4
0238	jal		340
023a	j		21c
023c	keyne	V7
023e	jal		272
0240	keyne	V8
0242	jal		284
0244	keyne	V9
0246	jal		296
0248	keyeq	V2
024a	j		250
024c	li		V6, 00
024e	mv		DT, V6
0250	mv		V6, DT
0252	skip_eq	V6, 00
0254	j		23c
0256	draw	V0, V1, 4
0258	add		V1, V1, 01
025a	j		22a
025c	li		I, 2c4
025e	add		I, I, V4
0260	li		V6, 00
0262	skip_ne	V3, 01
0264	li		V6, 04
0266	skip_ne	V3, 02
0268	li		V6, 08
026a	skip_ne	V3, 03
026c	li		V6, 0c
026e	add		I, I, V6
0270	ret
0272	draw	V0, V1, 4
0274	add		V0, V0, ff
0276	jal		334
0278	skip_eq	VF, 01
027a	ret
027c	draw	V0, V1, 4
027e	add		V0, V0, 01
0280	jal		334
0282	ret
0284	draw	V0, V1, 4
0286	add		V0, V0, 01
0288	jal		334
028a	skip_eq	VF, 01
028c	ret
028e	draw	V0, V1, 4
0290	add		V0, V0, ff
0292	jal		334
0294	ret
0296	draw	V0, V1, 4
0298	add		V3, V3, 01
029a	skip_ne	V3, 04
029c	li		V3, 00
029e	jal		25c
02a0	jal		334
02a2	skip_eq	VF, 01
02a4	ret
02a6	draw	V0, V1, 4
02a8	add		V3, V3, ff
02aa	skip_ne	V3, ff
02ac	li		V3, 03
02ae	jal		25c
02b0	jal		334
02b2	ret
02b4	mv		V0, V0
02b6	li		V7, 05
02b8	li		V8, 06
02ba	li		V9, 04
02bc	li		V1, 1f
02be	li		V5, 10
02c0	li		V2, 07
02c2	ret
02c4	skip_ne	V0, e0
02c8	skip_ne	V0, c0
02ca	skip_ne	V0, 00
02cc	clear
02ce	skip_ne	V0, 00
02d0	skip_ne	V0, 60
02d2	skip_ne	V0, 00
02d4	skip_ne	V0, 40
02d6	li		V0, 00
02d8	jal		0e0
02dc	rand	V0
02de	skip_ne	V0, 00
02e0	clear
02e2	mv		V0, V0
02e4	skip_ne	V0, 40
02e6	rand	V0
02e8	clear
02ea	jal		000
02ec	li		V0, 40
02ee	skip_ne	V0, 00
02f0	mv		V0, VE
02f4	skip_ne	V0, c0
02f6	mv		V0, V0
02f8	rand	V0
02fc	skip_ne	V0, c0
02fe	mv		V0, V0
0300	rand	V0
0304	mv		V0, VC
0306	skip_ne	V0, 00
0308	ISTRUZIONE SCONOSCIUTA 0060
030a	rand	V0
030c	mv		V0, VC
030e	skip_ne	V0, 00
0310	ISTRUZIONE SCONOSCIUTA 0060
0312	rand	V0
0314	rand	V0
0318	rand	V0
031c	rand	V0
0320	rand	V0
0324	skip_ne	V0, 40
0326	skip_ne	V0, 40
0328	ISTRUZIONE SCONOSCIUTA 00F0
032c	skip_ne	V0, 40
032e	skip_ne	V0, 40
0330	ISTRUZIONE SCONOSCIUTA 00F0
0334	draw	V0, V1, 4
0336	li		V6, 35
0338	add		V6, V6, ff
033a	skip_eq	V6, 00
033c	j		338
033e	ret
0340	li		I, 2b4
0342	mv		VC, V1
0344	skip_eq	VC, 1e
0346	add		VC, VC, 01
0348	skip_eq	VC, 1e
034a	add		VC, VC, 01
034c	skip_eq	VC, 1e
034e	add		VC, VC, 01
0350	jal		35e
0352	skip_ne	VB, 0a
0354	jal		372
0356	skip_ne	V1, VC
0358	ret
035a	add		V1, V1, 01
035c	j		350
035e	li		V0, 1b
0360	li		VB, 00
0362	draw	V0, V1, 1
0364	skip_eq	VF, 00
0366	add		VB, VB, 01
0368	draw	V0, V1, 1
036a	add		V0, V0, 01
036c	skip_eq	V0, 25
036e	j		362
0370	ret
0372	li		V0, 1b
0374	draw	V0, V1, 1
0376	add		V0, V0, 01
0378	skip_eq	V0, 25
037a	j		374
037c	mv		VE, V1
037e	mv		VD, VE
0380	add		VE, VE, ff
0382	li		V0, 1b
0384	li		VB, 00
0386	draw	V0, VE, 1
0388	skip_eq	VF, 00
038a	j		390
038c	draw	V0, VE, 1
038e	j		394
0390	draw	V0, VD, 1
0392	add		VB, VB, 01
0394	add		V0, V0, 01
0396	skip_eq	V0, 25
0398	j		386
039a	skip_ne	VB, 00
039c	j		3a6
039e	add		VD, VD, ff
03a0	add		VE, VE, ff
03a2	skip_eq	VD, 01
03a4	j		382
03a6	jal		3c0
03a8	skip_eq	VF, 01
03aa	jal		3c0
03ac	add		VA, VA, 01
03ae	jal		3c0
03b0	mv		V0, VA
03b2	li		VD, 07
03b4	and		V0, VD
03b6	skip_ne	V0, 04
03b8	add		V5, V5, fe
03ba	skip_ne	V5, 02
03bc	li		V5, 04
03be	ret
03c0	li		I, 700
03c2	multistore 0-2
03c4	li		I, 804
03c6	bcd		VA
03c8	multiload 0-2
03ca	non implementata
03cc	li		VD, 32
03ce	li		VE, 00
03d0	draw	VD, VE, 5
03d2	add		VD, VD, 05
03d4	non implementata
03d6	draw	VD, VE, 5
03d8	add		VD, VD, 05
03da	non implementata
03dc	draw	VD, VE, 5
03de	li		I, 700
03e0	multiload 0-2
03e2	li		I, 2b4
03e4	ret
03e6	li		VA, 00
03e8	li		V0, 19
03ea	ret
03ec	skip_eq	V7, 23
