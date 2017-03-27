/*
 * WARNING: functions here are called from the startup code before knowing the
 * actual CPU type. So it must be compatible with any processor (including
 * 680x0 and ColdFire models). So it must NOT be compiled with -m68020
 * or similar
 */

#include "lib.h"
#include <mint/mintbind.h>
#include <mint/cookie.h>
#include <mint/ssystem.h>

typedef struct {
	union {
		__uint32_t id;
		long end;
	} cookie;
	long cookie_value;
} COOKIE;

#define fetch_sysvar(var) Ssystem(S_GETLVAL, (long)(var), 0L)
#define fetch_cookie(id, val) Ssystem(S_GETCOOKIE, (long)(id), (long)(val))
#define vec_inq(vec) Setexc(vec, VEC_INQUIRE)

#define ssystem_inq() Ssystem(S_INQUIRE, 0L, 0L)


static short __has_no_ssystem(NOTHING)
{
	return ssystem_inq() == 0 ? 0 : 1;
}


static COOKIE *_get_jarptr(NOTHING)
{
	COOKIE *p;
	
	if (__has_no_ssystem())
	{
		/*
		 * cookie jar ptr is longword aligned, thus
		 * we can use Setexc to fetch its value
		 */
		p = (COOKIE *)vec_inq(0x5A0 / 4);
	} else
	{
		long val;
		static volatile const VOIDPTR jarptr = (volatile const VOIDPTR)(0x5a0);
		
		val = fetch_sysvar(jarptr);
		if (val < 0)
			val = 0;
		p = (COOKIE *)val;
	}
	return p;
}

/******************************************************************************/
/* Cookie_JarInstalled()                                                      */
/* -------------------------------------------------------------------------- */
/* See if the cookie jar is installed.                                        */
/* -------------------------------------------------------------------------- */
/* Parameter:                                                                 */
/*                   none                                                     */
/* -------------------------------------------------------------------------- */
/* Return value:                                                              */
/*                   TRUE if jar is installed                                 */
/******************************************************************************/

int ck_ji(NOTHING)
{
	return _get_jarptr() == 0 ? 1 : 0;
}

/******************************************************************************/
/* Cookie_UsedEntries()                                                       */
/* -------------------------------------------------------------------------- */
/* Inquire the number of used cookie jar entries. The number includes         */
/* the null cookie, so a return value of 0 means that there is no             */
/* cookie jar at all.                                                         */
/* -------------------------------------------------------------------------- */
/* Parameter:                                                                 */
/*                   none                                                     */
/* -------------------------------------------------------------------------- */
/* Return value:                                                              */
/*                   number of used cookie jar entries                        */
/******************************************************************************/

int ck_ue(NOTHING)
{
	COOKIE *p;
	int entries = 0;
	
	p = _get_jarptr();
	if (p != NULL)
	{
		for (;;)
		{
			++entries;
			if (p->cookie.end == 0)
				break;
			p++;
		}
	}
	return entries;
}

/******************************************************************************/
/* Cookie_JarSize()                                                           */
/* -------------------------------------------------------------------------- */
/* Inquire the total number of cookie jar entries.                            */
/* -------------------------------------------------------------------------- */
/* Parameter:                                                                 */
/*                   none                                                     */
/* -------------------------------------------------------------------------- */
/* Return value:                                                              */
/*                   total number of cookie jar entries                       */
/******************************************************************************/

int ck_js(NOTHING)
{
	int size = 0;
	COOKIE *p;
	
	p = _get_jarptr();
	if (p != NULL)
	{
		while (p->cookie.end != 0)
		{
			p++;
		}
		size = (int)p->cookie_value;
	}
	return size;
}

/*** ---------------------------------------------------------------------- ***/

static COOKIE *SearchJar(P(__uint32_t) id)
PP(__uint32_t id;)
{
	COOKIE *p;
	
	p = _get_jarptr();
	if (p != NULL)
	{
		for (;;)
		{
			if (p->cookie.end == 0)
				return NULL;
			else if (p->cookie.id == id)
				break;
			else
				p++;
		}
	}
	return p;
}

/******************************************************************************/
/* Cookie_ReadJar()                                                           */
/* -------------------------------------------------------------------------- */
/* Read the value of the specified cookie.                                    */
/* -------------------------------------------------------------------------- */
/* Parameter:                                                                 */
/* -> id             cookie name                                              */
/* <- value          pointer to cookie value (may be NULL)                    */
/* -------------------------------------------------------------------------- */
/* Return value:                                                              */
/*                   TRUE if successful                                       */
/******************************************************************************/

static int get_cookie(P(__uint32_t) id, P(long *) value)
PP(__uint32_t id;)
PP(long *value;)
{
	long ret;
	long val;
	
	val = -42;
	ret = fetch_cookie((volatile const VOIDPTR)id, (long)&val);
	if (ret == -1)
		return 0;
	/*
	 * Backward compatibility for MiNT 1.14.7:
	 * Ssystem() returns cookie value and ignores arg2!!
	 */
	if (val == -42)
		val = ret;
	*value = val;
	return 1;
}


static VOID setval(P(long) valp, P(long) val)
PP(long valp;)
PP(long val;)
{
	*((long *)valp) = val;
}

int ck_rdj(P(__uint32_t) id, P(long *) value)
PP(__uint32_t id;)
PP(long *value;)
{
	long valp = (long)value;
	if (valp == 0)
		valp = (long)&id;

	if (__has_no_ssystem())
	{
		COOKIE *p;
		
		p = SearchJar(id);
		if (p != NULL)
		{
			setval(valp, p->cookie_value);
			return 1;
		}
	} else
	{
		long val;
		
		if (get_cookie(id, &val))
		{
			setval(valp, val);
			return 1;
		}
	}
	return 0;
}

/******************************************************************************/
/* Cookie_WriteJar()                                                          */
/* -------------------------------------------------------------------------- */
/* Insert a new entry into the cookie jar. If no cookie jar exists            */
/* or the current cookie jar is full, a new, bigger cookie jar is             */
/* installed. The increment in size can be set using Cookie_SetOptions.       */
/* -------------------------------------------------------------------------- */
/* Parameter:                                                                 */
/* -> id             cookie name                                              */
/* -> value          cookie value                                             */
/* -------------------------------------------------------------------------- */
/* Return value:                                                              */
/*                   TRUE if successful                                       */
/******************************************************************************/

int ck_wrj(P(__uint32_t) id, P(long) value)
PP(__uint32_t id;)
PP(long value;)
{
	COOKIE *p;
	
	p = SearchJar(id);
	if (p != NULL)
	{
		p->cookie_value = value;
		return 1;
	}
	return 0;
}

/*
 * MiNTLib compatibility
 */

int Getcookie(P(long) cookie, P(long *) val)
PP(long cookie;)
PP(long *val;)
{
	return ck_rdj(cookie, val) ? 0 : -1;
}
