top_srcdir=..
subdir=util

# 
# cross compiling
#
#EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
#CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common
#CPPFLAGS += -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT) ar68$(EXEEXT) analyze$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(SIZE_OBJS) $(LDFLAGS) -o $@ $(LIBS)

nm68$(EXEEXT): $(NM_OBJS)
	$(AM_V_LD)$(LD) $(NM_OBJS) $(LDFLAGS) -o $@ $(LIBS)

ar68$(EXEEXT): $(AR_OBJS)
	$(AM_V_LD)$(LD) $(AR_OBJS) $(LDFLAGS) -o $@ $(LIBS)

analyze$(EXEEXT): $(ANALYZE_OBJS)
	$(AM_V_LD)$(LD) $(ANALYZE_OBJS) $(LDFLAGS) -o $@ $(LIBS)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
