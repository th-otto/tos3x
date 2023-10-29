top_srcdir=..
subdir=lo68

# 
# cross compiling
#
include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CPPFLAGS = -I ../common -DGEMDOS
#CPPFLAGS += -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = lo68$(EXEEXT) reloc$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

lo68$(EXEEXT): $(LO68_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(LO68_OBJS) $(LDFLAGS) $(GLIBC_SO) -o $@

reloc$(EXEEXT): $(RELOC_OBJS) ${MAKEFILE}
	$(AM_V_LD)$(LD) $(RELOC_OBJS) $(LDFLAGS) $(GLIBC_SO) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

crt0.o: crt0.S

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
