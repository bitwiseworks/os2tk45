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
 *  About this file ...  WKSTA.H                                    *
 *                                                                  *
 *  This file contains information about the NetWksta APIs.         *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H              *
 *                                                                  *
 ********************************************************************/




#ifndef NETWKSTA_INCLUDED

#define NETWKSTA_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif

/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetWkstaGetInfo ( const unsigned char LSFAR * pszServer,
                    short                       sLevel,
                    unsigned char LSFAR       * pbBuffer,
                    unsigned short              cbBuffer,
                    unsigned short LSFAR      * pcbTotalAvail );

extern API_FUNCTION
  NetWkstaSetInfo ( const unsigned char LSFAR * pszServer,
                    short                       sLevel,
                    unsigned char LSFAR       * pbBuffer,
                    unsigned short              cbBuffer,
                    short                       sParmNum );


extern API_FUNCTION
  NetWkstaSetUID ( const unsigned char LSFAR * pszServer,
                   const unsigned char LSFAR * pszUserName,
                   const unsigned char LSFAR * pszPassword,
                   const unsigned char LSFAR * pszParms,
                   unsigned short              fsLogOff );

extern API_FUNCTION
  NetWkstaSetUID2 ( unsigned char LSFAR *   pszReserved,
                    unsigned char LSFAR *   pszDomain,
                    unsigned char LSFAR *   pszUserName,
                    unsigned char LSFAR *   pszPassword,
                    unsigned char LSFAR *   pszParms,
                    unsigned short          usLogoffForce,
                    short                   sLevel,
                    unsigned char LSFAR *   pbBuffer,
                    unsigned short          cbBuffer,
                    unsigned short LSFAR  * pcbTotalAvail );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/

struct wksta_info_0 {
    unsigned short              wki0_reserved_1;
    unsigned long               wki0_reserved_2;
    unsigned char LSFAR * LSPTR wki0_root;
    unsigned char LSFAR * LSPTR wki0_computername;
    unsigned char LSFAR * LSPTR wki0_username;
    unsigned char LSFAR * LSPTR wki0_langroup;
    unsigned char               wki0_ver_major;
    unsigned char               wki0_ver_minor;
    unsigned long               wki0_reserved_3;
    unsigned short              wki0_charwait;
    unsigned long               wki0_chartime;
    unsigned short              wki0_charcount;
    unsigned short              wki0_reserved_4;
    unsigned short              wki0_reserved_5;
    unsigned short              wki0_keepconn;
    unsigned short              wki0_keepsearch;
    unsigned short              wki0_maxthreads;
    unsigned short              wki0_maxcmds;
    unsigned short              wki0_reserved_6;
    unsigned short              wki0_numworkbuf;
    unsigned short              wki0_sizworkbuf;
    unsigned short              wki0_maxwrkcache;
    unsigned short              wki0_sesstimeout;
    unsigned short              wki0_sizerror;
    unsigned short              wki0_numalerts;
    unsigned short              wki0_numservices;
    unsigned short              wki0_errlogsz;
    unsigned short              wki0_printbuftime;
    unsigned short              wki0_numcharbuf;
    unsigned short              wki0_sizcharbuf;
    unsigned char LSFAR * LSPTR wki0_logon_server;
    unsigned char LSFAR * LSPTR wki0_wrkheuristics;
    unsigned short              wki0_mailslots;
};  /* wksta_info_0 */

struct wksta_info_1 {
    unsigned short              wki1_reserved_1;
    unsigned long               wki1_reserved_2;
    unsigned char LSFAR * LSPTR wki1_root;
    unsigned char LSFAR * LSPTR wki1_computername;
    unsigned char LSFAR * LSPTR wki1_username;
    unsigned char LSFAR * LSPTR wki1_langroup;
    unsigned char               wki1_ver_major;
    unsigned char               wki1_ver_minor;
    unsigned long               wki1_reserved_3;
    unsigned short              wki1_charwait;
    unsigned long               wki1_chartime;
    unsigned short              wki1_charcount;
    unsigned short              wki1_reserved_4;
    unsigned short              wki1_reserved_5;
    unsigned short              wki1_keepconn;
    unsigned short              wki1_keepsearch;
    unsigned short              wki1_maxthreads;
    unsigned short              wki1_maxcmds;
    unsigned short              wki1_reserved_6;
    unsigned short              wki1_numworkbuf;
    unsigned short              wki1_sizworkbuf;
    unsigned short              wki1_maxwrkcache;
    unsigned short              wki1_sesstimeout;
    unsigned short              wki1_sizerror;
    unsigned short              wki1_numalerts;
    unsigned short              wki1_numservices;
    unsigned short              wki1_errlogsz;
    unsigned short              wki1_printbuftime;
    unsigned short              wki1_numcharbuf;
    unsigned short              wki1_sizcharbuf;
    unsigned char LSFAR * LSPTR wki1_logon_server;
    unsigned char LSFAR * LSPTR wki1_wrkheuristics;
    unsigned short              wki1_mailslots;
    unsigned char LSFAR * LSPTR wki1_logon_domain;
    unsigned char LSFAR * LSPTR wki1_oth_domains;
    unsigned short              wki1_numdgrambuf;
};  /* wksta_info_1 */

struct wksta_info_10 {
    unsigned char LSFAR * LSPTR wki10_computername;
    unsigned char LSFAR * LSPTR wki10_username;
    unsigned char LSFAR * LSPTR wki10_langroup;
    unsigned char               wki10_ver_major;
    unsigned char               wki10_ver_minor;
    unsigned char LSFAR * LSPTR wki10_logon_domain;
    unsigned char LSFAR * LSPTR wki10_oth_domains;
};  /* wksta_info_10 */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Net32WkstaGetInfo ( const unsigned char * pszServer,
                    unsigned long         ulLevel,        /* was signed short */
                    unsigned char       * pbBuffer,
                    unsigned long         ulBuffer,       /* was USHORT */
                    unsigned long       * pulTotalAvail); /* was USHORT */

extern API32_FUNCTION
Net32WkstaSetInfo ( const unsigned char * pszServer,
                    unsigned long         ulLevel,      /* was signed short */
                    unsigned char       * pbBuffer,
                    unsigned long         ulBuffer,     /* was USHORT */
                    unsigned long         ulParmNum);   /* was signed short */

extern API32_FUNCTION
Net32WkstaSetUID2 ( unsigned char  * pszReserved,
                    unsigned char  * pszDomain,
                    unsigned char  * pszUserName,
                    unsigned char  * pszPassword,
                    unsigned char  * pszParms,
                    unsigned long    usLogoffForce,  /* was USHORT */
                    unsigned long    ulLevel,        /* was signed short */
                    unsigned char  * pbBuffer,
                    unsigned long    ulBuffer,       /* was USHORT */
                    unsigned long  * pulTotalAvail); /* was USHORT */
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *             Special values and constants                     *
 *                                                              *
 ****************************************************************/


/*
 *       Constants for use as NetWkstaSetInfo parmnum parameter
 */

#define WKSTA_CHARWAIT_PARMNUM          10
#define WKSTA_CHARTIME_PARMNUM          11
#define WKSTA_CHARCOUNT_PARMNUM         12
#define WKSTA_ERRLOGSZ_PARMNUM          27
#define WKSTA_PRINTBUFTIME_PARMNUM      28
#define WKSTA_WRKHEURISTICS_PARMNUM     32
#define WKSTA_OTHDOMAINS_PARMNUM        35

/*
 *      Definitions for NetWkstaSetUID's ucond parameter
 */

#define WKSTA_NOFORCE         0
#define WKSTA_FORCE           1
#define WKSTA_LOTS_OF_FORCE   2
#define WKSTA_MAX_FORCE       3



/*
 *      Maximum number of additional domains
 */
#define MAX_OTH_DOMAINS     4


#ifdef ANY_32_BIT
#pragma pack()
#endif
#endif /* NETWKSTA_INCLUDED */
