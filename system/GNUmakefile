top_srcdir=..
subdir=system

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil
CC += -mshort

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cache100.prg foldr100.prg

all: $(PROGRAMS)

include SRCFILES

cache100.prg: $(CACHE100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${CACHE100_OBJS}

foldr100.prg: $(FOLDR100_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${FOLDR100_OBJS}

cacheold.prg: $(CACHE100_OLD_OBJS) ${MAKEFILE}
	$(AM_V_LD)${LD} ${LDFLAGS} -nostdlib -nostartfiles -s -o $@ ${CACHE100_OLD_OBJS}

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
