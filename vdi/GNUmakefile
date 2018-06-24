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

vdi.a: $(VDI_$(TOSVERSION)_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(VDI_$(TOSVERSION)_OBJS)

#vdi.o: tosvars.o vars.o $(AES_OBJS) vdiext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
#	$(LD) -nostartfiles -nostdlib -o $@ -Wl,--script=../common/rom.ld,-Ttext=0x$(rombase),-Tbss=0 $(VDI_OBJS) tosvars.o vars.o vdiext.o -lgcc

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(CC) -Wa,--mri --traditional-cpp $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(CC) -Wa,--mri --traditional-cpp $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

check:: vdi.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(vdi_toff):0x24 --offsets=0x$(vdi_tbase):0 --bytes=0x$(vdi_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img vdi.o
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(vdi_doff):0x$(vdi_tleno) --offsets=0x$(vdi_dbase):0x$(vdi_tlen) --bytes=0x$(vdi_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img vdi.o
	

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
