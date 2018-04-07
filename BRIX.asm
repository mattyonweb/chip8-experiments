0200	li		VE, 05
0202	li		V5, 00      ;ipotizzo sia il punteggio del giocatore

initDisegnaTavolette:
    0204	li		VB, 06      ;spiazzamento sulle y
    0206	li		VA, 00      ;spiazzamento sulle x
    0208	li		I, 30c      ;indirizzo spirito tavoletta da rompere
    020a	draw	VA, VB, 1   ;disegna tavoletta
    020c	add		VA, VA, 04  ;sposta di 4 sulle x
    020e	skip_eq	VA, 40      ;se non ho ancora disegnato 10 tavolette
    0210	j		208         ;disegnane un'altra
    0212	add		VB, VB, 02  ;altrimenti: spostati in giù sulle y di 2
    0214	skip_eq	VB, 12      ;se non ho ancora disegnato 6 file di tavolette
    0216	j		206         ;disegnane un'altra

initDisegnaRacchetta:
    0218	li		VC, 20      ;altrimenti: VC = x della racchetta (32)?
    021a	li		VD, 1f      ;VD = y della racchetta(31)?
    021c	li		I, 310      ;indirizzo rachetta
    021e	draw	VC, VD, 1   ;disegna racchetta

disegnaVite:
    0220	jal		2f6 (disegnaPunteggio)  ;disegna punteggio
    0222	li		V0, 00      ;v0 = X delle vite rimanenti (sono 4!)
    0224	li		V1, 00      ;v1 = Y delle vite rimanenti
    0226	li		I, 312      ;spirito delle vite rimanenti
    0228	draw	V0, V1, 1   ;disegna le    vite rimanenti
    022a	add		V0, V0, 08  ;spostati di 8 sulle x
    022c	li		I, 30e      ;spirito della quinta vita rimanente
    022e	draw	V0, V1, 1   ;disegna la quinta vita rimanente
    
0230	li		V0, 40      ;v0=40   
0232	mv		DT, V0      ;dt=v0
0234	mv		V0, DT      ;v0=dt
0236	skip_eq	V0, 00      ;se v0 != 0         ;;INTERESSANTE: siamo sicuri che arrivi prima o poi a 0 preciso?
0238	j		234         ;allora torna indietro a 234
023a	rand	V6          ;v6 = xPallina = random()
023c	li		V7, 1e      ;v7 = yPallina = penultima riga
023e	li		V8, 01      ;"accelerazione" sulle x (velocità laterale)
0240	li		V9, ff      ;"accelerazione" sulle y (vai in su di 1) (ff sfrutta l'overflow)
0242	li		I, 30e      ;spirito pallina
0244	draw	V6, V7, 1   ;disegna pallina

spostaRacchetta:
    0246	li		I, 310      ;spirito racchetta
    0248	draw	VC, VD, 1   ;cancella (?) racchetta
    024a	li		V0, 04      ;input da controllare = 0x4
    024c	keyne	V0          ;se key() == 0x4:
    024e	add		VC, VC, fe  ;sposta a sx di 2 la racchetta (x.racchetta -= 2) TODO: perché 2?
    0250	li		V0, 06      ;input da controllare = 0x4
    0252	keyne	V0          ;se key() == 0x6:
    0254	add		VC, VC, 02  ;sposta a dx di 2 la racchetta
    0256	li		V0, 3f      ;v0 = 63 = 0011 1111
    0258	and		VC, V0      ;AND tra x.racchetta e la maschera F3; serve per il wrapping: se x.racchetta > 63 allora questa operazione disegna la racchetta non a 64 ma a 0
    025a	draw	VC, VD, 1   ;disegna racchetta nella nuova posizione
    
025c	li		I, 30e      ;spirito pallina
025e	draw	V6, V7, 1   ;cancella pallina
0260	add_curry	V6, V6, V8  ;aggiorna x pallina 
0262	add_curry	V7, V7, V9  ;aggiorna y pallina (=> decresce y di 1)
0264	li		V0, 3f      ;3f = 63 = maxX = 0011 1111
0266	and		V6, V0      ;sempre per il wrapping sulle x (v. 0258) (?che senso ha il wrapping qui?)
0268	li		V1, 1f      ;1f = 31 = maxY = 0001 1111
026a	and		V7, V1      ;wrapping sulle y (?)
026c	skip_ne	V7, 1f      ;se ypallina == 32: (se pallina tocca il bordo sotto)
026e	j		2ac         ;j to pallina in fondo
0270	skip_ne	V6, 00      ;se pallina tocca il bordo a sx:
0272	li		V8, 01      ;inverti la sua velocità laterale (che, per toccare il bordo sx, deve essere per forza ff)
0274	skip_ne	V6, 3f      ;se pallina tocca bordo dx
0276	li		V8, ff      ;inverti velocità laterale (v.272)
0278	skip_ne	V7, 00      ;se pallina tocca soffitto
027a	li		V9, 01      ;inverti senso di marcia verso il giù
027c	draw	V6, V7, 1   ;disegna la pallina nella nuova locazione
027e	skip_eq	VF, 01      ;se la pallina non tocca nulla:
0280	j		2aa         ;salta a 2aa (ricomincia il ciclo dal movimento della racchetta)
0282	skip_ne	V7, 1f      ;se la pallina tocca qualcosa E si trova nell'ultima riga (= tocca la racchetta)
0284	j		2aa         ;va a 2aa (j spostaRacchetta(), v.280)
0286	li		V0, 05      ;se la pallina tocca un blocchetto, metti 5 in v0
0288	sub_curry	V0, V0, V7  ;v0 = v0 - y.pallina; VF = 0 se y.pallina > 5
028a	skip_eq	VF, 00      ;se y.pallina < 5: (??? che cosa dovrebbe trovare come ostacolo???)
028c	j		2aa         ;va a 2aa (j spostaRacchetta(), v.280)
028e	li		V0, 01      ;se altrimenti y.pallina nei luoghi dove ci sono i blocchetti, v0=1
0290	mv		ST, V0      ;st=01 (aka. suona il beep (quasi) subito?)
0292	mv		V0, V6      ;v0 = x.pallina
0294	li		V1, fc      ;v1 = fc = 1111 1100 (NON CAPISCO PERCHE)
0296	and		V0, V1      ;v0 = 1111 1100 && x.pallina (FORSE PER TROVARE IL PUNTO DI ORIGINE DEL BLOCCHETTO PER POTERLO CANCELLARE TUTTO?)
0298	li		I, 30c      ;spirito blocchetto
029a	draw	V0, V7, 1   ;cancella(?) il blocchetto 
029c	li		V0, fe      ;fe = 1111 1110
029e	xor		V9, V0      ;xor 1111 1110, velocità verticale pallina (che è o 1 o -1=ff) (-> v9 è o 0000 0001 o 1111 1111 (ff))
02a0	jal		2f6         ;CANCELLA il punteggio ormai vecchio (hai colpito un blocchettino!) 
02a2	add		V5, V5, 01  ;aggiungi uno al punteggio
02a4	jal		2f6         ;DISEGNA il punteggio aggiornato
02a6	skip_ne	V5, 60      ;se ho rotto tutti i blocchettini
02a8	j		2de         ;fine
02aa	j		246         ;va alla procedura spostaRacchetta()

pallinaInFondo:
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

end:
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

disegnaPunteggio:
    02f6	li		I, 314              ;punta a spazio vuoto
    02f8	bcd		V5                  ;salva le centinaia-decine-unità di V5 (punteggio?) nello spazio vuoto
    02fa	multiload 0-2               ;i registri V0,V1,V2 ora contengono centinaia-decine-unita
    02fc	non implementata            ;carica spiriti numeri
    02fe	li		V3, 37              ;coordinate spirito numero x
    0300	li		V4, 00              ;coordinate spirito numero y
    0302	draw	V3, V4, 5           ;disegna il numero
    0304	add		V3, V3, 05          ;spostati a dx di 5 (?)
    0306	non implementata            ;prendi spirito numero
    0308	draw	V3, V4, 5           ;disegna seconda cifra punteggio
    030a	ret                         ;ritorna
    
030c	ISTRUZIONE SCONOSCIUTA E000 ;è lo spirito del blocchetto da rompere
030e	mv		V0, V0              ;è lo spirito della palla (X-------)
0310	ISTRUZIONE SCONOSCIUTA FC00 ;è lo spirito della racchetta
0312	li		I, a00              ;è lo spirito delle vite rimanenti (X-X-X-X-)
