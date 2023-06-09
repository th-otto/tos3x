#define T0OBJ 0
#define T1OBJ 28
#define T2OBJ 51
#define T3OBJ 60
#define T4OBJ 74
#define FREEBB 2
#define FREEIMG 10
#define FREESTR 96

BYTE *rs_strings[] = {
"0000",
"__:__",
"9999",
"0000A",
"__:__ _M",
"9999F",
"000000",
"__/__/__",
"999999",
"Opzioni",
"01234567890123456",
"",
"",
"12345678.123",
"01234567890123456",
"",
"",
"12345678.123",
"01234567890123456",
"",
"",
"12345678.123",
"01234567890123456",
"",
"",
"12345678.123",
"12 Hour",
"",
"",
"Orario:",
"ABCDEFGHIJKLMNOPQRSTUVWX",
"________________________",
"pppppppppppppppppppppppp",
"Percorso file CPX",
"",
"",
"Salva",
"OK",
"Annulla",
"Pannello di Controllo",
"estensibile",
"Versione 1.32",
"-----------------",
"Copyright � 1990-1993",
"Atari Corporation",
"OK",
" Nome file:",
" Versione:",
" Residente:",
"00000000.000",
"0000 ",
"YES",
"00.00 ",
" ID:",
"Esci",
"Info CPX",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
" Attivi ",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
"01234567",
"",
"",
" Inattivi ",
"",
"",
"Esci",
"ABCDEFGHIJKLMNOPQ",
"_________________",
"ppppppppppppppppp"};

WORD IMAG0[] = {
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFFF, 0xFFFF, 0xFFF0};

WORD IMAG1[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 
0x8000, 0x1, 0x8000, 0x1, 
0x8007, 0xF001, 0x800F, 0xF801, 
0x801F, 0xEC01, 0x8018, 0x401, 
0x8018, 0x401, 0x8010, 0x401, 
0x801E, 0x3C01, 0x8017, 0x5401, 
0x8011, 0x401, 0x800B, 0x2801, 
0x800D, 0xD801, 0x8006, 0x2801, 
0x8007, 0xD001, 0x802E, 0x1001, 
0x8039, 0xE001, 0x8038, 0x1, 
0x8000, 0x1, 0x8000, 0x1, 
0x8000, 0x1, 0xFFFF, 0xFFFF};

WORD IMAG2[] = {
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFFF, 0xFFFF, 0xFFF0};

WORD IMAG3[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 
0x8000, 0x1, 0x8000, 0x1, 
0x8007, 0xF001, 0x800F, 0xF801, 
0x801F, 0xEC01, 0x8018, 0x401, 
0x8018, 0x401, 0x8010, 0x401, 
0x801E, 0x3C01, 0x8017, 0x5401, 
0x8011, 0x401, 0x800B, 0x2801, 
0x800D, 0xD801, 0x8006, 0x2801, 
0x8007, 0xD001, 0x802E, 0x1001, 
0x8039, 0xE001, 0x8038, 0x1, 
0x8000, 0x1, 0x8000, 0x1, 
0x8000, 0x1, 0xFFFF, 0xFFFF};

WORD IMAG4[] = {
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFFF, 0xFFFF, 0xFFF0};

WORD IMAG5[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 
0x8000, 0x1, 0x8000, 0x1, 
0x8007, 0xF001, 0x800F, 0xF801, 
0x801F, 0xEC01, 0x8018, 0x401, 
0x8018, 0x401, 0x8010, 0x401, 
0x801E, 0x3C01, 0x8017, 0x5401, 
0x8011, 0x401, 0x800B, 0x2801, 
0x800D, 0xD801, 0x8006, 0x2801, 
0x8007, 0xD001, 0x802E, 0x1001, 
0x8039, 0xE001, 0x8038, 0x1, 
0x8000, 0x1, 0x8000, 0x1, 
0x8000, 0x1, 0xFFFF, 0xFFFF};

WORD IMAG6[] = {
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFF, 0xFFFF, 0xFE00, 
0xFF, 0xFFFF, 0xFE00, 0xFF, 
0xFFFF, 0xFE00, 0xFF, 0xFFFF, 
0xFE00, 0xFFF, 0xFFFF, 0xFFF0};

WORD IMAG7[] = {
0x0, 0x0, 0xFFFF, 0xFFFF, 
0x8000, 0x1, 0x8000, 0x1, 
0x8007, 0xF001, 0x800F, 0xF801, 
0x801F, 0xEC01, 0x8018, 0x401, 
0x8018, 0x401, 0x8010, 0x401, 
0x801E, 0x3C01, 0x8017, 0x5401, 
0x8011, 0x401, 0x800B, 0x2801, 
0x800D, 0xD801, 0x8006, 0x2801, 
0x8007, 0xD001, 0x802E, 0x1001, 
0x8039, 0xE001, 0x8038, 0x1, 
0x8000, 0x1, 0x8000, 0x1, 
0x8000, 0x1, 0xFFFF, 0xFFFF};

WORD IMAG8[] = {
0x0, 0x0, 0x0, 0x4, 
0x1FFF, 0x8000, 0x6, 0x1000, 
0xC000, 0x7, 0x1204, 0xC000, 
0x3FF, 0x950A, 0xC000, 0x7FF, 
0xD294, 0xC000, 0xFFF, 0xD168, 
0xC000, 0xFFF, 0x9090, 0xC000, 
0xF07, 0x1090, 0xC000, 0xF06, 
0x1168, 0xC000, 0xF04, 0x1294, 
0xC000, 0xF00, 0x150A, 0xC000, 
0xF00, 0x1204, 0xC000, 0x0, 
0x1000, 0xC000, 0x3FFF, 0x1FFF, 
0xC000, 0x2001, 0x8FFF, 0x8000, 
0x2C0D, 0x8000, 0x0, 0x2E1D, 
0x801E, 0x0, 0x2739, 0x801E, 
0x0, 0x23F1, 0x821E, 0x0, 
0x21E1, 0x861E, 0x0, 0x21E1, 
0x8E1E, 0x0, 0x21E1, 0x9FFE, 
0x0, 0x23F1, 0xBFFE, 0x0, 
0x2739, 0xBFFC, 0x0, 0x2E1D, 
0x9FF8, 0x0, 0x2C0D, 0x8E00, 
0x0, 0x2001, 0x8600, 0x0, 
0x3FFF, 0x8200, 0x0, 0x1FFF, 
0x8000, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0};

WORD IMAG9[] = {
0x0, 0x1FFF, 0xF800, 0x0, 
0x1524, 0xAC00, 0x0, 0x1492, 
0x4C00, 0x2, 0x1FFF, 0xFC00, 
0x3, 0x1000, 0xC00, 0xFF, 
0x97FF, 0xC00, 0x1FF, 0xD7FF, 
0xC00, 0x3FF, 0xD000, 0xC00, 
0x3FF, 0x97FF, 0xEC00, 0x383, 
0x1400, 0xAC00, 0x382, 0x1554, 
0xAC00, 0x380, 0x1400, 0xAC00, 
0x380, 0x1554, 0xAC00, 0x380, 
0x1400, 0xAC00, 0x0, 0x1400, 
0xAC00, 0xFFFF, 0x17FF, 0xEC00, 
0x8001, 0x9000, 0xC00, 0xB00D, 
0x9FFF, 0xFC00, 0xB81D, 0x8FFF, 
0xFC00, 0x9C39, 0x8000, 0x0, 
0x8E71, 0x800C, 0x0, 0x87E1, 
0x801E, 0x0, 0x83C1, 0x803F, 
0x0, 0x83C1, 0x807F, 0x8000, 
0x87E1, 0x801E, 0x0, 0x8E71, 
0x801E, 0x0, 0x9C39, 0x801E, 
0x0, 0xB81D, 0x9FFE, 0x0, 
0xB00D, 0x9FFC, 0x0, 0x8001, 
0x9FF8, 0x0, 0xFFFF, 0x8000, 
0x0, 0x7FFF, 0x8000, 0x0};

LONG rs_frstr[] = {
0};

BITBLK rs_bitblk[] = {
8L, 6, 32, 0, 0, 1,
9L, 6, 32, 0, 0, 1};

LONG rs_frimg[] = {
0};

ICONBLK rs_iconblk[] = {
0L, 1L, 13L, 4096,21,9, 20,0,32,24, 0,24,72,8,
2L, 3L, 17L, 4096,21,9, 20,0,32,24, 0,24,72,8,
4L, 5L, 21L, 4096,21,9, 20,0,32,24, 0,24,72,8,
6L, 7L, 25L, 4096,21,9, 20,0,32,24, 0,24,72,8};

TEDINFO rs_tedinfo[] = {
0L, 1L, 2L, 3, 6, 2, 0x1180, 0x0, 0, 5,6,
3L, 4L, 5L, 3, 6, 2, 0x1180, 0x0, 0, 6,9,
6L, 7L, 8L, 3, 6, 2, 0x1180, 0x0, 0, 7,9,
10L, 11L, 12L, 3, 6, 0, 0x1180, 0x0, -1, 18,1,
14L, 15L, 16L, 3, 6, 0, 0x1180, 0x0, -1, 18,1,
18L, 19L, 20L, 3, 6, 0, 0x1180, 0x0, -1, 18,1,
22L, 23L, 24L, 3, 6, 0, 0x1180, 0x0, -1, 18,1,
26L, 27L, 28L, 3, 6, 2, 0x1100, 0x0, -1, 8,1,
30L, 31L, 32L, 3, 6, 2, 0x1100, 0x0, -1, 25,25,
33L, 34L, 35L, 3, 6, 2, 0x1180, 0x0, -1, 18,1,
56L, 57L, 58L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
59L, 60L, 61L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
62L, 63L, 64L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
65L, 66L, 67L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
68L, 69L, 70L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
71L, 72L, 73L, 3, 6, 2, 0x11A1, 0x0, -1, 9,1,
74L, 75L, 76L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
77L, 78L, 79L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
80L, 81L, 82L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
83L, 84L, 85L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
86L, 87L, 88L, 3, 6, 2, 0x1180, 0x0, -1, 9,1,
89L, 90L, 91L, 3, 6, 2, 0x11A1, 0x0, -1, 11,1,
93L, 94L, 95L, 3, 6, 2, 0x1100, 0x0, -2, 18,18};

OBJECT rs_object[] = {
-1, 1, 5, G_BOX, NONE, NORMAL, 0xFF1111L, 0,0, 32,267,
5, 2, 4, G_BOX, NONE, NORMAL, 0xFF1100L, 0,0, 32,1,
3, -1, -1, G_FBOXTEXT, HIDETREE, NORMAL, 0x0L, 769,0, 1548,1,
4, -1, -1, G_FBOXTEXT, NONE, NORMAL, 0x1L, 1537,0, 1548,1,
1, -1, -1, G_FBOXTEXT, NONE, NORMAL, 0x2L, 786,0, 1291,1,
0, 6, 27, G_BOX, NONE, NORMAL, 0x1141L, 0,257, 32,10,
7, -1, -1, G_BUTTON, NONE, SHADOWED, 0x9L, 1282,512, 1547,513,
24, 8, 20, G_BOX, NONE, NORMAL, 0xFF1100L, 1024,2561, 1564,776,
12, 9, 11, G_BOX, NONE, NORMAL, 0xFF1100L, 0,0, 1564,2,
10, -1, -1, G_TEXT, NONE, NORMAL, 0x3L, 1291,2048, 528,513,
11, -1, -1, G_ICON, NONE, NORMAL, 0x0L, 1281,0, 9,2,
8, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 1564,2,
16, 13, 15, G_BOX, NONE, NORMAL, 0xFF1100L, 0,258, 1564,2,
14, -1, -1, G_TEXT, NONE, NORMAL, 0x4L, 1291,2048, 784,513,
15, -1, -1, G_ICON, NONE, NORMAL, 0x1L, 1281,0, 9,2,
12, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 1564,2,
20, 17, 19, G_BOX, NONE, NORMAL, 0xFF1100L, 0,516, 1564,2,
18, -1, -1, G_TEXT, NONE, NORMAL, 0x5L, 1291,2048, 272,257,
19, -1, -1, G_ICON, NONE, NORMAL, 0x2L, 1281,0, 9,2,
16, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 1564,2,
7, 21, 23, G_BOX, NONE, NORMAL, 0xFF1100L, 0,774, 1564,2,
22, -1, -1, G_TEXT, NONE, NORMAL, 0x6L, 1291,2048, 528,513,
23, -1, -1, G_ICON, NONE, NORMAL, 0x3L, 1281,0, 9,2,
20, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 1564,2,
26, 25, 25, G_BOX, NONE, NORMAL, 0xFF1111L, 797,3330, 258,3333,
24, -1, -1, G_BOX, NONE, NORMAL, 0xFF1100L, 0,0, 258,3333,
27, -1, -1, G_BOXCHAR, NONE, NORMAL, 0x1FF1100L, 797,2561, 258,513,
5, -1, -1, G_BOXCHAR, LASTOB, NORMAL, 0x2FF1100L, 797,2824, 258,513,
-1, 1, 20, G_BOX, NONE, NORMAL, 0xFF1141L, 12,1538, 32,3849,
2, -1, -1, G_BOX, NONE, NORMAL, 0xFF1141L, 0,0, 1552,3073,
5, 3, 4, G_BOX, NONE, NORMAL, 0xFF1100L, 0,0, 1552,3073,
4, -1, -1, G_BOXTEXT, NONE, SHADOWED, 0x7L, 1287,1536, 1032,1,
2, -1, -1, G_STRING, NONE, NORMAL, 0x1DL, 768,1536, 7,1,
9, 6, 8, G_BOX, NONE, NORMAL, 0xFF1141L, 0,3585, 520,3,
7, -1, -1, G_BOX, NONE, NORMAL, 0xFF1100L, 769,1792, 773,514,
8, -1, -1, G_IMAGE, NONE, NORMAL, 0x0L, 1793,2304, 1028,3585,
5, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 520,3,
13, 10, 12, G_BOX, NONE, NORMAL, 0xFF1141L, 1032,3585, 520,3,
11, -1, -1, G_BOX, NONE, NORMAL, 0xFF1100L, 769,1792, 773,514,
12, -1, -1, G_IMAGE, NONE, NORMAL, 0x1L, 1537,2048, 1540,2,
9, -1, -1, G_IBOX, NONE, NORMAL, 0x1100L, 0,0, 520,3,
18, 14, 17, G_BOX, NONE, NORMAL, 0xFF1141L, 0,5, 32,3330,
15, -1, -1, G_FBOXTEXT, EDITABLE, NORMAL, 0x8L, 1027,1281, 281,769,
16, -1, -1, G_BOXCHAR, NONE, NORMAL, 0x4FF1100L, 1280,1281, 770,769,
17, -1, -1, G_BOXCHAR, NONE, NORMAL, 0x3FF1100L, 285,1281, 770,769,
13, -1, -1, G_BOXTEXT, NONE, NORMAL, 0x9L, 0,0, 32,1,
20, 19, 19, G_BOX, NONE, NORMAL, 0xFF1141L, 0,3847, 779,2,
18, -1, -1, G_BUTTON, 0x5, NORMAL, 0x24L, 1281,1792, 8,1,
0, 21, 22, G_BOX, NONE, NORMAL, 0xFF1141L, 1291,3847, 788,2,
22, -1, -1, G_BUTTON, 0x7, NORMAL, 0x25L, 257,1792, 8,1,
20, -1, -1, G_BUTTON, 0x25, NORMAL, 0x26L, 1546,1792, 1288,1,
-1, 1, 1, G_BOX, NONE, NORMAL, 0xFF1141L, 0,0, 32,10,
0, 2, 8, G_BOX, NONE, NORMAL, 0xFF1100L, 1537,2048, 1052,3848,
3, -1, -1, G_STRING, NONE, NORMAL, 0x27L, 1539,1536, 21,1,
4, -1, -1, G_STRING, NONE, NORMAL, 0x28L, 1032,1793, 1035,257,
5, -1, -1, G_STRING, NONE, NORMAL, 0x29L, 1543,3842, 13,1,
6, -1, -1, G_STRING, NONE, NORMAL, 0x2AL, 1541,3331, 17,1,
7, -1, -1, G_STRING, NONE, NORMAL, 0x2BL, 1539,3076, 21,3328,
8, -1, -1, G_STRING, NONE, NORMAL, 0x2CL, 1541,3333, 17,1,
1, -1, -1, G_BUTTON, 0x27, NORMAL, 0x2DL, 522,1799, 8,1,
-1, 1, 1, G_BOX, NONE, NORMAL, 0xFF1141L, 0,0, 32,10,
0, 2, 13, G_BOX, NONE, OUTLINED, 0x11100L, 1794,1, 538,3847,
12, 3, 11, G_BOX, NONE, NORMAL, 0xFF1100L, 1280,2049, 1816,2308,
4, -1, -1, G_STRING, NONE, NORMAL, 0x2EL, 0,512, 14,1,
5, -1, -1, G_STRING, NONE, NORMAL, 0x2FL, 0,769, 10,1,
6, -1, -1, G_STRING, NONE, NORMAL, 0x30L, 0,1539, 15,1,
11, 7, 10, G_IBOX, NONE, NORMAL, 0x1100L, 524,0, 12,2308,
8, -1, -1, G_STRING, NONE, NORMAL, 0x31L, 0,768, 12,1,
9, -1, -1, G_STRING, NONE, NORMAL, 0x32L, 0,1282, 5,1,
10, -1, -1, G_STRING, NONE, NORMAL, 0x33L, 0,1795, 515,3840,
6, -1, -1, G_STRING, NONE, NORMAL, 0x34L, 0,1025, 6,1,
2, -1, -1, G_STRING, NONE, NORMAL, 0x35L, 0,1026, 1540,257,
13, -1, -1, G_BUTTON, 0x7, NORMAL, 0x36L, 265,2054, 8,1,
1, -1, -1, G_STRING, LASTOB, NORMAL, 0x37L, 1544,1280, 1288,257,
-1, 1, 24, G_BOX, NONE, NORMAL, 0xFF1141L, 0,0, 32,3849,
7, 2, 6, G_BOX, NONE, NORMAL, 0xFF1100L, 2,1281, 1546,5,
3, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0xAL, 0,0, 1546,1,
4, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0xBL, 0,1, 1546,1,
5, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0xCL, 0,2, 1546,1,
6, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0xDL, 0,3, 1546,1,
1, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0xEL, 0,4, 1546,1,
8, -1, -1, G_BOXTEXT, NONE, NORMAL, 0xFL, 2,768, 525,257,
9, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, 0x1FF1100L, 1804,1281, 770,257,
10, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, 0x2FF1100L, 1804,1285, 770,1,
12, 11, 11, G_BOX, TOUCHEXIT, NORMAL, 0xFF1111L, 1804,1794, 770,3330,
10, -1, -1, G_BOX, TOUCHEXIT, NORMAL, 0xFF1100L, 0,0, 770,2305,
18, 13, 17, G_BOX, NONE, NORMAL, 0xFF1100L, 1552,1281, 1546,5,
14, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0x10L, 0,0, 1546,1,
15, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0x11L, 0,1, 1546,1,
16, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0x12L, 0,2, 1546,1,
17, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0x13L, 0,3, 1546,1,
12, -1, -1, G_TEXT, TOUCHEXIT, NORMAL, 0x14L, 0,4, 1546,1,
20, 19, 19, G_BOX, TOUCHEXIT, NORMAL, 0xFF1111L, 1307,1794, 770,3330,
18, -1, -1, G_BOX, TOUCHEXIT, NORMAL, 0xFF1100L, 0,0, 770,2305,
21, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, 0x1FF1100L, 1307,1281, 770,257,
22, -1, -1, G_BOXCHAR, TOUCHEXIT, NORMAL, 0x2FF1100L, 1307,1285, 770,1,
23, -1, -1, G_BOXTEXT, NONE, NORMAL, 0x15L, 1552,768, 525,257,
24, -1, -1, G_BUTTON, 0x47, NORMAL, 0x5CL, 780,1544, 8,1,
0, -1, -1, G_FBOXTEXT, 0x65, NORMAL, 0x16L, 2,2822, 284,513};

LONG rs_trindex[] = {
0L,
28L,
51L,
60L,
74L};

struct foobar {
	WORD	dummy;
	WORD	*image;
	} rs_imdope[] = {
0, &IMAG0[0],
0, &IMAG1[0],
0, &IMAG2[0],
0, &IMAG3[0],
0, &IMAG4[0],
0, &IMAG5[0],
0, &IMAG6[0],
0, &IMAG7[0],
0, &IMAG8[0],
0, &IMAG9[0]};

#define NUM_STRINGS 96
#define NUM_FRSTR 0
#define NUM_IMAGES 10
#define NUM_BB 2
#define NUM_FRIMG 0
#define NUM_IB 4
#define NUM_TI 23
#define NUM_OBS 99
#define NUM_TREE 5

BYTE pname[] = "ITALY.RSC";
