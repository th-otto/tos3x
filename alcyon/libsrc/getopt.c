/* Getopt for GNU.
   NOTE: getopt is now part of the C library, so if you don't know what
   "Keep this file name-space clean" means, talk to roland@gnu.ai.mit.edu
   before changing it!

   Copyright (C) 1987, 88, 89, 90, 91, 92, 93, 94
   	Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include "lib.h"

#define _(x) x


/* This version of `getopt' appears to the caller like standard Unix `getopt'
   but it behaves differently for the user, since it allows the user
   to intersperse the options with the other arguments.

   As `getopt_long' works, it permutes the elements of ARGV so that,
   when it is done, all the options precede everything else.  Thus
   all application programs are extended to handle flexible argument order.

   Using `getopt' or setting the environment variable POSIXLY_CORRECT
   disables permutation.
   Then the behavior is completely standard.

   GNU application programs can use a third alternative mode in which
   they can distinguish the relative order of options and other arguments.  */

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

/* Describe how to deal with options that follow non-option ARGV-elements.

   If the caller did not specify anything,
   the default is REQUIRE_ORDER if the environment variable
   POSIXLY_CORRECT is defined, PERMUTE otherwise.

   REQUIRE_ORDER means don't recognize them as options;
   stop option processing when the first non-option is seen.
   This is what Unix does.
   This mode of operation is selected by either setting the environment
   variable POSIXLY_CORRECT, or using `+' as the first character
   of the list of option characters, or by calling getopt.

   PERMUTE is the default.  We permute the contents of ARGV as we
   scan, so that eventually all the non-options are at the end.
   This allows options to be given in any order, even with programs
   that were not written to expect this.

   RETURN_IN_ORDER is an option available to programs that were
   written to expect options and other ARGV-elements in any order
   and that care about the ordering of the two.  We describe each
   non-option ARGV-element as if it were the argument of an option
   with character code 1.  Using `-' as the first character of the
   list of option characters selects this mode of operation.

   The special argument `--' forces an end of option-scanning regardless
   of the value of `ordering'.  In the case of RETURN_IN_ORDER, only
   `--' can cause `getopt' to return EOF with `optind' != ARGC.  */

#define REQUIRE_ORDER 0
#define PERMUTE 1
#define RETURN_IN_ORDER 2

/* Data type for reentrant functions.  */
struct _getopt_data
{
	/* These have exactly the same meaning as the corresponding global
	   variables, except that they are used for the reentrant
	   versions of getopt.  */
	int optind;
	int opterr;
	int optopt;
	int optswitch;
	const char *optarg;

	/* Internal members.  */

	const char *argv0;
	
	/* True if the internal members have been initialized.  */
	int __initialized;

	/* The next char to be scanned in the option-element
	   in which the last option character we returned was found.
	   This allows us to pick up the scan where we left off.

	   If this is zero, or a null string, it means resume the scan
	   by advancing to the next ARGV-element.  */
	const char *__nextchar;

	/* See __ord above.  */
	int __ordering;

	/* If the POSIXLY_CORRECT environment variable is set
	   or getopt was called.  */
	int __posixly_correct;

	/* Handle permutation of arguments.  */

	/* Describe the part of ARGV that contains non-options that have
	   been skipped.  `first_nonopt' is the index in ARGV of the first
	   of them; `last_nonopt' is the index after the last of them.  */

	int __first_nonopt;
	int __last_nonopt;
};

const char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';
int optswitch = '-';
static struct _getopt_data getodata;


/* Exchange two adjacent subsequences of ARGV.
   One subsequence is elements [first_nonopt,last_nonopt)
   which contains all the non-options that have been skipped so far.
   The other is elements [last_nonopt,optind), which contains all
   the options processed since those non-options were skipped.

   `first_nonopt' and `last_nonopt' are relocated so that they describe
   the new indices of the non-options in ARGV after they are moved.  */

static VOID exchange(P(const char **) argv, P(struct _getopt_data *) d)
PP(register const char **argv;)
PP(register struct _getopt_data *d;)
{
	register int bottom;
	register int middle;
	register int top;
	register const char *tem;
	int len;
	register int i;
	int swap;
	
	bottom = d->__first_nonopt;
	middle = d->__last_nonopt;
	top = d->optind;

	/* Exchange the shorter segment with the far end of the longer segment.
	   That puts the shorter segment into the right place.
	   It leaves the longer segment in the right place overall,
	   but it consists of two parts that need to be swapped next.  */

	while (top > middle && middle > bottom)
	{
		if (top - middle > middle - bottom)
		{
			/* Bottom segment is the short one.  */
			len = middle - bottom;

			/* Swap it with the top part of the top segment.  */
			for (i = 0; i < len; i++)
			{
				swap = top - (middle - bottom) + i;
				tem = argv[bottom + i];
				argv[bottom + i] = argv[swap];
				argv[swap] = tem;
			}
			/* Exclude the moved bottom segment from further swapping.  */
			top -= len;
		} else
		{
			/* Top segment is the short one.  */
			len = top - middle;

			/* Swap it with the bottom part of the bottom segment.  */
			for (i = 0; i < len; i++)
			{
				swap = middle + i;
				tem = argv[bottom + i];
				argv[bottom + i] = argv[swap];
				argv[swap] = tem;
			}
			/* Exclude the moved top segment from further swapping.  */
			bottom += len;
		}
	}

	/* Update records for the slots the non-options now occupy.  */

	d->__first_nonopt += (d->optind - d->__last_nonopt);
	d->__last_nonopt = d->optind;
}


/* Initialize the internal data when the first call is made.  */

static const char *_getoinit(P(const char *) optstring, P(struct _getopt_data *) d, P(int) posixly_correct)
PP(register const char *optstring;)
PP(register struct _getopt_data *d;)
PP(int posixly_correct;)
{
	/* Start processing options with ARGV-element 1 (since ARGV-element 0
	   is the program name); the sequence of previously skipped
	   non-option ARGV-elements is empty.  */

	if (d->optind == 0)
		d->optind = 1;				/* Don't scan ARGV[0], the program name.  */
	
	d->__first_nonopt = d->__last_nonopt = d->optind;

	d->__nextchar = NULL;

	d->__posixly_correct = posixly_correct || (getenv("POSIXLY_CORRECT") != NULL);

	/* Determine how to handle the ordering of options and nonoptions.  */

	if (optstring[0] == '-')
	{
		d->__ordering = RETURN_IN_ORDER;
		++optstring;
	} else if (optstring[0] == '+')
	{
		d->__ordering = REQUIRE_ORDER;
		++optstring;
	} else if (d->__posixly_correct)
	{
		d->__ordering = REQUIRE_ORDER;
	} else
	{
		d->__ordering = PERMUTE;
	}
	
	d->__initialized = 1;

	return optstring;
}


/* Scan elements of ARGV (whose length is ARGC) for option characters
   given in OPTSTRING.

   If an element of ARGV starts with '-', and is not exactly "-" or "--",
   then it is an option element.  The characters of this element
   (aside from the initial '-') are option characters.  If `getopt'
   is called repeatedly, it returns successively each of the option characters
   from each of the option elements.

   If `getopt' finds another option character, it returns that character,
   updating `optind' and `nextchar' so that the next call to `getopt' can
   resume the scan with the following option character or ARGV-element.

   If there are no more option characters, `getopt' returns `EOF'.
   Then `optind' is the index in ARGV of the first ARGV-element
   that is not an option.  (The ARGV-elements have been permuted
   so that those that are not options now come last.)

   OPTSTRING is a string containing the legitimate option characters.
   If an option character is seen that is not listed in OPTSTRING,
   return '?' after printing an error message.  If you set `opterr' to
   zero, the error message is suppressed but we still return '?'.

   If a char in OPTSTRING is followed by a colon, that means it wants an arg,
   so the following text in the same ARGV-element, or the text of the following
   ARGV-element, is returned in `optarg'.  Two colons mean an option that
   wants an optional arg; if there is text in the current ARGV-element,
   it is returned in `optarg', otherwise `optarg' is set to zero.

   If OPTSTRING starts with `-' or `+', it requests different methods of
   handling the non-option ARGV-elements.
   See the comments about RETURN_IN_ORDER and REQUIRE_ORDER, above.

   Long-named options begin with `--' instead of `-'.
   Their names may be abbreviated as long as the abbreviation is unique
   or is an exact match for some defined option.  If they have an
   argument, it follows the option name in the same ARGV-element, separated
   from the option name by a `=', or else the in next ARGV-element.
   When `getopt' finds a long-named option, it returns 0 if that option's
   `flag' field is nonzero, the value of the option's `val' field
   if the `flag' field is zero.

   The elements of ARGV aren't really const, because we permute them.
   But we pretend they're const in the prototype to be compatible
   with other systems.

   LONGOPTS is a vector of `struct option' terminated by an
   element containing a name which is zero.

   LONGIND returns the index in LONGOPT of the long-named option found.
   It is only valid when a long-named option has been found by the most
   recent call.

   If LONG_ONLY is nonzero, '-' as well as '--' can introduce
   long-named options.  */

static int _getoir(P(int) argc, P(char *const *) argv, P(const char *) optstring,
				   P(const struct option *) longopts, P(int *) longind,
				   P(int) long_only, P(struct _getopt_data *) d, P(int) posixly_correct)
PP(register int argc;)
PP(register char *const *argv;)
PP(register const char *optstring;)
PP(const struct option *longopts;)
PP(int *longind;)
PP(int long_only;)
PP(register struct _getopt_data *d;)
PP(int posixly_correct;)
{
	int print_errors;

	print_errors = d->opterr;
	if (argc < 1)
		return EOF;

	d->optarg = NULL;

	if (d->optind == 0 || !d->__initialized)
	{
		optstring = _getoinit(optstring, d, posixly_correct);
		if (d->argv0 == NULL)
			d->argv0 = argv[0];
	} else if (optstring[0] == '-' || optstring[0] == '+')
	{
		optstring++;
	}
	if (optstring[0] == ':')
		print_errors = 0;

	/* Test whether ARGV[optind] points to a non-option argument.
	   Either it does not have option syntax, or there is an environment flag
	   from the shell indicating it is not an option.  The later information
	   is only used when the used in the GNU libc.  */
# define NONOPTION_P ((argv[d->optind][0] != '-' && argv[d->optind][0] != '+') || argv[d->optind][1] == '\0')

	if (d->__nextchar == NULL || *d->__nextchar == '\0')
	{
		/* Advance to the next ARGV-element.  */

		/* Give FIRST_NONOPT & LAST_NONOPT rational values if OPTIND has been
		   moved back by the user (who may also have changed the arguments).  */
		if (d->__last_nonopt > d->optind)
			d->__last_nonopt = d->optind;
		if (d->__first_nonopt > d->optind)
			d->__first_nonopt = d->optind;

		if (d->__ordering == PERMUTE)
		{
			/* If we have just processed some options following some non-options,
			   exchange them so that the options come first.  */

			if (d->__first_nonopt != d->__last_nonopt && d->__last_nonopt != d->optind)
				exchange((const char **)NO_CONST(argv), d);
			else if (d->__last_nonopt != d->optind)
				d->__first_nonopt = d->optind;

			/* Skip any additional non-options
			   and extend the range of non-options previously skipped.  */

			while (d->optind < argc && NONOPTION_P)
				d->optind++;
			d->__last_nonopt = d->optind;
		}

		/* The special ARGV-element `--' means premature end of options.
		   Skip it like a null option,
		   then exchange with previous non-options as if it were an option,
		   then skip everything else like a non-option.  */

		if (d->optind != argc && !strcmp(argv[d->optind], "--"))
		{
			d->optind++;

			if (d->__first_nonopt != d->__last_nonopt && d->__last_nonopt != d->optind)
				exchange((const char **)NO_CONST(argv), d);
			else if (d->__first_nonopt == d->__last_nonopt)
				d->__first_nonopt = d->optind;
			d->__last_nonopt = argc;

			d->optind = argc;
		}

		/* If we have done all the ARGV-elements, stop the scan
		   and back over any non-options that we skipped and permuted.  */

		if (d->optind == argc)
		{
			/* Set the next-arg-index to point at the non-options
			   that we previously skipped, so the caller will digest them.  */
			if (d->__first_nonopt != d->__last_nonopt)
				d->optind = d->__first_nonopt;
			return EOF;
		}

		/* If we have come to a non-option and did not permute it,
		   either stop the scan or describe it to the caller and pass it by.  */

		if (NONOPTION_P)
		{
			if (d->__ordering == REQUIRE_ORDER)
				return EOF;
			d->optswitch = argv[d->optind][0];
			d->optarg = argv[d->optind++];
			return 1;
		}

		/* We have found another option-ARGV-element.
		   Skip the initial punctuation.  */

		d->__nextchar = (argv[d->optind] + 1 + (longopts != NULL && argv[d->optind][1] == '-'));
	}

	/* Decode the current option-ARGV-element.  */

	/* Check whether the ARGV-element is a long option.

	   If long_only and the ARGV-element has the form "-f", where f is
	   a valid short option, don't consider it an abbreviated form of
	   a long option that starts with f.  Otherwise there would be no
	   way to give the -f short option.

	   On the other hand, if there's a long option "fubar" and
	   the ARGV-element is "-fu", do consider that an abbreviation of
	   the long option, just like "--fu", and not "-f" with arg "u".

	   This distinction seems to be the most useful approach.  */

	if (longopts != NULL
		&& (argv[d->optind][1] == '-' || (long_only && (argv[d->optind][2] || !strchr(optstring, argv[d->optind][1])))))
	{
		const char *nameend;
		const struct option *p;
		const struct option *pfound = NULL;
		int exact = 0;
		int ambig = 0;
		int indfound = -1;
		int option_index;

		d->optswitch = argv[d->optind][1] == '-' ? '+' : argv[d->optind][0];

		for (nameend = d->__nextchar; *nameend && *nameend != '='; nameend++)
			/* Do nothing.  */ ;

		/* Test all long options for either exact match
		   or abbreviated matches.  */
		for (p = longopts, option_index = 0; p->name; p++, option_index++)
			if (!strncmp(p->name, d->__nextchar, (__intptr_t)nameend - (__intptr_t)d->__nextchar))
			{
				if ((unsigned int) ((__intptr_t)nameend - (__intptr_t)d->__nextchar) == (unsigned int) strlen(p->name))
				{
					/* Exact match found.  */
					pfound = p;
					indfound = option_index;
					exact = 1;
					break;
				} else if (pfound == NULL)
				{
					/* First nonexact match found.  */
					pfound = p;
					indfound = option_index;
				} else if (long_only
						   || pfound->has_arg != p->has_arg || pfound->flag != p->flag || pfound->val != p->val)
				{
					/* Second or later nonexact match found.  */
					ambig = 1;
				}
			}

		if (ambig && !exact)
		{
			if (print_errors)
			{
				fprintf(stderr, _("%s: option '%s' is ambiguous\n"), d->argv0, argv[d->optind]);
			}
			d->__nextchar += strlen(d->__nextchar);
			d->optind++;
			d->optopt = 0;
			return '?';
		}

		if (pfound != NULL)
		{
			option_index = indfound;
			d->optind++;
			if (*nameend)
			{
				if (pfound->has_arg != no_argument)
				{
					d->optarg = nameend + 1;
				} else
				{
					if (print_errors)
					{
						if (argv[d->optind - 1][1] == '-')
						{
							/* --option */
							fprintf(stderr, _("%s: option '--%s' doesn't allow an argument\n"), d->argv0, pfound->name);
						} else
						{
							/* +option or -option */
							fprintf(stderr, _("%s: option '%c%s' doesn't allow an argument\n"),
									d->argv0, argv[d->optind - 1][0], pfound->name);
						}
					}
					d->__nextchar += strlen(d->__nextchar);
					d->optopt = pfound->val;
					return '?';
				}
			} else if (pfound->has_arg == required_argument)
			{
				if (d->optind < argc)
				{
					d->optarg = argv[d->optind++];
				} else
				{
					if (print_errors)
					{
						fprintf(stderr, _("%s: option '--%s' requires an argument\n"), d->argv0, pfound->name);
					}
					d->__nextchar += strlen(d->__nextchar);
					d->optopt = pfound->val;
					return optstring[0] == ':' ? ':' : '?';
				}
			}
			d->__nextchar += strlen(d->__nextchar);
			if (longind != NULL)
				*longind = option_index;
			if (pfound->flag)
			{
				*(pfound->flag) = pfound->val;
				return 0;
			}
			return pfound->val;
		}

		/* Can't find it as a long option.  If this is not getopt_long_only,
		   or the option starts with '--' or is not a valid short
		   option, then it's an error.
		   Otherwise interpret it as a short option.  */
		if (!long_only || argv[d->optind][1] == '-' || strchr(optstring, *d->__nextchar) == NULL)
		{
			if (print_errors)
			{
				if (argv[d->optind][1] == '-')
				{
					/* --option */
					fprintf(stderr, _("%s: unrecognized option '--%s'\n"), d->argv0, d->__nextchar);
				} else
				{
					/* +option or -option */
					fprintf(stderr, _("%s: unrecognized option '%c%s'\n"), d->argv0, argv[d->optind][0], d->__nextchar);
				}
			}
			d->optopt = *d->__nextchar;
			d->optswitch = argv[d->optind][0];
			d->__nextchar = "";
			d->optind++;
			return '?';
		}
	}

	/* Look at and handle the next short option-character.  */

	{
		char c = *d->__nextchar++;
		const char *temp = strchr(optstring, c);

		d->optswitch = argv[d->optind][0];

		/* Increment `optind' when we start to process its last character.  */
		if (*d->__nextchar == '\0')
			++d->optind;

		if (temp == NULL || c == ':' || c == ';')
		{
			if (print_errors)
			{
				if (d->__posixly_correct)
					/* 1003.2 specifies the format of this message.  */
					fprintf(stderr, _("%s: illegal option -- %c\n"), d->argv0, c);
				else
					fprintf(stderr, _("%s: invalid option -- '%c'\n"), d->argv0, c);
			}
			d->optopt = c;
			return '?';
		}
		
		if (temp[1] == ':')
		{
			if (temp[2] == ':')
			{
				/* This is an option that accepts an argument optionally.  */
				if (*d->__nextchar != '\0')
				{
					d->optarg = d->__nextchar;
					d->optind++;
				} else
				{
					d->optarg = NULL;
				}
				d->__nextchar = NULL;
			} else
			{
				/* This is an option that requires an argument.  */
				if (*d->__nextchar != '\0')
				{
					d->optarg = d->__nextchar;
					/* If we end this ARGV-element by taking the rest as an arg,
					   we must advance to the next element now.  */
					d->optind++;
				} else if (d->optind == argc)
				{
					if (print_errors)
					{
						/* 1003.2 specifies the format of this message.  */
						fprintf(stderr, _("%s: option requires an argument -- '%c'\n"), d->argv0, c);
					}
					d->optopt = c;
					if (optstring[0] == ':')
						c = ':';
					else
						c = '?';
				} else
				{
					/* We already incremented `optind' once;
					   increment it again when taking next ARGV-elt as argument.  */
					d->optarg = argv[d->optind++];
				}
				d->__nextchar = NULL;
			}
		}
		return c;
	}
}


static int _getoi(P(int) argc, P(char *const *) argv, P(const char *) optstring,
				 P(const struct option *) longopts, P(int *) longind, P(int) long_only, P(int) posixly_correct)
PP(int argc;)
PP(char *const *argv;)
PP(const char *optstring;)
PP(const struct option *longopts;)
PP(int *longind;)
PP(int long_only;)
PP(int posixly_correct;)
{
	int result;
	register struct _getopt_data *d;
	
	d = &getodata;
	d->optind = optind;
	d->opterr = opterr;

	result = _getoir(argc, argv, optstring, longopts, longind, long_only, d, posixly_correct);

	optind = d->optind;
	optarg = d->optarg;
	optopt = d->optopt;

	return result;
}


int __posix_getopt(P(int) argc, P(char *const *) argv, P(const char *) optstring)
PP(int argc;)
PP(char *const *argv;)
PP(const char *optstring;)
{
	return _getoi(argc, argv, optstring, NULL, NULL, 0, 1);
}


int __GETOPT_ID(getopt)(P(int) argc, P(char *const *) argv, P(const char *) optstring)
PP(int argc;)
PP(char *const *argv;)
PP(const char *optstring;)
{
	return _getoi(argc, argv, optstring, NULL, NULL, 0, 0);
}


#ifdef __SHORT_EXTERNAL_NAMES
int __GETOPT_ID(getopl)(P(int) argc, P(char *const *)argv, P(const char *)options, P(const struct option *)long_options, P(int *) opt_index)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
{
	return _getoi(argc, argv, options, long_options, opt_index, 0, 0);
}

int __GETOPT_ID(getoplo)(P(int) argc, P(char *const *) argv, P(const char *) options, P(const struct option *) long_options, P(int *)opt_index)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
{
	return _getoi(argc, argv, options, long_options, opt_index, 1, 0);
}
#else
int __GETOPT_ID(getopt_long)(P(int) argc, P(char *const *)argv, P(const char *)options, P(const struct option *)long_options, P(int *) opt_index)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
{
	return _getoi(argc, argv, options, long_options, opt_index, 0, 0);
}

int __GETOPT_ID(getopt_long_only)(P(int) argc, P(char *const *)argv, P(const char *)options, P(const struct option *)long_options, P(int *) opt_index)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
{
	return _getoi(argc, argv, options, long_options, opt_index, 1, 0);
}
#endif


/*
 * reentrant versions
 */

#ifndef __SHORT_EXTERNAL_NAMES

int __GETOPT_ID(getopt_r)(P(int) argc, P(char *const *) argv, P(const char *) optstring, P(struct _getopt_data *) d)
PP(int argc;)
PP(char *const *argv;)
PP(const char *optstring;)
PP(struct _getopt_data *d;)
{
	return _getoir(argc, argv, optstring, NULL, NULL, 0, d, 0);
}


VOID __GETOPT_ID(getopt_init_r)(P(const char *) argv0, P(struct _getopt_data **) pd)
PP(const char *argv0;)
PP(struct _getopt_data **pd;)
{
	struct _getopt_data *d;
	
	d = (struct _getopt_data *) malloc(sizeof(*d));
	*pd = d;
	if (d == NULL)
		return;
	d->optind = 0;
	d->opterr = 1;
	d->optopt = '?';
	d->optarg = NULL;
	d->argv0 = argv0;
	d->__initialized = 0;
}


VOID __GETOPT_ID(getopt_error_r)(P(struct _getopt_data *) d, P(int) err)
PP(struct _getopt_data *d;)
PP(int err;)
{
	d->opterr = err;
}


int __GETOPT_ID(getopt_finish_r)(P(struct _getopt_data **) pd)
PP(struct _getopt_data **pd;)
{
	struct _getopt_data *d;
	int opti;
	
	d = *pd;
	if (d == NULL)
		return -1;
	opti = d->optind;
	free(d);
	*pd = NULL;
	return opti;
}


int __GETOPT_ID(getopt_ind_r)(P(const struct _getopt_data *) d)
PP(struct _getopt_data **pd;)
{
	if (d == NULL)
		return -1;
	return d->optind;
}


const char *__GETOPT_ID(getopt_arg_r)(P(const struct _getopt_data *) d)
PP(struct _getopt_data **pd;)
{
	if (d == NULL)
		return NULL;
	return d->optarg;
}


int __GETOPT_ID(getopt_opt_r)(P(const struct _getopt_data *) d)
PP(struct _getopt_data **pd;)
{
	if (d == NULL)
		return -1;
	return d->optopt;
}


int __GETOPT_ID(getopt_switch_r)(P(const struct _getopt_data *) d)
PP(struct _getopt_data **pd;)
{
	if (d == NULL)
		return -1;
	return d->optswitch;
}


int __GETOPT_ID(getopt_long_r)(P(int) argc, P(char *const *) argv, P(const char *) options, P(const struct option *) long_options, P(int *) opt_index, P(struct _getopt_data *) d)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
PP(struct _getopt_data *d;)
{
	return _getoir(argc, argv, options, long_options, opt_index, 0, d, 0);
}


int __GETOPT_ID(getopt_long_only_r)(P(int) argc, P(char *const *) argv, P(const char *) options, P(const struct option *) long_options, P(int *) opt_index, P(struct _getopt_data *) d)
PP(int argc;)
PP(char *const *argv;)
PP(const char *options;)
PP(const struct option *long_options;)
PP(int *opt_index;)
PP(struct _getopt_data *d;)
{
	return _getoir(argc, argv, options, long_options, opt_index, 1, d, 0);
}

#endif /* __SHORT_EXTERNAL_NAMES */
