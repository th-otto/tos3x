function contermClick()
{
	var c = document.getElementById('conterm_click');
	var f = document.getElementById('conterm');
	f.value = (f.value & 6) + c.checked * 1;
}
function contermRepeat()
{
	var c = document.getElementById('conterm_repeat');
	var f = document.getElementById('conterm');
	f.value = (f.value & 5) + c.checked * 2;
}
function contermBell()
{
	var c = document.getElementById('conterm_bell');
	var f = document.getElementById('conterm');
	f.value = (f.value & 3) + c.checked * 4;
}
function contermChange()
{
	var c;
	var f = document.getElementById('conterm');
	c = document.getElementById('conterm_click');
	c.checked = (f.value & 1) != 0;
	c = document.getElementById('conterm_repeat');
	c.checked = (f.value & 2) != 0;
	c = document.getElementById('conterm_bell');
	c.checked = (f.value & 4) != 0;
}
function changeFuji()
{
	var f = document.getElementById('fuji');
	var i = document.getElementById('fujiimg');
	if (f.value == 0) {
		i.src = "fuji/atari.bmp";
	}
	if (f.value == 1) {
		i.src = "fuji/pak3.bmp";
	}
	if (f.value == 2) {
		i.src = "fuji/gib_dos.bmp";
	}
	if (f.value == 3) {
		i.src = "fuji/gt_tos.bmp";
	}
	if (f.value == 4) {
		i.src = "fuji/schegale.bmp";
	}
	if (f.value == 5) {
		i.src = "fuji/ufp_logo.bmp";
	}
	if (f.value == 6) {
		i.src = "fuji/x.bmp";
	}
}
function change6x6()
{
	var f = document.getElementById('tp_35_6');
	var i = document.getElementById('tp_35_6_img');
	if (f.value == 0) {
		i.src = "fonts/6x6/system.png";
	}
	if (f.value == 1) {
		i.src = "fonts/6x6/tospatch.png";
	}
	if (f.value == 2) {
		i.src = "fonts/6x6/polish.png";
	}
	if (f.value == 3) {
		i.src = "fonts/6x6/monaco.png";
	}
}
function change8x8()
{
	var f = document.getElementById('tp_35_8');
	var i = document.getElementById('tp_35_8_img');
	if (f.value == 0) {
		i.src = "fonts/8x8/system.png";
	}
	if (f.value == 1) {
		i.src = "fonts/8x8/tospatch.png";
	}
	if (f.value == 2) {
		i.src = "fonts/8x8/polish.png";
	}
	if (f.value == 3) {
		i.src = "fonts/8x8/monaco.png";
	}
}
function change8x16()
{
	var f = document.getElementById('tp_35_16');
	var i = document.getElementById('tp_35_16_img');
	if (f.value == 0) {
		i.src = "fonts/8x16/system.png";
	}
	if (f.value == 1) {
		i.src = "fonts/8x16/tospatch.png";
	}
	if (f.value == 2) {
		i.src = "fonts/8x16/polish.png";
	}
	if (f.value == 3) {
		i.src = "fonts/8x16/monaco.png";
	}
	if (f.value == 4) {
		i.src = "fonts/8x16/blue.png";
	}
	if (f.value == 5) {
		i.src = "fonts/8x16/classic.png";
	}
	if (f.value == 6) {
		i.src = "fonts/8x16/classic1.png";
	}
	if (f.value == 7) {
		i.src = "fonts/8x16/classic2.png";
	}
	if (f.value == 8) {
		i.src = "fonts/8x16/computer.png";
	}
	if (f.value == 9) {
		i.src = "fonts/8x16/cursive.png";
	}
	if (f.value == 10) {
		i.src = "fonts/8x16/desktop.png";
	}
	if (f.value == 11) {
		i.src = "fonts/8x16/deutsch.png";
	}
	if (f.value == 12) {
		i.src = "fonts/8x16/eight.png";
	}
	if (f.value == 13) {
		i.src = "fonts/8x16/griech.png";
	}
	if (f.value == 14) {
		i.src = "fonts/8x16/ibm_g2.png";
	}
	if (f.value == 15) {
		i.src = "fonts/8x16/kritzel.png";
	}
	if (f.value == 16) {
		i.src = "fonts/8x16/mac_font.png";
	}
	if (f.value == 17) {
		i.src = "fonts/8x16/mcappeal.png";
	}
	if (f.value == 18) {
		i.src = "fonts/8x16/old.png";
	}
	if (f.value == 19) {
		i.src = "fonts/8x16/rally.png";
	}
	if (f.value == 20) {
		i.src = "fonts/8x16/skyvox.png";
	}
	if (f.value == 21) {
		i.src = "fonts/8x16/stencil.png";
	}
	if (f.value == 22) {
		i.src = "fonts/8x16/sv.png";
	}
	if (f.value == 23) {
		i.src = "fonts/8x16/tempus.png";
	}
	if (f.value == 24) {
		i.src = "fonts/8x16/thin.png";
	}
	if (f.value == 25) {
		i.src = "fonts/8x16/thin1.png";
	}
	if (f.value == 26) {
		i.src = "fonts/8x16/typewrit.png";
	}
}
function change16x32()
{
	var f = document.getElementById('tp_35_32');
	var i = document.getElementById('tp_35_32_img');
	if (f.value == 0) {
		i.src = "fonts/16x32/system.png";
	}
	if (f.value == 3) {
		i.src = "fonts/16x32/monaco.png";
	}
}
function onload()
{
	changeFuji();
	change6x6();
	change8x8();
	change8x16();
	change16x32();
}