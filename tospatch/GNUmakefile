top_srcdir=..
subdir=tospatch

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

PROGRAMS = tospatch.ttp tosload.prg

all:: $(PROGRAMS)

include SRCFILES

#SYMBOLS = '[symbols,locals]'

tospatch.ttp: tospatch.o ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} $(SYMBOLS) tospatch.rel=tospatch.o
	$(AM_V_at)${RELMOD} tospatch.rel $@
	@$(RM) tospatch.rel
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ tospatch
endif

tospatch.o: tospatch.S GNUmakefile

tosload.prg: tosload.o ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} $(SYMBOLS) tosload.rel=tosload.o
	$(AM_V_at)${RELMOD} tosload.rel $@
	@$(RM) tosload.rel
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ tosload
endif

tosload.o: tosload.S GNUmakefile

check::
	@:

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel $(PROGRAMS)

distclean: clean
	@:
