/*      DESKAPP.C               3/18/89 - 9/18/89       Derek Mui       */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/

#include "desktop.h"


int16_t x_wildcmp PROTO((const char *source, const char *dest));


/* 306de: 00e27d68 */
BOOLEAN app_reschange(P(int16_t) res)
PP(register int16_t res;)
{
	if (res == gl_restype)
		return FALSE;

	gl_restype = res;
	pref_save = res;
	gl_rschange = TRUE;
	return TRUE;
}


/*
 * Find the application's icon type
 */
/* 306de: 00e27da0 */
APP *app_icon(P(const char *)name, P(int16_t) type, P(int16_t *)icon)
PP(register char *name;)
PP(int16_t type;)								/* looking for same type    */
PP(int16_t *icon;)								/* icon number          */
{
	register APP *app;

	app = applist;
	*icon = 4;							/* default icon index       */

	while (app)
	{
		if (type != -1)					/* match the type   */
		{
			if (app->a_type == type)
			{
				if (wildcmp(app->a_doc, name))
				{
					*icon = app->a_dicon;
					break;
				}
			}
		} else
		{
			if (app->a_type != FOLDER)	/* skip this */
			{
				if (x_wildcmp(app->a_name, name))
				{
					*icon = app->a_icon;
					break;
				}

				if (app->a_type == ICONS || app->a_type == TEXT)
				{
					if (x_wildcmp(app->a_doc, name))
					{
						*icon = app->a_dicon;
						break;
					}
				}
			}
		}
		app = app->a_next;
	}

	return app;
}


/*
 * Free an application node
 */
/* 306de: 00e27e4e */
VOID app_free(P(APP *)app)
PP(register APP *app;)
{
	register APP *list;

	if (app == applist)
		applist = app->a_next;
	else
	{
		list = applist;

		while (list)
		{
			if (list->a_next == app)
			{
				list->a_next = app->a_next;
				break;
			}

			list = list->a_next;
		}
	}

	lp_collect();
	app->a_next = appfree;
	appfree = app;
}


/*
 * Allocation an anode
 */
/* 306de: 00e27ea8 */
APP *app_alloc(NOTHING)
{
	register APP *list;

again:
	list = NULL;

	if (appfree)
	{
		list = appfree;
		appfree = appfree->a_next;
		list->a_next = applist;
		applist = list;
		list->a_doc[0] = 0;
		list->a_argu[0] = 0;
		list->a_key = 0;
		list->a_pref = 0;
		list->a_name = Nostr;			/* Point to no string   */
	} else
	{
		if (apbuf_init())				/* allocate buffer  */
			goto again;
	}

	return list;
}



/*
 * Find the actual application type
 */
/* 306de: 00e27f0c */
APP *app_xtype(P(const char *)name, P(BOOLEAN *) install)
PP(register const char *name;)
PP(BOOLEAN *install;)							/* application installed    */
{
	register APP *app;
	APP *markapp;

	app = applist;
	markapp = (APP *) 0;

	while (app)
	{
		if (app->a_type != FOLDER && app->a_type != ICONS)
		{
			if (x_wildcmp(app->a_name, name))
			{
				if (markapp)
					goto x_p1;

				*install = FALSE;
				return app;
			}

			if (!streq(app->a_doc, noext))
			{
				if (x_wildcmp(app->a_doc, name))
				{
					*install = TRUE;
					if (app->a_name[0])
						return app;

					if (!markapp)
						markapp = app;
				}
			}
		}
	x_p1:
		app = app->a_next;
	}

	return markapp;
}


/* 306de: 00e27fb4 */
int16_t x_wildcmp(P(const char *) source, P(const char *) dest)
PP(const char *source;)
PP(const char *dest;)
{
	if (*source)
	{
		if (*scasb(source, '*'))
			return wildcmp(source, g_name(dest));
		else
			return streq(source, dest);
	}
	return FALSE;
}


int16_t const ftab[] = {
	0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
	F11, F12, F13, F14, F15, F16, F17, F18, F19, F20
};


/*
 * Find the key that matches the application
 */
/* 306de: 00e2800a */
APP *app_key(P(int16_t) key)
PP(int16_t key;)
{
	register APP *app;

	app = applist;

	while (app)
	{
		if (ftab[app->a_key] == key)
			return app;

		app = app->a_next;
	}

	return NULL;
}
