#ifndef __COMPILER_H__
#define __COMPILER_H__ 1

#ifndef __GNUC__
# ifndef __PUREC__
#   ifndef __TURBOC__
#     ifndef LATTICE
#       define __ALCYON__ 1
#     endif
#   endif
# endif
#endif

#ifdef __ALCYON__
#define VOID						/* Void function return	   */
#define VOIDPTR char *
#define NOTHING                     /* no parameters */
#define PROTO(p) ()
#define P(t,v) v
#define PP(t,v) t v
#define volatile
#define const
#else
#define NOTHING void                /* no parameters */
#define PROTO(p) p
#define P(t, v) t v
#define PP(t, v)
#endif

#ifdef __TURBOC__
#define CDECL cdecl
#endif

#ifndef VOID
#define VOID	void					/* Void function return	   */
#endif

#ifndef VOIDPTR
#define VOIDPTR	void *
#endif

#ifndef CDECL
#define CDECL
#endif


#endif /* __COMPILER_H__ */
