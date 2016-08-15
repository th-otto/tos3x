#### makefile for final release TT AES/Desktop - 900731 kbad
##
## ttaes & desktop *.c *.h *.s must be in current directory,
## along with appropriate glue.XXX files.

# nasty workaround to avoid backslash at end-of-line
BS=\$(empty)

BIN=$(ALCYON)\bin$(BS)
LIB=$(ALCYON)\lib$(BS)
INC=$(ALCYON)\include$(BS)
EXEEXT=.ttp

V=0

AM_V_CC_0 = @echo CC $<; 
AM_V_CC_1 =
AM_V_CC = $(AM_V_CC_$(V))

AM_V_AS_0 = @echo AS $<; 
AM_V_AS_1 =
AM_V_AS = $(AM_V_AS_$(V))

AM_V_at_0 = @
AM_V_at_1 =
AM_V_at = $(AM_V_at_$(V))

##
#### Country definitions
## valid countries are: usa grm fre spa ita swe

COUNTRY = usa

##
#### Country #define - MUST BE UPPERCASE
## valid CDEFs are: USA UK GERMANY FRENCH SPAIN ITALY SWEDEN SWISSFRE
## SWISSGER 

CDEF_usa = USA
CDEF_uk = UK
CDEF_grm = GERMANY
CDEF_fre = FRENCH
CDEF_spa = SPAIN
CDEF_ita = ITALY
CDEF_swe = SWEDEN

CDEF = $(CDEF_$(COUNTRY))

CFLAGS= -f
AFLAGS = -u -L -s $(LIB)
CPPFLAGS = -I $(INC) -D$(CDEF)=1

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
	$(AM_V_CC)$(BIN)cp68$(EXEEXT) ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)$(BIN)c068$(EXEEXT) $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)$(BIN)c168$(EXEEXT) $*.1 $*.2 $*.s
	$(AM_V_at)$(BIN)as68$(EXEEXT) ${AFLAGS} $*.s
	$(AM_V_at)$(RM) $*.s $*.i $*.1 $*.2 $*.3

.c.oo:
	$(AM_V_CC)$(BIN)cp68$(EXEEXT) ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)$(BIN)c068$(EXEEXT) $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)$(BIN)c168$(EXEEXT) $*.1 $*.2 $*.s
	$(AM_V_at)$(BIN)optimize$(EXEEXT) $*.s > NUL:
	$(AM_V_at)$(BIN)as68$(EXEEXT) ${AFLAGS} $*.s
	$(AM_V_at)$(RM) $*.s $*.i $*.1 $*.2 $*.3
	$(AM_V_at)mv $*.o $*.oo

.s.o:
	$(AM_V_AS)$(BIN)oldmas -U -S $*.s
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
CRT0 = 		
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

DESKOBJ = $(DESKOBJ1) $(ALCYON)/lib/osbind.o

##
#### Actions
##

all:: usa.prg


## -l generates too many symbols, so we use -s instead
##		aln -w -l -k gemstart -q -o $@
usa.prg: $(STARTUP) $(AESOBJ) $(DESKOBJ) $(RESOURCE) makefile
		$(BIN)aln -w -o $@ $(STARTUP) $(AESOBJ) $(DESKOBJ) \
			-i glue.usa _USARSC \
			-i glue.grm _GRMRSC \
			-i glue.fre _FRERSC \
			-i glue.ita _ITARSC \
			-i glue.swe _SWERSC \
			-i glue.spa _SPARSC


## -l generates too many symbols, so we use -s instead
##		aln -h 7 -w -l -o $@ $(STARTUP) $(AESOBJ) ttdesk.o 
ttdesk.prg:	$(STARTUP) $(AESOBJ) $(DESKOBJ) $(RESOURCE) makefile
		$(BIN)aln -h 7 -s -w -o $@ \
		    $(STARTUP) $(AESOBJ) $(DESKOBJ) -i $(RESOURCE) _tosrsc


newglue.tos: newglue.o
	$(BIN)aln -y $(LIB) -o $@ $(CRT0) newglue.o gemlib vdibind aesbind osbind libf


clean::
	$(RM) *.o *.oo usa.prg ttdesk.prg newglue.tos
