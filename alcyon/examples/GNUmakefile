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

PROGRAMS = test.prg apskel.prg doodle.app
LIBS = -lgem

all: $(PROGRAMS)

include SRCFILES

test.prg: $(TEST_OBJS)
	$(AM_V_LD)$(LD) $(TEST_OBJS) $(LDFLAGS) -o $@ $(LIBS)

apskel.prg: $(APSKEL_OBJS)
	$(AM_V_LD)$(LD) $(APSKEL_OBJS) $(LDFLAGS) -o $@ $(LIBS)

doodle.app: $(DOODLE_OBJS)
	$(AM_V_LD)$(LD) $(APSKEL_OBJS) $(LDFLAGS) -o $@ $(LIBS)

install: all
	@:

install64: all
	@:

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
