/**************************************************************************
*   CTYPE.C  -  provides static tables for 'ctype.h' extern defs.
*	Modified 10/31/82 by MGL to add ___atab() for forced loading
***************************************************************************/


#include "lib.h"
#include <ctype.h>

char const __atab[] = {	/* table of ascii char types */

/********************************************************************/
/*	 nn0	nn1	nn2	nn3	nn4	nn5	nn6	nn7 */

/*	nul	soh	stx	etx	eot	enq	ack	bel */
/*00m*/ __c,	__c,	__c,	__c,	__c,	__c,	__c,	__c,

/*	bs	ht	nl	vt	np	cr	so	si  */
/*01m*/ __c,	__cs,	__cs,	__c,	__cs,	__cs,	__c,	__c,

/*	dle	dc1	dc2	dc3	dc4	nak	syn	etb */
/*02m*/ __c,	__c,	__c,	__c,	__c,	__c,	__c,	__c,

/*	can	em	sub	esc	fs	gs	rs	us  */
/*03m*/ __c,	__c,	__c,	__c,	__c,	__c,	__c,	__c,

/*	sp	!	"	#	$	%	&	'   */
/*04m*/ __ps,	__p,	__p,	__p,	__p,	__p,	__p,	__p,

/*	(	)	*	+	,	-	.	/   */
/*05m*/ __p,	__p,	__p,	__p,	__p,	__p,	__p,	__p,

/*	0	1	2	3	4	5	6	7   */
/*06m*/ __d,	__d,	__d,	__d,	__d,	__d,	__d,	__d,

/*	8	9	:	;	<	=	>	?   */
/*07m*/ __d,	__d,	__p,	__p,	__p,	__p,	__p,	__p,

/*	@	A	B	C	D	E	F	G   */
/*10m*/ __p,	__u,	__u,	__u,	__u,	__u,	__u,	__u,

/*	H	I	J	K	L	M	N	O   */
/*11m*/ __u,	__u,	__u,	__u,	__u,	__u,	__u,	__u,

/*	P	Q	R	S	T	U	V	W   */
/*12m*/ __u,	__u,	__u,	__u,	__u,	__u,	__u,	__u,

/*	X	Y	Z	[	\	]	^	_   */
/*13m*/ __u,	__u,	__u,	__p,	__p,	__p,	__p,	__p,

/*	`	a	b	c	d	e	f	g   */
/*14m*/ __p,	__l,	__l,	__l,	__l,	__l,	__l,	__l,

/*	h	i	j	k	l	m	n	o   */
/*15m*/ __l,	__l,	__l,	__l,	__l,	__l,	__l,	__l,

/*	p	q	r	s	t	u	v	w   */
/*16m*/ __l,	__l,	__l,	__l,	__l,	__l,	__l,	__l,

/*	x	y	z	{	|	}	~	del */
/*17m*/ __l,	__l,	__l,	__p,	__p,	__p,	__p,	__c

};

VOID ___atab(NOTHING)
{
}
