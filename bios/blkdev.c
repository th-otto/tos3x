#include "bios.h"
#include <toserrno.h>
#include "tosvars.h"
#include "../bdos/bdosdefs.h"

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
#if TOSVERSION >= 0x200
int16_t bpbsums[BPBSECT]; /* BUG: should be part of BLKDEV */
#endif
char fd_latch[NUMFLOPPIES];
char fd_wp[NUMFLOPPIES];
char drivechange[NUMFLOPPIES];
long rseed;
int curflop;
BLKDEV blkdev[NUMFLOPPIES];


static int16_t getiword PROTO((const uint8_t *addr));
static int16_t sectsum PROTO((const int16_t *buf, int count));
static ERROR dorwabs PROTO((int16_t rw, char *buf, RECNO recnr, int16_t dev, int16_t cnt));

#if TOSVERSION < 0x200
#define dskbufp dskbuf
extern char dskbuf[];
#endif



/*
 * default function for system variable hdv_init
 */
/* 306de: 00e055da */
/* 104de: 00fc16ba */
/* 404: 00e04792 */
VOID bhdv_init(NOTHING)
{
	int dev;
	char unused[10];
	
	UNUSED(unused);
	fd_mediach = 82;
	for (dev = curflop = nflops = 0; dev < NUMFLOPPIES; dev++)
	{
		drivechange[dev] = MEDIANOCHANGE;
#if TP_24 /* SKIP_B */
		if (dev >= 1)
			break;
#endif
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
/* 104de: 00fc1732 */
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
/* 206de: 00e0526a */
/* 306de: 00e0565c */
/* 104de: 00fc173c */
/* 404: 00e0480c */
BPB *bhdv_getbpb(P(int16_t) dev)
PP(int16_t dev;)
{
	register BLKDEV *bdev;
	register BPB *bpb;
	register int16_t bps;
	register int16_t spc;
	char unused[8];
	ERROR err;
#if TOSVERSION >= 0x200
	const int16_t *buf;
#endif
	
	UNUSED(unused);
	if (dev >= NUMFLOPPIES)
		return NULL;
	bdev = &blkdev[dev];
	bpb = &bdev->bpb;
	do {
		err = floprd(dskbufp, NULL, dev, 1, 0, 0, BPBSECT);
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
	/* BUG: original code assumes 2 FATs */
	/* BUG: original code ignores res field of bootsector */
#if TP_25 | !BINEXACT /* ED_BIOS */
	{
		int16_t res1;
		res1 = getiword(BS->res);
		if (res1 == 0)
			res1 = 1;
		bpb->fatrec = res1;
		if (BS->fat >= 2)
			bpb->fatrec += bpb->fsiz;
	}
#else
	bpb->fatrec = bpb->fsiz + 1;
#endif
	bpb->clsizb = bpb->recsiz * bpb->clsiz;
	bpb->rdlen = (getiword(BS->dir) << 5) / bpb->recsiz;
	bpb->datrec = bpb->fatrec + bpb->rdlen + bpb->fsiz;
	bpb->numcl = (getiword(BS->sec) - bpb->datrec) / bpb->clsiz;
	bpb->b_flags = 0;
#if FAT1_SUPPORT
	if (BS->fat < 2)
	{
		bpb->b_flags |= B_1FAT;
	}
#endif
#if TP_25 /* ED_BIOS */
	if (BS->numcl > 4078)
		bpb->b_flags |= B_16;
#endif

	bdev->geometry.sides = getiword(BS->sides);
	bdev->geometry.spt = getiword(BS->spt);
	bdev->geometry.spc = bdev->geometry.sides * bdev->geometry.spt;
	bdev->geometry.hidden = getiword(BS->hid);
	bdev->geometry.tracks = getiword(BS->sec) / bdev->geometry.spc; /* BUG: no check for divide by zero */
	
#define i bps /* reuse register variable */

	for (i = 0; i < 3; i++)
		bdev->serial[i] = BS->serial[i];
#if TOSVERSION >= 0x200
	for (i = 0; i < 4; i++)
		bdev->serial2[i] = BS->serial2[i];
#endif
	
#if TOSVERSION >= 0x200
	buf = (const int16_t *)dskbufp;
	for (i = 0; i < BPBSECT; i++)
	{
		/* BUG: always stores sums for Drive 0 */
#if TP_25 /* ED_BIOS */
		bpbsums[dev == 0 ? i : (i + BPBSECT)] = sectsum(buf, SECTOR_SIZE / 2);
#else
		bpbsums[i] = sectsum(buf, SECTOR_SIZE / 2);
#endif
		buf += SECTOR_SIZE / 2;
	}
#endif

	drivechange[dev] = (fd_latch[dev] = fd_wp[dev]) ? MEDIAMAYCHANGE : MEDIANOCHANGE;
	
#undef i
#undef BS

	return &bdev->bpb;
}


/*
 * default function for system variable hdv_mediach
 */
/* 306de: 00e058ae */
/* 104de: 00fc18ec */
/* 404: 00e04a6c */
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
/* 104de: 00fc1956 */
#if TOSVERSION >= 0x200
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
			err = floprd(dskbufp, NULL, dev, 1, 0, 0, BPBSECT);
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
#else
static int ckmediach(P(int16_t) _dev)
PP(int16_t _dev;)
{
	register BLKDEV *bdev;
	register int ret;
	register int dev;
	register ERROR err;
	
	dev = _dev;
	bdev = &blkdev[dev];
	ret = bhdv_mediach(dev);
	if (ret == MEDIACHANGE)
	{
		return ret;
	} else if (ret == MEDIAMAYCHANGE)
	{
		do {
			err = floprd(dskbufp, NULL, dev, 1, 0, 0, BPBSECT);
			if (err < 0)
			{
				err = callcrit((int16_t)err, dev);
			}
		} while (err == CRITIC_RETRY_REQUEST);

		if (err < 0)
			return err;

#define BS ((struct fat16_bs *)dskbufp)

#define i ret /* reuse register variable */
		for (i = 0; i < 3; i++)
			if (BS->serial[i] != bdev->serial[i])
				return MEDIACHANGE;
		if (!(fd_latch[dev] = fd_wp[dev]))
			drivechange[dev] = MEDIANOCHANGE;
#undef i
#undef BS

	}
	
	return MEDIANOCHANGE;
}
#endif


/*
 * default function for system variable hdv_rw
 */
/* 306de: 00e05a96 */
/* 104de: 00fc1a24 */
/* 404: 00e04c5c */
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


/* 206de: 00e0572c */
/* 306de: 00e05b1e */
/* 104de: 00fc1aac */
/* 404: 00e04ce2 */
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
#if TOSVERSION >= 0x200
	char *p;
	int i;
#endif
	
	bdev = &blkdev[dev];
	odd = !(((intptr_t) buf) & 1) ? 0 : 1;
	/*
	 * This can happen if nobody called Getbpb() before.
	 * Assume single-sided, 9 sectors per track.
	 */
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
#if TOSVERSION >= 0x200
		if (bufp != buf)
			cpy512(buf, bufp);
		if ((rw & RW_WRITE) && (track | side) == 0 && sector < BPBSECT)
		{
			p = bufp;
			for (i = 0; i < numsect; i++)
			{
#if TP_25 /* ED_BIOS */
				if ((sector + i) >= BPBSECT)
					break;
				bpbsums[sector + (dev == 0 ? i : (i + BPBSECT))] = sectsum((const int16_t *)p, SECTOR_SIZE / 2);
#else
				/* BUG: overflows for (sector + numsect) > BPBSECT */
				/* BUG: always stores sums for Drive 0 */
				bpbsums[sector + i] = sectsum((const int16_t *)p, SECTOR_SIZE / 2);
#endif
				p += SECTOR_SIZE;
			}
		}
		sector++;
#else
		sector++;
#endif
		
		do {
			if (rw & RW_WRITE)
			{
#if TOSVERSION < 0x200
				if (bufp != buf)
					cpy512(buf, bufp);
#endif
				err = flopwrt(bufp, NULL, dev, sector, track, side, numsect);
				if (err == 0 && fverify)
				{
					err = flopver(dskbufp, NULL, dev, sector, track, side, numsect);
					if (err == 0 && getiword((const uint8_t *)dskbufp) != 0)
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
/* 206de: 00e05946 */
/* 306de: 00e05d38 */
/* 104de: 00fc1c76 */
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
/* 206de: 00e05996 */
/* 306de: 00e05d88 */
/* 104de: 00fc1cc6 */
/* 404: 00e04f54 */
int16_t bhdv_boot(NOTHING)
{
	register int ret;
	
	chdv_init();
#if !TP_26 /* KILL_BOOT */
	if (nflops != 0
#if TP_27 /* NORM_BOOT */
		&& (bootdev < NUMFLOPPIES)
#endif
		)
	{
		ret = 2;   /* couldn't load */
#if (OS_COUNTRY == CTRY_PL) & !TP_27
		/* this code results from applying the NORM_BOOT patch of TOSPATCH */
		/* this is same as call below, optimized to squeeze it in the original bytes */
		asm("cmp.w     (_bootdev).w,d7");
		asm("ble.s     L9998");
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
		if (floprd(dskbufp, NULL, 0, 1, 0, 0, 1) == 0)
		{
			ret = 0;
		} else if (fd_wp[0] == 0)
		{
			return 3;   /* unreadable */
		}
#endif
	} else
#endif /* TP_26 */
	{
#if (OS_COUNTRY == CTRY_PL) & !TP_27
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


#if TOSVERSION >= 0x200
#define NUM_PROTOBT_ENTRIES 6
#else
#define NUM_PROTOBT_ENTRIES 4
#endif
static char const proto_data[NUM_PROTOBT_ENTRIES * 19] = {
#define LEW(x) x & 0xff, x / 0x100
	/* bps               spc res     fat dir       sec         media spf      spt      sides   hid */
	   LEW(SECTOR_SIZE), 1,  LEW(1), 2,  LEW(64),  LEW(360),   0xfc, LEW(2),  LEW(9),  LEW(1), LEW(0),   /* 0 = SS, 40 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(720),   0xfd, LEW(2),  LEW(9),  LEW(2), LEW(0),   /* 1 = DS, 40 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(720),   0xf9, LEW(5),  LEW(9),  LEW(1), LEW(0),   /* 2 = SS, 80 tracks, SD */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(112), LEW(1440),  0xf9, LEW(5),  LEW(9),  LEW(2), LEW(0),   /* 3 = DS, 80 tracks, SD */
#if TOSVERSION >= 0x200
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(224), LEW(2880),  0xf0, LEW(5),  LEW(18), LEW(2), LEW(0),   /* 4 = DS, 80 tracks, HD */ /* BUG: MSDOS uses 1 spc */
	   LEW(SECTOR_SIZE), 2,  LEW(1), 2,  LEW(224), LEW(5760),  0xf0, LEW(10), LEW(36), LEW(2), LEW(0),   /* 5 = DS, 80 tracks, ED */ /* BUG: MSDOS uses 240 dir entries, and 9 spf */
#endif
#undef LEW
};

/*
 * XBIOS #18 - Protobt - Generate a standard boot-sector for a floppy disk.
 */
/* 306de: 00e05e04 */
/* 104de: 00fc1d42 */
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
		/* BUG: serial2 checked by floppy mediach detection, but not set here */
	}
	
	if (disktype >= 0
#if TOSVERSION >= 0x200
		&& disktype < NUM_PROTOBT_ENTRIES
#endif
		)
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
/* 104de: 00fc1e2e */
#if (TOSVERSION >= 0x200) | !BINEXACT
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
#else
static int16_t sectsum(P(const int16_t *) buf, P(int) count)
PP(const int16_t *buf;)
PP(int count;)
{
	register int16_t sum;
	
	for (sum = 0; count-- != 0; )
		sum += *buf++;
	return sum;
}
#endif


/* 306de: 00e05f22 */
/* 104de: 00fc1e5e */
static int16_t getiword(P(const uint8_t *) addr)
PP(const uint8_t *addr;)
{
	return ((addr[1] & 0xff) << 8) | (addr[0] & 0xff);
}
