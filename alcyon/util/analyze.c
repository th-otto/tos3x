#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <option.h>

#include <cout.h>
#include <sendc68.h>
#include "util.h"

#define DEBUG 0

static char const prg_name[] = "analyze";

static int verbose;

static struct libhdr arhdr;
static struct hdr2 couthd;
static struct hdr2 prg_hd;
static FILE *ifp;
static const char *ifilname;
static char *prg_image;
static long prg_size;

struct symbol {
	unsigned short flags;
	long value;
	char name[SYNAMLEN + 1];
};

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
	size_t numsyms;
	struct symbol *symtab;
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
		switch (relword & RBMASK)
		{
		case DABS: /* absolute */
			if (last_upperword)
				m->ignore[i] = 1;
			break;
		case DRELOC: /* relocation to data segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case TRELOC: /* relocation to text segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case BRELOC: /* relocation to bss segment */
			slots++;
			m->ignore[i] = 1;
			break;
		case EXTVAR: /* relocation to external symbol */
			slots++;
			m->ignore[i] = 1;
			break;
		case LUPPER: /* upper word of a long */
			if (last_upperword)
				fprintf(stderr, "%s: warning: 2 consecutive marker for upper word\n", arhdr.lfname);
			m->ignore[i] = 1;
			break;
		case EXTREL: /* pc-relative to external symbol */
			slots++;
			m->ignore[i] = 1;
			break;
		case INSABS: /* first word of an instruction */
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
	long matchlen;
	
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
			tryend = (const unsigned short *)(prg_image + s->pos + s->size - (m->tsize << 1));
			start = (const unsigned short *)(prg_image + s->pos);
			while (start <= tryend)
			{
				thismatch = TRUE;
				for (j = 0; j < m->tsize; j++)
				{
					if (!m->ignore[j] && start[j] != m->text[j])
					{
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
#if DEBUG
				printf("%s found at %08lx-%08lx\n", m->name, pos, pos + (m->tsize << 1));
#endif
				if (pos == s->pos)
				{
					/* match at start of segment */
					if (matchlen == s->size)
					{
						/* exact match of segment */
						s->mem = m;
#if DEBUG
						printf("exact match\n");
#endif
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
#if DEBUG
						printf("match at start match %08lx-%08lx remain %08lx-%08lx\n",
							s->pos, s->pos + s->size,
							news->pos, news->pos + news->size);
#endif
					}
				} else
				{
					if ((pos + matchlen) == (s->pos + s->size))
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
#if DEBUG
						printf("match at end remain %08lx-%08lx match %08lx-%08lx\n",
							s->pos, s->pos + s->size,
							news->pos, news->pos + news->size);
#endif
					} else
					{
						long remain = s->pos + s->size - pos - matchlen;
						
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
						news->size = remain;
						news->mem = NULL;
						news->next = s->next;
						news->prev = s;
						s->next->prev = news;
						s->next = news;

#if DEBUG
						printf("match in middle before  %08lx-%08lx match %08lx-%08lx after %08lx-%08lx\n",
							s->prev->pos, s->prev->pos + s->prev->size,
							s->pos, s->pos + s->size,
							news->pos, news->pos + news->size);
#endif
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
			long pos;
			
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
				m->symtab = NULL;
				m->numsyms = 0;
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
				/* read symbol table */
				pos = ftell(ifp);
				l = couthd.ch_ssize;
				l /= OSTSIZE;
				if (l > 0)
				{
					int n;
					
					m->numsyms = l;
					m->symtab = (struct symbol *)malloc(m->numsyms * sizeof(struct symbol));
					for (i = 0; i < l; i++)
					{
						struct symbol *sym = &m->symtab[i];
						for (n = 0; n < SYNAMLEN; n++)
							sym->name[n] = getc(ifp);
						sym->name[n] = '\0';
						lgetw(&sym->flags, ifp);
						lgetl(&sym->value, ifp);
					}
				}
				fseek(ifp, pos + couthd.ch_ssize, SEEK_SET);
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
	
	if (verbose || DEBUG)
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
			m = s->mem;
			printf("0x%08lx - 0x%08lx %s\n", s->pos, s->pos + s->size, m ? m->name : "<unknown>");
			if (m && m->symtab)
			{
				size_t i;
				struct symbol *sym;
				
				for (i = 0; i < m->numsyms; i++)
				{
					sym = &m->symtab[i];
					if ((sym->flags & SYTX) && !(sym->flags & SYXR) && (sym->flags & SYGL))
						printf("             0x%08lx %s\n", sym->value + s->pos, sym->name);
				}
			}
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
			free(m->symtab);
			free(m);
		}
	}
	free(sorted_members);
	
	free(prg_image);
	
	return EXIT_SUCCESS;
}
