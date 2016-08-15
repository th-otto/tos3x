CC = cc
C68 = nc68
SOURCES = dir.c expr.c misc.c pass1a.c pass2.c symt.c main.c 
INCLUDES = as68.h cout.h order.h
VAXOBJS = vaxobj/dir.o vaxobj/expr.o vaxobj/misc.o vaxobj/pass1a.o \
		vaxobj/pass2.o vaxobj/symt.o vaxobj/main.o
C68OBJS = 68obj/dir.o 68obj/expr.o 68obj/misc.o 68obj/pass1a.o \
		68obj/pass2.o 68obj/symt.o 68obj/main.o
CFLAGS = -w -O -DVAX11
C68FLAGS = -L -r -DMC68000 -t0 -t1
LIB = -lV6
C68LIB = -l6

vax: ${VAXOBJS}
	@mkver -e "assembler -"
	${CC} ${CFLAGS} version.c ${VAXOBJS} -o as68.vax ${LIB}

as68: ${C68OBJS}
	@mkver -e "assembler -"
	${C68} ${C68FLAGS} ${C68OBJS} version.c -o as68.68 ${C68LIB}
	@setstack as68.68 8192 8192

2k: ${C68OBJS}
	@mkver -e "assembler -"
	${C68} -n2 ${C68FLAGS} -r ${C68OBJS} version.c -o as68.2k ${C68LIB}
	@setstack as68.2k 8192 8192

4k: ${C68OBJS}
	@mkver -e "assembler -"
	${C68} -n ${C68FLAGS} -r ${C68OBJS} version.c -o as68.4k ${C68LIB}
	@setstack as68.4k 8192 8192

all: vax 4k

tags:
	ctags ${SOURCES} ${INCLUDES}

vaxobj/dir.o: dir.c
	${CC} ${CFLAGS} -c dir.c;mv -f dir.o vaxobj/dir.o

vaxobj/expr.o: expr.c
	${CC} ${CFLAGS} -c expr.c;mv -f expr.o vaxobj/expr.o

vaxobj/misc.o: misc.c
	${CC} ${CFLAGS} -c misc.c;mv -f misc.o vaxobj/misc.o

vaxobj/pass1a.o: pass1a.c
	${CC} ${CFLAGS} -c pass1a.c;mv -f pass1a.o vaxobj/pass1a.o

vaxobj/pass2.o: pass2.c
	${CC} ${CFLAGS} -c pass2.c;mv -f pass2.o vaxobj/pass2.o

vaxobj/symt.o: symt.c
	${CC} ${CFLAGS} -c symt.c;mv -f symt.o vaxobj/symt.o

vaxobj/main.o: main.c
	${CC} ${CFLAGS} -c main.c;mv -f main.o vaxobj/main.o

68obj/dir.o: dir.c
	${C68} ${C68FLAGS} -c dir.c;mv -f dir.o 68obj/dir.o

68obj/expr.o: expr.c
	${C68} ${C68FLAGS} -c expr.c;mv -f expr.o 68obj/expr.o

68obj/misc.o: misc.c
	${C68} ${C68FLAGS} -c misc.c;mv -f misc.o 68obj/misc.o

68obj/pass1a.o: pass1a.c
	${C68} ${C68FLAGS} -c pass1a.c;mv -f pass1a.o 68obj/pass1a.o

68obj/pass2.o: pass2.c
	${C68} ${C68FLAGS} -c pass2.c;mv -f pass2.o 68obj/pass2.o

68obj/symt.o: symt.c
	${C68} ${C68FLAGS} -c symt.c;mv -f symt.o 68obj/symt.o

68obj/main.o: main.c
	${C68} ${C68FLAGS} -c main.c;mv -f main.o 68obj/main.o

