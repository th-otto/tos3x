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

include ../language.mak

all: vars.o bdos.a

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

check:: bdos.o
	offset=`echo 16 i $(gemdos_tbase_de) E00000 - p | dc`; \
	tlen=`echo 16 i $(gemdos_tlen_de) p | dc`; \
	dd if=../roms/tos$(TOSVERSION)$(LANG).img bs=1 skip=$$offset count=$$tlen of=bdos.orig 2> /dev/null; \
	offset=`echo 16 i $(gemdos_dbase_de) E00000 - p | dc`; \
	dlen=`echo 16 i $(gemdos_dlen_de) p | dc`; \
	dd if=../roms/tos$(TOSVERSION)$(LANG).img bs=1 skip=$$offset count=$$dlen seek=$$tlen of=bdos.orig 2> /dev/null; \
	total=`echo $$tlen $$dlen + p | dc`; \
	dd if=$< of=bdos.new bs=1 skip=36 count=$$total 2> /dev/null; \
	cmp bdos.orig bdos.new; \
	status=$$?; \
	$(RM) bdos.orig bdos.new; \
	exit $$status

clean:
	$(RM) *.o *.a $(PROGRAMS) bdos.orig bdos.new
