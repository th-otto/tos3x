#define T0OBJ 0
#define T1OBJ 63
#define FREEBB 0
#define FREEIMG 0
#define FREESTR 96

BYTE *rs_strings[] = {
"Save",
"OK",
"Cancel",
"dow",
"",
"",
"Info",
"",
"",
" Window ",
"",
"",
" Info",
"",
"",
"13",
"",
"",
"13",
"",
"",
"13",
"",
"",
" Mode ",
"",
"",
"Border ",
"",
"",
"Text ",
"",
"",
"Fill ",
"",
"",
"USA",
"OK",
"Cancel",
"Save",
" Window ",
" Info",
"Border ",
"Text ",
"Fill ",
" Mode ",
"FRG",
"OK",
"Abbruch",
"Sichern",
" Fenster ",
" Info",
"Rand ",
"Text ",
"F�llen ",
" Modus ",
"FRA",
"CONFIRME",
"Annule",
"Sauve",
" Fen�tre ",
" Info",
"Bord ",
"Texte ",
"Motif ",
" Mode ",
"SPA",
"CONFIRMAR",
"Anular",
"Grabar",
" Ventana ",
" Informaci�n",
"Borde ",
"Texto ",
"Llenado ",
" Modo ",
"ITA",
"OK",
"Annulla",
"Salva",
" Finestra ",
" Info",
"Bordo ",
"Testo ",
"Retino ",
" Modo ",
"SWE",
"OK",
"AVBRYT",
"SPARA",
" F�nster ",
" Info",
"Ram ",
"Text ",
"Bakgrund ",
" L�ge ",
"[1][You can not set the|colors of windows|in this TOS version.][  OK \
 ]",
"[1][In dieser TOS-Version|k�nnen keine Fenster-|farben gesetzt werden\
.][  OK  ]",
"[1][Vous ne pouvez pas d�finir|les couleurs des fen�tres dans|cette v\
ersion du TOS.][ CONFIRME ]",
"[1][No es posible configurar|los colores de las ventanas|con esta ver\
si�n de TOS.][CONFIRMAR]",
"[1][Non � possibile impostare|i colori delle finestre in|questa versi\
one del TOS.][  OK  ]",
"[1][Det �r inte m�jligt att �ndra|f�rgerna p� f�nstren i denna|TOS-ve\
rsion.][  OK  ]"};

LONG rs_frstr[] = {
96L,
97L,
98L,
99L,
100L,
101L};

BITBLK rs_bitblk[] = {
0};

LONG rs_frimg[] = {
0};

ICONBLK rs_iconblk[] = {
0};

TEDINFO rs_tedinfo[] = {
(char*)3L, (char*)4L, (char*)5L, 3, 6, 0, 0x11F0, 0x0, -1, 4,1,
(char*)6L, (char*)7L, (char*)8L, 3, 6, 0, 0x1180, 0x0, -1, 5,1,
(char*)9L, (char*)10L, (char*)11L, 3, 6, 2, 0x11A1, 0x0, -1, 9,1,
(char*)12L, (char*)13L, (char*)14L, 3, 6, 0, 0x1180, 0x0, -1, 6,1,
(char*)15L, (char*)16L, (char*)17L, 3, 6, 2, 0x1170, 0x0, -1, 3,1,
(char*)18L, (char*)19L, (char*)20L, 3, 6, 2, 0x1170, 0x0, -1, 3,1,
(char*)21L, (char*)22L, (char*)23L, 3, 6, 2, 0x1170, 0x0, -1, 3,1,
(char*)24L, (char*)25L, (char*)26L, 3, 6, 2, 0x11A1, 0x0, -1, 7,1,
(char*)27L, (char*)28L, (char*)29L, 3, 6, 1, 0x1180, 0x0, -1, 8,1,
(char*)30L, (char*)31L, (char*)32L, 3, 6, 1, 0x1180, 0x0, -1, 6,1,
(char*)33L, (char*)34L, (char*)35L, 3, 6, 1, 0x1180, 0x0, -1, 6,1};

OBJECT rs_object[] = {
-1, 1, 62, G_BOX, NONE, NORMAL, (void*)0x1101L, 12,1538, 32,11,
3, 2, 2, G_BOX, NONE, NORMAL, (void*)0x1109L, 256,9, 1801,2,
1, -1, -1, G_BUTTON, 0x5, NORMAL, (void*)0x0L, 1,2048, 8,1,
6, 4, 5, G_BOX, NONE, NORMAL, (void*)0x1108L, 10,9, 22,2,
5, -1, -1, G_BUTTON, 0x7, NORMAL, (void*)0x1L, 1536,2048, 1545,1,
3, -1, -1, G_BUTTON, 0x5, NORMAL, (void*)0x2L, 1035,2048, 1545,1,
24, 7, 11, G_IBOX, NONE, SHADOWED, (void*)0xFF1100L, 1797,3328, 25,520,
10, 8, 9, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 0,0, 25,257,
9, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x0L, 531,0, 515,257,
7, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x7FF1100L, 1302,0, 770,257,
11, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x1L, 20,513, 5,257,
6, 12, 19, G_IBOX, NONE, NORMAL, (void*)0x11100L, 0,1026, 25,3589,
13, -1, -1, G_BOX, NONE, NORMAL, (void*)0x1100L, 0,0, 1046,3076,
14, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x6FF1100L, 1302,3332, 770,257,
19, 15, 18, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 1302,0, 770,3076,
16, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x1FF1100L, 0,0, 770,257,
17, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x2FF1100L, 0,2819, 770,257,
18, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1100L, 0,513, 770,769,
14, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1111L, 0,1538, 770,1025,
11, 20, 23, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1100L, 0,3332, 1046,257,
21, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x4FF1100L, 0,0, 770,257,
22, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x3FF1100L, 276,0, 770,257,
23, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1100L, 1026,0, 9,257,
19, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1111L, 1291,0, 776,257,
62, 25, 30, G_IBOX, NONE, SHADOWED, (void*)0xFF1100L, 1792,1024, 25,520,
29, 26, 28, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 0,0, 25,257,
27, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x5FF1100L, 0,0, 258,257,
28, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x7FF1170L, 1302,0, 770,257,
25, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x2L, 770,0, 20,257,
30, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x3L, 0,513, 25,257,
24, 31, 61, G_IBOX, NONE, NORMAL, (void*)0x11100L, 0,1026, 25,3589,
51, 32, 50, G_BOX, NONE, NORMAL, (void*)0x1100L, 0,0, 1046,3076,
34, 33, 33, G_BOX, TOUCHEXIT, NORMAL, (void*)0x31151L, 524,256, 10,1,
32, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x4L, 6,0, 3,1,
36, 35, 35, G_BOX, TOUCHEXIT, NORMAL, (void*)0x31151L, 524,769, 10,1,
34, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x5L, 6,0, 3,1,
38, 37, 37, G_BOX, TOUCHEXIT, NORMAL, (void*)0x31151L, 524,1282, 10,1,
36, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x6L, 6,0, 3,1,
39, -1, -1, G_BOXTEXT, TOUCHEXIT, NORMAL, (void*)0x7L, 768,2051, 1801,257,
48, 40, 47, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 524,2307, 10,1,
41, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21101L, 0,0, 513,1,
42, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21111L, 513,0, 513,1,
43, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21121L, 1026,0, 513,1,
44, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21131L, 1539,0, 513,1,
45, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21141L, 5,0, 513,1,
46, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21151L, 518,0, 513,1,
47, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21161L, 1031,0, 513,1,
39, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0x21171L, 1544,0, 513,1,
49, -1, -1, G_TEXT, NONE, NORMAL, (void*)0x8L, 256,256, 12,1,
50, -1, -1, G_TEXT, NONE, NORMAL, (void*)0x9L, 256,769, 12,1,
31, -1, -1, G_TEXT, NONE, NORMAL, (void*)0xAL, 256,1282, 12,1,
56, 52, 55, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 1302,0, 770,3076,
53, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x1FF1100L, 0,0, 770,513,
54, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x2FF1100L, 0,3075, 770,1,
55, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1111L, 0,1794, 770,1025,
51, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1100L, 0,769, 770,769,
61, 57, 60, G_BOX, NONE, NORMAL, (void*)0xFF1100L, 0,3332, 1046,257,
58, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x4FF1100L, 0,0, 770,257,
59, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x3FF1100L, 276,0, 770,257,
60, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1111L, 1288,0, 779,257,
56, -1, -1, G_BOX, TOUCHEXIT, NORMAL, (void*)0xFF1100L, 1026,0, 6,257,
30, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, (void*)0x6FF1100L, 1302,3332, 770,257,
0, -1, -1, G_IBOX, 0x60, NORMAL, (void*)0xFF1100L, 1024,256, 1794,1793,
-1, 1, 56, G_BOX, NONE, NORMAL, (void*)0x11100L, 0,0, 78,16,
12, 2, 11, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 2,1, 11,14,
3, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x24L, 0,0, 11,1,
4, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x25L, 1,2, 2,1,
5, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x26L, 1,3, 6,1,
6, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x27L, 1,4, 4,1,
7, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x28L, 1,6, 8,1,
8, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x29L, 1,7, 5,1,
9, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x2AL, 1,9, 7,1,
10, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x2BL, 1,10, 5,1,
11, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x2CL, 1,11, 5,1,
1, -1, -1, G_BUTTON, SELECTABLE, NORMAL, (void*)0x2DL, 1,12, 6,1,
23, 13, 22, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 14,1, 11,14,
14, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x2EL, 0,0, 11,1,
15, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x2FL, 1,2, 2,1,
16, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x30L, 1,3, 7,1,
17, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x31L, 1,4, 7,1,
18, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x32L, 1,6, 9,1,
19, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x33L, 1,7, 5,1,
20, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x34L, 1,9, 5,1,
21, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x35L, 1,10, 5,1,
22, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x36L, 1,11, 7,1,
12, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x37L, 1,12, 7,1,
34, 24, 33, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 26,1, 11,14,
25, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x38L, 0,0, 11,1,
26, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x39L, 1,2, 8,1,
27, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3AL, 1,3, 6,1,
28, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3BL, 1,4, 5,1,
29, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3CL, 1,6, 9,1,
30, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3DL, 1,7, 5,1,
31, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3EL, 1,9, 5,1,
32, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x3FL, 1,10, 6,1,
33, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x40L, 1,11, 6,1,
23, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x41L, 1,12, 6,1,
45, 35, 44, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 38,1, 14,14,
36, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x42L, 0,0, 14,1,
37, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x43L, 1,2, 9,1,
38, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x44L, 1,3, 6,1,
39, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x45L, 1,4, 6,1,
40, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x46L, 1,6, 9,1,
41, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x47L, 1,7, 12,1,
42, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x48L, 1,9, 6,1,
43, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x49L, 1,10, 6,1,
44, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4AL, 1,11, 8,1,
34, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4BL, 1,12, 6,1,
56, 46, 55, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 53,1, 12,14,
47, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4CL, 0,0, 12,1,
48, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4DL, 1,2, 2,1,
49, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4EL, 1,3, 7,1,
50, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x4FL, 1,4, 5,1,
51, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x50L, 1,6, 10,1,
52, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x51L, 1,7, 5,1,
53, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x52L, 1,9, 6,1,
54, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x53L, 1,10, 6,1,
55, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x54L, 1,11, 7,1,
45, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x55L, 1,12, 6,1,
0, 57, 66, G_BOX, NONE, NORMAL, (void*)0xFF112AL, 66,1, 11,14,
58, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x56L, 0,0, 11,1,
59, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x57L, 1,2, 2,1,
60, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x58L, 1,3, 6,1,
61, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x59L, 1,4, 5,1,
62, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x5AL, 1,6, 9,1,
63, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x5BL, 1,7, 5,1,
64, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x5CL, 1,9, 4,1,
65, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x5DL, 1,10, 5,1,
66, -1, -1, G_BUTTON, NONE, NORMAL, (void*)0x5EL, 1,11, 9,1,
56, -1, -1, G_BUTTON, LASTOB, NORMAL, (void*)0x5FL, 1,12, 6,1};

LONG rs_trindex[] = {
0L,
63L};

struct foobar {
	WORD	dummy;
	WORD	*image;
	} rs_imdope[] = {
0};

#define NUM_STRINGS 102
#define NUM_FRSTR 6
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_TI 11
#define NUM_OBS 130
#define NUM_TREE 2

BYTE pname[] = "WCOLORX.RSC";
