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
 *  About this file ...  SERVER.H                                   *
 *                                                                  *
 *  This file contains information about the NetServer APIs.        *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *       Data structure templates.                                  *
 *                                                                  *
 *        Definition of special values.                             *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *           file depends on values defined in NETCONS.H.           *
 *                                                                  *
 ********************************************************************/



#ifndef NETSERVER_INCLUDED

#define NETSERVER_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif

/****************************************************************
 *                                                              *
 *            Function prototypes (16-bit or mixed-model)       *
 *                                                              *
 ****************************************************************/
extern API_FUNCTION
  NetServerAdminCommand ( const unsigned char LSFAR * pszServer,
                          const unsigned char LSFAR * pszCommand,
                          short LSFAR *               psResult,
                          unsigned char LSFAR *       pbBuffer,
                          unsigned short              cbBuffer,
                          unsigned short LSFAR *      pcbReturned,
                          unsigned short LSFAR *      pcbTotalAvail );

extern API_FUNCTION
  NetServerNameAdd ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszServerName );

extern API_FUNCTION
  NetServerNameDel ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszServerName );

extern API_FUNCTION
  NetServerNameEnum ( const unsigned char LSFAR * pszServer,
                      short                       sLevel,
                      unsigned char LSFAR *       pbBuffer,
                      unsigned short              cbBuffer,
                      unsigned short LSFAR *      pcEntriesRead,
                      unsigned short LSFAR *      pcTotalAvail );

extern API_FUNCTION
  NetServerDiskEnum ( const unsigned char LSFAR * pszServer,
                      short                       sLevel,
                      unsigned char LSFAR *       pbBuffer,
                      unsigned short              cbBuffer,
                      unsigned short LSFAR *      pcEntriesRead,
                      unsigned short LSFAR *      pcTotalAvail );

extern API_FUNCTION
  NetServerEnum ( const unsigned char LSFAR * pszServer,
                  short                       sLevel,
                  unsigned char LSFAR *       pbBuffer,
                  unsigned short              cbBuffer,
                  unsigned short LSFAR *      pcEntriesRead,
                  unsigned short LSFAR *      pcTotalAvail );

extern API_FUNCTION
  NetServerEnum2 ( const unsigned char LSFAR * pszServer,
                   short                       sLevel,
                   unsigned char LSFAR *       pbBuffer,
                   unsigned short              cbBuffer,
                   unsigned short LSFAR *      pcEntriesRead,
                   unsigned short LSFAR *      pcTotalAvail,
                   unsigned long               flServerType,
                   unsigned char LSFAR *       pszDomain );

extern API_FUNCTION
  NetServerGetInfo ( const unsigned char LSFAR * pszServer,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              cbBuffer,
                     unsigned short LSFAR *      pcbTotalAvail );

extern API_FUNCTION
  NetServerSetInfo ( const unsigned char LSFAR * pszServer,
                     short                       sLevel,
                     const unsigned char LSFAR * pbBuffer,
                     unsigned short              cbBuffer,
                     short                       sParmNum );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/
struct server_info_0 {
    unsigned char   sv0_name[CNLEN + 1];        /* Server name        */
};  /* server_info_0 */

struct server_info_1 {
    unsigned char      sv1_name[CNLEN + 1];
    unsigned char      sv1_version_major;  /* Major version # of net   */
    unsigned char      sv1_version_minor;  /* Minor version # of net   */
    unsigned long      sv1_type;           /* Server type              */
    unsigned char LSFAR * LSPTR sv1_comment;        /* Exported server comment  */
};  /* server_info_1 */

struct server_info_2 {
    unsigned char      sv2_name[CNLEN + 1];
    unsigned char      sv2_version_major;
    unsigned char      sv2_version_minor;
    unsigned long      sv2_type;
    unsigned char LSFAR * LSPTR sv2_comment;
    unsigned long      sv2_ulist_mtime;   /* User list, last modification time    */
    unsigned long      sv2_glist_mtime;   /* Group list, last modification time   */
    unsigned long      sv2_alist_mtime;   /* Access list, last modification time  */
    unsigned short     sv2_users;         /* max number of users allowed          */
    unsigned short     sv2_disc;          /* auto-disconnect timeout(in minutes)  */
    unsigned char LSFAR * LSPTR sv2_alerts;        /* alert names (semicolon separated)    */
    unsigned short     sv2_security;      /* SV_USERSECURITY or SV_SHARESECURITY  */
    unsigned short     sv2_auditing;      /* 0 = no auditing; nonzero = auditing  */

    unsigned short     sv2_numadmin;      /* max number of administrators allowed */
    unsigned short     sv2_lanmask;       /* bit mask representing the srv'd nets */
    unsigned short     sv2_hidden;        /* 0 = visible; nonzero = hidden        */
    unsigned short     sv2_announce;      /* visible server announce rate (sec)   */
    unsigned short     sv2_anndelta;      /* announce randomize interval (sec)    */
                                          /* name of guest account                */
    unsigned char      sv2_guestacct[UNLEN + 1];
    unsigned char      sv2_pad1;          /* Word alignment pad byte             */
    unsigned char LSFAR * LSPTR sv2_userpath;      /* ASCIIZ path to user directories      */
    unsigned short     sv2_chdevs;        /* max # shared character devices       */
    unsigned short     sv2_chdevq;        /* max # character device queues        */
    unsigned short     sv2_chdevjobs;     /* max # character device jobs          */
    unsigned short     sv2_connections;   /* max # of connections             */
    unsigned short     sv2_shares;        /* max # of shares                     */
    unsigned short     sv2_openfiles;     /* max # of open files             */
    unsigned short     sv2_sessopens;     /* max # of open files per session     */
    unsigned short     sv2_sessvcs;       /* max # of virtual circuits per client */
    unsigned short     sv2_sessreqs;      /* max # of simul. reqs. from a client  */
    unsigned short     sv2_opensearch;    /* max # of open searches             */
    unsigned short     sv2_activelocks;   /* max # of active file locks           */
    unsigned short     sv2_numreqbuf;     /* number of server (standard) buffers  */
    unsigned short     sv2_sizreqbuf;     /* size of svr (standard) bufs (bytes)  */
    unsigned short     sv2_numbigbuf;     /* number of big (64K) buffers          */
    unsigned short     sv2_numfiletasks;  /* number of file worker processes      */
    unsigned short     sv2_alertsched;    /* alert counting interval (minutes)    */
    unsigned short     sv2_erroralert;    /* error log alerting threshold         */
    unsigned short     sv2_logonalert;    /* logon violation alerting threshold   */
    unsigned short     sv2_accessalert;   /* access violation alerting threshold  */
    unsigned short     sv2_diskalert;     /* low disk space alert threshold (KB)  */
    unsigned short     sv2_netioalert;    /* net I/O error ratio alert threshold  */
                                          /*  (tenths of a percent)               */
    unsigned short     sv2_maxauditsz;    /* Maximum audit file size (KB)        */
    unsigned char LSFAR * LSPTR sv2_srvheuristics; /* performance related server switches*/
};  /* server_info_2 */


struct server_info_3 {
    unsigned char      sv3_name[CNLEN + 1];
    unsigned char      sv3_version_major;
    unsigned char      sv3_version_minor;
    unsigned long      sv3_type;
    unsigned char LSFAR * LSPTR sv3_comment;
    unsigned long      sv3_ulist_mtime;   /* User list, last modification time    */
    unsigned long      sv3_glist_mtime;   /* Group list, last modification time   */
    unsigned long      sv3_alist_mtime;   /* Access list, last modification time  */
    unsigned short     sv3_users;         /* max number of users allowed      */
    unsigned short     sv3_disc;          /* auto-disconnect timeout(in minutes)  */
    unsigned char LSFAR * LSPTR sv3_alerts;        /* alert names (semicolon separated)    */
    unsigned short     sv3_security;      /* SV_USERSECURITY or SV_SHARESECURITY  */
    unsigned short     sv3_auditing;      /* 0 = no auditing; nonzero = auditing  */

    unsigned short     sv3_numadmin;      /* max number of administrators allowed */
    unsigned short     sv3_lanmask;       /* bit mask representing the srv'd nets */
    unsigned short     sv3_hidden;        /* 0 = visible; nonzero = hidden      */
    unsigned short     sv3_announce;      /* visible server announce rate (sec)   */
    unsigned short     sv3_anndelta;      /* announce randomize interval (sec)    */
                                          /* name of guest account             */
    unsigned char      sv3_guestacct[UNLEN + 1];
    unsigned char      sv3_pad1;          /* Word alignment pad byte             */
    unsigned char LSFAR * LSPTR sv3_userpath;      /* ASCIIZ path to user directories      */
    unsigned short     sv3_chdevs;        /* max # shared character devices      */
    unsigned short     sv3_chdevq;        /* max # character device queues      */
    unsigned short     sv3_chdevjobs;     /* max # character device jobs      */
    unsigned short     sv3_connections;   /* max # of connections             */
    unsigned short     sv3_shares;        /* max # of shares                     */
    unsigned short     sv3_openfiles;     /* max # of open files             */
    unsigned short     sv3_sessopens;     /* max # of open files per session     */
    unsigned short     sv3_sessvcs;       /* max # of virtual circuits per client */
    unsigned short     sv3_sessreqs;      /* max # of simul. reqs. from a client  */
    unsigned short     sv3_opensearch;    /* max # of open searches             */
    unsigned short     sv3_activelocks;   /* max # of active file locks      */
    unsigned short     sv3_numreqbuf;     /* number of server (standard) buffers  */
    unsigned short     sv3_sizreqbuf;     /* size of svr (standard) bufs (bytes)  */
    unsigned short     sv3_numbigbuf;     /* number of big (64K) buffers      */
    unsigned short     sv3_numfiletasks;  /* number of file worker processes      */
    unsigned short     sv3_alertsched;    /* alert counting interval (minutes)    */
    unsigned short     sv3_erroralert;    /* error log alerting threshold      */
    unsigned short     sv3_logonalert;    /* logon violation alerting threshold   */
    unsigned short     sv3_accessalert;   /* access violation alerting threshold  */
    unsigned short     sv3_diskalert;     /* low disk space alert threshold (KB)  */
    unsigned short     sv3_netioalert;    /* net I/O error ratio alert threshold  */
                                          /*  (tenths of a percent)               */
    unsigned short     sv3_maxauditsz;    /* Maximum audit file size (KB)      */
    unsigned char LSFAR * LSPTR sv3_srvheuristics; /* performance related server switches*/
    unsigned long      sv3_auditedevents; /* Audit event control mask      */
    unsigned short     sv3_autoprofile;   /* (0,1,2,3) = (NONE,LOAD,SAVE,or BOTH) */
    unsigned char LSFAR * LSPTR sv3_autopath;      /* file pathname (where to load & save) */
};  /* server_info_3 */


struct server_info_20 {
    unsigned long                si20_reserved;
    unsigned short               si20_count;
};  /* server_info_20 */


struct server_record_id_list {
    unsigned char LSFAR * LSPTR  sril_srv_rec_id;
    unsigned char LSFAR * LSPTR  sril_remark;
};  /* server_record_id_list */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *         Function prototypes (32-bit versions)                *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32ServerAdminCommand ( const unsigned char   * pszServer,
                            const unsigned char   * pszCommand,
                            unsigned long * pulResult,
                            unsigned char * pbBuffer,
                            unsigned long   cbBuffer,
                            unsigned long * pcbReturned,
                            unsigned long * pcbTotalAvail );

extern API32_FUNCTION
  Net32ServerDiskEnum ( const unsigned char   * pszServer,
                        unsigned long   ulLevel,
                        unsigned char * pbBuffer,
                        unsigned long   cbBuffer,
                        unsigned long * pcEntriesRead,
                        unsigned long * pcTotalAvail );

extern API32_FUNCTION
  Net32ServerEnum2 ( const unsigned char     * pszServer,
                     unsigned long     ulLevel,
                     unsigned char   * pbBuffer,
                     unsigned long     cbBuffer,
                     unsigned long   * pcEntriesRead,
                     unsigned long   * pcTotalAvail,
                     unsigned long     flServerType,
                     unsigned char   * pszDomain );

extern API32_FUNCTION
  Net32ServerGetInfo ( const unsigned char    * pszServer,
                       unsigned long    ulLevel,
                       unsigned char  * pbBuffer,
                       unsigned long    cbBuffer,
                       unsigned long  * pcbTotalAvail );

extern API32_FUNCTION
  Net32ServerSetInfo ( const unsigned char    * pszServer,
                       unsigned long    ulLevel,
                       const unsigned char    * pbBuffer,
                       unsigned long    cbBuffer,
                       unsigned long    ulParmNum );

extern API32_FUNCTION
  Net32ServerNameAdd ( const unsigned char * pszServer,
                       const unsigned char * pszServerName );

extern API32_FUNCTION
  Net32ServerNameDel ( const unsigned char * pszServer,
                       const unsigned char * pszServerName );

extern API32_FUNCTION
  Net32ServerNameEnum ( const unsigned char * pszServer,
                        unsigned long         ulLevel,
                        unsigned char *       pbBuffer,
                        unsigned long         cbBuffer,
                        unsigned long *       pcEntriesRead,
                        unsigned long *       pcTotalAvail );

#endif /* PURE_32 */



/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/

/*
 *      Mask to be applied to svX_version_major in order to obtain
 *      the major version number.
 */

#define MAJOR_VERSION_MASK      0x0F

/*
 *      Bit-mapped values for svX_type fields. X = 1, 2 or 3.
 */

#define SV_TYPE_WORKSTATION     0x00000001
#define SV_TYPE_SERVER          0x00000002
#define SV_TYPE_SQLSERVER       0x00000004
#define SV_TYPE_DOMAIN_CTRL     0x00000008
#define SV_TYPE_DOMAIN_BAKCTRL  0x00000010
#define SV_TYPE_TIME_SOURCE     0x00000020
#define SV_TYPE_AFP             0x00000040
#define SV_TYPE_NOVELL          0x00000080
#define SV_TYPE_DOMAIN_MEMBER   0x00000100              /* Additional server */
#define SV_TYPE_PRINTQ_SERVER   0x00000200
#define SV_TYPE_DIALIN_SERVER   0x00000400
#define SV_TYPE_SERVER_UNIX     0x00000800
#define SV_TYPE_NT              0x00001000
#define SV_TYPE_WFW             0x00002000      /* Win for Wkgrp server */
#define SV_TYPE_OK_BACKUP       0x00010000      /* Ok to become a backup i.e. potential backup browser */
#define SV_TYPE_BACKUP          0x00020000      /* backup browse server */
#define SV_TYPE_MASTER          0x00040000      /* master browse server */
#define SV_TYPE_DOMAIN_MASTER   0x00080000
#define SV_TYPE_SERVER_OSF      0x00100000
#define SV_TYPE_SERVER_VMS      0x00200000
#define SV_TYPE_WINDOWS         0x00400000      /* operating systems of Windows95 or above */
#define SV_TYPE_DCE             0x10000000      /* Server is running IBM DSS or equivalent */
#define SV_TYPE_ALTERNATE_XPORT 0x20000000      /* return information about additional transport stack */
#define SV_TYPE_LOCAL_LIST_ONLY 0x40000000      /* return information only from local list */

#define SV_TYPE_WORKGROUP       0x80000000   /* also used for domain enumeration */
#define SV_TYPE_BROWSER (SV_TYPE_OK_BACKUP|SV_TYPE_BACKUP|SV_TYPE_MASTER)
#define SV_TYPE_ALL             0xFFFFFFFF   /* handy for NetServerEnum2 */

/*
 *      Special value for svX_disc that specifies infinite disconnect
 *      time. X = 2 or 3.
 */

#define SV_NODISC               0xFFFF  /* No autodisconnect timeout enforced */

/*
 *      Values of svX_security field. X = 2 or 3.
 */

#define SV_USERSECURITY         1
#define SV_SHARESECURITY        0

/*
 *      Values of svX_hidden field. X = 2 or 3.
 */

#define SV_HIDDEN               1
#define SV_VISIBLE              0

/*
 *      Values for parmnum parameter to NetServerSetInfo.
 */

#define SV_COMMENT_PARMNUM      5
#define SV_DISC_PARMNUM         10
#define SV_ALERTS_PARMNUM       11
#define SV_HIDDEN_PARMNUM       16
#define SV_ANNOUNCE_PARMNUM     17
#define SV_ANNDELTA_PARMNUM     18
#define SV_ALERTSCHED_PARMNUM   37
#define SV_ERRORALERT_PARMNUM   38
#define SV_LOGONALERT_PARMNUM   39
#define SV_ACCESSALERT_PARMNUM  40
#define SV_DISKALERT_PARMNUM    41
#define SV_NETIOALERT_PARMNUM   42
#define SV_MAXAUDITSZ_PARMNUM   43

#define SVI1_NUM_ELEMENTS       5
#define SVI2_NUM_ELEMENTS       44
#define SVI3_NUM_ELEMENTS       45

/*
 *      Maxmimum length for command string to NetServerAdminCommand.
 */

#define SV_MAX_CMD_LEN          PATHLEN


/*
 *      Maxmimum # of server names supported by NetServerNameXXX API.
 */

#define SV_MAX_SRV_NAMES         8

/*
 *      Masks describing AUTOPROFILE parameters
 */

#define SW_AUTOPROF_LOAD_MASK   0x1
#define SW_AUTOPROF_SAVE_MASK   0x2


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETSERVER_INCLUDED */
