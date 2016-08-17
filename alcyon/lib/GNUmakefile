# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../include -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all::

include SRCFILES

all:: ${LIBM} ${OPTIONOBJS} ${CRT0}

${LIBM}: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(RM) *.o *.a *.ndx

echo:
	echo $(COBJS:.o=.c)
