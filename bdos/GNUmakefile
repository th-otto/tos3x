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

all: config.h vars.o bdos.a

include ../config.mak
include ../language.mak

include SRCFILES

bdos.a: checklang $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

check:: bdos.o checklang
	$(AM_V_at)if ! test -f ../roms/tos$(TOSVERSION)$(COUNTRY).img; then \
		echo "../roms/tos$(TOSVERSION)$(COUNTRY).img: no such file" >&2; \
		exit 1; \
	fi
	$(AM_V_at)offset=`echo 16 i $(gemdos_tbase) $(rombase) - p | dc`; \
	tlen=`echo 16 i $(gemdos_tlen) p | dc`; \
	dd if=../roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$tlen of=bdos.orig 2> /dev/null; \
	offset=`echo 16 i $(gemdos_dbase) $(rombase) - p | dc`; \
	dlen=`echo 16 i $(gemdos_dlen) p | dc`; \
	dd if=../roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$dlen seek=$$tlen of=bdos.orig 2> /dev/null; \
	total=`echo $$tlen $$dlen + p | dc`; \
	dd if=$< of=bdos.new bs=1 skip=36 count=$$total 2> /dev/null; \
	cmp bdos.orig bdos.new; \
	status=$$?; \
	$(RM) bdos.orig bdos.new; \
	exit $$status

dist::
	$(CP) -a $(BDOS_SRCS) GNUmakefile Makefile SRCFILES optimize.mak tos306de.map tos306us.map ../$(DISTDIR)/bdos

clean:
	$(RM) *.o *.a $(PROGRAMS) bdos.orig bdos.new
