<!DOCTYPE html>
<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>TOS Patches</title>
<meta http-equiv="content-type" content="text/html;charset=UTF-8" />
<meta name="keywords" content="ORCS, CAT, GC, PBEM, PBM, GC-Ork, GCORK, ARAnyM, UDO, EmuTOS" />
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
<form action="tospatch.php" method="get" id="tospatch" enctype="application/x-www-form-urlencoded">
<fieldset>
<table>

<tr>
<td>
TOS version:
</td>
<td colspan="2">
<select id="tosversion" name="tosversion">
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
Reduce the delay after which - on system start - <br />
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
Fixes a bug the autoexec routine (see ST-Computer 1/90)
</td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Alternative image for bombs:
</td>
<td>
<input type="checkbox" name="tp_16" value="1" /><br />
</td>
<td>
Replaces the Atari bomb images <img src="bomb.bmp" width="16" height="16" style="border:0" alt="Bomb" />
with the original mushrooms <img src="mushroom.bmp" width="16" height="16" style="border:0" alt="Mushroom" />
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
The blitter will be disavowed and ignored by TOS
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
<input type="checkbox" name="tp_20" value="1" checked="checked" /><br />
</td>
<td>
Original value is 30 seconds. Minimum value is 5 seconds.
Does not work with the Atari Laser Printer.
</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
<input type="number" name="prntimeout" value="30" min="5" max="30" style="width: 4em" /> seconds<br />
</td>
<td></td>
</tr>

<tr><td colspan="3"><hr /></td></tr>

<tr>
<td>
Set conterm system variable:
</td>
<td>
<input type="checkbox" name="tp_21" value="1" /><br />
</td>
<td>
<input type="checkbox" id="conterm_bell" name="conterm_bell" value="1" checked="checked" onclick="contermBell();" />Bit 2 set: bell on CNTRL-G<br />
<input type="checkbox" id="conterm_repeat" name="conterm_repeat" value="1" checked="checked" onclick="contermRepeat();" />Bit 1 set: key repeat on<br />
<input type="checkbox" id="conterm_click" name="conterm_click" value="1" checked="checked" onclick="contermClick();" />Bit 0 set: key click on<br />
</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
<input type="number" id="conterm" name="conterm" value="7" min="0" max="7" style="width: 4em" onchange="contermChange();" /><br />
</td>
<td></td>
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
Produces errors with some driver, take care!
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
when 2 drives are connected.
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
<img id="tp_35_32_img" src="fonts/16x32/system.png" width="296" height="552" style="border:0" alt="Atari" />
</td>
</tr>
</table>
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
