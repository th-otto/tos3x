/*
*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/romcart.c,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:30:35 $	$Locker: kbad $
* =======================================================================
*  $Log:	romcart.c,v $
* Revision 2.2  89/04/26  18:30:35  mui
* TT
* 
* Revision 2.1  89/02/22  05:32:18  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:36:19  lozben
* Initial revision
* 
*************************************************************************
*/
/*	ROMCART.C		2/26/85 - 04/14/85	Lowell Webster	*/
/*	remove cart_start	06/10/85		Mike Schmal	*/
/*	Fix the cart_exe	07/09/85		Derek Mui	*/
/*	Fix the cart_app	3/21/86			Derek Mui	*/
/*	Added sh_rom at cart_find	3/21/86		Derek Mui	*/
/*	Comment out cart_app	7/2/86			Derek Mui	*/
/*	Take out sh_rom		7/2/86			Derek Mui	*/
/*	Clean up		1/21/88			D.Mui		*/
/*	Added c_sfirst()	6/18/90			D.Mui		*/


#include <portab.h>
#include <machine.h>
#include <dos.h>

#define CART_BASE 0xFA0000L
#define CART_START 0xFA0004L
#define CART_MAGIC 0xABCDEF42L
#define CA_ISCRYS 0x40000000L
#define CA_ISPARM 0x80000000L
#define TEXTBASE 8

extern char *dos_exec();

extern int16_t wildcmp();

extern VOID pstart();

extern VOID gotopgm();

extern int16_t cre_aproc();

extern int16_t DOS_AX;

extern char *g_name();

extern char dtabuf[];					/* dta buffer   */


#define CARTNODE struct cartnode
CARTNODE
{
	CARTNODE *c_next;

	int32_t *c_init;

	int32_t *c_code;

	int16_t c_time;

	int16_t c_date;

	int32_t c_size;

	char c_name[14];
};

GLOBAL CARTNODE *cart_ptr;

GLOBAL char *cart_dta;



cart_init()
{
	cart_ptr = ((CARTNODE *) CART_BASE);
	if (cart_ptr->c_next == CART_MAGIC)
	{
		cart_ptr = ((CARTNODE *) CART_START);
		return (TRUE);
	} else
	{
		cart_ptr = NULL;
		return (FALSE);
	}
}


CARTNODE * cart_find(fill)
int16_t fill;
{
	register char *pdta;

	register CARTNODE *pcart;

	if (cart_ptr)
	{
		if (fill)
		{
			pdta = cart_dta;
			bfill(42, NULL, &pdta[0]);	/* zero it out  */
			pdta[21] = F_RDONLY;		/* fill time,date,size,name */
			LBCOPY(&pdta[22], &cart_ptr->c_time, 21);
		}
		pcart = cart_ptr;
		cart_ptr = cart_ptr->c_next;	/* point to next    */
		return (pcart);
	}
	return (NULL);
}


int16_t cart_sfirst(pdta, attr)
char *pdta;

int16_t attr;
{
	cart_dta = pdta;
	cart_init();
	return (cart_snext());
}

int16_t cart_snext()
{
	if (cart_find(TRUE))
		return (TRUE);
	else
	{
		DOS_AX = E_NOFILES;
		return (FALSE);
	}
}

int16_t ld_cartacc()
{
	register char *psp;

	register CARTNODE *pcart;

	register int16_t num_load;

	cart_init();
	num_load = 0;
	while (pcart = cart_find(FALSE))
	{
		if (wildcmp("*.ACC", &pcart->c_name[0]))
		{
			if (cre_aproc())			/* create PD    */
			{
				num_load++;
				psp = dos_exec("", 5, "");	/* create psp   */
				LLSET(&psp[TEXTBASE], pcart->c_code);
				pstart(&gotopgm, &pcart->c_name[0], psp);	/* go for it    */
			} else
				break;
		}
	}
	return (num_load);
}

int16_t cart_exec(pcmd, ptail)
char *pcmd,
*ptail;
{
	register char *psp;

	register CARTNODE *pcart;

	cart_init();

	while (pcart = cart_find(FALSE))
	{
		if (strcmp(pcmd, &pcart->c_name[0]))
			break;
	}
	psp = dos_exec("", 5, ptail);
	LLSET(&psp[TEXTBASE], pcart->c_code);
	dos_exec("", 4, psp);
	dos_free(*(int32_t *) (&psp[0x2c]));
	dos_free(psp);
	return (TRUE);
}


int16_t c_sfirst(path)
char *path;
{
	char *file;

	CARTNODE *pcart;

	file = g_name(path);
	cart_init();
	while (pcart = cart_find(FALSE))
	{
		if (strcmp(file, &pcart->c_name[0]))
			return (TRUE);
	}

	return (FALSE);
}
