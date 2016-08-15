#include <portab.h>
#include <falcon.h>

struct horsync {
	unsigned short hht;
	unsigned short hbb;
	unsigned short hbe;
	unsigned short hdb;
	unsigned short hde;
	unsigned short hss;
};

struct versync {
	unsigned short vft;
	unsigned short vbb;
	unsigned short vbe;
	unsigned short vdb;
	unsigned short vde;
	unsigned short vss;
};

#ifdef __ALCYON__
#  define video_control vdoctrl
#  define video_clock   vdoclk
#endif

extern struct horsync horsync_184E;
extern struct horsync *videl_horsync;
extern struct versync versync_185a;
extern struct versync *videl_versync;
extern short scanline_offset; /* 0x1866 */
extern short scanline_width; /* 0x1868 */
extern short video_control; /* 0x186a */
extern short video_clock; /* 0x186c */
extern long x186e;

extern VOID xbios_Vsync(VOID);

VOID CDECL set_videomode PROTO((unsigned short mode));

/* TOS404: 00e0835a */
VOID CDECL set_videomode(P(unsigned short, mode))
PP(register unsigned short, mode;)
{
	union {
		UBYTE bytes[4];
		long addr;
	} old_vbase;
	
	xbios_Vsync();
	*videl_horsync = horsync_184E;
	*videl_versync = versync_185a;
	if (x186e != 0)
	{
		UBYTE c;
		register char *high, *mid, *low;
		
		old_vbase.bytes[0] = 0;
		high = (char *)0xffff8201l;
		c = *high;
		old_vbase.bytes[1] = c;
		mid = (char *)0xffff8203l;
		c = *mid;
		old_vbase.bytes[2] = c;
		low = (char *)0xffff820dl;
		c = *low;
		old_vbase.bytes[3] = c;
		old_vbase.addr += x186e;
		*high = old_vbase.bytes[1];
		*mid = old_vbase.bytes[2];
		*low = old_vbase.bytes[3];
	}
	*((volatile short *)0xffff820el) = scanline_offset;
	*((volatile short *)0xffff8210l) = scanline_width;
	*((volatile short *)0xffff82c2l) = video_control;
	*((volatile short *)0xffff82c0l) = video_clock;
	*((volatile short *)0xffff8266l) = 0;
	switch (mode & 0x07)
	{
	case VIDEL_TRUECOLOR:
		*((short *)0xffff8266l) = 0x0100;
		break;
	case VIDEL_8BPP:
		*((short *)0xffff8266l) = 0x0010;
		break;
	case VIDEL_4BPP:
		if (mode & VIDEL_COMPAT)
		{
			*((char *)0xffff8260l) = 0;
			if (mode & VIDEL_VGA)
				*((short *)0xffff82c2l) |= 0x01;
		}
		break;
	case VIDEL_2BPP:
		*((char *)0xffff8260l) = 1;
		*((short *)0xffff8210l) = scanline_width;
		*((short *)0xffff82c2l) = ((mode & (VIDEL_COMPAT|VIDEL_VGA)) == (VIDEL_COMPAT|VIDEL_VGA)) | video_control;
		break;
	case VIDEL_1BPP:
		if ((((*((char *)0xffff8006l)) >> 6) & 0x03) == 0)
			*((volatile char *)0xffff8260l) = 2;
		else
			*((volatile short *)0xffff8266l) = 0x400;
		break;
	}
}

/*
[00e0835a] 4e56 fffc                 link      a6,#$FFFC
[00e0835e] 48e7 1100                 movem.l   d3/d7,-(a7)
[00e08362] 3e2e 0008                 move.w    8(a6),d7

[00e083fe] 2007                      move.l    d7,d0
[00e08400] 0240 0007                 andi.w    #$0007,d0
[00e08404] 0c40 0005                 cmpi.w    #$0005,d0
[00e08408] 6400 009a                 bcc       $00E084A4
[00e0840c] 303b 0206                 move.w    $00E08414(pc,d0.w),d0
[00e08410] 4efb 0004                 jmp       $00E08416(pc,d0.w)
casetab:
[00e08414] 006c                      dc.w case0-casetab-2
           003a                      dc.w case1-casetab-2
           001c                      dc.w case2-casetab-2
[00e0841a] 0012                      dc.w case3-casetab-2
           0008                      dc.w case4-casetab-2
case4:
[00e0841e] 307c 8266                 move.w    #$8266,a0
[00e08422] 30bc 0100                 move.w    #$0100,(a0)
[00e08426] 607c                      bra.s     $00E084A4
case3:
[00e08428] 307c 8266                 move.w    #$8266,a0
[00e0842c] 30bc 0010                 move.w    #$0010,(a0)
[00e08430] 6072                      bra.s     $00E084A4
case2:
[00e08432] 0807 0007                 btst      #7,d7
[00e08436] 676c                      beq.s     $00E084A4
[00e08438] 307c 8260                 move.w    #$8260,a0
[00e0843c] 4210                      clr.b     (a0)
[00e0843e] 0807 0004                 btst      #4,d7
[00e08442] 6760                      beq.s     $00E084A4
[00e08444] 307c 82c2                 move.w    #$82C2,a0
[00e08448] 08e8 0000 0001            bset      #0,1(a0)
[00e0844e] 6054                      bra.s     $00E084A4
case1:
[00e08450] 307c 8260                 move.w    #$8260,a0
[00e08454] 10bc 0001                 move.b    #$01,(a0)
[00e08458] 307c 8210                 move.w    #$8210,a0
[00e0845c] 30b9 0000 1868            move.w    $00001868,(a0)
[00e08462] 307c 82c2                 move.w    #$82C2,a0
[00e08466] 2007                      move.l    d7,d0
[00e08468] 0240 0090                 andi.w    #$0090,d0
[00e0846c] 0c40 0090                 cmpi.w    #$0090,d0
[00e08470] 57c1                      seq       d1
[00e08472] 0241 0001                 andi.w    #$0001,d1
[00e08476] 3039 0000 186a            move.w    $0000186A,d0
[00e0847c] 8041                      or.w      d1,d0
[00e0847e] 3080                      move.w    d0,(a0)
[00e08480] 6022                      bra.s     $00E084A4
case0:
[00e08482] 307c 8006                 move.w    #$8006,a0
[00e08486] 1010                      move.b    (a0),d0
[00e08488] 4880                      ext.w     d0
[00e0848a] ec40                      asr.w     #6,d0
[00e0848c] 0240 0003                 andi.w    #$0003,d0
[00e08490] 660a                      bne.s     $00E0849C
[00e08492] 307c 8260                 move.w    #$8260,a0
[00e08496] 10bc 0002                 move.b    #$02,(a0)
[00e0849a] 6008                      bra.s     $00E084A4
[00e0849c] 307c 8266                 move.w    #$8266,a0
[00e084a0] 30bc 0400                 move.w    #$0400,(a0)
default:
[00e084a4] 4cdf 0088                 movem.l   (a7)+,d3/d7
[00e084a8] 4e5e                      unlk      a6
[00e084aa] 4e75                      rts       

*/
