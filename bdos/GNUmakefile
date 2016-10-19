top_srcdir=..
subdir=bdos

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

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
	$(AM_V_at)offset=`echo 16 i 10 o $(bdos_tbase) $(rombase) - p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x24 --offsets=0x$(bdos_tbase):0 --bytes=0x$(bdos_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<
	$(AM_V_at)offset=`echo 16 i 10 o $(bdos_dbase) $(rombase) - p | dc`; \
	offset2=`echo 16 i 10 o $(bdos_tlen) 24 + p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x$$offset2 --offsets=0x$(bdos_dbase):0x$(bdos_tlen) --bytes=0x$(bdos_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS) bdos.orig bdos.new $(top_srcdir)/common/config.h

distclean: clean
