top_srcdir=..
subdir=vdi

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: vdi.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

vdi.a: $(VDI_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS)

check:: vdi.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)if ! test -f $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img; then \
		echo "$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img: no such file" >&2; \
		exit 1; \
	fi
	$(AM_V_at)offset=`echo 16 i $(vdi_tbase) $(rombase) - p | dc`; \
	tlen=`echo 16 i $(vdi_tlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$tlen of=vdi.orig 2> /dev/null; \
	offset=`echo 16 i $(vdi_dbase) $(rombase) - p | dc`; \
	dlen=`echo 16 i $(vdi_dlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$dlen seek=$$tlen of=vdi.orig 2> /dev/null; \
	total=`echo $$tlen $$dlen + p | dc`; \
	dd if=$< of=vdi.new bs=1 skip=36 count=$$total 2> /dev/null; \
	cmp vdi.orig vdi.new; \
	status=$$?; \
	$(RM) vdi.orig vdi.new; \
	exit $$status

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
