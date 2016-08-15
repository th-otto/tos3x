CC = cc
C68 = nc68
VAXOBJS = vaxobj/canon.o \
		vaxobj/codegen.o \
		vaxobj/interf.o \
		vaxobj/main.o \
		vaxobj/optab.o \
		vaxobj/putexpr.o \
		vaxobj/smatch.o \
		vaxobj/sucomp.o \
		vaxobj/tabl.o \
		vaxobj/util.o \
		vaxobj/cskels.o
C68OBJS = 68obj/canon.o \
		68obj/codegen.o \
		68obj/interf.o \
		68obj/main.o \
		68obj/optab.o \
		68obj/putexpr.o \
		68obj/smatch.o \
		68obj/sucomp.o \
		68obj/tabl.o \
		68obj/util.o \
		68obj/cskels.o
CFLAGS = -O -w -DVAX11 -DDEBUG
C68FLAGS = -L -r -DMC68000 -DDEBUG -t0 -t1
LIB = -lV6
C68LIB = -l6
.IGNORE:

vax: ${VAXOBJS}
	mkver -e "Code Generator -"
	${CC} ${CFLAGS} version.c ${VAXOBJS} -o c168.vax ${LIB}

c168: ${C68OBJS}
	mkver -e "Code Generator -"
	${C68} -r ${C68FLAGS} version.c ${C68OBJS} -o c168.68 ${C68LIB}
	setstack c168.68 8000 8000

2k: ${C68OBJS}
	mkver -e "Code Generator -"
	${C68} -n2 -r ${C68FLAGS} version.c ${C68OBJS} -o c168.2k ${C68LIB}
	setstack c168.2k 8000 8000

4k: ${C68OBJS}
	mkver -e "Code Generator -"
	${C68} -n -r ${C68FLAGS} version.c ${C68OBJS} -o c168.4k ${C68LIB}
	setstack c168.4k 8000 8000

all: vax 4k

vaxobj/canon.o: canon.c
	${CC} ${CFLAGS} -c canon.c;mv -f canon.o vaxobj/canon.o

vaxobj/codegen.o: codegen.c
	${CC} ${CFLAGS} -c codegen.c;mv -f codegen.o vaxobj/codegen.o

vaxobj/interf.o: interf.c
	${CC} ${CFLAGS} -c interf.c;mv -f interf.o vaxobj/interf.o

vaxobj/main.o: main.c
	${CC} ${CFLAGS} -c main.c;mv -f main.o vaxobj/main.o

vaxobj/optab.o: optab.c
	${CC} ${CFLAGS} -c optab.c;mv -f optab.o vaxobj/optab.o

vaxobj/putexpr.o: putexpr.c
	${CC} ${CFLAGS} -c putexpr.c;mv -f putexpr.o vaxobj/putexpr.o

vaxobj/smatch.o: smatch.c
	${CC} ${CFLAGS} -c smatch.c;mv -f smatch.o vaxobj/smatch.o

vaxobj/sucomp.o: sucomp.c
	${CC} ${CFLAGS} -c sucomp.c;mv -f sucomp.o vaxobj/sucomp.o

vaxobj/tabl.o: tabl.c
	${CC} ${CFLAGS} -c tabl.c;mv -f tabl.o vaxobj/tabl.o

vaxobj/util.o: util.c
	${CC} ${CFLAGS} -c util.c;mv -f util.o vaxobj/util.o

vaxobj/cskels.o: cskels.c
	${CC} ${CFLAGS} -c cskels.c;mv -f cskels.o vaxobj/cskels.o

68obj/canon.o: canon.c
	${C68} ${C68FLAGS} -c canon.c;mv -f canon.o 68obj/canon.o

68obj/codegen.o: codegen.c
	${C68} ${C68FLAGS} -c codegen.c;mv -f codegen.o 68obj/codegen.o

68obj/interf.o: interf.c
	${C68} ${C68FLAGS} -c interf.c;mv -f interf.o 68obj/interf.o

68obj/main.o: main.c
	${C68} ${C68FLAGS} -c main.c;mv -f main.o 68obj/main.o

68obj/optab.o: optab.c
	${C68} ${C68FLAGS} -c optab.c;mv -f optab.o 68obj/optab.o

68obj/putexpr.o: putexpr.c
	${C68} ${C68FLAGS} -c putexpr.c;mv -f putexpr.o 68obj/putexpr.o

68obj/smatch.o: smatch.c
	${C68} ${C68FLAGS} -c smatch.c;mv -f smatch.o 68obj/smatch.o

68obj/sucomp.o: sucomp.c
	${C68} ${C68FLAGS} -c sucomp.c;mv -f sucomp.o 68obj/sucomp.o

68obj/tabl.o: tabl.c
	${C68} ${C68FLAGS} -c tabl.c;mv -f tabl.o 68obj/tabl.o

68obj/util.o: util.c
	${C68} ${C68FLAGS} -c util.c;mv -f util.o 68obj/util.o

68obj/cskels.o: cskels.c
	${C68} ${C68FLAGS} -c cskels.c;mv -f cskels.o 68obj/cskels.o
