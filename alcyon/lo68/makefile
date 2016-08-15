CC = cc
C68 = c68
SOURCES = main.c symt.c version.c
OBJS = main.o symt.o version.o
CFLAGS = -n -O -DVAX11 -w
C68FLAGS = -n -L -r -DMC68000
LIB = -lV6
LIB68 = -l6

vax:
	@mkver -e "lo68 -"
	${CC} ${CFLAGS} ${SOURCES} -o lo68.vax ${LIB}

lo68:
	@mkver -e "lo68 -"
	${C68} ${C68FLAGS} ${SOURCES} -o lo68.4k ${LIB68}
	@setstack lo68.4k 8000 8000; size lo68.4k

all: vax lo68
