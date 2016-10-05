include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: aes.a aes.o

include ../config.mak
include ../language.mak

include SRCFILES

CPPFLAGS +=

aes.a: tosvars.o $(AES_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ tosvars.o $(AES_OBJS1)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS2)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS3)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS4)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS5)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS6)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS7)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
vars.o: vars.S config.h linea.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

aes.o: checklang tosvars.o vars.o $(AES_OBJS) aesext.o ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(aes_tbase) $(aes_dbase) 0 tosvars.o vars.o $(AES_OBJS) aesext.o

aesext.o: aesext.S config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) config.h
