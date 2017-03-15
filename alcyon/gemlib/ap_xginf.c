#include "aeslib.h"

#ifndef WF_WINX
#define WF_WINX         0x5758
#endif

short __magix;

static short has_appl_getinfo(void)
{
    static short has_agi = -1; /* do the check only once */
    short out1 = 0, out2, out3, out4;
    
    /* check for appl_getinfo() being present */
    if (has_agi < 0)
    {
        has_agi = 0;
        /* AES 4.0? */
        if (global[0] >= 0x400)
             has_agi = 1;
        else
        /* Mag!X 2.0? */
        if (__magix >= 0x200)
            has_agi = 2;
        else
        if (appl_find( "?AGI\0\0\0\0") >= 0)
            has_agi = 3;
        else
        /* WiNX >= 2.2 ? */
        if (wind_get(0, WF_WINX, &out1, &out2, &out3, &out4) == WF_WINX &&
        	(out1 & 0xfff) >= 0x220)
            has_agi = 4;
    }
    return has_agi;
}


short appl_xgetinfo(P(short) type, P(short *) out1, P(short *) out2, P(short *) out3, P(short *) out4)
PP(short type;)
PP(short *out1;)
PP(short *out2;)
PP(short *out3;)
PP(short *out4;)
{
    short ret;

    /* no appl_getinfo? return error code */
	if (!has_appl_getinfo() || (ret = appl_getinfo(type, out1, out2, out3, out4)) == 0)
	{
	    if (out1 != NULL)
	    	*out1 = 0;
	    if (out2 != NULL)
		    *out2 = 0;
		if (out3 != NULL)
		    *out3 = 0;
		if (out4 != NULL)
		    *out4 = 0;
		return 0;
	}
	return ret;
}
