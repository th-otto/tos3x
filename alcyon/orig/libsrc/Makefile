include ../Makefile.cmn
include ../Makefile.sil

all::

include SRCFILES
include optimize.mak

STARTUPS = ${CRT0} crtg.o gemst0.o gemst1.o gemst2.o gemst3.o gemst4.o gemst8k.o gemsta.o

all:: ${LIBC} ${LIBC_NDX} liba.a ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${STARTUPS} srcfiles.sh

${LIBC}: $(OBJS) ${OPTIONOBJS} ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS3)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS4)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS5)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS6)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS7)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS8)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS9)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS10)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS11)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS12)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS13)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS14)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS15)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS16)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS17)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS18)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS2)

# library with all objects, including optional and runtime,
# for use by analyze tool
FIXOBJS = fixxopen.o fixlst.o fixwrtc.o fixwrite.o bugtty.o
liba.a: ${LIBC} ${OPTIONOBJS} ${STARTUPS} $(FIXOBJS) $(MCOBJS) $(MAOBJS)
	cp $< $@
	$(AR) $(ARFLAGS) $@ ${STARTUPS}
	$(AR) $(ARFLAGS) $@ $(FIXOBJS)
	$(AR) $(ARFLAGS) $@ ${OPTIONOBJS1}
	$(AR) $(ARFLAGS) $@ ${OPTIONOBJS2}
	$(AR) $(ARFLAGS) $@ ${MCOBJS1}
	$(AR) $(ARFLAGS) $@ ${MCOBJS2}
	$(AR) $(ARFLAGS) $@ ${MAOBJS1}
	$(AR) $(ARFLAGS) $@ ${MAOBJS2}
	$(AR) $(ARFLAGS) $@ ${MAOBJS3}
	$(AR) $(ARFLAGS) $@ ${MAOBJS4}

fixxopen.o: fixxopen.c xopen.c
fixlst.o: fixlst.c lstout.c
bugtty.o: bugtty.c ttyout.c
fixwrtc.o: fixwrtc.c wrtchr.c
fixwrite.o: fixwrite.c write.c

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

${LIBM}: $(MOBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MCOBJS2)

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} nofloat.o ${STARTUPS} $(LIB)

gemst0.o: gemst0.s
gemst0.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=0" > $@
	$(AM_V_at)cat $< >> $@

gemst1.o: gemst1.s
gemst1.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=1" > $@
	$(AM_V_at)cat $< >> $@

gemst2.o: gemst2.s
gemst2.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=2" > $@
	$(AM_V_at)cat $< >> $@

gemst3.o: gemst3.s
gemst3.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=3" > $@
	$(AM_V_at)cat $< >> $@

gemst4.o: gemst4.s
gemst4.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=4" > $@
	$(AM_V_at)cat $< >> $@

gemst8k.o: gemst8k.s
gemst8k.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=-8192" > $@
	$(AM_V_at)cat $< >> $@

gemsta.o: gemsta.s
gemsta.s: gemstart.S
	$(AM_V_GEN)
	$(AM_V_at)echo "STACK=-1" > $@
	$(AM_V_at)cat $< >> $@

clean:
	$(RM) *.o *.a *.ndx gemst0.s gemst1.s gemst2.s gemst3.s gemst4.s gemst8k.s gemsta.s
