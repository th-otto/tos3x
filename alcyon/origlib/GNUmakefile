# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../include -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all: libc.a options.a crt0.o

include SRCFILES

libc.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

clean:
	$(RM) *.o *.a *.ndx

echo:
	echo $(COBJS:.o=.c)
