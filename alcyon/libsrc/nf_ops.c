#include "lib.h"
#include <mint/arch/nf_ops.h>
#include <mint/cookie.h>
#include <osbind.h>

#ifndef FALSE
#  define FALSE 0
#  define TRUE  1
#endif

#define NF_ID   0x7300
#define NF_CALL 0x7301

#ifdef __AHCC__

static long __asm__ __CDECL _nf_get_id(const char *feature_name)
{
	dc.w NF_ID
	rts
}


static long __asm__ __CDECL _nf_call(long id, ...)
{
	dc.w NF_CALL
	rts
}

static char const nf_version_str[] = NF_ID_VERSION;

static long __asm__ _nf_det(void)
{
	pea		nf_version_str
	moveq	#0,d0			/* assume no NatFeats available */
	move.l	d0,-(sp)
	lea		_nf_illegal(pc),a1
	move.l	0x0010.w,a0		/* illegal instruction vector */
	move.l	a1,0x0010.w
	move.l	sp,a1			/* save the ssp */

	nop						/* flush pipelines (for 68040+) */

	dc.w	NF_ID			/* Jump to NF_ID */
	tst.l	d0
	beq.s	_nf_illegal
	moveq	#1,d0			/* NatFeats detected */
	move.l	d0,(sp)

_nf_illegal:
	move.l	a1,sp
	move.l	a0,0x0010.w
	nop						/* flush pipelines (for 68040+) */
	move.l	(sp)+,d0
	addq.l	#4,sp			/* pop nf_version argument */
	rts
}

#else
#ifdef __PUREC__

static long nf_get_id_instr(void) NF_ID;
static long nf_call_instr(void) NF_CALL;

static long __CDECL _nf_get_id(const char *feature_name)
{
	UNUSED(feature_name);
	return nf_get_id_instr();
}


static long __CDECL _nf_call(long id, ...)
{
	UNUSED(id);
	return nf_call_instr();
}

static void push_a0(void *) 0x2f08;
static void moveq_0_d0(void) 0x7000;
static void moveq_1_d0(void) 0x7001;
static void push_d0(void) 0x2F00;
static void nop(void) 0x4e71;

/* lea nf_illegal(pc),a1 */
static void lea_nf_illegal_a(void) 0x43FA;
static void lea_nf_illegal_b(void) 0x0018;
#define lea_nf_illegal() lea_nf_illegal_a(); lea_nf_illegal_b()

/* move.l 0x0010.w,a1 */
static void fetch_illegal_vec_a(void) 0x2078;
static void fetch_illegal_vec_b(void) 0x0010;
#define fetch_illegal_vec() fetch_illegal_vec_a(); fetch_illegal_vec_b()

/* move.l a0,0x0010.w */
static void store_illegal_vec_a0_a(void) 0x21c8;
static void store_illegal_vec_a0_b(void) 0x0010;
#define store_illegal_vec_a0() store_illegal_vec_a0_a(); store_illegal_vec_a0_b()

/* move.l a1,0x0010.w */
static void store_illegal_vec_a1_a(void) 0x21c9;
static void store_illegal_vec_a1_b(void) 0x0010;
#define store_illegal_vec_a1() store_illegal_vec_a1_a(); store_illegal_vec_a1_b()

/* move.l sp,a1 */
static void get_sp(void) 0x224F;

/* tst.l d0 */
static void test_d0(void) 0x4a80;

/* beq.s _nf_illegal */
static void beqs_nf_illegal(void) 0x6704;

/* move d0,(sp) */
static void move_d0_sp(void) 0x2e80;

/* move.l a1,sp */
static void restore_sp(void) 0x2e49;

/* move.l (sp)+,d0 */
static void pop_d0(void) 0x201f;

/* addq.l #4,sp */
static long addq4_sp(void) 0x588F;

static long _nf_det(void)
{
	push_a0(NF_ID_VERSION);
	moveq_0_d0();			/* assume no NatFeats available */
	push_d0();
	lea_nf_illegal();
	fetch_illegal_vec();	/* illegal instruction vector */
	store_illegal_vec_a1();
	get_sp();				/* save the ssp */

	nop();					/* flush pipelines (for 68040+) */

	nf_get_id_instr();		/* Jump to NF_ID */
	test_d0();
	beqs_nf_illegal();
	moveq_1_d0();			/* NatFeats detected */
	move_d0_sp();

/* _nf_illegal: */
	restore_sp();
	store_illegal_vec_a0();
	nop();					/* flush pipelines (for 68040+) */
	
	pop_d0();
	return addq4_sp();		/* pop nf_version argument */
}

#endif /* __PUREC__ */
#endif /* __AHCC__ */

#ifdef __GNUC__

#define ASM_NATFEAT3(opcode) "\t.word " #opcode "\n"
#define ASM_NATFEAT2(opcode) ASM_NATFEAT3(opcode)
#define ASM_NATFEAT(n) ASM_NATFEAT2(n)

static long __attribute__((noinline)) __CDECL _nf_get_id(const char *feature_name)
{
	register long ret __asm__ ("d0");
	UNUSED(feature_name);
	__asm__ volatile(
		ASM_NATFEAT(NF_ID)
	: "=g"(ret)  /* outputs */
	: /* inputs  */
	: __CLOBBER_RETURN("d0") "d1" AND_MEMORY /* clobbered regs */
	);
	return ret;
}


static long __attribute__((noinline)) __CDECL _nf_call(long id, ...)
{
	register long ret __asm__ ("d0");
	UNUSED(id);
	__asm__ volatile(
		ASM_NATFEAT(NF_CALL)
	: "=g"(ret)  /* outputs */
	: /* inputs  */
	: __CLOBBER_RETURN("d0") "d1" AND_MEMORY /* clobbered regs */
	);
	return ret;
}


/*
 * on ColdFire, the NF_ID opcode is actually
 * "mvs.b d0,d1",
 * which means the following code will NOT detect
 * the presence of an emulator (should there ever
 * be an emulator capable of emulating a ColdFire processor).
 * Luckily, executing the code on a CF processor is still
 * harmless since all it does is clobber D1.
 */
static long _nf_det(void)
{
	register long ret __asm__ ("d0");
	register const char *nf_version_str __asm__("a1") = NF_ID_VERSION;
	
	__asm__ volatile(
	"\tmove.l	%1,-(sp)\n"
	"\tmoveq	#0,d0\n"			/* assume no NatFeats available */
	"\tmove.l	d0,-(sp)\n"
	"\tlea		(1f:w,pc),a1\n"
	"\tmove.l	(0x0010).w,a0\n"	/* illegal instruction vector */
	"\tmove.l	a1,(0x0010).w\n"
	"\tmove.l	sp,a1\n"			/* save the ssp */

	"\tnop\n"						/* flush pipelines (for 68040+) */

	ASM_NATFEAT(NF_ID)				/* Jump to NF_ID */
	"\ttst.l	d0\n"
	"\tbeq.s	1f\n"
	"\tmoveq	#1,d0\n"			/* NatFeats detected */
	"\tmove.l	d0,(sp)\n"

"1:\n"
	"\tmove.l	a1,sp\n"
	"\tmove.l	a0,(0x0010).w\n"
	"\tmove.l	(sp)+,d0\n"
	"\taddq.l	#4,sp\n"			/* pop nf_version argument */

	"\tnop\n"						/* flush pipelines (for 68040+) */
	: "=g"(ret)  /* outputs */
	: "g"(nf_version_str)		/* inputs  */
	: __CLOBBER_RETURN("d0") "a0", "d1" AND_MEMORY
	);
	return ret;
}

#endif /* __GNUC__ */


#ifdef __ALCYON__

static long _nf_det(NOTHING)
{
	asm("pea    _nf_vstr(pc)");
	asm("moveq  #0,d0");			/* assume no NatFeats available */
	asm("move.l d0,-(sp)");
	asm("lea    _nf_ill(pc),a1");
	asm("move.l $0010,a0");			/* illegal instruction vector */
	asm("move.l a1,$0010");
	asm("move.l sp,a1");			/* save the ssp */

	asm("nop");						/* flush pipelines (for 68040+) */

	asm("dc.w   $7300");			/* Jump to NF_ID */
	asm("tst.l  d0");
	asm("beq.s  _nf_ill");
	asm("moveq  #1,d0");			/* NatFeats detected */
	asm("move.l d0,(sp)");

asm("_nf_ill:");
	asm("move.l a1,sp");
	asm("move.l a0,$0010");
	asm("nop");						/* flush pipelines (for 68040+) */
	asm("move.l (sp)+,d0");
	asm("addq.l #4,sp");			/* pop nf_version argument */
	asm("bra.s nfret");

asm("__nf_get_id:");
	asm("dc.w $7300");
	asm("rts");

asm("__nf_call:");
	asm("dc.w $7301");
	asm("rts");

asm("_nf_vstr:");
	asm("dc.b 'N','F','_','V','E','R','S','I','O','N',0");
	asm("even");
asm("nfret:");
}

#endif 

static struct nf_ops _nf_ops = { _nf_get_id, _nf_call };
static struct nf_ops *nf_ops;

/* NatFeat code */
typedef struct {
	long magic;
	long __CDECL (*nfGetID) PROTO((const char *));
	long __CDECL (*nfCall) PROTO((long ID, ...));
} NatFeatCookie;


static int _nf_coo(NOTHING)
{
	NatFeatCookie *nf_ptr = NULL;

	if (Cookie_ReadJar(C___NF, (long *)&nf_ptr) == 0 ||		/* "__NF" */
		nf_ptr == NULL)
	{
		return FALSE;
	}

	if (nf_ptr->magic != 0x20021021L)		/* NatFeat magic constant */
	{
		return FALSE;
	}
	_nf_ops.get_id = nf_ptr->nfGetID;
	_nf_ops.call = nf_ptr->nfCall;
	return TRUE;
}



struct nf_ops *nf_init(NOTHING)
{
	/*
	 * The __NF cookie is deprecated, but there is currently
	 * no standard defined what to do e.g. on ColdFire, where the
	 * NF opcodes are not illegal, and the detection would fail.
	 * So there is current no choice but to check for the cookie,
	 * until we find a better solution.
	 */
	if (nf_ops == NULL)
	{
		VOIDPTR call;
		
		if (_nf_coo())
			nf_ops = &_nf_ops;
		call = _nf_det;
		if (Supexec(call))
			nf_ops = &_nf_ops;
	}
	return nf_ops;
}


long nf_get_id(P(const char *) feature_name)
PP(const char *feature_name;)
{
	struct nf_ops *ops;
	long id = 0;
	
	if ((ops = nf_init()) != NULL)
	{
		id = NF_GET_ID(ops, feature_name);
	}
	return id;
}
