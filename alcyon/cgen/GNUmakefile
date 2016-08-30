# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

PROGRAMS = c168$(EXEEXT)

LIBS =

CPPFLAGS = -I../common -I ../include
CFLAGS = $(OPTS) $(WARN)

all: $(PROGRAMS)

include SRCFILES

c168$(EXEEXT): $(C168_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(C168_OBJS) ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
