/****************************** Module Header ******************************\
*
* Module Name: BSESUB.H
*
* OS/2 Base include file.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
* Subcomponents marked with "+" are partially included by default:
*
*   #define:                To include:
*
*   INCL_KBD                KBD
*   INCL_VIO                VIO
*   INCL_MOU                MOU
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between NOINC
* and INC comments.
*
\***************************************************************************/

/* NOINC */
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
/* INC */

#ifndef __BSESUB__

/* NOINC */
#define __BSESUB__
/* INC */

#ifdef INCL_SUB
   #define INCL_KBD
   #define INCL_VIO
   #define INCL_MOU
#endif /* INCL_SUB */

#ifdef INCL_KBD

   #define Kbd16CharIn        KBD16CHARIN
   #define Kbd16Close         KBD16CLOSE
   #define Kbd16DeRegister    KBD16DEREGISTER
   #define Kbd16FlushBuffer   KBD16FLUSHBUFFER
   #define Kbd16FreeFocus     KBD16FREEFOCUS
   #define Kbd16GetCp         KBD16GETCP
   #define Kbd16GetFocus      KBD16GETFOCUS
   #define Kbd16GetHWID       KBD16GETHWID
   #define Kbd16GetStatus     KBD16GETSTATUS
   #define Kbd16Open          KBD16OPEN
   #define Kbd16Peek          KBD16PEEK
   #define Kbd16Register      KBD16REGISTER
   #define Kbd16SetCp         KBD16SETCP
   #define Kbd16SetCustXt     KBD16SETCUSTXT
   #define Kbd16SetFgnd       KBD16SETFGND
   #define Kbd16SetHWID       KBD16SETHWID
   #define Kbd16SetStatus     KBD16SETSTATUS
   #define Kbd16StringIn      KBD16STRINGIN
   #define Kbd16Synch         KBD16SYNCH
   #define Kbd16Xlate         KBD16XLATE

   #define KbdCharIn          KBD16CHARIN
   #define KbdClose           KBD16CLOSE
   #define KbdDeRegister      KBD16DEREGISTER
   #define KbdFlushBuffer     KBD16FLUSHBUFFER
   #define KbdFreeFocus       KBD16FREEFOCUS
   #define KbdGetCp           KBD16GETCP
   #define KbdGetFocus        KBD16GETFOCUS
   #define KbdGetHWID         KBD16GETHWID
   #define KbdGetStatus       KBD16GETSTATUS
   #define KbdOpen            KBD16OPEN
   #define KbdPeek            KBD16PEEK
   #define KbdRegister        KBD16REGISTER
   #define KbdSetCp           KBD16SETCP
   #define KbdSetCustXt       KBD16SETCUSTXT
   #define KbdSetFgnd         KBD16SETFGND
   #define KbdSetHWID         KBD16SETHWID
   #define KbdSetStatus       KBD16SETSTATUS
   #define KbdStringIn        KBD16STRINGIN
   #define KbdSynch           KBD16SYNCH
   #define KbdXlate           KBD16XLATE

   typedef unsigned short  HKBD;
   typedef HKBD        *   PHKBD;

   APIRET16 APIENTRY16 KbdRegister (PSZ pszModName,
                                    PSZ pszEntryPt,
                                    ULONG FunMask);

   #define KR_KBDCHARIN               0x00000001L
   #define KR_KBDPEEK                 0x00000002L
   #define KR_KBDFLUSHBUFFER          0x00000004L
   #define KR_KBDGETSTATUS            0x00000008L
   #define KR_KBDSETSTATUS            0x00000010L
   #define KR_KBDSTRINGIN             0x00000020L
   #define KR_KBDOPEN                 0x00000040L
   #define KR_KBDCLOSE                0x00000080L
   #define KR_KBDGETFOCUS             0x00000100L
   #define KR_KBDFREEFOCUS            0x00000200L
   #define KR_KBDGETCP                0x00000400L
   #define KR_KBDSETCP                0x00000800L
   #define KR_KBDXLATE                0x00001000L
   #define KR_KBDSETCUSTXT            0x00002000L

   #define IO_WAIT                    0
   #define IO_NOWAIT                  1

   APIRET16 APIENTRY16 KbdDeRegister (void);

   /* KBDKEYINFO structure, for KbdCharIn and KbdPeek */
   /* NOINC */
   #pragma pack(2)
   /* INC */


   typedef  struct _KBDKEYINFO      /* kbci */
   {
      UCHAR    chChar;
      UCHAR    chScan;
      UCHAR    fbStatus;
      UCHAR    bNlsShift;
      USHORT   fsState;
      ULONG    time;
   }KBDKEYINFO;
   typedef KBDKEYINFO *PKBDKEYINFO;

   /* NOINC */
   #pragma pack()
   /* INC */


   APIRET16 APIENTRY16  KbdCharIn(PKBDKEYINFO pkbci,
                                  USHORT fWait,
                                  HKBD hkbd);

   APIRET16 APIENTRY16  KbdPeek (PKBDKEYINFO pkbci,
                                 HKBD hkbd);

   /* structure for KbdStringIn() */

   typedef struct _STRINGINBUF     /* kbsi */
   {
      USHORT cb;
      USHORT cchIn;
   } STRINGINBUF;
   typedef STRINGINBUF  *PSTRINGINBUF;

   APIRET16 APIENTRY16  KbdStringIn (PCH pch,
                                     PSTRINGINBUF pchIn,
                                     USHORT fsWait,
                                     HKBD hkbd);

   APIRET16 APIENTRY16  KbdFlushBuffer (HKBD hkbd);

   /* KBDINFO.fsMask */

   #define KEYBOARD_ECHO_ON                0x0001
   #define KEYBOARD_ECHO_OFF               0x0002
   #define KEYBOARD_BINARY_MODE            0x0004
   #define KEYBOARD_ASCII_MODE             0x0008
   #define KEYBOARD_MODIFY_STATE           0x0010
   #define KEYBOARD_MODIFY_INTERIM         0x0020
   #define KEYBOARD_MODIFY_TURNAROUND      0x0040
   #define KEYBOARD_2B_TURNAROUND          0x0080
   #define KEYBOARD_SHIFT_REPORT           0x0100

   #ifndef INCL_DOSDEVIOCTL  /* following constants defined in bsedev.h */
      /* KBDINFO.fsState/KBDKEYINFO.fsState/KBDTRANS.fsState */
      #define KBDSTF_RIGHTSHIFT               0x0001
      #define KBDSTF_LEFTSHIFT                0x0002
      #define KBDSTF_CONTROL                  0x0004
      #define KBDSTF_ALT                      0x0008
      #define KBDSTF_SCROLLLOCK_ON            0x0010
      #define KBDSTF_NUMLOCK_ON               0x0020
      #define KBDSTF_CAPSLOCK_ON              0x0040
      #define KBDSTF_INSERT_ON                0x0080
      #define KBDSTF_LEFTCONTROL              0x0100
      #define KBDSTF_LEFTALT                  0x0200
      #define KBDSTF_RIGHTCONTROL             0x0400
      #define KBDSTF_RIGHTALT                 0x0800
      #define KBDSTF_SCROLLLOCK               0x1000
      #define KBDSTF_NUMLOCK                  0x2000
      #define KBDSTF_CAPSLOCK                 0x4000
      #define KBDSTF_SYSREQ                   0x8000
   #endif  /* INCL_DOSDEVIOCTL */

   /* KBDINFO structure, for KbdSet/GetStatus */
   typedef struct _KBDINFO         /* kbst */
   {
      USHORT cb;
      USHORT fsMask;
      USHORT chTurnAround;
      USHORT fsInterim;
      USHORT fsState;
   }KBDINFO;
   typedef KBDINFO *PKBDINFO;


   APIRET16 APIENTRY16  KbdSetStatus (PKBDINFO pkbdinfo,
                                      HKBD hkbd);

   APIRET16 APIENTRY16  KbdGetStatus (PKBDINFO pkbdinfo,
                                      HKBD hdbd);

   APIRET16 APIENTRY16  KbdSetCp (USHORT usReserved,
                                  USHORT pidCP,
                                  HKBD hdbd);

   APIRET16 APIENTRY16  KbdGetCp (ULONG ulReserved,
                                  PUSHORT pidCP,
                                  HKBD hkbd);

   APIRET16 APIENTRY16  KbdOpen (PHKBD phkbd);

   APIRET16 APIENTRY16  KbdClose (HKBD hkbd);

   APIRET16 APIENTRY16  KbdGetFocus (USHORT fWait,
                                     HKBD hkbd);

   APIRET16 APIENTRY16  KbdFreeFocus (HKBD hkbd);

   APIRET16 APIENTRY16  KbdSynch (USHORT fsWait);

   APIRET16 APIENTRY16  KbdSetFgnd(VOID);

   /* structure for KbdGetHWID() */
   typedef struct _KBDHWID         /* kbhw */
   {
      USHORT cb;
      USHORT idKbd;
      USHORT usReserved1;
      USHORT usReserved2;
   } KBDHWID;
   typedef KBDHWID  *PKBDHWID;

   APIRET16 APIENTRY16  KbdGetHWID (PKBDHWID pkbdhwid,
                                    HKBD hkbd);

   APIRET16 APIENTRY16  KbdSetHWID (PKBDHWID pkbdhwid,
                                    HKBD hkbd);


   /* KBDTRANS.fbStatus */

   #define KBDTRF_SHIFT_KEY_IN             0x01
   #define KBDTRF_CONVERSION_REQUEST       0x20
   #define KBDTRF_FINAL_CHAR_IN            0x40
   #define KBDTRF_INTERIM_CHAR_IN          0x80

   /* structure for KbdXlate() */
   /* NOINC */
   #pragma pack(2)
   /* INC */

   typedef  struct _KBDTRANS        /* kbxl */
   {
      UCHAR      chChar;
      UCHAR      chScan;
      UCHAR      fbStatus;
      UCHAR      bNlsShift;
      USHORT     fsState;
      ULONG      time;
      USHORT     fsDD;
      USHORT     fsXlate;
      USHORT     fsShift;
      USHORT     sZero;
   } KBDTRANS;
   typedef KBDTRANS *PKBDTRANS;

   /* NOINC */
   #pragma pack()
   /* INC */

   APIRET16 APIENTRY16  KbdXlate (PKBDTRANS pkbdtrans,
                                  HKBD hkbd);

   APIRET16 APIENTRY16  KbdSetCustXt (PUSHORT usCodePage,
                                      HKBD hkbd);

#endif /* INCL_KBD */

#ifdef INCL_VIO

   #define VioCheckCharType        VIO16CHECKCHARTYPE
   #define VioDeRegister           VIO16DEREGISTER
   #define VioEndPopUp             VIO16ENDPOPUP
   #define VioGetAnsi              VIO16GETANSI
   #define VioGetBuf               VIO16GETBUF
   #define VioGetConfig            VIO16GETCONFIG
   #define VioGetCp                VIO16GETCP
   #define VioGetCurPos            VIO16GETCURPOS
   #define VioGetCurType           VIO16GETCURTYPE
   #define VioGetFont              VIO16GETFONT
   #define VioGetMode              VIO16GETMODE
   #define VioGetPhysBuf           VIO16GETPHYSBUF
   #define VioGetState             VIO16GETSTATE
   #define VioModeUndo             VIO16MODEUNDO
   #define VioModeWait             VIO16MODEWAIT
   #define VioPopUp                VIO16POPUP
   #define VioPrtSc                VIO16PRTSC
   #define VioPrtScToggle          VIO16PRTSCTOGGLE
   #define VioReadCellStr          VIO16READCELLSTR
   #define VioReadCharStr          VIO16READCHARSTR
   #define VioRedrawSize           VIO16REDRAWSIZE
   #define VioRegister             VIO16REGISTER
   #define VioSavRedrawUndo        VIO16SAVREDRAWUNDO
   #define VioSavRedrawWait        VIO16SAVREDRAWWAIT
   #define VioScrLock              VIO16SCRLOCK
   #define VioScrUnLock            VIO16SCRUNLOCK
   #define VioScrollDn             VIO16SCROLLDN
   #define VioScrollLf             VIO16SCROLLLF
   #define VioScrollRt             VIO16SCROLLRT
   #define VioScrollUp             VIO16SCROLLUP
   #define VioSetAnsi              VIO16SETANSI
   #define VioSetCp                VIO16SETCP
   #define VioSetCurPos            VIO16SETCURPOS
   #define VioSetCurType           VIO16SETCURTYPE
   #define VioSetFont              VIO16SETFONT
   #define VioSetMode              VIO16SETMODE
   #define VioSetState             VIO16SETSTATE
   #define VioShowBuf              VIO16SHOWBUF
   #define VioWrtCellStr           VIO16WRTCELLSTR
   #define VioWrtCharStr           VIO16WRTCHARSTR
   #define VioWrtCharStrAtt        VIO16WRTCHARSTRATT
   #define VioWrtNAttr             VIO16WRTNATTR
   #define VioWrtNCell             VIO16WRTNCELL
   #define VioWrtNChar             VIO16WRTNCHAR
   #define VioWrtTTY               VIO16WRTTTY

   #define Vio16CheckCharType      VIO16CHECKCHARTYPE
   #define Vio16DeRegister         VIO16DEREGISTER
   #define Vio16EndPopUp           VIO16ENDPOPUP
   #define Vio16GetAnsi            VIO16GETANSI
   #define Vio16GetBuf             VIO16GETBUF
   #define Vio16GetConfig          VIO16GETCONFIG
   #define Vio16GetCp              VIO16GETCP
   #define Vio16GetCurPos          VIO16GETCURPOS
   #define Vio16GetCurType         VIO16GETCURTYPE
   #define Vio16GetFont            VIO16GETFONT
   #define Vio16GetMode            VIO16GETMODE
   #define Vio16GetPhysBuf         VIO16GETPHYSBUF
   #define Vio16GetState           VIO16GETSTATE
   #define Vio16ModeUndo           VIO16MODEUNDO
   #define Vio16ModeWait           VIO16MODEWAIT
   #define Vio16PopUp              VIO16POPUP
   #define Vio16PrtSc              VIO16PRTSC
   #define Vio16PrtScToggle        VIO16PRTSCTOGGLE
   #define Vio16ReadCellStr        VIO16READCELLSTR
   #define Vio16ReadCharStr        VIO16READCHARSTR
   #define Vio16RedrawSize         VIO16REDRAWSIZE
   #define Vio16Register           VIO16REGISTER
   #define Vio16SavRedrawUndo      VIO16SAVREDRAWUNDO
   #define Vio16SavRedrawWait      VIO16SAVREDRAWWAIT
   #define Vio16ScrLock            VIO16SCRLOCK
   #define Vio16ScrUnLock          VIO16SCRUNLOCK
   #define Vio16ScrollDn           VIO16SCROLLDN
   #define Vio16ScrollLf           VIO16SCROLLLF
   #define Vio16ScrollRt           VIO16SCROLLRT
   #define Vio16ScrollUp           VIO16SCROLLUP
   #define Vio16SetAnsi            VIO16SETANSI
   #define Vio16SetCp              VIO16SETCP
   #define Vio16SetCurPos          VIO16SETCURPOS
   #define Vio16SetCurType         VIO16SETCURTYPE
   #define Vio16SetFont            VIO16SETFONT
   #define Vio16SetMode            VIO16SETMODE
   #define Vio16SetState           VIO16SETSTATE
   #define Vio16ShowBuf            VIO16SHOWBUF
   #define Vio16WrtCellStr         VIO16WRTCELLSTR
   #define Vio16WrtCharStr         VIO16WRTCHARSTR
   #define Vio16WrtCharStrAtt      VIO16WRTCHARSTRATT
   #define Vio16WrtNAttr           VIO16WRTNATTR
   #define Vio16WrtNCell           VIO16WRTNCELL
   #define Vio16WrtNChar           VIO16WRTNCHAR
   #define Vio16WrtTTY             VIO16WRTTTY


   typedef unsigned short  HVIO;
   typedef HVIO        *   PHVIO;

   APIRET16 APIENTRY16  VioRegister (PSZ pszModName,
                                     PSZ pszEntryName,
                                     ULONG flFun1,
                                     ULONG flFun2);

   APIRET16 APIENTRY16  VioGlobalReg(PSZ pszModName,
                                     PSZ pszEntryName,
                                     ULONG flFun1,
                                     ULONG flFun2,
                                     USHORT usReturn);

   /* first parameter registration constants   */
   #define VR_VIOGETCURPOS            0x00000001L
   #define VR_VIOGETCURTYPE           0x00000002L
   #define VR_VIOGETMODE              0x00000004L
   #define VR_VIOGETBUF               0x00000008L
   #define VR_VIOGETPHYSBUF           0x00000010L
   #define VR_VIOSETCURPOS            0x00000020L
   #define VR_VIOSETCURTYPE           0x00000040L
   #define VR_VIOSETMODE              0x00000080L
   #define VR_VIOSHOWBUF              0x00000100L
   #define VR_VIOREADCHARSTR          0x00000200L
   #define VR_VIOREADCELLSTR          0x00000400L
   #define VR_VIOWRTNCHAR             0x00000800L
   #define VR_VIOWRTNATTR             0x00001000L
   #define VR_VIOWRTNCELL             0x00002000L
   #define VR_VIOWRTTTY               0x00004000L
   #define VR_VIOWRTCHARSTR           0x00008000L

   #define VR_VIOWRTCHARSTRATT        0x00010000L
   #define VR_VIOWRTCELLSTR           0x00020000L
   #define VR_VIOSCROLLUP             0x00040000L
   #define VR_VIOSCROLLDN             0x00080000L
   #define VR_VIOSCROLLLF             0x00100000L
   #define VR_VIOSCROLLRT             0x00200000L
   #define VR_VIOSETANSI              0x00400000L
   #define VR_VIOGETANSI              0x00800000L
   #define VR_VIOPRTSC                0x01000000L
   #define VR_VIOSCRLOCK              0x02000000L
   #define VR_VIOSCRUNLOCK            0x04000000L
   #define VR_VIOSAVREDRAWWAIT        0x08000000L
   #define VR_VIOSAVREDRAWUNDO        0x10000000L
   #define VR_VIOPOPUP                0x20000000L
   #define VR_VIOENDPOPUP             0x40000000L
   #define VR_VIOPRTSCTOGGLE          0x80000000L

   /* second parameter registration constants  */
   #define VR_VIOMODEWAIT             0x00000001L
   #define VR_VIOMODEUNDO             0x00000002L
   #define VR_VIOGETFONT              0x00000004L
   #define VR_VIOGETCONFIG            0x00000008L
   #define VR_VIOSETCP                0x00000010L
   #define VR_VIOGETCP                0x00000020L
   #define VR_VIOSETFONT              0x00000040L
   #define VR_VIOGETSTATE             0x00000080L
   #define VR_VIOSETSTATE             0x00000100L

   APIRET16 APIENTRY16  VioDeRegister (void);

   APIRET16 APIENTRY16  VioGetBuf (PULONG pLVB,
                                   PUSHORT pcbLVB,
                                   HVIO hvio);

   APIRET16 APIENTRY16  VioGetCurPos (PUSHORT pusRow,
                                      PUSHORT pusColumn,
                                      HVIO hvio);

   APIRET16 APIENTRY16  VioSetCurPos (USHORT usRow,
                                      USHORT usColumn,
                                      HVIO hvio);

   /* structure for VioSet/GetCurType() */
   typedef struct _VIOCURSORINFO   /* vioci */
   {
      USHORT   yStart;
      USHORT   cEnd;
      USHORT   cx;
      USHORT   attr;
   } VIOCURSORINFO;

   typedef VIOCURSORINFO *PVIOCURSORINFO;

   APIRET16 APIENTRY16  VioGetCurType (PVIOCURSORINFO pvioCursorInfo,
                                       HVIO hvio);

   APIRET16 APIENTRY16  VioSetCurType (PVIOCURSORINFO pvioCursorInfo,
                                       HVIO hvio);


   /* VIOMODEINFO.color constants */

   #define COLORS_2        0x0001
   #define COLORS_4        0x0002
   #define COLORS_16       0x0004

   /* structure for VioSet/GetMode() */
   /* NOINC */
   #pragma pack(1)
   /* INC */


   typedef  struct _VIOMODEINFO     /* viomi */
   {
      USHORT cb;
      UCHAR  fbType;
      UCHAR  color;
      USHORT col;
      USHORT row;
      USHORT hres;
      USHORT vres;
      UCHAR  fmt_ID;
      UCHAR  attrib;
      ULONG  buf_addr;
      ULONG  buf_length;
      ULONG  full_length;
      ULONG  partial_length;
      PCH    ext_data_addr;
   } VIOMODEINFO;
   typedef VIOMODEINFO *PVIOMODEINFO;

   /* NOINC */
   #pragma pack()
   /* INC */

   #define VGMT_OTHER                 0x01
   #define VGMT_GRAPHICS              0x02
   #define VGMT_DISABLEBURST          0x04

   APIRET16 APIENTRY16  VioGetMode (PVIOMODEINFO pvioModeInfo,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioSetMode (PVIOMODEINFO pvioModeInfo,
                                    HVIO hvio);

   /* structure for VioGetPhysBuf() */
   typedef struct _VIOPHYSBUF      /* viopb */
   {
      PBYTE    pBuf;
      ULONG    cb;
      SEL      asel[1];
   } VIOPHYSBUF;

   typedef VIOPHYSBUF  *PVIOPHYSBUF;

   APIRET16 APIENTRY16  VioGetPhysBuf (PVIOPHYSBUF pvioPhysBuf,
                                       USHORT usReserved);

   APIRET16 APIENTRY16  VioReadCellStr (PCH pchCellStr,
                                        PUSHORT pcb,
                                        USHORT usRow,
                                        USHORT usColumn,
                                        HVIO hvio);

   APIRET16 APIENTRY16  VioReadCharStr (PCH pchCellStr,
                                        PUSHORT pcb,
                                        USHORT usRow,
                                        USHORT usColumn,
                                        HVIO hvio);

   APIRET16 APIENTRY16  VioWrtCellStr (PCH pchCellStr,
                                       USHORT cb,
                                       USHORT usRow,
                                       USHORT usColumn,
                                       HVIO hvio);

   APIRET16  APIENTRY16 VioWrtCharStr (PCH pchStr,
                                       USHORT cb,
                                       USHORT usRow,
                                       USHORT usColumn,
                                       HVIO hvio);

   APIRET16 APIENTRY16  VioScrollDn (USHORT usTopRow,
                                     USHORT usLeftCol,
                                     USHORT usBotRow,
                                     USHORT usRightCol,
                                     USHORT cbLines,
                                     PBYTE pCell,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioScrollUp (USHORT usTopRow,
                                     USHORT usLeftCol,
                                     USHORT usBotRow,
                                     USHORT usRightCol,
                                     USHORT cbLines,
                                     PBYTE pCell,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioScrollLf (USHORT usTopRow,
                                     USHORT usLeftCol,
                                     USHORT usBotRow,
                                     USHORT usRightCol,
                                     USHORT cbCol,
                                     PBYTE pCell,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioScrollRt (USHORT usTopRow,
                                     USHORT usLeftCol,
                                     USHORT usBotRow,
                                     USHORT usRightCol,
                                     USHORT cbCol,
                                     PBYTE pCell,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioWrtNAttr (PBYTE pAttr,
                                     USHORT cb,
                                     USHORT usRow,
                                     USHORT usColumn,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioWrtNCell (PBYTE pCell,
                                     USHORT cb,
                                     USHORT usRow,
                                     USHORT usColumn,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioWrtNChar (PCH pchChar,
                                     USHORT cb,
                                     USHORT usRow,
                                     USHORT usColumn,
                                     HVIO hvio);

   APIRET16 APIENTRY16  VioWrtTTY (PCH pch,
                                   USHORT cb,
                                   HVIO hvio);

   APIRET16 APIENTRY16  VioWrtCharStrAtt (PCH pch,
                                          USHORT cb,
                                          USHORT usRow,
                                          USHORT usColumn,
                                          PBYTE pAttr,
                                          HVIO hvio);

   #define VCC_SBCSCHAR               0
   #define VCC_DBCSFULLCHAR           1
   #define VCC_DBCS1STHALF            2
   #define VCC_DBCS2NDHALF            3

   APIRET16 APIENTRY16  VioCheckCharType (PUSHORT pType,
                                          USHORT usRow,
                                          USHORT usColumn,
                                          HVIO hvio);

   APIRET16 APIENTRY16  VioShowBuf (USHORT offLVB,
                                    USHORT cb,
                                    HVIO hvio);


   #define ANSI_ON                    1
   #define ANSI_OFF                   0

   APIRET16 APIENTRY16  VioSetAnsi (USHORT fAnsi,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioGetAnsi (PUSHORT pfAnsi,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioPrtSc (HVIO hvio);

   APIRET16 APIENTRY16  VioPrtScToggle (HVIO hvio);

   #define VSRWI_SAVEANDREDRAW        0
   #define VSRWI_REDRAW               1

   #define VSRWN_SAVE                 0
   #define VSRWN_REDRAW               1

   #define UNDOI_GETOWNER             0
   #define UNDOI_RELEASEOWNER         1

   #define UNDOK_ERRORCODE            0
   #define UNDOK_TERMINATE            1

   APIRET16 APIENTRY16  VioRedrawSize (PULONG pcbRedraw);

   APIRET16 APIENTRY16  VioSavRedrawWait (USHORT usRedrawInd,
                                          PUSHORT pNotifyType,
                                          USHORT usReserved);

   APIRET16 APIENTRY16  VioSavRedrawUndo (USHORT usOwnerInd,
                                          USHORT usKillInd,
                                          USHORT usReserved);

   #define VMWR_POPUP                 0
   #define VMWN_POPUP                 0

   APIRET16 APIENTRY16  VioModeWait (USHORT usReqType,
                                     PUSHORT pNotifyType,
                                     USHORT usReserved);

   APIRET16 APIENTRY16  VioModeUndo (USHORT usOwnerInd,
                                     USHORT usKillInd,
                                     USHORT usReserved);

   #define LOCKIO_NOWAIT              0
   #define LOCKIO_WAIT                1

   #define LOCK_SUCCESS               0
   #define LOCK_FAIL                  1

   APIRET16 APIENTRY16  VioScrLock (USHORT fWait,
                                    PUCHAR pfNotLocked,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioScrUnLock (HVIO hvio);

   #define VP_NOWAIT                  0x0000
   #define VP_WAIT                    0x0001
   #define VP_OPAQUE                  0x0000
   #define VP_TRANSPARENT             0x0002

   APIRET16 APIENTRY16  VioPopUp (PUSHORT pfWait,
                                  HVIO hvio);

   APIRET16 APIENTRY16  VioEndPopUp (HVIO hvio);


   /* VIOCONFIGINFO.adapter constants */

   #define DISPLAY_MONOCHROME      0x0000
   #define DISPLAY_CGA             0x0001
   #define DISPLAY_EGA             0x0002
   #define DISPLAY_VGA             0x0003
   #define DISPLAY_8514A           0x0007
   #define DISPLAY_IMAGEADAPTER    0x0008
   #define DISPLAY_XGA             0x0009
   #define DISPLAY_SVGA            0x000A

   /* VIOCONFIGINFO.display constants */

   #define MONITOR_MONOCHROME      0x0000
   #define MONITOR_COLOR           0x0001
   #define MONITOR_ENHANCED        0x0002
   #define MONITOR_8503            0x0003
   #define MONITOR_851X_COLOR      0x0004
   #define MONITOR_8514            0x0009
   #define MONITOR_FLATPANEL       0x000A
   #define MONITOR_8507_8604       0x000B
   #define MONITOR_8515            0x000C
   #define MONITOR_9515            0x000F
   #define MONITOR_9517            0x0011
   #define MONITOR_9518            0x0012
   #define MONITOR_OEM             0x0013

   /* structure for VioGetConfig() */
   /* NOINC */
   #pragma pack(2)
   /* INC */

   typedef  struct _VIOCONFIGINFO   /* vioin */
   {
      USHORT  cb;
      USHORT  adapter;
      USHORT  display;
      ULONG   cbMemory;
      USHORT  Configuration;
      USHORT  VDHVersion;
      USHORT  Flags;
      ULONG   HWBufferSize;
      ULONG   FullSaveSize;
      ULONG   PartSaveSize;
      USHORT  EMAdaptersOFF;
      USHORT  EMDisplaysOFF;
   } VIOCONFIGINFO;

   typedef VIOCONFIGINFO *PVIOCONFIGINFO;

   /* NOINC */
   #pragma pack()
   /* INC */

   #define VIO_CONFIG_CURRENT         0
   #define VIO_CONFIG_PRIMARY         1
   #define VIO_CONFIG_SECONDARY       2

   APIRET16 APIENTRY16  VioGetConfig (USHORT usConfigId,
                                      PVIOCONFIGINFO pvioin,
                                      HVIO hvio);

   /* structure for VioGet/SetFont() */
   typedef struct _VIOFONTINFO     /* viofi */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  cxCell;
      USHORT  cyCell;
      PVOID16 pbData;
      USHORT  cbData;
   } VIOFONTINFO;
   typedef VIOFONTINFO *PVIOFONTINFO;

   #define VGFI_GETCURFONT            0
   #define VGFI_GETROMFONT            1

   APIRET16 APIENTRY16  VioGetFont (PVIOFONTINFO pviofi,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioSetFont (PVIOFONTINFO pviofi,
                                    HVIO hvio);

   APIRET16 APIENTRY16  VioGetCp (USHORT usReserved,
                                  PUSHORT pIdCodePage,
                                  HVIO hvio);

   APIRET16 APIENTRY16  VioSetCp (USHORT usReserved,
                                  USHORT idCodePage,
                                  HVIO hvio);

   typedef struct _VIOPALSTATE     /* viopal */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  iFirst;
      USHORT  acolor[1];
   } VIOPALSTATE;
   typedef VIOPALSTATE *PVIOPALSTATE;

   typedef struct _VIOOVERSCAN     /* vioos */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  color;
   } VIOOVERSCAN;
   typedef VIOOVERSCAN *PVIOOVERSCAN;

   typedef struct _VIOINTENSITY    /* vioint */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  fs;
   } VIOINTENSITY;
   typedef VIOINTENSITY *PVIOINTENSITY;

   typedef struct _VIOCOLORREG    /* viocreg */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  firstcolorreg;
      USHORT  numcolorregs;
      PCH     colorregaddr;
   } VIOCOLORREG;
   typedef VIOCOLORREG *PVIOCOLORREG;

   typedef struct _VIOSETULINELOC    /* viouline */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  scanline;
   } VIOSETULINELOC;
   typedef VIOSETULINELOC *PVIOSETULINELOC;

   typedef struct _VIOSETTARGET    /* viosett */
   {
      USHORT  cb;
      USHORT  type;
      USHORT  defaultalgorithm;
   } VIOSETTARGET;
   typedef VIOSETTARGET *PVIOSETTARGET;

   APIRET16 APIENTRY16  VioGetState (PVOID pState,
                                     HVIO hvio);
   APIRET16 APIENTRY16  VioSetState (PVOID pState,
                                     HVIO hvio);

#endif /* INCL_VIO */



#ifdef INCL_MOU

   #define MouClose                MOU16CLOSE
   #define MouDeRegister           MOU16DEREGISTER
   #define MouDrawPtr              MOU16DRAWPTR
   #define MouFlushQue             MOU16FLUSHQUE
   #define MouGetDevStatus         MOU16GETDEVSTATUS
   #define MouGetEventMask         MOU16GETEVENTMASK
   #define MouGetNumButtons        MOU16GETNUMBUTTONS
   #define MouGetNumMickeys        MOU16GETNUMMICKEYS
   #define MouGetNumQueEl          MOU16GETNUMQUEEL
   #define MouGetPtrPos            MOU16GETPTRPOS
   #define MouGetPtrShape          MOU16GETPTRSHAPE
   #define MouGetScaleFact         MOU16GETSCALEFACT
   #define MouGetThreshold         MOU16GETTHRESHOLD
   #define MouInitReal             MOU16INITREAL
   #define MouOpen                 MOU16OPEN
   #define MouReadEventQue         MOU16READEVENTQUE
   #define MouRegister             MOU16REGISTER
   #define MouRemovePtr            MOU16REMOVEPTR
   #define MouSetDevStatus         MOU16SETDEVSTATUS
   #define MouSetEventMask         MOU16SETEVENTMASK
   #define MouSetPtrPos            MOU16SETPTRPOS
   #define MouSetPtrShape          MOU16SETPTRSHAPE
   #define MouSetScaleFact         MOU16SETSCALEFACT
   #define MouSetThreshold         MOU16SETTHRESHOLD
   #define MouSynch                MOU16SYNCH

   #define Mou16Close              MOU16CLOSE
   #define Mou16DeRegister         MOU16DEREGISTER
   #define Mou16DrawPtr            MOU16DRAWPTR
   #define Mou16FlushQue           MOU16FLUSHQUE
   #define Mou16GetDevStatus       MOU16GETDEVSTATUS
   #define Mou16GetEventMask       MOU16GETEVENTMASK
   #define Mou16GetNumButtons      MOU16GETNUMBUTTONS
   #define Mou16GetNumMickeys      MOU16GETNUMMICKEYS
   #define Mou16GetNumQueEl        MOU16GETNUMQUEEL
   #define Mou16GetPtrPos          MOU16GETPTRPOS
   #define Mou16GetPtrShape        MOU16GETPTRSHAPE
   #define Mou16GetScaleFact       MOU16GETSCALEFACT
   #define Mou16GetThreshold       MOU16GETTHRESHOLD
   #define Mou16InitReal           MOU16INITREAL
   #define Mou16Open               MOU16OPEN
   #define Mou16ReadEventQue       MOU16READEVENTQUE
   #define Mou16Register           MOU16REGISTER
   #define Mou16RemovePtr          MOU16REMOVEPTR
   #define Mou16SetDevStatus       MOU16SETDEVSTATUS
   #define Mou16SetEventMask       MOU16SETEVENTMASK
   #define Mou16SetPtrPos          MOU16SETPTRPOS
   #define Mou16SetPtrShape        MOU16SETPTRSHAPE
   #define Mou16SetScaleFact       MOU16SETSCALEFACT
   #define Mou16SetThreshold       MOU16SETTHRESHOLD
   #define Mou16Synch              MOU16SYNCH


   typedef unsigned short  HMOU;
   typedef HMOU           *PHMOU;

   APIRET16 APIENTRY16  MouRegister (PSZ pszModName,
                                     PSZ pszEntryName,
                                     ULONG flFuns);

   #define MR_MOUGETNUMBUTTONS        0x00000001L
   #define MR_MOUGETNUMMICKEYS        0x00000002L
   #define MR_MOUGETDEVSTATUS         0x00000004L
   #define MR_MOUGETNUMQUEEL          0x00000008L
   #define MR_MOUREADEVENTQUE         0x00000010L
   #define MR_MOUGETSCALEFACT         0x00000020L
   #define MR_MOUGETEVENTMASK         0x00000040L
   #define MR_MOUSETSCALEFACT         0x00000080L
   #define MR_MOUSETEVENTMASK         0x00000100L
   #define MR_MOUOPEN                 0x00000800L
   #define MR_MOUCLOSE                0x00001000L
   #define MR_MOUGETPTRSHAPE          0x00002000L
   #define MR_MOUSETPTRSHAPE          0x00004000L
   #define MR_MOUDRAWPTR              0x00008000L
   #define MR_MOUREMOVEPTR            0x00010000L
   #define MR_MOUGETPTRPOS            0x00020000L
   #define MR_MOUSETPTRPOS            0x00040000L
   #define MR_MOUINITREAL             0x00080000L
   #define MR_MOUSETDEVSTATUS         0x00100000L

   APIRET16 APIENTRY16  MouDeRegister (void);

   APIRET16 APIENTRY16  MouFlushQue (HMOU hmou);

   #define MHK_BUTTON1                0x0001
   #define MHK_BUTTON2                0x0002
   #define MHK_BUTTON3                0x0004

   /* structure for MouGet/SetPtrPos() */
   typedef struct _PTRLOC      /* moupl */
   {
      USHORT row;
      USHORT col;
   } PTRLOC;
   typedef PTRLOC *PPTRLOC;

   APIRET16 APIENTRY16  MouGetPtrPos (PPTRLOC pmouLoc,
                                      HMOU hmou);
   APIRET16 APIENTRY16  MouSetPtrPos (PPTRLOC pmouLoc,
                                      HMOU hmou);

   /* structure for MouGet/SetPtrShape() */
   typedef struct _PTRSHAPE    /* moups */
   {
      USHORT cb;
      USHORT col;
      USHORT row;
      USHORT colHot;
      USHORT rowHot;
   } PTRSHAPE;
   typedef PTRSHAPE *PPTRSHAPE;

   APIRET16 APIENTRY16  MouSetPtrShape (PBYTE pBuf,
                                        PPTRSHAPE pmoupsInfo,
                                        HMOU hmou);

   APIRET16 APIENTRY16  MouGetPtrShape (PBYTE pBuf,
                                        PPTRSHAPE pmoupsInfo,
                                        HMOU hmou);

   /* MouGetDevStatus/MouSetDevStatus device status constants */

   #define MOUSE_QUEUEBUSY        0x0001
   #define MOUSE_BLOCKREAD        0x0002
   #define MOUSE_FLUSH            0x0004
   #define MOUSE_UNSUPPORTED_MODE 0x0008
   #define MOUSE_DISABLED         0x0100
   #define MOUSE_MICKEYS          0x0200


   APIRET16 APIENTRY16  MouGetDevStatus (PUSHORT pfsDevStatus,
                                         HMOU hmou);

   APIRET16 APIENTRY16  MouGetNumButtons (PUSHORT pcButtons,
                                          HMOU hmou);

   APIRET16 APIENTRY16  MouGetNumMickeys (PUSHORT pcMickeys,
                                          HMOU hmou);

   /* MouReadEventQue */

   #define MOU_NOWAIT      0x0000
   #define MOU_WAIT        0x0001

   /* structure for MouReadEventQue() */
   /* NOINC */
   #pragma pack(2)
   /* INC */

   typedef  struct _MOUEVENTINFO    /* mouev */
   {
      USHORT fs;
      ULONG  time;
      SHORT  row;
      SHORT  col;
   } MOUEVENTINFO;
   typedef MOUEVENTINFO *PMOUEVENTINFO;

   /* NOINC */
   #pragma pack()
   /* INC */

   APIRET16 APIENTRY16  MouReadEventQue (PMOUEVENTINFO pmouevEvent,
                                         PUSHORT pfWait,
                                         HMOU hmou);

   /* structure for MouGetNumQueEl() */
   typedef struct _MOUQUEINFO      /* mouqi */
   {
      USHORT cEvents;
      USHORT cmaxEvents;
   } MOUQUEINFO;
   typedef MOUQUEINFO *PMOUQUEINFO;

   APIRET16 APIENTRY16  MouGetNumQueEl (PMOUQUEINFO qmouqi,
                                        HMOU hmou);

   /* MouGetEventMask/MouSetEventMask events */

   #define MOUSE_MOTION                 0x0001
   #define MOUSE_MOTION_WITH_BN1_DOWN   0x0002
   #define MOUSE_BN1_DOWN               0x0004
   #define MOUSE_MOTION_WITH_BN2_DOWN   0x0008
   #define MOUSE_BN2_DOWN               0x0010
   #define MOUSE_MOTION_WITH_BN3_DOWN   0x0020
   #define MOUSE_BN3_DOWN               0x0040

   APIRET16 APIENTRY16  MouGetEventMask (PUSHORT pfsEvents,
                                         HMOU hmou);

   APIRET16 APIENTRY16  MouSetEventMask (PUSHORT pfsEvents,
                                         HMOU hmou);

   /* structure for MouGet/SetScaleFact() */
   typedef  struct _SCALEFACT   /* mousc */
   {
      USHORT rowScale;
      USHORT colScale;
   } SCALEFACT;
   typedef SCALEFACT *PSCALEFACT;

   APIRET16 APIENTRY16  MouGetScaleFact (PSCALEFACT pmouscFactors,
                                         HMOU hmou);

   APIRET16 APIENTRY16  MouSetScaleFact (PSCALEFACT pmouscFactors,
                                         HMOU hmou);

   APIRET16 APIENTRY16  MouOpen (PSZ pszDvrName,
                                 PHMOU phmou);

   APIRET16 APIENTRY16  MouClose (HMOU hmou);

   /* structure for MouRemovePtr() */
   typedef struct _NOPTRRECT   /* mourt */
   {
      USHORT row;
      USHORT col;
      USHORT cRow;
      USHORT cCol;
   } NOPTRRECT;
   typedef NOPTRRECT *PNOPTRRECT;

   APIRET16 APIENTRY16  MouRemovePtr (PNOPTRRECT pmourtRect,
                                      HMOU hmou);

   APIRET16 APIENTRY16  MouDrawPtr (HMOU hmou);

   #define MOU_NODRAW                 0x0001
   #define MOU_DRAW                   0x0000
   #define MOU_MICKEYS                0x0002
   #define MOU_PELS                   0x0000

   APIRET16 APIENTRY16  MouSetDevStatus (PUSHORT pfsDevStatus,
                                         HMOU hmou);

   APIRET16 APIENTRY16  MouInitReal (PSZ);

   APIRET16 APIENTRY16  MouSynch(USHORT pszDvrName);

   typedef struct _THRESHOLD       /* threshold */
   {
      USHORT Length;          /* Length Field            */
      USHORT Level1;          /* First movement level    */
      USHORT Lev1Mult;        /* First level multiplier  */
      USHORT Level2;          /* Second movement level   */
      USHORT lev2Mult;        /* Second level multiplier */
   } THRESHOLD;
   typedef THRESHOLD *PTHRESHOLD;

   APIRET16 APIENTRY16  MouGetThreshold(PTHRESHOLD pthreshold,
                                        HMOU hmou);

   APIRET16 APIENTRY16  MouSetThreshold(PTHRESHOLD pthreshold,
                                        HMOU hmou);

#endif /* INCL_MOU */

/* restore default packing */
/* NOINC */
#pragma pack()
/* INC */

#endif /* __BSESUB__ */

/* NOINC */
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
/* INC */

/**************************** end of file **********************************/
