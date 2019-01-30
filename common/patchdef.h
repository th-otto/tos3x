/*
 * Defaults which patches should be applied.
 *
 * You can either edit the settings below, or
 * (better) create a file common/localcnf.h instead
 * with your preferred settings
 */
 
/*
 * TP_01: KILL_RESET: Omit RESET instruction for RAM-TOS on older ST's
 */
#ifndef TP_01
#define TP_01 (RAMVERSION)
#endif

/*
 * TP_02: COLOR_60HZ: Switch to 60Hz on color monitors
 */
#ifndef TP_02
#define TP_02 0
#endif

/*
 * TP_03: RAMRESET: Modified reset routine.
 * Will keep RAM-TOS resident even after Hardreset (via keyboard)
 */
#ifndef TP_03
#define TP_03 (RAMVERSION)
#endif

/*
 * TP_04: MEM_INIT: Clear memory from $100-$400
 * Clear the memory starting at $100 instead of $400 on reset,
 * to remove any junk.
 * This patch is disabled if the RAMRESET patch is also active.
 */
#ifndef TP_04
#define TP_04 0
#endif

/*
 * TP_05: SHELL_P: Clear _shell_p
 * Clear the _shell_p system variable on reset.
 */
#ifndef TP_05
#define TP_05 1
#endif

/*
 * TP_06: MSTE_CACHE_ON: Turn on 16Mhz for Mega-STE
 * Turns on 16 MHz and cache on Mega-STE already when booting
 */
#ifndef TP_06
#define TP_06 0
#endif

/*
 * TP_07: SET_STEP: Set steprate for floppy drives A: and B:
 * 0 = 6 ms
 * 1 = 12 ms
 * 2 = 2 ms
 * 3 = 3 ms
 */
#ifndef TP_07
#define TP_07 0
#define STEP_RATE 3
#endif

/*
 * TP_08: SET_FDC_COOKIE: Install HD cookie
 *
 * Do not use this on Mega-STE/TT, because
 * there it will be autodetected
 *
 * Format: 0xkkkkkk / normal (Mega STE/TT): $01415443
 * kkkkkk: ID who installed the cookie.
 * x=0: normal density (DD 720kB/360kB)
 * x=1: high density (HD 1.44MB)
 * x=2: extra-high density (ED 2.88MB)
 * x>2: reserved
 */
#ifndef TP_08
#define TP_08 0
#define FDC_COOKIE $01415443
#endif

/*
 * TP_09: SET_TIME_CRC: Reduce time for CRC test
 * TP_10: SET_TIME_RAM: Reduce time for RAM test
 * BOOT_TIME: time (in s) for above
 * TP_11: SKIP_WAIT: Skip display of ramtest bar
 * TP_12: SKIP_RAM: Show only amount of memory
 *
 * Reduce the time after which - after system start -
 * a CRC test or a RAM test is run. Additionally, the
 * RAM test can be configured to display only the
 * memory configuration or the bar.
 */
#ifndef TP_09
#define TP_09 0
#endif
#ifndef TP_10
#define TP_10 0
#endif
#ifndef TP_11
#define TP_11 0
#endif
#ifndef TP_12
#define TP_12 0
#endif
#ifndef BOOT_TIME
#define BOOT_TIME 0
#endif

/*
 * TP_13: BOOT_DEV: Fix boot device error (see ST-Computer 1/90)
 */
#ifndef TP_13
#define TP_13 1
#endif

/*
 * TP_14: CT_HD: Boot function for c't "Billigloesung"
 */
#ifndef TP_14
#define TP_14 0
#endif

/*
 * TP_15: STACK_POINTER: Fix stack pointer in autoexec
 * Fixes a bug in the autoexec routine (see ST-Computer 1/90)
 */
#ifndef TP_15
#define TP_15 1
#endif

/*
 * TP_16: PILZE: Alternative image for bombs
 * Replaces the Atari bomb images with the original mushrooms
 * 0: Original bombs
 * 1: Mushrooms
 * 2: Atari logo
 * 3: Pfeifenkopf
 * 4: Stinkefinger
 * 5: Skull
 */
#ifndef TP_16
#define TP_16 0
#endif

/*
 * TP_17: LOCK_CLOCK: Lock Mega-ST clock
 * 
 * Prevents the hardware clock from being set by TOS.
 * A separate program is then needed to update it.
 */
#ifndef TP_17
#define TP_17 0
#endif

/*
 * TP_18: IGNORE_BLITTER: Ignore the blitter
 * 
 * The blitter will be disavowed and ignored by TOS
 * Should be set if PAK patches are also applied.
 */
#ifndef TP_18
#define TP_18 TP_50
#endif

/*
 * TP_19: FASTPRN: Fast printer routines for the parallel port
 * 
 * Output, Input- and wait functions will be replaced.
 * Corresponds to FASTPRN.PRG from Ecki from the c't magazine.
 */
#ifndef TP_19
#define TP_19 1
#endif

/*
 * TP_20: TIMEOUT: Set printer timeout
 * 
 * Original value is 30 seconds. Minimum value is 5 seconds.
 * Does not work with the Atari Laser Printer.
 */
#ifndef TP_20
#define TP_20 1
#endif
#ifndef PRNTIMEOUT
#define PRNTIMEOUT 30
#endif

/*
 * TP_21: SET_CONTERM: Set conterm
 * 
 * Bit 2 set: bell on CNTRL-G
 * Bit 1 set: key repeat on
 * Bit 0 set: key click on
 */
#ifndef TP_21
#define TP_21 0
#endif
#ifndef CONTERM
#define CONTERM 6
#endif

/*
 * TP_22: SEEK_UP: Set hdmode to zero
 *
 * Borrowed from SEEKUP from Martin Osieka.
 * The patch only changes the initialization, everything
 * else remains unchanged.
 *
 * SEEKUP turns off the seek rate doubling on STs
 * (recognizable by the seek noise of the drive)
 */
#ifndef TP_22
#define TP_22 0
#endif

/*
 * TP_23: FASTLOAD: Set fastload-bit for floppy reads
 *
 * Produces errors with some drives, take care!
 * (see ST-Computer 1/90)
 */
#ifndef TP_23
#define TP_23 0
#endif

/*
 * TP_24: SKIP_B: Skip the search for drive B:
 *
 * This allows faster booting. Do not use that
 * when 2 drives are connected. The virtual drive
 * B: will still be present on the desktop.
 */
#ifndef TP_24
#define TP_24 0
#endif

/*
 * TP_25: ED_BIOS: Support ED drives
 *
 * New functions Getbpb and Rwabs with support for ED drives:
 * - better support for media change detection
 * - Rwabs()-function does not destroy VDI buffers anymore
 * - Floppy discs with 1 FAT only are supported
 *
 * Original routines from Harun Scheutzow.
 */
#ifndef TP_25
#define TP_25 0
#endif

/*
 * TP_26: KILL_BOOT: Prevent execution of floppy boot sector
 */
#ifndef TP_26
#define TP_26 0
#endif

/*
 * TP_27: NORM_BOOT: Normal boot
 *
 * Similar to above, but prevents execution of floppy bootsector
 * only if system was already booted from harddisk. This was
 * normal behaviour until TOS 1.4.
 */
#ifndef TP_27
#define TP_27 0
#endif

/*
 * TP_28: VDIFIX: New v_opnvwk() function
 *
 * Replace v_opnvwk() by a new function to fix a bug.
 * Same functionality as VDIFIX.PRG autofolder program.
 */
#ifndef TP_28
#define TP_28 1
#endif

/*
 * TP_29: M_SHRINK: Mshrink fix
 *
 * Fixes a bug in Mshrink that can destroy the memory lists
 */
#ifndef TP_29
#define TP_29 1
#endif

/*
 * TP_30: DRAW_BOXES: Turn off grow- and shrinkboxes
 */
#ifndef TP_30
#define TP_30 0
#endif

/*
 * TP_31: MS_DOS: Correct MS-DOS Format
 *
 * Writes $EB34904E into the first bytes of the boot sector
 * when formatting floppy discs
 */
#ifndef TP_31
#define TP_31 0
#endif

/*
 * TP_32: CACHE_0X0: Activate caches for all cpus >= 68020
 *
 * This patch is still experimental. Note also that with this patch active,
 * you won't be able to activate/deactivate the blitter from the desktop
 * when you have a cpu >= 68020.
 */
#ifndef TP_32
#define TP_32 0
#endif

/*
 * TP_33: FUJI: Substitute starting fuji
 *
 * 0: original atari fuji
 * 1: modified atari fuji with PAK/3 letters from SE
 * 2: 'Gib Dos keine Chance' from Reiner Rosin/TRK
 * 3: 'GeTuned' from Sven Spitzmacher
 * 4: Schegale
 * 5: UFP logo
 * 6: X
 * 7: Intel Outside
 */
#ifndef TP_33
#define TP_33 0
#endif

/*
 * TP_34: NEC_P6: Change printer line spacing
 *
 * Changes the line spacing for hardcopy. Enables use
 * of hardcopies for NEC-P6, as well as EPSON printers
 * (see ST-Computer 10/90)
 */
#ifndef TP_34
#define TP_34 0
#endif

/*
 * TP_35: FONTS: Change font images
 *
 * Changes the images of the builtin fonts
 * (the data only; all other properties must remain)
 * 0: original system font
 * 1: new font from tospatch archive
 * 2: polish font found in tos306pl
 * 3: Monaco Mono
 * below are only available in 8x16:
 * 4: blue
 * 5: classic
 * 6: classic 1
 * 7: classic 2
 * 8: computer
 * 9: cursive
 * 10: desktop
 * 11: deutsch
 * 12: eight
 * 13: griech
 * 14: ibm_g2
 * 15: kritzel
 * 16: mac_font
 * 17: mcappeal
 * 18: old
 * 19: rally
 * 20: skyvox
 * 21: stencil
 * 22: sv
 * 23: tempus
 * 24: thin
 * 25: thin1
 * 26: typewrit
 */
/* for 6x6 font */
#ifndef TP_35_6
#define TP_35_6 0
#endif
/* for 8x8 font */
#ifndef TP_35_8
#define TP_35_8 0
#endif
/* for 8x16 font */
#ifndef TP_35_16
#define TP_35_16 0
#endif
/* for 16x32 font (TOS 3.06 only) */
#ifndef TP_35_32
#define TP_35_32 0
#endif

/*
 * TP_36: INFO_TXT: Set custom info text
 *
 * This string will be displayed in place of the hyphens in the About dialog
 * (29 characters max., ASCII only)
 */
#ifndef TP_36
#define TP_36 "-----------------------------"
#endif

/*
 * TP_37: ICONS: Set custom desktop icons
 * TP_37_1: floppy
 * TP_37_2: folder
 * TP_37_3: trashcan
 * TP_37_4: program
 * TP_37_5: file
 * TP_37_6: printer
 * TP_37_7: laserprinter
 * TP_37_8: cdrom
 * TP_37_9: cartridge
 * TP_37_10: floppy2
 * TP_37_11: new program
 * TP_37_12: harddisk
 */

/*
 * TP_38: ICONS: Set custom mouse cursors
 * TP_38_0: Arrow
 * TP_38_1: Text cursor
 * TP_38_2: Busy bee
 * TP_38_3: Point hand
 * TP_38_4: Flat hand
 * TP_38_5: Thin cross
 * TP_38_6: Thick cross
 * TP_38_7: Outline cross
 */

/*
 * TP_39: MOUSE: Set custom alert icons
 * TP_39_1: Note icon
 * TP_39_2: Question icon
 * TP_39_3: Stop icon
 */

/*
 * TP_40: RSC: Replace resource files
 * TP_40_1: AES resource file
 * TP_40_2: DESKTOP resource file
 * TP_40_3: NEWDESK.INF file
 */

/*
 * TP_41: HD_BIT: HD-Floppy controlling
 *
 * 0: don't change
 * 6: use sound-chip bit 6
 * 7: use sound-chip bit 7
 *
 * TP_42: HD_INVERT: Invert level
 *
 * Control HD-floppy selection via sound-chip bit 6 or 7.
 * If the HD controller expects low-level, also set TP_42 (HD_INVERT) to 1,
 * otherwise high-level will be used.
 * Note: ST-Book uses a low-level on bit 7.
 */
#ifndef TP_41
#define TP_41 0
#endif
#ifndef TP_42
#define TP_42 0
#endif

/*
 * TP_43: HD_WAIT: Harddisk waiter
 *
 * 0: do not use
 * 1: Version 1
 *    Unknown routine that may only work with 1 harddisk.
 *    Can be aborted by pressing ALTERNATE.
 *    Only tested with Megafile
 * 2: Version 2
 *    Simple version that just waits for 12sec
 *    This should work in any case.
 *    Can be aborted by pressing any key.
 * 3: Version 3
 *    New version written by EWK, can wait for any ACSI device.
 *    Can be aborted by pressing Shift-Shift.
 * 4: Version 4
 *    A version that waits instead of displaying the ramtest bar.
 *    Uses DMAread instead of waiting a specific time.
 *    Can be aborted by pressing ALTERNATE.
 *
 * TP_44: HD_WAITTIME: Waiting time in seconds
 * TP_45: HD_WAITDEVICE: Device to wait for,
 *        for version 3
 */
#ifndef HD_WAIT
#define HD_WAIT 0
#endif
#ifndef HD_WAITTIME
#define HD_WAITTIME 12
#endif
#ifndef HD_WAITDEVICE
#define HD_WAITDEVICE 0
#endif

/*
 * TP_46: NOVA: NovaPatch
 *
 * Activation of nova card already when booting.
 * For all Nova cards except type 2 and 6.
 */
#ifndef TP_46
#define TP_46 0
#endif

/*
 * TP_47: SHBUF: Increase buffer for NEWDESK.INF
 *
 * Increases the buffer used for reading the NEWDESK.INF.
 * Original routines from SHBUF 1.3 by Martin Osieka.
 * For more information, see SHBUF.
 *
 * SHBUFSIZE: Maximum is 32766 => 32k per buffer.
 * Since there are 2 buffers, that are 64K of memory.
 * Original value is 4k.
 */
#ifndef TP_47
#define TP_47 0
#endif
#ifndef SHBUFSIZE
#define SHBUFSIZE 32766
#endif

/*
 * TP_48: ARROWFIX: New scroll functions for GEM
 *
 * Original routines from ARROWFIX 1.5 by Martin Osieka.
 * For more information, see ARROWFIX.
 *
 * ARROWTIME: default time delay until first
 * repetition of scroll messages, in 0.125s. This time
 * can also be set by the ARROWFIX.CPX. Valid values are 0-9.
 */
#ifndef TP_48
#define TP_48 0
#endif
#ifndef ARROWTIME
#define ARROWTIME 3
#endif

/*
 * TP_49: Fix memory size display
 *
 * The memory test at boot time could
 * not correctly print values > 64MB
 */
#ifndef TP_49
#define TP_49 0
#endif

/*
 * TP_50: PAK: Activate PAK Patches
 *
 *   TP_51: StaticRAM:    1: TOS for Static-RAMs
 *                        0: TOS for ROMs/EPROMs
 *   TP_52: Grafik_Bombs  1: Display bombs on graphic cards/extensions,
 *                           works on all bitplane organized systems
 *                        0: Display bombs on ST screen
 *   TP_53: BootJMP:      1: PAK-trick to boot up the PAK ROMS with TOS 1.x
 *                           on the mainboard.
 *                        0: don't use
 *   TP_55: HasTTMfp:     1: This ST has been upgraded with a TT-MFP
 *                        0: No TT-MFP present
 *   TP_56: MSTE:         1: Do not eliminate MSTE/TT-HD switch
 *                        0: Eliminate MSTE/TT-HD switch for normal STs
 *                           (no longer needed; determined at runtime)
 *   TP_57: Alt_Sync:     1: Use alternative video synchronisation
 *                        0: Use normal video synchronisation
 */
#ifndef TP_50
#define TP_50 0
#endif
#ifndef TP_51
#define TP_51 0
#endif
#ifndef TP_52
#define TP_52 0
#endif
#ifndef TP_53
#define TP_53 1
#endif
#ifndef TP_54
#define TP_54 0
#endif
#ifndef TP_55
#define TP_55 0
#endif
#ifndef TP_56
#define TP_56 0
#endif
#ifndef TP_57
#define TP_57 1
#endif

/*
 * TP_58: NEWVIDEO: Activate EPROM sequencer
 * TP_59: SWITCH:
 *    0 - do not switch
 *    6 - use PSG bit 6
 *    7 - use PSG bit 7
 *
 * Used to activate an EPROM sequencer already in the bootphase.
 * Only for STs with EPROM sequencer expansion.
 */
#ifndef TP_58
#define TP_58 0
#endif
#ifndef TP_59
#define TP_59 0
#endif

/*
 * TP_60: NO_MMU_RESET: Do not reset MMU
 *
 * Needed to make the TOS work when run from TT-RAM,
 * eg. when using ROMRAM.
 */
#ifndef TP_60
#define TP_60 0
#endif

/*
 * TP_61: FPU_TEST: Fix check for FPU
 * Fixes a bug in FPU detection
 */
#ifndef TP_61
#define TP_61 1
#endif

/*
 * TP_62: LOGO: Logo output independent of resolution
 *
 * Uses LineA variables to display the logo, instead
 * of testing the resolution
 */
#ifndef TP_62
#define TP_62 1
#endif

/*
 * TP_63: Magnum
 *
 * Adjust floppy routines vor Magnum Fastram
 *
 * Needed for Magnum-ST with 16MB-SIMM in computers with 1MB ST-RAM,
 * when the low 3 MB ($100000 - $3fffff) should be used
 *
 * Originally by Michael Schwingen
 * see also http://www.uweschneider.de/en/downloads.php
 */
#ifndef TP_63
#define TP_63 0
#endif

/*
 * TP_64: IDE_BOOT: Try to boot from IDE first
 */
#ifndef TP_64
#define TP_64 0
#endif

/*
 * TP_65: EAGLE: GE-Soft Eagle patch
 *
 * Prevent monochrome monitor change
 */
#ifndef TP_65
#define TP_65 0
#endif

/*
 * TP_66: SCSI_ID: Initiatoridentification
 *
 * Initiatoridentification on SCSI access. TT will
 * report ID 7. Needed to be able to boot
 * from certain harddisks (eg. Quantum Empire)
 */
#ifndef TP_66
#define TP_66 1
#endif

/*
 * TP_67: FLOP_TEST: Floppy test fix
 *
 * When testing for floppy, select track #1 instead of track #10
 */
#ifndef TP_67
#define TP_67 0
#endif

/*
 * TP_68: FLOPPY: Increase floppy delay
 *
 * Increase delay for floppy access, to prevent problems
 * with 50MHz. Original value 281.
 */
#ifndef TP_68
#define TP_68 0
#endif
#ifndef FLOPDELAY
#define FLOPDELAY 512
#endif
