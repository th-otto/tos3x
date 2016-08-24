include ../GNUmakefile.common
include ../Makefile.silent
include SRCFILES

LIBS =

CPPFLAGS = -I../common
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cp68$(EXEEXT)

all: $(PROGRAMS)

cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: all
	$(CP) cp68$(EXEEXT) $(BIN)

clean::
	$(RM) *.o $(PROGRAMS)
