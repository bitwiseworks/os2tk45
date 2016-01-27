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
 *  About this file ...  SERVICE.H                                  *
 *                                                                  *
 *  This file contains information about the NetService APIs.       *
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


#ifndef NETSERVICE_INCLUDED

#define NETSERVICE_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif

/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetServiceControl ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszService,
                      unsigned char               fbOpCode,
                      unsigned char               fbArg,
                      unsigned char       LSFAR * pbBuffer,
                      unsigned short              cbBuffer );

extern API_FUNCTION
  NetServiceEnum ( const unsigned char LSFAR * pszServer,
                   short                       sLevel,
                   unsigned char       LSFAR * pbBuffer,
                   unsigned short              cbBuffer,
                   unsigned short      LSFAR * pcEntriesRead,
                   unsigned short      LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetServiceGetInfo ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszService,
                      short                       sLevel,
                      unsigned char       LSFAR * pbBuffer,
                      unsigned short              cbBuffer,
                      unsigned short      LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetServiceInstall ( const unsigned char LSFAR * pszServer,
                      const unsigned char LSFAR * pszService,
                      const unsigned char LSFAR * pszCmdArgs,
                      unsigned char       LSFAR * pbBuffer,
                      unsigned short              cbBuffer );

extern API_FUNCTION
  NetServiceStatus ( const unsigned char LSFAR * pbBuffer,
                     unsigned short              cbBuffer );


/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/


struct service_info_0 {
    unsigned char   svci0_name[SNLEN+1]; /* service name                  */
};      /* service_info_0 */

struct service_info_1 {
    unsigned char   svci1_name[SNLEN+1]; /* service name                  */
    unsigned short  svci1_status;        /* See status values below       */
    unsigned long   svci1_code;          /* install code of service       */
    unsigned short  svci1_pid;           /* pid of service program        */
};      /* service_info_1 */

struct service_info_2 {
    unsigned char   svci2_name[SNLEN+1];   /* service name                  */
    unsigned short  svci2_status;          /* See status values below       */
    unsigned long   svci2_code;            /* install code of service       */
    unsigned short  svci2_pid;             /* pid of service program        */
    unsigned char   svci2_text[STXTLEN+1]; /* text area for use by services */
};      /* service_info_2 */

struct service_status {
    unsigned short  svcs_status;          /* See status values below         */
    unsigned long   svcs_code;            /* install code of service         */
    unsigned short  svcs_pid;             /* pid of service program          */
    unsigned char   svcs_text[STXTLEN+1]; /* text area for use by services   */
};      /* service_status */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *             for "pure" 32-bit code                           *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32ServiceControl ( const unsigned char * pszServer,
                        const unsigned char * pszService,
                        unsigned short        usOpcode,     /* was char    */
                        unsigned short        usArg,        /* was char    */
                        unsigned char       * pbBuffer,
                        unsigned long         ulBuffer );   /* was USHORT  */

extern API32_FUNCTION
  Net32ServiceEnum ( const unsigned char  * pszServer,
                     unsigned long          ulLevel,        /* was short */
                     unsigned char        * pbBuffer,
                     unsigned long          ulBuffer,       /* was USHORT  */
                     unsigned long        * pulEntriesRead, /* was USHORT  */
                     unsigned long        * pulTotalAvail );/* was PUSHORT */

extern API32_FUNCTION
  Net32ServiceGetInfo ( const unsigned char * pszServer,
                        const unsigned char * pszService,
                        unsigned long         ulLevel,      /* was short */
                        unsigned char       * pbBuffer,
                        unsigned long         ulBuffer,        /* was USHORT  */
                        unsigned long       * pulTotalAvail ); /* was PUSHORT */

extern API32_FUNCTION
  Net32ServiceInstall ( const unsigned char  * pszServer,
                        const unsigned char  * pszService,
                        const unsigned char  * pszCmdArgs,
                        unsigned char        * pbBuffer,
                        unsigned long          ulBuffer ); /* was USHORT */

extern API32_FUNCTION
  Net32ServiceStatus ( const unsigned char * pbBuffer,
                       unsigned long         ulBuffer );   /* was USHORT */



/****************************************************************
 *                                                              *
 *          Data structure templates, PURE_32 only              *
 *                                                              *
 ****************************************************************/

/* Just for safety, define HEV. */
#ifndef __HEV__
typedef ULONG HEV;
#endif
struct service_ctrl_info_0 {
    unsigned short  svc_ctrl0_opcode;      /* service control operation     */
    unsigned short  svc_ctrl0_arg;         /* Additional control data       */
    HEV             svc_ctrl0_sem_handle;  /* Sem handle for control loop   */
};      /* service_ctrl_info_0 */

struct service_status_1 {
    unsigned short  svcs_status;          /* Same as in service_status       */
    unsigned long   svcs_code;            /* Same as in service_status       */
    unsigned short  svcs_pid;             /* Same as in service_status       */
    unsigned char   svcs_text[STXTLEN+1]; /* Same as in service_status       */
    unsigned long   svcs_flags;           /* Flags for service control, etc. */
};      /* service_status_1 */
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/

/*
 *      SERVICE_RCV_SIG_FLAG is the value passed to DosSetSigHandler when
 *      installing the handler within a service, to receive control
 *      signals.
 */

#define SERVICE_RCV_SIG_FLAG 5

/*
 *      Flags for the new 32-bit services
 */
#define SERVICE_CTRL_USES_SEMAPHORE    1

/*
 *      Bitmask and bit values for svci1_status, svci2_status, and
 *      svcs_status fields.  For each "subfield", there is a mask
 *      defined, and a number of constants representing the value
 *      obtained by doing (status & mask).
 */

/* Bits 0,1 -- general status */

#define SERVICE_INSTALL_STATE           0x03
#define SERVICE_UNINSTALLED             0x00
#define SERVICE_INSTALL_PENDING         0x01
#define SERVICE_UNINSTALL_PENDING       0x02
#define SERVICE_INSTALLED               0x03

/* Bits 2,3 -- paused/active status */

#define SERVICE_PAUSE_STATE             0x0C
#define SERVICE_ACTIVE                  0x00
#define SERVICE_CONTINUE_PENDING        0x04
#define SERVICE_PAUSE_PENDING           0x08
#define SERVICE_PAUSED                  0x0C

/* Bit 4 -- uninstallable indication */

#define SERVICE_NOT_UNINSTALLABLE       0x00
#define SERVICE_UNINSTALLABLE           0x10

/* Bit 5 -- pausable indication */

#define SERVICE_NOT_PAUSABLE            0x00
#define SERVICE_PAUSABLE                0x20

/* Requester service only:
 * Bits 8,9,10 -- redirection paused/active */

#define SERVICE_REDIR_PAUSED            0x700
#define SERVICE_REDIR_DISK_PAUSED       0x100
#define SERVICE_REDIR_PRINT_PAUSED      0x200
#define SERVICE_REDIR_COMM_PAUSED       0x400

/*
 *      Standard LAN Manager service names.
 */

#define SERVICE_WORKSTATION     "REQUESTER"
#define SERVICE_SERVER          "SERVER"
#define SERVICE_MESSENGER       "MESSENGER"
#define SERVICE_NETRUN          "NETRUN"
#define SERVICE_ALERTER         "ALERTER"
#define SERVICE_NETLOGON        "NETLOGON"
#define SERVICE_NETPOPUP        "NETPOPUP"
#define SERVICE_REPL            "REPLICATOR"
#define SERVICE_DCDBREPL        "DCDBREPL"

/*
 *      Additional standard LAN Manager for MS-DOS services
 */

#define SERVICE_DOS_ENCRYPTION  "ENCRYPT"


/*
 *      NetServiceControl opcodes.
 */

#define SERVICE_CTRL_INTERROGATE        0
#define SERVICE_CTRL_PAUSE              1
#define SERVICE_CTRL_CONTINUE           2
#define SERVICE_CTRL_UNINSTALL          3

/*
 *      Requester service only:  Bits used in the "arg" parameter
 *      to NetServiceControl in conjunction with the opcode
 *      SERVICE_CTRL_PAUSE or SERVICE_CTRL_CONTINUE, to pause or
 *      continue redirection.
 */

#define SERVICE_CTRL_REDIR_DISK         0x1
#define SERVICE_CTRL_REDIR_PRINT        0x2
#define SERVICE_CTRL_REDIR_COMM         0x4


/*
 *      Values for svci1_code, svci2_code, and svcs_code when
 *      status of the service is SERVICE_INSTALL_PENDING or
 *      SERVICE_UNINSTALL_PENDING.
 *      A service can optionally provide a hint to the installer
 *      that the install is proceeding and how long to wait
 *      (in 0.1 second increments) before querying status again.
 */

#define SERVICE_IP_NO_HINT              0x0
#define SERVICE_CCP_NO_HINT             0x0

#define SERVICE_IP_QUERY_HINT           0x10000
#define SERVICE_CCP_QUERY_HINT          0x10000

        /* Mask for install proceeding checkpoint number */
#define SERVICE_IP_CHKPT_NUM            0x0FF
#define SERVICE_CCP_CHKPT_NUM           0x0FF

        /* Mask for wait time hint before querying again */
#define SERVICE_IP_WAIT_TIME            0x0FF00
#define SERVICE_CCP_WAIT_TIME           0x0FF00

        /* Shift count for building wait time _code values */
#define SERVICE_IP_WAITTIME_SHIFT       8



#define SERVICE_IP_CODE(tt,nn) \
  ((long)SERVICE_IP_QUERY_HINT|(long)(nn|(tt<<SERVICE_IP_WAITTIME_SHIFT)))

#define SERVICE_CCP_CODE(tt,nn) \
  ((long)SERVICE_CCP_QUERY_HINT|(long)(nn|(tt<<SERVICE_IP_WAITTIME_SHIFT)))

#define SERVICE_UIC_CODE(cc,mm) \
  ((long)(((long)cc<<16)|(long)(unsigned)mm))

/***    SERVICE_BASE is the base of service error codes,
 *      chosen to avoid conflict with OS, redirector,
 *      netapi, and errlog codes.
 */

#define SERVICE_BASE    3050




/*  Uninstall codes, to be used in high byte of 'code' on final NetStatus,
 *  which sets the status to UNINSTALLED
 */

#define SERVICE_UIC_NORMAL      0
#define SERVICE_UIC_BADPARMVAL  (SERVICE_BASE + 1) \
        /* IBMLAN.INI or the command line has an illegal value for "%1". */
        /* A IBMLAN.INI entry or what you just typed includes an illegal value for "%1". */
#define SERVICE_UIC_MISSPARM    (SERVICE_BASE + 2) \
        /* The required parameter %1 was not provided on the command line or in IBMLAN.INI. */
#define SERVICE_UIC_UNKPARM     (SERVICE_BASE + 3) \
        /* LAN Manager doesn't recognize "%1" as a valid option.  */
#define SERVICE_UIC_RESOURCE    (SERVICE_BASE + 4) /* A request for %1 resources could not be satisfied. */
#define SERVICE_UIC_CONFIG      (SERVICE_BASE + 5) /* A problem exists with the system configuration:  %1. */
#define SERVICE_UIC_SYSTEM      (SERVICE_BASE + 6) /* A system error has occurred. */
#define SERVICE_UIC_INTERNAL    (SERVICE_BASE + 7) /* An internal consistency error has occurred. */
#define SERVICE_UIC_AMBIGPARM   (SERVICE_BASE + 8) \
        /* IBMLAN.INI or the command line has an ambiguous option: %1. */
#define SERVICE_UIC_DUPPARM     (SERVICE_BASE + 9) \
        /* IBMLAN.INI or the command line has a duplicate parameter: %1. */
#define SERVICE_UIC_KILL        (SERVICE_BASE + 10) \
        /* The service did not respond to control and was stopped with the DosKillProc function.*/
#define SERVICE_UIC_EXEC        (SERVICE_BASE + 11) \
        /* An error occurred when attempting to run the service program. */
#define SERVICE_UIC_SUBSERV     (SERVICE_BASE + 12) \
        /* The sub-service %1 failed to start. */
#define SERVICE_UIC_CONFLPARM   (SERVICE_BASE + 13) \
        /* There is a conflict in the value or use of these options: %1 */
#define SERVICE_UIC_FILE        (SERVICE_BASE + 14) \
        /* There is a problem with the file %1.  %2*/


/***
 *      The modifiers
 */

/*  General: */
#define SERVICE_UIC_M_NULL      0

/*  BADPARMVAL:  A text string in service_info_2.text */
/*  MISSPARM:  ditto */
/*  UNKPARM:  ditto */
/*  AMBIGPARM:  ditto */
/*  DUPPARM:  ditto */
/*  SUBSERV:  ditto */

/* CONFIG: */
/*  see also 3074 to 3091 */
#define SERVICE_UIC_ADDGR_FAIL  (SERVICE_BASE + 15) /* @E
   NCBAddGroupName failed%0         */
#define SERVICE_UIC_NO_STANDAL  (SERVICE_BASE + 16) /* @E
   Server can not be standalone%0   */
#define SERVICE_UIC_NUMMAILSLOT (SERVICE_BASE + 17) /* @E
   Number of Mailslots must be greater than zero%0 */
/*  RESOURCE: */
#define SERVICE_UIC_M_MEMORY    (SERVICE_BASE + 20) /* @I
        *memory%0 */
#define SERVICE_UIC_M_DISK      (SERVICE_BASE + 21) /* @I
        *disk space%0 */
#define SERVICE_UIC_M_THREADS   (SERVICE_BASE + 22) /* @I
        *thread%0 */
#define SERVICE_UIC_M_PROCESSES (SERVICE_BASE + 23) /* @I
        *process%0 */

/*  CONFIG: */
/* 3065 to 3070 is also config error range */
#define SERVICE_UIC_M_SECURITY  (SERVICE_BASE + 24) /* @I
        *Security failure%0 */

#define SERVICE_UIC_M_SEC_FILE_ERR (SERVICE_BASE + 28) /* @I
        *The server cannot access the user accounts database (NET.ACC)%0 */
#define SERVICE_UIC_M_FILES     (SERVICE_BASE + 29) /* @I
        *There are incompatible files installed in the LANMAN tree%0 */
#define SERVICE_UIC_M_LOGS      (SERVICE_BASE + 30) /* @I
        *The LANMAN\LOGS directory is invalid%0 */
#define SERVICE_UIC_M_LANGROUP  (SERVICE_BASE + 31) /* @I
        *The domain specified could not be used%0 */
#define SERVICE_UIC_M_MSGNAME   (SERVICE_BASE + 32) /* @I
        *The computername is being used as a message alias on another computer%0 */
#define SERVICE_UIC_M_UAS       (SERVICE_BASE + 34) /* @I
        *The user accounts system isn't configured correctly%0 */
#define SERVICE_UIC_M_SERVER_SEC_ERR (SERVICE_BASE + 35) /* @I
        *The server isn't running with user-level security%0 */
#define SERVICE_UIC_M_WKSTA     (SERVICE_BASE + 37) /* @I
        *The workstation is not configured appropriately%0 */
#define SERVICE_UIC_M_ERRLOG    (SERVICE_BASE + 38) /* @I
        *View your error log for details%0 */
#define SERVICE_UIC_M_ADDPAK    (SERVICE_BASE + 40) /* @I
        *ADDPAK file is corrupt.  Delete LANMAN\NETPROG\ADDPAK.SER and reapply all ADDPAKs%0 */
#define SERVICE_UIC_M_LAZY      (SERVICE_BASE + 41) \
        /* The LM386 server cannot be started because CACHE.EXE is not running%0 */


/* SYSTEM:  A DOS or NET error number */
/* INTERNAL:  None */
/* KILL:  None */
/* EXEC:  None */

/***
 *      End modifiers
 */


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETSERVICE_INCLUDED */
