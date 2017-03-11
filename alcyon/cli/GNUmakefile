top_srcdir=..
subdir=cli

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = command.tos

all: $(PROGRAMS)

include SRCFILES

command.tos: $(COMA_OBJS) gemdos.o bios.o
	$(AM_V_LD)$(LD) -nostdlib $^ $(LDFLAGS) -o $@ -lgcc

gemdos.o: ../libsrc/gemdos.S
	$(AM_V_AS)$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

bios.o: ../libsrc/bios.S
	$(AM_V_AS)$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i ../bin/$$i; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
