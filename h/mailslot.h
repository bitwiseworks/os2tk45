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
 *  About this file ...  MAILSLOT.H                                 *
 *                                                                  *
 *  This file contains information about the Mailslot APIs.         *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 ********************************************************************/


#ifndef NETMAILSLOT_INCLUDED

#define NETMAILSLOT_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *               Function prototypes - Mailslot                 *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  DosMakeMailslot ( const unsigned char LSFAR * pszName,
                    unsigned short              cbMessageSize,
                    unsigned short              cbMailslotSize,
                    unsigned LSINT LSFAR *      phMailslot );

extern API_FUNCTION
  DosDeleteMailslot ( unsigned LSINT hMailslot );

extern API_FUNCTION
  DosMailslotInfo ( unsigned LSINT         hMailslot,
                    unsigned short LSFAR * pcbMessageSize,
                    unsigned short LSFAR * pcbMailslotSize,
                    unsigned short LSFAR * pcbNextSize,
                    unsigned short LSFAR * pusNextPriority,
                    unsigned short LSFAR * pcMessages);

extern API_FUNCTION
  DosPeekMailslot ( unsigned  LSINT        hMailslot,
                    unsigned char LSFAR *  pbBuffer,
                    unsigned short LSFAR * pcbReturned,
                    unsigned short LSFAR * pcbNextSize,
                    unsigned short LSFAR * pusNextPriority );

extern API_FUNCTION
  DosReadMailslot ( unsigned  LSINT        hMailslot,
                    unsigned char LSFAR *  pbBuffer,
                    unsigned short LSFAR * pcbReturned,
                    unsigned short LSFAR * pcbNextSize,
                    unsigned short LSFAR * pusNextPriority,
                    long                   cTimeout );

extern API_FUNCTION
  DosWriteMailslot ( const unsigned char LSFAR * pszName,
                     const unsigned char LSFAR * pbBuffer,
                     unsigned short              cbBuffer,
                     unsigned short              usPriority,
                     unsigned short              usClass,
                     long                        cTimeout );



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *               Function prototypes - Mailslot                 *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Dos32MakeMailslot ( const unsigned char * pszName,
                    unsigned long         ulMessageSize,  /* was USHORT*/
                    unsigned long         ulMailslotSize, /* was USHORT*/
                    unsigned long       * phMailslot );  /* was unsigned LSINT*/

extern API32_FUNCTION
Dos32DeleteMailslot ( unsigned long  hMailslot );

extern API32_FUNCTION
Dos32MailslotInfo ( unsigned long    hMailslot,        /* was unsigned LSINT */
                    unsigned long  * pulMessageSize,   /* was USHORT */
                    unsigned long  * pulMailslotSize,  /* was USHORT */
                    unsigned long  * pulNextSize,      /* was USHORT */
                    unsigned long  * pulNextPriority,  /* was USHORT */
                    unsigned long  * pulMessages);     /* was USHORT */

extern API32_FUNCTION
Dos32PeekMailslot ( unsigned long    hMailslot,        /* was unsigned LSINT */
                    unsigned char  * pbBuffer,
                    unsigned long  * pulReturned,      /* was USHORT */
                    unsigned long  * pulNextSize,      /* was USHORT */
                    unsigned long  * pulNextPriority); /* was USHORT */

extern API32_FUNCTION
Dos32ReadMailslot ( unsigned long    hMailslot,        /* was unsigned LSINT */
                    unsigned char  * pbBuffer,
                    unsigned long  * pulReturned,      /* was USHORT */
                    unsigned long  * pulNextSize,      /* was USHORT */
                    unsigned long  * pulNextPriority,  /* was USHORT */
                    long             lTimeout );

extern API32_FUNCTION
Dos32WriteMailslot ( const unsigned char * pszName,
                     const unsigned char * pbBuffer,
                     unsigned long         ulBuffer,     /* was USHORT */
                     unsigned long         ulPriority,   /* was USHORT */
                     unsigned long         ulClass,      /* was USHORT */
                     long                  lTimeout );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - Mailslot         *
 *                                                              *
 ****************************************************************/

#define MAILSLOT_NO_TIMEOUT     -1

#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETMAILSLOT_INCLUDED */

