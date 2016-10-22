top_srcdir=..

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent
include optimize.mak

all: aes.a aes.o

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

#.s.o:
#	$(AM_V_AS)oldmas -U -S $*.s

CPPFLAGS +=

STARTUP =	gemjstrt.o

aes.a: tosvars.o $(STARTUP) $(AES_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ tosvars.o $(STARTUP) $(AES_OBJS1)
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
gemjstrt.o: gemjstrt.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

apgrlib.o: apgrlib.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

apgsxif.o: apgsxif.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gemgrlib.o: gemgrlib.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gemgsxif.o: gemgsxif.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gsx2.o: gsx2.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jbind.o: jbind.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jdispa.o: jdispa.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jdos.o: jdos.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

large.o: large.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

optimize.o: optimize.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

ratrbp.o: ratrbp.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

trap14.o: trap14.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i



vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/commoon/config.h linea.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

aes.o: tosvars.o vars.o $(STARTUP) $(AES_OBJS) aesext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(aes_tbase) $(aes_dbase) 0 tosvars.o vars.o $(STARTUP) $(AES_OBJS) aesext.o

aesext.o: aesext.S $(top_srcdir)/commoon/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

##
#### Actions
##

RESOURCE =	glue.$(COUNTRY)

## -l generates too many symbols, so we use -s instead
##		aln -w -l -k gemstart -q -o $@
usa.prg: $(STARTUP) $(AESOBJ) $(DESKOBJ) $(RESOURCE) makefile
		aln -w -o $@ $(STARTUP) $(AESOBJ) $(DESKOBJ) \
			-i glue.usa _USARSC \
			-i glue.grm _GRMRSC \
			-i glue.fre _FRERSC \
			-i glue.ita _ITARSC \
			-i glue.swe _SWERSC \
			-i glue.spa _SPARSC


##
### AES dependencies
##

STDGEM.H = portab.h machine.h gem$(COUNTRY).h
GEMPD.H = struct88.h baspag88.h
GEMOLIB.H = obdefs.h gemlib.h

gemaints.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemaplib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) funcdef.h
gemasync.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gembase.oo:  $(STDGEM.H) struct88.h
gembind.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h crysbind.h gemdefn.h
gemcli.oo:   $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) gemdefn.h dos.h
gemctrl.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h funcdef.h
gemdisp.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) funcdef.h
gemevlib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemflag.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemfmalt.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h gemdefn.h 
gemfmlib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h gemdefn.h
gemfslib.oo: $(STDGEM.H) struct88.h $(GEMOLIB.H) taddr.h dos.h gemdefn.h
gemglobe.oo: $(STDGEM.H) struct88.h $(GEMOLIB.H) gemdefn.h
geminit.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h gemdefn.h
geminput.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) funcdef.h
gemmnlib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
gemobed.oo:  $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h gemdefn.h
gemobjop.oo: $(STDGEM.H) obdefs.h taddr.h
gemoblib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
gempd.oo:    $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemqueue.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemrom.oo:   $(STDGEM.H) obdefs.h taddr.h
gemrslib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
gemsclib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemshlib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h dos.h gemdefn.h
gemwmlib.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
gemrlist.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
gemwrect.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H)
romcart.oo:  $(STDGEM.H) dos.h
mn_event.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_mbar.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_menu.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_popup.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_submn.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_tools.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h
mn_index.oo: $(STDGEM.H) $(GEMPD.H) $(GEMOLIB.H) taddr.h

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
