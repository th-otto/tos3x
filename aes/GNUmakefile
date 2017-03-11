top_srcdir=..
subdir=aes

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CC += -mshort

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: aes.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

STARTUP =	gemjstrt.o

aes.a: $(STARTUP) $(AES_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(AES_OBJS)

#aes.o: vars.o $(STARTUP) $(AES_OBJS) aesext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
#	$(LD) -nostartfiles -nostdlib -o $@ -Wl,--script=../common/rom.ld,-Ttext=0x$(rombase),-Tbss=0 $(STARTUP) $(AES_OBJS) aesext.o -lgcc

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(CC) -Wa,--mri --traditional-cpp $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

check:: aes.o $(top_srcdir)/common/sections.mak
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(aes_toff):0x24 --offsets=0x$(aes_tbase):0 --bytes=0x$(aes_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img aes.o
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(aes_doff):0x$(aes_tleno) --offsets=0x$(aes_dbase):0x$(aes_tlen) --bytes=0x$(aes_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img aes.o

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
