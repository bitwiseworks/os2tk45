/****************************** Module Header ******************************\
*
* Module Name: OS2NLS.H
*
* OS/2 Presentation Manager DBCS include file
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The definitions in this file are for use on Double Byte Character Set
* (DBCS) versions of OS/2 only. They may not be used on SBCS versions.
*
* This is included from PMWIN.H when INCL_NLS symbol is defined
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


/* NOINC */
#ifndef __OS2NLS__
   /* INC */
   #define __OS2NLS__
   
   /* resource ID offset for bi-lingual system resources (menu & string) */
   
   #define RID_DBE_OFFSET          0x1000
   #define STR_DBE_OFFSET          0x1000
   
   
   /* definition of Virtual key values for DBCS */
   
   #define VK_DBE_FIRST            0x80 /* VK_DBCSFIRST */
   #define VK_DBE_LAST             0xff /* VK_DBCSLAST  */
   
   #define VK_DBE_IMEFIRST         0xA0 /* VK_DBE_FIRST + 0x20 */
   #define VK_DBE_IMELAST          0xFF /* VK_DBE_LAST */
   
   /* OEM keyboard support including PS/2 keyboard */
   
   #define VK_DBE_OEMFIRST         0xC0
   #define VK_DBE_OEMLAST          0xCF
   
   /* for codepage 932 */
   
   #define VK_DBE_ALPHANUMERIC     0x80 /* VK_DBE_FIRST + 0x00 */
   #define VK_DBE_KATAKANA         0x81 /* VK_DBE_FIRST + 0x01 */
   #define VK_DBE_HIRAGANA         0x82 /* VK_DBE_FIRST + 0x02 */
   #define VK_DBE_SBCSCHAR         0x83 /* VK_DBE_FIRST + 0x03 */
   #define VK_DBE_DBCSCHAR         0x84 /* VK_DBE_FIRST + 0x04 */
   #define VK_DBE_SBCSDBCSCHAR     0x85 /* VK_DBE_FIRST + 0x05 */
   #define VK_DBE_ROMAN            0x86 /* VK_DBE_FIRST + 0x06 */
   
   /* for codepage 934 */
   
   #define VK_DBE_HANJA            0x87 /* VK_DBE_FIRST + 0x07 */
   #define VK_DBE_HANGEUL          0x88 /* VK_DBE_FIRST + 0x08 */
   #define VK_DBE_JAMO             0x89 /* VK_DBE_FIRST + 0x09 */
   
   /* for codepage 936 */
   
   #define VK_DBE_HANZI            0x8A /* VK_DBE_FIRST + 0x0A */
   
   /* for codepage 938 */
   
   #define VK_DBE_TSANGJYE         0x8C /* VK_DBE_FIRST + 0x0C */
   #define VK_DBE_PHONETIC         0x8D /* VK_DBE_FIRST + 0x0D */
   
   #define VK_DBE_KANJI            0xa0 /* VK_DBE_IMEFIRST + 0x00 */
   #define VK_DBE_IMEACTIVATE      0xa0 /* VK_DBE_IMEFIRST + 0x00 */
   #define VK_DBE_CONV             0xa1 /* VK_DBE_IMEFIRST + 0x01 */
   #define VK_DBE_NOCONV           0xa2 /* VK_DBE_IMEFIRST + 0x02 */
   #define VK_DBE_TANGO            0xa3 /* VK_DBE_IMEFIRST + 0x03 */
   #define VK_DBE_WORDREGISTER     0xa3 /* VK_DBE_IMEFIRST + 0x03 */
   #define VK_DBE_IMEDIALOG        0xa4 /* VK_DBE_IMEFIRST + 0x04 */
   #define VK_DBE_IME_MBSELECTED   0xa5 /* VK_DBE_IMEFIRST + 0x05 */
   #define VK_DBE_IME_LBSELECTED   0xa6 /* VK_DBE_IMEFIRST + 0x06 */
   #define VK_DBE_CODEINPUT        0xa7 /* VK_DBE_IMEFIRST + 0x07 */
   #define VK_DBE_ALLCANDIDATE     0xa8 /* VK_DBE_IMEFIRST + 0x08 */
   #define VK_DBE_PREVCANDIDATE    0xa9 /* VK_DBE_IMEFIRST + 0x09 */
   #define VK_DBE_IMEDEACTIVATE    0xaa /* VK_DBE_IMEFIRST + 0x0a */
   #define VK_DBE_IME_WORDREGISTED 0xab /* VK_DBE_IMEFIRST + 0x0b */
   #define VK_DBE_IMECONTROL       0xac /* VK_DBE_IMEFIRST + 0x0c */
   #define VK_DBE_IME_FREEPS       0xad /* VK_DBE_IMEFIRST + 0x0d */
   #define VK_DBE_IMEMODEEXIT      0xae /* VK_DBE_IMEFIRST + 0x0e */
   
   /* Window Class definitoin for DBCS (migration) */
   
   #define WC_DBE_APPSTAT          ((PSZ)0xffff0010L) /* WC_APPSTAT */
   #define WC_DBE_KBDSTAT          ((PSZ)0xffff0011L) /* WC_KBDSTAT */
   #define WC_DBE_PECIC            ((PSZ)0xffff0012L) /* WC_PECIC   */
   
   /* Double byte character set messages */
   
   #define WM_DBE_SETAPPLSTAT      0x00b6
   #define WM_DBE_KKCPARAMS        0x00bc
   
   /* DBCS unique statndsrd Frame Create Flag (migration) */
   
   #define FCF_DBE_STATAREA        0x80000000L /* FCF_DBE_APPSTAT */
   
   /* WM_SETWINDOWPARAM ctldata of DBCS status area */
   
   typedef struct _KEYSTSDATA          /* keysts */
   {
      ULONG       wpmctlflag;
      ULONG       textlength;
      PSZ         pTextString;
      PSZ         pAttrString;
      PSZ         pRomanString;
   } KEYSTSDATA;
   typedef KEYSTSDATA *PKEYSTSDATA;
   
   typedef struct _WPMCTLPARAMS        /* wpmctlprm - old definition */
   {
      ULONG       wpmctlflag;
      ULONG       textlength;
      PSZ         pTextString;
      PSZ         pAttrString;
   } WPMCTLPARAMS;
   typedef WPMCTLPARAMS *PWPMCTLPARAMS;
   
   /* wpmctlflag value definition */
   
   #define DBE_KSF_ALPHANUMERIC       0x0001
   #define DBE_KSF_KATAKANA           0x0002
   #define DBE_KSF_HIRAGANA           0x0004
   #define DBE_KSF_CAPS               0x0008
   #define DBE_KSF_ROMAN              0x0010
   #define DBE_KSF_HANKAKU            0x0020
   #define DBE_KSF_ZENKAKU            0x0040
   #define DBE_KSF_SBCS               DBE_KSF_HANKAKU
   #define DBE_KSF_DBCS               DBE_KSF_ZENKAKU
   #define DBE_KSF_UPPER              0x0080
   #define DBE_KSF_LOWER              0x0100
   #define DBE_KSF_TEXT               0x0200
   #define DBE_KSF_NOROMAN            0x0400
   #define DBE_KSF_DISABLEONTHESPOT   0x0800
   #define DBE_KSF_ROMANTEXT          0x1000
   
   /* For Code Page 934(Korea) wpmctlflag value definition  */
   
   #define DBE_KSF_JAMO               0x0002
   #define DBE_KSF_HANGEUL            0x0004
   
   /* For Code Page 938(Taiwan) wpmctlflag value definition  */
   
   #define DBE_KSF_TWNIME             0x0002             /* ssc-t */
   #define DBE_KSF_PHONETIC           0x0002
   #define DBE_KSF_TSANGJYE           0x0004
   
   /* WM_SETWINDOWPARAM ctldata of Pecic control */
   
   typedef struct _PECICDATA        /* pecicdata */
   {
      ULONG       wpmctlflag;
      ULONG       textlength;
      PSZ         pTextString;
      PSZ         pAttrString;
      ULONG       CursorPos;
      ULONG       CursorAttr;
   } PECICDATA;
   typedef PECICDATA *PPECICDATA;
   
   /* wndparams definition of WM_DBE_APPLSTAT */
   
   #define DBE_WPM_APPLSTAT_VISIBILITY   0x0004
   
   /* ctldata of DBE_WPM_APPLSTAT_VISIBLITY */
   
   #define DBE_AS_APPLTOP          0x0001
   #define DBE_AS_KBDTOP           0x0002
   #define DBE_AS_PECICTOP         0x0004
   #define DBE_AS_APPLBOTTOM       0x0008
   
   /** WM_DBE_KKCPARAMS definition **/
   /* mp1(lpkkcparams) */
   
   typedef struct _KKCPARAMS        /* kkcparams */
   {
      ULONG       lenKKCParams;
      ULONG       fRequest;
      ULONG       fKKCParams;
      PVOID       pKKCData;
      ULONG       Reserved;
   } KKCPARAMS;
   typedef KKCPARAMS *PKKCPARAMS;
   
   /* fRequest */
   #define DBE_KKCPARAMS_SET           0x0001
   #define DBE_KKCPARAMS_QUERY         0x0002
   
   /* fKKCParams */
   #define DBE_KKCPARAMS_PHYSICALATTRIBUTE  0x0001
   #define DBE_KKCPARAMS_PECICCURSORPOS     0x0002
   
   #pragma pack(1) /* force structure alignment packing */
   
   /* Level format for ColorLookUpTable */
   typedef struct _LEVEL   /* Level */
   {
      UCHAR       attri1;                         /** 1st Attribute         **/
      UCHAR       attri2;                         /** 2nd Attribute         **/
   } LEVEL;
   typedef LEVEL *PLEVEL;
   
   #pragma pack()  /* restore packing to default */
   
   /* Color Look Up Table format */
   typedef struct _COLORLOOKUPTBL   /* ColorLookUpTable */
   {
      ULONG       nAttr;                          /** number of Attribute   **/
      LEVEL       PhysicalLevel[32];              /** struct of Levels      **/
   } COLORLOOKUPTBL;
   typedef COLORLOOKUPTBL *PCOLORLOOKUPTBL;
   
   /** WinDBCSIMEControl() Definition & Prototype **/
   
   /* type */
   #define DBE_IMCTL_QUERY         0x0001
   #define DBE_IMCTL_SET           0x0002
   
   typedef struct _IMEMODE         /* imemode */
   {
      ULONG  lenIMEMODE;
      ULONG  fIMEMode;
      ULONG  hIME;                /* query mode only valid */
      ULONG  hModIME;             /* query mode only valid */
   } IMEMODE;
   typedef IMEMODE *PIMEMODE;
   
   /* definition of fIMEMode */
   #define DBE_IMCTL_IMEENABLE      0x0001
   #define DBE_IMCTL_IMEDISABLE     0x0002
   #define DBE_IMCTL_IMEMODE        0x0004
   #define DBE_IMCTL_NOTIMEMODE     0x0008
   #define DBE_IMCTL_INHERIT_PFRAME 0x0100
   #define DBE_IMCTL_INTERIMREQUEST 0x8000
   
   ULONG APIENTRY WinDBCSIMEControl( HAB hab,
                                    HWND hwndFrame,
                                    ULONG ulType,
                                    PIMEMODE pIMEMode );
   
   
   /** WinDBCSModeControl() Definition & Prototype **/
   
   /* WinDBCSModeControl parameters definitions */
   /* type */
   #define DBE_MCTL_QUERY          0x0001
   #define DBE_MCTL_SET            0x0002
   
   /* arg */
   #define DBE_MCTL_INPUTMODEREQUEST 0x0001
   #define DBE_MCTL_JAPANREQ       0x0001   /* migration */
   
   /* parm - MCTL_JAPAN */
   #define DBE_MCTL_ALPHANUMERIC   0x0001
   #define DBE_MCTL_KATAKANA       0x0002
   #define DBE_MCTL_HIRAGANA       0x0004
   #define DBE_MCTL_SBCSCHAR       0x0008
   #define DBE_MCTL_DBCSCHAR       0x0010
   #define DBE_MCTL_ROMAN          0x0020
   #define DBE_MCTL_NOROMAN        0x0040
   
   #define DBE_MCTL_KKCREQ         0x1000
   #define DBE_MCTL_CP932REQ       0x0001
   #define DBE_MCTL_KKCENABLE      0x1000
   #define DBE_MCTL_KKCDISABLE     0x2000
   #define DBE_MCTL_HANKAKU        0x0008
   #define DBE_MCTL_ZENKAKU        0x0010
   
   /* parm - MCTL_KOREA */
   #define DBE_MCTL_JAMO           0x0002
   #define DBE_MCTL_HANGEUL        0x0004
   #define DBE_MCTL_INTERIMREQUEST 0x0080
   
   /* parm - MCTL_TCHINESE */
   #define DBE_MCTL_PHONETIC       0x0002
   #define DBE_MCTL_TSANGJYE       0x0004
   
   /* parm - MCTL_TAIWAN */
   #define DBE_MCTL_TWNIME         0x0002                                  //j-ssc
   
   /* parm - MCTL_PRCHINESE */
   #define DBE_MCTL_HANZI          0x0002
   
   /* for MDI support */
   #define DBE_MCTL_INHERIT_PFRAME 0x0100
   
   ULONG APIENTRY WinDBCSModeControl(HAB hab,
                                     HWND hwndFrame,
                                     ULONG ulType,
                                     ULONG ulArg,
                                     PULONG pparm);
   
   /* Font Driver Description structure  */
   
   typedef struct _FD_DESC    /* fd_desc */
   {
      ULONG  flType;
      CHAR   str64Desc[64];
   } FD_DESC ;
   typedef FD_DESC *PFD_DESC;
   
   /* Font Driver functions */
   
#ifdef __cplusplus
   BOOL    APIENTRY WinDBCSLoadFontDriver(HAB hab,
                                          PCSZ  psz);
#else
   BOOL    APIENTRY WinDBCSLoadFontDriver(HAB hab,
                                          PSZ psz);
#endif
   
#ifdef __cplusplus
   BOOL    APIENTRY WinDBCSUnloadFontDriver(HAB hab,
                                            PCSZ  psz);
#else
   BOOL    APIENTRY WinDBCSUnloadFontDriver(HAB hab,
                                            PSZ psz);
#endif
   
#ifdef __cplusplus
   BOOL    APIENTRY WinDBCSQueryFDDescription(HAB hab,
                                              PCSZ  psz,
                                              PFD_DESC pfd_desc);
#else
   BOOL    APIENTRY WinDBCSQueryFDDescription(HAB hab,
                                              PSZ psz,
                                              PFD_DESC pfd_desc);
#endif
   
   /* fd_type definition */
   
   #define DBE_FD_DESC_SYSTEM      0x0001
   #define DBE_FD_DESC_PUBLIC      0x0002
   #define DBE_FD_DESC_PRIVATE     0x0004
   
   /* System menu item number ID in MENUITEM for DBCS unique function */
   #define SC_DBE_IME                 0x8018
   #define SC_DBE_IMESEPARATOR        0x8019
   
   /* NOINC */
#endif /* __OS2NLS__ */
/* INC */

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
