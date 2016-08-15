CC = cc
C68 = nc68
CFLAGS = -O -w -DVAX11
C68FLAGS = -L -r -DMC68000 -t0 -t1
LIB = -lV6
C68LIB = -l6
.IGNORE:

vax:
	mkver -e "c68 Main Driver -"
	${CC} ${CFLAGS} -n version.c main.c -o c68.vax ${LIB}

4k:
	mkver -e "c68 Main Driver -"
	${C68} ${C68FLAGS} -n version.c main.c -o c68.4k ${C68LIB}
	setstack c68.4k 8000 8000

all: vax 4k

obsolete:
	mkver -e "c68 Main Driver -"
	${C68} ${C68FLAGS} version.c main.c -o c68.68 ${C68LIB}
	setstack c68.68 8000 8000
	${C68} ${C68FLAGS} -n2 version.o main.o -o c68.2k ${C68LIB}
	setstack c68.2k 8000 8000
