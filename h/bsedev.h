/*static char *SCCSID = "@(#)bsedev.h   6.10 92/03/15";*/
/**************************************************************************\
*                                                                         *
* Module Name: BSEDEV.H                                                   *
*                                                                         *
* OS/2 Structures and constants for use with DosDevIOCtl                  *
*                                                                         *
* Copyright (c) 1987-1992, IBM Corporation                                *
*                                                                         *
\**************************************************************************/

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

#ifndef __BSEDEV__
/* NOINC */
#define __BSEDEV__
/* INC */

#define BSEDEV_INCLUDED

/* Input and Output Control Categories */

#define IOCTL_ASYNC                        0x0001
#define IOCTL_SCR_AND_PTRDRAW              0x0003
#define IOCTL_KEYBOARD                     0x0004
#define IOCTL_PRINTER                      0x0005
#define IOCTL_LIGHTPEN                     0x0006
#define IOCTL_POINTINGDEVICE               0x0007
#define IOCTL_DISK                         0x0008
#define IOCTL_PHYSICALDISK                 0x0009
#define IOCTL_MONITOR                      0x000A
#define IOCTL_GENERAL                      0x000B
#define IOCTL_POWER                        0x000C
#define IOCTL_OEMHLP                       0x0080
#define IOCTL_TESTCFG_SYS                  0x0080
#define IOCTL_CDROMDISK                    0x0080
#define IOCTL_CDROMAUDIO                   0x0081
#define IOCTL_TOUCH_DEVDEP                 0x0081
#define IOCTL_TOUCH_DEVINDEP               0x0081

/* Serial-Device Control */

#define ASYNC_SETBAUDRATE                  0x0041
#define ASYNC_SETLINECTRL                  0x0042
#define ASYNC_EXTSETBAUDRATE               0x0043
#define ASYNC_TRANSMITIMM                  0x0044
#define ASYNC_SETBREAKOFF                  0x0045
#define ASYNC_SETMODEMCTRL                 0x0046
#define ASYNC_SETBREAKON                   0x004B
#define ASYNC_STOPTRANSMIT                 0x0047
#define ASYNC_STARTTRANSMIT                0x0048
#define ASYNC_SETDCBINFO                   0x0053
#define ASYNC_SETENHANCEDMODEPARMS         0x0054
#define ASYNC_GETBAUDRATE                  0x0061
#define ASYNC_GETLINECTRL                  0x0062
#define ASYNC_EXTGETBAUDRATE               0X0063
#define ASYNC_GETCOMMSTATUS                0x0064
#define ASYNC_GETLINESTATUS                0x0065
#define ASYNC_GETMODEMOUTPUT               0x0066
#define ASYNC_GETMODEMINPUT                0x0067
#define ASYNC_GETINQUECOUNT                0x0068
#define ASYNC_GETOUTQUECOUNT               0x0069
#define ASYNC_GETCOMMERROR                 0x006D
#define ASYNC_GETCOMMEVENT                 0x0072
#define ASYNC_GETDCBINFO                   0x0073
#define ASYNC_GETENHANCEDMODEPARMS         0x0074

/* Screen/Pointer-Draw Control */

#define SCR_ALLOCLDT                       0x0070
#define SCR_DEALLOCLDT                     0x0071
#define PTR_GETPTRDRAWADDRESS              0x0072
#define VID_INITCALLVECTOR                 0X0073
#define SCR_ABIOSPASSTHRU                  0X0074
#define SCR_ALLOCLDTOFF                    0x0075
#define SCR_ALLOCLDTBGVAL                  0X0076
#define SCR_ALLOCVIDEOBUFFER               0X007E
#define SCR_GETROMFONTADDR                 0X007F

/* Keyboard Control */

#define KBD_SETTRANSTABLE                  0x0050
#define KBD_SETINPUTMODE                   0x0051
#define KBD_SETINTERIMFLAG                 0x0052
#define KBD_SETSHIFTSTATE                  0x0053
#define KBD_SETTYPAMATICRATE               0x0054
#define KBD_SETFGNDSCREENGRP               0x0055
#define KBD_SETSESMGRHOTKEY                0x0056
#define KBD_SETFOCUS                       0x0057
#define KBD_SETKCB                         0x0058
#define KBD_SETREADNOTIFICATION            0X0059
#define KBD_ALTERKBDLED                    0X005A
#define KBD_SETNLS                         0x005C
#define KBD_CREATE                         0x005D
#define KBD_DESTROY                        0x005E
#define KBD_GETINPUTMODE                   0x0071
#define KBD_GETINTERIMFLAG                 0x0072
#define KBD_GETSHIFTSTATE                  0x0073
#define KBD_READCHAR                       0x0074
#define KBD_PEEKCHAR                       0x0075
#define KBD_GETSESMGRHOTKEY                0x0076
#define KBD_GETKEYBDTYPE                   0x0077
#define KBD_GETCODEPAGEID                  0x0078
#define KBD_XLATESCAN                      0x0079
#define KBD_QUERYKBDHARDWAREID             0X007A
#define KBD_QUERYKBDCODEPAGESUPPORT        0X007B

/* Printer Control */

#define PRT_QUERYJOBHANDLE                 0x0021
#define PRT_SETFRAMECTL                    0x0042
#define PRT_SETINFINITERETRY               0x0044
#define PRT_INITPRINTER                    0x0046
#define PRT_ACTIVATEFONT                   0x0048
#define PRT_SETPRINTJOBTITLE               0x004D
#define PRT_SETIRQTIMEOUT                  0x004E
#define PRT_SETCOMMMODE                    0x0052
#define PRT_SETDATAXFERMODE                0x0053
#define PRT_GETFRAMECTL                    0x0062
#define PRT_GETINFINITERETRY               0x0064
#define PRT_GETPRINTERSTATUS               0x0066
#define PRT_QUERYACTIVEFONT                0x0069
#define PRT_VERIFYFONT                     0x006A
#define PRT_QUERYIRQTIMEOUT                0x006E
#define PRT_QUERYCOMMMODE                  0x0072
#define PRT_QUERYDATAXFERMODE              0x0073
#define PRT_QUERDEVICEID                   0x0074

/* Pointing-Device (Mouse) Control */

#define MOU_ALLOWPTRDRAW                   0x0050
#define MOU_UPDATEDISPLAYMODE              0x0051
#define MOU_SCREENSWITCH                   0x0052
#define MOU_SETSCALEFACTORS                0x0053
#define MOU_SETEVENTMASK                   0x0054
#define MOU_SETHOTKEYBUTTON                0x0055
#define MOU_REASSIGNTHRESHOLDVALUES        0X0055
#define MOU_SETPTRSHAPE                    0x0056
#define MOU_DRAWPTR                        0x0057
#define MOU_UNMARKCOLLISIONAREA            0X0057
#define MOU_REMOVEPTR                      0x0058
#define MOU_MARKCOLLISIONAREA              0X0058
#define MOU_SETPTRPOS                      0x0059
#define MOU_SETPROTDRAWADDRESS             0x005A
#define MOU_SETREALDRAWADDRESS             0x005B
#define MOU_SETMOUSTATUS                   0x005C
#define MOU_DISPLAYMODECHANGE              0x005D
#define MOU_GETBUTTONCOUNT                 0x0060
#define MOU_GETMICKEYCOUNT                 0x0061
#define MOU_GETMOUSTATUS                   0x0062
#define MOU_READEVENTQUE                   0x0063
#define MOU_GETQUESTATUS                   0x0064
#define MOU_GETEVENTMASK                   0x0065
#define MOU_GETSCALEFACTORS                0x0066
#define MOU_GETPTRPOS                      0x0067
#define MOU_GETPTRSHAPE                    0x0068
#define MOU_GETHOTKEYBUTTON                0x0069
#define MOU_QUERYTHRESHOLDVALUES           0x0069
#define MOU_VER                            0x006A
#define MOU_QUERYPOINTERID                 0X006B

/* Disk/Diskette Control */

#define DSK_LOCKDRIVE                      0x0000
#define DSK_UNLOCKDRIVE                    0x0001
#define DSK_REDETERMINEMEDIA               0x0002
#define DSK_SETLOGICALMAP                  0x0003
#define DSK_BEGINFORMAT                    0X0004
#define DSK_BLOCKREMOVABLE                 0x0020
#define DSK_GETLOGICALMAP                  0x0021
#define DSK_UNLOCKEJECTMEDIA               0X0040
#define DSK_SETDEVICEPARAMS                0x0043
#define DSK_WRITETRACK                     0x0044
#define DSK_FORMATVERIFY                   0x0045
#define DSK_DISKETTECONTROL                0X005D
#define DSK_QUERYMEDIASENSE                0X0060
#define DSK_GETDEVICEPARAMS                0x0063
#define DSK_READTRACK                      0x0064
#define DSK_VERIFYTRACK                    0x0065
#define DSK_GETLOCKSTATUS                  0X0066

/* Physical-Disk Control */

#define PDSK_LOCKPHYSDRIVE                 0x0000
#define PDSK_UNLOCKPHYSDRIVE               0x0001
#define PDSK_WRITEPHYSTRACK                0x0044
#define PDSK_GETPHYSDEVICEPARAMS           0x0063
#define PDSK_READPHYSTRACK                 0x0064
#define PDSK_VERIFYPHYSTRACK               0x0065

/* Character-Monitor Control */
#define MON_REGISTERMONITOR                0x0040

/* General Device Control */

#define DEV_FLUSHINPUT                     0x0001
#define DEV_FLUSHOUTPUT                    0x0002
#define DEV_SYSTEMNOTIFYPDD                0X0041
#define DEV_QUERYMONSUPPORT                0x0060

/* Advanced Power Management */

#define POWER_SENDPOWEREVENT               0X0040
#define POWER_SETPOWEREVENTRES             0X0041
#define POWER_GETPOWERSTATUS               0X0060
#define POWER_GETPOWEREVENT                0X0061
#define POWER_GETPOWERINFO                 0X0062

/* OEMHlp */

#define OEMHLP_GETOEMADAPTIONINFO          0X0000
#define OEMHLP_GETMACHINEINFO              0X0001
#define OEMHLP_GETDISPLAYCOMBCODE          0X0002
#define OEMHLP_GETVIDEOFONTS               0X0003
#define OEMHLP_READEISACONFIGINFO          0X0004
#define OEMHLP_GETROMBIOSINFO              0X0005
#define OEMHLP_GETMISCVIDEOINFO            0X0006
#define OEMHLP_GETVIDEOADAPTER             0X0007
#define OEMHLP_GETSVGAINFO                 0X0008
#define OEMHLP_GETMEMINFO                  0X0009
#define OEMHLP_GETDMQSINFO                 0X000A
#define OEMHLP_PCI                         0x000B
#define OEMHLP_QUERYDISKINFO               0x000E

 /* Test System Config */

#define TESTCFG_SYS_GETBIOSADAPTER         0x0040
#define TESTCFG_SYS_ISSUEINIOINSTR         0x0041
#define TESTCFG_SYS_ISSUEOUTIOINSTR        0x0042
#define TESTCFG_SYS_GETBUSARCH             0x0060
#define TESTCFG_SYS_GETALLPOSIDS           0x0061
#define TESTCFG_SYS_GETALLEISAIDS          0x0062

/* CDROM Drive and Disk */

#define CDROMDISK_RESETDRIVE               0X0040
#define CDROMDISK_EJECTDISK                0X0044
#define CDROMDISK_LOCKUNLOCKDOOR           0X0046
#define CDROMDISK_SEEK                     0X0050
#define CDROMDISK_DEVICESTATUS             0X0060
#define CDROMDISK_GETDRIVER                0X0061
#define CDROMDISK_GETSECTORSIZE            0X0063
#define CDROMDISK_GETHEADLOC               0X0070
#define CDROMDISK_READLONG                 0X0072
#define CDROMDISK_GETVOLUMESIZE            0X0078
#define CDROMDISK_GETUPC                   0X0079

/* CDROM Audio */

#define CDROMAUDIO_SETCHANNELCTRL          0X0040
#define CDROMAUDIO_PLAYAUDIO               0X0050
#define CDROMAUDIO_STOPAUDIO               0X0051
#define CDROMAUDIO_RESUMEAUDIO             0X0052
#define CDROMAUDIO_GETCHANNEL              0X0060
#define CDROMAUDIO_GETAUDIODISK            0X0061
#define CDROMAUDIO_GETAUDIOTRACK           0X0062
#define CDROMAUDIO_GETSUBCHANNELQ          0X0063
#define CDROMAUDIO_GETAUDIOSTATUS          0X0065

/* Touch device-dependent IOCtls */

#define TOUCH_DEVDEP_SETCALIBCONST         0X0052
#define TOUCH_DEVDEP_READDATA              0X0053
#define TOUCH_DEVDEP_SETDATAMODE           0X0054
#define TOUCH_DEVDEP_SETCLICKLOCK          0X0055
#define TOUCH_DEVDEP_SETTOUCHTHRESHOLD     0X0056
#define TOUCH_DEVDEP_SETEMULXY             0X0057
#define TOUCH_DEVDEP_SETDATAREPORTRATE     0X0058
#define TOUCH_DEVDEP_SETLOWPASSFILTER      0X0059
#define TOUCH_DEVDEP_WRITEMEMLOC           0X005A
#define TOUCH_DEVDEP_GETCALIBCONST         0X0060
#define TOUCH_DEVDEP_GETDATAMODE           0X0061
#define TOUCH_DEVDEP_GETCLICKLOCK          0X0062
#define TOUCH_DEVDEP_GETTOUCHTHRESHOLD     0X0063
#define TOUCH_DEVDEP_GETEMULXY             0X0064
#define TOUCH_DEVDEP_GETDATAREPORTRATE     0X0065
#define TOUCH_DEVDEP_GETLOWPASSFILTER      0X0066
#define TOUCH_DEVDEP_READMEMLOC            0X0067

/* Touch device-INdependent IOCtls */

#define TOUCH_DEVINDEP_SETCOORDSYS         0X0050
#define TOUCH_DEVINDEP_SETSELECTMECH       0X0052
#define TOUCH_DEVINDEP_SETEVENTMASK        0X0053
#define TOUCH_DEVINDEP_SETQUEUESIZE        0X0054
#define TOUCH_DEVINDEP_SETEMULSTATE        0X0055
#define TOUCH_DEVINDEP_GETCOORDSYS         0X0060
#define TOUCH_DEVINDEP_GETSELECTMECH       0X0062
#define TOUCH_DEVINDEP_GETEVENTMASK        0X0063
#define TOUCH_DEVINDEP_GETQUEUESIZE        0X0064
#define TOUCH_DEVINDEP_GETEMULSTATE        0X0065
#define TOUCH_DEVINDEP_GETREADEVENTQUEUE   0X0066

/* ASYNC_GETCOMMERROR, ASYNC_SETBREAKOFF, ASYNC_SETBREAKON,
 * ASYNC_SETMODEMCTRL
 */

#define RX_QUE_OVERRUN                     0x0001
#define RX_HARDWARE_OVERRUN                0x0002
#define PARITY_ERROR                       0x0004
#define FRAMING_ERROR                      0x0008

/* ASYNC_GETCOMMEVENT */
#define CHAR_RECEIVED                      0x0001
#define LAST_CHAR_SENT                     0x0004
#define CTS_CHANGED                        0x0008
#define DSR_CHANGED                        0x0010
#define DCD_CHANGED                        0x0020
#define BREAK_DETECTED                     0x0040
#define ERROR_OCCURRED                     0x0080
#define RI_DETECTED                        0x0100

/* ASYNC_GETCOMMSTATUS */
#define TX_WAITING_FOR_CTS                 0x0001
#define TX_WAITING_FOR_DSR                 0x0002
#define TX_WAITING_FOR_DCD                 0x0004
#define TX_WAITING_FOR_XON                 0x0008
#define TX_WAITING_TO_SEND_XON             0x0010
#define TX_WAITING_WHILE_BREAK_ON          0x0020
#define TX_WAITING_TO_SEND_IMM             0x0040
#define RX_WAITING_FOR_DSR                 0x0080

/* ASYNC_GETLINESTATUS */
#define WRITE_REQUEST_QUEUED               0x0001
#define DATA_IN_TX_QUE                     0x0002
#define HARDWARE_TRANSMITTING              0x0004
#define CHAR_READY_TO_SEND_IMM             0x0008
#define WAITING_TO_SEND_XON                0x0010
#define WAITING_TO_SEND_XOFF               0x0020

/* ASYNC_GETMODEMINPUT */
#define CTS_ON                             0x10
#define DSR_ON                             0x20
#define RI_ON                              0x40
#define DCD_ON                             0x80

/* DSK_SETDEVICEPARAMS */
#define BUILD_BPB_FROM_MEDIUM              0x00
#define REPLACE_BPB_FOR_DEVICE             0x01
#define REPLACE_BPB_FOR_MEDIUM             0x02

/* DSK_GETDEVICEPARAMS */
#define GET_BPB_FROM_DEVICE                0x00
#define GET_BPB_FROM_MEDIUM                0x01

/* KBD_GETINPUTMODE, KBD_PEEKCHAR, KBD_SETINPUTMODE*/
#define ASCII_MODE                         0x00
#define BINARY_MODE                        0x80

/* KBD_GETINTERIMFLAG */
#define CONVERSION_REQUEST                 0x20
#define INTERIM_CHAR                       0x80

/* KBD_GETSESMGRHOTKEY */
#define HOTKEY_MAX_COUNT                   0x0000
#define HOTKEY_CURRENT_COUNT               0x0001

/* KBD_PEEKCHAR */
#define KBD_DATA_RECEIVED                  0x0001
#define KBD_DATA_BINARY                    0x8000

/* KBD_READCHAR */
#define KBD_READ_WAIT                      0x0000
#define KBD_READ_NOWAIT                    0x8000

/* KBD_SETINPUTMODE */
#define SHIFT_REPORT_MODE                  0x01

#ifndef INCL_MOU

   #define MOUSE_MOTION                    0x0001
   #define MOUSE_MOTION_WITH_BN1_DOWN      0x0002
   #define MOUSE_BN1_DOWN                  0x0004
   #define MOUSE_MOTION_WITH_BN2_DOWN      0x0008
   #define MOUSE_BN2_DOWN                  0x0010
   #define MOUSE_MOTION_WITH_BN3_DOWN      0x0020
   #define MOUSE_BN3_DOWN                  0x0040

   #define MHK_BUTTON1                     0x0001
   #define MHK_BUTTON2                     0x0002
   #define MHK_BUTTON3                     0x0004

   #define MOU_NOWAIT                      0x0000
   #define MOU_WAIT                        0x0001

#endif /* #ifndef INCL_MOU */

/* MOU_GETHOTKEYBUTTON, MOU_SETHOTKEYBUTTON */
#define MHK_NO_HOTKEY                      0x0000

/* MOU_GETMOUSTATUS */
#define MOUSE_QUEUEBUSY                    0x0001
#define MOUSE_BLOCKREAD                    0x0002
#define MOUSE_FLUSH                        0x0004
#define MOUSE_UNSUPPORTED_MODE             0x0008
#define MOUSE_DISABLED                     0x0100
#define MOUSE_MICKEYS                      0x0200

/* PRT_GETPRINTERSTATUS */
#define PRINTER_TIMEOUT                    0x0001
#define PRINTER_IO_ERROR                   0x0008
#define PRINTER_SELECTED                   0x0010
#define PRINTER_OUT_OF_PAPER               0x0020
#define PRINTER_ACKNOWLEDGED               0x0040
#define PRINTER_NOT_BUSY                   0x0080

/* fbCtlHndShake */
#define MODE_DTR_CONTROL                   0x01
#define MODE_DTR_HANDSHAKE                 0x02
#define MODE_CTS_HANDSHAKE                 0x08
#define MODE_DSR_HANDSHAKE                 0x10
#define MODE_DCD_HANDSHAKE                 0x20
#define MODE_DSR_SENSITIVITY               0x40

/* fbFlowReplace */
#define MODE_AUTO_TRANSMIT                 0x01
#define MODE_AUTO_RECEIVE                  0x02
#define MODE_ERROR_CHAR                    0x04
#define MODE_NULL_STRIPPING                0x08
#define MODE_BREAK_CHAR                    0x10
#define MODE_RTS_CONTROL                   0x40
#define MODE_RTS_HANDSHAKE                 0x80
#define MODE_TRANSMIT_TOGGLE               0xC0

/* fbTimeout */
#define MODE_NO_WRITE_TIMEOUT              0x01
#define MODE_READ_TIMEOUT                  0x02
#define MODE_WAIT_READ_TIMEOUT             0x04
#define MODE_NOWAIT_READ_TIMEOUT           0x06

typedef struct _DCBINFO         /* dcbinf */
        {
        USHORT usWriteTimeout;
        USHORT usReadTimeout;
        BYTE   fbCtlHndShake;
        BYTE   fbFlowReplace;
        BYTE   fbTimeout;
        BYTE   bErrorReplacementChar;
        BYTE   bBreakReplacementChar;
        BYTE   bXONChar;
        BYTE   bXOFFChar;
        } DCBINFO, *PDCBINFO;

/* NOINC */
#pragma pack(1)
/* INC */

typedef struct _TRACKLAYOUT     /* trckl */
        {
        BYTE   bCommand;
        USHORT usHead;
        USHORT usCylinder;
        USHORT usFirstSector;
        USHORT cSectors;
        struct {
               USHORT usSectorNumber;
               USHORT usSectorSize;
               } TrackTable[1];
        } TRACKLAYOUT, *PTRACKLAYOUT;

/* NOINC */
#pragma pack()
/* INC */

#define DEVTYPE_48TPI                      0x0000
#define DEVTYPE_96TPI                      0x0001
#define DEVTYPE_35                         0x0002
#define DEVTYPE_8SD                        0x0003
#define DEVTYPE_8DD                        0x0004
#define DEVTYPE_FIXED                      0x0005
#define DEVTYPE_TAPE                       0x0006
#define DEVTYPE_UNKNOWN                    0x0007


/* NOINC */
#pragma pack(1)
/* INC */

typedef struct _BIOSPARAMETERBLOCK      /* bspblk */
        {
        USHORT usBytesPerSector;
        BYTE   bSectorsPerCluster;
        USHORT usReservedSectors;
        BYTE   cFATs;
        USHORT cRootEntries;
        USHORT cSectors;
        BYTE   bMedia;
        USHORT usSectorsPerFAT;
        USHORT usSectorsPerTrack;
        USHORT cHeads;
        ULONG  cHiddenSectors;
        ULONG  cLargeSectors;
        BYTE   abReserved[6];
        USHORT cCylinders;
        BYTE   bDeviceType;
        USHORT fsDeviceAttr;
        } BIOSPARAMETERBLOCK;
typedef BIOSPARAMETERBLOCK *PBIOSPARAMETERBLOCK;

/* NOINC */
#pragma pack()
/* INC */

typedef struct _SCREENGROUP     /* scrgrp */
        {
        USHORT idScreenGrp;
        USHORT fTerminate;
        } SCREENGROUP;
typedef  SCREENGROUP *PSCREENGROUP;

typedef struct _FRAME           /* frm */
        {
        BYTE bCharsPerLine;
        BYTE bLinesPerInch;
        } FRAME;
typedef FRAME *PFRAME;

typedef struct _KBDTYPE         /* kbdtyp */
        {
        USHORT usType;
        USHORT reserved1;
        USHORT reserved2;
        } KBDTYPE;
typedef KBDTYPE *PKBDTYPE;

typedef struct _LINECONTROL     /* lnctl */
        {
        BYTE bDataBits;
        BYTE bParity;
        BYTE bStopBits;
        BYTE fTransBreak;
        } LINECONTROL;
typedef LINECONTROL *PLINECONTROL;

/* MODEMSTATUS.fbModemOn, ASYNC_GETMODEMOUTPUT */
#define DTR_ON                             0x01
#define RTS_ON                             0x02

/* MODEMSTATUS.fbModemOff */
#define DTR_OFF                            0xFE
#define RTS_OFF                            0xFD

typedef struct _MODEMSTATUS     /* mdmst */
        {
        BYTE fbModemOn;
        BYTE fbModemOff;
        } MODEMSTATUS;
typedef MODEMSTATUS *PMODEMSTATUS;

/* NOINC */
#pragma pack(1)
/* INC */

typedef struct _TRACKFORMAT     /* trckfmt */
        {
        BYTE bCommand;
        USHORT usHead;
        USHORT usCylinder;
        USHORT usReserved;
        USHORT cSectors;
        struct {
               BYTE bCylinder;
               BYTE bHead;
               BYTE idSector;
               BYTE bBytesSector;
               } FormatTable[1];
        } TRACKFORMAT;

typedef TRACKFORMAT *PTRACKFORMAT;

/* NOINC */
#pragma pack()
/* INC */

typedef struct _RXQUEUE         /* rxq */
        {
        USHORT cch;
        USHORT cb;
        } RXQUEUE;
typedef RXQUEUE *PRXQUEUE;

typedef struct _DEVICEPARAMETERBLOCK    /* dvpblck */
        {
        USHORT reserved1;
        USHORT cCylinders;
        USHORT cHeads;
        USHORT cSectorsPerTrack;
        USHORT reserved2;
        USHORT reserved3;
        USHORT reserved4;
        USHORT reserved5;
        } DEVICEPARAMETERBLOCK;

typedef DEVICEPARAMETERBLOCK     *PDEVICEPARAMETERBLOCK;

/* The following must be packed on 2 bytes boundary */
/* NOINC */
#pragma pack(2)
/* INC */

typedef struct _PTRDRAWFUNCTION         /* ptrdfnc */
        {
        USHORT usReturnCode;
        PFN pfnDraw;
        PCH pchDataSeg;
        } PTRDRAWFUNCTION;

typedef PTRDRAWFUNCTION *PPTRDRAWFUNCTION;

/* NOINC */
#pragma pack()
/* INC */

typedef struct _PTRDRAWADDRESS          /* ptrdaddr */
        {
        USHORT reserved;
        PTRDRAWFUNCTION ptrdfnc;
        } PTRDRAWADDRESS;

typedef PTRDRAWADDRESS  *PPTRDRAWADDRESS;

typedef struct _SHIFTSTATE      /* shftst */
        {
        USHORT fsState;
        BYTE   fNLS;
        } SHIFTSTATE;

typedef SHIFTSTATE  *PSHIFTSTATE;



/* HOTKEY.fsHotKey/SHIFTSTATE.fsState */
#define RIGHTSHIFT                         0x0001
#define LEFTSHIFT                          0x0002
#define CONTROL                            0x0004
#define ALT                                0x0008
#define SCROLLLOCK_ON                      0x0010
#define NUMLOCK_ON                         0x0020
#define CAPSLOCK_ON                        0x0040
#define INSERT_ON                          0x0080
#define LEFTCONTROL                        0x0100
#define LEFTALT                            0x0200
#define RIGHTCONTROL                       0x0400
#define RIGHTALT                           0x0800
#define SCROLLLOCK                         0x1000
#define NUMLOCK                            0x2000
#define CAPSLOCK                           0x4000
#define SYSREQ                             0x8000

typedef struct _HOTKEY          /* htky */
        {
        USHORT fsHotKey;
        UCHAR  uchScancodeMake;
        UCHAR  uchScancodeBreak;
        USHORT idHotKey;
        } HOTKEY;

typedef HOTKEY *PHOTKEY;

typedef struct _MONITORPOSITION         /* mnpos */
        {
        USHORT fPosition;
        USHORT index;
        ULONG  pbInBuf;
        USHORT offOutBuf;
        } MONITORPOSITION;

typedef MONITORPOSITION  *PMONITORPOSITION;

typedef struct _RATEDELAY       /* rtdly */
        {
        USHORT usDelay;
        USHORT usRate;
        } RATEDELAY;
typedef RATEDELAY  *PRATEDELAY;

typedef struct _CODEPAGEINFO    /* cpi */
        {
        PBYTE pbTransTable;
        USHORT idCodePage;
        USHORT idTable;
        } CODEPAGEINFO;
typedef *PCODEPAGEINFO;

typedef struct _CPID    /* cpid */
        {
        USHORT idCodePage;
        USHORT Reserved;
        } CPID;
typedef CPID *PCPID;

typedef struct _LDTADDRINFO     /* ldtaddr */
        {
        PULONG pulPhysAddr;
        USHORT cb;
        } LDTADDRINFO;

typedef LDTADDRINFO *PLDTADDRINFO;

typedef struct _PTRDRAWDATA     /* ptrdd */
        {
        USHORT cb;
        USHORT usConfig;
        USHORT usFlag;
        } PTRDRAWDATA;
#pragma pack()                           /*restore default packing */

typedef PTRDRAWDATA *PPTRDRAWDATA;


/*
 * SCREENDD IOCTL definitions
 */
#define SCREENDD_GETCURRENTBANK         0x00    /* Get current bank */
#define SCREENDD_SETCURRENTBANK         0x01    /* Set current bank */
#define SCREENDD_SVGA_ID                0x08    /* Get SVGA info IOCTL */
#define SCREENDD_SVGA_OEM               0x09    /* Get OEM info IOCTL */
#define SCREENDD_UPDATEMEMORY           0x0A    /* Update Memory    IOCTl   */
#define SCREENDD_GETLINEARACCESS        0x0B    /* GetLinearAccess  IOCTl   */
#define SCREENDD_GETGLOBALACCESS        0x0C    /* GetGlobalAccess  IOCTl   */
#define SCREENDD_FREEGLOBALACCESS       0x0D    /* FreeGlobalAccess IOCTl   */
#define SCREENDD_REGISTER_RING0_CALLER  0x0E    /* Register VVID as Ring0 PMI caller */
#define SCREENDD_WAIT_ON_RING0_CALLER   0x0F    /* Wait for RING0 caller to register */
/* this IOCTL is used internally between videopmi & screendd: */
#define SCREENDD_SVGA_APM               0x10    /* Wait 4 APM */
#define SCREENDD_GET_CS_ALIAS           0x11
/*
** APM constants
*/
#define APM_RESUME              1
#define APM_SUSPEND             2
#define APM_DISABLED            4

#define SCREENDD_CATEGORY               0x80
#define SCREENDD_NAME                   "SCREEN$"

#define GETLINEAR_FLAG_MAPPHYSICAL      0x00000010L
#define GETLINEAR_FLAG_MAPPROCESS       0x00000020L
#define GETLINEAR_FLAG_MAPSHARED        0x00000400L
#define GETLINEAR_FLAG_MAPVERIFY        0x40000000L
#define GETLINEAR_FLAG_MAPATTACH        0x80000000L

#define EGA_BIT         4                /* EGA */
#define VGA_BIT         8                /* VGA */
#define EGAVGA_BIT      (EGA_BIT+VGA_BIT)/* EGA/VGA */

                                       /* SCREENDD_GETLINEARACCESS */
typedef struct _GETLINIOCTLDATA {  /* */
        ULONG   PacketLength;
        ULONG   PhysicalAddress;
        ULONG   ApertureSize;
        PBYTE   LinearAddress;
        ULONG   LinearFlags;
} GETLINIOCTLDATA;
typedef GETLINIOCTLDATA *PGETLINIOCTLDATA;

                                /*  SCREENDD_SETCURRENTBANK/GETCURRENTBANK */
typedef struct _BANKINFO {     /* */
        ULONG  ulBankLength;
        USHORT usBank;
        USHORT usVideoModeType;
        USHORT usReadWriteMode;
} BANKINFO;

#define READ_BANK    0
#define WRITE_BANK   1
#define MODE_TEXT    0
#define MODE_PLANAR  1
#define MODE_LINEAR  2

                                 /* SCREENDD_GETGLOBALACCESS */
typedef struct _GLOBALIOCTLDATA {   /* */
        ULONG   ProcessAddress;
        ULONG   AddressLength;
        ULONG   GlobalAddress;
} GLOBALIOCTLDATA;
                                           /* data packet for SCREENDD_SVGA_ID */
        /* Rather than cause build troubles using PCI.H, */
        /* the new fields must be cast during use */
typedef struct _OEMSVGAINFO { /* */
        USHORT AdapterType;
        USHORT ChipType;
        ULONG  Memory;
               /* PCI_CFG_BASE_ADDR_COUNT = 6 */
        ULONG  aulPCIRangeSize [6];
               /* PCI_CFG_SPACE_SIZE = 0x0100 = 256 */
        BYTE   abPCIConfiguration [256];
} OEMSVGAINFO;
                                           /* data packet for SCREENDD_SVGA_OEM */
/* OEM data packet format must contain total length in bytes as its first word. */
/* The rest of it can be in manufacturer/adapter specific format. */

typedef struct _OEMINFO {    /* */
        ULONG  OEMLength;
        USHORT Manufacturer;
        ULONG  ManufacturerData;
        USHORT ManufacturerModel;
} OEMINFO;
                                          /* paramter packet for GETGLOBALACCESS */
typedef struct _GETGLOBALPACKET {  /* */
        ULONG   GlobalPktLength;
        GLOBALIOCTLDATA GlobalPktData[1];
} GETGLOBALPACKET;


/* NOINC */
#pragma pack(1)
/* INC */

typedef struct _OEMHLPDISKINFO {  /* */
   unsigned short ODI_Reserved;         /* Reserved                           */
   unsigned short ODI_Flags;            /* Information flags                  */
   unsigned long  ODI_Cylinders;        /* Number of Phys Cylinders           */
   unsigned long  ODI_Heads;            /* Number of Phys Heads               */
   unsigned long  ODI_SectorsPerTrack;  /* Number of Phys sectors per track   */
   unsigned long  ODI_PhysSectors_low;  /* Number of Phys Sectors (low dword) */
   unsigned long  ODI_PhysSectors_high; /* Number of Phys Sectors (high dword)*/
   unsigned short ODI_SectorSize;       /* Number of bytes in a sector        */
   unsigned long  ODI_Reserved2;        /* Reserved                           */
   unsigned short ODI_IOPort;           /* I/O port base address              */
   unsigned short ODI_ControlPort;      /* Control port address               */
   unsigned char  ODI_HeadRegisterUN;   /* Head Register upper nibble         */
   unsigned char  ODI_Reserved3;        /* Reserved                           */
   unsigned char  ODI_IRQ;              /* IRQ information                    */
   unsigned char  ODI_RWMult_BlkCnt;    /* Block count, ATA R/W Mult commands */
   unsigned char  ODI_DMAInfo;          /* DMA information                    */
   unsigned char  ODI_PIOInfo;          /* PIO information                    */
   unsigned short ODI_HWFlags;          /* hardware specific option flags     */
   unsigned short ODI_Reserved4;        /* Reserved                           */
   unsigned char  ODI_Revision;         /* Revision level of DPT Extension    */
   unsigned char  ODI_Reserved5;        /* Reserved                           */
} OEMHLPDISKINFO;

/* NOINC */
#pragma pack()
/* INC */

#endif /* __BSEDEV__ */

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
