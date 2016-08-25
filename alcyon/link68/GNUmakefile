# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = link68$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS)
	$(AM_V_LD)$(LD) $(LINK_OBJS) $(LDFLAGS) -o $@

check::

cp68.rel:: link68$(EXEEXT)
	for i in ../orig/libsrc/crtg.o ../orig/libsrc/libc.a ../orig/libsrc/kcreat.o ../orig/libsrc/kwritef.o \
		../orig/cp68/cexpr.o ../orig/cp68/lex.o ../orig/cp68/macro.o \
		../orig/cp68/main.o ../orig/cp68/util.o \
		../orig/cp68/kputc.o ../orig/cp68/kgetc.o ../orig/cp68/kopen.o ; do \
		cp $$i .; \
	done
	./link68$(EXEEXT) '[s,un]' $@=crtg.o,cexpr.o,lex.o,macro.o,main.o,util.o,kputc.o,kgetc.o,kopen.o,kcreat.o,kwritef.o,libc.a

clean:
	$(RM) *.o *.a $(PROGRAMS)
