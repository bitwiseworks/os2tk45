/***************************************************************************
*
* Module Name: VMD.H (MVDM Virtual Mouse Exported Definitions)
*
* OS/2 public header file.
*
* Copyright (c) 1987 - 1993  IBM Corporation
*
*****************************************************************************/


#ifndef INCL_NONE                       // include everything by default
#define INCL_VMOUSE
#endif

#ifdef  INCL_VMOUSE
#define INCL_VMOUSEINT
#define INCL_VMOUSESYSREQ
#define INCL_VMOUSEDEVREQ
#endif

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */


#ifndef __VMD__
#define __VMD__

#ifdef  INCL_VMOUSEINT

/***    VMD INT 33h constants
 */

#define INT_MOUSE            0x33       // mouse software interrupt

#define INT33_RESET             0       // mouse reset and status
#define INT33_SHOWPOINTER       1       // show pointer
#define INT33_HIDEPOINTER       2       // hide pointer
#define INT33_QUERYSTATUS       3       // query button status and mouse pos.
#define INT33_SETPOSITION       4       // set mouse position
#define INT33_GETPRESSINFO      5       // get button press information
#define INT33_GETRELEASEINFO    6       // get button release information
#define INT33_SETHORIZONTAL     7       // set horizontal min/max boundaries
#define INT33_SETVERTICAL       8       // set vertical min/max boundaries
#define INT33_DEFINEGRPOINTER   9       // define graphics pointer
#define INT33_DEFINETEXTPOINTER 10      // define text/hardware pointer
#define INT33_GETMOTIONCOUNTS   11      // get mouse motion counters
#define INT33_SETUSERSUB        12      // set user call mask/address
#define INT33_ENABLELIGHTPEN    13      // enable lightpen emulation
#define INT33_DISABLELIGHTPEN   14      // disable lightpen emulation
#define INT33_SETMICKEYRATIO    15      // set mickeys-to-8-pixels ratio
#define INT33_SETCONDITIONAL    16      // set conditional-off area
#define INT33_FUNCTION17        17      // UNDEFINED (points to function 19)
#define INT33_DEFINEVARPOINTER  18      // define variable-size graphics pointer
#define INT33_SETDBLSPEED       19      // set dbl-speed threshold
#define INT33_SWAPUSERSUB       20      // swap user call mask/address
#define INT33_QUERYSTATESIZE    21      // query save-state storage req.
#define INT33_SAVESTATE         22      // save driver state
#define INT33_RESTORESTATE      23      // restore driver state
#define INT33_SETALTUSERSUB     24      // set alternate call mask/address
#define INT33_QUERYALTUSERSUB   25      // query alternate call mask/address
#define INT33_SETSENSITIVITY    26      // set mickeys/pixel, dbl-speed thres.
#define INT33_QUERYSENSITIVITY  27      // query mickeys/pixel, dbl-speed thres.
#define INT33_SETINTERRUPTRATE  28      // set interrupt rate (inport only)
#define INT33_SETPOINTERCOLOR   28      // set pointer color
#define INT33_SETCRTPAGE        29      // set CRT page #
#define INT33_QUERYCRTPAGE      30      // query CRT page #
#define INT33_DISABLEDRIVER     31      // disable driver
#define INT33_ENABLEDRIVER      32      // enable driver
#define INT33_SOFTWARERESET     33      // more complete mouse reset
#define INT33_SETLANGUAGE       34      // set language #
#define INT33_QUERYLANGUAGE     35      // query language #
#define INT33_QUERYMOUSEVERSION 36      // query mouse version, type, and IRQ

#define INT33_GETSWITCHSETTINGS 51      // get switch settings
#define INT33_GETMOUSEINI       52      // get MOUSE.INI
#define INT33_MAXFUNC           INT33_GETMOUSEINI

#define INT33_ERROR             -1      // error return in AX of INT 33 function

#define MAX_PTRWIDTH            2       // maximum # bytes across
#define MAX_PTRHEIGHT           16      // maximum # bytes in height

#define BUTTON_LEFT             0       // button ordinals
#define BUTTON_RIGHT            1
#define BUTTON_CENTER           2

#define BUTBIT_LEFT             0x0001  // button bit positions
#define BUTBIT_RIGHT            0x0002
#define BUTBIT_CENTER           0x0004

#define CALLMASK_MOTION         0x0001  // motion
#define CALLMASK_LEFTDOWN       0x0002  // LEFT button down
#define CALLMASK_LEFTUP         0x0004  // LEFT button up
#define CALLMASK_RIGHTDOWN      0x0008  // RIGHT button down
#define CALLMASK_RIGHTUP        0x0010  // RIGHT button up
#define CALLMASK_SHIFTBUTTON    0x0020  // SHIFT key held while button up/down
#define CALLMASK_CTRLBUTTON     0x0040  // CTRL key held while button up/down
#define CALLMASK_ALTBUTTON      0x0080  // ALT key held while button up/down
#define CALLMASK_CENTERDOWN     0x0100  // CENTER button down
#define CALLMASK_CENTERUP       0x0200  // CENTER button up
#define CALLMASK_ALLOWED        0x03FF
#define CALLMASK_BUTTONSHIFTS   (CALLMASK_SHIFTBUTTON | \
                                 CALLMASK_CTRLBUTTON  | \
                                 CALLMASK_ALTBUTTON)

#define CALL12MASK_CENTERDOWN   0x0020  // CENTER button down (SETUSERSUB only)
#define CALL12MASK_CENTERUP     0x0040  // CENTER button up (SETUSERSUB only)
#define CALL12MASK_ALLOWED      0x007F


#define DEF_XMICKEYS    8               // (x_pixel)
#define DEF_YMICKEYS    16              // (y_pixel)
#define DEF_XMICKEYS_J  4               // (x_pixel) J-DOS
#define DEF_YMICKEYS_J  4               // (y_pixel) J-DOS

#define DEF_XMICSSCALE  50              // range 0-100
#define DEF_YMICSSCALE  50              // range 0-100
#define DEF_THRESHOLDSCALE      50      // range 0-100

#define DEF_TEXTANDMASK 0xffff          // (DfltTextAndMask)
#define DEF_TEXTXORMASK 0x7700          // (DfltTextXorMask)

#define DEF_THRESHOLD   64
#define DEF_THRESHOLD_J 0x7fff          // disable threshold
                                        // function in J-DOS.
#define DEF_COLOR       15              // pointer color

#endif


#ifdef  INCL_VMOUSESYSREQ

/***    VMSE constants
 */

#define VMD_NAME "VMOUSE$"              // name of VMD

/***    VMSE functions for use by PMVDMP
 */

#define VMSESYSREQ_SETACCESS    1       // obtain exclusive access for VDM
#define VMSESYSREQ_SETFOCUS     2       // notify VDM is gaining/losing focus
#define VMSESYSREQ_POSTMOUSE    3       // send mouse events to a VDM
#define VMSESYSREQ_MAX          3

#endif


#ifdef  INCL_VMOUSEDEVREQ

/***EK+ VMD constants
 */

#define VMDEVREQ_REGISTER       1       // VMD-defined functions for
#define VMDEVREQ_SETSIZE        2       // communication from VVD to VMD
#define VMDEVREQ_SEAMLESS       3       // Seamless notification
#define VMDEVREQ_PROTINT33      4       // VDPX to VMD for Pmode Int 33h calls
#define VMDEVREQ_DPMITASK       5       // DPMI program start/stop notification
/*end*/


/***ET+ VMD VDHRequestVDD communication structures
 *
 *  VMREG is the input packet passed with the VMDEVREQ_REGISTER request
 *  from a VVD, and contains all the functions required by VMD of every
 *  supported VVD, which are:
 *
 *      Show pointer
 *      Hide pointer
 *      Define text pointer
 *      Define graphics pointer
 *      Enable/disable light pen emulation
 *
 *  VMFUNC is the output packet filled in on the same (VMDEVREQ_REGISTER)
 *  request.  It points to the following functions:
 *
 *      VDM Light pen request (to call only when light pen emulation enabled)
 *
 *  VMSSIZE is the input packet passed with the VMDEVREQ_SETSIZE request
 *  from a VVD, and defines the current video mode (or -1 if unknown),
 *  physical screen dimensions, and cell size.  There is no output packet
 *  for this request.
 */

typedef BOOL (*EXPENTRY PFNSHOWPTR)(HVDM, ULONG, ULONG);
typedef BOOL (*EXPENTRY PFNHIDEPTR)(HVDM);
typedef BOOL (*EXPENTRY PFNDEFTEXT)(HVDM, ULONG, ULONG, BOOL);
typedef BOOL (*EXPENTRY PFNDEFGRAPH)(HVDM, PUSHORT, ULONG, ULONG);
typedef BOOL (*EXPENTRY PFNSETPAGE)(HVDM, ULONG);
typedef BOOL (*EXPENTRY PFNSETLPEN)(HVDM, BOOL);
typedef BOOL (EXPENTRY *PFNSETCOLOR)(HVDM, ULONG);

typedef struct vmreg_s {                /* vmreg */
    ULONG       vmreg_nb;               // size of structure, in bytes (24)
    PFNSHOWPTR  vmreg_pfnShowPtr;       // ptr to show pointer fn.
    PFNHIDEPTR  vmreg_pfnHidePtr;       // ptr to hide pointer fn.
    PFNDEFTEXT  vmreg_pfnDefTextPtr;    // ptr to define text pointer fn.
    PFNDEFGRAPH vmreg_pfnDefGraphPtr;   // ptr to define graphics pointer fn.
    PFNSETPAGE  vmreg_pfnSetVideoPage;  // ptr to set video page fn.
    PFNSETLPEN  vmreg_pfnSetLPenEm;     // ptr to enable/disable lpen fn.
    PFNSETCOLOR vmreg_pfnSetPtrColor;   // ptr to set pointer color fn.
} VMREG;

typedef VMREG *PVMREG;


typedef struct vmstat_s {
    BOOL    vmstat_fPtrHidden;          // visual pointer status
    ULONG   vmstat_x;                   // current virtual position
    ULONG   vmstat_y;
    ULONG   vmstat_flButtons;           // current button status (see BUTBITs)
} VMSTAT;

typedef VMSTAT *PVMSTAT;

typedef BOOL (*EXPENTRY PFNQUERYSTAT)(HVDM, PVMSTAT);

typedef struct vmfunc_s {               /* vmfunc */
    ULONG        vmfunc_nb;             // size of structure, in bytes (8)
    PFNQUERYSTAT vmfunc_pfnQueryStatus; // ptr to query virtual status proc.
} VMFUNC;

typedef VMFUNC *PVMFUNC;

typedef struct vmss_s {                 /* vmss */
    ULONG   vmss_nb;                    // size of structure, in bytes (36)
    LONG    vmss_lMode;                 // video mode (eg, 00h-13h, or -1)
    ULONG   vmss_ulWidth;               // width of screen, in pixels
    ULONG   vmss_ulHeight;              // height of screen, in pixels
    ULONG   vmss_ulCellWidth;           // width of screen cells, in pixels
    ULONG   vmss_ulCellHeight;          // height of screen cells, in pixels
    ULONG   vmss_ulPtrWidth;            // width of pointer drawing size, in pixels
    ULONG   vmss_ulPtrHeight;           // height of pointer drawing size, in pixels
    ULONG   vmss_ulPtrUnitWidth;        // width of pointer drawing unit, in pixels
} VMSSIZE;

typedef VMSSIZE *PVMSSIZE;
/*end*/

#endif  // INCL_VMOUSEDEVREQ

#endif /* __VMD__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
