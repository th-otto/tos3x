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

all: bdos.a

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
