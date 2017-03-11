top_srcdir=..
subdir=desk

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil
include optimize.mak

all: $(top_srcdir)/common/config.h desk.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

CPPFLAGS +=

desk.a: tosvars.o $(top_srcdir)/common/config.h $(DESK_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS1)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS2)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS3)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS4)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS5)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

desk.o: vars.o $(DESK_OBJS) deskext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	# sections.mak might have just been generated, make sure it is reread
	$(MAKE) TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY) single

single:
	$(LD) -u -s -l -o desk.o -a $(desk_tbase) $(desk_dbase) 0 vars.o $(DESK_OBJS) deskext.o

osbind.o: osbind.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

deskif.o: deskif.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

deskext.o: deskext.S $(top_srcdir)/common/config.h $(top_srcdir)/common/sections.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i


check:: desk.o
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(desk_toff):0x24 --offsets=0x$(desk_tbase):0 --bytes=0x$(desk_tlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img desk.o
	$(AM_V_at)$(CMP) $(CMPL) -x --ignore-initial=0x$(desk_doff):0x$(desk_tleno) --offsets=0x$(desk_dbase):0x$(desk_tlen) --bytes=0x$(desk_dlen) $(top_srcdir)/roms/tos$(TOSVERSION)$(COUNTRY).img desk.o

clean:
	$(RM) *.o *.a *.ndx *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
