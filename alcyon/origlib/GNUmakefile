# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../include -I $(INC)
CFLAGS = $(OPTS) $(WARN)

CRT0O=crt0.o
LIBC=libc.a

all: ${LIBC} options.a ${CRT0O} srcfiles.sh

include SRCFILES

${LIBC}: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

srcfiles.sh: SRCFILES ${MAKEFILE}
	$(AM_V_GEN)
	@echo "#!mupfel"            > $@
	@echo AOBJS1=\"${AOBJS1}\" >> $@
	@echo AOBJS2=\"${AOBJS2}\" >> $@
	@echo COBJS1=\"${COBJS1}\" >> $@
	@echo COBJS2=\"${COBJS2}\" >> $@
	@echo COBJS3=\"${COBJS3}\" >> $@
	@echo COBJS4=\"${COBJS4}\" >> $@
	@echo COBJS5=\"${COBJS5}\" >> $@
	@echo COBJS6=\"${COBJS6}\" >> $@
	@echo COBJS7=\"${COBJS7}\" >> $@
	@echo COBJS8=\"${COBJS8}\" >> $@
	@echo OPTIONOBJS=\"${OPTIONOBJS}\" >> $@
	@echo CRT0O=\"${CRT0O}\" >> $@
	@echo LIBC=\"${LIBC}\" >> $@
	@echo LIBO=\"${LIBO}\" >> $@

clean:
	$(RM) *.o *.a *.ndx

echo:
	echo $(COBJS:.o=.c)
