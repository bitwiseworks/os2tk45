/****************************** Module Header ******************************\
*
* Module Name: PMSPL.H
*
* OS/2 Presentation Manager Spooler constants, types and function declarations
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1993
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_SPLERRORS          defined if INCL_ERRORS defined
*   INCL_SPLFSE             file system emulation calls
*   INCL_SPLDOSPRINT        DosPrint APIs
*   INCL_SPLBIDI            bidirectional print defines in pmsplb.h
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between XLATOFF
* and XLATON comments.
*
\***************************************************************************/

/* XLATOFF */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( none )
   #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* XLATON */

/* XLATOFF */
#ifndef PMSPL_INCLUDED
   /* XLATON */
   #define PMSPL_INCLUDED
   
   
   
   /* XLATOFF */
   #define DosPrintDestControl       SplControlDevice
   #define DosPrintDestGetInfo       SplQueryDevice
   #define DosPrintDestEnum          SplEnumDevice
   #define DosPrintJobContinue       SplReleaseJob
   #define DosPrintJobPause          SplHoldJob
   #define DosPrintJobDel            SplDeleteJob
   #define DosPrintJobGetInfo        SplQueryJob
   #define DosPrintJobSetInfo        SplSetJob
   #define DosPrintJobEnum           SplEnumJob
   #define DosPrintQAdd              SplCreateQueue
   #define DosPrintQPause            SplHoldQueue
   #define DosPrintQContinue         SplReleaseQueue
   #define DosPrintQDel              SplDeleteQueue
   #define DosPrintQGetInfo          SplQueryQueue
   #define DosPrintQSetInfo          SplSetQueue
   #define DosPrintQEnum             SplEnumQueue
   #define DosPrintDestAdd           SplCreateDevice
   #define DosPrintDestSetInfo       SplSetDevice
   #define DosPrintDestDel           SplDeleteDevice
   #define DosPrintQPurge            SplPurgeQueue
   #define DosPrintDriverEnum        SplEnumDriver
   #define DosPrintQProcessorEnum    SplEnumQueueProcessor
   #define DosPrintPortEnum          SplEnumPort
   /* XLATON */
   
   /*** if error definitions are required then allow Spooler errors ********/
   #ifdef INCL_ERRORS
      #define INCL_SPLERRORS
   #endif /* INCL_ERRORS */
   
   /*** Names of various OS2SYS.INI spooler variables **********************/
   
   #define SPL_INI_SPOOLER         "PM_SPOOLER"
   #define SPL_INI_QUEUE           "PM_SPOOLER_QUEUE"
   #define SPL_INI_PRINTER         "PM_SPOOLER_PRINTER"
   #define SPL_INI_PRINTERDESCR    "PM_SPOOLER_PRINTER_DESCR"
   #define SPL_INI_QUEUEDESCR      "PM_SPOOLER_QUEUE_DESCR"
   #define SPL_INI_QUEUEDD         "PM_SPOOLER_QUEUE_DD"
   #define SPL_INI_QUEUEDDDATA     "PM_SPOOLER_QUEUE_DDDATA"
   
   /*** General SPL return values ******************************************/
   #define SPL_ERROR     0L
   #define SPL_OK        1L
   
   /*** handle to a spool file *********************************************/
   typedef LHANDLE HSPL;               /* hspl */
   
   /* Used in recording of PM_Q_STD data via SplStdxxx calls */
   typedef LHANDLE HSTD;               /* hstd */
   typedef HSTD *PHSTD;
   
   /*** spooler manager open data ******************************************/
   typedef PSZ *PQMOPENDATA;       /* pqmdop */
   
   
   
   
   /*** Spooler Queue Processor interface **********************************/
   
   /* control codes for SplQpControl */
   #define SPLC_ABORT     1
   #define SPLC_PAUSE     2
   #define SPLC_CONTINUE  3
   
   /* flag defines for optional SplQpQueryFlags           */
   /* set this to allow spooler to bypass Queue Processor */
   /* for PM_Q_RAW jobs.  This allows print while a job   */
   /* is still spooling.                                  */
   #define QP_RAWDATA_BYPASS 0x00000001L
   /* set this if queue driver can print separator pages  */
   #define QP_PRINT_SEPARATOR_PAGE  0x00000002
   
   /*** handle to a spooler queue processor ********************************/
   typedef LHANDLE HPROC;              /* hproc */
   
   /*** spooler processor open data ****************************************/
   typedef PSZ *PQPOPENDATA;       /* pqpdop */
   
   /*** Spooler Queue Processor interface **********************************/
   /* API's exported by Queue Processor DLL's                              */
   HPROC EXPENTRY SplQpOpen(LONG        cData,
   PQPOPENDATA pQPDataIn);
   
   BOOL  EXPENTRY SplQpPrint(HPROC hproc,
   PSZ   pszFileName);
   
   BOOL  EXPENTRY SplQpClose(HPROC hproc);
   
   BOOL  EXPENTRY SplQpControl(HPROC hproc,
   LONG  cmdCode);
   
   BOOL  EXPENTRY SplQpQueryDt(PLONG    pcDataType,
   PSZ *paszDataTypes);
   
   BOOL  EXPENTRY SplQpInstall(HWND hwnd);
   
   /* The next API is optionally exported by Queue Processors */
   /* The flags may be set to QP_RAWDATA_BYPASS to allow the  */
   /* spooler to print the job while spooling, bypassing this */
   /* queue processor                                         */
   
   BOOL  EXPENTRY SplQpQueryFlags(PULONG pulFlags);
   
   BOOL  APIENTRY SplQpPrintSeparator( HPROC hproc,
                                      PBYTE pBuf,
                                      ULONG cbBuf,
                                      ULONG ulFlags );
   
   /*************************************************************************
   * There are two definitions governing the QpOpen data block :-
   * 1) the original  QPOPENDATA, which is used as
   *    an array of PSZ and has a list of associated QPDAT defines
   *    for accessing elements of the array. This has been extended
   *    from 6 to 12 elements. The last element is a numeric type
   *    and cannot satisfactorily be accessed via array indexing.
   *
   * 2) a new SQPOPENDATA structure (see below)
   *
   * The old defn has been retained to support existing QProcs.
   *
   *************************************************************************/
   
   /*** Definition for elements within the PQPOPENDATA block ***************/
   #define QPDAT_ADDRESS     0
   #define QPDAT_DRIVER_NAME 1
   #define QPDAT_DRIVER_DATA 2
   #define QPDAT_DATA_TYPE   3
   #define QPDAT_COMMENT     4
   #define QPDAT_PROC_PARAMS 5
   #define QPDAT_SPL_PARAMS  6      /* SplQmOpen Spooler params **       */
   #define QPDAT_NET_PARAMS  7      /* SplQmOpen Network params **       */
   #define QPDAT_DOC_NAME    8      /* SplQmStartDoc name       **       */
   #define QPDAT_QUEUE_NAME  9      /* Queue name for job       **       */
   #define QPDAT_TOKEN      10      /* SplQmOpen token name     **       */
   #define QPDAT_JOBID      11      /* SQM job identity         **       */
   
   typedef struct _SQPOPENDATA       /* SplQpOpenData */
   {
      PSZ       pszLogAddress;     /*                                   */
      PSZ       pszDriverName;     /*                                   */
      PDRIVDATA pdriv;             /*                                   */
      PSZ       pszDataType;       /*                                   */
      PSZ       pszComment;        /*                                   */
      PSZ       pszProcParams;     /*                                   */
      PSZ       pszSpoolParams;    /*                                   */
      PSZ       pszNetworkParams;  /*                                   */
      PSZ       pszDocName;        /*                                   */
      PSZ       pszQueueName;      /*                                   */
      PSZ       pszToken;          /*                                   */
      USHORT    idJobId;           /*                                   */
   } SQPOPENDATA;                   /*                                   */
   typedef SQPOPENDATA *PSQPOPENDATA; /* ptr to SQPOPENDATA */
   
   
   /*************************************************************************
   *** Error information and return codes
   *************************************************************************/
   
   /* Error information for SplMessageBox */
   #define SPLINFO_QPERROR       0x0001
   #define SPLINFO_DDERROR       0x0002
   #define SPLINFO_SPLERROR      0x0004
   #define SPLINFO_OTHERERROR    0x0080
   #define SPLINFO_INFORMATION   0x0100
   #define SPLINFO_WARNING       0x0200
   #define SPLINFO_ERROR         0x0400
   #define SPLINFO_SEVERE        0x0800
   #define SPLINFO_USERINTREQD   0x1000
   
   /* Error Data for SplMessageBox */
   #define SPLDATA_PRINTERJAM    0x0001
   #define SPLDATA_FORMCHGREQD   0x0002
   #define SPLDATA_CARTCHGREQD   0x0004
   #define SPLDATA_PENCHGREQD    0x0008
   #define SPLDATA_DATAERROR     0x0010
   #define SPLDATA_UNEXPECTERROR 0x0020
   #define SPLDATA_OTHER         0x8000
   
   /* return code for fSplStdQueryLength */
   #define SSQL_ERROR (-1L)
   
   #ifdef INCL_SPLERRORS
      #include <pmerr.h>
   #endif /* INCL_SPLERRORS */
   
   typedef unsigned long SPLERR;
   
   #ifdef INCL_SPLDOSPRINT
      /* length for character arrays in structs (excluding zero terminator) */
      #define CNLEN           15             /* Computer name length      */
      #define UNLEN           20             /* Maximum user name length  */
      #define QNLEN           12             /* Queue name maximum length */
      #define PDLEN            8             /* Print destination length  */
      #define DTLEN            9             /* Spool file data type      */
      /* e.g. PM_Q_STD,PM_Q_RAW    */
      #define QP_DATATYPE_SIZE        15     /* returned by SplQpQueryDt  */
      #define DRIV_DEVICENAME_SIZE    31     /* see DRIVDATA struc        */
      #define DRIV_NAME_SIZE           8     /* name of device driver     */
      #define PRINTERNAME_SIZE        32     /* max printer name length   */
      #define FORMNAME_SIZE           31     /* max form name length      */
      #define MAXCOMMENTSZ            48     /* queue comment length      */
      
      typedef struct _PRJINFO      /* prj1 */
      {
         USHORT  uJobId;
         CHAR    szUserName[UNLEN+1];
         CHAR    pad_1;
         CHAR    szNotifyName[CNLEN+1];
         CHAR    szDataType[DTLEN+1];
         PSZ     pszParms;
         USHORT  uPosition;
         USHORT  fsStatus;
         PSZ     pszStatus;
         ULONG   ulSubmitted;
         ULONG   ulSize;
         PSZ     pszComment;
      } PRJINFO;
      typedef PRJINFO *PPRJINFO;
      typedef PRJINFO *NPPRJINFO;
      
      typedef struct _PRJINFO2      /* prj2 */
      {
         USHORT  uJobId;
         USHORT  uPriority;
         PSZ     pszUserName;
         USHORT  uPosition;
         USHORT  fsStatus;
         ULONG   ulSubmitted;
         ULONG   ulSize;
         PSZ     pszComment;
         PSZ     pszDocument;
      } PRJINFO2;
      typedef PRJINFO2 *PPRJINFO2;
      typedef PRJINFO2 *NPPRJINFO2;
      
      typedef struct _PRJINFO3      /* prj */
      {
         USHORT  uJobId;
         USHORT  uPriority;
         PSZ     pszUserName;
         USHORT  uPosition;
         USHORT  fsStatus;
         ULONG   ulSubmitted;
         ULONG   ulSize;
         PSZ     pszComment;
         PSZ     pszDocument;
         PSZ     pszNotifyName;
         PSZ     pszDataType;
         PSZ     pszParms;
         PSZ     pszStatus;
         PSZ     pszQueue;
         PSZ     pszQProcName;
         PSZ     pszQProcParms;
         PSZ     pszDriverName;
         PDRIVDATA pDriverData;
         PSZ     pszPrinterName;
      } PRJINFO3;
      typedef PRJINFO3 *PPRJINFO3;
      typedef PRJINFO3 *NPPRJINFO3;
      
      typedef struct _PRJINFO4      /* prj4 */
      {
         USHORT uJobId;
         USHORT uPriority;
         PSZ    pszUserName;
         USHORT uPosition;
         USHORT fsStatus;
         ULONG  ulSubmitted;
         ULONG  ulSize;
         PSZ    pszComment;
         PSZ    pszDocument;
         PSZ    pszSpoolFileName;
         PSZ    pszPortName;
         PSZ    pszStatus;
         ULONG  ulPagesSpooled;
         ULONG  ulPagesSent;
         ULONG  ulPagesPrinted;
         ULONG  ulTimePrinted;
         ULONG  ulExtendJobStatus;
         ULONG  ulStartPage;
         ULONG  ulEndPage;
      } PRJINFO4;
      typedef PRJINFO4 *PPRJINFO4;
      typedef PRJINFO4 *NPPRJINFO4;
      
      typedef struct _PRDINFO       /* prd1 */
      {
         CHAR    szName[PDLEN+1];
         CHAR    szUserName[UNLEN+1];
         USHORT  uJobId;
         USHORT  fsStatus;
         PSZ     pszStatus;
         USHORT  time;
      } PRDINFO;
      typedef PRDINFO *PPRDINFO;
      typedef PRDINFO *NPPRDINFO;
      
      typedef struct _PRDINFO3      /* prd */
      {
         PSZ     pszPrinterName;
         PSZ     pszUserName;
         PSZ     pszLogAddr;
         USHORT  uJobId;
         USHORT  fsStatus;
         PSZ     pszStatus;
         PSZ     pszComment;
         PSZ     pszDrivers;
         USHORT  time;
         USHORT  usTimeOut;
      } PRDINFO3;
      typedef PRDINFO3 *PPRDINFO3;
      typedef PRDINFO3 *NPPRDINFO3;
      
      typedef struct _PRQINFO      /* prq1 */
      {
         CHAR    szName[QNLEN+1];
         CHAR    pad_1;
         USHORT  uPriority;
         USHORT  uStartTime;
         USHORT  uUntilTime;
         PSZ     pszSepFile;
         PSZ     pszPrProc;
         PSZ     pszDestinations;
         PSZ     pszParms;
         PSZ     pszComment;
         USHORT  fsStatus;
         USHORT  cJobs;
      } PRQINFO;
      typedef PRQINFO *PPRQINFO;
      typedef PRQINFO *NPPRQINFO;
      
      typedef struct _PRQINFO3     /* prq */
      {
         PSZ     pszName;
         USHORT  uPriority;
         USHORT  uStartTime;
         USHORT  uUntilTime;
         USHORT  fsType;
         PSZ     pszSepFile;
         PSZ     pszPrProc;
         PSZ     pszParms;
         PSZ     pszComment;
         USHORT  fsStatus;
         USHORT  cJobs;
         PSZ     pszPrinters;
         PSZ     pszDriverName;
         PDRIVDATA pDriverData;
      } PRQINFO3;
      typedef PRQINFO3 *PPRQINFO3;
      typedef PRQINFO3 *NPPRQINFO3;
      
      typedef struct _PRQINFO6     /* prq6 */
      {
         PSZ     pszName;
         USHORT  uPriority;
         USHORT  uStartTime;
         USHORT  uUntilTime;
         USHORT  fsType;
         PSZ     pszSepFile;
         PSZ     pszPrProc;
         PSZ     pszParms;
         PSZ     pszComment;
         USHORT  fsStatus;
         USHORT  cJobs;
         PSZ     pszPrinters;
         PSZ     pszDriverName;
         PDRIVDATA pDriverData;
         PSZ     pszRemoteComputerName;
         PSZ     pszRemoteQueueName;
      } PRQINFO6;
      typedef PRQINFO6 *PPRQINFO6;
      typedef PRQINFO6 *NPPRQINFO6;
      
      /*
      * structure for DosPrintJobGetId
      */
      typedef struct _PRIDINFO     /* prjid */
      {
         USHORT  uJobId;
         CHAR    szComputerName[CNLEN + 1];
         CHAR    szQueueName[QNLEN+1];
         CHAR    pad_1;
      } PRIDINFO;
      typedef PRIDINFO *PPRIDINFO;
      typedef PRIDINFO *NPPRIDINFO;
      
      /*
      * structure returned by SplQmGetJobID
      */
      typedef struct _QMJOBINFO   /* qmjob */
      {
         ULONG ulJobID;
         PSZ   pszComputerName;
         PSZ   pszQueueName;
      } QMJOBINFO;
      typedef QMJOBINFO *PQMJOBINFO;
      
      /*
      * structure for DosPrintDriverEnum
      */
      typedef struct _PRDRIVINFO    /* prdid */
      {
         CHAR    szDrivName[DRIV_NAME_SIZE+1+DRIV_DEVICENAME_SIZE+1];
      } PRDRIVINFO;
      typedef PRDRIVINFO *PPRDRIVINFO;
      typedef PRDRIVINFO *NPPRDRIVINFO;
      
      /*
      * structures for SplQueryDriver/SplSetDriver level 2
      */
      typedef struct _PRDRIVINFO2    /* prdid2 */
      {
         PSZ     pszPrinterName;
         PSZ     pszDriverName;
         USHORT  usFlags;
         USHORT  cDriverProps; /* Count of DRIVPROPS array that follows */
      } PRDRIVINFO2;
      typedef PRDRIVINFO2 *PPRDRIVINFO2;
      
      typedef struct _DRIVPROPS      /* dprop */
      {
         PSZ     pszKeyName;
         ULONG   cbBuf;
         PVOID   pBuf;
      } DRIVPROPS;
      typedef DRIVPROPS *PDRIVPROPS;
      typedef DRIVPROPS *NPDRIVPROPS;
      
      /*
      * structure for DosPrintQProcessorEnum
      */
      typedef struct _PRQPROCINFO    /* prqp */
      {
         CHAR    szQProcName[QNLEN+1];
      } PRQPROCINFO;
      typedef PRQPROCINFO *PPRQPROCINFO;
      typedef PRQPROCINFO *NPPRQPROCINFO;
      
      /*
      * structure for DosPrintPortEnum Level 0
      */
      typedef struct _PRPORTINFO    /* prpo */
      {
         CHAR    szPortName[PDLEN+1];
      } PRPORTINFO;
      typedef PRPORTINFO *PPRPORTINFO;
      typedef PRPORTINFO *NPPRPORTINFO;
      
      /*
      * structure for DosPrintPortEnum Level 1
      */
      typedef struct _PRPORTINFO1    /* prpo1 */
      {
         PSZ     pszPortName ;
         PSZ     pszPortDriverName ;
         PSZ     pszPortDriverPathName ;
      } PRPORTINFO1;
      typedef PRPORTINFO1 *PPRPORTINFO1;
      typedef PRPORTINFO1 *NPPRPORTINFO1;
      
      /*
      * structure for SplQueryPort/SplEnumPort/SplSetPort Level 2
      */
      typedef struct _PRPORTINFO2   /* prpo2 */
      {
         PSZ     pszPortName ;
         PSZ     pszPortDriver ;
         PSZ     pszProtocolConverter ;
         ULONG   ulReserved;
         ULONG   ulMode ;
         ULONG   ulPriority ;
      } PRPORTINFO2;
      typedef PRPORTINFO2 *PPRPORTINFO2;
      
      /*** Values for ulMode in PRPORTINFO2  *************/
      #define PRPORT_AUTODETECT                1
      #define PRPORT_DISABLE_BIDI              2
      #define PRPORT_ENABLE_BIDI               3
      
      /*** ulVersion values for SplCreatePort ******************************/
      #define SPLPORT_VERSION_REGULAR      0
      #define SPLPORT_VERSION_VIRTUAL      1
      
      /*** Values for parmnum in SplSetPort  *************/
      #define PRPO_PORT_DRIVER                 1
      #define PRPO_PROTOCOL_CNV                2
      #define PRPO_MODE                        3
      #define PRPO_PRIORITY                    4
      
      /*** Values for parmnum in DosPrintQSetInfo/SplSetQueue  *************/
      #define PRQ_PRIORITY_PARMNUM            2
      #define PRQ_STARTTIME_PARMNUM           3
      #define PRQ_UNTILTIME_PARMNUM           4
      #define PRQ_SEPARATOR_PARMNUM           5
      #define PRQ_PROCESSOR_PARMNUM           6
      #define PRQ_DESTINATIONS_PARMNUM        7
      #define PRQ_PARMS_PARMNUM               8
      #define PRQ_COMMENT_PARMNUM             9
      #define PRQ_TYPE_PARMNUM               10
      #define PRQ_PRINTERS_PARMNUM           12
      #define PRQ_DRIVERNAME_PARMNUM         13
      #define PRQ_DRIVERDATA_PARMNUM         14
      #define PRQ_REMOTE_COMPUTER_PARMNUM    15
      #define PRQ_REMOTE_QUEUE_PARMNUM       16
      #define PRQ_MAXPARMNUM                 16
      
      /*** Print Queue Priority ********************************************/
      #define PRQ_MAX_PRIORITY                1        /* highest priority */
      #define PRQ_DEF_PRIORITY                5
      #define PRQ_MIN_PRIORITY                9        /* lowest priority  */
      #define PRQ_NO_PRIORITY                 0
      
      /*** Print queue status bitmask and values for level 1 ***************/
      #define PRQ_STATUS_MASK                 3
      #define PRQ_ACTIVE                      0
      #define PRQ_PAUSED                      1        /* queue is held    */
      #define PRQ_ERROR                       2
      #define PRQ_PENDING                     3        /* pending deletion */
      
      /*** Print queue status bits for level 3 *****************************/
      #define PRQ3_PAUSED                   0x1        /* queue is held    */
      #define PRQ3_PENDING                  0x2        /* pending deletion */
      
      /*** Print queue type bits for level 3 *******************************/
      #define PRQ3_TYPE_RAW         0x0001 /* spools printer-specific data */
      #define PRQ3_TYPE_BYPASS      0x0002 /* allow print while spooling   */
      #define PRQ3_TYPE_APPDEFAULT  0x0004 /* set for Application default Q*/
      
      /*** Values for parmnum in DosPrintJobSetInfo/SplSetJob **************/
      #define PRJ_NOTIFYNAME_PARMNUM        3
      #define PRJ_DATATYPE_PARMNUM          4
      #define PRJ_PARMS_PARMNUM             5
      #define PRJ_POSITION_PARMNUM          6
      #define PRJ_JOBFILEINUSE_PARMNUM      7
      #define PRJ_COMMENT_PARMNUM          11
      #define PRJ_DOCUMENT_PARMNUM         12
      #define PRJ_STATUSCOMMENT_PARMNUM    13
      #define PRJ_PRIORITY_PARMNUM         14
      #define PRJ_PROCPARMS_PARMNUM        16
      #define PRJ_DRIVERDATA_PARMNUM       18
      #define PRJ_SPOOLFILENAME_PARMNUM    19
      #define PRJ_PAGESSPOOLED_PARMNUM     20
      #define PRJ_PAGESSENT_PARMNUM        21
      #define PRJ_PAGESPRINTED_PARMNUM     22
      #define PRJ_TIMEPRINTED_PARMNUM      23
      #define PRJ_EXTENDSTATUS_PARMNUM     24
      #define PRJ_STARTPAGE_PARMNUM        25
      #define PRJ_ENDPAGE_PARMNUM          26
      #define PRJ_MAXPARMNUM               26
      
      /*** Bitmap masks for status field of PRJINFO. ***********************/
      /* 2-7 bits also used in device status */
      #define PRJ_QSTATUS      0x0003      /* Bits 0,1  */
      #define PRJ_DEVSTATUS    0x0ffc      /* 2-11 bits */
      #define PRJ_COMPLETE     0x0004      /* Bit 2     */
      #define PRJ_INTERV       0x0008      /* Bit 3     */
      #define PRJ_ERROR        0x0010      /* Bit 4     */
      #define PRJ_DESTOFFLINE  0x0020      /* Bit 5     */
      #define PRJ_DESTPAUSED   0x0040      /* Bit 6     */
      #define PRJ_NOTIFY       0x0080      /* Bit 7     */
      #define PRJ_DESTNOPAPER  0x0100      /* Bit 8     */
      #define PRJ_DESTFORMCHG  0x0200      /* BIT 9     */
      #define PRJ_DESTCRTCHG   0x0400      /* BIT 10    */
      #define PRJ_DESTPENCHG   0x0800      /* BIT 11    */
      #define PRJ_JOBFILEINUSE 0x4000      /* Bit 14    */
      #define PRJ_DELETED      0x8000      /* Bit 15    */
      
      /*** Bitmap masks for ulExtendedJobStatus field of PRJINFO4. *********/
      #define PRJ4_INPRINTER      0x00000001      /* Bit 0     */
      #define PRJ4_STACKED        0x00000002      /* Bit 1     */
      #define PRJ4_HELDINPRINTER  0x00000004      /* Bit 2     */
      #define PRJ4_JOBSTARTED     0x00000008      /* Bit 3     */
      
      /*** Values of PRJ_QSTATUS bits in fsStatus field of PRJINFO. ********/
      #define PRJ_QS_QUEUED                 0
      #define PRJ_QS_PAUSED                 1
      #define PRJ_QS_SPOOLING               2
      #define PRJ_QS_PRINTING               3
      
      /*** Print Job Priority **********************************************/
      #define PRJ_MAX_PRIORITY             99          /* lowest priority  */
      #define PRJ_MIN_PRIORITY              1          /* highest priority */
      #define PRJ_NO_PRIORITY               0
      
      
      /*** Bitmap masks for status field of PRDINFO. ***********************/
      #define PRD_STATUS_MASK       0x0003      /* Bits 0,1 */
      #define PRD_DEVSTATUS         0x0ffc      /* 2-11 bits */
      
      /*** Values of PRD_STATUS_MASK bits in fsStatus field of PRDINFO. ****/
      #define PRD_ACTIVE                 0
      #define PRD_PAUSED                 1
      
      /*** Control codes used in DosPrintDestControl/SplControlDevice ******/
      #define PRD_DELETE                    0
      #define PRD_PAUSE                     1
      #define PRD_CONT                      2
      #define PRD_RESTART                   3
      
      /*** Values for parmnum in DosPrintDestSetInfo/SplSetDevice **********/
      #define PRD_LOGADDR_PARMNUM      3
      #define PRD_COMMENT_PARMNUM      7
      #define PRD_DRIVERS_PARMNUM      8
      #define PRD_TIMEOUT_PARMNUM      10
      
      /*** structure for SplEnumPrinter ************************************/
      typedef struct _PRINTERINFO     /* prtin */
      {
         ULONG   flType;
         PSZ     pszComputerName;
         PSZ     pszPrintDestinationName;
         PSZ     pszDescription;
         PSZ     pszLocalName;
      } PRINTERINFO ;
      typedef PRINTERINFO *PPRINTERINFO;
      
      /*** Flags for flType in  PRINTERINFO and SplEnumPrinter *************/
      #define    SPL_PR_QUEUE            0x00000001 /* include queues */
      #define    SPL_PR_DIRECT_DEVICE    0x00000002 /* unattached devices */
      #define    SPL_PR_QUEUED_DEVICE    0x00000004 /* queued devices */
      #define    SPL_PR_LOCAL_ONLY       0x00000100 /* exclude remote queues */
      
      
   #endif  /* INCL_SPLDOSPRINT */
   
   /*********************************************************************/
   /*                      FUNCTION PROTOTYPES                          */
   /*                                                                   */
   /* In the case of the DosPrintxxx, the API is presented by new name  */
   /* with the old name in parenthesis.                                 */
   /*********************************************************************/
   
#ifdef __cplusplus
   HSPL APIENTRY SplQmOpen(PCSZ          pszToken,
                           LONG        lCount,
                           PQMOPENDATA pqmdopData);
#else
   HSPL APIENTRY SplQmOpen(PSZ         pszToken,
                           LONG        lCount,
                           PQMOPENDATA pqmdopData);
#endif
   
#ifdef __cplusplus
   BOOL APIENTRY SplQmStartDoc(HSPL  hspl,
                               PCSZ  pszDocName);
#else
   BOOL APIENTRY SplQmStartDoc(HSPL  hspl,
                               PSZ pszDocName);
#endif
   
   BOOL APIENTRY SplQmWrite(HSPL    hspl,
                            LONG  lCount,
                            PVOID pData);
   
   BOOL APIENTRY SplQmEndDoc(HSPL hspl);
   
   BOOL APIENTRY SplQmClose(HSPL hspl);
   
   BOOL APIENTRY SplQmAbort(HSPL hspl);
   
   BOOL APIENTRY SplQmAbortDoc(HSPL hspl);
   
#ifdef __cplusplus
   ULONG  APIENTRY SplMessageBox(PCSZ    pszLogAddr,
                                 ULONG fErrInfo,
                                 ULONG fErrData,
                                 PCSZ    pszText,
                                 PCSZ    pszCaption,
                                 ULONG idWindow,
                                 ULONG fStyle);
#else
   ULONG  APIENTRY SplMessageBox(PSZ   pszLogAddr,
                                 ULONG fErrInfo,
                                 ULONG fErrData,
                                 PSZ   pszText,
                                 PSZ   pszCaption,
                                 ULONG idWindow,
                                 ULONG fStyle);
#endif
   
   BOOL  APIENTRY SplQmNewPage( HSPL hspl,
                               ULONG ulPageNumber );
   ULONG APIENTRY SplQmGetJobID( HSPL hspl,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                PULONG pcbNeeded );
   
   #ifdef INCL_SPLFSE
      /*** Direct Device File System Interface **************************/
#ifdef __cplusplus
      ULONG  APIENTRY PrtOpen(PCSZ     pszDeviceName,
                              PHFILE phDevice,
                              PULONG pActionTaken,
                              ULONG  cbFileSize,
                              ULONG  uFileAttr,
                              ULONG  openFlag,
                              ULONG  openMode,
                              ULONG  reserved);
#else
      ULONG  APIENTRY PrtOpen(PSZ    pszDeviceName,
                              PHFILE phDevice,
                              PULONG pActionTaken,
                              ULONG  cbFileSize,
                              ULONG  uFileAttr,
                              ULONG  openFlag,
                              ULONG  openMode,
                              ULONG  reserved);
#endif
      
      ULONG  APIENTRY PrtClose(HFILE hDevice);
      
      ULONG  APIENTRY PrtWrite(HFILE  hDevice,
                               PVOID  pchData,
                               ULONG  cbData,
                               PULONG pcbWritten);
      
      ULONG  APIENTRY PrtDevIOCtl(PVOID pData,
                                  PVOID pParms,
                                  ULONG uFunction,
                                  ULONG uCategory,
                                  HFILE hDevice);
      
      VOID   APIENTRY PrtAbort(HFILE hDevice);
      ULONG  APIENTRY PrtAbortDoc(HFILE  hFile,
                                  PVOID pchData,
                                  ULONG cbData,
                                  ULONG ulFlags );
      ULONG  APIENTRY PrtResetAbort(HFILE  hFile );
      ULONG  APIENTRY PrtNewPage(HFILE  hFile,
                                 ULONG ulPageNumber );
   #endif /* include File System Emulation functions */
   
   
   #ifdef INCL_SPLBIDI
      #include <pmsplb.h>
   #endif /* INCL_SPLBIDI */
   
   
   BOOL APIENTRY SplStdOpen(HDC hdc);
   
   BOOL APIENTRY SplStdClose(HDC hdc);
   
   BOOL APIENTRY SplStdStart(HDC hdc);
   
   HSTD APIENTRY SplStdStop(HDC hdc);
   
   BOOL APIENTRY SplStdDelete(HSTD hMetaFile);
   
   BOOL APIENTRY SplStdGetBits(HSTD hMetaFile,
                               LONG offData,
                               LONG cbData,
                               PCH  pchData);
   
   LONG APIENTRY SplStdQueryLength(HSTD hMetaFile);
   
   
   /*              SplCreateDevice(DosPrintDestAdd) */
#ifdef __cplusplus
   SPLERR APIENTRY SplCreateDevice(PCSZ    pszComputerName,
                                   ULONG ulLevel,
                                   PVOID pBuf,
                                   ULONG cbBuf);
#else
   SPLERR APIENTRY SplCreateDevice(PSZ   pszComputerName,
                                   ULONG ulLevel,
                                   PVOID pBuf,
                                   ULONG cbBuf);
#endif
   
   /*              SplControlDevice(DosPrintDestControl) */
#ifdef __cplusplus
   SPLERR APIENTRY SplControlDevice(PCSZ    pszComputerName,
                                    PCSZ    pszPortName,
                                    ULONG ulControl);
#else
   SPLERR APIENTRY SplControlDevice(PSZ   pszComputerName,
                                    PSZ   pszPortName,
                                    ULONG ulControl);
#endif
   
   /*              SplDeleteDevice(DosPrintDestDel) */
#ifdef __cplusplus
   SPLERR APIENTRY SplDeleteDevice(PCSZ  pszComputerName,
                                   PCSZ  pszPrintDeviceName);
#else
   SPLERR APIENTRY SplDeleteDevice(PSZ pszComputerName,
                                   PSZ pszPrintDeviceName);
#endif
   
   
   /*              SplEnumDevice(DosPrintDestEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY SplEnumDevice(PCSZ     pszComputerName,
                                 ULONG  ulLevel,
                                 PVOID  pBuf,
                                 ULONG  cbBuf,
                                 PULONG pcReturned,
                                 PULONG pcTotal,
                                 PULONG pcbNeeded,
                                 PVOID  pReserved);
#else
   SPLERR APIENTRY SplEnumDevice(PSZ    pszComputerName,
                                 ULONG  ulLevel,
                                 PVOID  pBuf,
                                 ULONG  cbBuf,
                                 PULONG pcReturned,
                                 PULONG pcTotal,
                                 PULONG pcbNeeded,
                                 PVOID  pReserved);
#endif
   
   /*              SplQueryDevice(DosPrintDestGetInfo) */
#ifdef __cplusplus
   SPLERR APIENTRY SplQueryDevice(PCSZ     pszComputerName,
                                  PCSZ     pszPrintDeviceName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#else
   SPLERR APIENTRY SplQueryDevice(PSZ    pszComputerName,
                                  PSZ    pszPrintDeviceName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#endif
   
   /*              SplSetDevice(DosPrintDestSetInfo) */
#ifdef __cplusplus
   SPLERR APIENTRY SplSetDevice(PCSZ    pszComputerName,
                                PCSZ    pszPrintDeviceName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#else
   SPLERR APIENTRY SplSetDevice(PSZ   pszComputerName,
                                PSZ   pszPrintDeviceName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY SplCopyJob(PCSZ     pszSrcComputerName,
                              PCSZ     pszSrcQueueName,
                              ULONG  ulSrcJob,
                              PCSZ     pszTrgComputerName,
                              PCSZ     pszTrgQueueName,
                              PULONG pulTrgJob);
#else
   SPLERR APIENTRY SplCopyJob(PSZ    pszSrcComputerName,
                              PSZ    pszSrcQueueName,
                              ULONG  ulSrcJob,
                              PSZ    pszTrgComputerName,
                              PSZ    pszTrgQueueName,
                              PULONG pulTrgJob);
#endif
   
   /*              SplDeleteJob(DosPrintJobDel) */
#ifdef __cplusplus
   SPLERR APIENTRY SplDeleteJob(PCSZ    pszComputerName,
                                PCSZ    pszQueueName,
                                ULONG ulJob);
#else
   SPLERR APIENTRY SplDeleteJob(PSZ   pszComputerName,
                                PSZ   pszQueueName,
                                ULONG ulJob);
#endif
   
   /*              SplEnumJob(DosPrintJobEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY SplEnumJob(PCSZ     pszComputerName,
                              PCSZ     pszQueueName,
                              ULONG  ulLevel,
                              PVOID  pBuf,
                              ULONG  cbBuf,
                              PULONG pcReturned,
                              PULONG pcTotal,
                              PULONG pcbNeeded,
                              PVOID  pReserved);
#else
   SPLERR APIENTRY SplEnumJob(PSZ    pszComputerName,
                              PSZ    pszQueueName,
                              ULONG  ulLevel,
                              PVOID  pBuf,
                              ULONG  cbBuf,
                              PULONG pcReturned,
                              PULONG pcTotal,
                              PULONG pcbNeeded,
                              PVOID  pReserved);
#endif
   
   /*              SplHoldJob(DosPrintJobPause)   */
#ifdef __cplusplus
   SPLERR APIENTRY SplHoldJob(PCSZ    pszComputerName,
                              PCSZ    pszQueueName,
                              ULONG ulJob);
#else
   SPLERR APIENTRY SplHoldJob(PSZ   pszComputerName,
                              PSZ   pszQueueName,
                              ULONG ulJob);
#endif
   
   /*              SplQueryJob(DosPrintJobGetInfo)  */
#ifdef __cplusplus
   SPLERR APIENTRY SplQueryJob(PCSZ     pszComputerName,
                               PCSZ     pszQueueName,
                               ULONG  ulJob,
                               ULONG  ulLevel,
                               PVOID  pBuf,
                               ULONG  cbBuf,
                               PULONG pcbNeeded);
#else
   SPLERR APIENTRY SplQueryJob(PSZ    pszComputerName,
                               PSZ    pszQueueName,
                               ULONG  ulJob,
                               ULONG  ulLevel,
                               PVOID  pBuf,
                               ULONG  cbBuf,
                               PULONG pcbNeeded);
#endif
   
   /*              SplReleaseJob(DosPrintJobContinue) */
#ifdef __cplusplus
   SPLERR APIENTRY SplReleaseJob(PCSZ    pszComputerName,
                                 PCSZ    pszQueueName,
                                 ULONG ulJob);
#else
   SPLERR APIENTRY SplReleaseJob(PSZ   pszComputerName,
                                 PSZ   pszQueueName,
                                 ULONG ulJob);
#endif
   
   /*              SplSetJob(DosPrintJobSetInfo)*/
#ifdef __cplusplus
   SPLERR APIENTRY SplSetJob(PCSZ    pszComputerName,
                             PCSZ    pszQueueName,
                             ULONG ulJob,
                             ULONG ulLevel,
                             PVOID pBuf,
                             ULONG cbBuf,
                             ULONG ulParmNum);
#else
   SPLERR APIENTRY SplSetJob(PSZ   pszComputerName,
                             PSZ   pszQueueName,
                             ULONG ulJob,
                             ULONG ulLevel,
                             PVOID pBuf,
                             ULONG cbBuf,
                             ULONG ulParmNum);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY SplCreatePort(PCSZ      pszComputerName,
                                 PCSZ      pszPortName,
                                 PCSZ      pszPortDriver,
                                 ULONG   ulVersion,
                                 PVOID   pBuf,
                                 ULONG   cbBuf );
#else
   SPLERR APIENTRY SplCreatePort(PSZ     pszComputerName,
                                 PSZ     pszPortName,
                                 PSZ     pszPortDriver,
                                 ULONG   ulVersion,
                                 PVOID   pBuf,
                                 ULONG   cbBuf );
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY SplDeletePort(PCSZ      pszComputerName,
                                 PCSZ      pszPortName );
#else
   SPLERR APIENTRY SplDeletePort(PSZ     pszComputerName,
                                 PSZ     pszPortName );
#endif
   
   /*               SplEnumPort(DosPrintPortEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY  SplEnumPort(PCSZ     pszComputerName,
                                ULONG  ulLevel,
                                PVOID  pBuf,
                                ULONG  cbBuf,
                                PULONG pcReturned,
                                PULONG pcTotal,
                                PULONG pcbNeeded,
                                PVOID  pReserved);
#else
   SPLERR APIENTRY  SplEnumPort(PSZ    pszComputerName,
                                ULONG  ulLevel,
                                PVOID  pBuf,
                                ULONG  cbBuf,
                                PULONG pcReturned,
                                PULONG pcTotal,
                                PULONG pcbNeeded,
                                PVOID  pReserved);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY  SplQueryPort(PCSZ      pszComputerName,
                                 PCSZ      pszPortName,
                                 ULONG   ulLevel,
                                 PVOID   pBuf,
                                 ULONG   cbBuf,
                                 PULONG  pcbNeeded);
#else
   SPLERR APIENTRY  SplQueryPort(PSZ     pszComputerName,
                                 PSZ     pszPortName,
                                 ULONG   ulLevel,
                                 PVOID   pBuf,
                                 ULONG   cbBuf,
                                 PULONG  pcbNeeded);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY  SplSetPort(PCSZ     pszComputerName,
                               PCSZ     pszPortName,
                               ULONG  ulLevel,
                               PVOID  pBuf,
                               ULONG  cbBuf,
                               ULONG  ulParmNum);
#else
   SPLERR APIENTRY  SplSetPort(PSZ    pszComputerName,
                               PSZ    pszPortName,
                               ULONG  ulLevel,
                               PVOID  pBuf,
                               ULONG  cbBuf,
                               ULONG  ulParmNum);
#endif
   
   /*              SplCreateQueue(DosPrintQAdd) */
#ifdef __cplusplus
   SPLERR APIENTRY SplCreateQueue(PCSZ    pszComputerName,
                                  ULONG ulLevel,
                                  PVOID pBuf,
                                  ULONG cbBuf);
#else
   SPLERR APIENTRY SplCreateQueue(PSZ   pszComputerName,
                                  ULONG ulLevel,
                                  PVOID pBuf,
                                  ULONG cbBuf);
#endif
   
   /*              SplReleaseQueue(DosPrintQContinue) */
#ifdef __cplusplus
   SPLERR APIENTRY SplReleaseQueue(PCSZ  pszComputerName,
                                   PCSZ  pszQueueName);
#else
   SPLERR APIENTRY SplReleaseQueue(PSZ pszComputerName,
                                   PSZ pszQueueName);
#endif
   
   
   /*              SplDeleteQueue(DosPrintQDel) */
#ifdef __cplusplus
   SPLERR APIENTRY SplDeleteQueue(PCSZ  pszComputerName,
                                  PCSZ  pszQueueName);
#else
   SPLERR APIENTRY SplDeleteQueue(PSZ pszComputerName,
                                  PSZ pszQueueName);
#endif
   
   
   /*              SplEnumQueue(DosPrintQEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY SplEnumQueue(PCSZ     pszComputerName,
                                ULONG  ulLevel,
                                PVOID  pBuf,
                                ULONG  cbBuf,
                                PULONG pcReturned,
                                PULONG pcTotal,
                                PULONG pcbNeeded,
                                PVOID  pReserved);
#else
   SPLERR APIENTRY SplEnumQueue(PSZ    pszComputerName,
                                ULONG  ulLevel,
                                PVOID  pBuf,
                                ULONG  cbBuf,
                                PULONG pcReturned,
                                PULONG pcTotal,
                                PULONG pcbNeeded,
                                PVOID  pReserved);
#endif
   
   /*               SplQueryQueue(DosPrintQGetInfo) */
#ifdef __cplusplus
   SPLERR APIENTRY  SplQueryQueue(PCSZ     pszComputerName,
                                  PCSZ     pszQueueName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#else
   SPLERR APIENTRY  SplQueryQueue(PSZ    pszComputerName,
                                  PSZ    pszQueueName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#endif
   
   /*              SplHoldQueue(DosPrintQPause) */
#ifdef __cplusplus
   SPLERR APIENTRY SplHoldQueue(PCSZ  pszComputerName,
                                PCSZ  pszQueueName);
#else
   SPLERR APIENTRY SplHoldQueue(PSZ pszComputerName,
                                PSZ pszQueueName);
#endif
   
   
   /*              SplPurgeQueue(DosPrintQPurge) */
#ifdef __cplusplus
   SPLERR APIENTRY SplPurgeQueue(PCSZ  pszComputerName,
                                 PCSZ  pszQueueName);
#else
   SPLERR APIENTRY SplPurgeQueue(PSZ pszComputerName,
                                 PSZ pszQueueName);
#endif
   
   
   /*               SplSetQueue(DosPrintQSetInfo) */
#ifdef __cplusplus
   SPLERR APIENTRY  SplSetQueue(PCSZ    pszComputerName,
                                PCSZ    pszQueueName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#else
   SPLERR APIENTRY  SplSetQueue(PSZ   pszComputerName,
                                PSZ   pszQueueName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#endif
   
   /*               SplEnumDriver(DosPrintDriverEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY  SplEnumDriver(PCSZ     pszComputerName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcReturned,
                                  PULONG pcTotal,
                                  PULONG pcbNeeded,
                                  PVOID  pReserved);
#else
   SPLERR APIENTRY  SplEnumDriver(PSZ    pszComputerName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcReturned,
                                  PULONG pcTotal,
                                  PULONG pcbNeeded,
                                  PVOID  pReserved);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY SplQueryDriver(PCSZ     pszComputerName,
                                  PCSZ     pszDriverName,
                                  PCSZ     pszPrinterName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#else
   SPLERR APIENTRY SplQueryDriver(PSZ    pszComputerName,
                                  PSZ    pszDriverName,
                                  PSZ    pszPrinterName,
                                  ULONG  ulLevel,
                                  PVOID  pBuf,
                                  ULONG  cbBuf,
                                  PULONG pcbNeeded);
#endif
   
#ifdef __cplusplus
   SPLERR APIENTRY SplSetDriver(PCSZ    pszComputerName,
                                PCSZ    pszDriverName,
                                PCSZ    pszPrinterName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#else
   SPLERR APIENTRY SplSetDriver(PSZ   pszComputerName,
                                PSZ   pszDriverName,
                                PSZ   pszPrinterName,
                                ULONG ulLevel,
                                PVOID pBuf,
                                ULONG cbBuf,
                                ULONG ulParmNum);
#endif
   
   /*               SplEnumQueueProcessor(DosPrintQProcessorEnum) */
#ifdef __cplusplus
   SPLERR APIENTRY  SplEnumQueueProcessor(PCSZ     pszComputerName,
                                          ULONG  ulLevel,
                                          PVOID  pBuf,
                                          ULONG  cbBuf,
                                          PULONG pcReturned,
                                          PULONG pcTotal,
                                          PULONG pcbNeeded,
                                          PVOID  pReserved);
#else
   SPLERR APIENTRY  SplEnumQueueProcessor(PSZ    pszComputerName,
                                          ULONG  ulLevel,
                                          PVOID  pBuf,
                                          ULONG  cbBuf,
                                          PULONG pcReturned,
                                          PULONG pcTotal,
                                          PULONG pcbNeeded,
                                          PVOID  pReserved);
#endif
   
   
#ifdef __cplusplus
   SPLERR APIENTRY SplEnumPrinter(PCSZ     pszComputerName,
                                  ULONG  uLevel,
                                  ULONG  flType,
                                  PVOID  pBuf,
                                  ULONG  cbbuf,
                                  PULONG pcReturned,
                                  PULONG pcTotal,
                                  PULONG pcbNeeded,
                                  PVOID  pReserved);
#else
   SPLERR APIENTRY SplEnumPrinter(PSZ    pszComputerName,
                                  ULONG  uLevel,
                                  ULONG  flType,
                                  PVOID  pBuf,
                                  ULONG  cbbuf,
                                  PULONG pcReturned,
                                  PULONG pcTotal,
                                  PULONG pcbNeeded,
                                  PVOID  pReserved);
#endif
   
   
   
   /* XLATOFF */
#endif
/* XLATON */

/* XLATOFF */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( restore )
   #endif
   #pragma info( restore )
#endif
/* XLATON */

/**************************** end of file **********************************/
