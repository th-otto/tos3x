/* List of all known TOS-ERRORs */

#define TOS_E_OK      0 /* Ok = no error */
#define TOS_ERROR    -1 /* Error: unknown error */
#define TOS_EDRVNR   -2 /* Drive not ready: device timeout */
#define TOS_EUNCMD   -3 /* Unknown command: device access */
#define TOS_E_CRC    -4 /* CRC error: sector read */
#define TOS_EBADRQ   -5 /* Bad request: error at device request */
#define TOS_E_SEEK   -6 /* Seek error: wrong tracknumber */
#define TOS_EMEDIA   -7 /* Unknown media: bootsector defect or not there */
#define TOS_ESECNF   -8 /* Sector not found: wrong sectornumber */
#define TOS_EPAPER   -9 /* Out of paper: printer not ready */
#define TOS_EWRITF  -10 /* Write fault */
#define TOS_EREADF  -11 /* Read fault */
#define TOS_EGENRL  -12 /* General error: reserved for future catastrophes */
#define TOS_EWRPRO  -13 /* Write on write-protected media */
#define TOS_E_CHNG  -14 /* Media change detected */
#define TOS_EUNDEV  -15 /* Unknown device */
#define TOS_EBADSF  -16 /* Bad sectors on format */
#define TOS_EOTHER  -17 /* Insert other disk (request) */
#define TOS_EINSERT -18 /* Insert disk: Metados error */
#define TOS_EDVNRSP -19 /* Device not responding: Metados error */

#define TOS_EINVFN  -32 /* Invalid function number: GEMDOS */
#define TOS_ENOSYS  TOS_EINVFN
#define TOS_EFILNF  -33 /* File not found */
#define TOS_EPTHNF  -34 /* Path not found */
#define TOS_ENHNDL  -35 /* Handle pool exhausted */
#define TOS_EACCDN  -36 /* Access denied */
#define TOS_EIHNDL  -37 /* Invalid handle */
#define TOS_ENSMEM  -39 /* Insufficient memory */
#define TOS_EIMBA   -40 /* Invalid memory block address */
#define TOS_EDRIVE  -46 /* Invalid drive specification */
#define TOS_ENSAME  -48 /* Not the same drive */
#define TOS_ENMFIL  -49 /* No more files: Fsfirst/Fsnext */
#define TOS_ELOCKED -58 /* Record is locked */
#define TOS_ENSLOCK -59 /* Matching lock not found */
#define TOS_EBADARG -64 /* Range error/Bad argument */
#define TOS_EINTRN  -65 /* GEMDOS internal error */
#define TOS_EPLFMT  -66 /* Invalid executable file format */
#define TOS_EGSBF   -67 /* Memory block groth failure */
#define TOS_EBREAK  -68 /* Break by CTRL-C: KAOS/MagiC */
#define TOS_EXCPT   -69 /* Break by Exception: KAOS/MagiC */

#define TOS_EEXIST  -85 /* file exist, try later */
#define TOS_ENAMETOOLONG -86 /* name too long */
#define TOS_ENOTTY	-87	/* not a tty */
#define TOS_ERANGE	-88	/* range error */
#define TOS_EDOM	-89	/* domain error */
#define TOS_EIO     -90 /* I/O error */
#define TOS_ENOSPC  -91 /* disk full */

#define TOS_EMOUNT  -200 /* used by MiNT-kernel */

