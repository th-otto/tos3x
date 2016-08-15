/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#ifdef PDP11
    struct {
        char lobyte;
        char hibyte;
    };
    struct {
        char *ptrw1;
        char *ptrw2;
    };
    struct {
        short hiword;	/* formally wd1 */
        short loword;	/* formally wd2 */
    };
    struct {
        int swd1;
    };
#endif

#ifdef MC68000
    struct {
        char hibyte;
        char lobyte;
    };
    struct {
        char *ptrw2;
    };
    struct {
        short hiword;	/* formally wd1 */
        short loword;	/* formally wd2 */
    };
    struct {
        int swd1;
    };
#endif

#ifdef VAX11
    struct {
        short loword;
        short hiword;
    };
    struct {
        short swd1;
    };

    struct {
        char lobyte;
        char hibyte;
    };
    struct {
        char *ptrw2;
    };
#endif
