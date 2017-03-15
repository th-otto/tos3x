top_srcdir=..
subdir=examples

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = test.prg apskel.prg
LIBS = -lgem

all: $(PROGRAMS)

include SRCFILES

test.prg: $(TEST_OBJS)
	$(AM_V_LD)$(LD) $(TEST_OBJS) $(LDFLAGS) -o $@ $(LIBS)

apskel.prg: $(APSKEL_OBJS)
	$(AM_V_LD)$(LD) $(APSKEL_OBJS) $(LDFLAGS) -o $@ $(LIBS)

install: all
	@:

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
