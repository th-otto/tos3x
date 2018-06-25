tosrsc$(COUNTRY).o: tosrsc$(COUNTRY).c

tosrsc$(COUNTRY).c: $(MKBININC) glue.$(COUNTRY)
	$(AM_V_GEN)$(MKBININC) glue.$(COUNTRY) tosrsc $@

glue.us: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemus.rsc ../desk/rsc/$(TOSVERSION)/deskus.rsc ../desk/rsc/$(TOSVERSION)/deskus.inf
	$(AM_V_GEN)$(MKGLUE) us $(TOSVERSION)

glue.de: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemde.rsc ../desk/rsc/$(TOSVERSION)/deskde.rsc ../desk/rsc/$(TOSVERSION)/deskde.inf
	$(AM_V_GEN)$(MKGLUE) de $(TOSVERSION)

glue.fr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemfr.rsc ../desk/rsc/$(TOSVERSION)/deskfr.rsc ../desk/rsc/$(TOSVERSION)/deskfr.inf
	$(AM_V_GEN)$(MKGLUE) fr $(TOSVERSION)

glue.uk: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemuk.rsc ../desk/rsc/$(TOSVERSION)/deskuk.rsc ../desk/rsc/$(TOSVERSION)/deskuk.inf
	$(AM_V_GEN)$(MKGLUE) uk $(TOSVERSION)

glue.es: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemes.rsc ../desk/rsc/$(TOSVERSION)/deskes.rsc ../desk/rsc/$(TOSVERSION)/deskes.inf
	$(AM_V_GEN)$(MKGLUE) es $(TOSVERSION)

glue.it: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemit.rsc ../desk/rsc/$(TOSVERSION)/deskit.rsc ../desk/rsc/$(TOSVERSION)/deskit.inf
	$(AM_V_GEN)$(MKGLUE) it $(TOSVERSION)

glue.se: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemse.rsc ../desk/rsc/$(TOSVERSION)/deskse.rsc ../desk/rsc/$(TOSVERSION)/deskse.inf
	$(AM_V_GEN)$(MKGLUE) se $(TOSVERSION)

glue.sf: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemsf.rsc ../desk/rsc/$(TOSVERSION)/desksf.rsc ../desk/rsc/$(TOSVERSION)/desksf.inf
	$(AM_V_GEN)$(MKGLUE) sf $(TOSVERSION)

glue.sg: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemsg.rsc ../desk/rsc/$(TOSVERSION)/desksg.rsc ../desk/rsc/$(TOSVERSION)/desksg.inf
	$(AM_V_GEN)$(MKGLUE) sg $(TOSVERSION)

glue.tr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemtr.rsc ../desk/rsc/$(TOSVERSION)/desktr.rsc ../desk/rsc/$(TOSVERSION)/desktr.inf
	$(AM_V_GEN)$(MKGLUE) tr $(TOSVERSION)

glue.fi: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemfi.rsc ../desk/rsc/$(TOSVERSION)/deskfi.rsc ../desk/rsc/$(TOSVERSION)/deskfi.inf
	$(AM_V_GEN)$(MKGLUE) fi $(TOSVERSION)

glue.no: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemno.rsc ../desk/rsc/$(TOSVERSION)/deskno.rsc ../desk/rsc/$(TOSVERSION)/deskno.inf
	$(AM_V_GEN)$(MKGLUE) no $(TOSVERSION)

glue.dk: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemdk.rsc ../desk/rsc/$(TOSVERSION)/deskdk.rsc ../desk/rsc/$(TOSVERSION)/deskdk.inf
	$(AM_V_GEN)$(MKGLUE) dk $(TOSVERSION)

glue.pl: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gempl.rsc ../desk/rsc/$(TOSVERSION)/deskpl.rsc ../desk/rsc/$(TOSVERSION)/deskpl.inf
	$(AM_V_GEN)$(MKGLUE) pl $(TOSVERSION)

glue.gr: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemgr.rsc ../desk/rsc/$(TOSVERSION)/deskgr.rsc ../desk/rsc/$(TOSVERSION)/deskgr.inf
	$(AM_V_GEN)$(MKGLUE) gr $(TOSVERSION)

glue.ru: $(MKGLUE) ../aes/rsc/$(TOSVERSION)/gemru.rsc ../desk/rsc/$(TOSVERSION)/deskru.rsc ../desk/rsc/$(TOSVERSION)/deskru.inf
	$(AM_V_GEN)$(MKGLUE) ru $(TOSVERSION)

