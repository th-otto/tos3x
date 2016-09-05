# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

PROGRAMS = as68$(EXEEXT)

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: $(PROGRAMS)

include SRCFILES

as68$(EXEEXT): $(AS68_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(AS68_OBJS) ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
