
01/28/93 cjg - This version of XCONTROL is the release version
 	       for the FALCON and MULTI-TOS.
	     
	     - NOTE: After building the Control Panel, run PRGFLAGS
	             and set the PROTECTION to SUPERVISOR.
		     This is so that there is no conflict when
		     the exchange mouse button vector occurs.

03/31/93 cjg - This version uses malloc() instead of Malloc().
	       Therefore, this version uses less memory than
	       the previous version under MultiTOS.

	     - The 'Number of Slots' option has been removed.

	     - Still need to set PROTECTION to SUPERVISOR.


identical:
    xcontrol.prg; compile using Turbo-C 2.0
    fsound.cpx; compile using Turbo-C 2.0
    general.cpx; compile using Turbo-C 2.0
    modem.cpx; compile using Turbo-C 2.0
    multitos.cpx; compile using Turbo-C 2.0
    printer.cpx; compile using Turbo-C 2.0
    ttsound.cpx; compile using Turbo-C 2.0
    
not identical:
    colorvdi.cpx
    config.cpx
    setenv.cpx
    submenu.cpx
    wcolors.cpx; compiled by Lattice-C 5.06.??

tbd:
    maccel

others:
    system (incomplete)
    alerts (for tests only)
