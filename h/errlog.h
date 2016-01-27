/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/* The source code for this program is not published or otherwise divested of  */
/* its tradesecrets, irrespective of what has been deposited with the U.S.     */
/* Copyright Office.                                                           */
/*                                                                             */
/*******************************************************************************/
/*
 *  Change History:
 *
 *  @p01  DCR733  92JUL09 R700 GY <DCR733 NO_IPE. Added NELOG_BadSpareB,
 *                                 NELOG_NoHotFix, NELOG_BadVolume>
 *  @p02 DCR736.1 92JUL28 R700 JC  Added NELOG_DCDBChecksumError,
 *                                 NELOG_DCDBIOError, NELOG_DCDBNotFoundError.
 *  @p03 PAZ11924 92AUG11 R700 BTL <Remove Nonused LocalSecFail1/2/3 and
 *                                  LocalSecGeneralFail>
 *  @d04 DCR590   92ju127 R700 EAM <Sideband. NET3199,NET3200,NET3201>
 *  @d05 DCR827   92AUG15 R700  GY <Add NELOG_SrvNoResource and associated
 *                                  strings >
 *
 *     Post ISO/CMVC Revision history: (f = feature d = defect)                *
 *  Flag Change Fixer Date    Tester Date    Review Date    Release  Describe  *
 *  ---- ------ ----- ------- ------ ------- ------ ------- -------  --------  *
 *  @f06 f02359 MBB   93Nov11
 *   Add Net32 prototypes and internal-only structs; make all 'char' UCHAR
 *  @f07 f04721 BTL   94Feb24
 *   Add additional R0SV out of resource message numbers
 *  @f08 f02433 gy    94Mar04  :Add msgs to support alerting DASD Limits.
 *                              (3268-3277)
 *  @d09 D06991 MBB   94Apr09
 *   Declare all 32-bit functions as API32_FUNCTION
 *  @d10 d07636 gy    94may13  :Modify 3268,3269
 *  @d11 D09549 MBB   94Jun24
 *   Just before NETERRLOG_INCLUDED, eliminate #ifdef and #endif around the
 *   #pragma pack(1) statement
 *  @f12 F05438 MBB   94Aug08                                LS 4.0
 *   Make .h files compatible with C++
 *  d 28237 21nov95 sl : remove NELOG_UPS_PowerOut
 *  @f13 f55820 sld   99Jun08 sld    99Jun08 lc     99Jul01 ls.53
 *   Add NELOG_JFSDirAlerts_FatalError and NELOG_JFSDirAlerts_NoAlerts
 *  @f14 f55952 lc    99Jun14 lc     99Jun17 sld    99Jul22 ls.53
 *   Add NELOG_NetlogonSyncRefused
 *
 */
/**END_INTERNAL**/

/********************************************************************/
/**                        IBM LAN Server                          **/
/** Copyright (c) International Business Machines Corp., 1987-1993 **/
/** Copyright (c) Microsoft Corp., 1987-1990                       **/
/********************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  ERRLOG.H                                   *
 *                                                                  *
 *  This file contains information about the NetErrorLog APIs.      *
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


#ifndef NETERRLOG_INCLUDED

#define NETERRLOG_INCLUDED

#pragma pack(1)


/****************************************************************
 *                                                              *
 *              Data Structures for LogRead                     *
 *                                                              *
 ****************************************************************/


#ifndef LOGFLAGS_FORWARD

typedef struct loghandle
    {
        unsigned long   time;           /* Timestamp of first record */
        unsigned long   last_flags;     /* Last call's flags */
        unsigned long   offset;         /* Current byte offset in log */
        unsigned long   rec_offset;     /* Current record offset in log */
    }   HLOG;


#define LOGFLAGS_FORWARD        0
#define LOGFLAGS_BACKWARD       0x1
#define LOGFLAGS_SEEK           0x2

#endif

/****************************************************************
 *                                                              *
 *                Function prototypes                           *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetErrorLogClear ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszBackupFile,
                     unsigned char LSFAR *       pszReserved );

extern API_FUNCTION
  NetErrorLogOpen ( const unsigned char LSFAR * pszServer,
                    unsigned LSFAR *            phErrorLog,
                    unsigned char LSFAR *       pszReserved );

extern API_FUNCTION
  NetErrorLogRead ( const unsigned char LSFAR *     pszServer,
                    const unsigned char LSFAR *     pszReserved1,
                    HLOG LSFAR *                    phErrorLog,
                    unsigned long                   ulOffset,
                    unsigned short LSFAR  *         pusReserved2,
                    unsigned long                   ulReserved3,
                    unsigned long                   flOffset,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short                  cbBuffer,
                    unsigned short LSFAR  *         pcbReturned,
                    unsigned short LSFAR  *         pcbTotalAvail );

extern API_FUNCTION
  NetErrorLogWrite ( unsigned char LSFAR *       pszReserved1,
                     unsigned short              usCode,
                     const unsigned char LSFAR * pszComponent,
                     const unsigned char LSFAR * pbBuffer,
                     unsigned short              cbBuffer,
                     const unsigned char LSFAR * pszStrBuf,
                     unsigned short              cStrBuf,
                     unsigned char LSFAR *       pszReserved2 );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/


struct error_log {
        unsigned short  el_len;
        unsigned short  el_reserved;
        unsigned long   el_time;
        unsigned short  el_error;
        unsigned char   el_name[SNLEN+1];
        unsigned short  el_data_offset;
        unsigned short  el_nstrings;
}; /* error_log */



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *       Function prototypes for "pure" 32-bit code             *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32ErrorLogClear ( const unsigned char * pszServer,
                       const unsigned char * pszBackupFile,
                       unsigned char       * pszReserved );

extern API32_FUNCTION
  Net32ErrorLogRead ( const unsigned char    * pszServer,
                      const unsigned char    * pszReserved1,
                      HLOG                   * phErrorLogHandle,
                      unsigned long            ulOffset,
                      unsigned short         * pusReserved2,
                      unsigned long            ulReserved3,
                      unsigned long            ulOpenflags,
                      unsigned char          * pbBuffer,
                      unsigned long            ulBuffer,
                      unsigned long          * pulReturned,
                      unsigned long          * pulTotalAvail );

extern API32_FUNCTION
  Net32ErrorLogWrite ( unsigned char       * pszReserved1,
                       unsigned short        usCode,
                       const unsigned char * pszComponent,
                       const unsigned char * pbBuffer,
                       unsigned long         ulBuffer,
                       const unsigned char * pszStrBuf,
                       unsigned short        cStrBuf,
                       unsigned char       * pszReserved2 );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/



#define ERRLOG_BASE     3100            /* NELOG errors start here */



/*
 *      Generic (could be used by more than one service)
 *       error log messages from 0 to 25
 */

#define NELOG_Internal_Error            (ERRLOG_BASE + 0)
        /*
         * The operation failed because a network software error occurred.
         */


#define NELOG_Resource_Shortage         (ERRLOG_BASE + 1)
        /*
         * The system ran out of a resource controlled by the %1 option.
         */

#define NELOG_Unable_To_Lock_Segment    (ERRLOG_BASE + 2)
        /*
         * The service failed to obtain a long-term lock on the
         *  segment for NCBs. The error code is the data.
         */

#define NELOG_Unable_To_Unlock_Segment  (ERRLOG_BASE + 3)
        /*
         * The service failed to release the long-term lock on the
         *  segment for NCBs. The error code is the data.
         */

#define NELOG_Uninstall_Service         (ERRLOG_BASE + 4)
        /*
         * There was an error stopping service %1.
         *  The error code from NetServiceControl is the data.
         */

#define NELOG_Init_Exec_Fail            (ERRLOG_BASE + 5)
        /*
         * Initialization failed because of an OS/2 exec failure on
         *  path %1. The OS/2 error code is the data.
         */

#define NELOG_Ncb_Error                 (ERRLOG_BASE + 6)
        /*
         * An unexpected NCB was received. The NCB is the data.
         */

#define NELOG_Ioctl_Error               (ERRLOG_BASE + 8)
        /*
         * A DosDevIoctl or DosFsCtl to NETWKSTA.SYS failed.
         * The data shown is in this format:
         *     DWORD  approx CS:IP of call to ioctl or fsctl
         *     WORD   error code
         *     WORD   ioctl or fsctl number
         */

#define NELOG_System_Semaphore          (ERRLOG_BASE + 9)
        /*
         * Unable to create or open system semaphore %1.
         *  The error code is the data.
         */

#define NELOG_Init_OpenCreate_Err       (ERRLOG_BASE + 10)
        /*
         * Initialization failed because of an open/create error on the
         *  file %1. The OS/2 error code is the data.
         */

#define NELOG_NetBios                   (ERRLOG_BASE + 11)
        /*
         * An unexpected NetBios error occurred.
         *  The error code is the data.
         */

#define NELOG_SMB_Illegal               (ERRLOG_BASE + 12)
        /*
         * An illegal SMB was received.
         *  The SMB is the data.
         */

#define NELOG_Service_Fail              (ERRLOG_BASE + 13)
        /*
         * Initialization failed because the requested service %1
         *  could not be started.
         */

#define NELOG_Entries_Lost              (ERRLOG_BASE + 14)
        /*
         * Some entries in the error log were lost because of a buffer
         * overflow.
         */


/*
 *      Server specific error log messages from 20 to 40
 */

#define NELOG_Init_Seg_Overflow         (ERRLOG_BASE + 20)
        /*
         * Initialization parameters controlling resource usage other
         *  than net buffers are sized so that too much memory is needed.
         */

#define NELOG_Srv_No_Mem_Grow           (ERRLOG_BASE + 21)
        /*
         * The server cannot increase the size of a memory segment.
         */

#define NELOG_Access_File_Bad           (ERRLOG_BASE + 22)
        /*
         * Initialization failed because account file: %1 is either incorrect
         * or not present.
         */

#define NELOG_Srvnet_Not_Started        (ERRLOG_BASE + 23)
        /*
         * Initialization failed because network: %1 was not started.
         */

#define NELOG_Init_Chardev_Err          (ERRLOG_BASE + 24)
        /*
         * The server failed to start. Either all three chdev
         *  parameters must be zero or all three must be nonzero.
         */

#define NELOG_Remote_API                (ERRLOG_BASE + 25)
        /* A remote API request was halted due to the following
         * invalid description string: %1.
         */

#define NELOG_Ncb_TooManyErr            (ERRLOG_BASE + 26)
        /* The network %1 ran out of NCBs.  You may need to increase NCBs
         * for this network.  The following information includes the
         * number of NCBs submitted by the server when this error occurred:
         */

#define NELOG_Mailslot_err              (ERRLOG_BASE + 27)
        /* The server cannot create the %1 mailslot needed to send
         * the ReleaseMemory alert message.  The error received is:
         */

#define NELOG_ReleaseMem_Alert          (ERRLOG_BASE + 28)
        /* The server failed to register for the RELEASEMEMORY alert,
         * with recipient %1. The error code from
         * NetAlertStart is the data.
         */

#define NELOG_AT_cannot_write           (ERRLOG_BASE + 29)
        /* The server cannot update the AT schedule file. The file
         * is corrupt.
         */

#define NELOG_Cant_Make_Msg_File        (ERRLOG_BASE + 30)
        /* The server encountered an error when calling
         * NetIMakeLMFileName. The error code is the data.
         */

#define NELOG_Exec_Netservr_NoMem       (ERRLOG_BASE + 31)
        /* Initialization failed because of an OS/2 exec failure on
         * path %1. There is not enough memory to start the process.
         * The OS/2 error code is the data.
         */

#define NELOG_Server_Lock_Failure       (ERRLOG_BASE + 32)
        /* Longterm lock of server buffers failed.
         * Check swap disk free space and reboot system to start server.
         */
/*
 *      Message service and POPUP specific error log messages from 40 to 55
 */

#define NELOG_Msg_Shutdown              (ERRLOG_BASE + 40)
        /*
         * The service has stopped due to repeated consecutive
         *  occurrences of an NCB error.  The last bad NCB follows
         *  in raw data.
         */

#define NELOG_Msg_Sem_Shutdown          (ERRLOG_BASE + 41)
        /*
         * The Message server has stopped due to a lock on the
         *  Message server shared data segment.
         */

#define NELOG_Msg_Log_Err               (ERRLOG_BASE + 50)
        /*
         * A file system error occurred while opening or writing to the
         *  system message log file, %1. Message logging has been
         *  switched off due to the error. The error code is the data.
         */


#define NELOG_VIO_POPUP_ERR             (ERRLOG_BASE + 51)
        /*
         * Unable to display message POPUP due to OS/2 VIO call error.
         *  The error code is the data.
         */


#define NELOG_Msg_Unexpected_SMB_Type   (ERRLOG_BASE + 52)
        /*
         * An illegal SMB was received.  The SMB is the data.
         */


/*
 *      Requester specific error log messages from 60 to 75
 */


#define NELOG_Wksta_Infoseg             (ERRLOG_BASE + 60)
        /*
         * The workstation information segment is bigger than 64K bytes.
         *  The size follows, in DWORD format:
         */

#define NELOG_Wksta_Compname            (ERRLOG_BASE + 61)
        /*
         * The workstation was unable to get the name-number of computer name.
         */

#define NELOG_Wksta_BiosThreadFailure   (ERRLOG_BASE + 62)
        /*
         * The workstation could not initialize the Async NetBios Thread.
         *  The error code is the data.
         */

#define NELOG_Wksta_IniSeg              (ERRLOG_BASE + 63)
        /*
         * The workstation could not open the initial shared segment.
         *  The error code is the data.
         */

#define NELOG_Wksta_HostTab_Full        (ERRLOG_BASE + 64)
        /*
         * The workstation host table is full.
         */

#define NELOG_Wksta_Bad_Mailslot_SMB    (ERRLOG_BASE + 65)
        /*
         * A bad mailslot SMB was received.  The SMB is the data.
         */

#define NELOG_Wksta_UASInit             (ERRLOG_BASE + 66)
        /*
         * The workstation encountered an error while trying to start the UAS.
         *  The error code is the data.
         */

#define NELOG_Wksta_SSIRelogon          (ERRLOG_BASE + 67)
        /*
         * The workstation encountered an error while responding to an SSI revalidation request.
         *  The function code and the error codes are the data.
         */



/*
 *      Alerter service specific error log messages from 70 to 79
 */


#define NELOG_Build_Name                (ERRLOG_BASE + 70)
        /*
         * The Alerter service had a problem creating the list of
         * alert recipients.  The error code is %1.
         */

#define NELOG_Name_Expansion            (ERRLOG_BASE + 71)
        /*
         * There was an error expanding %1 as a group name. Try
         *  splitting the group into two or more smaller groups.
         */


#define NELOG_Message_Send              (ERRLOG_BASE + 72)
        /*
         * There was an error sending %2 the alert message -
         *  (
         *  %3 )
         *  The error code is %1.
         */


#define NELOG_Mail_Slt_Err              (ERRLOG_BASE + 73)
        /*
         * There was an error in creating or reading the alerter mailslot.
         *  The error code is %1.
         */

#define NELOG_AT_cannot_read            (ERRLOG_BASE + 74)
        /*
         * The server could not read the AT schedule file.
         */

#define NELOG_AT_sched_err              (ERRLOG_BASE + 75)
        /*
         * The server found an invalid AT schedule record.
         */

#define NELOG_AT_schedule_file_created  (ERRLOG_BASE + 76)
        /*
         * The server could not find an AT schedule file so it created one.
         */

#define NELOG_Srvnet_NB_Open            (ERRLOG_BASE + 77)
        /*
         * The server could not access the %1 network with NetBiosOpen.
         */

#define NELOG_AT_Exec_Err               (ERRLOG_BASE + 78)
        /*
         * The AT command processor couldn't run %1.
         */

/*
 *      Cache Lazy Write and HPFS386 specific error log messages from 80 to 89
 */

#define NELOG_Lazy_Write_Err            (ERRLOG_BASE + 80)
        /*
         * WARNING:  Because of a lazy-write error, drive %1 now
         *  contains some corrupt data.  The cache is stopped.
         */

#define NELOG_HotFix                    (ERRLOG_BASE + 81)
        /*
         * A defective sector on drive %1 has been replaced (hotfixed).
         * No data was lost.  You should run CHKDSK soon to restore full
         * performance and replenish the volume's spare sector pool.
         *
         * The hotfix occurred while processing a remote request.
         */

#define NELOG_HardErr_From_Server       (ERRLOG_BASE + 82)
        /*
         * A disk error occurred on the HPFS volume in drive %1.
         * The error occurred while processing a remote request.
         */

#define NELOG_SrvNoResource             (ERRLOG_BASE + 86)
        /*
         * The server ran out of the following resource: %1
         */

#define  NELOG_BadSpareB                (ERRLOG_BASE + 87)
        /*
         * A critical sector error was detected on drive %1.  Back up the drive.  No data was lost.
         */

#define  NELOG_NoHotFix                 (ERRLOG_BASE + 88)
        /*
         * A bad sector on drive %1 cannot be fixed .  Run CHKDSK /F on the drive.
         */

#define  NELOG_BadVolume                (ERRLOG_BASE + 89)
        /*
         * Drive %1 is experiencing severe disk errors.  Back up the drive immediately.
         */

/*
 *      NETWKSTA.SYS specific error log messages from 90 to 99
 */

#define NELOG_NetWkSta_Internal_Error   (ERRLOG_BASE + 90)
        /*
         * A NetWksta internal error has occurred:
         *  %1
         */

#define NELOG_NetWkSta_No_Resource      (ERRLOG_BASE + 91)
        /*
         * The redirector is out of a resource: %1.
         */

#define NELOG_NetWkSta_SMB_Err          (ERRLOG_BASE + 92)
        /*
         * An SMB error occurred on connection to %1.
         *  The SMB header is the data.
         */

#define NELOG_NetWkSta_VC_Err           (ERRLOG_BASE + 93)
        /*
         * A virtual circuit error occurred on the session to %1.
         *  The NCB command and return code is the data.
         */

#define NELOG_NetWkSta_Stuck_VC_Err     (ERRLOG_BASE + 94)
        /*
         * Hanging up a stuck session to %1.
         */

#define NELOG_NetWkSta_NCB_Err          (ERRLOG_BASE + 95)
        /*
         * An NCB error occurred (%1).
         *  The NCB is the data.
         */

#define NELOG_NetWkSta_Write_Behind_Err (ERRLOG_BASE + 96)
        /*
         * A write operation to %1 failed.
         *  Data may have been lost.
         */

#define NELOG_NetWkSta_Reset_Err        (ERRLOG_BASE + 97)
        /*
         * Reset of driver %1 failed to complete the NCB.
         *  The NCB is the data.
         */

#define NELOG_NetWkSta_Too_Many         (ERRLOG_BASE + 98)
        /*
         * The amount of resource %1 requested was more
         *  than the maximum. The maximum amount was allocated.
         */

/*
 *      Sideband specific error log messages from 99 to 101
 */

#define NELOG_SB_Disabled               (ERRLOG_BASE + 99)

        /*
         * Sideband transmissions have been disabled for the session
         *  with server %1.
         */

#define NELOG_SB_Reenabled              (ERRLOG_BASE + 100)

        /*
         * Sideband transmissions have been re-enabled for the session
         *  with server %1.
         */

#define NELOG_SB_Not_Supported          (ERRLOG_BASE + 101)

        /*
         * Sideband transmissions are not supported on %1 as defined in
         *  IBMLAN.INI.
         */
/*
 *      Spooler specific error log messages from 102 to 103
 */

#define NELOG_Srv_Thread_Failure                (ERRLOG_BASE + 104)
        /*
         * The server could not create a thread.
         *  The THREADS parameter in CONFIG.SYS should be increased.
         */

#define NELOG_Srv_Close_Failure                 (ERRLOG_BASE + 105)
        /*
         * The server could not close %1.
         *  The file is probably corrupt.
         */

#define NELOG_ReplUserCurDir                   (ERRLOG_BASE + 106)
    /*
     *The replicator cannot update directory %1. It has tree integrity
     * and is the current directory for some process.
     */

#define NELOG_ReplCannotMasterDir               (ERRLOG_BASE + 107)
    /*
     *The server cannot export directory %1, to client %2.
     * It is exported from another server.
     */

#define NELOG_ReplUpdateError                   (ERRLOG_BASE + 108)
    /*
     *The replication server could not update directory %2 from the source
     *on %3 due to error %1.
     */

#define NELOG_ReplLostMaster                    (ERRLOG_BASE + 109)
    /*
     *Master %1 did not send an update notice for directory %2 at the expected
     * time.
     */

#define NELOG_NetlogonAuthDCFail                (ERRLOG_BASE + 110)
    /*
     *Failed to authenticate with %2, the domain controller for domain %1.
     */

#define NELOG_ReplLogonFailed                   (ERRLOG_BASE + 111)
    /*
     *The replicator attempted to log on at %3 as %2 and failed.
     */

#define NELOG_ReplNetErr                        (ERRLOG_BASE + 112)
        /*
         *  Network error %1 occurred.
         */



#define NELOG_ReplMaxFiles                        (ERRLOG_BASE + 113)
        /*
         *  Replicator limit for files in a directory has been exceeded.
         */


#define NELOG_ReplMaxTreeDepth                    (ERRLOG_BASE + 114)
        /*
         *  Replicator limit for tree depth has been exceeded.
         */



#define NELOG_ReplBadMsg                         (ERRLOG_BASE + 115)
        /*
         *  Unrecognized message received in mailslot.
         */

#define NELOG_ReplSysErr                        (ERRLOG_BASE + 116)
        /*
         *  System error %1 occurred.
         */

#define NELOG_ReplUserLoged                      (ERRLOG_BASE + 117)
        /*
         *  Cannot log on. User is currently logged on and argument TRYUSER
         *  is set to NO.
         */

#define NELOG_ReplBadImport                       (ERRLOG_BASE + 118)
        /*
         *  IMPORT path %1 cannot be found.
         */


#define NELOG_ReplBadExport                       (ERRLOG_BASE + 119)
        /*
         *  EXPORT path %1 cannot be found.
         */


#define NELOG_ReplSignalFileErr                   (ERRLOG_BASE + 120)
        /*
         *  Replicator failed to update signal file in directory %2 due to
         *  %1 system error.
         */


#define NELOG_DiskFT                            (ERRLOG_BASE+121)
        /*
         * Disk Fault Tolerance Error
         *
         *%1
         */

#define NELOG_ReplAccessDenied                   (ERRLOG_BASE + 122)
        /*
         *  Replicator could not access %2
         *  on %3 due to %1 system error.
         */

#define NELOG_NetlogonFailedPrimary              (ERRLOG_BASE + 123)
    /*
     *The primary domain controller for domain %1 has apparently failed.
     */

#define NELOG_NetlogonPasswdSetFailed (ERRLOG_BASE + 124)
    /*
     * Error in changing this machine's password.
     */

#define NELOG_NetlogonTrackingError              (ERRLOG_BASE + 125)
    /*
     *Error in updating the logon or logoff information for %1.
     */

#define NELOG_NetlogonSyncError                  (ERRLOG_BASE + 126)
    /*
     *Error in synchronizing with primary domain controller %1
     */

#define NELOG_BadReplIniParm                     (ERRLOG_BASE + 127)
    /*
     * The replicator detected the following invalid parameter in
     *  REPL.INI in subdirectory %1 : %2
     */
#define NELOG_ReplUpdateACL                     (ERRLOG_BASE + 128)
    /*
     * The following error occured while updating the access control
     * profile for %1
     */

#define NELOG_NetlogonSyncRefused               (ERRLOG_BASE + 129)
    /*
     *The NET.ACC located on the primary domain controller for domain %1 can not be used
     * for synchronization.
     */

/*
 *      UPS service specific error log messages from 130 to 135
 */

#define NELOG_UPS_Shutdown              (ERRLOG_BASE + 131)
        /*
         * The UPS service performed server shutdown.
         */

#define NELOG_UPS_CmdFileError          (ERRLOG_BASE + 132)
        /*
         * The UPS service did not complete execution of the
         * user specified shutdown command file.
         */

#define NELOG_UPS_CannotOpenDriver      (ERRLOG_BASE+133)
        /* The UPS driver could not be opened.  The error code is
         * the data.
         */

#define NELOG_UPS_LowBattery            (ERRLOG_BASE + 134)
        /*
         * Low battery detected.
         */

/*
 *      HPFS386 'Out of Resource' messages from 135 to 143
 *
 */
#define NELOG_R0SV_NO_RAM_WORKBUF        (ERRLOG_BASE + 135)
        /*
         * RAM for 64Kb work buffer
         */

#define NELOG_R0SV_NO_RAM_HEAP           (ERRLOG_BASE + 136)
        /*
         * RAM for heap expansion
         */

#define NELOG_R0SV_NO_RAM_HANDLE_TBL     (ERRLOG_BASE + 137)
        /*
         * RAM for file handle table growth
         */

#define NELOG_R0SV_NO_CONNECTIONS        (ERRLOG_BASE + 138)
        /*
         * LAN connections
         */

#define NELOG_R0SV_NO_OPEN_FILE_HANDLES  (ERRLOG_BASE + 139)
        /*
         * Open file handles
         */

#define NELOG_R0SV_NO_SEARCH_HANDLES     (ERRLOG_BASE + 140)
        /*
         * Search handles
         */

#define NELOG_R0SV_NO_FIND_HANDLES       (ERRLOG_BASE + 141)
        /*
         * Find handles
         */

#define NELOG_R0SV_NO_WORKBUF            (ERRLOG_BASE + 142)
        /*
         * 64Kb work buffers
         */

#define NELOG_R0SV_NO_HEAP               (ERRLOG_BASE + 143)
        /*
         * Heap space
         */

#define NELOG_R0SV_NO_RAM_RABUF          (ERRLOG_BASE + 144)
        /*
         * RAM for 64KB read-ahead buffer
         */

/*
 *      Multimedia, Isochronous LAN and Bandwidth Management from 145 to 149
 */

#define NELOG_BM_GenFailure             (ERRLOG_BASE + 145)
        /*
         * A bandwidth management error occurred. The error is the data %1.
         */

#define NELOG_BM_SessFailure             (ERRLOG_BASE +146)
        /*
         *  A bandwidth management error occurred on the session to %1.
         *  The error is the data %2.
         */

#define NELOG_R0SV_NO_RABUF              (ERRLOG_BASE + 147)
        /*
         * 64KB read-ahead buffers
         */


/*
 *      Remote boot server specific error log messages are from 150 to 164
 */

#define NELOG_Missing_Parameter         (ERRLOG_BASE + 150)
        /*
         * Initialization failed because of an invalid or missing
         *  parameter in IBMLAN.INI: %1.
         */

#define NELOG_Invalid_Config_Line       (ERRLOG_BASE + 151)
        /*
         * Initialization failed because of an invalid line in the
         *  configuration file %1. The invalid line is the data.
         */

#define NELOG_Invalid_Config_File       (ERRLOG_BASE + 152)
        /*
         * Initialization failed because of an error in the configuration
         *  file %1.
         */

#define NELOG_File_Changed              (ERRLOG_BASE + 153)
        /*
         * The file %1 has been changed after initialization.
         *  The boot block loading was temporarily terminated.
         */

#define NELOG_Files_Dont_Fit            (ERRLOG_BASE + 154)
        /*
         * The files do not fit to the 'boot' block configuration
         *  file %1. Change BASE and ORG definitions or the order
         *  of the files.
         */

#define NELOG_Wrong_DLL_Version         (ERRLOG_BASE + 155)
        /*
         * Initialization failed because the dynamic link
         *  library %1 returned a wrong version number.
         */

#define NELOG_Error_in_DLL              (ERRLOG_BASE + 156)
        /*
         * There was an unrecoverable error in the dynamic
         *  link library of the service.
         */

#define NELOG_System_Error              (ERRLOG_BASE + 157)
        /*
         * The system returned an unexpected error code.
         *  The error code is the data.
         */

#define NELOG_FT_ErrLog_Too_Large (ERRLOG_BASE + 158)
        /*
         * The fault-tolerance error log file, LANROOT\LOGS\FT.LOG
         *  is more than 64K.
         */

#define NELOG_FT_Update_In_Progress (ERRLOG_BASE + 159)
        /*
         * The fault-tolerance error-log file, LANROOT\LOGS\FT.LOG, had the
         * update in progress bit set upon opening, which means that the
         * system crashed while working on the errorlog.
         */

#define NELOG_DCDBChecksumError         (ERRLOG_BASE+165)
        /*
         * A checksum error occurred when accessing the domain control
         * database file %1.
         */

#define NELOG_DCDBIOError               (ERRLOG_BASE+166)
        /*
         * An error occurred when reading from or writing to the
         * domain control database file %1.
         */

#define NELOG_DCDBFileNotFound          (ERRLOG_BASE+167)
        /*
         * The domain control database file %1 was not found.
         */

#define  NELOG_DirAlerts_Threshold                (ERRLOG_BASE+168)
        /*
         * The write operation was successful, but the size of directory %1
         * crossed a threshold.  The remaining space in the directory is %2 KB.
         *
         * (%1 is the directory name)
         */

#define  NELOG_DirAlerts_DirFull                  (ERRLOG_BASE+169)
        /*
         * There was not enough space within directory %1 to perform a write
         * operation.
         *
         * (%1 is the directory name)
         */

#define  NELOG_DirAlerts_FatalError               (ERRLOG_BASE+170)
        /*
         * The server encountered an error while initializing internal
         * data structures.  The server uses these structures to process
         * alerts that pertain to directory limits.  The server will not post
         * alerts.  The OS/2 return code is %1.
         */

#define  NELOG_DirAlerts_IniFailure               (ERRLOG_BASE+171)
         /*
          * The server encountered an error while initializing internal
          * data structures.  The server uses these structures to process
          * alerts that pertain to directory limits.  The server will now
          * use its own default values.  The INI file in use is %1.  The
          * return code is %2.
          */

#define NELOG_DirAlerts_NoAlerts                   (ERRLOG_BASE+172)
        /*
         * An error occurred as the server attempted to send a message
         * concerning a directory limit.  No message was sent.  The OS/2
         * return code is %1.
         */

#define  NELOG_DlimNoHeap                         (ERRLOG_BASE+173)
        /*
         * The 386 HPFS server failed to enable directory limits on drive
         * %1 because of insufficient heap space.  The drive is still
         * operational.
         */

#define  NELOG_DlimDskErr                         (ERRLOG_BASE+174)
        /*
         * The 386 HPFS server failed to enable directory limits
         * on drive %1 because of a disk error.  The drive is still
         * operational.
         */

#define  NELOG_DirAlerts_BadIniParm               (ERRLOG_BASE+175)
        /*
         * The server detected a parameterin the INI file %1 that is not
         * valid.  The parameter is:  %2.
         */

#define  NELOG_DirAlerts_BadIniSyntax             (ERRLOG_BASE+176)
        /*
         * The server detected a parameter in the INI file %1 that does
         * not have a valid syntax.  The parameter is %2.
         */

#define  NELOG_DirAlerts_BadDrive                 (ERRLOG_BASE+177)
        /*
         * The server detected and ignored a drive letter that is not valid
         * within a parameter in the INI file %1.  The parameter is %2.
         */

#define  NELOG_JFSDirAlerts_FatalError               (ERRLOG_BASE+178)
        /*
         * The server encountered an error while initializing support for
         * directory limits on JFS volumes.  Disrectory limits will not be
         * enforced on JFS volumes.  The OS/2 return code is %1.
         */

#define  NELOG_JFSDirAlerts_NoAlerts                 (ERRLOG_BASE+179)
        /*
         * The server encountered an error while initializing internal data
         * structures.  The server uses these structures to process alerts that
         * pertain to directory limits on JFS volumes.  The server will not post
         * alerts for JFS volumes.  The OS/2 return code is %1.
         */

/****************************************************************
 * Microsoft has created a generic error log entry for OEMs to use to
 * log errors from OEM value added services.  The code, which is the
 * 2nd arg to NetErrorLogWrite, is 3299.  This value is manifest in
 * NET/H/ERRLOG.H as NELOG_OEM_Code.  The text for error log entry
 * NELOG_OEM_Code is:  "%1 %2 %3 %4 %5 %6 %7 %8 %9.".
 *
 * Microsoft suggests that OEMs use the insertion strings as follows:
 * %1:  OEM System Name (e.g. 3+Open)
 * %2:  OEM Service Name (e.g. 3+Mail)
 * %3:  Severity level (e.g.  error, warning, etc.)
 * %4:  OEM error log entry sub-identifier  (e.g. error code #)
 * %5 - % 9:  Text.
 *
 * The call to NetErrorWrite must set nstrings = 9, and provide 9
 * ASCIIZ strings.  If the caller does not have 9 insertion strings,
 * provide null strings for the empty insertion strings.
 ****************************************************************/

#define NELOG_OEM_Code                          (ERRLOG_BASE + 199)
        /*
         * %1 %2 %3 %4 %5 %6 %7 %8 %9.
         */


#pragma pack()

#endif /* NETERRLOG_INCLUDED */
