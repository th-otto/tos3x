top_srcdir=..

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

all: aes.a

include $(top_srcdir)/config.mak
include $(top_srcdir)/language.mak

include SRCFILES

aes.a: $(AES_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ $(AES_OBJS)

check:: aes.o checklang
	$(AM_V_at)if ! test -f $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img; then \
		echo "$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img: no such file" >&2; \
		exit 1; \
	fi
	$(AM_V_at)offset=`echo 16 i $(aes_tbase) $(rombase) - p | dc`; \
	tlen=`echo 16 i $(aes_tlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$tlen of=aes.orig 2> /dev/null; \
	offset=`echo 16 i $(aes_dbase) $(rombase) - p | dc`; \
	dlen=`echo 16 i $(aes_dlen) p | dc`; \
	dd if=$(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img bs=1 skip=$$offset count=$$dlen seek=$$tlen of=aes.orig 2> /dev/null; \
	total=`echo $$tlen $$dlen + p | dc`; \
	dd if=$< of=aes.new bs=1 skip=36 count=$$total 2> /dev/null; \
	cmp aes.orig aes.new; \
	status=$$?; \
	$(RM) aes.orig aes.new; \
	exit $$status

dist::
	$(CP) -a $(AES_SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/aes

clean:
	$(RM) *.o *.a $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
