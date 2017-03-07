#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef O_BINARY
#  ifdef _O_BINARY
#    define O_BINARY _O_BINARY
#  else
#    define O_BINARY 0
#  endif
#endif

# define same_file(s, t) \
    ((((s)->st_ino == (t)->st_ino) && ((s)->st_dev == (t)->st_dev)))

# define same_file_attributes(s, t) \
   ((s)->st_mode == (t)->st_mode \
    && (s)->st_nlink == (t)->st_nlink \
    && (s)->st_uid == (t)->st_uid \
    && (s)->st_gid == (t)->st_gid \
    && (s)->st_size == (t)->st_size \
    && (s)->st_mtime == (t)->st_mtime \
    && (s)->st_ctime == (t)->st_ctime)

#define FALSE 0
#define TRUE  1

#ifndef STAT_BLOCKSIZE
# ifdef HAVE_STRUCT_STAT_ST_BLKSIZE
#  define STAT_BLOCKSIZE(s) ((s).st_blksize)
# else
#  define STAT_BLOCKSIZE(s) (8 * 1024)
# endif
#endif

#define NULL_DEVICE "/dev/null"

#define EXIT_TROUBLE 2
#define _(x) x
#define N_(x) x


/* The name of this program */
static char const program_name[] = "cmprom";

/* Filenames of the compared files.  */
static char const *file[2];

/* File descriptors of the files.  */
static int file_desc[2];

/* Status of the files.  */
static struct stat stat_buf[2];

typedef int word;

/* Read buffers for the files.  */
static word *buffer[2];

/* Optimal block size for the files.  */
static size_t buf_size;

/* Initial prefix to ignore for each file.  */
static off_t ignore_initial[2];
static off_t offsets[2];

/* Number of bytes to compare.  */
static uintmax_t bytes = UINTMAX_MAX;

/* Output format.  */
static enum comparison_type
{
	type_first_diff,					/* Print the first difference.  */
	type_all_diffs,						/* Print all differences.  */
	type_no_stdout,						/* Do not output to stdout; only stderr.  */
	type_status							/* Exit status only.  */
} comparison_type;

/* If nonzero, print values of bytes quoted like cat -t does. */
static int opt_print_bytes;
static int opt_print_hex;

/* Values for long options that do not have single-letter equivalents.  */
enum
{
	HELP_OPTION = CHAR_MAX + 1
};

static struct option const long_options[] = {
	{ "print-bytes", no_argument, NULL, 'b' },
	{ "print-chars", no_argument, NULL, 'c' },
	{ "ignore-initial", required_argument, NULL, 'i' },
	{ "offsets", required_argument, NULL, 'O' },
	{ "verbose", no_argument, NULL, 'l' },
	{ "bytes", required_argument, NULL, 'n' },
	{ "hex", no_argument, NULL, 'x'  },
	{ "silent", no_argument, NULL, 's'},
	{ "quiet", no_argument, NULL, 's' },
	{ "version", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, HELP_OPTION },
	{ NULL, no_argument, NULL, 0 }
};


static void try_help(char const *reason_msgid, char const *operand)
{
	if (reason_msgid)
	{
		fprintf(stderr, reason_msgid, operand);
		fputc('\n', stderr);
	}
	fprintf(stderr, _("Try '%s --help' for more information.\n"), program_name);
	exit(EXIT_TROUBLE);
}

static char const valid_suffixes[] = "kKMGTPEZY0";

enum strtol_error
  {
    LONGINT_OK = 0,

    /* These two values can be ORed together, to indicate that both
       errors occurred.  */
    LONGINT_OVERFLOW = 1,
    LONGINT_INVALID_SUFFIX_CHAR = 2,

    LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW = (LONGINT_INVALID_SUFFIX_CHAR
                                                 | LONGINT_OVERFLOW),
    LONGINT_INVALID = 4
  };
typedef enum strtol_error strtol_error;

#define STRTOL_T_MAXIMUM ULONG_MAX
#define STRTOL_T_MINIMUM 0
#define __strtol_t unsigned long
#define TYPE_SIGNED(x) 0
#define INT_BUFSIZE_BOUND(x) 40
#define INT_STRLEN_BOUND(x) (INT_BUFSIZE_BOUND(x) - 1)



static strtol_error bkm_scale(__strtol_t * x, int scale_factor)
{
	if (TYPE_SIGNED(__strtol_t) && *x < STRTOL_T_MINIMUM / scale_factor)
	{
		*x = STRTOL_T_MINIMUM;
		return LONGINT_OVERFLOW;
	}
	if (STRTOL_T_MAXIMUM / scale_factor < *x)
	{
		*x = STRTOL_T_MAXIMUM;
		return LONGINT_OVERFLOW;
	}
	*x *= scale_factor;
	return LONGINT_OK;
}


static strtol_error bkm_scale_by_power(__strtol_t * x, int base, int power)
{
	strtol_error err = LONGINT_OK;

	while (power--)
		err |= bkm_scale(x, base);
	return err;
}


static strtol_error xstrtoumax(const char *s, char **ptr, int strtol_base, __strtol_t *val, const char *val_suffixes)
{
	char *t_ptr;
	char **p;
	__strtol_t tmp;
	strtol_error err = LONGINT_OK;

	p = (ptr ? ptr : &t_ptr);

	if (!TYPE_SIGNED(__strtol_t))
	{
		const char *q = s;
		unsigned char ch = *q;

		while (isspace(ch))
			ch = *++q;
		if (ch == '-')
			return LONGINT_INVALID;
	}

	errno = 0;
	tmp = strtoul(s, p, strtol_base);

	if (*p == s)
	{
		/* If there is no number but there is a valid suffix, assume the
		   number is 1.  The string is invalid otherwise.  */
		if (val_suffixes && **p && strchr(val_suffixes, **p))
			tmp = 1;
		else
			return LONGINT_INVALID;
	} else if (errno != 0)
	{
		if (errno != ERANGE)
			return LONGINT_INVALID;
		err = LONGINT_OVERFLOW;
	}

	/* Let valid_suffixes == NULL mean "allow any suffix".  */
	/* FIXME: update all callers except the ones that allow suffixes
	   after the number, changing last parameter NULL to "".  */
	if (!val_suffixes)
	{
		*val = tmp;
		return err;
	}

	if (**p != '\0')
	{
		int base = 1024;
		int suffixes = 1;
		strtol_error overflow;

		if (!strchr(val_suffixes, **p))
		{
			*val = tmp;
			return err | LONGINT_INVALID_SUFFIX_CHAR;
		}

		if (strchr(val_suffixes, '0'))
		{
			/* The "valid suffix" '0' is a special flag meaning that
			   an optional second suffix is allowed, which can change
			   the base.  A suffix "B" (e.g. "100MB") stands for a power
			   of 1000, whereas a suffix "iB" (e.g. "100MiB") stands for
			   a power of 1024.  If no suffix (e.g. "100M"), assume
			   power-of-1024.  */

			switch (p[0][1])
			{
			case 'i':
				if (p[0][2] == 'B')
					suffixes += 2;
				break;

			case 'B':
			case 'D':					/* 'D' is obsolescent */
				base = 1000;
				suffixes++;
				break;
			}
		}

		switch (**p)
		{
		case 'b':
			overflow = bkm_scale(&tmp, 512);
			break;

		case 'B':
			overflow = bkm_scale(&tmp, 1024);
			break;

		case 'c':
			overflow = 0;
			break;

		case 'E':						/* exa or exbi */
			overflow = bkm_scale_by_power(&tmp, base, 6);
			break;

		case 'G':						/* giga or gibi */
		case 'g':						/* 'g' is undocumented; for compatibility only */
			overflow = bkm_scale_by_power(&tmp, base, 3);
			break;

		case 'k':						/* kilo */
		case 'K':						/* kibi */
			overflow = bkm_scale_by_power(&tmp, base, 1);
			break;

		case 'M':						/* mega or mebi */
		case 'm':						/* 'm' is undocumented; for compatibility only */
			overflow = bkm_scale_by_power(&tmp, base, 2);
			break;

		case 'P':						/* peta or pebi */
			overflow = bkm_scale_by_power(&tmp, base, 5);
			break;

		case 'T':						/* tera or tebi */
		case 't':						/* 't' is undocumented; for compatibility only */
			overflow = bkm_scale_by_power(&tmp, base, 4);
			break;

		case 'w':
			overflow = bkm_scale(&tmp, 2);
			break;

		case 'Y':						/* yotta or 2**80 */
			overflow = bkm_scale_by_power(&tmp, base, 8);
			break;

		case 'Z':						/* zetta or 2**70 */
			overflow = bkm_scale_by_power(&tmp, base, 7);
			break;

		default:
			*val = tmp;
			return err | LONGINT_INVALID_SUFFIX_CHAR;
		}

		err |= overflow;
		*p += suffixes;
		if (**p)
			err |= LONGINT_INVALID_SUFFIX_CHAR;
	}

	*val = tmp;
	return err;
}


/* Update ignore_initial[F] according to the result of parsing an
   *operand ARGPTR of --ignore-initial, updating *ARGPTR to point
   *after the operand.  If DELIMITER is nonzero, the operand may be
   *followed by DELIMITER; otherwise it must be null-terminated.  */
static void specify_ignore_initial(int f, char **argptr, char delimiter)
{
	__strtol_t val;
	char const *arg = *argptr;
	strtol_error e = xstrtoumax (arg, argptr, 0, &val, valid_suffixes);
	
	if (!(e == LONGINT_OK || (e == LONGINT_INVALID_SUFFIX_CHAR && **argptr == delimiter)) || STRTOL_T_MAXIMUM < val)
		try_help(_("invalid --ignore-initial value '%s'"), arg);
	if (ignore_initial[f] < val)
		ignore_initial[f] = val;
}


static void specify_offset(int f, char **argptr, char delimiter)
{
	__strtol_t val;
	char const *arg = *argptr;
	strtol_error e = xstrtoumax (arg, argptr, 0, &val, valid_suffixes);
	
	if (!(e == LONGINT_OK || (e == LONGINT_INVALID_SUFFIX_CHAR && **argptr == delimiter)) || STRTOL_T_MAXIMUM < val)
		try_help(_("invalid --offsets value '%s'"), arg);
	offsets[f] = val;
}


/* Specify the output format.  */
static void specify_comparison_type(enum comparison_type t)
{
	if (comparison_type && comparison_type != t)
		try_help(_("options -l and -s are incompatible"), 0);
	comparison_type = t;
}


static void check_stdout(void)
{
	fflush(stdout);
	if (ferror(stdout))
	{
		fprintf(stderr, "%s", _("write failed"));
		exit(EXIT_TROUBLE);
	}
}


/* Position file F to ignore_initial[F] bytes from its initial position,
   and yield its new position.  Don't try more than once.  */

static off_t file_position(int f)
{
	static int positioned[2];
	static off_t position[2];

	if (!positioned[f])
	{
		positioned[f] = TRUE;
		position[f] = lseek(file_desc[f], ignore_initial[f], SEEK_CUR);
	}
	return position[f];
}


/* Compare two blocks of memory P0 and P1 until they differ.
   If the blocks are not guaranteed to be different, put sentinels at the ends
   of the blocks before calling this function.

   Return the offset of the first byte that differs.  */

static size_t block_compare(word const *p0, word const *p1)
{
	word const *l0, *l1;
	char const *c0, *c1;

	/* Find the rough position of the first difference by reading words,
	   not bytes.  */

	for (l0 = p0, l1 = p1; *l0 == *l1; l0++, l1++)
		continue;

	/* Find the exact differing position (endianness independent).  */

	for (c0 = (char const *) l0, c1 = (char const *) l1; *c0 == *c1; c0++, c1++)
		continue;

	return c0 - (char const *) p0;
}



/* Return the number of newlines in BUF, of size BUFSIZE,
   where BUF[NBYTES] is available for use as a sentinel.  */
static size_t count_newlines(char *buf, size_t bufsize)
{
	size_t count = 0;
	char *p;
	char *lim = buf + bufsize;

	*lim = '\n';
	for (p = buf; (p = rawmemchr(p, '\n')) != lim; p++)
		count++;
	return count;
}


/* Put into BUF the unsigned char C, making unprintable bytes
   visible by quoting like cat -t does.  */

static void sprintc(char *buf, unsigned char c)
{
	if (!isprint(c))
	{
		if (c >= 128)
		{
			*buf++ = 'M';
			*buf++ = '-';
			c -= 128;
		}
		if (c < 32)
		{
			*buf++ = '^';
			c += 64;
		} else if (c == 127)
		{
			*buf++ = '^';
			c = '?';
		}
	}

	*buf++ = c;
	*buf = 0;
}



#undef MIN
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

/* Read NBYTES bytes from descriptor FD into BUF.
   NBYTES must not be SIZE_MAX.
   Return the number of characters successfully read.
   On error, return SIZE_MAX, setting errno.
   The number returned is always NBYTES unless end-of-file or error.  */

static size_t block_read(int fd, char *buf, size_t nbytes)
{
	char *bp = buf;
	char const *buflim = buf + nbytes;
	size_t readlim = MIN(SSIZE_MAX, SIZE_MAX);

	do
	{
		size_t bytes_remaining = buflim - bp;
		size_t bytes_to_read = MIN(bytes_remaining, readlim);
		ssize_t nread = read(fd, bp, bytes_to_read);

		if (nread <= 0)
		{
			if (nread == 0)
				break;

			/* Accommodate Tru64 5.1, which can't read more than INT_MAX
			   bytes at a time.  They call that a 64-bit OS?  */
			if (errno == EINVAL && INT_MAX < bytes_to_read)
			{
				readlim = INT_MAX;
				continue;
			}

			/* This is needed for programs that have signal handlers on
			   older hosts without SA_RESTART.  It also accommodates
			   ancient AIX hosts that set errno to EINTR after uncaught
			   SIGCONT.  See <news:1r77ojINN85n@ftp.UU.NET>
			   (1993-04-22).  */
			if (errno == EINTR)
				continue;

			return SIZE_MAX;
		}
		bp += nread;
	} while (bp < buflim);

	return bp - buf;
}


static char *offtostr(off_t i, char *buf)
{
	char *p = buf + INT_STRLEN_BOUND(inttype);
	*p = 0;

	if (i < 0)
	{
		do
			*--p = '0' - i % 10;
		while ((i /= 10) != 0);
		*--p = '-';
	} else
	{
		do
			*--p = '0' + i % 10;
		while ((i /= 10) != 0);
	}

	return p;
}


/* Least common multiple of two buffer sizes A and B.  However, if
   either A or B is zero, or if the multiple is greater than LCM_MAX,
   return a reasonable buffer size.  */

static size_t buffer_lcm(size_t a, size_t b, size_t lcm_max)
{
	size_t lcm, m, n, q, r;

	/* Yield reasonable values if buffer sizes are zero.  */
	if (!a)
		return b ? b : 8 * 1024;
	if (!b)
		return a;

	/* n = gcd (a, b) */
	for (m = a, n = b; (r = m % n) != 0; m = n, n = r)
		continue;

	/* Yield a if there is an overflow.  */
	q = a / n;
	lcm = q * b;
	return lcm <= lcm_max && lcm / b == q ? lcm : a;
}


/* Compare the two files already open on 'file_desc[0]' and 'file_desc[1]',
   using 'buffer[0]' and 'buffer[1]'.
   Return EXIT_SUCCESS if identical, EXIT_FAILURE if different,
   >1 if error.  */
static int cmp(void)
{
	off_t line_number = 1;				/* Line number (1...) of difference. */
	off_t byte_number = 0;				/* Byte number (1...) of difference. */
	uintmax_t remaining = bytes;		/* Remaining number of bytes to compare.  */
	size_t read0, read1;				/* Number of bytes read from each file. */
	size_t first_diff;					/* Offset (0...) in buffers of 1st diff. */
	size_t smaller;						/* The lesser of 'read0' and 'read1'. */
	word *buffer0 = buffer[0];
	word *buffer1 = buffer[1];
	char *buf0 = (char *) buffer0;
	char *buf1 = (char *) buffer1;
	int differing = 0;
	int f;
	int offset_width = 0;

	if (comparison_type == type_all_diffs)
	{
		off_t byte_number_max = MIN(bytes, STRTOL_T_MAXIMUM);

		for (f = 0; f < 2; f++)
		{
			if (S_ISREG(stat_buf[f].st_mode))
			{
				off_t file_bytes = stat_buf[f].st_size - file_position(f);

				if (file_bytes < byte_number_max)
					byte_number_max = file_bytes;
			}
		}
		
		if (opt_print_hex)
		{
			for (offset_width = 3; (byte_number_max /= 16) != 0; offset_width++)
				continue;
		} else
		{
			for (offset_width = 1; (byte_number_max /= 10) != 0; offset_width++)
				continue;
		}
	}

	for (f = 0; f < 2; f++)
	{
		off_t ig = ignore_initial[f];

		if (ig && file_position(f) == -1)
		{
			/* lseek failed; read and discard the ignored initial prefix.  */
			do
			{
				size_t bytes_to_read = MIN(ig, buf_size);
				size_t r = block_read(file_desc[f], buf0, bytes_to_read);

				if (r != bytes_to_read)
				{
					if (r == SIZE_MAX)
					{
						fprintf(stderr, "%s: %s\n", file[f], strerror(errno));
						exit(EXIT_TROUBLE);
					}
					break;
				}
				ig -= r;
			} while (ig);
		}
	}

	do
	{
		size_t bytes_to_read = buf_size;

		if (remaining != UINTMAX_MAX)
		{
			if (remaining < bytes_to_read)
				bytes_to_read = remaining;
			remaining -= bytes_to_read;
		}

		read0 = block_read(file_desc[0], buf0, bytes_to_read);
		if (read0 == SIZE_MAX)
		{
			fprintf(stderr, "%s: %s\n", file[0], strerror(errno));
			exit(EXIT_TROUBLE);
		}
		read1 = block_read(file_desc[1], buf1, bytes_to_read);
		if (read1 == SIZE_MAX)
		{
			fprintf(stderr, "%s: %s\n", file[1], strerror(errno));
			exit(EXIT_TROUBLE);
		}

		smaller = MIN(read0, read1);

		/* Optimize the common case where the buffers are the same.  */
		if (memcmp(buf0, buf1, smaller) == 0)
		{
			first_diff = smaller;
		} else
		{
			/* Insert sentinels for the block compare.  */
			buf0[read0] = ~buf1[read0];
			buf1[read1] = ~buf0[read1];

			first_diff = block_compare(buffer0, buffer1);
		}

		byte_number += first_diff;
		if (comparison_type == type_first_diff)
			line_number += count_newlines(buf0, first_diff);

		if (first_diff < smaller)
		{
			switch (comparison_type)
			{
			case type_first_diff:
				{
					char byte_buf[INT_BUFSIZE_BOUND(off_t) * 2 + 1];
					char line_buf[INT_BUFSIZE_BOUND(off_t)];
					char const *byte_num;
					char const *line_num = offtostr(line_number, line_buf);

					if (opt_print_hex)
					{
						if (offsets[0] != offsets[1])
							sprintf(byte_buf, "0x%llx-0x%llx", (unsigned long long)byte_number + offsets[0], (unsigned long long)byte_number + offsets[1]);
						else
							sprintf(byte_buf, "0x%llx", (unsigned long long)byte_number + offsets[0]);
						byte_num = byte_buf;
					} else
					{
						byte_num = offtostr(byte_number, byte_buf);
					}
					if (!opt_print_bytes)
					{
						/* See POSIX 1003.1-2001 for this format.  This
						   message is used only in the POSIX locale, so it
						   need not be translated.  */
						static char const char_message[] = "%s %s differ: char %s, line %s\n";

						/* The POSIX rationale recommends using the word
						   "byte" outside the POSIX locale.  Some gettext
						   implementations translate even in the POSIX
						   locale if certain other environment variables
						   are set, so use "byte" if a translation is
						   available, or if outside the POSIX locale.  */
						static char const byte_msgid[] = N_("%s %s differ: byte %s, line %s\n");
						char const *byte_message = _(byte_msgid);
						int use_byte_message = byte_message != byte_msgid;

						printf(use_byte_message ? byte_message : char_message, file[0], file[1], byte_num, line_num);
					} else
					{
						unsigned char c0 = buf0[first_diff];
						unsigned char c1 = buf1[first_diff];
						char s0[5];
						char s1[5];

						sprintc(s0, c0);
						sprintc(s1, c1);
						if (opt_print_hex)
						{
							printf (_("%s %s differ: byte %s, line %s is 0x%02x %s 0x%02x %s\n"),
								file[0], file[1], byte_num, line_num, c0, s0, c1, s1);
						} else
						{
							printf(_("%s %s differ: byte %s, line %s is %3o %s %3o %s\n"),
								file[0], file[1], byte_num, line_num, c0, s0, c1, s1);
						}
					}
				}
				/* Fall through.  */
			case type_status:
				return EXIT_FAILURE;

			case type_all_diffs:
				do
				{
					unsigned char c0 = buf0[first_diff];
					unsigned char c1 = buf1[first_diff];

					if (c0 != c1)
					{
						char byte_buf[INT_BUFSIZE_BOUND(off_t)];
						char const *byte_num;
						
						if (opt_print_hex)
						{
							if (offsets[0] != offsets[1])
								sprintf(byte_buf, "0x%llx-0x%llx", (unsigned long long)byte_number + offsets[0], (unsigned long long)byte_number + offsets[1]);
							else
								sprintf(byte_buf, "0x%llx", (unsigned long long)byte_number + offsets[0]);
							byte_num = byte_buf;
						} else
						{
							byte_num = offtostr(byte_number, byte_buf);
						}
						if (!opt_print_bytes)
						{
							/* See POSIX 1003.1-2001 for this format.  */
							if (opt_print_hex)
								printf ("%*s 0x%02x 0x%02x\n", offset_width, byte_num, c0, c1);
							else
								printf("%*s %3o %3o\n", offset_width, byte_num, c0, c1);
						} else
						{
							char s0[5];
							char s1[5];

							sprintc(s0, c0);
							sprintc(s1, c1);
							if (opt_print_hex)
								printf ("%*s 0x%02x %-4s 0x%02x %s\n", offset_width, byte_num, c0, s0, c1, s1);
							else
								printf("%*s %3o %-4s %3o %s\n", offset_width, byte_num, c0, s0, c1, s1);
						}
					}
					byte_number++;
					first_diff++;
				} while (first_diff < smaller);
				differing = -1;
				break;

			case type_no_stdout:
				differing = 1;
				break;
			}
		}

		if (read0 != read1)
		{
			if (differing <= 0 && comparison_type != type_status)
			{
				/* See POSIX 1003.1-2001 for this format.  */
				fprintf(stderr, _("cmp: EOF on %s\n"), file[read1 < read0]);
			}

			return EXIT_FAILURE;
		}
	} while (differing <= 0 && read0 == buf_size);

	return differing == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}


static char const *const option_help_msgid[] = {
	N_("-b, --print-bytes          print differing bytes"),
	N_("-i, --ignore-initial=SKIP         skip first SKIP bytes of both inputs"),
	N_("-i, --ignore-initial=SKIP1:SKIP2  skip first SKIP1 bytes of FILE1 and\n"
	   "                                         first SKIP2 bytes of FILE2"),
	N_("-l, --verbose              output byte numbers and differing byte values"),
	N_("-x, --hex                  print values in hexadecimal"),
	N_("-n, --bytes=LIMIT          compare at most LIMIT bytes"),
	N_("-s, --quiet, --silent      suppress all normal output"),
	N_("    --help                 display this help and exit"),
	N_("-v, --version              output version information and exit"),
	0
};

static void usage(void)
{
	char const *const *p;

	printf(_("Usage: %s [OPTION]... FILE1 [FILE2 [SKIP1 [SKIP2]]]\n"), program_name);
	printf("%s\n", _("Compare two files byte by byte."));
	printf("\n%s\n\n",
		   _("The optional SKIP1 and SKIP2 specify the number of bytes to skip\n"
			 "at the beginning of each file (zero by default)."));

	fputs(_("\
Mandatory arguments to long options are mandatory for short options too.\n\
"), stdout);
	for (p = option_help_msgid; *p; p++)
		printf("  %s\n", _(*p));
	printf("\n%s\n\n%s\n%s\n", _("SKIP values may be followed by the following multiplicative suffixes:\n\
kB 1000, K 1024, MB 1,000,000, M 1,048,576,\n\
GB 1,000,000,000, G 1,073,741,824, and so on for T, P, E, Z, Y."),
		_("If a FILE is '-' or missing, read standard input."),
		_("Exit status is 0 if inputs are the same, 1 if different, 2 if trouble."));
}


static void print_version(void)
{
}


int main(int argc, char **argv)
{
	int c, f, exit_status;
	size_t words_per_buffer;

	/* Parse command line options.  */

	while ((c = getopt_long(argc, argv, "bcxi:O:ln:sv", long_options, 0)) != -1)
	{
		switch (c)
		{
		case 'b':
		case 'c':						/* 'c' is obsolescent as of diffutils 2.7.3 */
			opt_print_bytes = TRUE;
			break;

		case 'i':
			specify_ignore_initial(0, &optarg, ':');
			if (*optarg++ == ':')
				specify_ignore_initial(1, &optarg, 0);
			else if (ignore_initial[1] < ignore_initial[0])
				ignore_initial[1] = ignore_initial[0];
			break;

		case 'O':
			specify_offset(0, &optarg, ':');
			if (*optarg++ == ':')
				specify_offset(1, &optarg, 0);
			break;

		case 'x':
			opt_print_hex = TRUE;
			break;

		case 'l':
			specify_comparison_type(type_all_diffs);
			break;

		case 'n':
			{
				__strtol_t n;

				if (xstrtoumax(optarg, 0, 0, &n, valid_suffixes) != LONGINT_OK)
					try_help(_("invalid --bytes value '%s'"), optarg);
				if (n < bytes)
					bytes = n;
			}
			break;

		case 's':
			specify_comparison_type(type_status);
			break;

		case 'v':
			print_version();
			check_stdout();
			return EXIT_SUCCESS;

		case HELP_OPTION:
			usage();
			check_stdout();
			return EXIT_SUCCESS;

		default:
			try_help(0, 0);
		}
	}
	
	if (optind == argc)
		try_help(_("missing operand after '%s'"), argv[argc - 1]);

	file[0] = argv[optind++];
	file[1] = optind < argc ? argv[optind++] : "-";

	for (f = 0; f < 2 && optind < argc; f++)
	{
		char *arg = argv[optind++];

		specify_ignore_initial(f, &arg, 0);
	}

	if (optind < argc)
		try_help(_("extra operand '%s'"), argv[optind]);

	for (f = 0; f < 2; f++)
	{
		/* If file[1] is "-", treat it first; this avoids a misdiagnostic if
		   stdin is closed and opening file[0] yields file descriptor 0.  */
		int f1 = f ^ (strcmp(file[1], "-") == 0);

		/* Two files with the same name and offset are identical.
		   But wait until we open the file once, for proper diagnostics.  */
		if (f && ignore_initial[0] == ignore_initial[1] && strcmp(file[0], file[1]) == 0)
			return EXIT_SUCCESS;

		if (strcmp(file[f1], "-") == 0)
		{
			file_desc[f1] = 0;
		} else
		{
			file_desc[f1] = open(file[f1], O_RDONLY | O_BINARY, 0);
		}
		
		if (file_desc[f1] < 0 || fstat(file_desc[f1], stat_buf + f1) != 0)
		{
			if (file_desc[f1] < 0 && comparison_type == type_status)
			{
				exit(EXIT_TROUBLE);
			} else
			{
				fprintf(stderr, "%s: %s\n", file[f1], strerror(errno));
				exit(EXIT_TROUBLE);
			}
		}
	}

	/* If the files are links to the same inode and have the same file position,
	   they are identical.  */

	if (0 < same_file(&stat_buf[0], &stat_buf[1])
		&& same_file_attributes(&stat_buf[0], &stat_buf[1]) && file_position(0) == file_position(1))
		return EXIT_SUCCESS;

	/* If output is redirected to the null device, we can avoid some of
	   the work.  */

	if (comparison_type != type_status)
	{
		struct stat outstat,
		 nullstat;

		if (fstat(STDOUT_FILENO, &outstat) == 0
			&& stat(NULL_DEVICE, &nullstat) == 0 && 0 < same_file(&outstat, &nullstat))
			comparison_type = type_no_stdout;
	}

	/* If only a return code is needed,
	   and if both input descriptors are associated with plain files,
	   conclude that the files differ if they have different sizes
	   and if more bytes will be compared than are in the smaller file.  */

	if (comparison_type == type_status && S_ISREG(stat_buf[0].st_mode) && S_ISREG(stat_buf[1].st_mode))
	{
		off_t s0 = stat_buf[0].st_size - file_position(0);
		off_t s1 = stat_buf[1].st_size - file_position(1);

		if (s0 < 0)
			s0 = 0;
		if (s1 < 0)
			s1 = 0;
		if (s0 != s1 && MIN(s0, s1) < bytes)
			exit(EXIT_FAILURE);
	}

	/* Get the optimal block size of the files.  */

	buf_size = buffer_lcm(STAT_BLOCKSIZE(stat_buf[0]), STAT_BLOCKSIZE(stat_buf[1]), PTRDIFF_MAX - sizeof(word));

	/* Allocate word-aligned buffers, with space for sentinels at the end.  */

	words_per_buffer = (buf_size + 2 * sizeof(word) - 1) / sizeof(word);
	buffer[0] = malloc(2 * sizeof(word) * words_per_buffer);
	buffer[1] = buffer[0] + words_per_buffer;

	exit_status = cmp();

	for (f = 0; f < 2; f++)
		if (close(file_desc[f]) != 0)
		{
			fprintf(stderr, "%s: %s\n", file[f], strerror(errno));
			exit(EXIT_TROUBLE);
		}
	if (exit_status != EXIT_SUCCESS && comparison_type < type_no_stdout)
		check_stdout();
	exit(exit_status);
	return exit_status;
}
