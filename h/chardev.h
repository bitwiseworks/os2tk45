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
 *  About this file ...  CHARDEV.H                                  *
 *                                                                  *
 *  This file contains information about the NetCharDev             *
 *  and NetHandle class APIs.                                       *
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
 *              Character Device Class                          *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETCHARDEV ) || !defined( LAN_INCLUDED )) \
    && !defined( NETCHARDEV_INCLUDED )

#define NETCHARDEV_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes - CHARDEV                   *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetCharDevControl ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszDevName,
                      short                       sOpCode );

extern API_FUNCTION
  NetCharDevEnum ( const unsigned char LSFAR *     pszServer,
                   short                           sLevel,
                   unsigned char LSFAR *           pbBuffer,
                   unsigned short                  cbBuffer,
                   unsigned short LSFAR *          pcEntriesRead,
                   unsigned short LSFAR *          pcTotalAvail );

extern API_FUNCTION
  NetCharDevGetInfo ( const unsigned char LSFAR *     pszServer,
                      const unsigned char LSFAR *     pszDevName,
                      short                           sLevel,
                      unsigned char LSFAR *           pbBuffer,
                      unsigned short                  cbBuffer,
                      unsigned short LSFAR *          pcbTotalAvail );

extern API_FUNCTION
  NetCharDevQEnum ( const unsigned char LSFAR *     pszServer,
                    const unsigned char LSFAR *     pszUserName,
                    short                           sLevel,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short                  cbBuffer,
                    unsigned short LSFAR *          pcEntriesRead,
                    unsigned short LSFAR *          pcTotalAvail );

extern API_FUNCTION
  NetCharDevQGetInfo ( const unsigned char LSFAR *     pszServer,
                       const unsigned char LSFAR *     pszQueueName,
                       const unsigned char LSFAR *     pszUserName,
                       short                           sLevel,
                       unsigned char LSFAR *           pbBuffer,
                       unsigned short                  cbBuffer,
                       unsigned short LSFAR *          pcbTotalAvail );

extern API_FUNCTION
  NetCharDevQSetInfo ( const unsigned char LSFAR * pszServer,
                       const unsigned char LSFAR * pszQueueName,
                       short                       sLevel,
                       const unsigned char LSFAR * pbBuffer,
                       unsigned short              cbBuffer,
                       short                       sParmNum );

extern API_FUNCTION
  NetCharDevQPurge ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszQueueName );

extern API_FUNCTION
  NetCharDevQPurgeSelf ( const unsigned char LSFAR * pszServer,
                         const unsigned char LSFAR * pszQueueName,
                         const unsigned char LSFAR * pszComputerName );

/****************************************************************
 *                                                              *
 *              Data structure templates - CHARDEV              *
 *                                                              *
 ****************************************************************/

struct chardev_info_0 {
    unsigned char          ch0_dev[DEVLEN+1];  /* device name                       */
};      /* chardev_info_0 */

struct chardev_info_1 {
    unsigned char  ch1_dev[DEVLEN+1]; /* device name                        */
    unsigned char  ch1_pad1;          /* pad to a word boundary             */
    unsigned short ch1_status;        /* status                             */
                                      /*   bit 0 reserved                   */
                                      /*   bit 1 on = opened                */
                                      /*   bit 1 off = idle                 */
                                      /*   bit 2 on = error                 */
                                      /*   bit 2 off = no error             */
    unsigned char  ch1_username[UNLEN+1]; /* name of device's current user  */
    unsigned char  ch1_pad2;          /* pad to a word boundary             */
    unsigned long  ch1_time;          /* time current user attached         */
}; /* chardev_info_1 */


struct chardevQ_info_0 {
    unsigned char          cq0_dev[NNLEN+1];   /* queue name (network name)         */
}; /* chardevQ_info_0 */

struct chardevQ_info_1 {
    unsigned char               cq1_dev[NNLEN+1];  /* queue name (network name)     */
    unsigned char               cq1_pad;           /* pad to a word boundary      */
    unsigned short              cq1_priority;      /* priority (1 - 9)             */
    unsigned char LSFAR * LSPTR cq1_devs;          /* names of devices assigned to queue */
    unsigned short              cq1_numusers;      /* # of users waiting in queue        */
    unsigned short              cq1_numahead;      /* # of users in queue ahead of this  */
                                                   /*     user. -1 is returned if the    */
                                                   /*     user is not in the queue.      */
}; /* chardevQ_info_1 */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - CHARDEV                   *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
Net32CharDevControl ( const unsigned char * pszServer,
                      const unsigned char * pszDevName,
                      unsigned long         ulOpCode );   /*was signed short*/

extern API32_FUNCTION
Net32CharDevEnum ( const unsigned char * pszServer,
                   unsigned long         ulLevel,         /*was signed short*/
                   unsigned char       * pbBuffer,
                   unsigned long         ulBuffer,        /*was USHORT*/
                   unsigned long       * pulEntriesRead,  /*was USHORT*/
                   unsigned long       * pulTotalAvail ); /*was USHORT*/

extern API32_FUNCTION
Net32CharDevGetInfo ( const unsigned char * pszServer,
                      const unsigned char * pszDevName,
                      unsigned long         ulLevel,        /*was signed short*/
                      unsigned char       * pbBuffer,
                      unsigned long         ulBuffer,        /*was USHORT*/
                      unsigned long       * pulTotalAvail ); /*was USHORT*/

extern API32_FUNCTION
Net32CharDevQEnum ( const unsigned char * pszServer,
                    const unsigned char * pszUserName,
                    unsigned long         ulLevel,          /*was signed short*/
                    unsigned char       * pbBuffer,
                    unsigned long         ulBuffer,          /*was USHORT*/
                    unsigned long       * pulEntriesRead,    /*was USHORT*/
                    unsigned long       * pulTotalAvail );   /*was USHORT*/

extern API32_FUNCTION
Net32CharDevQGetInfo ( const unsigned char * pszServer,
                       const unsigned char * pszQueueName,
                       const unsigned char * pszUserName,
                       unsigned long         ulLevel,       /*was signed short*/
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,       /*was USHORT*/
                       unsigned long       * pulTotalAvail); /*was USHORT*/

extern API32_FUNCTION
Net32CharDevQSetInfo ( const unsigned char * pszServer,
                       const unsigned char * pszQueueName,
                       unsigned long         ulLevel,     /*was signed short*/
                       const unsigned char * pbBuffer,
                       unsigned long         ulBuffer,    /*was USHORT*/
                       unsigned long         ulParmNum ); /*was signed short*/

extern API32_FUNCTION
Net32CharDevQPurge ( const unsigned char * pszServer,
                     const unsigned char * pszQueueName );

extern API32_FUNCTION
Net32CharDevQPurgeSelf ( const unsigned char * pszServer,
                         const unsigned char * pszQueueName,
                         const unsigned char * pszComputerName );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - CHARDEV          *
 *                                                              *
 ****************************************************************/

/*
 *      Bits for chardev_info_1 field ch1_status.
 */

#define CHARDEV_STAT_OPENED             0x02
#define CHARDEV_STAT_ERROR              0x04

/*
 *      Opcodes for NetCharDevControl
 */

#define CHARDEV_CLOSE                   0

/*
 *      Values for parmnum parameter to NetCharDevQSetInfo.
 */

#define CHARDEVQ_PRIORITY_PARMNUM       2
#define CHARDEVQ_DEVICES_PARMNUM        3


/*
 *      Minimum, maximum, and recommended default for priority.
 */

#define CHARDEVQ_MAX_PRIORITY           1
#define CHARDEVQ_MIN_PRIORITY           9
#define CHARDEVQ_DEF_PRIORITY           5

/*
 *      Value indicating no requests in the queue.
 */

#define CHARDEVQ_NO_REQUESTS            -1


#ifdef ANY_32_BIT
#pragma pack()
#endif
#endif /* NETCHARDEV_INCLUDED */

/****************************************************************
 *                                                              *
 *              Handle Class                                    *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETHANDLE ) || !defined( LAN_INCLUDED )) \
    && !defined( NETHANDLE_INCLUDED )

#define NETHANDLE_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes - HANDLE                    *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetHandleGetInfo ( unsigned short         hHandle,
                     short                  sLevel,
                     unsigned char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetHandleSetInfo ( unsigned short hHandle,
                     short          sLevel,
                     unsigned char LSFAR *   pbBuffer,
                     unsigned short cbBuffer,
                     unsigned short sParmNum );


/****************************************************************
 *                                                              *
 *              Data structure templates - HANDLE               *
 *                                                              *
 ****************************************************************/

struct handle_info_1 {
        unsigned long   hdli1_chartime; /* time in msec to wait before send */
        unsigned short  hdli1_charcount; /* max size of send buffer */
}; /* handle_info_1 */

struct handle_info_2 {
      unsigned char LSFAR * LSPTR hdli2_username;  /* owner of name-pipe handle */
}; /* handle_info_2 */


/****************************************************************
 *                                                              *
 *              Special values and constants - HANDLE           *
 *                                                              *
 ****************************************************************/


/*
 *      Handle Get Info Levels
 */

#define HANDLE_INFO_LEVEL_1             1
#define HANDLE_INFO_LEVEL_2             2


/*
 *      Handle Set Info    parm numbers
 */

#define HANDLE_SET_CHAR_TIME            1
#define HANDLE_SET_CHAR_COUNT           2


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETHANDLE_INCLUDED */
