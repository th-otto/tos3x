#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <option.h>

#include <cout.h>
#include <sendc68.h>
#include "util.h"

static char const prg_name[] = "analyze";

static int verbose;

static struct libhdr arhdr;
static struct hdr2 couthd;
static struct hdr2 prg_hd;
static FILE *ifp;
static const char *ifilname;
static char *prg_image;
static long prg_size;

struct member {
	long tsize;
	long dsize;
	long bsize;
	long pos;
	long lfsize;
	char name[LIBNSIZE + 1];
	unsigned short *text;
	unsigned short *reloc;
	char *ignore;
	int found;
	struct member *next;
};
static struct member *members;
static struct member **sorted_members;
static size_t num_members;

struct segment {
	struct segment *next;
	struct segment *prev;
	long pos;
	long size;
	struct member *mem;
};
static struct segment shead;


static int readhdr(NOTHING)
{
	if (getchd(ifp, &couthd) != 0)
	{
		fprintf(stderr, "error reading %s\n", ifilname);
		return FALSE;
	}

	if (couthd.ch_magic < MAGIC || couthd.ch_magic > MAGICST)
	{
		fprintf(stderr, "file format error: %s %x\n", ifilname, couthd.ch_magic);
		return FALSE;
	}
	return TRUE;
}


static VOID load_program(P(const char *) filename)
PP(const char *filename;)
{
	FILE *fp;
	unsigned short *p;
	long i;
	
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "unable to open %s\n", filename);
		exit(EXIT_FAILURE);
	}
	if (getchd(fp, &prg_hd) < 0 || prg_hd.ch_magic != EX_MAGIC)
	{
		fclose(fp);
		fprintf(stderr, "%s: not an executable\n", filename);
		exit(EXIT_FAILURE);
	}
	prg_size = (prg_hd.ch_tsize + 1) >> 1;
	prg_image = (char *)malloc(prg_size * sizeof(unsigned short));
	p = (unsigned short *)prg_image;
	for (i = 0; i < prg_size; i++, p++)
	{
		if (lgetw(p, fp) < 0)
		{
			fprintf(stderr, "error reading %s\n", filename);
			exit(EXIT_FAILURE);
		}
	}
	fclose(fp);
	printf("%s: text 0x%08lx, data 0x%08lx, bss 0x%08lx\n",
		filename, prg_hd.ch_tsize, prg_hd.ch_dsize, prg_hd.ch_bsize);
}


static int swap_cpm_relocs_in(P(struct member *) m)
PP(struct member *m;)
{
	long slots = 0;
	long i;
	unsigned short relword;
	int last_upperword = 0;
	
	for (i = 0; i < m->tsize; i++)
	{
		lgetw(m->reloc + i, ifp);
	}
	for (i = 0; i < m->tsize; i++)
	{
		relword = m->reloc[i];
		switch (relword & 0x07)
		{
		case 0x00: /* absolute */
			if (last_upperword)
				m->ignore[i] = 1;
			break;
		case 0x01: /* relocation to data segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case 0x02: /* relocation to text segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case 0x03: /* relocation to bss segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case 0x04: /* relocation to external symbol */
			slots++;
			m->ignore[i] = 1;
			break;
		case 0x05: /* upper word of a long */
			if (last_upperword)
				fprintf(stderr, "%s: warning: 2 consecutive marker for upper word\n", arhdr.lfname);
			m->ignore[i] = 1;
			break;
		case 0x06: /* pc-relative to external symbol */
			slots++;
			m->ignore[i] = 1;
			break;
		case 0x07: /* first word of an instruction */
			if (last_upperword)
				fprintf(stderr, "%s: warning: upper word marker before instruction\n", arhdr.lfname);
			break;
		}
		last_upperword = (relword & 0x07) == 0x05;
	}

	return 0;
}


static int search_lib(NOTHING)
{
	register struct segment *s, *next;
	register size_t i;
	register long j;
	register struct member *m;
	register int match, thismatch, anymatch;
	const unsigned short *tryend;
	const unsigned short *start;
	long longest_match, matchlen;
	
	anymatch = FALSE;
	for (s = shead.next; s != &shead; s = next)
	{
		next = s->next;
		if (s->mem != NULL)
		{
			continue;
		}
		for (i = 0; i < num_members; i++)
		{
			m = sorted_members[i];
			if ((m->tsize << 1) > s->size)
				continue;
			if (m->found)
				continue;
			match = FALSE;
			longest_match = 0;
			tryend = (const unsigned short *)(prg_image + s->pos + s->size - (m->tsize << 1));
			start = (const unsigned short *)(prg_image + s->pos);
			while (start <= tryend)
			{
				thismatch = TRUE;
				for (j = 0; j < m->tsize; j++)
				{
					if (!m->ignore[j] && start[j] != m->text[j])
					{
						matchlen = j << 1;
						if (matchlen > longest_match)
							longest_match = matchlen;
						thismatch = FALSE;
						break;
					}
				}
				if (thismatch)
				{
					match = TRUE;
					break;
				}
				start++;
			}
			next = s->next;
			if (match)
			{
				long pos = ((const char *)start) - prg_image;
				struct segment *news;
				
				anymatch = TRUE;
				matchlen = m->tsize << 1;
				m->found = TRUE;
				printf("%s found at %08lx-%08lx\n", m->name, pos, pos + (m->tsize << 1));
				if (pos == s->pos)
				{
					/* match at start of segment */
					if (matchlen == s->size)
					{
						/* exact match of segment */
						s->mem = m;
					} else
					{
						/* mark start of found text, put remainder in new segment */
						news = (struct segment *)malloc(sizeof(*s));
						news->pos = s->pos + matchlen;
						news->size = s->size - matchlen;
						news->mem = NULL;
						news->next = s->next;
						news->prev = s;
						s->next->prev = news;
						s->next = news;
						s->mem = m;
						s->size = matchlen;
						/* continue search with new segment */
						next = news;
					}
				} else
				{
					if ((pos + matchlen) == s->size)
					{
						/* match at end of segment */
						news = (struct segment *)malloc(sizeof(*s));
						news->pos = pos;
						news->size = matchlen;
						news->mem = m;
						news->next = s->next;
						news->prev = s;
						s->next->prev = news;
						s->next = news;
						s->size -= matchlen;
						/* continue search with old segment */
						next = s;
					} else
					{
						/* match in middle of segment */
						news = (struct segment *)malloc(sizeof(*s));
						news->pos = pos;
						news->size = matchlen;
						news->mem = m;
						news->next = s->next;
						news->prev = s;
						s->next->prev = news;
						s->next = news;
						s->size = pos - s->pos;
						/* continue search with old segment */
						next = s;

						s = news;
						news = (struct segment *)malloc(sizeof(*s));
						news->pos = s->pos + matchlen;
						news->size = matchlen;
						news->mem = NULL;
						news->next = s->next;
						news->prev = s;
						s->next->prev = news;
						s->next = news;
					}
				}
				break;
			}
		}
	}
	return anymatch;
}
 

/*
 * sort archive member by decreasing size,
 * so we try the larger ones first
 */
static int cmp_member(P(const VOIDPTR) _m1, P(const VOIDPTR) _m2)
PP(const VOIDPTR _m1;)
PP(const VOIDPTR _m2;)
{
	register const struct member *m1 = *((const struct member *const *)_m1);
	register const struct member *m2 = *((const struct member *const *)_m2);
	if (m1->tsize > m2->tsize)
		return -1;
	if (m1->tsize < m2->tsize)
		return 1;
	return 0;
}

	
int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	unsigned short magic;
	register struct member *m;
	struct member **last;
	
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <library> <program>\n", prg_name);
		return EXIT_FAILURE;
	}
	
	load_program(argv[2]);
	
	ifilname = argv[1];
	if ((ifp = fopen(ifilname, "rb")) == NULL)
	{
		fprintf(stderr, "unable to open %s\n", ifilname);
		return EXIT_FAILURE;
	}
	
	if (lgetw(&magic, ifp) != 0 || magic != LIBMAGIC)
	{
		fprintf(stderr, "%s: not an archive\n", ifilname);
		return EXIT_FAILURE;
	}
	
	num_members = 0;
	while (getarhd(ifp, &arhdr) == 0 && arhdr.lfname[0] != '\0')
	{
		long memberpos = ftell(ifp);
		char name[LIBNSIZE + 1];

		strncpy(name, arhdr.lfname, LIBNSIZE);
		name[LIBNSIZE] = '\0';
		if (readhdr() && couthd.ch_magic == EX_MAGIC)
		{
			long i;
			long l;
			long tsize;
			
			tsize = (couthd.ch_tsize + 1) >> 1;
			if (tsize <= 0)
			{
				if (verbose)
					fprintf(stderr, "%s has no text segment, ignored\n", name);
			} else
			{
				m = (struct member *)malloc(sizeof(*m));
				m->lfsize = arhdr.lfsize;
				m->tsize = tsize;
				m->dsize = couthd.ch_dsize;
				m->bsize = couthd.ch_bsize;
				strcpy(m->name, name);
				m->pos = ftell(ifp);
				m->next = NULL;
				m->found = FALSE;
				last = &members;
				while (*last)
					last = &(*last)->next;
				*last = m;
				num_members++;
				m->text = (unsigned short *)malloc(m->tsize * sizeof(*m->text));
				m->reloc = (unsigned short *)malloc(m->tsize * sizeof(*m->reloc));
				m->ignore = (char *)malloc(m->tsize * sizeof(*m->ignore));
				memset(m->ignore, 0, m->tsize * sizeof(*m->ignore));
				/* read text segment */
				for (i = 0; i < m->tsize; i++)
				{
					lgetw(m->text + i, ifp);
				}
				/* skip data segment */
				l = ((couthd.ch_dsize + 1) >> 1) << 1;
				fseek(ifp, l, SEEK_CUR);
				/* skip symbol table */
				l = couthd.ch_ssize;
				fseek(ifp, l, SEEK_CUR);
				if (couthd.ch_bstart == 0)
				{
					/* read relocation info */
					swap_cpm_relocs_in(m);
				} else
				{
					memset(m->reloc, 0, m->tsize * sizeof(*m->reloc));
				}
			}
		} else
		{
			fprintf(stderr, "%s not an object file, ignored\n", name);
		}
		
		{
			long l = arhdr.lfsize;
		
			if (l & 1)
				l++;
			fseek(ifp, memberpos + l, SEEK_SET);
		}
	}
	
	fclose(ifp);
	
	if (num_members == 0)
	{
		fprintf(stderr, "%s: no archive members found\n", ifilname);
		return EXIT_FAILURE;
	}
	
	{
		size_t i;
		sorted_members = (struct member **)malloc(num_members * sizeof(struct member *));
		for (i = 0, m = members; m; m = m->next, i++)
			sorted_members[i] = m;
		qsort(sorted_members, num_members, sizeof(struct member *), cmp_member);
	}
	
	if (verbose || 1)
	{
		size_t i;

		for (i = 0; i < num_members;i++)
		{
			m = sorted_members[i];
			printf("     %6ld %-*s text 0x%08lx, data 0x%08lx, bss 0x%08lx\n",
				m->lfsize, LIBNSIZE, m->name,
				m->tsize << 1, m->dsize, m->bsize);
		}
	}
	
	{
		struct segment *s = (struct segment *)malloc(sizeof(*s));
		shead.next = shead.prev = s;
		s->next = s->prev = &shead;
		s->pos = 0;
		s->size = prg_hd.ch_tsize;
		s->mem = NULL;
	}
	
	while (search_lib())
		;
	
	{
		struct segment *s;
		
		for (s = shead.next; s != &shead; s = s->next)
		{
			printf("0x%08lx - 0x%08lx %s\n", s->pos, s->pos + s->size, s->mem ? s->mem->name : "<unknown>");
		}
	}
	
	{
		struct segment *s, *next;
		
		for (s = shead.next; s != &shead; s = next)
		{
			next = s->next;
			free(s);
		}
	}
	
	{
		struct member *next;
		
		for (m = members; m; m = next)
		{
			next = m->next;
			free(m->text);
			free(m->reloc);
			free(m->ignore);
			free(m);
		}
	}
	free(sorted_members);
	
	free(prg_image);
	
	return EXIT_SUCCESS;
}
