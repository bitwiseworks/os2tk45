/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  AppParm.h                                  *
 *                                                                  *
 *  This file contains information about the Net32AppParmXXX an     *
 *  UniNet32AppParmXXX APIs                                         *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H.             *
 *                                                                  *
 *******************************************************************/

/*******************************************************************************
 *                                                                             *
 *              Function prototypes - Net32AppParmXXX                          *
 *                                                                             *
 ******************************************************************************/

#include <unidef.h>

extern ULONG APIENTRY
 UniNet32AppParmAdd  (const UCHAR *, /* points to servername string */
                  ULONG *,           /* points to RC modifier       */
                  UniChar *,         /* point to appname string     */
                                     /* previously defined in DCDB  */
                  ULONG,             /* level (0, 1)                */
                  UniChar *,         /* point to buffer data struc. */
                  ULONG,             /* buflen in bytes             */
                  ULONG ,            /* # of entries in buf         */
                  UniChar *);        /* reserved                    */

extern ULONG APIENTRY
 UniNet32AppParmDel (const UCHAR  *, /* points to servername string */
                  ULONG *,           /* points to RC modifier       */
                  UniChar *,         /* point to appname string     */
                                     /* previously defined in DCDB  */
                  ULONG ,            /* level (0, 1)                */
                  ULONG,             /* AppParmDel flags            */ /* F07 */
                  UniChar *,         /* point to buffer data struc. */
                  ULONG ,            /* buflen in bytes             */
                  ULONG ,            /* # of entries in buf         */
                                     /* if 0 then delete all parms  */
                  UniChar *);        /* reserved                    */

/* AppParmDel flags - Bit definitions           */                     /* F07 */

/*             DEL request will delete any User-level .INI files that are F07 */
/*                 related to the application that is being deleted       F07 */
/*                 (thereby removing the "User-Overrides" for that appl)  F07 */
#define APSFREMOVEUSEROVERRIDES  0x00000001

/*             NOTE THAT ALL OTHER BITS MUST BE SET TO ZERO - OTHERWISE   F07 */
/*             A FATAL RC WILL BE RETURNED!                               F07 */
#define APSFRESERVEDDEL          0xFFFFFFFE



extern ULONG APIENTRY
 UniNet32AppParmGet  (const UCHAR *, /* points to servername string */
                  ULONG *,           /* points to RC modifier       */
                  UniChar *,         /* point to appname string     */
                                     /* previously defined in DCDB  */
                  UniChar *,         /* points to userid string,    */
                                     /* previously defined in UAS   */
                  ULONG,             /* level (0,1)                 */
                  ULONG,             /* AppParmGet flags            */ /* F07 */
                  UniChar *,         /* point to buffer data struc. */
                  ULONG,             /* buflen in bytes             */
                  ULONG *,           /* points to # of entries in   */
                                     /* buf returned                */
                  ULONG *,           /* points to # of entries      */
                                     /* available.                  */
                  ULONG *,           /* points to # of bytes        */
                                     /* currently available.        */
                  UniChar *);        /* reserved                    */

/* AppParmGet flags - Bit definitions           */                     /* F07 */
/*   THESE BIT SETTINGS MUST MATCH THE RELATED  */                     /* F07 */
/*   BIT VALUES IN THE: apsX_flags DEFINITION   */                     /* F07 */

/*             GET will return only invocation parameter field types      F07 */
#define        APSFINVOCPARMONLY 0x00000100

/*             GET will return only OS/2 .INI style field types           F07 */
#define        APSFOS2INIONLY    0x00000200

/*             GET will return only environment variable field types      F07 */
#define        APSFENVVARONLY    0x00000400

/*             GET will return only Windows .INI style field types        F07 */
#define        APSFWININIONLY    0x00000800

/*             GET will return ALL field types                            F07 */
#define        APSFALLFIELDS    (APSFINVOCPARMONLY+APSFOS2INIONLY+APSFENVVARONLY+APSFWININIONLY)

/*             NOTE THAT ALL OTHER BITS MUST BE SET TO ZERO - OTHERWISE   F07 */
/*             A FATAL RC WILL BE RETURNED!                               F07 */
#define        APSFRESERVEDGET   0xFFFFF0FF

extern ULONG APIENTRY
 UniNet32AppParmSet  (const UCHAR *, /* points to servername string */
                  ULONG *,           /* points to RC modifier       */
                  UniChar *,         /* point to appname string     */
                                     /* previously defined in DCDB  */
                  UniChar *,         /* points to userid string,    */
                                     /* previously defined in UAS   */
                  ULONG,             /* level (1)                   */
                  UniChar *,         /* point to buffer data struc. */
                  ULONG,             /* buflen in bytes             */
                  ULONG ,            /* # of entries in             */
                                     /* buf sent.                   */
                  UniChar *);        /* reserved                    */

extern ULONG APIENTRY
Net32AppParmAdd(const UCHAR *, ULONG *, UCHAR *, ULONG, UCHAR *,
                ULONG, ULONG, UCHAR * );

extern ULONG APIENTRY
Net32AppParmDel(const UCHAR *, ULONG *, UCHAR *, ULONG, ULONG, UCHAR *,
                ULONG, ULONG, UCHAR * );                               /* F07 */

extern ULONG APIENTRY
Net32AppParmGet(const UCHAR *, ULONG *, UCHAR *, UCHAR *, ULONG, ULONG,
                UCHAR *, ULONG, ULONG *, ULONG *, ULONG *, UCHAR * );  /* F07 */

extern ULONG APIENTRY
Net32AppParmSet(const UCHAR *, ULONG *, UCHAR *, UCHAR *, ULONG, UCHAR *,
                ULONG, ULONG , UCHAR * );

typedef ULONG (* APIENTRY pfnUniNet32AppParmAdd)(const UCHAR *, ULONG *, UniChar *, ULONG, UniChar *,
                 ULONG, ULONG , UniChar *);

typedef ULONG (* APIENTRY pfnUniNet32AppParmDel)(const UCHAR *, ULONG *,
                UniChar *, ULONG, ULONG, UniChar *, ULONG, ULONG, UniChar *); /* F07 */

typedef ULONG (* APIENTRY pfnUniNet32AppParmGet)(const UCHAR *, ULONG *,
                  UniChar *, UniChar *, ULONG, ULONG, UniChar *, ULONG,
                  ULONG *, ULONG *, ULONG *, UniChar *);               /* F07 */

typedef ULONG (* APIENTRY pfnUniNet32AppParmSet)(const UCHAR *, ULONG *, UniChar *, UniChar *, ULONG,
                 UniChar *, ULONG, ULONG , UniChar *);

typedef ULONG (* APIENTRY pfnNet32AppParmAdd)(const UCHAR *, ULONG *, UCHAR *, ULONG, UCHAR *,
               ULONG, ULONG, UCHAR * );

typedef ULONG (* APIENTRY pfnNet32AppParmDel)(const UCHAR *, ULONG *,
               UCHAR *, ULONG, ULONG, UCHAR *, ULONG, ULONG, UCHAR * ); /* F07 */

typedef ULONG (* APIENTRY pfnNet32AppParmGet)(const UCHAR *, ULONG *,
               UCHAR *, UCHAR *, ULONG, ULONG, UCHAR *,
               ULONG, ULONG *, ULONG *, ULONG *, UCHAR * );           /* F07 */

typedef ULONG (* APIENTRY pfnNet32AppParmSet)(const UCHAR *, ULONG *, UCHAR *, UCHAR *, ULONG, UCHAR *,
               ULONG, ULONG , UCHAR * );


/******************************************************************************/
/*                                                                            */
/*          Net32AppParm API data structures (for Levels 0 & 1)           F07 */
/*                                                                            */
/******************************************************************************/
/*
 * Unicode version
 */
struct app_parm_info_0 {
/*             Combining the 3 fields below, uniquely identifies parms for F07 */
/*             .INI files (first 2 fields need to be NULL for env vars). F07 */

               UniChar * aps0_ini_file_name;  /* Only used w/.INI files  F07 */
               UniChar * aps0_ini_app_name;   /* Only used w/.INI files  F07 */
               UniChar * aps0_parm_name;      /* parm name                   */
               ULONG     aps0_flags;          /* parm flags                  */
};

struct app_parm_info_1 {

/*             Combining the 3 fields below, uniquely identifies parms for F07 */
/*             .INI files (first 2 fields need to be NULL for env vars). F07 */

               UniChar * aps1_ini_file_name;  /* Only used w/.INI files  F07 */
               UniChar * aps1_ini_app_name;   /* Only used w/.INI files  F07 */
               UniChar * aps1_parm_name;      /* a.k.a. ini_key_name     F07 */

               ULONG     aps1_flags;          /* parm flags                  */
               UniChar * aps1_parm_value;     /* ptr to parm value           */
               ULONG     aps1_parm_length;    /* data length (only used &    */
                                              /* req'd for binary data)  F07 */
               ULONG     aps1_reserved;       /* reserved - Must be 0    F07 */
};

/*
 * ascii version
 */
struct app_parm_info_a0 {
/*             Combining the 3 fields below, uniquely identifies parms for F07 */
/*             .INI files (first 2 fields need to be NULL for env vars). F07 */

                 UCHAR *  aps0_ini_file_aname;  /* Only used w/INI files F07*/
                 UCHAR *  aps0_ini_app_aname;   /* Only used w/INI files F07*/
                 UCHAR *  aps0_parm_aname;      /* parm name               */
                 ULONG    aps0_aflags;          /* parm flags              */
};

struct app_parm_info_a1 {
/*             Combining the 3 fields below, uniquely identifies parms for F07 */
/*             .INI files (first 2 fields need to be NULL for env vars). F07 */

                 UCHAR *  aps1_ini_file_aname;  /* Only used w/INI files F07*/
                 UCHAR *  aps1_ini_app_aname;   /* Only used w/INI files F07*/
                 UCHAR *  aps1_parm_aname;      /* a.k.a. ini_key_name   F07*/

                 ULONG    aps1_aflags;          /* parm flags               */
                 UCHAR *  aps1_parm_avalue;     /* parm value               */
                 ULONG    aps1_parm_alength;    /* data length (only used & */
                                                /* req'd for binary data)F07*/
                 ULONG    aps1_areserved;       /* reserved - Must be 0  F07*/
};


/***************************************************************************
 *                                                                         *
 *              AppParm Flag definitions for Net32AppParm APIs          F07*
 *                                                                         *
 * Notes:                                                                  *
 *       These flags pertain only to the specific parmname/parmvalue    F07*
 *         pair that are are defined with.                              F07*
 *  ***  All bits other than those discussed below, MUST be set to zero F07*
 *                                                                         *
 * APSFREQUIRED                                                            *
 * APSFUSEROVER                                                            *
 * APSFDEFAULT                                                             *
 * Notes:                                                                  *
 *       If APSFREQUIRED is SET, then the parm value will be checked for   *
 *          a NULL value (on Net32AppParmGet requests). The return code:   *
 *          ERR_DefinitionIncomplete will be returned, if any of the       *
 *          required parameters have not been supplied.                    *
 *       If APSFUSEROVER is SET, then this parm can (but does not have to  *
 *          be) overridden, on a per-user basis (i.e. the subsequent       *
 *          user setting, will override the previously supplied default).  *
 *       If APSFDEFAULT is SET, then restore the default application-wide  *
 *          parm (i.e. delete the user-specific overriden parm).           *
 *                                                                         *
 *============================                                             *
 * APSFBINNONULL    ====\                                                  *
 * APSFSTRING       ==== >> mutually exclusive                             *
 * APSFINT          ====/                                                  *
 * Notes:                                                                  *
 *       The above flags can not be set by UniNet32AppParmSet.             *
 *       If APSFBINNONUL is SET, then parm is Binary (i.e. it can contain  *
 *          embedded nulls).                                               *
 *       If APSFSTRING is SET, then parm is String (i.e. it's a null-      *
 *          terminated string).                                            *
 *       If APSFINT is SET, then parm is Integer.                          *
 *                                                                         *
 *============================                                             *
 * APSFINVOCPARM    ====\                                                  *
 * APSFINIFIELD     ==== >> mutually exclusive                             *
 * APSFENVRONVAR    ====/                                                  *
 * APSFWININIFIELD  ===/                                               F07 *
 * Notes:                                                                  *
 *       The above flags can not be set by UniNet32AppParmSet.             *
 *       If APFSINVOCPARM is set then concatenate the parameters to        *
 *          the invoked program.                                           *
 *          Ex: Prog. Path=d:\hr\ben                                       *
 *           Command Line= benefit.exe                                     *
 *           Work Dir.=                                                    *
 *           Parameter: USER='empno=53467'                                 *
 *             start benefit 'empno=43467'                                 *
 *                                                                         *
 *       If APSFINIFIELD  is SET, then APSFSTRING or APSFBINNONULL     F07 *
 *          must also be set.                                              *
 *       If APSFENVRONVAR is SET, then APSFSTRING must also be set.        *
 *       If APFSENVRONVAR is SET, then prepend|append|replace the          *
 *          parameter value to the environment variable.                   *
 *       If APSFWININIFIELD is SET, then APSFSTRING must also be set.  F07 *
 *                                                                         *
 *============================                                             *
 * APSFBEGIN     ----\                                                     *
 * APSFEND       ----->> mutually exclusive                                *
 * APSFREPLACE   ----/                                                     *
 * Notes:                                                                  *
 *       The APSFBEGIN, APSFEND, & APSFREPLACE are only valid for environ- *
 *          ment variables (i.e. when APSFENVRONVAR is set).          F07  *
 *       If APSFBEGIN is SET, then prepend the specifed value before the   *
 *          environment variable.                                          *
 *       If APSFEND is SET, then append the specified value to the end of  *
 *          the environment variable.                                      *
 *       If APSFREPLACE is SET, then replace the environment variable with *
 *          the specified value.                                           *
 *                                                                         *
 *        APSFREPLACE is NOT VALID with APSFENVRONVAR when                 *
 *             aps_parm_name is LIBPATH                                    *
 *                                                                         *
 *==============================                                           *
 * All the above flags will be returned in the UniNet32AppParmGet.         *
 * All the above flags except APSFDEFAULT, APSFEND  will be ignored in     *
 * UniNet32AppParmSet.                                                     *
 **************************************************************************/
/*
 * Parm Flags Bit Values
 */

#define        APSFREQUIRED      0x00000001
#define        APSFUSEROVER      0x00000002
#define        APSFDEFAULT       0x00000004
#define        APSFBINNONULL     0x00000010
#define        APSFSTRING        0x00000020
#define        APSFINT           0x00000040
#define        APSFINVOCPARM     0x00000100

/*             Used for OS/2 style .INI files                             F07 */
#define        APSFINIFIELD      0x00000200

#define        APSFENVRONVAR     0x00000400

/*             Used for Windows style .INI files                          F07 */
#define        APSFWININIFIELD   0x00000800

#define        APSFBEGIN         0x00001000
#define        APSFEND           0x00002000
#define        APSFREPLACE       0x00004000

/*             NOTE THAT ALL OTHER BITS MUST BE SET TO ZERO - OTHERWISE   F07 */
/*             A FATAL RC WILL BE RETURNED!                               F07 */
#define        APSFRESERVED      0xFFFF8088

/*
 * Maximum size in bytes of environment parameter in config.sys (OS2 limit)
 * parm name (1KB) = parm value (1KB)
 *
 */
#define        MAX_ENV_LEN       1024

/******************************************************************************/

/* Return Code Modifier (RCM) values                                          */
/******************************************************************************/
#define ERR_LEVEL                        1001
#define ERR_INPUT_BUFF                   1002
#define ERR_INVALID_NUM_OF_ENTRIES       1003
#define ERR_INVALID_REQUEST              1004
#define ERR_INVALID_APPNAME              1005
#define ERR_INVALID_DATATYPE             1006
#define ERR_INVALID_FIELDTYPE            1007
#define ERR_INVALID_PLACEMENT            1008
#define ERR_RESERVED_FLD_NOT_0           1009
#define ERR_CONVERSION_FAILURE           1010
#define ERR_ITEM_NOT_FOUND               1011
#define ERR_SCHEMA_CHANGED               1012
#define ERR_APP_NOT_FOUND                1013
#define ERR_NO_INI_FILE                  1014
#define ERR_INI_FILE_WRITE               1015
#define ERR_INI_ENTRY_EXISTS             1016
#define ERR_INIT_FAILED                  1017
#define ERR_TERM_FAILED                  1018
#define ERR_INI_FILE_OPEN_FAILED         1019
#define ERR_INI_FILE_CLOSE_FAILED        1020
#define ERR_CANNOT_GET_KEYS              1021
#define ERR_REQUIRED_VALUE_MISSING       1022
#define ERR_CANNOT_DELETE_INI_FILE       1023
#define ERR_NOT_USER_OVERRIDABLE         1024
#define ERR_INBUF_TOO_SMALL              1025
#define ERR_GET_LAN_ROOT_FAILURE         1026
#define ERR_NO_USER_DIRECTORY            1027
#define ERR_CANT_CREATE_USER_DIRECTORY   1028
#define ERR_USER_ENTRY_DELETED           1029
#define ERR_INI_NAME_NOT_ALLOWED         1030
#define ERR_USER_INI_NAME_MISSING        1031
#define ERR_STRUCTURE_CORRUPTION         1032

