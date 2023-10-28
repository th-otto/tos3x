top_srcdir=..
subdir=lo68

# 
# cross compiling
#
include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CPPFLAGS = -I ../include -DGEMDOS
#CPPFLAGS += -I $(INC)
CFLAGS = $(OPTS) $(WARN)
LIBS = ../lib/libc.a

PROGRAMS = lo68$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

lo68$(EXEEXT): $(LO68_OBJS) ${MAKEFILE}
#	$(AM_V_LD)$(LD) $(LO68_OBJS) $(LDFLAGS) $(GLIBC_SO) -o $@
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@ $(CRT0) ../lib/nofloat.o $(LO68_OBJS) $(LIBS)

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
