typedef struct
{
   /*  0 */ VOIDPTR  pb_scrptr;  /* Pointer to start of screen memory */
   /*  4 */ int16_t  pb_offset;  /* Offset to be added to this        */
   /*  6 */ int16_t  pb_width;   /* Screen width in dots              */
   /*  8 */ int16_t  pb_height;  /* Screen height in dots             */
   /* 10 */ int16_t  pb_left;    /* Left margin in dots               */
   /* 12 */ int16_t  pb_right;   /* Right margin in dots              */
   /* 14 */ int16_t  pb_screz;   /* Resolution                        */
   /* 16 */ int16_t  pb_prrez;   /* Printer type (Atari/Epson)        */
   /* 18 */ VOIDPTR  pb_colptr;  /* Pointer to color palette          */
   /* 22 */ int16_t  pb_prtype;  /* 0: Atari matrix monochrome
                           1: Atari matrix color
                           2: Atari daisywheel monochrome
                           3: Epson matrix monochrome        */
   /* 24 */ int16_t  pb_prport;  /* Centronics/RS-232 port            */
   /* 26 */ VOIDPTR  pb_mask;    /* Pointer to halftone mask          */
   /* 30 */ 
} PBDEF;

