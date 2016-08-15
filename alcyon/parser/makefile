CC = cc
C68 = nc68
SOURCES = decl.c expr.c icode.c init.c interf.c lex.c main.c stmt.c tabl.c \
			putexpr.c misc.c node.c symt.c tree.c
INCLUDES = parser.h ../icode.h
VAXOBJS = vaxobj/decl.o vaxobj/expr.o vaxobj/icode.o vaxobj/init.o \
		vaxobj/interf.o vaxobj/lex.o vaxobj/main.o vaxobj/stmt.o \
		vaxobj/tabl.o vaxobj/putexpr.o vaxobj/misc.o vaxobj/node.o \
		vaxobj/symt.o vaxobj/tree.o
C68OBJS = 68obj/decl.o 68obj/expr.o 68obj/icode.o 68obj/init.o \
		68obj/interf.o 68obj/lex.o 68obj/main.o 68obj/stmt.o \
		68obj/tabl.o 68obj/putexpr.o 68obj/misc.o 68obj/node.o \
		68obj/symt.o 68obj/tree.o
CFLAGS = -w -O -DVAX11 -DDEBUG
C68FLAGS = -L -r -f -DMC68000 -DDEBUG -t0 -t1
LIB = -lV6
C68LIB = -l6

vax: ${VAXOBJS}
	@mkver -e "parser -"
	${CC} ${CFLAGS} version.c ${VAXOBJS} -o c068.vax ${LIB}

c068: ${C68OBJS}
	@mkver -e "parser -"
	${C68} ${C68FLAGS} ${C68OBJS} version.c -o c068.68 ${C68LIB}
	@setstack c068.68 8192 8192

2k: ${C68OBJS}
	@mkver -e "parser -"
	${C68} -n2 ${C68FLAGS} -r ${C68OBJS} version.c -o c068.2k ${C68LIB}
	@setstack c068.2k 8192 8192

4k: ${C68OBJS}
	@mkver -e "parser -"
	${C68} -n ${C68FLAGS} -r ${C68OBJS} version.c -o c068.4k ${C68LIB}
	@setstack c068.4k 8192 8192

all: vax 4k

tags:
	ctags ${SOURCES} ${INCLUDES}

vaxobj/decl.o: decl.c
	${CC} ${CFLAGS} -c decl.c;mv -f decl.o vaxobj/decl.o

vaxobj/expr.o: expr.c
	${CC} ${CFLAGS} -c expr.c;mv -f expr.o vaxobj/expr.o

vaxobj/icode.o: icode.c
	${CC} ${CFLAGS} -c icode.c;mv -f icode.o vaxobj/icode.o

vaxobj/init.o: init.c
	${CC} ${CFLAGS} -c init.c;mv -f init.o vaxobj/init.o

vaxobj/interf.o: interf.c
	${CC} ${CFLAGS} -c interf.c;mv -f interf.o vaxobj/interf.o

vaxobj/lex.o: lex.c
	${CC} ${CFLAGS} -c lex.c;mv -f lex.o vaxobj/lex.o

vaxobj/main.o: main.c
	${CC} ${CFLAGS} -c main.c;mv -f main.o vaxobj/main.o

vaxobj/stmt.o: stmt.c
	${CC} ${CFLAGS} -c stmt.c;mv -f stmt.o vaxobj/stmt.o

vaxobj/tabl.o: tabl.c
	${CC} ${CFLAGS} -c tabl.c;mv -f tabl.o vaxobj/tabl.o

vaxobj/putexpr.o: putexpr.c
	${CC} ${CFLAGS} -c putexpr.c;mv -f putexpr.o vaxobj/putexpr.o

vaxobj/misc.o: misc.c
	${CC} ${CFLAGS} -c misc.c;mv -f misc.o vaxobj/misc.o

vaxobj/node.o: node.c
	${CC} ${CFLAGS} -c node.c;mv -f node.o vaxobj/node.o

vaxobj/symt.o: symt.c
	${CC} ${CFLAGS} -c symt.c;mv -f symt.o vaxobj/symt.o

vaxobj/tree.o: tree.c
	${CC} ${CFLAGS} -c tree.c;mv -f tree.o vaxobj/tree.o

68obj/decl.o: decl.c
	${C68} ${C68FLAGS} -c decl.c;mv -f decl.o 68obj/decl.o

68obj/expr.o: expr.c
	${C68} ${C68FLAGS} -c expr.c;mv -f expr.o 68obj/expr.o

68obj/icode.o: icode.c
	${C68} ${C68FLAGS} -c icode.c;mv -f icode.o 68obj/icode.o

68obj/init.o: init.c
	${C68} ${C68FLAGS} -c init.c;mv -f init.o 68obj/init.o

68obj/interf.o: interf.c
	${C68} ${C68FLAGS} -c interf.c;mv -f interf.o 68obj/interf.o

68obj/lex.o: lex.c
	${C68} ${C68FLAGS} -c lex.c;mv -f lex.o 68obj/lex.o

68obj/main.o: main.c
	${C68} ${C68FLAGS} -c main.c;mv -f main.o 68obj/main.o

68obj/stmt.o: stmt.c
	${C68} ${C68FLAGS} -c stmt.c;mv -f stmt.o 68obj/stmt.o

68obj/tabl.o: tabl.c
	${C68} ${C68FLAGS} -c tabl.c;mv -f tabl.o 68obj/tabl.o

68obj/putexpr.o: putexpr.c
	${C68} ${C68FLAGS} -c putexpr.c;mv -f putexpr.o 68obj/putexpr.o

68obj/misc.o: misc.c
	${C68} ${C68FLAGS} -c misc.c;mv -f misc.o 68obj/misc.o

68obj/node.o: node.c
	${C68} ${C68FLAGS} -c node.c;mv -f node.o 68obj/node.o

68obj/symt.o: symt.c
	${C68} ${C68FLAGS} -c symt.c;mv -f symt.o 68obj/symt.o

68obj/tree.o: tree.c
	${C68} ${C68FLAGS} -c tree.c;mv -f tree.o 68obj/tree.o


