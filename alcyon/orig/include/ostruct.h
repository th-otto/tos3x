typedef struct
{
    char      d_reserved[21];  /* reserved for GEMDOS   */
    char      d_attrib;        /* file attribut         */
    short     d_time;          /* packed time           */
    short     d_date;          /* packed date           */
    long      d_length;        /* filesize              */
    char      d_fname[14];     /* file name             */
} DTA;

