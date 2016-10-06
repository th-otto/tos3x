top_srcdir=..

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent
include optimize.mak

all: desk.a desk.o

include $(top_srcdir)/config.mak
include $(top_srcdir)/language.mak

include SRCFILES

CPPFLAGS +=

desk.a: tosvars.o $(top_srcdir)/common/config.h $(DESK_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ tosvars.o $(DESK_OBJS1)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS2)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS3)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS4)
	$(AR) $(ARFLAGS) $@ $(DESK_OBJS5)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
vars.o: vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

desk.o: checklang tosvars.o vars.o $(DESK_OBJS) deskext.o ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(desk_tbase) $(desk_dbase) 0 tosvars.o vars.o $(DESK_OBJS) deskext.o

deskext.o: deskext.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

##
#### Desktop dependencies
##

STDDESK.H = mobdefs.h defines.h window.h desk$(COUNTRY).h
GEMDESK.H = extern.h osbind.h gemdefs.h

deskact.o:  $(STDDESK.H) $(GEMDESK.H)
deskapp.o:  $(STDDESK.H) $(GEMDESK.H)
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

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
