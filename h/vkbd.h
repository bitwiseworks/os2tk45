/*static char *SCCSID = "@(#)vkbd.h     6.3 92/02/14";*/
/***    VKBD.H
 *
 *
 *      MVDM Virtual Keyboard Exported Definitions
 *      Copyright (c) 1992 IBM Corporation
 *
 *      For include file hierarchy, see MVDM.H
 *
 *
 */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */



#ifndef __VKBD__
#define __VKBD__


#ifndef INCL_NONE               /* include everything by default */
#define INCL_VKBD
#endif

#ifdef  INCL_VKBD
#define INCL_VKBDSYSREQ
#define INCL_VKBDINTERFACES
#endif



#ifdef  INCL_VKBDSYSREQ

/***    VKBD constants
 */

#define VKBD_NAME               "VKBD$"

/* VKBD functions for use by PMVDMP
 */
#ifdef  INCL_NLSSHLD

#define NIP_PMINPUTMETHOD   0x0000  // Current Input method is of PM
#define NIP_FSINPUTMETHOD   0x0001  // Current Input method is of FS
#define NIP_PS55            0x0000  // PS/55 BIOS : bit 2-1 : 0 0
#define NIP_PS2             0x0002  // PS/2  BIOS :           0 1
                                    // PS/55 GALE :(reserved) 1 0
                                    // Reserved   :           1 1
#endif  //INCL_NLSSHLD

#define VKBDSYSREQ_SETACCESS    1   /* Obtain exclusive access for VDM    */
#define VKBDSYSREQ_SETFOCUS     2   /* Notify VDM is gaining/losing focus */
#define VKBDSYSREQ_POSTSCAN     3   /* Send scan code (typed)             */
#define VKBDSYSREQ_POSTCHAR     4   /* Send character code (pasted)       */
#define VKBDSYSREQ_MONREGIST    5   // Register Monitor into VKBD
#define VKBDSYSREQ_MONDEREGIST  6   // Deregister Monitor from VKBD
#define VKBDSYSREQ_MONREAD      7   // Read key-stroke
#define VKBDSYSREQ_MONWRITE     8   // Write key-stroke
#define VKBDSYSREQ_SETSHIFT     9   // Set Keyboard shift status
#define VKBDSYSREQ_GETSHIFT    10   // Get Keyboard shift status
#define VKBDSYSREQ_REGWVDM     11   // Register PMFEP into VKBD
#define VKBDSYSREQ_DEREGWVDM   12   // Deregister PMFEP from VKBD
#define VKBDSYSREQ_READWVDM    13   // Read key-stroke
#define VKBDSYSREQ_WRITEWVDM   14   // Write key-stroke
#define VKBDSYSREQ_MAX         14


/***    VKBD data types
 */

typedef struct keypkt_s {           /* kp (for VKBDSYSREQ_POSTSCAN/CHAR) */
    UCHAR       kp_Scan;            /* scan code                         */
    UCHAR       kp_Char;            /* translated char                   */
    USHORT      kp_fsState;         /* shift states                      */
    USHORT      kp_fsDDFlags;       /* translated flags                  */
    USHORT      kp_fsKey;           /* key flags                         */
} KEYPKT;
typedef KEYPKT *PKEYPKT;
typedef PKEYPKT *PPKEYPKT;

typedef struct shiftpkt_s {         /* sp (for VKBDSYSREQ_SETFOCUS)            */
    USHORT      fSetFocus;          /* TRUE to set focus, FALSE to clear focus */
    USHORT      fsShift;            /* PM shift states during set focus        */
                                    /*   undefined during clear focus          */
} SHIFTPKT;
typedef SHIFTPKT *PSHIFTPKT;
typedef PSHIFTPKT *PPSHIFTPKT;

#define VKBDEVENT_CREATE        0
#define VKBDEVENT_TERM          1

#define VKBDEVENT_KEY           2
#define VKBDEVENT_SHIFT         3
#define VKBDEVENT_DOS_SHIFT     4

#define VKBDEVENT_CHG_ENV       5

#define VKBD_KKC_DOS            0x0000
#define VKBD_KKC_SHARED         0x0001

#define VKBD_VDMIMFEP_MODE      0x0000
#define VKBD_VDMPMFEP_MODE      0x0002

#define VKBD_REG_VDMIMFEP       0x0001
#define VKBD_REG_VDMPMFEP       0x0002

#define VKBD_US_BIOS_MODE       0x8000
#define VKBD_JP_BIOS_DOSV       0x0002    /* 1 means Environment is DOSV */

#define VKBD_ENV_TO_FULLSCREEN  0
#define VKBD_ENV_TO_WINDOWED    1

typedef struct vkevent_s {          /* ve (for DBCS SystemReq)  */
        USHORT  ve_event_id;
        SGID    ve_sgid;
        UCHAR   ve_monitor_flag;
        UCHAR   ve_r_scancode;
        UCHAR   ve_charcode;
        UCHAR   ve_scancode;
        UCHAR   ve_nlsstate;
        UCHAR   ve_nlsshift;
        USHORT  ve_shift;
        ULONG   ve_timestamp;
        USHORT  ve_DDFlags;
} VKEVENT;

typedef VKEVENT *PVKEVENT;

#ifdef  DOSK
#define DBCS_INPUT_METHOD_LIST  "DOS_FEP\0"
#else
#define DBCS_INPUT_METHOD_LIST  "OS2_FS_FEP\0DOS_FEP\0"
#endif  //DOSK


#endif /* INCL_VKBDSYSREQ */



/***    VKBD function prototypes
 */

#ifdef  INCL_VKBDINTERFACES

typedef INT (* HOOKENTRY PFNSCRNP)(VOID);

VOID VDHENTRY VDHRegisterScreenProc(PFNSCRNP);
USHORT VDHENTRY VDHQueryKeyShift(HVDM);

/* VPL Idle detection services */
VOID VDHENTRY VDHWakeIdle(HVDM);
VOID VDHENTRY VDHReportPeek(ULONG);
VOID VDHENTRY VDHNotIdle(VOID);
BOOL * VDHENTRY VDHGetBusyFlagPtr(void);

/* requests for SVC access to VPL polling services */
#define VPL_NAME "VPL_IDLE"
#define VDHVPL_BUSY_ADDR 0         /* provide V86 busy byte address */
#define VDHVPL_WAIT      1         /* make VDM sleep                */
#define VDHVPL_NOIDLE    2         /* Turn idle detection off       */
#define VDHVPL_WAKE      3         /* Report busy activity          */

#endif

#endif /* __VKBD__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
