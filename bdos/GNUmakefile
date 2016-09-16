# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent
CC += -mshort

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

include bdos.lng

all: vars.o bdos.a

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a $(PROGRAMS)
