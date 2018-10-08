### 	Makefile

COPTS	=       -d0 -cf -v -O #-n -O
CMODEL	=	-w -r0 -b0
CREFS	=	
DEFINES =	
LMODEL	=	snb
LDOPTS  =       -N -tx -Lnbg+$(ALTLIB) #-Lnbg   
ALTLIB  =	aesa$(LMODEL).lib
#
CFLAGS	=	$(COPTS) $(CMODEL) $(CREFS) $(DEFINES)
ASFLAGS =
LDFLAGS =	$(COPTS) $(CMODEL) $(LDOPTS)
MAPOPTS =	#MAP $*.map

MENUTOOL  =	cpxstart.o colorvdi.o 

#.s.o:
#	mac $*.s
#	dr2lc $@
#.s.o:
#	e:\lattice\bin\mas -us $*.s
#	dr2lc $@
.s.o:
	gentt -L7 $*.s

colorvdi.cpx:	$(MENUTOOL)
		lcc $(LDFLAGS)=$@ $(MENUTOOL)
		@gstfix $@

cpxstart.o:
colorvdi.o:	country.h colorvdi.h colorvdi.rsh
