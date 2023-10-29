top_srcdir=..
subdir=cpp

include ../GNUmakefile.cmn
include ../Makefile.sil
include SRCFILES

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)
LDFLAGS += -s

PROGRAMS = cp68$(EXEEXT)

all: $(PROGRAMS)

cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${CC} ${CFLAGS} ${OBJS} ${LIBS} $(GLIBC_SO) $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean::
	$(RM) *.o $(PROGRAMS)
