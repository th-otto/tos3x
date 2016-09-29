* #include "asmdefs.h"
* #include "tosvars.inc"

mmutable_ram equ $700 ; CPU root table in memory

/*
 * pmmu tree
 */
#define tia  mmutable_ram
#define tib1 mmutable_ram+4*16
#define tib2 mmutable_ram+4*32
#define tic  mmutable_ram+4*48


/*
 * pmmu descriptor flags
 */
/* short-format page descriptor */
#define PD   $01
/* short-format table descriptor */
#define TD   $02
/* cache inhibit (page descriptors only) */
#define CI   $40

/*
 * some macros useful for table creation
 */
#define TABLE(table)    table+TD
#define PAGE(addr)      addr+PD
#define PAGE_CI(addr)   addr+CI+PD

		text

		xdef initmmu
initmmu:
        lea.l     mmutable_ram,a0
        lea.l     mmurom_table,a1
        move.w    #(mmuromend-mmurom_table)/4-1,d0
initmmu1:
        move.l    (a1)+,(a0)+
        dbf       d0,initmmu1
        TO_CRP(mmu_crp)
        TO_TC(mmu_tc)
        TO_TTR0(mmu_tt0)
        TO_TTR1(mmu_tt1)
        rts

	    data
mmurom_table:
/* tia: top level table (for 0x00000000-0xffffffff) */
        dc.l      TABLE(tib1)        /* for 0x00000000-0x0fffffff, use table tib1 */
        dc.l      PAGE($10000000)    /* map 0x10000000-0x7fffffff to the same */
        dc.l      PAGE($20000000)    /*   physical addresses, allow caching   */
        dc.l      PAGE($30000000)
        dc.l      PAGE($40000000)
        dc.l      PAGE($50000000)
        dc.l      PAGE($60000000)
        dc.l      PAGE($70000000)
        dc.l      PAGE_CI($80000000)   /* map 0x80000000-0xefffffff to the same */
        dc.l      PAGE_CI($90000000)   /*   physical addresses, DON'T allow caching */
        dc.l      PAGE_CI($a0000000)
        dc.l      PAGE_CI($b0000000)
        dc.l      PAGE_CI($c0000000)
        dc.l      PAGE_CI($d0000000)
        dc.l      PAGE_CI($e0000000)
        dc.l      TABLE(tib2)        /* for 0xf0000000-0xffffffff, use table tib2 */
/* tib1: second level table (for 0x00000000-0x0fffffff) */
        dc.l      TABLE(tic)         /* for 0x00000000-0x00ffffff, use table tic */
        dc.l      PAGE($01000000)    /* map 0x01000000-0x0fffffff to the same */
        dc.l      PAGE($02000000)    /*   physical addresses, allow caching   */
        dc.l      PAGE($03000000)
        dc.l      PAGE($04000000)
        dc.l      PAGE($05000000)
        dc.l      PAGE($06000000)
        dc.l      PAGE($07000000)
        dc.l      PAGE($08000000)
        dc.l      PAGE($09000000)
        dc.l      PAGE($0a000000)
        dc.l      PAGE($0b000000)
        dc.l      PAGE($0c000000)
        dc.l      PAGE($0d000000)
        dc.l      PAGE($0e000000)
        dc.l      PAGE($0f000000)
/* tib2: second-level table (for 0xf0000000-0xffffffff) */
        dc.l      PAGE_CI($00000000)  /* map 0xf0000000-0xfeffffff to the same */
        dc.l      PAGE_CI($01000000)  /*   physical addresses, DON'T allow caching */
        dc.l      PAGE_CI($02000000)
        dc.l      PAGE_CI($03000000)
        dc.l      PAGE_CI($04000000)
        dc.l      PAGE_CI($05000000)
        dc.l      PAGE_CI($06000000)
        dc.l      PAGE_CI($07000000)
        dc.l      PAGE_CI($08000000)
        dc.l      PAGE_CI($09000000)
        dc.l      PAGE_CI($0a000000)
        dc.l      PAGE_CI($0b000000)
        dc.l      PAGE_CI($0c000000)
        dc.l      PAGE_CI($0d000000)
        dc.l      PAGE_CI($0e000000)
        dc.l      TABLE(tic)           /* for 0xf0000000-0xffffffff, use table tic */
/* tic: third-level table for standard TT/Falcon addresses */
        dc.l      PAGE($00000000)      /* map 0xff000000-0xffefffff */
        dc.l      PAGE($00100000)      /*   0x00000000-0x00efffff, allow caching       */
        dc.l      PAGE($00200000)
        dc.l      PAGE($00300000)
        dc.l      PAGE($00400000)
        dc.l      PAGE($00500000)
        dc.l      PAGE($00600000)
        dc.l      PAGE($00700000)
        dc.l      PAGE($00800000)
        dc.l      PAGE($00900000)
        dc.l      PAGE($00a00000)
        dc.l      PAGE($00b00000)
        dc.l      PAGE($00c00000)
        dc.l      PAGE($00d00000)
        dc.l      PAGE($00e00000)
        dc.l      PAGE_CI($00f00000)   /* map 0xfff00000-0xffffffff 0x00f00000-0x00ffffff, DON'T allow caching */
mmuromend:

mmu_crp:
        dc.l                      $80000002,mmutable_ram; no limit, short desc, addr $700
mmu_tc:
        dc.l                      $80f04445 ; enable, pagesize 32k, IS=0, TIA=4, TIB=4, TIC=4, TID=5
mmu_tt0:
        dc.l                      $017e8107 ; for 0x01000000-0x7fffffff, caching allowed
mmu_tt1:
        dc.l                      $807e8507 ; for 0x80000000-0xfeffffff, *caching inhibited*
