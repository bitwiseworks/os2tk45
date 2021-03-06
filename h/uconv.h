/*
 * uconv.h - Unicode Conversions
 *
 * Function:
 *     Declare structures and prototypes for Unicode Conversion functions.
 *
 * Copyright:
 *     Copyright (C) IBM Corp. 1995, 1997
 *
 */

#ifndef _ULS_UCONV
   #define _ULS_UCONV
   #include <unidef.h>

   #ifdef __cplusplus
     extern "C" {
   #endif

   #ifndef _ULS_UCONVP
     #define _ULS_UCONVP
     typedef void * UconvObject;          /* uconv Type pointer            */
   #endif

   #ifndef _ULS_UNICHAR_DEFINED
     #define _ULS_UNICHAR_DEFINED
     typedef unsigned  short  UniChar ;
   #endif


   #ifndef ULS_ATTR
   #define ULS_ATTR
   /*
    * Substitution options
    */
   #define UCONV_OPTION_SUBSTITUTE_FROM_UNICODE  1
   #define UCONV_OPTION_SUBSTITUTE_TO_UNICODE    2
   #define UCONV_OPTION_SUBSTITUTE_BOTH          3

   /*
    * Conversion options
    */
   #define CVTTYPE_PATH     0x00000004   /* Treat string as a path   */
   #define CVTTYPE_CDRA     0x00000002   /* Use CDRA control mapping */
   #define CVTTYPE_CTRL7F   0x00000001   /* Treat 0x7F as a control  */

   /*
    * Conversion mask.  This is a bit mask.  If the bit is on, the
    * corresponding character 0x00-0x1f is treated as a control.
    * Otherwise it is treated as a glyph.
    */
   #define DSPMASK_DATA    0xffffffff
   #define DSPMASK_DISPLAY 0x00000000
   #define DSPMASK_TAB     0x00000200
   #define DSPMASK_LF      0x00000400
   #define DSPMASK_CR      0x00002000
   #define DSPMASK_CRLF    0x00002400

   /*
    * Encoding schemes.  This is the primary item used to check if a
    * a codepage is valid for a particular purpose.  A length check
    * may also be made in some cases.
    */
   enum uconv_esid {                     /* Process Display  VIO    GPI   */
       ESID_sbcs_data        = 0x2100,   /*    x      x      x       x    */
       ESID_sbcs_pc          = 0x3100,   /*    x      x      x       x    */
       ESID_sbcs_ebcdic      = 0x1100,   /*           x      x       x    */
       ESID_sbcs_iso         = 0x4100,   /*    x      x      x       x    */
       ESID_sbcs_windows     = 0x4105,   /*    x      x      x       x    */
       ESID_sbcs_alt         = 0xF100,   /*           x      x       x    */
       ESID_dbcs_data        = 0x2200,   /*           x              x    */
       ESID_dbcs_pc          = 0x3200,   /*    x      x      x       x    */
       ESID_dbcs_ebcdic      = 0x1200,   /*                          x    */
       ESID_mbcs_data        = 0x2300,   /*           x      x       x    */
       ESID_mbcs_pc          = 0x3300,   /*           x              x    */
       ESID_mbcs_ebcdic      = 0x1301,   /*                               */
       ESID_ucs_2            = 0x7200,   /*                               */
       ESID_ugl              = 0x72FF,   /*                               */
       ESID_utf_8            = 0x7807,   /*           x      x       x    */
       ESID_upf_8            = 0x78FF    /*    x      x      x       x    */
   };

   #ifndef __conv_endian_t
       #define __conv_endian_t

       #define     ENDIAN_SYSTEM   0x0000
       #define     ENDIAN_BIG      0xfeff
       #define     ENDIAN_LITTLE   0xfffe

       typedef struct _conv_endian_rec {
           unsigned short  source;   /* Used by FromUcs                   */
           unsigned short  target;   /* Used by ToUcs                     */
       } conv_endian_t;
   #endif

   typedef struct _uconv_attribute_t {
       unsigned long  version;       /* Q/S Version (must be zero)        */
       char           mb_min_len;    /* Q   Minimum char size             */
       char           mb_max_len;    /* Q   Maximum char size             */
       char           usc_min_len;   /* Q   UCS min size                  */
       char           usc_max_len;   /* Q   UCS max size                  */
       unsigned short esid;          /* Q   Encoding scheme ID            */
       char           options;       /* Q/S Substitution options          */
       char           state;         /* Q/S State for stateful convert    */
       conv_endian_t  endian;        /* Q/S Source and target endian      */
       unsigned long  displaymask;   /* Q/S Display/data mask             */
       unsigned long  converttype;   /* Q/S Conversion type               */
       unsigned short subchar_len;   /* Q/S MBCS sub len      0=table     */
       unsigned short subuni_len;    /* Q/S Unicode sub len   0=table     */
       char           subchar[16];   /* Q/S MBCS sub characters           */
       UniChar        subuni[8];     /* Q/S Unicode sub characters        */
   } uconv_attribute_t;

   /*
    * User defined character range
    */
   typedef struct {                /* User Defined character range      */
       unsigned short   first;     /* First codepoint                   */
       unsigned short   last;      /* Last codepoint                    */
   } udcrange_t;
   #endif  /* ULS_ATTR */
   #define uconv_error_t int

   /*
    * UniCreateUconvObject: Create Unicode Conversion Object
    *
    * return code:
    *    0             - Conversion Object sucessfully initialized
    *    UCONV_EMFILE  - Maximum file descriptors are opened
    *    UCONV_ENFILE  - Too many files currently open in the system
    *    UCONV_ENOMEM  - Insuffcient memory
    *    UCONV_EINVAL  - The conversion specified by code_set or the
    *                    modifier supplied is not recognized by the
    *                    implementation.
    *
    */
   int CALLCONV UniCreateUconvObject(
             UniChar     * code_set,   /* I  - Unicode name of uconv table */
             UconvObject * uobj  );    /* O  - Uconv object handle         */

   /*
    * UniQueryUconvObject: Query Unicode Coversion Object
    *
    * return code:
    *    0             - Operation successful
    *    UCONV_EBADF   - The conversion object specified is not
    *                    recognized by the implementation.
    */
   int CALLCONV UniQueryUconvObject(
             UconvObject         uobj, /* I  - Uconv object handle         */
             uconv_attribute_t * attr, /* O  - Uconv attributes            */
             size_t              size, /* I  - Size of attribute structure */
             char          first[256], /* O  - First byte of multibyte     */
             char          other[256], /* O  - Other byte of multibyte     */
             udcrange_t udcrange[32]); /* O  - User defined char range     */

   /*
    * UniSetUconvObject:  Set Unicode Conversion Object
    *
    * return code:
    *    0             - Operation successful
    *    UCONV_EBADF   - The conversion object specified is not
    *                    recognized by the implementation.
    *    UCONV_BADATTR - attribute supplied contains invalid attribute
    *                    for the conversion object
    */
   int CALLCONV UniSetUconvObject(
             UconvObject         uobj, /* I  - Uconv object handle         */
             uconv_attribute_t * attr); /* I  - Uconv attributes            */

   /*
    * UniUconvToUcs:  Convert string to unicode
    *
    * return code:
    *    0             - Operation successful
    *    UCONV_EBADF   - The conversion object specified is not
    *                    recognized by the implementation.
    *    UCONV_E2BIG   - Input conversion stopped due to lack of space
    *                    in the output buffer
    *    UCONV_EINVAL  - Input conversion stopped due to incomplete
    *                    character or shift sequence at the end of the
    *                    input buffer.
    *    UCONV_EILSEQ  - Input conversion stopped due to an input byte
    *                    that does not belong to the input code set.
    */
   int CALLCONV UniUconvToUcs(
             UconvObject uobj,         /* I  - Uconv object handle         */
             void    * * inbuf,        /* IO - Input buffer                */
             size_t    * inbytes,      /* IO - Input buffer size (bytes)   */
             UniChar * * outbuf,       /* IO - Output buffer size          */
             size_t    * outchars,     /* IO - Output size (chars)         */
             size_t    * subst  );     /* IO - Substitution count          */

   /*
    * UniUconvFromUcs:  Convert string from unicode
    *    0             - Operation successful
    *    UCONV_EBADF   - The conversion object specified is not
    *                    recognized by the implementation.
    *    UCONV_E2BIG   - Input conversion stopped due to lack of space
    *                    in the output buffer
    *    UCONV_EINVAL  - Input conversion stopped due to incomplete
    *                    character or shift sequence at the end of the
    *                    input buffer.
    *    UCONV_EILSEQ  - Input conversion stopped due to an input byte
    *                    that does not belong to the input code set.
    */
   int CALLCONV UniUconvFromUcs(
             UconvObject uobj,         /* I  - Uconv object handle         */
             UniChar * * inbuf,        /* IO - Input buffer                */
             size_t    * inchars,      /* IO - Input buffer size (bytes)   */
             void    * * outbuf,       /* IO - Output buffer size          */
             size_t    * outbytes,     /* IO - Output size (chars)         */
             size_t    * subst   );    /* IO - Substitution count          */

   /*
    * UniFreeUconvObject: Close Unicode Conversion Object
    *    0             - Operation successful
    *    UCONV_EBADF   - The conversion object specified is not
    *                    recognized by the implementation.
    */
   int CALLCONV UniFreeUconvObject(
             UconvObject   uobj   );   /* I  - Uconv object handle         */

   /*
    * UniMapCpToUcsCp: Convert a codepage number to unicode name
    *
    * Returns:
    *     0             Goodf there is an error, the contents
    *     UCONV_E2BIG   The buffer is not large enough
    *
    * Notes:
    *     Output is undefined if there is an error.
    *
    *     Output buffer must be large enough to hold terminating null.
    */
   int CALLCONV UniMapCpToUcsCp(
             unsigned long ulCodePage, /* I  - Codepage to convert         */
             UniChar * ucsCodePage,    /* O  - Output buffer               */
             size_t    n );            /* I  - UniChars in output buffer   */

   /*
    * UniStrFromUcs: Convert a null-terminated string from Unicode.
    *
    * Returns:
    *     0               Good
    *     UCONV_EBADF     Invalid Handle
    *     UCONV_EILSEQ    Unmappable char substitution off
    *     UCONV_E2BIG     Max characters converted
    *
    * Note:
    *     Output buffer must be large enough to hold terminating null
    *
    */
    int CALLCONV UniStrFromUcs(
              UconvObject   co,        /* I  - Conversion object           */
              char        * target,    /* O  - Buffer for converted string */
              UniChar     * source,    /* I  - String to convert           */
              int           len);      /* I  - Bytes in target buffer      */

   /*
    * UniStrToUcs: Convert a null-terminated string to Unicode.
    *
    * Returns:
    *     0               Good
    *     UCONV_EBADF     Invalid Handle
    *     UCONV_EILSEQ    Unmappable char substitution off
    *     UCONV_E2BIG     Max characters converted
    *
    * Note:
    *     Output buffer must be large enough to hold terminating null
    */
   int CALLCONV UniStrToUcs(
             UconvObject  co,          /* I  - Conversion object           */
             UniChar    * target,      /* O  - Buffer for converted string */
             char       * source,      /* I  - Character string to convert */
             int          len);        /* I  - UniChars in target buffer   */

 /*
  * for compatibility with old return codes for ULS
  */
  #define UCONV_EOTHER           ULS_OTHER
  #define UCONV_EILSEQ           ULS_ILLEGALSEQUENCE
  #define UCONV_EINVAL           ULS_INVALID
  #define UCONV_E2BIG            ULS_BUFFERFULL
  #define UCONV_ENOMEM           ULS_NOMEMORY
  #define UCONV_EBADF            ULS_BADOBJECT
  #define UCONV_BADATTR          ULS_BADATTR
  #define UCONV_EMFILE           ULS_MAXFILESPERPROC
  #define UCONV_ENFILE           ULS_MAXFILES
  #define UCONV_NOTIMPLEMENTED   ULS_NOTIMPLEMENTED

  #define IBM_437         (UniChar *)L"IBM-437"
  #define IBM_819         (UniChar *)L"IBM-819"
  #define IBM_850         (UniChar *)L"IBM-850"
  #define UTF_8           (UniChar *)L"IBM-1208"
  #define UCS_2           (UniChar *)L"IBM-1200"
  #define ISO8859_1       (UniChar *)L"IBM-819"

  #ifdef __cplusplus
  }
  #endif

#endif
