<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
          "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>TOS Patches</title>
<meta http-equiv="content-type" content="text/html;charset=UTF-8" />
<meta name="keywords" content="ORCS, CAT, GC, PBEM, PBM, GC-Ork, GCORK, ARAnyM, UDO, EmuTOS" />
<link rel="stylesheet" type="text/css" href="tospatch.css" />
</head>

<body>
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

<tr><td>&nbsp;</td></tr>

<tr>
<td colspan="2">
Patches:
</td>
</tr>

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
</tr>

<tr>
<td>
Set conterm system variable:
</td>
<td>
<input type="checkbox" name="tp_21" value="1" /><br />
</td>
<td>
Bit 2 set: bell on CNTRL-G<br />
Bit 1 set: key repeat on<br />
Bit 0 set: key click on<br />
</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
<input type="number" name="conterm" value="7" min="0" max="7" style="width: 4em" /><br />
</td>
</tr>

<tr><td>&nbsp;</td></tr>

<tr>
<td>
<input id="runit" style="background-color: #cccccc; font-weight: bold;" type="submit" value="Create TOS image" />
</td>
</tr>

</table>
</fieldset>
</form>
</td>

<tr><td>&nbsp;</td></tr>

</tr>
</table>

<div style="text-align:center">
<p>
<a href="../index.html"> <img src="../images/home1.png" width="180" height="60" style="border:0" alt="Home" /></a>
</p>
</div>

</body>
</html>
