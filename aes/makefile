#### makefile for final release TT AES/Desktop - 900731 kbad
##
## ttaes & desktop *.c *.h *.s must be in current directory,
## along with appropriate glue.XXX files.

V=0

AM_V_CC_0 = @echo CC $<; 
AM_V_CC_1 =
AM_V_CC = $(AM_V_CC_0)

AM_V_AS_0 = @echo AS $<; 
AM_V_AS_1 =
AM_V_AS = $(AM_V_AS_0)

AM_V_at_0 = @
AM_V_at_1 =
AM_V_at = $(AM_V_at_0)

##
#### Country definitions
## valid countries are: usa grm fre spa ita swe

COUNTRY = usa

##
#### Country #define - MUST BE UPPERCASE
## valid CDEFs are: USA UK GERMANY FRENCH SPAIN ITALY SWEDEN SWISSFRE
## SWISSGER 

CDEF = USA

## NOTE: When making UK AES, set COUNTRY to usa and CDEF to UK.
##       This will build a ttaesusa.o which must then be manually
##	 renamed to ttaesuk.o.
##	The same goes for SWISSGER and SWISSFRA -- use grm and fre,
##	and manually rename the ttaesxxx.o file that gets built.

##
#### Default actions:
##

.SUFFIXES: .oo .o .s .c
#.c.s:
#	cp68 -D$(CDEF)=1 $*.c $*.i
#	c068 $*.i $*.1 $*.2 $*.3 -f
#	c168 $*.1 $*.2 $*.s
#	$(RM) $*.s $*.i $*.1 $*.2 $*.3

.c.o:
	$(AM_V_CC)cp68 -D$(CDEF)=1 $*.c $*.i
	$(AM_V_at)c068 $*.i $*.1 $*.2 $*.3 -f
	$(AM_V_at)c168 $*.1 $*.2 $*.s
	$(AM_V_at)as68 -l -u $*.s
	$(AM_V_at)$(RM) $*.s $*.i $*.1 $*.2 $*.3

.c.oo:
	$(AM_V_CC)cp68 -D$(CDEF)=1 $*.c $*.i
	$(AM_V_at)c068 $*.i $*.1 $*.2 $*.3 -f
	$(AM_V_at)c168 $*.1 $*.2 $*.s
	$(AM_V_at)optimize $*.s > NUL:
	$(AM_V_at)as68 -l -u $*.s
	$(AM_V_at)$(RM) $*.s $*.i $*.1 $*.2 $*.3
	$(AM_V_at)mv $*.o $*.oo

.s.o:
	$(AM_V_AS)oldmas -U -S $*.s
##
#### Default dependencies & object equates
##

STDDESK.H =	portab.h mobdefs.h defines.h window.h \
		desk$(COUNTRY).h
GEMDESK.H =	extern.h osbind.h gemdefs.h

STDGEM.H =	portab.h machine.h gem$(COUNTRY).h
GEMPD.H =	struct88.h baspag88.h
GEMOLIB.H =	obdefs.h gemlib.h

RESOURCE =	glue.$(COUNTRY)

STARTUP =	gemjstrt.o

AESOBJ	=	gemaints.oo gemaplib.oo gemasync.oo gembase.oo  gembind.oo  \
		gemcli.oo   gemctrl.oo  gemdisp.oo  gemevlib.oo gemflag.oo  \
		gemfmalt.oo gemfmlib.oo gemfslib.oo gemglobe.oo geminit.oo  \
		geminput.oo gemmnlib.oo gemobed.oo  gemobjop.oo gempd.oo    \
		gemqueue.oo gemrom.oo   gemrslib.oo gemsclib.oo gemshlib.oo \
		gemwmlib.oo gemrlist.oo romcart.oo  gemoblib.oo \
		mn_mbar.oo  mn_menu.oo  mn_popup.oo mn_submn.oo \
		mn_tools.oo mn_index.oo mn_event.oo \
		apgrlib.o   apgsxif.o   gemgrlib.o  gemgsxif.o  \
		gsx2.o      jbind.o     jdispa.o    jdos.o      large.o    \
		optimize.o  ratrbp.o    trap14.o

# gemwrect.oo 

DESKOBJ1 =	deskact.o  deskapp.o  deskbind.o deskbutt.o deskdir.o  \
		deskdisk.o deskfile.o deskinf.o  deskins.o  deskmem.o  \
		deskmenu.o deskmisc.o deskopen.o deskpref.o desksear.o \
		desksele.o deskshel.o deskshow.o deskstor.o desktop.o  \
		deskupda.o deskwin.o  deskmn.o   deskif.o

DESKOBJ = $(DESKOBJ1) ../lib/osbind.o

##
#### Actions
##

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


## -l generates too many symbols, so we use -s instead
##		aln -h 7 -w -l -o $@ $(STARTUP) $(AESOBJ) ttdesk.o 
ttdesk.prg:	$(STARTUP) $(AESOBJ) $(DESKOBJ) $(RESOURCE) makefile
		aln -h 7 -s -w -o $@ \
		    $(STARTUP) $(AESOBJ) $(DESKOBJ) -i $(RESOURCE) _tosrsc



##
#### Desktop dependencies
##

deskact.o:  $(STDDESK.H) $(GEMDESK.H)
deskapp.o:  $(STDDESK.H) $(GEMDESK.H)
deskbind.o: portab.h
deskbutt.o: $(STDDESK.H) $(GEMDESK.H)
deskdir.o:  $(STDDESK.H) $(GEMDESK.H) error.h
deskdisk.o: $(STDDESK.H) $(GEMDESK.H)
deskfile.o: $(STDDESK.H) $(GEMDESK.H) error.h
deskinf.o:  $(STDDESK.H) $(GEMDESK.H)
deskins.o:  $(STDDESK.H) $(GEMDESK.H) error.h
deskmem.o:  $(STDDESK.H) extern.h osbind.h
deskmenu.o: $(STDDESK.H) $(GEMDESK.H)
deskmisc.o: $(STDDESK.H) $(GEMDESK.H) error.h
deskopen.o: $(STDDESK.H) $(GEMDESK.H)
deskpref.o: $(STDDESK.H) extern.h gemdefs.h
desksear.o: $(STDDESK.H) $(GEMDESK.H) error.h
desksele.o: $(STDDESK.H) $(GEMDESK.H)
deskshel.o: $(STDDESK.H) $(GEMDESK.H) error.h
deskshow.o: $(STDDESK.H) $(GEMDESK.H)
deskstor.o: $(STDDESK.H) osbind.h gemdefs.h
desktop.o:  $(STDDESK.H) $(GEMDESK.H)
deskupda.o: $(STDDESK.H) $(GEMDESK.H)
deskwin.o:  $(STDDESK.H) $(GEMDESK.H) error.h
deskmn.o:   $(STDDESK.H) $(GEMDESK.H)

##
### AES dependencies
##

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


clean::
	$(RM) $(STARTUP) $(DESKOBJ1) $(AESOBJ) ttdesk.prg
