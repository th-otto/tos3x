top_srcdir=..
subdir=gemlib

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

GEMLIB=gem.a
GEMLIB_NDX=gem.ndx

CPPFLAGS = -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all::

include SRCFILES

all:: ${GEMLIB} gemstart.o

${GEMLIB}: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx
