include ../Makefile.common
include SRCFILES

PROGRAMS = cp68$(EXEEXT)

LIBS = $(LIBC)

all: $(PROGRAMS)

cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: all
	$(CP) cp68$(EXEEXT) $(BIN)

clean::
	$(RM) *.o $(PROGRAMS)
