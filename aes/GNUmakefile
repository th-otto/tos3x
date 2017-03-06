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

all: aes.a aes.o

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

STARTUP =	gemjstrt.o

aes.a: $(STARTUP) $(AES_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(AES_OBJS)

aes.o: vars.o $(STARTUP) $(AES_OBJS) aesext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	$(LD) -nostartfiles -nostdlib -o $@ -Wl,--script=../common/rom.ld,-Ttext=0x$(rombase),-Tbss=0 $(STARTUP) $(AES_OBJS) aesext.o -lgcc

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)$(CC) -Wa,--mri --traditional-cpp $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

check::
	$(AM_V_at)offset=`echo 16 i 10 o $(aes_tbase) $(rombase) - p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x24 --offsets=0x$(aes_tbase):0 --bytes=0x$(aes_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img aes.o; \
	status=$$?; \
	test $$status != 0 && echo "data:"; \
	offset=`echo 16 i 10 o $(aes_dbase) $(rombase) - p | dc`; \
	offset2=`echo 16 i 10 o $(aes_tlen) 24 + p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x$$offset2 --offsets=0x$(aes_dbase):0x$(aes_tlen) --bytes=0x$(aes_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img aes.o; \
	status2=$$?; \
	test $$status == 0 -a $$status2 == 0

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
