### 	Makefile

COPTS	=       -v
CMODEL	=	-w -r0 -b0
CREFS	=	
DEFINES =	-I..\..\include -Irsc
LMODEL	=	snb
LDOPTS  =       -N -tx -Lnbg+$(ALTLIB)
ALTLIB  =	aesa$(LMODEL).lib
#
CFLAGS	=	$(COPTS) $(CMODEL) $(CREFS) $(DEFINES)
ASFLAGS =
LDFLAGS =	$(COPTS) $(CMODEL) $(LDOPTS)
MAPOPTS =	#MAP $*.map

WCOLORS =	csnbcpx.o cpxstart.o wcolorx.o asm.o rc_inter.o rc_cente.o ob_draw.o gr_mksta.o ev_timer.o

#.s.o:
#	mac $*.s
#	dr2lc $@
.s.o:
	asm $*.s

.c.o:
	lc $(COPTS) $(CMODEL) $(CREFS) $(DEFINES) $<

# PREFIX = PREFIX rsc/en.hdr

all: wcolors.cpx

wcolors.cp:	$(WCOLORS)
		clink FROM $(WCOLORS: =+) TO $@ LIB lcgsnb.lib+lcsnb.lib NODEBUG BATCH QUIET ADDSYM

wcolors.cpx:	wcolors.cp
		cat rsc/en.hdr $< > $@

cpxstart.o:	cpxstart.s
csnbcpx.o:	csnbcpx.s
wcolors.o:	wcolorx.c wcolorx.h wcolorx.rsh
asm.o:		asm.s

clean::
	$(RM) *.o *.prg *.cpx nul
