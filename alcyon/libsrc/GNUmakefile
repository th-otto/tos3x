top_srcdir=..
subdir=libsrc

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../include -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all::

include SRCFILES

all:: ${LIBC} ${LIBM} ${OPTIONOBJS} ${CRT0}

${LIBC}: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

${LIBM}: $(MOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MOBJS)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a *.ndx
