top_srcdir=..
subdir=vdi

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

CC += -mshort

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: vdi.a vdi.o

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

vdi.a: $(VDI_$(TOSVERSION)_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(VDI_$(TOSVERSION)_OBJS)

vdi.o: vars.o $(AES_OBJS) vdiext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	$(LD) -nostartfiles -nostdlib -o $@ -Wl,--script=../common/rom.ld,-Ttext=0x$(rombase),-Tbss=0 $(VDI_OBJS) vars.o vdiext.o -lgcc

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(CC) -Wa,--mri --traditional-cpp $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

check:: vdi.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)offset=`echo 16 i 10 o $(vdi_tbase) $(rombase) - p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x24 --offsets=0x$(vdi_tbase):0 --bytes=0x$(vdi_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<; \
	status=$$?; \
	offset=`echo 16 i 10 o $(vdi_dbase) $(rombase) - p | dc`; \
	offset2=`echo 16 i 10 o $(vdi_tlen) 24 + p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x$$offset2 --offsets=0x$(vdi_dbase):0x$(vdi_tlen) --bytes=0x$(vdi_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img $<; \
	status2=$$?; \
	test $$status == 0 -a $$status2 == 0
	

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
