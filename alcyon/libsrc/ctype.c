/**************************************************************************
*   CTYPE.C  -  provides static tables for 'ctype.h' extern defs.
***************************************************************************/

#include "lib.h"
#include <ctype.h>

/* table of ascii char types */
char const __atab[256] = {

/********************************************************************/
/*	 nn0	nn1	nn2	nn3	nn4	nn5	nn6	nn7 */

/*	nul	soh	stx	etx	eot	enq	ack	bel */
/*00m*/ __C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,

/*	bs	ht	nl	vt	np	cr	so	si  */
/*01m*/ __C_c,	__C_cs,	__C_cs,	__C_c,	__C_cs,	__C_cs,	__C_c,	__C_c,

/*	dle	dc1	dc2	dc3	dc4	nak	syn	etb */
/*02m*/ __C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,

/*	can	em	sub	esc	fs	gs	rs	us  */
/*03m*/ __C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,	__C_c,

/*	sp	!	"	#	$	%	&	'   */
/*04m*/ __C_ps,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,

/*	(	)	*	+	,	-	.	/   */
/*05m*/ __C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,

/*	0	1	2	3	4	5	6	7   */
/*06m*/ __C_d,	__C_d,	__C_d,	__C_d,	__C_d,	__C_d,	__C_d,	__C_d,

/*	8	9	:	;	<	=	>	?   */
/*07m*/ __C_d,	__C_d,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,

/*	@	A	B	C	D	E	F	G   */
/*10m*/ __C_p,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,

/*	H	I	J	K	L	M	N	O   */
/*11m*/ __C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,

/*	P	Q	R	S	T	U	V	W   */
/*12m*/ __C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,	__C_u,

/*	X	Y	Z	[	\	]	^	_   */
/*13m*/ __C_u,	__C_u,	__C_u,	__C_p,	__C_p,	__C_p,	__C_p,	__C_p,

/*	`	a	b	c	d	e	f	g   */
/*14m*/ __C_p,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,

/*	h	i	j	k	l	m	n	o   */
/*15m*/ __C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,

/*	p	q	r	s	t	u	v	w   */
/*16m*/ __C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,	__C_l,

/*	x	y	z	{	|	}	~	del */
/*17m*/ __C_l,	__C_l,	__C_l,	__C_p,	__C_p,	__C_p,	__C_p,	__C_c

};
