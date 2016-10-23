##
#### TOS version
## currently 3.06, 2.06 and 2.08 is supported

TOSVERSION=306

##
#### Country definition
## valid countries are: us de fr es it sv

COUNTRY=de


$(top_srcdir)/common/config.h: $(top_srcdir)/config.mak ${MAKEFILE}
	$(AM_V_GEN)echo '/* automatically generated - DO NOT EDIT */' > $@
	$(AM_V_at)echo '#define TOSVERSION 0x$(TOSVERSION)' >> $@
	$(AM_V_at)echo '#define ATOSVERSION $$$(TOSVERSION)' >> $@
	$(AM_V_at)echo '#define OS_COUNTRY CTRY_$(COUNTRY)' >> $@

$(top_srcdir)/common/sections.mak: $(top_srcdir)/common/sections.inc $(top_srcdir)/common/config.h
	$(AM_V_GEN)$(CPP) -D__MAKEFILE__ $< $(CPPOUT) sections.i
	$(AM_V_at)sed -e 's/$(QBS)$$//g' -e '//d' sections.i > sections.1
	$(AM_V_at)bc -q < sections.1 > $@
	$(AM_V_at)$(RM) sections.i sections.1
