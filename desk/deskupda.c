/*	DESKUPDA.C		03/12/90		D.Mui		*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/

#include <portab.h>
#include <mobdefs.h>
#include <defines.h>
#include <window.h>
#include <gemdefs.h>
#include <deskusa.h>
#include <osbind.h>
#include <extern.h>

extern char *scasb();

extern char afile[];

extern char *r_slash();

extern char *escan_str();

extern char *strcpy();

extern char *escan_str();

extern char *save_str();


char *q_addr;							/* Inf file address */

int16_t q_change;							/* Inf file is changed  */

char *newbuff;

char q_path[40];

/*	Load in the new inf	*/

int16_t q_inf()
{
	if (q_addr = Malloc((int32_t) (INFSIZE)))
	{
		q_change = FALSE;
		bfill(INFSIZE, 0, q_addr);
		strcpy(afile, q_addr);
	}
}


/*	Search the matching file and modify the inf file	*/

int16_t q_sea(old, new)
char *old;

char *new;
{
	register char *ptr1,
	*copy1,
	*copy2;

	char *end2,
	*p,
	*end,
	*sl;

	register char temp;

	char save;

	int16_t alloc,
	 special,
	 offset;

	char buffer[16];

	char argu[ARGULEN];

	char tmp1,
	 tmp2;

	copy1 = ptr1 = q_addr;
	alloc = FALSE;

	while (temp = *ptr1++)
	{
		if (temp == '#')
		{								/* Desktop file     */
			temp = *ptr1;

			switch (temp)
			{
			case 'F':
			case 'G':
			case 'P':
			case 'Y':
				offset = 10;
				special = TRUE;

				if (*(ptr1 + 9) != ' ')
					offset = 12;

				break;
			case 'X':
			case 'V':
				offset = 16;
				special = FALSE;
				break;

			default:
				continue;
			}

			p = ptr1 + offset;			/* beginning of path    */
			end = scasb(p, '@');		/* end of path      */
			tmp1 = *end;
			*end = 0;

			if (temp == 'V')
			{
				sl = r_slash(p);
				tmp2 = *sl;
				*sl = 0;
			}
			/* label        */
			end2 = escan_str(end + 1, buffer);
			if (special)
			{
				if (*(end2 + 1) == 0xD)	/* for old inf file */
				{
					argu[0] = 0;
					end2++;
				} else
					end2 = escan_str(end2 + 1, argu);
			}

			if (streq(old, p))
			{
				if (!alloc)
				{
					if (copy2 = Malloc((int32_t) (INFSIZE)))
					{
						bfill(INFSIZE, 0, copy2);
						alloc = TRUE;
						newbuff = copy2;
					} else
						goto qq_1;
				}

				if (new)				/* Modify the path  */
				{
					*p = 0;
					copy2 = strcpy(copy1, copy2) - 1;
					copy2 = save_str(copy2, new);
					if (temp == 'V')
						copy2 = save_str(copy2 - 2, wilds);
					/* copy the label   */
					copy2 = save_str(copy2, buffer);
					if (special)
						copy2 = save_str(copy2, argu);
					*copy2++ = 0xD;
					*copy2++ = 0xA;
				} else					/* remove the path  */
				{
					*(ptr1 - 1) = 0;
					copy2 = strcpy(copy1, copy2) - 1;
					*(ptr1 - 1) = '#';
				}

				copy1 = ptr1 = end2 + 3;
				q_change = TRUE;
			}							/* same string  */
		  qq_1:
			if (temp == 'V')
				*sl = tmp2;

			*end = tmp1;
		}
	}

	if (alloc)							/* swap the buffer  */
	{
		strcpy(copy1, copy2);
		Mfree(q_addr);
		q_addr = newbuff;
	}
}


/*	Write out the new inf file	*/
/*	always write out newdesk.inf	*/

q_write()
{
	register int16_t handle;

	if (q_change)
	{
		desk_wait(TRUE);				/* file exits?  */
		if ((handle = Fopen(inf_path, 0)) > 0)
		{
			Fclose(handle);				/* truncates it */
			if ((handle = Fcreate(inf_path, 0)) > 0)
			{
				Fwrite(handle, (int32_t) strlen(q_addr), q_addr);
				Fclose(handle);
			}
		}
		desk_wait(FALSE);
		q_change = FALSE;
	}
}
