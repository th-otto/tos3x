top_srcdir=..
subdir=bdos

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CC += -mshort

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: $(top_srcdir)/common/config.h vars.o bdos.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(BDOS_OBJS)

$(BDOS_OBJS) vars.o vdidsp.o: $(top_srcdir)/common/config.h

check:: bdos.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(bdos_toff):0x24 --offsets=0x$(bdos_tbase):0 --bytes=0x$(bdos_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bdos.o
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(bdos_doff):0x$(bdos_tleno) --offsets=0x$(bdos_dbase):0x$(bdos_tlen) --bytes=0x$(bdos_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bdos.o

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
