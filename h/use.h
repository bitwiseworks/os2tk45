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
 *  About this file ...  USE.H                                      *
 *                                                                  *
 *  This file contains information about the NetUse APIs.           *
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
 ********************************************************************/


#if !defined(OS2_INCLUDED) && !defined(NETCONS_INCLUDED)
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#endif


#ifndef NETUSE_INCLUDED

#define NETUSE_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetUseAdd ( const unsigned char LSFAR * pszServer,
              short              sLevel,
              const unsigned char LSFAR * pbBuffer,
              unsigned short     cbBuffer );

extern API_FUNCTION
  NetUseDel ( const unsigned char LSFAR * pszServer,
              const unsigned char LSFAR * pszDeviceName,
              unsigned short     usForce );

extern API_FUNCTION
  NetUseEnum ( const unsigned char LSFAR *     pszServer,
               short                  sLevel,
               unsigned char LSFAR *           pbBuffer,
               unsigned short         cbBuffer,
               unsigned short LSFAR * pcEntriesRead,
               unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetUseGetInfo ( const unsigned char LSFAR *     pszServer,
                  const unsigned char LSFAR *     pszUseName,
                  short                  sLevel,
                  unsigned char LSFAR *           pbBuffer,
                  unsigned short         cbBuffer,
                  unsigned short LSFAR * pcbTotalAvail );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/


struct use_info_0 {
    unsigned char      ui0_local[DEVLEN+1];
    unsigned char      ui0_pad_1;
    unsigned char LSFAR * LSPTR ui0_remote;
};  /* use_info_0 */

struct use_info_1 {
    unsigned char      ui1_local[DEVLEN+1];
    unsigned char      ui1_pad_1;
    unsigned char LSFAR * LSPTR ui1_remote;
    unsigned char LSFAR * LSPTR ui1_password;
    unsigned short     ui1_status;
    short              ui1_asg_type;
    unsigned short     ui1_refcount;
    unsigned short     ui1_usecount;
};  /* use_info_1 */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Net32UseAdd ( const unsigned char    * pszServer,
              unsigned long    ulLevel,             /* was signed short */
              const unsigned char    * pbBuffer,
              unsigned long    ulBuffer );          /* was USHORT */

extern API32_FUNCTION
Net32UseDel ( const unsigned char    * pszServer,
              const unsigned char    * pszDeviceName,
              unsigned long    ulForce );           /* was USHORT */

extern API32_FUNCTION
Net32UseEnum ( const unsigned char    * pszServer,
               unsigned long    ulLevel,            /* was signed short */
               unsigned char  * pbBuffer,
               unsigned long    ulBuffer,           /* was USHORT */
               unsigned long  * pulEntriesRead,     /* was USHORT */
               unsigned long  * pulTotalAvail );    /* was USHORT */

extern API32_FUNCTION
Net32UseGetInfo ( const unsigned char    * pszServer,
                  const unsigned char    * pszUseName,
                  unsigned long    ulLevel,         /* was signed short */
                  unsigned char  * pbBuffer,
                  unsigned long    ulBuffer,        /* was USHORT */
                  unsigned long  * pulTotalAvail ); /* was USHORT */
#endif /* PURE_32 */



/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/


/*
 *      Definitions for NetUseDel's last parameter
 */

#define USE_NOFORCE             0
#define USE_FORCE               1
#define USE_LOTS_OF_FORCE       2


/*
 *      Values appearing in the ui1_status field of use_info_1 structure.
 *      Note that USE_SESSLOST and USE_DISCONN are synonyms.
 */

#define USE_OK                  0
#define USE_PAUSED              1
#define USE_SESSLOST            2
#define USE_DISCONN             2
#define USE_NETERR              3
#define USE_CONN                4
#define USE_RECONN              5


/*
 *      Values of the ui1_asg_type field of use_info_1 structure
 */

#define USE_WILDCARD            -1
#define USE_DISKDEV             0
#define USE_SPOOLDEV            1
#define USE_CHARDEV             2
#define USE_IPC                 3
/* @f04 begin */
#define USE_PERMCONN_FLAG      0x1000
/* @f04 end */



#ifdef ANY_32_BIT
#pragma pack()
#endif
#endif /* NETUSE_INCLUDED */
