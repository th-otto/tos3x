top_srcdir=..
subdir=desk

# 
# cross compiling
#
EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))

include $(top_srcdir)/GNUmakefile.common
include $(top_srcdir)/Makefile.silent

CC += -mshort

LIBS =

CPPFLAGS = -I$(top_srcdir)/common -I$(INC)
CFLAGS = $(OPTS) $(WARN)

all: desk.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

desk.a: $(DESK_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS)

check::
	$(AM_V_at)offset=`echo 16 i 10 o $(desk_tbase) $(rombase) - p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x24 --offsets=0x$(desk_tbase):0 --bytes=0x$(desk_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img desk.o; \
	status=$$?; \
	test $$status != 0 && echo "data:"; \
	offset=`echo 16 i 10 o $(desk_dbase) $(rombase) - p | dc`; \
	offset2=`echo 16 i 10 o $(desk_tlen) 24 + p | dc`; \
	$(CMP) $(CMPL) -x --ignore-initial=0x$$offset:0x$$offset2 --offsets=0x$(desk_dbase):0x$(desk_tlen) --bytes=0x$(desk_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img desk.o; \
	status2=$$?; \
	test $$status == 0 -a $$status2 == 0

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
