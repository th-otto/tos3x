.SUFFIXES: .oo .o .s .c

BIN=$(ALCYON)\bin
LIB=$(ALCYON)\lib
INC=$(ALCYON)\include


.c.o:
	$(AM_V_CC)$(BIN)\cp68 $(DEFS) -i $(INC) $*.c $*.i
	$(AM_V_at)$(BIN)\c068 $*.i $*.1 $*.2 $*.3 -f
	$(AM_V_at)$(BIN)\c168 $*.1 $*.2 $*.s
	$(AM_V_at)$(BIN)\as68 -l -u $*.s
	$(AM_V_at)$(RM) $*.s $*.i $*.1 $*.2 $*.3

STARTUP = gemstart.o

all:	hello.tos

hello.tos: hello.o
	$(BIN)\aln -y $(LIB) -o $@ $(STARTUP) hello.o gemlib vdibind aesbind osbind libf
