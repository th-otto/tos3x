top_srcdir=..
subdir=cli

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = command.tos find.ttp

all: $(PROGRAMS)

include SRCFILES

command.tos: $(COMA_OBJS)
	$(AM_V_LD)$(LD) -nostdlib $(COMA_OBJS) $(LDFLAGS) -o $@ -lgcc

find.ttp: $(FIND_OBJS)
	$(AM_V_LD)$(LD) $(FIND_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i ../bin/$$i; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
