top_srcdir=../..
subdir=tospatch/src

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)
ifeq ($(USE_CROSS_ALCYON),0)
CFLAGS += -mno-short
endif

PROGRAMS = tospatch.ttp loadtos.prg romdump.tos

all:: $(PROGRAMS)

include SRCFILES

#ASYMBOLS := '[symbols,locals]'

tospatch.ttp: tospatch.o ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} $(ASYMBOLS) tospatch.rel=tospatch.o
	$(AM_V_at)${RELMOD} tospatch.rel $@
	@$(RM) tospatch.rel
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ tospatch.o
endif

tospatch.o: tospatch.S GNUmakefile

loadtos.prg: loadtos.o ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} $(ASYMBOLS) loadtos.rel=loadtos.o
	$(AM_V_at)${RELMOD} loadtos.rel $@
	@$(RM) loadtos.rel
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ loadtos.o
endif

loadtos.o: loadtos.S GNUmakefile

romdump.tos: romdump.o ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} $(ASYMBOLS) romdump.rel=$(LIB)$(CRT0),romdump.o,$(LIB)$(LIBC)
	$(AM_V_at)${RELMOD} romdump.rel $@
	@$(RM) romdump.rel
else
	$(AM_V_LD)${LD} ${LDFLAGS} -s -o $@ romdump.o
endif

romdump.o: romdump.c GNUmakefile

check::
	@:

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel $(PROGRAMS)

distclean: clean
	@:
