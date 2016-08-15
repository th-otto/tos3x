#ifndef __FCNTL_H__
#define __FCNTL_H__ 1

#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

/* File access modes for `open' and `fcntl'.  */
#ifndef O_RDONLY
#define	O_RDONLY	0x00	/* Open read-only.  */
#define	O_WRONLY	0x01	/* Open write-only.  */
#define	O_RDWR		0x02	/* Open read/write.  */

/* Mask for file access modes.  This is system-dependent in case
   some system ever wants to define some other flavor of access.  */
#define	O_ACCMODE	(O_RDONLY|O_WRONLY|O_RDWR)
#endif

#endif /* __FCNTL_H__ */
