include ../GNUmakefile.common
include ../Makefile.silent
include SRCFILES

LIB =

CPPFLAGS = -I../common
CFLAGS = $(OPTS) $(WARN)

all: $(BIN)cp68$(EXEEXT)


$(BIN)cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${CC} ${CFLAGS} ${OBJS} ${LIB} -o $@


clean::
	$(RM) *.o $(BIN)cp68$(EXEEXT)
