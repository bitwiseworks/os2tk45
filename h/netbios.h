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
 *  About this file ...  NETBIOS.H                                  *
 *                                                                  *
 *  This file contains information about the NetBios APIs.          *
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



#ifndef NETBIOS_INCLUDED

#define NETBIOS_INCLUDED

#ifdef  ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *               Function prototypes - NetBios                  *
 *                                                              *
 ****************************************************************/


extern API_FUNCTION
  NetBiosOpen ( unsigned char LSFAR *  pszDevName,
                unsigned char LSFAR *  pszReserved,
                unsigned short         usOpenOpt,
                unsigned short LSFAR * phDevName );

extern API_FUNCTION
  NetBiosClose ( unsigned short hDevName,
                 unsigned short usReserved );

extern API_FUNCTION
  NetBiosEnum ( const unsigned char LSFAR * pszServer,
                short                       sLevel,
                unsigned char LSFAR       * pbBuffer,
                unsigned short              cbBuffer,
                unsigned short LSFAR      * pcEntriesRead,
                unsigned short LSFAR      * pcTotalAvail );

extern API_FUNCTION
  NetBiosGetInfo ( const unsigned char LSFAR *  pszServer,
                   const unsigned char LSFAR *  pszNetBiosName,
                   short                        sLevel,
                   unsigned char LSFAR       *  pbBuffer,
                   unsigned short               cbBuffer,
                   unsigned short LSFAR       * pcbTotalAvail );

extern API_FUNCTION
  NetBiosSubmit ( unsigned short     hDevName,
                  unsigned short     usNcbOpt,
                  struct ncb LSFAR * pNCB );



/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/


struct netbios_info_0 {
    unsigned char  nb0_net_name[NETBIOS_NAME_LEN+1];
};      /* netbios_info_0 */

struct netbios_info_1 {
    unsigned char  nb1_net_name[NETBIOS_NAME_LEN+1];
    unsigned char  nb1_driver_name[DEVLEN+1];/* OS/2 device driver name        */
    unsigned char  nb1_lana_num;             /* LAN adapter number of this net */
    unsigned char  nb1_pad_1;
    unsigned short nb1_driver_type;
    unsigned short nb1_net_status;
    unsigned long  nb1_net_bandwidth;     /* Network bandwidth, bits/second */
    unsigned short nb1_max_sess;          /* Max number of sessions         */
    unsigned short nb1_max_ncbs;          /* Max number of outstanding NCBs */
    unsigned short nb1_max_names;         /* Max number of names            */
};      /* netbios_info_1 */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *               Function prototypes - NetBios                  *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
NetBios32Open ( unsigned char  * pszDevName,
                unsigned char  * pszReserved,
                unsigned long    ulOpenOpt,       /* was USHORT*/
                unsigned long  * phDevName );     /* was PUSHORT*/

extern API32_FUNCTION
NetBios32Close ( unsigned long  hDevName,         /* was USHORT*/
                 unsigned long  ulReserved );     /* was USHORT*/

extern API32_FUNCTION
NetBios32Enum ( const unsigned char    * pszServer,
                unsigned long    ulLevel,          /* was signed short*/
                unsigned char  * pbBuffer,
                unsigned long    ulBuffer,         /* was USHORT*/
                unsigned long  * pulEntriesRead,   /* was PUSHORT*/
                unsigned long  * pulTotalAvail );  /* was PUSHORT*/

extern API32_FUNCTION
NetBios32GetInfo ( const unsigned char    * pszServer,
                   const unsigned char    * pszNetBiosName,
                   unsigned long    ulLevel,          /* was signed short*/
                   unsigned char  * pbBuffer,
                   unsigned long    ulBuffer,         /* was USHORT*/
                   unsigned long  * pulTotalAvail );  /* was PUSHORT*/

extern API32_FUNCTION
NetBios32Submit ( unsigned long    hDevName,          /* was USHORT*/
                  unsigned long    ulNcbOpt,          /* was USHORT*/
                  struct ncb     * pNCB );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/


/*
 *      Driver types (nb1_driver_type).
 */

#define NB_TYPE_NCB     1
#define NB_TYPE_MCB     2

/*
 *      Bits defined in nb1_net_status.
 */

#define NB_LAN_FLAGS_MASK       0x3FFF  /* Mask for LAN Flags */
#define NB_LAN_MANAGED          0x0001  /* LAN is managed by redirector */
#define NB_LAN_LOOPBACK         0x0002  /* LAN is a loopback driver */
#define NB_LAN_SENDNOACK        0x0004  /* LAN allows SendNoAck NCBs */
#define NB_LAN_LMEXT            0x0008  /* LAN supports LAN Manager extended NCBs */
#define NB_LAN_INTNCB           0x0010  /* LAN allows NCB submission at */
                                        /* interrupt time (from NCBDone) */
#define NB_LAN_NORESET          0x0040  /*  @p01a  */

#define NB_OPEN_MODE_MASK       0xC000  /* Mask for NetBios Open Modes */
#define NB_OPEN_REGULAR         0x4000  /* NetBios opened in Regular mode */
#define NB_OPEN_PRIVILEGED      0x8000  /* NetBios opened in Privileged mode */
#define NB_OPEN_EXCLUSIVE       0xC000  /* NetBios opened in Exclusive mode */

/*
 *      Open modes for NetBiosOpen.
 */

#define NB_REGULAR      1
#define NB_PRIVILEGED   2
#define NB_EXCLUSIVE    3


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETBIOS_INCLUDED */
