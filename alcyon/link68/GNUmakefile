# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = link68$(EXEEXT) relmod$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS)
	$(AM_V_LD)$(LD) $(LINK_OBJS) $(LDFLAGS) -o $@

relmod$(EXEEXT): $(RELMOD_OBJS)
	$(AM_V_LD)$(LD) $(RELMOD_OBJS) $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

cp68.rel:: link68$(EXEEXT)
	for i in ../orig/libsrc/crtg.o ../orig/libsrc/libc.a ../orig/libsrc/kcreat.o ../orig/libsrc/kwritef.o \
		../orig/cp68/cexpr.o ../orig/cp68/lex.o ../orig/cp68/macro.o \
		../orig/cp68/main.o ../orig/cp68/util.o \
		../orig/cp68/kputc.o ../orig/cp68/kgetc.o ../orig/cp68/kopen.o ; do \
		cp $$i .; \
	done
	./link68$(EXEEXT) '[s,un]' $@=crtg.o,cexpr.o,lex.o,macro.o,main.o,util.o,kputc.o,kgetc.o,kopen.o,kcreat.o,kwritef.o,libc.a

link68.rel::
	cp ../libsrc/crt0.o .
	cp ../libsrc/libc.a .
	cp ../libsrc/nofloat.o .
	./link68$(EXEEXT) '[s]' $@=crt0.o,nofloat.o,link68.o,preproc.o,lnkmess.o,libc.a

clean:
	$(RM) *.o *.a $(PROGRAMS)
