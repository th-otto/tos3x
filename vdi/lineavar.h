/*
******************************  lineavar.h  **********************************
* 
* $Revision: 3.2 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/lineavar.h,v $
* ============================================================================
* $Author: lozben $	$Date: 91/09/10 19:55:56 $     $Locker:  $
* ============================================================================
*
* $Log:	lineavar.h,v $
* Revision 3.2  91/09/10  19:55:56  lozben
* Added "int16_t qCircle[80]" to the linea structure.
* 
* Revision 3.1  91/07/29  16:52:14  lozben
* Definition of the line1010 variable structure.
* 
******************************************************************************
*/

#ifndef _LINEAVAR_H_
#define _LINEAVAR_H_

typedef struct vdiVars {
    int16_t        _angle;
    int16_t        begAng;
    const FONT_HEAD   *curFont;           /* pointer to current font              */
    int16_t        delAng;
    int16_t        deltaY;
    int16_t        deltaY1;
    int16_t        deltaY2;
    int16_t        endAng;
    int16_t        filIntersect;
    int16_t        fillMaxY;
    int16_t        fillMinY;
    int16_t        nSteps;
    int16_t        oDeltaY;
    int16_t        sBegstY;
    int16_t        sEndstY;
    int16_t        sFilCol;
    int16_t        sFillPer;
    int16_t        sPatMsk;
    int16_t        *sPatPtr;
    int16_t        _start;
    int16_t        xC;
    int16_t        xRad;
    int16_t        y;
    int16_t        yC;
    int16_t        yRad;

    int16_t        mPosHx;	           /* Mouse hot spot - x coord             */
    int16_t        mPosHy;	           /* Mouse hot spot - y coord             */
    int16_t        mPlanes;            /* Ms planes (reserved, but we used it) */
    int16_t        mCdbBg;             /* Mouse background color as pel value  */
    int16_t        mCdbFg;             /* Mouse foreground color as pel value  */
    int16_t        maskForm[32];       /* Storage for ms cursor mask and form  */

    int16_t        inqTab[45];         /* info returned by vq_extnd VDI call   */
    int16_t        devTab[45];         /* info returned by v_opnwk VDI call    */

    int16_t        gCurX;              /* current mouse cursor X position      */
    int16_t        gCurY;              /* current mouse cursor Y position      */

    int16_t        hideCnt;            /* depth at which the ms is hidden      */
    int16_t        mouseBt;            /* current mouse button status          */
    int16_t        reqCol[16][3];      /* internal data for vq_color           */
    int16_t        sizTab[15];         /* size in device coordinates           */
    int16_t        termCh;             /* 16 bit character info                */
    int16_t        chcMode;            /* the mode of the Choice device        */
    ATTRIBUTE   *curWork;              /* pointer to current works attributes  */
    const FONT_HEAD   *defFont;              /* pointer to default font head         */
    const FONT_HEAD   *fontRing[4];          /* ptrs to link list of fnt hdrs        */
    int16_t        iniFontCount;       /* # of fonts in the FONT_RING lists    */
    int16_t        lineCW;             /* current line width                   */
    int16_t        locMode;            /* the mode of the Locator device       */
    int16_t        numQCLines;         /* # of line in the quarter circle      */

    int32_t        trap14Sav;	       /* space to save the return address     */
    int32_t        colOrMask;          /* some modes this is ored in VS_COLOR  */
    int32_t        colAndMask;         /* some modes this is anded in VS_COLOR */
    int32_t	trap14BSav;	               /* space to sav ret adr (for reentrency)*/

    int16_t	reserved0[32];	           /* reserved				    */
    int16_t        strMode;            /* the mode of the String device        */
    int16_t        valMode;            /* the mode of the Valuator device      */
    char        curMsStat;             /* Current mouse status                 */
    char        reserved1;             /* reserved                             */
    int16_t        disabCnt;           /* hide depth of alpha cursor           */


    /*
     * the next 5 bytes are used as a communication
     * block to the vblank cursor draw routine.
     * They must be contiguous!!!!!!
     */
    int16_t        xyDraw[2];          /* x,y communication block.             */
    char        drawFlag;              /* Non-zero means draw ms frm on vblank */

    char        mouseFlag;             /* Non-zero if mouse ints disabled      */

    int32_t        trap1Sav;           /* space to save return address         */
    int16_t        savCXY[2];          /* save area for cursor cell coords.    */

    int16_t        saveLen;            /* height of saved form                 */
    int16_t        *saveAddr;          /* screen addr of 1st word of plane 0   */
    int16_t        saveStat;           /* cursor save status                   */
    int32_t        saveArea[64];       /* save up to 4 planes. 16 longs/plane  */

    int16_t        (*timAddr) PROTO((NOTHING));       /* ptr to user installed routine        */
    int16_t        (*timChain) PROTO((NOTHING));      /* jmps here when done with above       */

    int16_t        (*userBut) PROTO((NOTHING));       /* user button vector                   */
    int16_t        (*userCur) PROTO((NOTHING));       /* user cursor vector                   */
    int16_t        (*userMot) PROTO((NOTHING));       /* user motion vector                   */

    int16_t        vCelHt;	           /* height of character cell             */
    int16_t        vCelMx;	           /* maximum horizontal cell index        */
    int16_t        vCelMy;             /* maximum vertical cell index          */
    int16_t        vCelWr;             /* screen width (bytes) * cel_ht        */
    int16_t        vColBg;             /* character cell text background color */
    int16_t        vColFg;             /* character cell text foreground color */
    int16_t        *vCurAd;            /* cursor address                       */
    int16_t        vCurOff;            /* byte ofsset to cur from screen base  */
    int16_t        vCurCx;             /* cursor cell X position               */
    int16_t        vCurCy;             /* cursor cell Y position               */
    char        vCTInit;               /* vCurTim reload value.                */
    char        vCurTim;               /* cursor blink rate (# of vblanks)     */
    int16_t        *vFntAd;            /* address of monospaced font data      */
    int16_t        vFntNd;             /* last ASCII code in font              */
    int16_t        vFntSt;             /* first ASCII code in font             */
    int16_t        vFntWr;             /* width of font form in bytes          */
    int16_t        vHzRez;             /* horizontal pixel resolution          */
    int16_t        *vOffAd;            /* address of font offset table         */

/*
 *              bit 0	cursor flash		0:disabled  1:enabled
 *		bit 1	flash state		0:off       1:on
 *  not used    bit 2   cursor visibility       0:invisible 1:visible
 *		bit 3	end of line		0:overwrite 1:wrap
 *		bit 4	reverse video		0:on        1:off
 *		bit 5	cursor position	saved	0:false	    1:true
 *  not used	bit 6	critical section	0:false	    1:true
 */
    char        vStat0;                /* cursor display mode (look above)     */

    char        vDelay;                /* cursor redisplay period              */
    int16_t        vVtRez;             /* vertical resolution of the screen    */
    int16_t        bytesLin;	       /* copy of vLinWr for concat            */

/*
 * the lineavars point here:
 */
    int16_t        vPlanes;	           /* number of video planes               */
    int16_t        vLinWr;	           /* number of bytes/video line           */

    int16_t        *contrl;            /* ptr to the CONTRL array              */
    int16_t        *intin;             /* ptr to the INTIN array               */
    int16_t        *ptsin;             /* ptr to the PTSIN array               */
    int16_t        *intout;            /* ptr to the INTOUT array              */
    int16_t        *ptsout;            /* ptr to the PTSOUT array              */

    /*
     * The following 4 variables are accessed by the line-drawing
     * routines as an array (to allow post-increment addressing).
     * THEY MUST BE CONTIGUOUS !!
     */
    int16_t        fgBp1;              /* foreground bit plane #1 value        */
    int16_t        fgBp2;              /* foreground bit plane #2 value        */
    int16_t        fgBp3;              /* foreground bit plane #3 value        */
    int16_t        fgBp4;              /* foreground bit plane #4 value        */

    int16_t        lstLin;             /* 0 => not last line of polyline       */
    int16_t        lnMask;             /* line style mask.                     */
    int16_t        wrtMode;	    /* writing mode.                        */


    int16_t        x1;                 /* X1 coordinate                        */
    int16_t        y1;                 /* Y1 coordinate                        */
    int16_t        x2;                 /* X2 coordinate                        */
    int16_t        y2;                 /* Y2 coordinate                        */

    int16_t        *patPtr;            /* ptr to pattern.                      */
    int16_t        patMsk;             /* pattern index. (mask)                */
    int16_t        multiFill;          /* multiplane fill flag. (0 => 1 plane) */

    int16_t        clip;               /* clipping flag.                       */
    int16_t        xMnClip;            /* x minimum clipping value.            */
    int16_t        yMnClip;            /* y minimum clipping value.            */
    int16_t        xMxClip;            /* x maximum clipping value.            */
    int16_t        yMxClip;            /* y maximum clipping value.            */

    int16_t        xAccDda;            /* accumulator for x DDA                */
    int16_t        ddaInc;             /* the fraction to be added to the DDA  */
    int16_t        tSclsts;            /* scale up or down flag.               */
    int16_t        monoStatus;         /* non-zero - cur font is monospaced    */
    int16_t        sourceX;            /* X coord of character in font         */
    int16_t        sourceY;            /* Y coord of character in font         */
    int16_t        destX;              /* X coord of character on screen       */
    int16_t        destY;              /* X coord of character on screen       */
    int16_t        delX;               /* width of character                   */
    int16_t        delY;               /* height of character                  */
    const uint16_t *fBase;             /* pointer to font data                 */
    int16_t        fWidth;             /* offset,segment and form with of font */
    int16_t        style;              /* special effects                      */
    int16_t        liteMask;           /* special effects                      */
    int16_t        skewMask;           /* special effects                      */
    int16_t        weight;             /* special effects                      */
    int16_t        rOff;	           /* Skew offset above baseline           */
    int16_t        lOff;	           /* Skew offset below baseline           */
    int16_t        scale;              /* replicate pixels                     */
    int16_t        chUp;               /* character rotation vector            */
    int16_t        textFg;             /* text foreground color                */
    int16_t        *scrtchP;           /* pointer to base of scratch buffer    */
    int16_t        scrPt2;             /* large buffer base offset             */
    int16_t        textBg;             /* text background color                */
    int16_t        copyTran;           /* cp rstr frm type flag (opaque/trans) */
    int16_t        (*quitFill) PROTO((NOTHING));      /* ptr to routine for quitting seedfill */
    int16_t	(*UserDevInit) PROTO((NOTHING));          /* ptr to user routine before dev_init  */
    int16_t	(*UserEscInit) PROTO((NOTHING));          /* ptr to user routine before esc_init  */
    int32_t	reserved2[8];	           /* reserved				    */

    VOID        (**routines) PROTO((NOTHING));     /* ptr to primitives vector list	    */
    const SCREENDEF   *curDev;	       /* ptr to a current device structure    */
    int16_t        bltMode;            /* 0: soft BiT BLiT 1: hard BiT BLiT    */

    /*
     * Stuff for 8 plane VDI
     */
    int16_t        reserved3;          /* reserved				    */

    int16_t        reqXCol[240][3];    /* extended request color array         */

    int16_t        *svBlkPtr;          /* points to the proper save block      */
    int32_t        fgBPlanes;          /* fg bit plns flags (bit 0 is plane 0) */

    /*
     * The following 4 variables are accessed by the line-drawing routines
     * as an array (to allow post-increment addressing).
     * THEY MUST BE CONTIGUOUS !!
     */
    int16_t        fgBP5;              /* foreground bitPlane #5 value.        */
    int16_t        fgBP6;              /* foreground bitPlane #6 value.        */
    int16_t        fgBP7;              /* foreground bitPlane #7 value.        */
    int16_t        fgBP8;              /* foreground bitPlane #8 value.        */

    /*
     * we don't get rid of the old area for compatibility reasons
     */
    int16_t        _saveLen;           /* height of saved form                 */
    int16_t        *_saveAddr;         /* screen addr of 1st word of plane 0   */
    int16_t        _saveStat;          /* cursor save status                   */
#if VIDEL_SUPPORT
    int32_t        _saveArea[256];     /* save up to 8 planes. 16 longs/plane  */
#else
    int16_t        _saveArea[256];     /* save up to 8 planes. 16 words/plane  */
#endif
    int16_t	qCircle[80];	           /* space to build circle coordinates    */

/*============================== NEW STUFF =================================*/

#if VIDEL_SUPPORT
    int16_t	bytPerPix;	               /* number of bytes per pixel (0 if < 1) */
    int16_t	formId;		               /* scrn form 2 ST, 1 stndrd, 3 pix      */
    int32_t	vlColBg;	               /* escape background color (long value) */
    int32_t	vlColFg;	               /* escape foreground color (long value) */
    int32_t	palMap[256];	           /* either a mapping of reg's or true val */
    int16_t	(*primitives[40])();	   /* space to copy vectors into	    */
#endif
} VDIVARS;

/* line_a variable structure */
extern	VDIVARS	*la;

#endif
