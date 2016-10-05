##
#### Country definition
## valid countries are: us de fr es it sv

COUNTRY=de
TOSVERSION=306


config.h: ../config.mak ${MAKEFILE}
	$(AM_V_GEN)echo '#define TOSVERSION 0x$(TOSVERSION)' > $@
	$(AM_V_at)echo '#define ATOSVERSION $$$(TOSVERSION)' >> $@
	$(AM_V_at)echo '#define OS_COUNTRY CTRY_$(COUNTRY)' >> $@
