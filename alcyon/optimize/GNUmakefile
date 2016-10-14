# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

PROGRAMS = optimize$(EXEEXT)

LIBS =

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all: $(PROGRAMS)

include SRCFILES

optimize$(EXEEXT): $(OPTIMIZE_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(OPTIMIZE_OBJS) ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
