top_srcdir=..
subdir=system

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cache100.prg foldr100.prg vdi_fix.prg

all:: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE100_OBJS) ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} cache100.rel=${CACHE100_OBJS}
	$(AM_V_at)${RELMOD} cache100.rel $@
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${CACHE100_OBJS}
endif

foldr100.prg: $(FOLDR100_OBJS) ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} foldr100.rel=${FOLDR100_OBJS}
	$(AM_V_at)${RELMOD} foldr100.rel $@
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${FOLDR100_OBJS}
endif

vdi_fix.prg: $(VDI_FIX_OBJS) ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} vdi_fix.rel=${VDI_FIX_OBJS}
	$(AM_V_at)${RELMOD} vdi_fix.rel $@
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${VDI_FIX_OBJS}
endif

vdi_fix.o: vdifix11/vdi_fix.S
	$(AM_V_AS)$(CC) -Wa,--mri $(ASFLAGS) -n $(CPPFLAGS) -c -o $@ $<

cacheold.prg: $(CACHE100_OLD_OBJS) ${MAKEFILE}
ifeq ($(USE_CROSS_ALCYON),1)
	$(AM_V_LD)${LINK68} cacheold.rel=${CACHE100_OLD_OBJS}
	$(AM_V_at)${RELMOD} cacheold.rel $@
else
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${CACHE100_OLD_OBJS}
endif

check::
	@:

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR1)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

dosdir::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(DOSDIR)/$(subdir)

clean::
	$(RM) *.o *.a *.i *.1 *.2 *.rel $(PROGRAMS)
