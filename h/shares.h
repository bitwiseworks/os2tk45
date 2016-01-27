/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/********************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  SHARES.H                                   *
 *                                                                  *
 *  This file contains information about the NetShare, NetSession,  *
 *  NetFile, and NetConnection APIs.  For each API class there is   *
 *  a section on:                                                   *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H.             *
 *                                                                  *
 *         This file is always included by LAN.H.                   *
 *                                                                  *
 ********************************************************************/



/****************************************************************
 *                                                              *
 *              Share Class                                     *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETSHARE ) || !defined( LAN_INCLUDED )) \
    && !defined( NETSHARE_INCLUDED )

#define NETSHARE_INCLUDED
#ifdef ANY_32_BIT
#pragma pack(1)
#endif

/****************************************************************
 *                                                              *
 *              Function prototypes - SHARE                     *
 *                                                              *
 ****************************************************************/


extern API_FUNCTION
  NetShareAdd ( const unsigned char LSFAR * pszServer,
                short                       sLevel,
                const unsigned char LSFAR * pbBuffer,
                unsigned short              cbBuffer );

extern API_FUNCTION
  NetShareCheck ( const unsigned char LSFAR *     pszServer,
                  const unsigned char LSFAR *     pszDeviceName,
                  unsigned short LSFAR *          pusType );

extern API_FUNCTION
  NetShareDel ( const unsigned char LSFAR * pszServer,
                const unsigned char LSFAR * pszNetName,
                unsigned short              usReserved );

extern API_FUNCTION
  NetShareEnum ( const unsigned char LSFAR *     pszServer,
                 short                           sLevel,
                 unsigned char LSFAR *           pbBuffer,
                 unsigned short                  cbBuffer,
                 unsigned short LSFAR *          pcEntriesRead,
                 unsigned short LSFAR *          pcTotalAvail );

extern API_FUNCTION
  NetShareGetInfo ( const unsigned char LSFAR *     pszServer,
                    const unsigned char LSFAR *     pszNetName,
                    short                           sLevel,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short                  cbBuffer,
                    unsigned short LSFAR *          pcbTotalAvail );

extern API_FUNCTION
  NetShareSetInfo ( const unsigned char LSFAR * pszServer,
                    const unsigned char LSFAR * pszNetName,
                    short                       sLevel,
                    const unsigned char LSFAR * pbBuffer,
                    unsigned short              cbBuffer,
                    short                       sParmNum );


/****************************************************************
 *                                                              *
 *              Data structure templates - SHARE                *
 *                                                              *
 ****************************************************************/

struct share_info_0 {
    unsigned char      shi0_netname[NNLEN+1];
};  /* share_info_0 */

struct share_info_1 {
    unsigned char      shi1_netname[NNLEN+1];
    unsigned char      shi1_pad1;
    unsigned short     shi1_type;
    unsigned char LSFAR * LSPTR shi1_remark;
};  /* share_info_1 */

struct share_info_2 {
    unsigned char      shi2_netname[NNLEN+1];
    unsigned char      shi2_pad1;
    unsigned short     shi2_type;
    unsigned char LSFAR * LSPTR shi2_remark;
    unsigned short     shi2_permissions;
    unsigned short     shi2_max_uses;
    unsigned short     shi2_current_uses;
    unsigned char LSFAR * LSPTR shi2_path;
    unsigned char      shi2_passwd[SHPWLEN+1];
    unsigned char      shi2_pad2;
};  /* share_info_2 */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - SHARE                     *
 *                for "pure" 32-bit code                        *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Net32ShareAdd ( const unsigned char * pszServer,
                unsigned long         ulLevel,           /* was signed short */
                const unsigned char * pbBuffer,
                unsigned long         ulBuffer );        /* was USHORT */

extern API32_FUNCTION
Net32ShareCheck ( const unsigned char * pszServer,
                  const unsigned char * pszDeviceName,
                  unsigned long       * pulType );      /* was USHORT */

extern API32_FUNCTION
Net32ShareDel ( const unsigned char * pszServer,
                const unsigned char * pszNetName,
                unsigned long         ulReserved );     /* was USHORT */

extern API32_FUNCTION
Net32ShareEnum ( const unsigned char     * pszServer,
                 unsigned long     ulLevel,       /* was signed short */
                 unsigned char   * pbBuffer,
                 unsigned long     ulBuffer,       /* was USHORT */
                 unsigned long   * pulEntriesRead, /* was USHORT */
                 unsigned long   * pulTotalAvail );/* was USHORT */

extern API32_FUNCTION
Net32ShareGetInfo ( const unsigned char     * pszServer,
                    const unsigned char     * pszNetName,
                    unsigned long     ulLevel,        /* was signed short */
                    unsigned char   * pbBuffer,
                    unsigned long     ulBuffer,        /* was USHORT */
                    unsigned long   * pulTotalAvail ); /* was USHORT */

extern API32_FUNCTION
Net32ShareSetInfo ( const unsigned char    * pszServer,
                    const unsigned char    * pszNetName,
                    unsigned long    ulLevel,     /* was signed short */
                    const unsigned char    * pbBuffer,
                    unsigned long    ulBuffer,    /* was USHORT */
                    unsigned long    ulParmNum ); /* was signed short */
#endif /* PURE_32 */

/****************************************************************
 *                                                              *
 *          Special values and constants - SHARE                *
 *                                                              *
 ****************************************************************/


/*
 *      Values for parmnum parameter to NetShareSetInfo.
 */

#define SHI_REMARK_PARMNUM              4
#define SHI_PERMISSIONS_PARMNUM         5
#define SHI_MAX_USES_PARMNUM            6
#define SHI_PASSWD_PARMNUM              9

#define SHI1_NUM_ELEMENTS               4
#define SHI2_NUM_ELEMENTS              10


/*
 *      Share types (shi1_type and shi2_type fields).
 */

#define STYPE_DISKTREE                  0
#define STYPE_PRINTQ                    1
#define STYPE_DEVICE                    2
#define STYPE_IPC                       3

#define SHI_USES_UNLIMITED              -1

#endif /* NETSHARE_INCLUDED */


/****************************************************************
 *                                                              *
 *         Session Class                                        *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETSESSION ) || !defined( LAN_INCLUDED )) \
    && !defined( NETSESSION_INCLUDED )

#define NETSESSION_INCLUDED


/****************************************************************
 *                                                              *
 *              Function prototypes - SESSION                   *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetSessionDel ( const unsigned char LSFAR * pszServer,
                  const unsigned char LSFAR * pszClientName,
                  short              sReserved );

extern API_FUNCTION
  NetSessionEnum ( const unsigned char LSFAR *     pszServer,
                   short                  sLevel,
                   unsigned char LSFAR *           pbBuffer,
                   unsigned short         cbBuffer,
                   unsigned short LSFAR * pcEntriesRead,
                   unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetSessionGetInfo ( const unsigned char LSFAR *     pszServer,
                      const unsigned char LSFAR *     pszClientName,
                      short                  sLevel,
                      unsigned char LSFAR *           pbBuffer,
                      unsigned short         cbBuffer,
                      unsigned short LSFAR * pcbTotalAvail );


/****************************************************************
 *                                                              *
 *            Data structure templates - SESSION                *
 *                                                              *
 ****************************************************************/


struct session_info_0 {
    unsigned char LSFAR * LSPTR sesi0_cname;
};  /* session_info_0 */

struct session_info_1 {
    unsigned char LSFAR * LSPTR sesi1_cname;
    unsigned char LSFAR * LSPTR sesi1_username;
    unsigned short     sesi1_num_conns;
    unsigned short     sesi1_num_opens;
    unsigned short     sesi1_num_users;
    unsigned long      sesi1_time;
    unsigned long      sesi1_idle_time;
    unsigned long      sesi1_user_flags;
};  /* session_info_1 */

struct session_info_2 {
    unsigned char LSFAR * LSPTR sesi2_cname;
    unsigned char LSFAR * LSPTR sesi2_username;
    unsigned short     sesi2_num_conns;
    unsigned short     sesi2_num_opens;
    unsigned short     sesi2_num_users;
    unsigned long      sesi2_time;
    unsigned long      sesi2_idle_time;
    unsigned long      sesi2_user_flags;
    unsigned char LSFAR * LSPTR sesi2_cltype_name;
};  /* session_info_2 */

struct session_info_10 {
        unsigned char LSFAR * LSPTR sesi10_cname;
        unsigned char LSFAR * LSPTR sesi10_username;
        unsigned long      sesi10_time;
        unsigned long      sesi10_idle_time;
};  /* session_info_10 */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - SESSION                   *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Net32SessionDel (const unsigned char    * pszServer,
                 const unsigned char    * pszClientName,
                 unsigned long    ulReserved);  /* was signed short */

extern API32_FUNCTION
Net32SessionEnum (const unsigned char    * pszServer,
                  unsigned long    ulLevel,         /* was signed short */
                  unsigned char  * pbBuffer,
                  unsigned long    ulBuffer,        /* was USHORT */
                  unsigned long  * pulEntriesRead,  /* was USHORT */
                  unsigned long  * pulTotalAvail);  /* was USHORT */

extern API32_FUNCTION
Net32SessionGetInfo (const unsigned char    * pszServer,
                     const unsigned char    * pszClientName,
                     unsigned long    ulLevel,        /* was signed short */
                     unsigned char  * pbBuffer,
                     unsigned long    ulBuffer,       /* was USHORT */
                     unsigned long  * pulTotalAvail); /* was USHORT */
#endif /* PURE_32 */



/****************************************************************
 *                                                              *
 *        Special values and constants - SESSION                *
 *                                                              *
 ****************************************************************/

/*
 *      Bits defined in sesi1_user_flags.
 */

#define SESS_GUEST              1       /* session is logged on as a guest */
#define SESS_NOENCRYPTION       2       /* session is not using encryption */


#define SESI1_NUM_ELEMENTS      8
#define SESI2_NUM_ELEMENTS      9

#endif /* NETSESSION_INCLUDED */


/****************************************************************
 *                                                              *
 *              Connection Class                                *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETCONNECTION ) || !defined( LAN_INCLUDED )) \
    && !defined( NETCONNECTION_INCLUDED )

#define NETCONNECTION_INCLUDED


/****************************************************************
 *                                                              *
 *              Function prototypes - CONNECTION                *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetConnectionEnum ( const unsigned char LSFAR *     pszServer,
                      const unsigned char LSFAR *     pszQualifier,
                      short                  sLevel,
                      unsigned char LSFAR *           pbBuffer,
                      unsigned short         cbBuffer,
                      unsigned short LSFAR * pcEntriesRead,
                      unsigned short LSFAR * pcTotalAvail );


/****************************************************************
 *                                                              *
 *              Data structure templates - CONNECTION           *
 *                                                              *
 ****************************************************************/

struct connection_info_0 {
    unsigned short      coni0_id;
};  /* connection_info_0 */

struct connection_info_1 {
    unsigned short     coni1_id;
    unsigned short     coni1_type;
    unsigned short     coni1_num_opens;
    unsigned short     coni1_num_users;
    unsigned long      coni1_time;
    unsigned char LSFAR * LSPTR coni1_username;
    unsigned char LSFAR * LSPTR coni1_netname;
};  /* connection_info_1 */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *       Function prototypes - CONNECTION (32-Bit)              *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32ConnectionEnum ( const unsigned char    * pszServer,
                        const unsigned char    * pszQualifier,
                        unsigned long    ulLevel,         /* was signed short*/
                        unsigned char  * pbBuffer,
                        unsigned long    ulBuffer,        /* was USHORT */
                        unsigned long  * pulEntriesRead,  /* was PUSHORT*/
                        unsigned long  * pulTotalAvail ); /* was PUSHORT*/
#endif /* PURE_32 */


#endif /* NETCONNECTION_INCLUDED */


/****************************************************************
 *                                                              *
 *              File Class                                      *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETFILE ) || !defined( LAN_INCLUDED )) \
    && !defined( NETFILE_INCLUDED )

#define NETFILE_INCLUDED


/****************************************************************
 *                                                              *
 *              Function prototypes - FILE                      *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetFileClose ( const unsigned char LSFAR * pszServer,
                 unsigned short     usFileId );

extern API_FUNCTION
  NetFileClose2 ( const unsigned char LSFAR * pszServer,
                  unsigned long      ulFileId );

extern API_FUNCTION
  NetFileEnum ( const unsigned char LSFAR *     pszServer,
                const unsigned char LSFAR *     pszBasePath,
                short                  sLevel,
                unsigned char LSFAR *           pbBuffer,
                unsigned short         cbBuffer,
                unsigned short LSFAR * pcEntriesRead,
                unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetFileEnum2 ( const unsigned char LSFAR *     pszServer,
                 const unsigned char LSFAR *     pszBasePath,
                 const unsigned char LSFAR *     pszUserName,
                 short                  sLevel,
                 unsigned char LSFAR *           pbBuffer,
                 unsigned short         cbBuffer,
                 unsigned short LSFAR * pcEntriesRead,
                 unsigned short LSFAR * pcEntriesRemaining,
                 void LSFAR *           pResumeKey );

extern API_FUNCTION
  NetFileGetInfo ( const unsigned char LSFAR *     pszServer,
                   unsigned short         usFileId,
                   short                  sLevel,
                   unsigned char LSFAR *           pbBuffer,
                   unsigned short         cbBuffer,
                   unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetFileGetInfo2 ( const unsigned char LSFAR *     pszServer,
                    unsigned long          ulFileId,
                    short                  sLevel,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short         cbBuffer,
                    unsigned short LSFAR * pcbTotalAvail );


/****************************************************************
 *                                                              *
 *              Data structure templates - FILE                 *
 *                                                              *
 ****************************************************************/

struct file_info_0 {
    unsigned short      fi0_id;
};  /* file_info_0 */

struct file_info_1 {
    unsigned short     fi1_id;
    unsigned short     fi1_permissions;
    unsigned short     fi1_num_locks;
    unsigned char LSFAR * LSPTR fi1_pathname;
    unsigned char LSFAR * LSPTR fi1_username;
};  /* file_info_1 */

struct file_info_2 {
    unsigned long       fi2_id;
};  /* file_info_2 */

struct file_info_3 {
    unsigned long      fi3_id;
    unsigned short     fi3_permissions;
    unsigned short     fi3_num_locks;
    unsigned char LSFAR * LSPTR fi3_pathname;
    unsigned char LSFAR * LSPTR fi3_username;
};  /* file_info_3 */


struct res_file_enum_2 {
    unsigned short      res_pad;         /* not used now */
    unsigned short      res_fs;          /* server type */
    unsigned long       res_pro;          /* progressive */
};  /* res_file_enum_2 */

#ifdef  PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - FILE                      *
 *                for "pure" 32-bit code                        *
 *                                                              *
 ****************************************************************/
extern API32_FUNCTION
Net32FileClose2 ( const unsigned char    * pszServer,
                  unsigned long    ulFileId );

extern API32_FUNCTION
Net32FileEnum2 ( const unsigned char    * pszServer,
                 const unsigned char    * pszBasePath,
                 const unsigned char    * pszUserName,
                 unsigned long    ulLevel,             /*was signed short*/
                 unsigned char  * pbBuffer,
                 unsigned long    ulBuffer,            /* was USHORT */
                 unsigned long  * pulEntriesRead,      /* was PUSHORT*/
                 unsigned long  * pulEntriesRemaining, /* was PUSHORT*/
                 void           * pResumeKey );

extern API32_FUNCTION
Net32FileGetInfo2 ( const unsigned char    * pszServer,
                    unsigned long    ulFileId,
                    unsigned long    ulLevel,          /*was signed short*/
                    unsigned char  * pbBuffer,
                    unsigned long    ulBuffer,         /* was USHORT */
                    unsigned long  * pulTotalAvail );  /* was PUSHORT*/
#endif /* PURE_32 */



/****************************************************************
 *                                                              *
 *              Special values and constants - FILE             *
 *                                                              *
 ****************************************************************/

                                        /* bit values for permissions */
#define PERM_FILE_READ          0x1     /* user has read access */
#define PERM_FILE_WRITE         0x2     /* user has write access */
#define PERM_FILE_CREATE        0x4     /* user has create access */


typedef struct res_file_enum_2 FRK;

#define FRK_INIT( f )   \
        {               \
                (f).res_pad = 0L;       \
                (f).res_fs = 0; \
                (f).res_pro = 0;        \
        }


#ifdef ANY_32_BIT
#pragma pack()
#endif
#endif /* NETFILE_INCLUDED */
