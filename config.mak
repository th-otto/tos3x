##
#### Country definition
## valid countries are: us de fr es it sv

COUNTRY=de

##
#### TOS version
## currently only 3.06 is supported

TOSVERSION=306


$(top_srcdir)/common/config.h: $(top_srcdir)/config.mak ${MAKEFILE}
	$(AM_V_GEN)echo '#define TOSVERSION 0x$(TOSVERSION)' > $@
	$(AM_V_at)echo '#define ATOSVERSION $$$(TOSVERSION)' >> $@
	$(AM_V_at)echo '#define OS_COUNTRY CTRY_$(COUNTRY)' >> $@
