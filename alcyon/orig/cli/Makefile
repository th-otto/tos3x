include ../Makefile.cmn
include ../Makefile.sil

PROGRAMS = command.tos

all: $(PROGRAMS)

include SRCFILES
include optimize.mak

command.rel: $(COMA_OBJS) $(LIB1OBJS) ${MAKEFILE}
	$(RM) $@
	$(LINK68) $@=coma.o,command.o,lmul.o

command.tos: command.rel
	$(RELMOD) $< $@
	$(PRGFLAGS) 7 7 $@

lmul.o:	../libsrc/lmul.o
	$(CP) $< $@

lrem.o:	../libsrc/lrem.o
	$(CP) $< $@

ldiv.o:	../libsrc/ldiv.o
	$(CP) $< $@

check:: $(PROGRAMS)
	cmp command.tos ${BIN}command.tos

clean:
	$(RM) *.o *.a command.s *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
