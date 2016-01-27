/********************************************************************
 *
 * Name: unikbd.h
 *
 * Function:
 *     Declare constants and functions prototypes used in keyboard
 *     translation.
 *
 * Copyright:
 *     Copyright (C) IBM Corp. 1996
 *
 * Notes:
 *
 *
 *
 ********************************************************************/

#ifndef __UNIKBD_INCLUDED__
  #define __UNIKBD_INCLUDED__

  #ifdef __cplusplus
  extern "C" {
  #endif

  #include <unidef.h>

  #define KBDLINK  CALLCONV

  /*
   * This should work from either OS/2 or CPS, so we need to declare
   * some OS/2 like constants, but only if OS2DEF is not already
   * included.
   */
  #ifndef OS2DEF_INCLUDED
    typedef unsigned short USHORT;
    typedef unsigned long  ULONG;
    typedef unsigned int   UINT;
    typedef unsigned int   APIRET;
    typedef unsigned char  BYTE;
    #define LOBYTE(w)      LOUCHAR(w)
    #define HIBYTE(w)      HIUCHAR(w)
    #define LOUCHAR(w)     ((UCHAR)(USHORT)(w))
    #define HIUCHAR(w)     ((UCHAR)(((USHORT)(w) >> 8) & 0xff))
    #define MAKEULONG(l,h) ((ULONG)(((USHORT)(l)) | ((ULONG)((USHORT)(h)))<<16))
  #endif

  /*
   * Simple types
   */
  typedef unsigned int    KHAND;   /* Handle to translate table */
  typedef unsigned char   VSCAN;   /* Virtual scan code */
  typedef unsigned short  VDKEY;   /* Virtual/Dead key  */
  typedef UniChar         KBDNAME; /* Keyboard name (UniChar) */


  /*
   * Define virtual keys.  This is mostly unnecessary if PMWIN is included
   */
  #ifndef INCL_WININPUT
    #define INCL_WININPUT
    #define  VK_BREAK            0x0004
    #define  VK_BACKSPACE        0x0005
    #define  VK_TAB              0x0006
    #define  VK_BACKTAB          0x0007
    #define  VK_NEWLINE          0x0008
    #define  VK_SHIFT            0x0009
    #define  VK_CTRL             0x000A
    #define  VK_ALT              0x000B
    #define  VK_ALTGRAF          0x000C
    #define  VK_PAUSE            0x000D
    #define  VK_CAPSLOCK         0x000E
    #define  VK_ESC              0x000F
    #define  VK_SPACE            0x0010
    #define  VK_PAGEUP           0x0011
    #define  VK_PAGEDOWN         0x0012
    #define  VK_END              0x0013
    #define  VK_HOME             0x0014
    #define  VK_LEFT             0x0015
    #define  VK_UP               0x0016
    #define  VK_RIGHT            0x0017
    #define  VK_DOWN             0x0018
    #define  VK_PRINTSCRN        0x0019
    #define  VK_INSERT           0x001A
    #define  VK_DELETE           0x001B
    #define  VK_SCRLLOCK         0x001C
    #define  VK_NUMLOCK          0x001D
    #define  VK_ENTER            0x001E
    #define  VK_SYSRQ            0x001F
    #define  VK_F1               0x0020
    #define  VK_F2               0x0021
    #define  VK_F3               0x0022
    #define  VK_F4               0x0023
    #define  VK_F5               0x0024
    #define  VK_F6               0x0025
    #define  VK_F7               0x0026
    #define  VK_F8               0x0027
    #define  VK_F9               0x0028
    #define  VK_F10              0x0029
    #define  VK_F11              0x002A
    #define  VK_F12              0x002B
    #define  VK_F13              0x002C
    #define  VK_F14              0x002D
    #define  VK_F15              0x002E
    #define  VK_F16              0x002F
    #define  VK_F17              0x0030
    #define  VK_F18              0x0031
    #define  VK_F19              0x0032
    #define  VK_F20              0x0033
    #define  VK_F21              0x0034
    #define  VK_F22              0x0035
    #define  VK_F23              0x0036
    #define  VK_F24              0x0037
    #define  VK_CLEAR            0x0039
    #define  VK_EREOF            0x003A
    #define  VK_PA1              0x003B
    #define  VK_ATTN             0x003C
    #define  VK_CRSEL            0x003D
    #define  VK_EXSEL            0x003E
    #define  VK_COPY             0x003F
    #define  VK_BLK1             0x0040
    #define  VK_BLK2             0x0041
  #endif

  /* Additional vkeys not defined by PMWIN */
  #define  VK_PA2              0x0050
  #define  VK_PA3              0x0051
  #define  VK_GROUP            0x0052
  #define  VK_GROUPLOCK        0x0053
  #define  VK_APPL             0x0054
  #define  VK_WINLEFT          0x0055
  #define  VK_WINRIGHT         0x0056
  #define  VK_M_DOWNLEFT       0x0061
  #define  VK_M_DOWN           0x0062
  #define  VK_M_DOWNRIGHT      0x0063
  #define  VK_M_LEFT           0x0064
  #define  VK_M_CENTER         0x0065
  #define  VK_M_RIGHT          0x0066
  #define  VK_M_UPLEFT         0x0067
  #define  VK_M_UP             0x0068
  #define  VK_M_UPRIGHT        0x0069
  #define  VK_M_BUTTONLOCK     0x006A
  #define  VK_M_BUTTONRELEASE  0x006B
  #define  VK_M_DOUBLECLICK    0x006C
  /* There are additional NLS vkeys */


  /*
   * Deadkey mappings
   */
  #define  DK_MIN              0x1000    /* Min deadkey */
  #define  DK_ACUTE            0x1001
  #define  DK_GRAVE            0x1002
  #define  DK_DIERESIS         0x1003
  #define  DK_UMLAUT           0x1003    /* Alternate name */
  #define  DK_CIRCUMFLEX       0x1004
  #define  DK_TILDE            0x1005
  #define  DK_CEDILLA          0x1006
  #define  DK_MACRON           0x1007
  #define  DK_BREVE            0x1008
  #define  DK_OGONEK           0x1009
  #define  DK_DOT              0x100a
  #define  DK_BAR              0x100b
  #define  DK_RING             0x100c
  #define  DK_CARON            0x100d
  #define  DK_HACEK            0x100d             /* Alternate name */
  #define  DK_HUNGARUMLAUT     0x100e
  #define  DK_ACUTEDIA         0x100f
  #define  DK_PSILI            0x1010
  #define  DK_DASIA            0x1011
  #define  DK_OVERLINE         0x1012
  #define  DK_UNDERDOT         0x1013

  #define  DK_MAX              0x1fff    /* Max deadkey */

  /*
   * Shift states
   */
  typedef struct  {             /* Keyboard shift state object */
      ULONG  Shift;             /* Actual shift and lock state */
      ULONG  Effective;         /* Effective shift and lock state */
      ULONG  Led;               /* Keyboard indicators */
  } USHIFTSTATE;

  /* Both Shift and Effective */
  #define KBD_SHIFT            0x00000001
  #define KBD_CONTROL          0x00000002
  #define KBD_ALT              0x00000004
  #define KBD_ALTCTRLSHIFT     0x00000007
  #define KBD_ALTGR            0x00000008
  #define KBD_NLS1             0x00000010     /* NLS lock states */
  #define KBD_NLS2             0x00000020
  #define KBD_NLS3             0x00000040
  #define KBD_NLS4             0x00000080

  /* Japanese use of NLS shifts */
  #define KBD_WIDE             KBD_NLS1
  #define KBD_KATAKANA         KBD_NLS2
  #define KBD_HIRAGANA         KBD_NLS3
  #define KBD_ROMANJI          KBD_NLS4
  /* Korean use of NLS shifts */
  #define KBD_JAMO             KBD_NLS2
  #define KBD_HANGEUL          KBD_NLS3
  #define KBD_HANJACSR         KBD_NLS4
  /* Taiwan use of NLS shifts */
  #define KBD_PHONETIC         KBD_NLS2
  #define KBD_TSANGJYE         KBD_NLS3

  /* Lock states */
  #define KBD_SCROLLLOCK       0x00000100
  #define KBD_NUMLOCK          0x00000200
  #define KBD_CAPSLOCK         0x00000400
  #define KBD_EXTRALOCK        0x00000800
  #define KBD_APPL             0x00001000     /* MS Application key */
  #define KBD_DBCS             0x00008000     /* DBCS active        */

  #define KBD_EFFECTIVE        0x0000ffff

  /* Left/Right shift separation */
  #define KBD_LEFTSHIFT        0x00010000
  #define KBD_RIGHTSHIFT       0x00020000
  #define KBD_LEFTCONTROL      0x00040000
  #define KBD_RIGHTCONTROL     0x00080000
  #define KBD_LEFTALT          0x00100000
  #define KBD_RIGHTALT         0x00200000
  #define KBD_LEFTWINDOWS      0x00400000
  #define KBD_RIGHTWINDOWS     0x00800000

  /* Status bits and additional LEDs */
  #define KBD_NOROMANJI        0x04000000
  #define KBD_KANJI            0x08000000
  #define KBD_DEADKEY          0x10000000
  #define KBD_WAIT             0x20000000
  #define KBD_HOLD             0x40000000
  #define KBD_LOCK             0x80000000

  /*
   * Define Make/Break constants
   */
  #define KEYEV_MAKEBREAK 0     /* Both make and break */
  #define KEYEV_MAKE      1     /* Key down            */
  #define KEYEV_BREAK     2     /* Key up              */
  #define KEYEV_REPEAT    3     /* Repeat key          */

  /*
   *  Virtual key event.
   */
  typedef struct {
      USHORT ldev;             /* Logical device 0=real */
      BYTE   makebreak;        /* Make/break indicator  */
      VSCAN  scan;             /* Virtual scan code     */
      ULONG  time;             /* Timestamp             */
  } INKEYEVENT;

  /*
   * Query keyboard structure
   */
  typedef struct {
      ULONG   len;             /* Length of structrue */
      USHORT  kbid;            /* Keyboard architecture id */
      USHORT  version;         /* Version number */
      BYTE    language[2];     /* Normal language          */
      BYTE    country[2];      /* Normal country           */
      USHORT  flags;           /* Flags (KBDF_)  */
      USHORT  resv;            /* Reserved       */
      UniChar description[32]; /* Description of keyboard */
  } KEYBOARDINFO;

  /*
   * Query keyboard flags
   */
  #define  KBDF_DEFAULTVKEY    0x0001    /* Use default VKEYs */
  #define  KBDF_NOCTRLSHIFT    0x0002    /* Ctrl+Shift equals Ctrl */
  #define  KBDF_NOALTGR        0x0004    /* Alt graphics is not used */
  #define  KBDF_SHIFTALTGR     0x0010    /* Altgr, shift-altgr separate */
  #define  KBDF_DEADGOOD       0x0020    /* Invalid dead use second char*/
  #define  KBDF_DEADPRIVATE    0x0040    /* Use only private dead keys  */

  #define  KBDF_SYSTEM         0x8000    /* System supplied keyboard */
  #define  KBDF_INTERNATIONAL  0x4000    /* Full-range character set */
  #define  KBDF_DVORAK         0x2000    /* Alternate letter keys */
  #define  KBDF_NATIONAL       0x1000    /* National letter keys */
  #define  KBDF_LETTERKEYS     0x3000    /* Letter key type */
  #define  KBDF_ISOKEYS        0x0800    /* Use ISO icons for key names */

  /* These layouts are advisory only */
  #define  KBDF_LAYOUT101      0x0000    /* Normal layout is 84/101 */
  #define  KBDF_LAYOUT102      0x0100    /* Normal layout is 85/102 */
  #define  KBDF_LAYOUT106      0x0200    /* Normal layout is 89/106 */
  #define  KBDF_LAYOUT103      0x0300    /* Normal layout is 86/103 */
  #define  KBDF_LAYOUT100      0x0400    /* Normal layout is 83/100 */
  #define  KBDF_LAYOUTS        0x0700    /* Layout related bits     */
  /*
   * Reset ShiftState types
   */
  #define KEYEV_SET      0     /* Set to specified value */
  #define KEYEV_RELEASE  1     /* Release all pressed keys */
  #define KEYEV_ZERO     2     /* Release all pressed and locked keys */


  /*
   *  Unicode keyboard API function prototypes
   */
  APIRET  CALLCONV UniCreateKeyboard(KHAND   * pkhand,
                                    KBDNAME * name,
                                    ULONG     mode);

  APIRET  CALLCONV UniDestroyKeyboard(KHAND khand);

  APIRET  CALLCONV UniQueryKeyboard(KHAND          khand,
                                   KEYBOARDINFO * kbdinfo);

  APIRET  CALLCONV UniResetShiftState(KHAND        khand,
                                     USHIFTSTATE * state,
                                     ULONG        type);

  APIRET  CALLCONV UniUpdateShiftState(KHAND        khand,
                                      USHIFTSTATE * state,
                                      VSCAN        scan,
                                      BYTE         makebreak);

  APIRET  CALLCONV UniTranslateKey(KHAND      khand,
                                  ULONG      eshift,
                                  VSCAN      scan,
                                  UniChar  * unichar,
                                  VDKEY    * vdkey,
                                  BYTE     * bscan);

  APIRET  CALLCONV UniTranslateDeadkey(KHAND      khand,
                                      VDKEY      dead,
                                      UniChar    inchar,
                                      UniChar  * outchar,
                                      VDKEY    * newdeadkey);

  APIRET  CALLCONV UniUntranslateKey(KHAND    khand,
                                    UniChar  unichar,
                                    VDKEY    vdkey,
                                    VSCAN  * pscan,
                                    ULONG  * eshift);

  #ifdef __cplusplus
  }
  #endif
#endif

