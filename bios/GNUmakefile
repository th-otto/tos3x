# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

top_srcdir=..

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

CC += -mshort

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: bios.a

include $(top_srcdir)/config.mak
include $(top_srcdir)/language.mak

include SRCFILES

bios.a: $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BIOS_OBJS)

startup.o: startup.S $(top_srcdir)/common/config.h header.h scrdmp.inc mmu030.inc romcrc.inc dma.inc nvram.inc ikbdclock.inc floppy.inc

keytbl.o: $(top_srcdir)/common/config.h

prtkeys$(EXEEXT): prtkeys.o
	$(AM_V_LD)${LD} ${LDFLAGS} -o $@ prtkeys.o

check:: bios.o checklang
	$(AM_V_at)if ! test -f $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img; then \
		echo "$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img: no such file" >&2; \
		exit 1; \
	fi
	$(AM_V_at)offset=`echo 16 i $(bios_tbase) $(rombase) - p | dc`; \
	tlen=`echo 16 i $(bios_tlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$tlen of=bios.orig 2> /dev/null; \
	offset=`echo 16 i $(bios_dbase) $(rombase) - p | dc`; \
	dlen=`echo 16 i $(bios_dlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$dlen seek=$$tlen of=bios.orig 2> /dev/null; \
	total=`echo $$tlen $$dlen + p | dc`; \
	dd if=$< of=bios.new bs=1 skip=36 count=$$total 2> /dev/null; \
	cmp bios.orig bios.new; \
	status=$$?; \
	$(RM) bios.orig bios.new; \
	exit $$status

dist::
	$(CP) -a $(BIOS_SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/bios

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
