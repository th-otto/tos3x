# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent
CC += -mshort

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cache100.prg

all: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${CACHE_OBJS}

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
