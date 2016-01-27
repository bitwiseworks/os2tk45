/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/**                                                              */
/*****************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  MESSAGE.H                                  *
 *                                                                  *
 *  This file contains information about the NetMessage APIs.       *
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


#ifndef NETMESSAGE_INCLUDED

#define NETMESSAGE_INCLUDED

#ifdef  ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetMessageBufferSend ( const unsigned char LSFAR * pszServer,
                         unsigned char LSFAR *       pszRecipient,
                         unsigned char LSFAR *       pbBuffer,
                         unsigned short              cbBuffer );

extern API_FUNCTION
  NetMessageFileSend ( const unsigned char LSFAR * pszServer,
                       unsigned char LSFAR *       pszRecipient,
                       unsigned char LSFAR *       pszFileSpec );

extern API_FUNCTION
  NetMessageLogFileGet ( const unsigned char LSFAR * pszServer,
                         unsigned char LSFAR *       pbBuffer,
                         unsigned short      cbBuffer,
                         short LSFAR *       pfsEnabled );

extern API_FUNCTION
  NetMessageLogFileSet ( const unsigned char LSFAR * pszServer,
                         unsigned char LSFAR *       pszFileSpec,
                         short               fsEnabled );

extern API_FUNCTION
  NetMessageNameAdd ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszMessageName,
                      short               fsFwdAction );

extern API_FUNCTION
  NetMessageNameDel ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszMessageName,
                      short               fsFwdAction );

extern API_FUNCTION
  NetMessageNameEnum ( const unsigned char LSFAR *     pszServer,
                       short                   sLevel,
                       unsigned char LSFAR *           pbBuffer,
                       unsigned short         cbBuffer,
                       unsigned short LSFAR * pcEntriesRead,
                       unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetMessageNameGetInfo ( const unsigned char LSFAR *     pszServer,
                          const unsigned char LSFAR *     pszMessageName,
                          short                   sLevel,
                          unsigned char LSFAR *           pbBuffer,
                          unsigned short          cbBuffer,
                          unsigned short LSFAR *  pcbTotalAvail );

extern API_FUNCTION
  NetMessageNameFwd ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszMessageName,
                      const unsigned char LSFAR * pszForwardName,
                      short               fsDelFwdName );

extern API_FUNCTION
  NetMessageNameUnFwd ( const unsigned char LSFAR * pszServer,
                        const unsigned char LSFAR * pszMessageName );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/

struct msg_info_0 {
    unsigned char       msgi0_name[CNLEN + 1];
};      /* msg_info_0 */

struct msg_info_1 {
    unsigned char       msgi1_name[CNLEN + 1];
    unsigned char       msgi1_forward_flag;
    unsigned char       msgi1_pad1;
    unsigned char       msgi1_forward[CNLEN + 1];
};      /* msg_info_1 */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *            for "pure" 32-bit code                            *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32MessageBufferSend ( const unsigned char  * pszServer,
                           unsigned char* pszRecipient,
                           unsigned char* pbBuffer,
                           unsigned long  ulBuffer );   /* was USHORT */

extern API32_FUNCTION
  Net32MessageFileSend ( const unsigned char* pszServer,
                         unsigned char      * pszRecipient,
                         unsigned char      * pszFileSpec );

extern API32_FUNCTION
  Net32MessageLogFileGet ( const unsigned char    * pszServer,
                           unsigned char  * pbBuffer,
                           unsigned long    ulBuffer,     /* was USHORT */
                           unsigned long  * pfsEnabled ); /* was signed short */

extern API32_FUNCTION
  Net32MessageLogFileSet ( const unsigned char    * pszServer,
                           unsigned char  * pszFileSpec,
                           unsigned long    fsEnabled );  /* was signed short */

extern API32_FUNCTION
  Net32MessageNameAdd ( const unsigned char    * pszServer,
                        const unsigned char    * pszMessageName,
                        unsigned long    fsFwdAction );    /* was signed short*/

extern API32_FUNCTION
  Net32MessageNameDel ( const unsigned char    * pszServer,
                        const unsigned char    * pszMessageName,
                        unsigned long    fsFwdAction );    /*was signed short */

extern API32_FUNCTION
  Net32MessageNameEnum ( const unsigned char    * pszServer,
                         unsigned long    ulLevel,        /*was signed short*/
                         unsigned char  * pbBuffer,
                         unsigned long    ulBuffer,       /* was USHORT */
                         unsigned long  * pulEntriesRead, /* was USHORT */
                         unsigned long  * pulTotalAvail); /* was USHORT */

extern API32_FUNCTION
  Net32MessageNameGetInfo ( const unsigned char    * pszServer,
                            const unsigned char    * pszMessageName,
                            unsigned long    ulLevel,       /*was signed short*/
                            unsigned char  * pbBuffer,
                            unsigned long    ulBuffer,       /* was USHORT */
                            unsigned long  * pulTotalAvail); /* was USHORT */

extern API32_FUNCTION
  Net32MessageNameFwd ( const unsigned char   * pszServer,
                        const unsigned char   * pszMessageName,
                        const unsigned char   * pszForwardName,
                        unsigned long   fsDelFwdName );  /* was short */

extern API32_FUNCTION
  Net32MessageNameUnFwd ( const unsigned char* pszServer,
                          const unsigned char* pszMessageName );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/


/*
 *      Values for msgi1_forward_flag.
 */

#define MSGNAME_NOT_FORWARDED   0       /* Name not forwarded */
#define MSGNAME_FORWARDED_TO    0x04    /* Name forward to remote station */
#define MSGNAME_FORWARDED_FROM  0x10    /* Name forwarded from remote station */


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETMESSAGE_INCLUDED */
