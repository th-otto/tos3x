<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
          "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>TOS Patchers</title>
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
<td>
<select id="tosversion" name="tosversion">
<option value="206"> 2.06</option>
<option value="306"> 3.06</option>
</select>
</td>
</tr>

<tr>
<td>
Country:
</td>
<td>
<select id="country" name="country">
<option value="us"> USA</option>
<option value="de"> Germany</option>
<option value="fr"> France</option>
<option value="uk"> United Kingdom</option>
<option value="es"> Spain</option>
<option value="it"> Italy</option>
<option value="se"> Sweden</option>
<option value="sf"> Switzerland (French)</option>
<option value="sg"> Switzerland (German)</option>
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
<tr>

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
<tr>

<tr>
<td>
Reset resident:
</td>
<td>
<input type="checkbox" name="tp_03" value="1" /><br />
</td>
<td>
Modified reset routine. </br>
Will keep RAM-TOS resident even after Hardreset (via keyboard)
</td>
<tr>

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
