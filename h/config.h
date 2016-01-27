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
 *  About this file ...  CONFIG.H                                   *
 *                                                                  *
 *  This file contains information about the NetConfig APIs.        *
 *                                                                  *
 ********************************************************************/


#ifndef NETCONFIG_INCLUDED

#define NETCONFIG_INCLUDED

#ifdef  ANY_32_BIT
#pragma pack(1)
#endif

/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetConfigGet ( const unsigned char LSFAR *     pszComponent,
                 const unsigned char LSFAR *     pszParameter,
                 unsigned char LSFAR *           pbBuffer,
                 unsigned short                  usBuffer,
                 unsigned short LSFAR *          pusParmlen );

extern API_FUNCTION
  NetConfigGetAll ( const unsigned char LSFAR *     pszComponent,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short                  usBuffer,
                    unsigned short LSFAR *          pusReturned,
                    unsigned short LSFAR *          pusTotalAvail );

extern API_FUNCTION
  NetConfigGet2 ( const unsigned char LSFAR *     pszServer,
                  const unsigned char LSFAR *     pszReserved,
                  const unsigned char LSFAR *     pszComponent,
                  const unsigned char LSFAR *     pszParameter,
                  const unsigned char LSFAR *     pbBuffer,
                  unsigned short                  usBuffer,
                  unsigned short LSFAR *          pusParmlen );

extern API_FUNCTION
  NetConfigGetAll2 ( const unsigned char LSFAR *     pszServer,
                     const unsigned char LSFAR *     pszReserved,
                     const unsigned char LSFAR *     pszComponent,
                     unsigned char LSFAR *           pbBuffer,
                     unsigned short                  usBuffer,
                     unsigned short LSFAR *          pusReturned,
                     unsigned short LSFAR *          pusTotalAvail );


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *       Function prototypes for "pure" 32-bit code             *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32ConfigGet2 ( const unsigned char * pszServer,
                    const unsigned char * pszReserved,
                    const unsigned char * pszComponent,
                    const unsigned char * pszParameter,
                    const unsigned char * pbBuffer,
                    unsigned long         ulBuffer,
                    unsigned long       * pulParmlen );

extern API32_FUNCTION
  Net32ConfigGetAll2 ( const unsigned char * pszServer,
                       const unsigned char * pszReserved,
                       const unsigned char * pszComponent,
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,
                       unsigned long       * pulReturned,
                       unsigned long       * pulTotalAvail );

extern ULONG _System
  Net32ConfigSet (PCHAR,      /* server, must be NULL      */
                  PCHAR,      /* reserved, must be NULL    */
                  PCHAR,      /* component                 */
                  ULONG,      /* level, must be 0          */
                  ULONG,      /* reserved, must be 0       */
                  PCHAR,      /* buffer                    */
                  ULONG,      /* buflen                    */
                  ULONG);     /* reserved, must be 0       */

  struct config_info_0
     {
      char     * Key;
      char     * Data;
     };
#endif /* PURE_32 */



#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETCONFIG_INCLUDED */
