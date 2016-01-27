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
 *  About this file ...  DCDB.H                                     *
 *                                                                  *
 *  This file contains information about the DCDB (Domain Control   *
 *  Data Base) APIs. For Each API class there is a section on:      *
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
 ********************************************************************/

#if !defined NETDCDB_INCLUDED
#define NETDCDB_INCLUDED
#pragma pack(1)

/****************************************************************
 *                                                              *
 *              Function prototypes - NetAliasXXX               *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
 NetAliasAdd (const unsigned char LSFAR *,   /* server                    */
              short,                         /* level (2)                 */
              unsigned char LSFAR *,         /* buffer (send)             */
              unsigned short);               /* buffer length             */

extern API_FUNCTION
 NetAliasDel (const unsigned char LSFAR *,     /* server                    */
              unsigned char LSFAR *,           /* alias                     */
              unsigned long);                  /* reserved                  */

extern API_FUNCTION
 NetAliasGetInfo (const unsigned char LSFAR *, /* server                    */
                  unsigned char LSFAR *,       /* alias                     */
                  short,                       /* level (0, 1, 2)           */
                  unsigned char LSFAR *,       /* buffer (receive)          */
                  unsigned short,              /* buffer length             */
                  unsigned short LSFAR *);     /* bytes available           */

extern API_FUNCTION
 NetAliasSetInfo (const unsigned char LSFAR *, /* server                    */
                  unsigned char LSFAR *,       /* alias                     */
                  short,                       /* level (1, 2)              */
                  unsigned char LSFAR *,       /* buffer (send)             */
                  unsigned short,              /* buffer length             */
                  unsigned short);             /* parameter number          */

extern API_FUNCTION
 NetAliasEnum (const unsigned char LSFAR *,  /* server                    */
               short,                        /* level (0, 1, 2)           */
               unsigned short,               /* type                      */
               unsigned char LSFAR *,        /* buffer (receive)          */
               unsigned short,               /* buffer length             */
               unsigned short LSFAR *,       /* entries returned          */
               unsigned short LSFAR *);      /* entries available         */


/****************************************************************
 *                                                              *
 *              Data structure templates - Alias                *
 *                                                              *
 ****************************************************************/

struct alias_info_0 {
   unsigned char                ai0_alias[ALIAS_LEN + 1];
};

struct alias_info_1 {
   unsigned char                ai1_alias[ALIAS_LEN + 1];
   unsigned char                ai1_pad;
   unsigned char LSFAR * LSPTR  ai1_remark;
   unsigned short               ai1_type;
};

struct alias_info_2 {
   unsigned char                ai2_alias[ALIAS_LEN + 1];
   unsigned char                ai2_pad_1;
   unsigned char LSFAR * LSPTR  ai2_remark;
   unsigned short               ai2_type;
   unsigned short               ai2_location;
   unsigned char                ai2_server[CNLEN + 1];
   unsigned short               ai2_mode;
   unsigned short               ai2_maxuses;
   unsigned char                ai2_netname[NNLEN + 1]; /* file or print    */
   unsigned char                ai2_pad_2;
   unsigned char                ai2_queue[QNLEN + 1];   /* print or serial    */
   unsigned char                ai2_pad_3;
   unsigned char LSFAR * LSPTR  ai2_path;               /* file only          */
   unsigned short               ai2_priority;           /* serial only        */
   unsigned char LSFAR * LSPTR  ai2_device_pool;        /* serial only        */
};



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - NetAliasXXX               *
 *              For "pure" 32-bit code                          *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
 Net32AliasAdd (const unsigned char *,      /* server                    */
                unsigned long,              /* level (2) - was signed short */
                unsigned char *,            /* buffer (send)             */
                unsigned long,              /* buffer length             */
                unsigned char *);           /* reserved                  */

extern API32_FUNCTION
 Net32AliasDel (const unsigned char *,      /* server                    */
                unsigned char *,            /* alias                     */
                unsigned long,              /* reserved                  */
                unsigned char *);           /* reserved                  */

extern API32_FUNCTION
 Net32AliasGetInfo (const unsigned char *,  /* server                    */
                    unsigned char *,        /* alias                     */
                    unsigned long,          /* level (0, 1, 2)--was short */
                    unsigned char *,        /* buffer (receive)          */
                    unsigned long,          /* buffer length             */
                    unsigned long *,        /* bytes available           */
                    unsigned char *);       /* reserved                  */

extern API32_FUNCTION
 Net32AliasSetInfo (const unsigned char *,  /* server                    */
                    unsigned char *,        /* alias                     */
                    unsigned long,          /* level (1, 2)-- was short  */
                    unsigned char *,        /* buffer (send)             */
                    unsigned long,          /* buffer length             */
                    unsigned long,          /* parameter number          */
                    unsigned char *);       /* reserved                  */

extern API32_FUNCTION
 Net32AliasEnum (const unsigned char *,     /* server                    */
                 unsigned long,             /* level (0, 1, 2)--was short */
                 unsigned long,             /* type                      */
                 unsigned char *,           /* buffer (receive)          */
                 unsigned long,             /* buffer length             */
                 unsigned long *,           /* entries returned          */
                 unsigned long *,           /* entries available         */
                 unsigned char *);          /* reserved                  */

extern API32_FUNCTION
 Net32AliasEnum2(
       const unsigned char * pszServer,           /* server             */
       unsigned long         ulLevel,             /* level (0, 1, 2)    */
       unsigned long         ulType,              /* type               */
       unsigned char       * pbBuffer,            /* buffer (receive)   */
       unsigned long         ulBuflen,            /* buffer length      */
       unsigned long       * pulEntriesReturned,  /* entries returned   */
       unsigned long       * pulEntriesAvail,     /* entries available  */
       unsigned char       * pszReserved);        /* reserved           */
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - Alias            *
 *                                                              *
 ****************************************************************/

/*
 * values for ai2_mode
 */

#define ALIAS_MODE_STARTUP             0x0000
#define ALIAS_MODE_BYADMIN             0x0001

/*
 * value for ai2_mode (files alias only)
 */

#define ALIAS_MODE_DYNAMIC             0x0002

/*
 * values for ai2_location
 */

#define ALIAS_LOCATION_INTERNAL        0x0000
#define ALIAS_LOCATION_EXTERNAL_DOS    0x0001
#define ALIAS_LOCATION_EXTERNAL_OS2    0x0002
#define ALIAS_LOCATION_EXTERNAL_BOTH   0x0003

/*
 * values for type
 */

#define ALIAS_TYPE_FILE                0x0001
#define ALIAS_TYPE_PRINTER             0x0002
#define ALIAS_TYPE_SERIAL              0x0004


/*
 * values for alias SetInfo
 */

#define ALIAS_REMARK_PARMNUM        3
#define ALIAS_SERVER_PARMNUM        6
#define ALIAS_MODE_PARMNUM          7
#define ALIAS_MAXUSES_PARMNUM       8
#define ALIAS_PATH_PARMNUM         13
#define ALIAS_PRIORITY_PARMNUM     14
#define ALIAS_DEVICEPOOL_PARMNUM   15

#define AI1_NUM_ELEMENTS            4
#define AI2_NUM_ELEMENTS           15

/****************************************************************
 *                                                              *
 *              Function prototypes - Logon Assignments         *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
 NetUserGetLogonAsn (const unsigned char LSFAR *, /* server                   */
                     unsigned char LSFAR *,       /* user                     */
                     short,                       /* level (1)                */
                     unsigned short,              /* type                     */
                     unsigned char LSFAR *,       /* buffer (receive)         */
                     unsigned short,              /* buffer length            */
                     unsigned short LSFAR *);     /* bytes available          */

extern API_FUNCTION
 NetUserSetLogonAsn(const unsigned char LSFAR *, /* server                    */
                    unsigned char LSFAR *,       /* user                      */
                    short,                       /* level (1)                 */
                    unsigned char LSFAR *,       /* buffer (send)             */
                    unsigned short);             /* buffer length             */

/****************************************************************
 *                                                              *
 *              Data structure templates - Logon Assignments    *
 *                                                              *
 ****************************************************************/

struct logon_asn_info_1
  {
   unsigned long    lai1_reserved;
   unsigned short   lai1_count;                 /* logon_asn_list structures */
  };

struct logon_asn_list
  {
   unsigned char    lal_alias[ALIAS_LEN + 1];
   unsigned char    lal_pad;
   unsigned short   lal_type;
   unsigned char    lal_device[DEVLEN + 1];     /* drive or device name */
  };



#ifdef PURE_32
/***************************************************************************
 *                                                                         *
 *     Function prototypes - Logon Assignments - "pure" 32-bit code        *
 *                                                                         *
 ***************************************************************************/

extern API32_FUNCTION
 Net32UserGetLogonAsn (const unsigned char *, /* server                     */
                       unsigned char *,       /* user                       */
                       unsigned long,         /* level (1); was short       */
                       unsigned long,         /* type; was short            */
                       unsigned char *,       /* buffer (receive)           */
                       unsigned long,         /* buffer length; was short   */
                       unsigned long *,       /* bytes available; was short */
                       unsigned char *);      /* reserved                   */

extern API32_FUNCTION
 Net32UserSetLogonAsn (const unsigned char *, /* server                     */
                       unsigned char *,       /* user                       */
                       unsigned long,         /* level (1); was short       */
                       unsigned char *,       /* buffer (send)              */
                       unsigned long,         /* buffer length; was short   */
                       unsigned char *);      /* reserved                   */
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Function prototypes - Application Selectors     *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
 NetUserGetAppSel(const unsigned char LSFAR *,  /* server                    */
                  unsigned char LSFAR *,        /* user                      */
                  short,                        /* level (1)                 */
                  unsigned short,               /* type                      */
                  unsigned char LSFAR *,        /* buffer (receive)          */
                  unsigned short,               /* buffer length             */
                  unsigned short LSFAR *);      /* bytes available           */

extern API_FUNCTION
 NetUserSetAppSel(const unsigned char LSFAR *,  /* server                    */
                  unsigned char LSFAR *,        /* user                      */
                  short,                        /* level (1)                 */
                  unsigned char LSFAR *,        /* buffer (send)             */
                  unsigned short);              /* buffer length             */

/****************************************************************
 *                                                              *
 *              Data structure templates - Application Selectors*
 *                                                              *
 ****************************************************************/

struct app_sel_info_1
  {
   unsigned long    asi1_reserved;
   unsigned short   asi1_count;                 /* app_sel_list structures */
  };

struct app_sel_list
  {
   unsigned char    asl_appname[APP_LEN + 1];
   unsigned char    asl_pad_1;
   unsigned short   asl_apptype;
   unsigned short   asl_reserved;
  };


extern API_FUNCTION
 NetUserDCDBInit (const unsigned char LSFAR *,  /* server    */
                  unsigned char LSFAR *,        /* user      */
                  unsigned long);               /* reserved  */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *  Function prototypes - Application Selectors, DBCD Init      *
 *             For "pure" 32-bit code                           *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
 Net32UserGetAppSel(const unsigned char *, /* server                        */
                    unsigned char *,       /* user                          */
                    unsigned long,         /* level (1); was short          */
                    unsigned long,         /* type; was short               */
                    unsigned char *,       /* buffer (receive)              */
                    unsigned long,         /* buffer length; was short      */
                    unsigned long *,       /* bytes available; was short    */
                    unsigned char *);      /* reserved                      */

extern API32_FUNCTION
 Net32UserSetAppSel(const unsigned char *, /* server                        */
                    unsigned char *,       /* user                          */
                    unsigned long,         /* level (1); was short          */
                    unsigned char *,       /* buffer (send)                 */
                    unsigned long,         /* buffer length; was short      */
                    unsigned char *);      /* reserved                      */

extern API32_FUNCTION
 Net32UserDCDBInit (const unsigned char *, /* server                        */
                    unsigned char *,       /* user                          */
                    unsigned long,         /* reserved                      */
                    unsigned char *);      /* reserved                      */
#endif /* PURE_32 */

/****************************************************************
 *                                                              *
 *              Function prototypes - Applications              *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
 NetAppAdd (const unsigned char LSFAR *,        /* server                    */
            unsigned char LSFAR *,              /* NULL = DCDB.A or -> user  */
            short,                              /* level (3)                 */
            unsigned char LSFAR *,              /* buffer (send)             */
            unsigned short);                    /* buffer length             */

extern API_FUNCTION
 NetAppDel (const unsigned char LSFAR *,        /* server                    */
            unsigned char LSFAR *,              /* NULL = DCDB.A or -> user  */
            unsigned char LSFAR *,              /* name of app to delete     */
            unsigned long);                     /* reserved                  */

extern API_FUNCTION
 NetAppGetInfo (const unsigned char LSFAR *,    /* server                    */
                unsigned char LSFAR *,          /* NULL = DCDB.A or -> user  */
                unsigned char LSFAR *,          /* app name                  */
                short,                          /* level (0, 1, 2, 3)        */
                unsigned char LSFAR *,          /* buffer (receive)          */
                unsigned short,                 /* buflen                    */
                unsigned short LSFAR *);        /* bytes available           */

extern API_FUNCTION
 NetAppSetInfo (const unsigned char LSFAR *,    /* server                    */
                unsigned char LSFAR *,          /* NULL = DCDB.A or -> user  */
                unsigned char LSFAR *,          /* appname                   */
                short,                          /* level (1, 2, 3)           */
                unsigned char LSFAR *,          /* buffer (send)             */
                unsigned short,                 /* buflen                    */
                unsigned short);                /* parmnum                   */

extern API_FUNCTION
 NetAppEnum (const unsigned char LSFAR *,       /* server                    */
             unsigned char LSFAR *,             /* NULL = DCDB.A or -> user  */
             short,                             /* level (0, 1, 2, 3)        */
             unsigned short,                    /* type                      */
             unsigned char LSFAR *,             /* buffer (receive)          */
             unsigned short,                    /* buflen                    */
             unsigned short LSFAR *,            /* entries returned          */
             unsigned short LSFAR *);           /* entries available         */

/****************************************************************
 *                                                              *
 *            Data structure templates - Applications           *
 *                                                              *
 ****************************************************************/
struct app_info_0 {
   unsigned char        app0_name[APP_LEN + 1];
};

struct app_info_1 {
   unsigned char        app1_name[APP_LEN + 1];
   unsigned char        app1_pad_1;
   unsigned char LSFAR * LSPTR  app1_remark;
};

/*
 *  The level 2 structure allows one to enumerate, get or set information
 *  about an application's fixed data only.
 */

struct app_info_2 {
   unsigned char        app2_name[APP_LEN + 1];
   unsigned char        app2_pad_1;
   unsigned char LSFAR * LSPTR  app2_remark;
   unsigned char LSFAR * LSPTR  app2_command;
   unsigned char LSFAR * LSPTR  app2_command_parms;
   unsigned char        app2_app_alias_or_drv[ALIAS_LEN + 1];
   unsigned char        app2_pad_2;
   unsigned char        app2_app_drive;
   unsigned char LSFAR * LSPTR  app2_app_path_to_dir;
   unsigned char        app2_wrkdir_alias_or_drv[ALIAS_LEN + 1];
   unsigned char        app2_pad_3;
   unsigned char        app2_wrkdir_drive;
   unsigned char LSFAR * LSPTR  app2_wrkdir_path_to_dir;
   unsigned char        app2_prompt;
   unsigned char        app2_interface;
   unsigned char        app2_apptype;
};

struct app_info_3 {
   unsigned char        app3_name[APP_LEN + 1];
   unsigned char        app3_pad_1;
   unsigned char LSFAR * LSPTR  app3_remark;
   unsigned char LSFAR * LSPTR  app3_command;
   unsigned char LSFAR * LSPTR  app3_command_parms;
   unsigned char        app3_app_alias_or_drv[ALIAS_LEN + 1];
   unsigned char        app3_pad_2;
   unsigned char        app3_app_drive;
   unsigned char LSFAR * LSPTR  app3_app_path_to_dir;
   unsigned char        app3_wrkdir_alias_or_drv[ALIAS_LEN + 1];
   unsigned char        app3_pad_3;
   unsigned char        app3_wrkdir_drive;
   unsigned char LSFAR * LSPTR  app3_wrkdir_path_to_dir;
   unsigned char        app3_prompt;
   unsigned char        app3_interface;
   unsigned char        app3_apptype;
   unsigned short       app3_res_count;
};

/*
 * the following structure contains redirections required by the application
 */

struct app_res_list {
   unsigned char  arl_alias[ALIAS_LEN + 1];
   unsigned char  arl_pad_1;
   unsigned char  arl_device[DEVLEN + 1];
};


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - Applications              *
 *                                                              *
 ****************************************************************/
extern API32_FUNCTION
 Net32AppAdd (const unsigned char *,         /* server                    */
              unsigned char *,               /* NULL = DCDB.A or -> user  */
              unsigned long,                 /* level (3); was short      */
              unsigned char *,               /* buffer (send)             */
              unsigned long,                 /* buffer length; was USHORT */
              unsigned char *);              /* reserved                  */

extern API32_FUNCTION
 Net32AppDel (const unsigned char *,         /* server                    */
              unsigned char *,               /* NULL = DCDB.A or -> user  */
              unsigned char *,               /* name of app to delete     */
              unsigned long,                 /* reserved                  */
              unsigned char *);              /* reserved                  */

extern API32_FUNCTION
 Net32AppGetInfo (const unsigned char *,     /* server                    */
                  unsigned char *,           /* NULL = DCDB.A or -> user  */
                  unsigned char *,           /* app name                  */
                  unsigned long,             /* level (0, 1, 2, 3); was short */
                  unsigned char *,           /* buffer (receive)          */
                  unsigned long,             /* buflen; was USHORT        */
                  unsigned long  *,          /* bytes available; was USHORT */
                  unsigned char *);          /* reserved                  */

extern API32_FUNCTION
 Net32AppSetInfo (const unsigned char *,     /* server                    */
                  unsigned char *,           /* NULL = DCDB.A or -> user  */
                  unsigned char *,           /* appname                   */
                  unsigned long,             /* level (1, 2, 3); was short */
                  unsigned char *,           /* buffer (send)             */
                  unsigned long,             /* buflen;  was USHORT       */
                  unsigned long,             /* parmnum; was USHORT       */
                  unsigned char *);          /* reserved                  */

extern API32_FUNCTION
 Net32AppEnum (const unsigned char *,        /* server                    */
               unsigned char *,              /* NULL = DCDB.A or -> user  */
               unsigned long,                /* level (0, 1, 2, 3); was short */
               unsigned long,                /* type; was USHORT          */
               unsigned char *,              /* buffer (receive)          */
               unsigned long,                /* buflen; was USHORT        */
               unsigned long  *,             /* entries returned;  was USHORT */
               unsigned long  *,             /* entries available; was USHORT */
               unsigned char *);             /* reserved                  */
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - Applications     *
 *                                                              *
 ****************************************************************/

/*
 * the definitions below are used in various app_info_x.appx_flags elements
 */

/*
 * appx.apptype:  application types  (also masks for NetAppEnum)
 */

#define APP_DOS_PUBLIC   0x01
#define APP_OS2_PUBLIC   0x02
#define APP_OS2_PRIVATE  0x04
#define APP_ALL          0x07
#define APP_DOS_PRIVATE  0x08
#define APP_ALL2         0x0F
#define APP_WORKSPACE_ON_DEMAND 0x20
#define APP_TYPE_UNKNOWN 0x40

/*
 * appx_flags.interface:  application interface - OS/2 apps only
 */

#define APP_PM           0x1
#define APP_PROT         0x2
#define APP_VIO          0x4

/*
 * values for application SetInfo
 */

#define APP_REMARK_PARMNUM     3
#define APP_CMDLINE_PARMNUM    4
#define APP_CMDPARMS_PARMNUM   5

#define APP1_NUM_ELEMENTS    3
#define APP2_NUM_ELEMENTS   16

/*
 * The following characters are NOT allowed in application names
 */
#define INVALID_APPNAME_CHARS   " .\"/\\[]:|<>+=;,?*"

/*
 * The following character is NOT allowed in a remark
 */
#define INVALID_REMARK_CHAR  "\\"

#pragma pack ()
#endif /* NETDCDB_INCLUDED */
