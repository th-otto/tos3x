<!DOCTYPE html>
<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>TOS Patches</title>
<meta http-equiv="content-type" content="text/html;charset=UTF-8" />
<meta name="keywords" content="ORCS, CAT, GC, PBEM, PBM, GC-Ork, GCORK, ARAnyM, UDO, EmuTOS, TOSPATCH" />
<link rel="stylesheet" type="text/css" href="tospatch.css" />
<script src="tospatch.js"></script>
</head>

<body onload="onload();">
<div>

<hr />
<h1>TOS patching made easy</h1>
<hr />

<table>
<tr>
<td>
<form action="tospatch.php" method="post" id="tospatch" enctype="multipart/form-data">
<fieldset>
<table>

<tr>
<td>
TOS version:
</td>
<td colspan="2">
<select id="tosversion" name="tosversion" onchange="changeTosVersion();">
<option value="206">2.06</option>
<option value="208">2.08 (STBook)</option>
<option value="306">3.06</option>
</select>
</td>
</tr>

<tr>
<td>
Country:
</td>
<td colspan="2">
<select id="country" name="country">
<option value="us">USA</option>
<option value="de">Germany</option>
<option value="fr">France</option>
<option value="uk">United Kingdom</option>
<option value="es">Spain</option>
<option value="it">Italy</option>
<option value="se">Sweden</option>
<option value="sf">Switzerland (French)</option>
<option value="sg">Switzerland (German)</option>
</select>
</td>
</tr>

<tr>
<td>
Output:
</td>
<td colspan="2">
<select id="ramversion" name="ramversion">
<option value="0">ROM image</option>
<option value="1">RAM image for 0.5 MB ST-RAM</option>
<option value="2">RAM image for 1 MB ST-RAM</option>
<option value="3">RAM image for 2 MB ST-RAM</option>
<option value="4">RAM image for 2.5 MB ST-RAM</option>
<option value="5">RAM image for 3 MB ST-RAM</option>
<option value="6">RAM image for 4 MB ST-RAM</option>
<option value="7">RAM image for 4 MB TT-RAM</option>
<option value="8">RAM image for 8 MB TT-RAM</option>
<option value="9">RAM image for 16 MB TT-RAM</option>
</select>
</td>
</tr>

<tr><td>&nbsp;</td><td></td><td></td></tr>

<tr>
<td colspan="2">
Patches:
</td>
<td></td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Kill Reset:
</td>
<td>
<input type="checkbox" name="tp_01" value="1" /><br />
</td>
<td>
Omit RESET instruction for RAM-TOS on older ST's
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Do not reset MMU:
</td>
<td>
<input type="checkbox" name="tp_60" value="1" /><br />
</td>
Needed to make the TOS work when run from TT-RAM,
eg. when using ROMRAM.
<td>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Color 60hz:
</td>
<td>
<input type="checkbox" name="tp_02" value="1" /><br />
</td>
<td>
Switch to 60Hz on color monitors
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Reset resident:
</td>
<td>
<input type="checkbox" name="tp_03" value="1" /><br />
</td>
<td>
Modified reset routine. <br />
Will keep RAM-TOS resident even after Hardreset (via keyboard)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Clear memory from $100-$400:
</td>
<td>
<input type="checkbox" name="tp_04" value="1" /><br />
</td>
<td>
Clear the memory starting at $100 instead of $400 on reset,
to remove any junk. <br />
This patch is disabled if the &lt;Reset Resident&gt; patch is also active.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Clear _shell_p:
</td>
<td>
<input type="checkbox" name="tp_05" value="1" checked="checked" /><br />
</td>
<td>
Clear the _shell_p system variable on reset.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Turn on 16Mhz for Mega-STE:
</td>
<td>
<input type="checkbox" name="tp_06" value="1" /><br />
</td>
<td>
Turns on 16 MHz and cache on Mega-STE already when booting
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set steprate for floppy drives A: and B:
</td>
<td>
<input type="checkbox" name="tp_07" value="1" /><br />
</td>
<td>
<select id="seekrate" name="seekrate">
<option value="2">2 ms</option>
<option value="3" selected="selected">3 ms</option>
<option value="0">6 ms</option>
<option value="1">12 ms</option>
</select>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Floppy test fix:
</td>
<td>
<input type="checkbox" name="tp_67" value="1" /><br />
</td>
<td>
When testing for floppy, select track #1 instead of track #10
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Increase floppy delay
</td>
<td>
<input type="checkbox" name="tp_68" value="1" /><br />
<input type="number" name="flopdelay" value="512" min="128" max="1024" style="width: 5em" /><br />
</td>
<td>
Increase delay for floppy access, to prevent problems
with 50MHz. Original value 281.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Install HD cookie:
</td>
<td>
<input type="checkbox" name="tp_08" value="1" /><br />
</td>
<td>
<select id="fdc_cookie" name="fdc_cookie">
<option value="$00415443">normal density (DD 720kB/360kB)</option>
<option value="$01415443" selected="selected">high density (HD 1.44MB)</option>
<option value="$02415443">extra-high density (ED 2.88MB)</option>
</select>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Change delay for CRC test:
</td>
<td>
<input type="checkbox" name="tp_09" value="1" /><br />
</td>
<td rowspan="5">
Reduce the time after which - after system start - <br />
a CRC test or a RAM test is run. Additionally, the <br />
RAM test can be configured to display only the <br />
memory configuration or the bar. <br />
</td>
</tr>
<tr>
<td>
Change delay for RAM test:
</td>
<td>
<input type="checkbox" name="tp_10" value="1" /><br />
</td>
</tr>
<tr>
<td>
Boot time for above:
</td>
<td>
<input type="number" name="boottime" value="80" min="0" max="80" style="width: 4em" /> seconds<br />
</td>
</tr>
<tr>
<td>
Skip display of ramtest bar:
</td>
<td>
<input type="checkbox" name="tp_11" value="1" /><br />
</td>
</tr>
<tr>
<td>
Show only amount of memory:
</td>
<td>
<input type="checkbox" name="tp_12" value="1" /><br />
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Fix memory size display:
</td>
<td>
<input type="checkbox" name="tp_49" value="1" checked="checked" /><br />
</td>
<td>
The memory test at boot time could
not correctly print values > 64MB.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Fix boot device error:
</td>
<td>
<input type="checkbox" name="tp_13" value="1" checked="checked" /><br />
</td>
<td>
Fixes a long-standing bug that sets the default
path for GEM to the floppy (see ST-Computer 1/90)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Fix check for FPU:
</td>
<td>
<input type="checkbox" name="tp_61" value="1" checked="checked" /><br />
</td>
<td>
Fixes a bug in FPU detection
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Boot function for c't "Billigl&ouml;sung":
</td>
<td>
<input type="checkbox" name="tp_14" value="1" /><br />
</td>
<td>
The c't "Billigl&ouml;sung" was a project started by
the german magazine c't around 1988. This patch adds support
for that adapter.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Fix stack pointer in autoexec:
</td>
<td>
<input type="checkbox" name="tp_15" value="1" checked="checked" /><br />
</td>
<td>
Fixes a bug in the autoexec routine (see ST-Computer 1/90)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Alternative image for bombs:
</td>
<td>
<select id="tp_16" name="tp_16" onchange="changeBombs();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Bombs</option>
<option value="1">Mushrooms</option>
<option value="2">Atari logo</option>
<option value="3">Pfeifenkopf</option>
<option value="4">Stinkefinger</option>
<option value="5">Skull</option>
</select>
<img id="tp_16_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" />
</td>
<td>
Replaces the Atari bomb images <img src="bombs/bomb.bmp" width="16" height="16" style="border:0" alt="Bomb" />
<!-- with the original mushrooms <img src="bombs/mushroom.bmp" width="16" height="16" style="border:0" alt="Mushroom" /> -->
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Lock Mega-ST clock:
</td>
<td>
<input type="checkbox" name="tp_17" value="1" /><br />
</td>
<td>
Prevents the hardware clock from being set by TOS. <br />
A separate program is then needed to update it.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Ignore the blitter:
</td>
<td>
<input type="checkbox" name="tp_18" value="1" /><br />
</td>
<td>
The blitter will be disavowed and ignored by TOS.
Should be set if PAK patches are also applied.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Fast printer routines for the parallel port:
</td>
<td>
<input type="checkbox" name="tp_19" value="1" checked="checked" /><br />
</td>
<td>
Output, Input- and wait functions will be replaced.
Corresponds to FASTPRN.PRG from Ecki from the c't magazine.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set printer timeout:
</td>
<td>
<input type="checkbox" name="tp_20" value="1" checked="checked" />
<input type="number" name="prntimeout" value="30" min="5" max="30" style="width: 4em" /> seconds<br />
</td>
<td>
Original value is 30 seconds. Minimum value is 5 seconds.
Does not work with the Atari Laser Printer.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set conterm system variable:
</td>
<td>
<input type="checkbox" name="tp_21" value="1" />
<input type="number" id="conterm" name="conterm" value="7" min="0" max="7" style="width: 4em" onchange="contermChange();" /><br />
</td>
<td>
<input type="checkbox" id="conterm_bell" name="conterm_bell" value="1" checked="checked" onclick="contermBell();" />Bit 2 set: bell on CNTRL-G<br />
<input type="checkbox" id="conterm_repeat" name="conterm_repeat" value="1" checked="checked" onclick="contermRepeat();" />Bit 1 set: key repeat on<br />
<input type="checkbox" id="conterm_click" name="conterm_click" value="1" checked="checked" onclick="contermClick();" />Bit 0 set: key click on<br />
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set hdmode to zero:
</td>
<td>
<input type="checkbox" name="tp_22" value="1" /><br />
</td>
<td>
Borrowed from SEEKUP from Martin Osieka.
The patch only changes the initialization, everything
else remains unchanged. <br />

SEEKUP turns off the seek rate doubling on STs
(recognizable by the seek noise of the drive)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set fastload-bit for floppy reads:
</td>
<td>
<input type="checkbox" name="tp_23" value="1" /><br />
</td>
<td>
Produces errors with some drives, take care!
(see ST-Computer 1/90)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Skip the search for drive B:
</td>
<td>
<input type="checkbox" name="tp_24" value="1" /><br />
</td>
<td>
This allows faster booting. Do not use that
when 2 drives are connected. The virtual drive
B: will still be present on the desktop.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Support ED drives:
</td>
<td>
<input type="checkbox" name="tp_25" value="1" /><br />
</td>
<td>
New functions Getbpb and Rwabs with support for ED drives: <br />
- better support for media change detection <br />
- Rwabs()-function does not destroy VDI buffers anymore <br />
- Floppy discs with 1 FAT only are supported <br />
<br />
Original routines from Harun Scheutzow.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
HD-Floppy controlling
</td>
<td>
<select id="tp_41" name="tp_41">
<option value="0" selected="selected">No change</option>
<option value="6">Use sound-chip bit 6</option>
<option value="7">Use sound-chip bit 7</option>
</select>
<input type="checkbox" name="tp_42" value="1" />Invert level<br />
</td>
<td>
Control HD-floppy selection via sound-chip bit 6 or 7.
If the HD controller expects low-level, use "Invert level",
otherwise high-level will be used.
Note: ST-Book uses a low-level on bit 7.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Prevent execution of floppy boot sector:
</td>
<td>
<input type="checkbox" name="tp_26" value="1" /><br />
</td>
<td></td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Normal boot:
</td>
<td>
<input type="checkbox" name="tp_27" value="1" /><br />
</td>
<td>
Similar to above, but prevents execution of floppy bootsector
only if system was already booted from harddisk. This was
normal behaviour until TOS 1.4.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Harddisk waiter
</td>
<td>
<select id="tp_43" name="tp_43">
<option value="0" selected="selected">None</option>
<option value="1" title="Unknown routine that may only work with 1 harddisk
Can be aborted by pressing ALTERNATE.
Only tested with Megafile">Version 1</option>
<option value="2" title="Simple version that just waits for 12sec
This should work in any case.
Can be aborted by pressing any key.">Version 2</option>
<option value="3" title="New version written by EWK, can wait for any ACSI device.
Can be aborted by pressing Shift-Shift.">Version 3</option>
<option value="4" title="A version that waits instead of
displaying the ramtest bar.
Uses DMAread instead of waiting a specific time.
Can be aborted by pressing ALTERNATE.">Version 4</option>
</select><br />
<input type="number" name="tp_44" value="12" min="0" max="327" style="width: 4em" />&nbsp;Wait time (in sec)<br />
<input type="number" name="tp_45" value="0" min="0" max="7" style="width: 4em" />&nbsp;ACSI device<br />
</td>
<td>
Wait for harddisk after powerup.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
New v_opnvwk() function:
</td>
<td>
<input type="checkbox" name="tp_28" value="1" checked="checked" /><br />
</td>
<td>
Replace v_opnvwk() by a new function to fix a bug.
Same functionality as VDIFIX.PRG autofolder program.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Mshrink fix:
</td>
<td>
<input type="checkbox" name="tp_29" value="1" checked="checked" /><br />
</td>
<td>
Fixes a bug in Mshrink that can destroy the memory lists
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Turn off grow- and shrinkboxes:
</td>
<td>
<input type="checkbox" name="tp_30" value="1" /><br />
</td>
<td>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Correct MS-DOS Format:
</td>
<td>
<input type="checkbox" name="tp_31" value="1" /><br />
</td>
<td>
Writes $EB34904E into the first bytes of the boot sector
when formatting floppy discs
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Activate caches for all cpus &gt;= 68020:
</td>
<td>
<input type="checkbox" name="tp_32" value="1" /><br />
</td>
<td>
This patch is still experimental. Note also that with this patch active,
you won't be able to activate/deactivate the blitter from the desktop
when you have a cpu &gt;= 68020.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Adjust floppy routines vor Magnum Fastram
</td>
<td>
<input type="checkbox" name="tp_63" value="1" /><br />
</td>
<td>
Needed for Magnum-ST with 16MB-SIMM in computers with 1MB ST-RAM,<br />
when the low 3 MB ($100000 - $3fffff) should be used<br />
<br />
Originally by Michael Schwingen<br />
see also <a href="http://www.uweschneider.de/en/downloads.php">http://www.uweschneider.de/en/downloads.php</a>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Substitute starting fuji:
</td>
<td colspan="2">
<table>
<tr>
<td>
<select id="fuji" name="tp_33" onchange="changeFuji();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">original atari fuji</option>
<option value="1">modified atari fuji with PAK/3 letters from SE</option>
<option value="2">'Gib Dos keine Chance' from Reiner Rosin/TRK</option>
<option value="3">'GeTuned' from Sven Spitzmacher</option>
<option value="4">Schegale</option>
<option value="5">UFP logo</option>
<option value="6">X</option>
<option value="7">Intel Outside</option>
</select>
</td>
<td>
<img id="fujiimg" src="fuji/atari.bmp" width="96" height="86" style="border:0" alt="Atari" />
</td>
</tr>
</table>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Logo output independent of resolution:
</td>
<td>
<input type="checkbox" name="tp_62" value="1" checked="checked" /><br />
</td>
<td>
Uses LineA variables to display the logo, instead
of testing the resolution
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Change printer line spacing:
</td>
<td>
<input type="checkbox" name="tp_34" value="1" /><br />
</td>
<td>
Changes the line spacing for hardcopy. Enables use
of hardcopies for NEC-P6, as well as EPSON printers
(see ST-Computer 10/90)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
NovaPatch:
</td>
<td>
<input type="checkbox" name="tp_46" value="1" /><br />
</td>
<td>
Activation of nova card already when booting.
For all Nova cards except type 2 and 6.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Increase buffer for NEWDESK.INF:
</td>
<td>
<input type="checkbox" name="tp_47" value="1" />
<input type="number" name="shbufsize" value="22698" min="4192" max="32766" style="width: 5em" /> bytes<br />
</td>
<td>
Increases the buffer used for reading the NEWDESK.INF.
Original routines from SHBUF 1.3 by Martin Osieka.
For more information, see SHBUF.
<br />
SHBUFSIZE: Maximum is 32766 => 32k per buffer.
Since there are 2 buffers, that are 64K of memory.
Original value is 4k.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
New scroll functions for GEM:
</td>
<td>
<input type="checkbox" name="tp_48" value="1" />
<input type="number" name="arrowtime" value="3" min="0" max="9" style="width: 2em" /> 0,125s<br />
</td>
<td>
Original routines from ARROWFIX 1.5 by Martin Osieka.
For more information, see ARROWFIX.
<br />
ARROWTIME: default time delay until first
repetition of scroll messages, in 0.125s. This time
can also be set by the ARROWFIX.CPX. Valid values are 0-9.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Activate PAK Patches
</td>
<td>
<input type="checkbox" id="tp_50" name="tp_50" value="1" onchange="changePak();" />
</td>
<td>
PAK III-Patches Version 2 (for 68020 and 68030)
<br />
</td>
</tr>
<tr><td></td><td></td><td></td></tr>
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_51" name="tp_51" value="1" disabled="disabled" />
TOS for Static-RAMs
</td></tr>
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_52" name="tp_52" value="1" disabled="disabled" />
Display bombs on graphic cards/extensions,
works on all bitplane organized systems
</td></tr>
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_53" name="tp_53" value="1" checked="checked" disabled="disabled" />
PAK-trick to boot up the PAK ROMS with TOS 1.x on the mainboard.
</td></tr>
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_55" name="tp_55" value="1" disabled="disabled" />
This ST has been upgraded with a TT-MFP
</td></tr>
<!-- no longer needed; determined at runtime
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_56" name="tp_56" value="1" disabled="disabled" />
Do not eliminate MSTE/TT-HD switch
</td></tr>
-->
<tr><td></td><td></td><td>
<input type="checkbox" id="tp_57" name="tp_57" value="1" checked="checked" disabled="disabled" />
Use alternative video synchronisation
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Activate EPROM sequencer:
</td>
<td>
<input type="checkbox" id="tp_58" name="tp_58" onchange="changeSwitch();" value="1" />
</td>
<td>
Used to activate an EPROM sequencer already in the bootphase.<br />
Only for STs with EPROM sequencer expansion.
</td>
</tr>
<tr>
<td>
Switch:
</td>
<td>
<select id="tp_59" name="tp_59" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Do not switch</option>
<option value="6">Use PSG bit 6</option>
<option value="7">Use PSG bit 7</option>
</select>
</td>
<td></td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
GE-Soft Eagle patch:
</td>
<td>
<input type="checkbox" id="tp_65" name="tp_65" value="1" />
</td>
<td>
Prevent monochrome monitor change
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Initiatoridentification:
</td>
<td>
<input type="checkbox" id="tp_66" name="tp_66" value="1" checked="checked" />
</td>
<td>
Initiatoridentification on SCSI access. TT will
report ID 7. Needed to be able to boot
from certain harddisks (eg. Quantum Empire)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td style="vertical-align: top;" >
WINX-Patch:
</td>
<td style="vertical-align: top;" >
<input type="checkbox" name="tp_winx" value="1"/><br />
Global flags: <input type="number" id="winx_global" name="winx_global" value="65535" min="0" max="65535" style="width: 6em" onchange="winxChangeGlobal();" /><br />
</td>
<td style="vertical-align: top;" >
<input type="checkbox" id="winx_g1" name="winx_g1" value="1" checked="checked" onclick="winxGlobal(1);" />40 windows (otherwise 8)<br />
<input type="checkbox" id="winx_g2" name="winx_g2" value="1" checked="checked" onclick="winxGlobal(2);" />right click to SCRENMGR (otherwise application)<br />
<input type="checkbox" id="winx_g3" name="winx_g3" value="1" checked="checked" onclick="winxGlobal(3);" />left click activates realtime (otherwise rightclick)<br />
<input type="checkbox" id="winx_g4" name="winx_g4" value="1" checked="checked" onclick="winxGlobal(4);" />activate scroll arrows<br />
<input type="checkbox" id="winx_g5" name="winx_g5" value="1" checked="checked" onclick="winxGlobal(5);" />activate scroll bars<br />
<input type="checkbox" id="winx_g6" name="winx_g6" value="1" checked="checked" onclick="winxGlobal(6);" />activate mover<br />
<input type="checkbox" id="winx_g7" name="winx_g7" value="1" checked="checked" onclick="winxGlobal(7);" />activate sizer<br />
<input type="checkbox" id="winx_g8" name="winx_g8" value="1" checked="checked" onclick="winxGlobal(8);" />wide slider background<br />
<input type="checkbox" id="winx_g9" name="winx_g9" value="1" checked="checked" onclick="winxGlobal(9);" />contrl-click on element activates background window<br />
<input type="checkbox" id="winx_g10" name="winx_g10" value="1" checked="checked" onclick="winxGlobal(10);" />click on title puts window to background<br />
<input type="checkbox" id="winx_g11" name="winx_g11" value="1" checked="checked" onclick="winxGlobal(11);" />3D-Lock for window frame<br />
<input type="checkbox" id="winx_g12" name="winx_g12" value="1" checked="checked" onclick="winxGlobal(12);" />pattern reference point correction<br />
<input type="checkbox" id="winx_g13" name="winx_g13" value="1" checked="checked" onclick="winxGlobal(13);" />use application name for appl_find()<br />
<input type="checkbox" id="winx_g14" name="winx_g14" value="1" checked="checked" onclick="winxGlobal(14);" />change mouse cursor for realtime functions<br />
<br />
</td>
</tr>
<tr>
<td></td>
<td style="vertical-align: top;" >
Local flags: <input type="number" id="winx_local" name="winx_local" value="65535" min="0" max="65535" style="width: 6em" onchange="winxChangeLocal();" /><br />
</td>
<td style="vertical-align: top;" >
<input type="checkbox" id="winx_l1" name="winx_l1" value="1" checked="checked" onclick="winxLocal(1);" />Control elements for background windows<br />
<input type="checkbox" id="winx_l2" name="winx_l2" value="1" checked="checked" onclick="winxLocal(2);" />Minimal amount of control elements<br />
<input type="checkbox" id="winx_l3" name="winx_l3" value="1" checked="checked" onclick="winxLocal(3);" />Application accepts WM_ONTOP/WM_UNTOPPED<br />
<input type="checkbox" id="winx_l4" name="winx_l4" value="1" checked="checked" onclick="winxLocal(4);" />Optimized redraw for activation<br />
<input type="checkbox" id="winx_l5" name="winx_l5" value="1" checked="checked" onclick="winxLocal(5);" />Optimized redraw for moving<br />
<input type="checkbox" id="winx_l6" name="winx_l6" value="1" checked="checked" onclick="winxLocal(6);" />Optimized redraw for sizing<br />
<input type="checkbox" id="winx_l7" name="winx_l7" value="1" checked="checked" onclick="winxLocal(7);" />Merge redraw messages<br />
<input type="checkbox" id="winx_l8" name="winx_l8" value="1" checked="checked" onclick="winxLocal(8);" />Alert on wrong AES calls<br />
<input type="checkbox" id="winx_l9" name="winx_l9" value="1" checked="checked" onclick="winxLocal(9);" />Separate scroll arrows<br />
<input type="checkbox" id="winx_l10" name="winx_l10" value="1" checked="checked" onclick="winxLocal(10);" />Scroll arrows with boxes<br />
<input type="checkbox" id="winx_l11" name="winx_l11" value="1" checked="checked" onclick="winxLocal(11);" />Allow to move windows outside of screen<br />
<input type="checkbox" id="winx_l12" name="winx_l12" value="1" checked="checked" onclick="winxLocal(12);" />Fix wrong window positions<br />
<br />
</td>
</tr>
<tr>
<td></td>
<td style="vertical-align: top;" >
Desktop flags: <input type="number" id="winx_desktop" name="winx_desktop" value="65535" min="0" max="65535" style="width: 6em" onchange="winxChangeDesktop();" /><br />
</td>
<td style="vertical-align: top;" >
<input type="checkbox" id="winx_d1" name="winx_d1" value="1" checked="checked" onclick="winxLocal(1);" />Control elements for background windows<br />
<input type="checkbox" id="winx_d2" name="winx_d2" value="1" checked="checked" onclick="winxLocal(2);" />Minimal amount of control elements<br />
<input type="checkbox" id="winx_d3" name="winx_d3" value="1" checked="checked" onclick="winxLocal(3);" />Application accepts WM_ONTOP/WM_UNTOPPED<br />
<input type="checkbox" id="winx_d4" name="winx_d4" value="1" checked="checked" onclick="winxLocal(4);" />Optimized redraw for activation<br />
<input type="checkbox" id="winx_d5" name="winx_d5" value="1" checked="checked" onclick="winxLocal(5);" />Optimized redraw for moving<br />
<input type="checkbox" id="winx_d6" name="winx_d6" value="1" checked="checked" onclick="winxLocal(6);" />Optimized redraw for sizing<br />
<input type="checkbox" id="winx_d7" name="winx_d7" value="1" checked="checked" onclick="winxLocal(7);" />Merge redraw messages<br />
<input type="checkbox" id="winx_d8" name="winx_d8" value="1" checked="checked" onclick="winxLocal(8);" />Alert on wrong AES calls<br />
<input type="checkbox" id="winx_d9" name="winx_d9" value="1" checked="checked" onclick="winxLocal(9);" />Separate scroll arrows<br />
<input type="checkbox" id="winx_d10" name="winx_d10" value="1" checked="checked" onclick="winxLocal(10);" />Scroll arrows with boxes<br />
<input type="checkbox" id="winx_d11" name="winx_d11" value="1" checked="checked" onclick="winxLocal(11);" />Allow to move windows outside of screen<br />
<input type="checkbox" id="winx_d12" name="winx_d12" value="1" checked="checked" onclick="winxLocal(12);" />Fix wrong window positions<br />
</td>
</tr>
<tr>
<td></td>
<td>
Initial scroll delay: <input type="number" id="winx_init_scroll" name="winx_init_scroll" value="460" min="0" max="5000" style="width: 6em" /><br />
</td>
<td></td>
</tr>
<tr>
<td></td>
<td>
Continous scroll delay: <input type="number" id="winx_scroll_cont" name="winx_scroll_cont" value="40" min="0" max="5000" style="width: 6em" /><br />
</td>
<td></td>
</tr>
<tr>
<td></td>
<td>
Continous close delay: <input type="number" id="winx_close_cont" name="winx_close_cont" value="1000" min="0" max="5000" style="width: 6em" /><br />
</td>
<td></td>
</tr>
<tr>
<td></td>
<td>
Continous full delay: <input type="number" id="winx_full_cont" name="winx_full_cont" value="2000" min="0" max="5000" style="width: 6em" /><br />
</td>
<td></td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td colspan="3">
<table>
<tr>
<td>
Substitute 6x6 font:
</td>
<td>
<select id="tp_35_6" name="tp_35_6" onchange="change6x6();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Original Atari font</option>
<option value="1">Modified Atari font</option>
<option value="2">Polish font from tos306pl</option>
<option value="3">Monaco Mono</option>
</select>
</td>
<td>
<img id="tp_35_6_img" src="fonts/6x6/system.png" width="136" height="136" style="border:0" alt="Atari" />
</td>
<td>
Substitute 8x8 font:
</td>
<td>
<select id="tp_35_8" name="tp_35_8" onchange="change8x8();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Original Atari font</option>
<option value="1">Modified atari font</option>
<option value="2">Polish font from tos306pl</option>
<option value="3">Monaco Mono</option>
</select>
</td>
<td>
<img id="tp_35_8_img" src="fonts/8x8/system.png" width="168" height="168" style="border:0" alt="Atari" />
</td>
</tr>
<tr>
<td>
Substitute 8x16 font:
</td>
<td>
<select id="tp_35_16" name="tp_35_16" onchange="change8x16();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Original Atari font</option>
<option value="1">Modified Atari font</option>
<option value="2">Polish font from tos306pl</option>
<option value="3">Monaco Mono</option>
<option value="4">Blue</option>
<option value="5">Classic</option>
<option value="6">Classic 1</option>
<option value="7">Classic 2</option>
<option value="8">Computer</option>
<option value="9">Cursive</option>
<option value="10">Desktop</option>
<option value="11">Deutsch</option>
<option value="12">Eight</option>
<option value="13">Griech</option>
<option value="14">IBM G2</option>
<option value="15">Kritzel</option>
<option value="16">Mac font</option>
<option value="17">Mc Appeal</option>
<option value="18">Old</option>
<option value="19">Rally</option>
<option value="20">Skyvox</option>
<option value="21">Stencil</option>
<option value="22">Sv</option>
<option value="23">Tempus</option>
<option value="24">Thin</option>
<option value="25">Thin 1</option>
<option value="26">Typewriter</option>
</select>
</td>
<td>
<img id="tp_35_16_img" src="fonts/8x16/system.png" width="168" height="296" style="border:0" alt="Atari" />
</td>
<td>
Substitute 16x32 font:
</td>
<td>
<select id="tp_35_32" name="tp_35_32" onchange="change16x32();" style="vertical-align: middle; padding: 0 0;" >
<option value="0" selected="selected">Original Atari font</option>
<option value="3">Monaco Mono</option>
</select>
</td>
<td>
<!--
<img id="tp_35_32_img" src="fonts/16x32/system.png" width="296" height="552" style="border:0" alt="Atari" />
-->
</td>
</tr>
</table>
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Custom info text:
</td>
<td>
<input type="text" name="tp_36" maxlength="29" /><br />
</td>
<td>
This string will be displayed in place of the hyphens in the About dialog
(29 characters max., ASCII only)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Note alert icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_39_1" name="tp_39_1" accept=".ico" onchange="previewIcon('tp_39_1');" /></td>
<td><img id="tp_39_1_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_39_1');"> Default: <img src="icons/note.ico" alt="Note" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Question alert icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_39_2" name="tp_39_2" accept=".ico" onchange="previewIcon('tp_39_2');" /></td>
<td><img id="tp_39_2_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_39_2');"> Default: <img src="icons/quest.ico" alt="Question" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Stop alert icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_39_3" name="tp_39_3" accept=".ico" onchange="previewIcon('tp_39_3');" /></td>
<td><img id="tp_39_3_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_39_3');"> Default: <img src="icons/stop.ico" alt="Stop" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Floppy icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_1" name="tp_37_1" accept=".ico" onchange="previewIcon('tp_37_1');" /></td>
<td><img id="tp_37_1_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_1');"> Default: <img src="icons/floppy.ico" alt="Floppy" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Folder icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_2" name="tp_37_2" accept=".ico" onchange="previewIcon('tp_37_2');" /></td>
<td><img id="tp_37_2_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_2');"> Default: <img src="icons/folder.ico" alt="Folder" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Trashcan icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_3" name="tp_37_3" accept=".ico" onchange="previewIcon('tp_37_3');" /></td>
<td><img id="tp_37_3_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_3');"> Default: <img src="icons/trash.ico" alt="Trashcan" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Program icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_4" name="tp_37_4" accept=".ico" onchange="previewIcon('tp_37_4');" /></td>
<td><img id="tp_37_4_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_4');"> Default: <img src="icons/program.ico" alt="Program" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
File icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_5" name="tp_37_5" accept=".ico" onchange="previewIcon('tp_37_5');" /></td>
<td><img id="tp_37_5_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_5');"> Default: <img src="icons/file.ico" alt="File" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr>
<td>
Harddisk icon:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_37_12" name="tp_37_12" accept=".ico" onchange="previewIcon('tp_37_12');" /></td>
<td><img id="tp_37_12_img" src="icons/empty.gif" alt="None" width="32" height="32" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_37_12');"> Default: <img src="icons/harddisk.ico" alt="Harddisk" width="32" height="32" style="border:0" /></a>
This must be a 32x32 icon.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Arrow cursor:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_0" name="tp_38_0" accept=".cur" onchange="previewIcon('tp_38_0');" /></td>
<td><img id="tp_38_0_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_0');"> Default: <img src="mform/mform0.cur" alt="Arrow" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Text cursor:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_1" name="tp_38_1" accept=".cur" onchange="previewIcon('tp_38_1');" /></td>
<td><img id="tp_38_1_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_1');"> Default: <img src="mform/mform1.cur" alt="Text" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Busy bee:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_2" name="tp_38_2" accept=".cur" onchange="previewIcon('tp_38_2');" /></td>
<td><img id="tp_38_2_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_2');"> Default: <img src="mform/mform2.cur" alt="Busy Bee" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Point hand:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_3" name="tp_38_3" accept=".cur" onchange="previewIcon('tp_38_3');" /></td>
<td><img id="tp_38_3_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_3');"> Default: <img src="mform/mform3.cur" alt="Point hand" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Flat hand:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_4" name="tp_38_4" accept=".cur" onchange="previewIcon('tp_38_4');" /></td>
<td><img id="tp_38_4_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_4');"> Default: <img src="mform/mform4.cur" alt="Flat hand" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Thin cross:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_5" name="tp_38_5" accept=".cur" onchange="previewIcon('tp_38_5');" /></td>
<td><img id="tp_38_5_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_5');"> Default: <img src="mform/mform5.cur" alt="Thin cross" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Thick cross:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_6" name="tp_38_5" accept=".cur" onchange="previewIcon('tp_38_6');" /></td>
<td><img id="tp_38_6_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_6');"> Default: <img src="mform/mform6.cur" alt="Thick cross" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr>
<td>
Outline cross:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_38_7" name="tp_38_7" accept=".cur" onchange="previewIcon('tp_38_7');" /></td>
<td><img id="tp_38_7_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_38_7');"> Default: <img src="mform/mform7.cur" alt="Outline cross" width="16" height="16" style="border:0" /></a>
This must be a 16x16 cursor.
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr><td colspan="3">
All files together must not exceed 64Kb. Also take care not to exceed
the total ROM size (there are approximately 8K free in the 2.06 version).
The files for 2.06 and 3.06 are identical, so you can use the
same resource files for both versions.
</tr>
<tr>
<td>
Substitute GEM resource:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_40_1" name="tp_40_1" accept=".rsc" onchange="previewIcon('tp_40_1');" /></td>
<td><img id="tp_40_1_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_40_1');">Reset</a><br />
</td>
</tr>

<tr>
<td>
Substitute DESKTOP resource:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_40_2" name="tp_40_2" accept=".rsc" onchange="previewIcon('tp_40_2');" /></td>
<td><img id="tp_40_2_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_40_2');">Reset</a><br />
</td>
</tr>

<tr>
<td>
Substitute NEWDESK.INF file:
</td>
<td>
<table>
<tr>
<td><input type="file" id="tp_40_3" name="tp_40_3" accept=".inf" onchange="previewIcon('tp_40_3');" /></td>
<td><img id="tp_40_3_img" src="icons/empty.gif" alt="None" width="16" height="16" style="border:0" /></td>
</tr>
</table>
</td>
<td>
<a href="javascript:killIcon('tp_40_3');">Reset</a><br />
</td>
</tr>


<tr><td colspan="3"><hr /></td></tr>

<tr><td>&nbsp;</td><td></td><td></td></tr>

<tr>
<td>
<input id="runit" style="background-color: #cccccc; font-weight: bold;" type="submit" value="Create TOS image" />
</td>
<td></td><td></td>
</tr>

</table>
</fieldset>
</form>
</td>
</tr>

<tr><td>&nbsp;</td></tr>

</table>

<div style="text-align:center">
<p>
<a href="../index.html"> <img src="../images/home1.png" width="180" height="60" style="border:0" alt="Home" /></a>
</p>
</div>

<div style="text-align:center">
<p>
<a href="https://validator.w3.org/check?uri=referer"><img
        src="../images/valid-xhtml11.png" height="31" width="88"
        alt="Valid XHTML 1.1!" /></a>
</p>
</div>

</div>
</body>
</html>
