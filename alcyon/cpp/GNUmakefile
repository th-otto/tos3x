top_srcdir=..
subdir=cpp

include ../GNUmakefile.common
include ../Makefile.silent
include SRCFILES

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cp68$(EXEEXT)

all: $(PROGRAMS)

cp68$(EXEEXT): ${OBJS}
	$(AM_V_LD)${CC} ${CFLAGS} ${OBJS} ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) cp68$(EXEEXT) $(BIN)

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean::
	$(RM) *.o $(PROGRAMS)
