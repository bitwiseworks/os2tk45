/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
 *  FILENAME: AppParmD.H                                                     *
 *                                                                           *
 *                                                                           *
 *  FUNCTIONS:     This .h file has the prototypes for using the internal    *
 *                 AppParm's DB ("Backing Store" DB) routines.  These        *
 *                 routines indirectly support the Net32AppParmXxx APIs.     *
 *                                                                           *
 *  LAST UPDATE: 09/05/97  11:00pm                                           *
 *                                                                           *
 ****************************************************************************/


/******************************************************************************/
/* External Prototypes                                                        */
/******************************************************************************/

API_FUNCTION AppParmDBAdd
           (const UCHAR   *pszServername,   /* server name                    */
                  ULONG   *pulRCM,          /* return code modifier (RCM)     */
                  UniChar *pszAppName,      /* app name defined in dcdb       */
                  ULONG    ulLevel,         /* level (0,1)                    */
                  UniChar *pbBuf,           /* buf returned to caller         */
                  ULONG    ulBufLen,        /* Length of input buffer         */
                  ULONG    ulEntriesInBuf); /* # of entries passed in buffer  */

API_FUNCTION AppParmDBDel
           (const UCHAR   *pszServername,   /* server name                    */
                  ULONG   *pulRCM,          /* return code modifier (RCM)     */
                  UniChar *pszAppName,      /* app name defined in DCDB       */
                  ULONG    ulLevel,         /* level 0 only                   */
                  ULONG    ulReqFlags,      /* AppParmDBDel request flags     */
                  UniChar *pbBuf,           /* buf received                   */
                  ULONG    ulBufLen,        /* size in bytes                  */
                  ULONG    ulEntriesInBuf); /* number of parms in buf         */

API_FUNCTION AppParmDBGet
           (const UCHAR   *pszServername,   /* server name                    */
                  ULONG   *pulRCM,          /* return code modifier (RCM)     */
                  UniChar *pszAppName,      /* app name defined in dcdb       */
                  UniChar *pszUserID,       /* user id defined in UAS         */
                  ULONG    ulLevel,         /* level (0,1)                    */
                  ULONG    ulReqFlags,      /* AppParmDBGet request flags     */
                  UniChar *pbBuf,           /* buf returned to caller         */
                  ULONG    ulBufLen,        /* Length of input buffer         */
                  ULONG   *pulEntriesInBuf, /* # of entries in buf returned   */
                  ULONG   *pulEntriesAvail, /* # of entries available         */
                  ULONG   *pulBytesAvail);  /* bytes available                */

API_FUNCTION AppParmDBSet
           (const UCHAR   *pszServername,   /* server name                    */
                  ULONG   *pulRCM,          /* return code modifier (RCM)     */
                  UniChar *pszAppName,      /* app name defined in dcdb       */
                  UniChar *pszUserID,       /* user id defined in UAS         */
                  ULONG    ulLevel,         /* level (0,1)                    */
                  UniChar *pbBuf,           /* buf returned to caller         */
                  ULONG    ulBufLen,        /* Length of input buffer         */
                  ULONG    ulEntriesInBuf); /* # of entries passed in buffer  */

