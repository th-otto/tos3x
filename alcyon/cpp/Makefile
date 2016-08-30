include ../Makefile.common
include SRCFILES

PROGRAMS = cp68$(EXEEXT)

LIBS = $(LIBC)

all: $(PROGRAMS)

cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${LD} ${LDFLAGS} -u -h 7 -o $@ $(CRT0) nofloat.o ${OBJS} ${LIBS}

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean::
	$(RM) *.o $(PROGRAMS)
