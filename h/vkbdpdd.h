/***    vkbdpdd.h
 *
 *
 *      Keyboard PDD/VDD shared structures/constants
 *      Copyright (c) 1992 IBM Corporation
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


#ifndef __VKBDPDD__
#define __VKBDPDD__


/* KBD PDD component definitions */

#define KBD_NAME                "KBD$"


/***    PDD function commands
 *
 *  Format:
 *      (*fpfnPKVDDProc)(PKBDCMD_*, f16p1, f16p2);
 *
 *  PKBDCMD_REGISTER:
 *      f16p1 == segment of 16:32 VDD entry point
 *      f16p2 ==  offset of 16:32 VDD entry point
 *
 *  PKBDCMD_DEREGISTER:
 *      f16p1 == F16PNULL
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_VDMCREATED:
 *      f16p1 -> KSGID      (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_QUERYID:
 *      f16p1 == F16PNULL
 *      f16p2 -> KID        (see packet below)
 *
 *  PKBDCMD_QUERYINHIBIT:
 *      f16p1 == F16PNULL
 *      f16p2 -> KINH       (see packet below)
 *
 *  PKBDCMD_SETLEDS:
 *      f16p1 -> KLEDS      (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_SETREPEATRATE:
 *      f16p1 -> KRPT       (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_TRANSLATE:
 *      f16p1 -> KXLT       (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_VDMTERMINTED:
 *      f16p1 -> KSGID      (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_FOCUS:
 *      f16p1 -> KFOCUS     (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_QUERYREVXLATE:
 *      f16p1 -> KQRX       (see packet below)
 *      f16p2 == F16PNULL
 *
 *  PKBDCMD_QUERYTYPE:
 *      f16p1 == F16PNULL
 *      f16p2 -> KTYPE      (see packet below)
 */

#define PKBDCMD_REGISTER        PDDCMD_REGISTER      /* register VDD              */
#define PKBDCMD_DEREGISTER      (PKBDCMD_REGISTER+1) /* de-register VDD           */
#define PKBDCMD_VDMCREATED      (PKBDCMD_REGISTER+2) /* notify VDM is created     */
#define PKBDCMD_QUERYID         (PKBDCMD_REGISTER+3) /* query physical kbd ID     */
#define PKBDCMD_QUERYINHIBIT    (PKBDCMD_REGISTER+4) /* query inhibit state       */
#define PKBDCMD_SETLEDS         (PKBDCMD_REGISTER+5) /* set physical LEDs         */
#define PKBDCMD_SETREPEATRATE   (PKBDCMD_REGISTER+6) /* set physical repeat rate  */
#define PKBDCMD_TRANSLATE       (PKBDCMD_REGISTER+7) /* translate a scan code     */
#define PKBDCMD_VDMTERMINATED   (PKBDCMD_REGISTER+8) /* notify VDM termination    */
#define PKBDCMD_KFOCUS          (PKBDCMD_REGISTER+9) /* notify of focus change    */
#define PKBDCMD_QUERYREVXLATE   (PKBDCMD_REGISTER+10)/* query reverse xlate table */
#define PKBDCMD_HOTKEY          (PKBDCMD_REGISTER+11)/* notify of Hot Key setting */
#define PKBDCMD_QUERYTYPE       (PKBDCMD_REGISTER+12)/* query KBD Type            */
#define PKBDCMD_SETXMODE        (PKBDCMD_REGISTER+13)/* Set xlate mode            */

typedef struct ksgid_s {
    USHORT  ksgid_len;                  /* length of packet (4)*/
    SGID    ksgid_sgid;                 /* screen group ID     */
} KSGID;

typedef KSGID *PKSGID;

/*c-begin*/
#define KSGID_PKT_LEN   sizeof(KSGID)
/*c-end*/


typedef struct kid_s {
    USHORT kid_len;                     /* length of packet (4)*/
    USHORT kid_ID;                      /* keyboard ID         */
} KID;

typedef KID *PKID;

/*c-begin*/
#define KID_PKT_LEN     sizeof(KID)*/
/*c-end*/


typedef struct kinh_s {
    USHORT kinh_len;                    /* length of packet (3)*/
    BBOOL  kinh_fInhibited;             /* TRUE if inhibited, FALSE if not*/
} KINH;

typedef KINH *PKINH;

/*c-begin*/
#define KINH_PKT_LEN    sizeof(KINH)
/*c-end*/


typedef struct kled_s {
    USHORT kled_len;                    /* length of packet (4)*/
    USHORT kled_fsLEDs;                 /* bit0:SCROLL; bit1:NUM; bit2:CAPS*/
} KLEDS;

typedef KLEDS *PKLEDS;

/*c-begin
#define KLEDS_PKT_LEN   sizeof(KLEDS)
/*c-end*/
/*masm-begin*/
/*KLEDS_PKT_LEN EQU     SIZE kled_s*/
/*masm-end*/

typedef struct khotkey_s {
    USHORT khk_len;
    USHORT khk_action;
    ULONG khk_hotkeys;
} KHOTKEYS;

typedef KHOTKEYS *PKHOTKEYS;

/*c-begin*/
#define KHOTKEYS_PKT_LEN   sizeof(KHOTKEYS)
/*c-end*/

typedef struct krpt_s {
    USHORT krpt_len;                    /* length of packet (6)*/
    USHORT krpt_usDelay;                /* delay, in ms. (0 implies default)*/
    USHORT krpt_usRate;                 /* rate, in cps (0 implies default)*/
} KRPT;

typedef KRPT *PKRPT;

/*c-begin*/
#define KRPT_PKT_LEN    sizeof(KRPT)
/*c-end*/

typedef struct kkey_s {
    UCHAR  key_len;                     /* length of packet (14)                */
    UCHAR  key_chOrgScan;               /* original scan code                   */
    UCHAR  key_chChar;                  /* ASCII code (output)                  */
    UCHAR  key_chScan;                  /* scan code (input)                    */
    UCHAR  key_fbStatus;                /* (output, normally KEYSTATUS_FINAL)   */
    UCHAR  key_bNLSShift;               /* (reserved, set to zero on input)     */
    USHORT key_fsState;                 /* shift states (input/output)          */
    ULONG  key_time;                    /* timestamp (ignored)                  */
    USHORT key_fsDDFlags;               /* device driver/monitor flags (output) */
} KKEY;

typedef KKEY *PKKEY;
typedef KKEY FAR16 *F16PKKEY;

/*c-begin*/
#define KKEY_PKT_LEN    sizeof(KKEY)
/*c-end*/


typedef struct kxf_s {
    USHORT kxf_len;                     /* length of packet (11)                */
    UCHAR  kxf_fbXlate;                 /* translation flags                    */
    UCHAR  kxf_fbKey;                   /* key flags                            */
    UCHAR  kxf_fbSpec;                  /* multi-keypacket events               */
    UCHAR  kxf_fbMode;                  /* input mode                           */
    UCHAR  kxf_fbAltNum;                /* Alt-keypad accumulator               */
    UCHAR  kxf_fbDshift;                /* Dshift down information              */
    USHORT kxf_fsHotKey;                /* interrupt-time "hot key" shift state */
    UCHAR  kxf_fbAccent;                /* DCR 1546 Extended Accent use         */
} KXF;

typedef KXF *PKXF;
typedef KXF FAR16 *F16PKXF;

/*c-begin*/
#define KXF_PKT_LEN     sizeof(KXF)
/*c-end*/


typedef struct kxlt_s {
    USHORT   kxlt_len;                  /* length of packet (16)            */
    F16PKKEY kxlt_f16pKey1;             /* pointer to key pkt 1             */
    F16PKKEY kxlt_f16pKey2;             /* pointer to key pkt 2             */
    F16PKXF  kxlt_f16pKxf;              /* pointer to translation flags pkt */
    USHORT   kxlt_usCodePage;           /* codepage to use                  */
} KXLT;

typedef KXLT *PKXLT;

/*c-begin*/
#define KXLT_PKT_LEN    sizeof(KXLT)
/*c-end*/

typedef struct kfocus_s {
    USHORT kf_len;                    /* length of packet (4)*/
    USHORT kf_fsFocus;                /* TRUE (set) FALSE (free) focus*/
} KFOCUS;

typedef KFOCUS *PKFOCUS;

/*c-begin*/
#define KFOCUS_PKT_LEN sizeof(KFOCUS)
/*c-end*/

typedef struct rxentry_s {
    UCHAR   rx_chScan;
    UCHAR   rx_fbRXFlags;
} RXENTRY;

typedef RXENTRY *PRX;
typedef RXENTRY FAR16 *F16PRX;

typedef struct shentry_s {
    USHORT  sh_fsShift;
    UCHAR   sh_scanShift;
    UCHAR   sh_scanPrefix;
} SHENTRY;

typedef SHENTRY *PSH;
typedef SHENTRY FAR16 *F16PSH;

/*c-begin*/
#define SHENTRY_SIZE sizeof(SHENTRY)
/*c-end*/

typedef struct ktype_s {
    USHORT kt_len;                      // length of packet (8)
    USHORT kt_type;                     // keyboard type
    UCHAR  kt_id1;                      // keyboard ID1
    UCHAR  kt_id2;                      // keyboard ID2
    UCHAR  kt_id3;                      // keyboard ID3
    UCHAR  kt_res;                      // reserved 0
} KTYPE;

typedef KTYPE *PKTYPE;

/*c-begin*/
#define KT_PKT_LEN     sizeof(KTYPE)
/*c-end*/

typedef struct pkmode_s {
    USHORT pk_len;                    // length of packet (4)
    USHORT pk_PMode;                  // physical keyboard mode
} PKBDMODE;

/*c-begin*/
#define PKMODE_PKT_LEN  sizeof(PKBDMODE)
/*c-end*/

typedef UCHAR *PNUM;
typedef UCHAR FAR16 *F16PNUM;

#define NUM_RXENTRIES   256     /*max. entries in rev. xlate table */
#define NUM_SHENTRIES   6       /*max. entries in shift table      */
#define NUM_NUMENTRIES  10      /*max. entries in num scan table   */

typedef RXENTRY RXTABLE[NUM_RXENTRIES];
typedef SHENTRY SHTABLE[NUM_SHENTRIES];
typedef UCHAR   NUMTABLE[NUM_NUMENTRIES];

/* RXFlags equates */

#define RXFLAGS_NONKEY          0
#define RXFLAGS_LOWCASE         1
#define RXFLAGS_UPCASE          2
#define RXFLAGS_NOSHIFT         3
#define RXFLAGS_SHIFT           4
#define RXFLAGS_CTRL            5

/* Shift Table indexes */

#define SHINDX_LSHIFT   0
#define SHINDX_LCTRL    1
#define SHINDX_LALT     2
#define SHINDX_RSHIFT   3
#define SHINDX_RCTRL    4
#define SHINDX_RALT     5

typedef struct kqrx_s {
    USHORT  kqrx_len;                   /*length of packet (18)              */
    USHORT  kqrx_usCodePage;            /*code page number                   */
    USHORT  kqrx_hCodePage;             /*codepage table handle              */
    F16PRX  kqrx_f16pRXTable;           /*pointer to rev. xlate table        */
    F16PSH  kqrx_f16pShTable;           /*pointer to shift table             */
    F16PNUM kqrx_f16pNumTable;          /*pointer to table for num pad scans */
} KQRX;

typedef KQRX *PKQRX;

/*c-begin*/
#define KQRX_PKT_LEN sizeof(KQRX)
/*c-end*/


/***    VDD function commands
 *
 *  Format:
 *      (*fpfnVKPDDProc)(VKBDCMD_*, f16p1, f16p2);
 *
 *  VKBDCMD_INTEVENT:
 *      f16p1 -> KINT       (see packet below)
 *      f16p2 == NULL
 *
 *  VKBDCMD_CPCHANGED:
 *      f16p1 -> KCPC       (see packet below)
 *      f16p2 == NULL
 *
 *  VKBDCMD_HOTPLUG
 *      f16p1 -> KHP        (see packet below)
 *      f16p2 == NULL
 */

#define VKBDCMD_INTEVENT        0x01    /* to notify of a scan code          */
#define VKBDCMD_CPCHANGED       0x02    /* to notify of a codepage changed   */
#define VKBDCMD_HOTPLUG         0x03    /* to notify of a hot plug occurance */
#define VKBDCMD_ALTESC          0x04    /* to nofity of an ALT-ESC hotkey    */

typedef struct kint_s {
    USHORT kint_len;                    /* length of packet */
    USHORT kint_chScan;                 /* scan code        */
    USHORT kint_usSGID;                 /* screen group ID  */
} KINT;

typedef KINT *PKINT;

/*c-begin*/
#define KINT_PKT_LEN    sizeof(KINT)
/*c-end*/

typedef struct kcpc_s {
    USHORT kcpc_len;                    /* length of packet        */
    USHORT kcpc_usCodePage;             /* code page being changed */
    USHORT kcpc_hCodePage;              /* code page handle        */
} KCPC;

typedef KCPC *PKCPC;

/*c-begin*/
#define KCPC_PKT_LEN    sizeof(KCPC)
/*c-end*/

typedef struct khp_s {
    USHORT khp_len;                     /* length of packet       */
    USHORT khp_usKbdHWID;               /* current keyboard hw ID */
} KHP;

typedef KHP *PKHP;

/*c-begin*/
#define KHP_PKT_LEN    sizeof(KHP)
/*c-end*/


/* Flags for various fields in the XLTPKT structure */

#define KEYSTATUS_FINAL         0x40    /* key is final */

#define KEYSTATE_RIGHTSHIFT     BIOSKFLAG_RIGHTSHIFT
#define KEYSTATE_LEFTSHIFT      BIOSKFLAG_LEFTSHIFT
#define KEYSTATE_EITHERCTRL     BIOSKFLAG_CTRL
#define KEYSTATE_EITHERALT      BIOSKFLAG_ALT
#define KEYSTATE_SCROLLLOCK     BIOSKFLAG_SCROLLLOCK
#define KEYSTATE_NUMLOCK        BIOSKFLAG_NUMLOCK
#define KEYSTATE_CAPSLOCK       BIOSKFLAG_CAPSLOCK
#define KEYSTATE_INSERT         BIOSKFLAG_INSERT
#define KEYSTATE_LEDMASK        (KEYSTATE_SCROLLLOCK + \
                                 KEYSTATE_NUMLOCK    + \
                                 KEYSTATE_CAPSLOCK)

#define KEYSTATE_LEFTCTRL       (BIOSKFLAG1_LEFTCTRL   * 0x0100)
#define KEYSTATE_LEFTALT        (BIOSKFLAG1_LEFTALT    * 0x0100)
#define KEYSTATE_RIGHTCTRL      (BIOSKFLAG3_RIGHTCTRL  * 0x0100)
#define KEYSTATE_RIGHTALT       (BIOSKFLAG3_RIGHTALT   * 0x0100)
#define KEYSTATE_SCROLLDOWN     (BIOSKFLAG1_SCROLLDOWN * 0x0100)
#define KEYSTATE_NUMDOWN        (BIOSKFLAG1_NUMDOWN    * 0x0100)
#define KEYSTATE_CAPSDOWN       (BIOSKFLAG1_CAPSDOWN   * 0x0100)
#define KEYSTATE_SYSREQDOWN     (BIOSKFLAG1_SYSSHIFT   * 0x2000)

#define KEYSTATE_LRSHIFTS       (KEYSTATE_LEFTSHIFT + KEYSTATE_RIGHTSHIFT + \
                                 KEYSTATE_LEFTCTRL  + KEYSTATE_RIGHTCTRL  + \
                                 KEYSTATE_LEFTALT   + KEYSTATE_RIGHTALT)

#define KEYSTATE_ALLSHIFTS      (KEYSTATE_LRSHIFTS + \
                                 KEYSTATE_EITHERCTRL + KEYSTATE_EITHERALT)

#define KEYFLAGS_NORMAL         0x0000  /* any normal key                        */
#define KEYFLAGS_ACK            0x0001  /* eg, SCAN_ACK                          */
#define KEYFLAGS_SECPREFIX      0x0002  /* eg, 0E0h, 0E1h                        */
#define KEYFLAGS_OVERRUN        0x0003  /* eg, SCAN_OVERRUN                      */
#define KEYFLAGS_RESEND         0x0004  /* eg, SCAN_RESEND                       */
#define KEYFLAGS_REBOOT         0x0005  /* never returned (PDD reboots)          */
#define KEYFLAGS_DUMP           0x0006  /* eg, Ctrl-Alt-NumLock (ignore)         */
#define KEYFLAGS_SHIFT          0x0007  /* eg, Shift, Ctrl, CapsLock, etc.       */
#define KEYFLAGS_PAUSE          0x0008  /* eg, Pause or Ctrl-Numlock             */
#define KEYFLAGS_PSPAUSE        0x0009  /* never returned (we're in RAW mode)    */
#define KEYFLAGS_WAKEUP         0x000A  /* only after PAUSE, never PSPAUSE       */
#define KEYFLAGS_BADACCENT      0x000B  /* eg, unaccentable key                  */
#define KEYFLAGS_HOTKEY         0x000C  /*                                       */
#define KEYFLAGS_READPEEK       0x000D  /*                                       */
#define KEYFLAGS_ACCENT         0x0010  /*                                       */
#define KEYFLAGS_BREAK          0x0011  /* eg, Ctrl-Break                        */
#define KEYFLAGS_PSBREAK        0x0012  /* never returned (we're in RAW mode)    */
#define KEYFLAGS_PRTSC          0x0013  /* eg, PrtSc                             */
#define KEYFLAGS_PRTECHO        0x0014  /* eg, Ctrl-PrtSc                        */
#define KEYFLAGS_PSPRTECHO      0x0015  /* never returned (we're in RAW mode)    */
#define KEYFLAGS_PRTFLUSH       0x0016  /* eg, Ctrl-Alt-PrtSc                    */
#define KEYFLAGS_SYSREQ         0x0017  /* eg, SysReq                            */
#define KEYFLAGS_UNDEFINED      0x003F  /* undefined (we map ignored keys here)  */
#define KEYFLAGS_TYPEMASK       0x003F  /* key type mask                         */

#define KEYFLAGS_RELEASED       0x0040  /* this was a release, not a press       */
#define KEYFLAGS_SECONDARY      0x0080  /* key was preceded by SECPREFIX         */
#define KEYFLAGS_MULTIMAKE      0x0100  /* ie, repeat of a "toggle" key          */
#define KEYFLAGS_USEDACCENT     0x0200  /* key translated using previous accent  */

/* Flags for various fields in the KXF structure */                              rt

#define KXFXLATE_ACCENTMASK     0x07    /* previous accent # saved here          */
#define KXFXLATE_SGPAUSED       0x08    /* session currently paused              */
#define KXFXLATE_USECHAR3       0x10    /* use CHAR3 as index into accent tbl    */
#define KXFXLATE_CTRLEMULATED   0x20    /* CTRL key emulated by enhanced kbd     */
#define KXFXLATE_E1PREFIX       0x40    /* E1 prefix just seen                   */
#define KXFXLATE_ACCENTINDEX3   0x80    /* chr3 of XlateTable index accent table */

#define KXFKEY_DUMPKEYONCE      0x01    /* dump key sequence has been hit once   */
#define KXFKEY_PRTSCDOWN        0x02    /* PrtSc is down now                     */
#define KXFKEY_SECPREFIX        0x04    /* E0 or E1 prefix just seen             */
#define KXFKEY_NORMALALTDOWN    0x08    /* normal Alt down (not AltGraph)        */

#define KXFSPEC_ALT2PAC         0x01    /* alt-pad: use two xlation key packets  */
#define KXFSPEC_E02PAC          0x02    /* E0: use two xlation key packets       */
#define KXFSPEC_ACCENT2PAC      0x04    /* accent: use two xlation key packets   */
#define KXFSPEC_ACCENT1PAC      0x08    /* accent: use one xlation key packets   */
#define KXFSPEC_USE2PAC         0x80    /* use two xlation key packets           */

#define KXFMODE_SHIFTREPORT     0x01    /* not needed for VDM support            */
#define KXFMODE_BINARY          0x80    /* set for normal VDM translation        */

#endif /* __VKBDPDD__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
