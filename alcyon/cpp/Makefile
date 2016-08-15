include ../Makefile.common
include SRCFILES

LIB =

all: $(BIN)cp68$(EXEEXT)


$(BIN)cp68$(EXEEXT): ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIB} -o $@


clean::
	$(RM) *.o
