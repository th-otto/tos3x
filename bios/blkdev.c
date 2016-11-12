#include "bios.h"
#include <toserrno.h>
#include "tosvars.h"
#include "ctrycodes.h"

/* read/write flags */
#define RW_READ             0
#define RW_WRITE            1
/* bit masks */
#define RW_RW               1
#define RW_NOMEDIACH        2
#define RW_NORETRIES        4
#define RW_NOTRANSLATE      8

#define CRITIC_RETRY_REQUEST 0x00010000L    /* special value returned by etv_critic */

#define BOOTABLE_CHECKSUM   0x1234

long fd_mediach;
int fd_err[NUMFLOPPIES]; /* only set once in bhdv_init; unused otherwise */
char drivechange[NUMFLOPPIES];
long fd_lastacc[NUMFLOPPIES];
int16_t bpbsums[BPBSECT]; /* BUG: should be part of BLKDEV */
char fd_latch[NUMFLOPPIES];
char fd_wp[NUMFLOPPIES];
char drivechange[NUMFLOPPIES];
long rseed;
int curflop;
BLKDEV blkdev[NUMFLOPPIES];


static int16_t getiword PROTO((const uint8_t *addr));
static int16_t sectsum PROTO((const int16_t *buf, int count));
static ERROR dorwabs PROTO((int16_t rw, char *buf, RECNO recnr, int16_t dev, int16_t cnt));


/*
 * default function for system variable hdv_init
 */
/* 306de: 00e055da */
VOID bhdv_init(NOTHING)
{
	int dev;
	char unused[10];
	
	UNUSED(unused);
	fd_mediach = 82;
	for (dev = curflop = nflops = 0; dev < NUMFLOPPIES; dev++)
	{
		drivechange[dev] = MEDIANOCHANGE;
#if BINEXACT
		/*
		 * Another strange hack: parameters are wrong,
		 * but the optimizer will convert the last 2 zeroes
		 * into a single 0L.
		 * If you pass NULL, that will be a single parameter,
		 * the compiler generates different code,
		 * and the optimizer will be fooled.
		 */
		if ((fd_err[dev] = flopini(NULL, NULL, dev, 0, 0, 0)) == 0)
#else		
		if ((fd_err[dev] = flopini(NULL, NULL, dev, 0, NULL)) == 0)
#endif
		{
			++nflops;
			drvbits |= 3;
		}
	}
}


/*
 * XBIOS #11 - Dbmsg - Output debug message
 *
 * Available Only if a resident debugger was loaded,
 * which supports this call. The only debugger that currently
 * supports this call is the Atari debugger.
 * Was never implemented in any offical ROM version.
 */
/* 306de: 00e05652 */
ERROR dbmsg(P(int16_t) rsrvd, P(int16_t) msg_num, P(int32_t) msg_arg)
PP(int16_t rsrvd;)
PP(int16_t msg_num;)
PP(int32_t msg_arg;)
{
	UNUSED(rsrvd);
	UNUSED(msg_num);
	UNUSED(msg_arg);
	return E_OK;
}


/*
 * default function for system variable hdv_bpb
 */
/* 306de: 00e0565c */
BPB *bhdv_getbpb(P(int16_t) dev)
PP(int16_t dev;)
{
	register BLKDEV *bdev;
	register BPB *bpb;
	register int16_t bps;
	register int16_t spc;
	char unused[8];
	ERROR err;
	const int16_t *buf;
	
	UNUSED(unused);
	if (dev >= NUMFLOPPIES)
		return NULL;
	bdev = &blkdev[dev];
	bpb = &bdev->bpb;
	do {
#ifdef __ALCYON__
		/* 0L = ugly hack to pass 2 zeroes */
		err = floprd(dskbufp, NULL, dev, 1, 0L, BPBSECT);
#else
		err = floprd(dskbufp, NULL, dev, 1, 0, 0, BPBSECT);
#endif
		if (err < 0)
		{
			err = callcrit((int16_t)err, dev);
		}
	} while (err == CRITIC_RETRY_REQUEST);
	
	if (err < 0)
		return NULL;
	
#define BS ((struct fat16_bs *)dskbufp)

	if ((bps = getiword(BS->bps)) <= 0 ||
		(spc = BS->spc & 0xff) <= 0)
		return NULL;
	
	bpb->recsiz = bps;
	bpb->clsiz = spc;
	bpb->fsiz = getiword(BS->spf);
	/* BUG: assumes 2 FATs */
	/* BUG: ignores res field of bootsector */
	bpb->fatrec = bpb->fsiz + 1;
	bpb->clsizb = bpb->recsiz * bpb->clsiz;
	bpb->rdlen = (getiword(BS->dir) << 5) / bpb->recsiz;
	bpb->datrec = bpb->fatrec + bpb->rdlen + bpb->fsiz;
	bpb->numcl = (getiword(BS->sec) - bpb->datrec) / bpb->clsiz;
	bpb->b_flags = 0;
	if (BS->fat < 2)
		bpb->b_flags |= B_1FAT;
	
	bdev->geometry.sides = getiword(BS->sides);
	bdev->geometry.spt = getiword(BS->spt);
	bdev->geometry.spc = bdev->geometry.sides * bdev->geometry.spt;
	bdev->geometry.hidden = getiword(BS->hid);
	bdev->geometry.tracks = getiword(BS->sec) / bdev->geometry.spc;
	
#define i bps /* reuse register variable */

	for (i = 0; i < 3; i++)
		bdev->serial[i] = BS->serial[i];
	for (i = 0; i < 4; i++)
		bdev->serial2[i] = BS->serial2[i];
	
	buf = (const int16_t *)dskbufp;
	for (i = 0; i < BPBSECT; i++)
	{
		/* BUG: always stores sums for Drive 0 */
		bpbsums[i] = sectsum(buf, SECTOR_SIZE / 2);
		buf += SECTOR_SIZE / 2;
	}

	drivechange[dev] = (fd_latch[dev] = fd_wp[dev]) ? MEDIAMAYCHANGE : MEDIANOCHANGE;
	
#undef i
#undef BS

	return &bdev->bpb;
}


/*
 * default function for system variable hdv_mediach
 */
/* 306de: 00e058ae */
int16_t bhdv_mediach(P(int16_t) dev)
PP(int16_t dev;)
{
	register int d;
	register char *p;
	
	if (dev >= NUMFLOPPIES)
		return E_UNDEV;
	d = dev;
	p = &drivechange[d];
	if (*p == MEDIACHANGE)
		return MEDIACHANGE;
	if (fd_latch[d])
		*p = MEDIAMAYCHANGE;
	return ((frclock - fd_lastacc[d]) < fd_mediach) ? MEDIANOCHANGE : *p;
}


/* 306de: 00e05918 */
static int ckmediach(P(int16_t) dev)
PP(int16_t dev;)
{
	register BLKDEV *bdev;
	register int ret;
	register ERROR err;
	register const uint8_t *p1;
	register const uint8_t *p2;
	int16_t *sums;
	
	bdev = &blkdev[dev];
	ret = bhdv_mediach(dev);
	if (ret == MEDIACHANGE)
	{
		return ret;
	} else if (ret == MEDIAMAYCHANGE)
	{
		do {
#ifdef __ALCYON__
			/* 0L = ugly hack to pass 2 zeroes */
			err = floprd(dskbufp, NULL, dev, 1, 0L, BPBSECT);
#else
			err = floprd(dskbufp, NULL, dev, 1, 0, 0, BPBSECT);
#endif
			if (err < 0)
			{
				err = callcrit((int16_t)err, dev);
			}
		} while (err == CRITIC_RETRY_REQUEST);

		if (err < 0)
			return err;

#define BS ((struct fat16_bs *)dskbufp)

		p1 = BS->serial;
		p2 = bdev->serial;
		if (p1[0] != p2[0] || p1[1] != p2[1] || p1[2] != p2[2])
			return MEDIACHANGE;
		p1 = BS->serial2;
		p2 = bdev->serial2;
		if (p1[0] != p2[0] || p1[1] != p2[1] || p1[2] != p2[2] || p1[3] != p2[3])
			return MEDIACHANGE;
		
		p1 = dskbufp;
		sums = &bpbsums[dev != 0 ? BPBSECT : 0];

#define i ret /* reuse register variable */
		for (i = 0; i < BPBSECT; i++)
		{
			if (sums[i] != sectsum((const int16_t *)p1, SECTOR_SIZE / 2))
				return MEDIACHANGE;
			p1 += SECTOR_SIZE;
		}
		if (!(fd_latch[dev] = fd_wp[dev]))
			drivechange[dev] = MEDIANOCHANGE;
#undef i
#undef BS

	}
	
	return MEDIANOCHANGE;
}


/*
 * default function for system variable hdv_rw
 */
/* 306de: 00e05a96 */
ERROR bhdv_rwabs(P(int16_t) rw, P(char *) buf, P(int16_t) cnt, P(RECNO) recnr, P(int16_t) dev, P(LRECNO) lrecnr)
PP(int16_t rw;)
PP(char *buf;)
PP(int16_t cnt;)
PP(RECNO recnr;)
PP(int16_t dev;)
PP(LRECNO lrecnr;) /* not supported by TOS */
{
	register int d;
	register ERROR err;
	
	if ((d = dev) >= NUMFLOPPIES)
		return E_UNDEV;
	if (nflops == 0)
		return E_DRVNR;
	if (buf == NULL)
	{
		drivechange[dev] = cnt;
		return E_OK;
	}
	if (rw < 2)
	{
		err = ckmediach(d);
		if (err != 0)
		{
			if (err == MEDIACHANGE)
				err = E_CHNG;
			return err;
		}
	}
	
	return dorwabs(rw, buf, recnr, d, cnt);
}


/* 306de: 00e05b1e */
static ERROR dorwabs(P(int16_t) rw, P(char *) buf, P(RECNO) recnr, P(int16_t) dev, P(int16_t) cnt)
PP(int16_t rw;)
PP(char *buf;)
PP(RECNO recnr;)
PP(int16_t dev;)
PP(int16_t cnt;)
{
	register BLKDEV *bdev;
	register ERROR err;
	register int track;
	register int side;
	register int sector;
	register int numsect;
	int odd;
	char *bufp;
	char *p;
	int i;
	
	bdev = &blkdev[dev];
	odd = !(((intptr_t) buf) & 1) ? 0 : 1;
	if (bdev->geometry.spc == 0)
		bdev->geometry.spt = bdev->geometry.spc = 9;
	while (cnt != 0)
	{
		bufp = odd ? dskbufp : buf;
		track = recnr / bdev->geometry.spc;
		sector = recnr % bdev->geometry.spc;
		if (sector < bdev->geometry.spt)
		{
			side = 0;
		} else
		{
			side = 1;
			sector -= bdev->geometry.spt;
		}
		if (odd)
		{
			numsect = 1;
		} else if ((bdev->geometry.spt - sector) < cnt)
		{
			numsect = bdev->geometry.spt - sector;
		} else
		{
			numsect = cnt;
		}
		if (bufp != buf)
			cpy512(buf, bufp);
		if ((rw & RW_WRITE) && (track | side) == 0 && sector < BPBSECT)
		{
			p = bufp;
			for (i = 0; i < numsect; i++)
			{
				/* BUG: always stores sums for Drive 0 */
				/* BUG: overflows for (sector + numsect) > BPBSECT */
				bpbsums[sector + i] = sectsum((const int16_t *)p, SECTOR_SIZE / 2);
				p += SECTOR_SIZE;
			}
		}
		sector++;
		
		do {
			if (rw & RW_WRITE)
			{
				err = flopwrt(bufp, NULL, dev, sector, track, side, numsect);
				if (err == 0 && fverify)
				{
					err = flopver(dskbufp, NULL, dev, sector, track, side, numsect);
					if (err == 0 && getiword(((struct fat16_bs *)dskbufp)->bra) != 0)
						err = E_BADSF;
				}
			} else
			{
				err = floprd(bufp, NULL, dev, sector, track, side, numsect);
				if (bufp != buf)
					cpy512(bufp, buf);
			}
			if (err < 0)
			{
				err = callcrit((int16_t)err, dev);
				if (rw < 2 && err == CRITIC_RETRY_REQUEST && ckmediach(dev) == MEDIACHANGE)
					err = E_CHNG;
			}
		} while (err == CRITIC_RETRY_REQUEST);
		
		if (err < 0)
			return err;
		
		buf += (intptr_t)numsect * SECTOR_SIZE;
		recnr += numsect;
		cnt -= numsect;
	}
	return E_OK;
}


/*
 * XBIOS #17 - Random - Random number generator
 */
/* 306de: 00e05d38 */
long random(NOTHING)
{
	if (rseed == 0)
	{
		rseed = (hz_200 << 16) | hz_200;
	}
	rseed = rseed * 0xBB40E62DL /* 3141592621L */ + 1;
	return (rseed >> 8) & 0xFFFFFFL;
}


/*
 * default function for system variable hdv_boot
 */
/* 306de: 00e05d88 */
int16_t bhdv_boot(NOTHING)
{
	register int ret;
	
	chdv_init();
	if (nflops != 0)
	{
		ret = 2;   /* couldn't load */
#if OS_COUNTRY == CTRY_PL
		/* apparently a patch that was applied by the maker of the PL version */
		asm("cmp.w     (_bootdev).w,d7");
		asm("ble.s     L9998");
		/* this is same as call below, optimized to squeeze it in the original bytes */
		asm("moveq.l   #1,d0");
		asm("move.w    d0,(a7)");
		asm("clr.l     -(a7)");
		asm("move.w    d0,-(a7)");
		asm("clr.w     -(a7)");
		asm("clr.l     -(a7)");
		asm("move.l    (_dskbufp).w,-(a7)");
		asm("bsr       _floprd");
		asm("adda.w    #$0010,a7");
		asm("tst.l     d0");
		asm("bne.s     L9991");
		asm("clr.w     d7");
		asm("bra.s     L9992");
		asm("L9991");
		asm("tst.b     _fd_wp");
		asm("bne.s     L9992");
		return 3;   /* unreadable */
		asm("L9992");
#else
#ifdef __ALCYON__
		/* 0L = ugly hack to pass 2 zeroes */
		if (floprd(dskbufp, NULL, 0, 1, 0L, 1) == 0)
#else
		if (floprd(dskbufp, NULL, 0, 1, 0, 0, 1) == 0)
#endif
		{
			ret = 0;
		} else if (fd_wp[0] == 0)
		{
			return 3;   /* unreadable */
		}
#endif
	} else
	{
#if OS_COUNTRY == CTRY_PL
		asm("L9998:")
#endif
		ret = 1;   /* no floppy */
	}
	if (ret != 0)
		return ret;
	if (sectsum((const int16_t *)dskbufp, SECTOR_SIZE / 2) == BOOTABLE_CHECKSUM)
		return 0; /* bootable */
	return 4; /* not valid boot sector */
}


#define NUM_PROTOBT_ENTRIES 6
static char const proto_data[NUM_PROTOBT_ENTRIES * 19] = {
#define LEW(x) x & 0xff, x / 0x100
	/* bps               spc res     fat dir       sec         media spf      spt      sides   hid */
	   LEW(SECTOR_SIZE), 1,  LEW(1), 2,  LEW(64),  LEW(360),   0xfc, LEW(2),  LEW(9),  LEW(1), LEW(0),   /* 0 = SS, 40 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(720),   0xfd, LEW(2),  LEW(9),  LEW(2), LEW(0),   /* 1 = DS, 40 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(720),   0xf9, LEW(5),  LEW(9),  LEW(1), LEW(0),   /* 2 = SS, 80 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(1440),  0xf9, LEW(5),  LEW(9),  LEW(2), LEW(0),   /* 3 = DS, 80 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(224), LEW(2880),  0xf0, LEW(5),  LEW(18), LEW(2), LEW(0),   /* 4 = DS, 80 tracks, HD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(224), LEW(5760),  0xf0, LEW(10), LEW(36), LEW(2), LEW(0),   /* 5 = DS, 80 tracks, ED */
#undef LEW
};

/*
 * XBIOS #18 - Protobt - Generate a standard boot-sector for a floppy disk.
 */
/* 306de: 00e05e04 */
VOID protobt(P(VOIDPTR) buf, P(int32_t) serialno, P(int16_t) disktype, P(int16_t) execflag)
PP(VOIDPTR buf;)
PP(int32_t serialno;)
PP(int16_t disktype;)
PP(int16_t execflag;)
{
	register int i;
	register int idx;
	register char *unused;
	int16_t *p;
	int16_t sum;
	
	UNUSED(unused);
	
	if (execflag < 0)
	{
		execflag = sectsum((const int16_t *)buf, SECTOR_SIZE / 2) != BOOTABLE_CHECKSUM ? FALSE : TRUE;
	}

#define BS ((struct fat16_bs *)buf)

	if (serialno >= 0)
	{
		if (serialno > 0xFFFFFFL)
			serialno = random();
		for (i = 0; i < 3; i++)
		{
			BS->serial[i] = serialno & 0xff;
			serialno >>= 8;
		}
	}
	
	if (disktype >= 0 && disktype < NUM_PROTOBT_ENTRIES)
	{
		idx = disktype * 19;
		for (i = 0; i < 19; idx++, i++)
		{
			(((char *)buf) + 11)[i] = proto_data[idx];
		}
	}
	
	for (sum = 0, p = (int16_t *)buf; p < ((int16_t *)buf) + (SECTOR_SIZE / 2 - 1); )
		sum += *p++;
	*p = BOOTABLE_CHECKSUM - sum;
	if (!execflag)
		++(*p);
	
#undef BS

}


/* 306de: 00e05ef8 */
static int16_t sectsum(P(const int16_t *) buf, P(int) count)
PP(const int16_t *buf;)
PP(int count;)
{
	register int16_t sum;
	register int16_t i;
	register const int16_t *p;
	
	for (i = count, p = buf, sum = 0; i-- != 0; )
		sum += *p++;
	return sum;
}


/* 306de: 00e05f22 */
static int16_t getiword(P(const uint8_t *) addr)
PP(const uint8_t *addr;)
{
	return ((addr[1] & 0xff) << 8) | (addr[0] & 0xff);
}
