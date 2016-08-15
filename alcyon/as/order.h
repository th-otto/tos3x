/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

#ifdef VAX11
	struct { char b4; char b3; char b2; char b1; };
	struct { char wb2; char wb1; };
#endif

#ifdef PDP11
	struct { char b2; char b1; char b4; char b3; };
	struct { char wb2; char wb1; };
#endif

#ifdef MC68000
	struct { char b1; char b2; char b3; char b4; };
	struct { char wb1; char wb2; };
#endif

