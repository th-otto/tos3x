CC = c68
CFLAGS = -a

all: 
	${CC} ${CFLAGS} test.c -o test -l6
	${CC} ${CFLAGS} test2.c -o test2
	${CC} ${CFLAGS} test3.c -o test3
	${CC} ${CFLAGS} -e fptest.c -o etest
	${CC} ${CFLAGS} -f fptest.c -o ftest

ar:
	rm -f t.a
	ar68 rv t.a test test2 test3 etest ftest
	
clean: 
	@rm -f test test2 test3 ftest etest
	@rm -f t.a core a.out c.out
