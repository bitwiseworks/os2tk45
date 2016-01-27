/****************************** Module Header ******************************\
*
* Module Name: OS2IME.H
*
* OS/2 Input Method Definitions file for IME Developer
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_OS2IME             input method
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
/* INC */

#ifdef __cplusplus
      extern "C" {
#endif

#ifndef __OS2IME__
/* NOINC */
#define __OS2IME__
/* INC */

#define OS2IME_INCLUDED



/*----------------------------------------------------------------------
 *
 *  Handle related Definition
 *
 *----------------------------------------------------------------------*/

typedef LHANDLE     HIMIP;
typedef HIMIP     * PHIMIP;




/*----------------------------------------------------------------------
 *
 *  Input Method Instance Definition
 *
 *----------------------------------------------------------------------*/

    #pragma pack(1) /* force structure alignment packing */

typedef struct _IMINSTANCE  {   /* iminst */

    /* Size of this structure */
    ULONG           ulSize;

    /* Information related to the client application */
    ULONG           ulSessionType;
    HWND            hwnd;
    ULONG           ulSessionID;
    ULONG           ulClientInfo;
    ULONG           ulCodepage;

    /* Status part */
    HIMIP           himipStatus;
    ULONG           ulInputMode;
    ULONG           ulConversionMode;
    POINTL          ptlStatusPos;
    SIZEL           slStatusSize;

    /* Conversion part */
    HIMIP           himipConversion;
    FATTRS          faConversionWindow;
    GRADIENTL       gradlConversionWindow;  /* include os2.h with INCL_GPIPRIMITIVES */
                                            /* to validate GRADIENTL definition */
    SIZEF           sfCharBox;
    CONVERSIONPOS   cpConversionPos;

    /* Result part */
    HIMIP           himipResult;

    /* Candidate part */
    HIMIP           himipCandidate;
    CANDIDATEPOS    cpCandidatePos;

    /* Information message part */
    HIMIP           himipInfoMsg;

    /* Register word part */
    HIMIP           himipRegWord;

    /* Private part */
    HIMIP           himipPrivate;

} IMINSTANCE;
typedef IMINSTANCE * PIMINSTANCE;

    #pragma pack()  /* restore packing to default */


/*----- bit definition of ulClientInfo -----*/

#define     IMI_CI_PMCALLABLE           0x00000001




/*----------------------------------------------------------------------
 *
 *  Status Part Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _STATUSSTRINGHEADER {    /* imstatus */

    ULONG   ulSize;

    ULONG   ulStringLen;
    ULONG   ulStringOffset;
    ULONG   ulStringAttrOffset;

    ULONG   ulCursorPos;
    ULONG   ulCursorAttr;

} STATUSSTRINGHEADER;
typedef STATUSSTRINGHEADER * PSTATUSSTRINGHEADER;

/* StringAttr definition */

#define     SP_STRING_REVERSED          0x01
#define     SP_STRING_UNDERSCORE        0x02
#define     SP_STRING_BACKCOLOR         0x04
#define     SP_STRING_FORECOLOR         0x08
#define     SP_STRING_SEPARATOR         0x10

/* CursorAttr definition */

#define     SP_CSR_INSERT               0x00000001
#define     SP_CSR_INVISIBLE            0x00000002



/*----------------------------------------------------------------------
 *
 *  Conversion Part Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _CONVERSIONSTRINGHEADER {    /*imconv */

    ULONG   ulSize;

    ULONG   ulReadingAttrLen;
    ULONG   ulReadingAttrOffset;
    ULONG   ulReadingClauseLen;
    ULONG   ulReadingClauseOffset;
    ULONG   ulReadingStringLen;
    ULONG   ulReadingStringOffset;

    ULONG   ulConversionAttrLen;
    ULONG   ulConversionAttrOffset;
    ULONG   ulConversionClauseLen;
    ULONG   ulConversionClauseOffset;
    ULONG   ulConversionStringLen;
    ULONG   ulConversionStringOffset;

    ULONG   ulCursorPos;
    ULONG   ulCursorAttr;

    ULONG   ulChangeStart;

} CONVERSIONSTRINGHEADER;
typedef CONVERSIONSTRINGHEADER * PCONVERSIONSTRINGHEADER;



/*----------------------------------------------------------------------
 *
 *  Result Part Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _RESULTSTRINGHEADER  {   /* imreslt */

    ULONG   ulSize;

    ULONG   ulReadingAttrLen;
    ULONG   ulReadingAttrOffset;
    ULONG   ulReadingClauseLen;
    ULONG   ulReadingClauseOffset;
    ULONG   ulReadingStringLen;
    ULONG   ulReadingStringOffset;

    ULONG   ulResultAttrLen;
    ULONG   ulResultAttrOffset;
    ULONG   ulResultClauseLen;
    ULONG   ulResultClauseOffset;
    ULONG   ulResultStringLen;
    ULONG   ulResultStringOffset;

} RESULTSTRINGHEADER;
typedef RESULTSTRINGHEADER * PRESULTSTRINGHEADER;



/*----------------------------------------------------------------------
 *
 *  Candidate Part Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _CANDIDATEINFOHEADER {   /* imcandinfo */

    ULONG   ulSize;
    ULONG   ulCount;
    ULONG   ulListOffset[32];
    ULONG   ulPrivateSize;
    ULONG   ulPrivateOffset;

} CANDIDATEINFOHEADER;
typedef CANDIDATEINFOHEADER * PCANDIDATEINFOHEADER;




/*----------------------------------------------------------------------
 *
 *  Information Message Part Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _INFOMSGHEADER {     /* iminfomsg */

    ULONG   ulSize;
    ULONG   ulErrorLevel;
    ULONG   ulTypeID;
    ULONG   ulTitleLen;
    ULONG   ulTitleOffset;
    ULONG   ulStringLen;
    ULONG   ulStringOffset;
    ULONG   ulPrivateLen;
    ULONG   ulPrivateOffset;

} INFOMSGHEADER;
typedef INFOMSGHEADER * PINFOMSGHEADER;




/*----------------------------------------------------------------------
 *
 *  pre-defined Part Window Class Definition
 *
 *----------------------------------------------------------------------*/

#define WC_IME_IME          "WC_IME_IME"   // till actual number is assigned
#define WC_IME_STATUS       "WC_IME_STATUS"   // till actual number is assigned
#define WC_IME_CONVERSION   "WC_IME_CONVERSION"   // till actual number is assigned
#define WC_IME_CANDIDATE    "WC_IME_CANDIDATE"   // till actual number is assigned
#define WC_IME_INFOMSG      "WC_IME_INFOMSG"   // till actual number is assigned
#define WC_IME_REGWORD      "WC_IME_REGWORD"   // till actual number is assigned



/*----------------------------------------------------------------------
 *
 *  pre-defined Conversion Part Window Class Related Definition
 *
 *----------------------------------------------------------------------*/

typedef struct _CONVCLR { /* convclr */
    ULONG ulAttr;
    COLOR cFore;
    COLOR cBack;
} CONVCLR;
typedef CONVCLR * PCONVCLR;

typedef struct _CONVERSIONDATA { /* conversiondata */
    USHORT  usSize;
    ULONG   ulCount;
    CONVCLR aConvClr[1];
} CONVERSIONDATA;
typedef CONVERSIONDATA * PCONVERSIONDATA;



/*----------------------------------------------------------------------
 *
 *  pre-defined Candidate Part Window Class Related Definition
 *
 *----------------------------------------------------------------------*/

typedef VOID ( * EXPENTRY PFNMOUNOTIFY )( HIMI himi,
                                          ULONG ulMsg,
                                          ULONG ulCandIndex );

#define CI_PREVPAGE                 0xfffffffe
#define CI_NEXTPAGE                 0xffffffff


typedef struct _CANDCTLDATA { /* candctldata */
    USHORT  usSize;
    ULONG   ulCtlFlag;
    PFNMOUNOTIFY pfnMouseNotify;
} CANDCTLDATA;
typedef CANDCTLDATA * PCANDCTLDATA;

#define CCD_MOUSENOTIFYENTRY        0x00000001




/*----------------------------------------------------------------------
 *
 *  WinQueryWindowULong indices for Input Method Editor Window
 *
 *----------------------------------------------------------------------*/

#define QWL_IMEW_HIMI               0
#define QWL_IMEW_PRIVATE            4
#define QWL_IMEW_HWNDSTATUS         8
#define QWL_IMEW_HWNDCONVERSION     12
#define QWL_IMEW_HWNDCANDIDATE      16
#define QWL_IMEW_HWNDINFOMSG        20
#define QWL_IMEW_HWNDREGWORD        24




/*----------------------------------------------------------------------
 *
 *  WInDefImeWindowProc
 *
 *----------------------------------------------------------------------*/

MRESULT APIENTRY WinDefImeWindowProc( HWND hwnd,
                                      ULONG msg,
                                      MPARAM mp1,
                                      MPARAM mp2 );



/*----------------------------------------------------------------------
 *
 *  WInDefImePartProc
 *
 *----------------------------------------------------------------------*/

MRESULT APIENTRY WinDefImePartProc( HWND hwnd,
                                    ULONG msg,
                                    MPARAM mp1,
                                    MPARAM mp2 );



/*----------------------------------------------------------------------
 *
 *  IME Exported Entries
 *
 *----------------------------------------------------------------------*/

    #pragma pack(1) /* force structure alignment packing */

typedef struct _IMEINIT {   /* imeinit */

    ULONG   ulIMVersion;
    ULONG   ulIMEVersion;
    ULONG   ulIMECaps;
    ULONG   ulConversionCaps;
    ULONG   ulInputCaps;
    CHAR    chClassName[256];
    CHAR    chWndProcName[256];
    UCHAR   chSoftKbdStr[95];                   //T400
    UCHAR   chImeModeText[17];                  //T400

} IMEINIT;
typedef IMEINIT * PIMEINIT;

    #pragma pack()  /* restore packing to default */

#define     IME_CAPS_NONE                       0x00000000
#define     IME_CAPS_PROVIDEUI                  0x00000001
#define     IME_CAPS_PRIVATEUI                  0x00000002
#define     IME_CAPS_SCREENSWITCHABLE           0x00000004
#define     IME_CAPS_ACCEPTCONVERSIONSTRING     0x00000008
#define     IME_CAPS_PROVIDEREADINGSTRING       0x00000010
#define     IME_CAPS_INHERITINPUTMODE           0x00000020
#define     IME_CAPS_INHERITCONVERSIONMODE      0x00000040
#define     IME_CAPS_UNICODE                    0x00000080


APIRET APIENTRY ImeInitialize( PIMEINIT pimeinit );

APIRET APIENTRY ImeTerminate( ULONG ulReserved );

typedef struct _NOTIFYEVENT {   /* ne */
    ULONG   ulEventType;
    ULONG   ulInput1;
    ULONG   ulInput2;
    ULONG   ulInput3;
    ULONG   ulOutput;
} NOTIFYEVENT;
typedef NOTIFYEVENT * PNOTIFYEVENT;

APIRET APIENTRY ImeNotifyEvent( HIMI hImi,
                                PNOTIFYEVENT pNotifyEvent );

#define     IME_NE_ATTACHIME                    1
#define     IME_NE_FOCUS                        2
#define     IME_NE_KEYEVENT                     3
#define     IME_NE_INSTANCEUPDATED              4

#define         IME_NE_IU_SESSIONTYPE               0x00000001
#define         IME_NE_IU_HWND                      0x00000002
#define         IME_NE_IU_SESSIONID                 0x00000004
#define         IME_NE_IU_CLIENTINFO                0x00000008
#define         IME_NE_IU_CODEPAGE                  0x00000010
#define         IME_NE_IU_INPUTMODE                 0x00000020
#define         IME_NE_IU_CONVERSIONMODE            0x00000040
#define         IME_NE_IU_STATUSPOS                 0x00000080
#define         IME_NE_IU_CONVERSIONFONT            0x00000100
#define         IME_NE_IU_CONVERSIONPOS             0x00000200
#define         IME_NE_IU_CANDIDATEPOS              0x00000400

#define     IME_NE_SHOWCANDIDATE                5
#define     IME_NE_SELECTCANDIDATE              6
#define     IME_NE_SETTOPCANDIDATE              7
#define     IME_NE_SETCANDIDATEPAGESIZE         8
#define     IME_NE_CONVERSIONSTRING             9

#define         IME_NE_CS_COMPLETE                  CNV_COMPLETE
#define         IME_NE_CS_CONVERT                   CNV_CONVERT
#define         IME_NE_CS_REVERT                    CNV_REVERT
#define         IME_NE_CS_CANCEL                    CNV_CANCEL

#define     IME_NE_SHOWSTATUSWINDOW             10
#define     IME_NE_CHANGECANDIDATELIST          11
#define     IME_NE_BROADCASTDATA                12


APIRET APIENTRY ImeConvertString( HIMI hImi,
                                  PSZ pszInput,
                                  PCANDIDATELISTHEADER pCandidateList,
                                  PULONG pulBufferLength,
                                  ULONG ulFlag );

#define     ICS_CONVERSION                  1
#define     ICS_REVERSECONVERSION           2


APIRET APIENTRY ImeEscape( HIMI hImi,
                           ULONG ulEscape,
                           PVOID pvData );

#define     IME_ESC_RESERVED_FIRST          0x00000000
#define     IME_ESC_RESERVED_LAST           0x00000FFF
#define     IME_ESC_PRIVATE_FIRST           0x00001000
#define     IME_ESC_PRIVATE_LAST            0x00001FFF

#define     IME_ESC_IME_NAME                IME_ESC_RESERVED_FIRST /* PRC ONLY */

APIRET APIENTRY ImeSetConversionString( HIMI hImi,
                                        ULONG ulRequestType,
                                        PVOID pConversionString,
                                        ULONG ulConversionLength,
                                        PVOID pvReadingString,
                                        ULONG ulReadingLength );


APIRET APIENTRY ImeRegisterWord( HIMI hImi,
                                 PSZ pszWord,
                                 PSZ pszReading,
                                 ULONG ulWordType );

APIRET APIENTRY ImeDeregisterWord( HIMI hImi,
                                   PSZ pszWord,
                                   PSZ pszReading,
                                   ULONG ulWordType );

APIRET APIENTRY ImeQueryRegisterWordType( HIMI hImi,
                                          PWORDTYPE paWordType,
                                          PULONG pulBufferLength );

APIRET APIENTRY ImeEnumRegisterWord( HIMI hImi,
                                     PSZ pszWord,
                                     PSZ pszReading,
                                     ULONG ulWordType,
                                     REGISTERWORDENUMPROC pfnCallback,
                                     PVOID pvData );


APIRET APIENTRY ImeDialog( HIMI hImi,
                           ULONG ulType,
                           PREGISTERWORDHEADER pRegWord );


    #pragma pack(1) /* force structure alignment packing */

typedef struct _REGISTERIMEHEADER {     /* imregime */
    ULONG       cbSize;
    ULONG       ulLangCount;
    ULONG       offsetLang;
    ULONG       ulDescriptionLen;
    ULONG       offsetDescription;
    ULONG       ulFlag;
} REGISTERIMEHEADER;
typedef REGISTERIMEHEADER * PREGISTERIMEHEADER;

    #pragma pack()  /* restore packing to default */


#define     IM_REG_FLAG_DEFAULT         0x00000001
#define     IM_REG_FLAG_IME16CALLABLE   0x00000002

APIRET APIENTRY ImRegisterIME( PSZ pszIMEName,
                               PREGISTERIMEHEADER pRegIme,
                               PULONG pImeId );

APIRET APIENTRY ImDeregisterIME( PSZ pszIMEName );


/*----------------------------------------------------------------------
 *
 *  Window Message for Input Method Editor Window
 *
 *----------------------------------------------------------------------*/

#define WM_QUERYIMEWINDOWCAPS       0x00c8


#define IWC_CONVERSIONANGLE         1
#define IWC_CONVERSIONMARGIN        2
#define IWC_FORCEPOSITION           3



/*----------------------------------------------------------------------
 *
 *  Input Method Interfaces for IME
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImRequestEvent( HIMI hImi,
                                ULONG msg,
                                ULONG mp1,
                                ULONG mp2 );


APIRET APIENTRY ImRequestIMInstance( HIMI hImi,
                                     PIMINSTANCE * ppIMInstance );

APIRET APIENTRY ImReleaseIMInstance( HIMI hImi );

APIRET APIENTRY ImQueryIMInstanceReqCount( HIMI hImi,
                                           PULONG pulCount );

APIRET APIENTRY ImCreateIMIPart( ULONG ulPartSize,
                                 PHIMIP phimiPart );

APIRET APIENTRY ImDestroyIMIPart( HIMIP himiPart );

APIRET APIENTRY ImQueryIMIPartSize( HIMIP himiPart,
                                    PULONG pulPartSize );

APIRET APIENTRY ImRequestIMIPart( HIMIP himiPart,
                                  PVOID * ppPartData );

APIRET APIENTRY ImReleaseIMIPart( HIMIP himiPart );

APIRET APIENTRY ImResizeIMIPart( HIMIP himiPart,
                                 ULONG ulNewSize );

APIRET APIENTRY ImQueryIMIPartReqCount( HIMIP himiPart,
                                        PULONG pulCount );

#define     IBD_RESERVED_FIRST      0x0000
#define     IBD_RESERVED_LAST       0x0fff
#define     IBD_PRIVATE_FIRST       0x1000
#define     IBD_PRIVATE_LAST        0x1fff

typedef struct _BROADCASTDATAHEADER {   /* brddata */
    ULONG   cbSize;
    BYTE    bData[1];
} BROADCASTDATAHEADER;
typedef BROADCASTDATAHEADER * PBROADCASTDATAHEADER;

APIRET APIENTRY ImBroadcastData( PSZ pszIMEName,
                                 ULONG ulIndex,
                                 PBROADCASTDATAHEADER pData );


/*----------------------------------------------------------------------
 *
 *  IME VKEY definitions
 *
 *----------------------------------------------------------------------*/

#define IMEVK_LBUTTON                    0x01
#define IMEVK_RBUTTON                    0x02
#define IMEVK_CANCEL                     0x03
#define IMEVK_MBUTTON                    0x04
#define IMEVK_BACK                       0x08
#define IMEVK_TAB                        0x09
#define IMEVK_CLEAR                      0x0C
#define IMEVK_RETURN                     0x0D
#define IMEVK_LEFTSHIFT                  0x10
#define IMEVK_LEFTCONTROL                0x11
#define IMEVK_MENU                       0x12
#define IMEVK_PAUSE                      0x13
#define IMEVK_CAPITAL                    0x14
#define IMEVK_KANA                       0x15
#define IMEVK_HANGEUL                    0x15
#define IMEVK_JUNJA                      0x17
#define IMEVK_FINAL                      0x18
#define IMEVK_IMEACTIVATE                0x19
#define IMEVK_HANJA                      0x19
#define IMEVK_ESCAPE                     0x1B
#define IMEVK_CONVERT                    0x1C
#define IMEVK_NONCONVERT                 0x1D
#define IMEVK_ACCEPT                     0x1E
#define IMEVK_MODECHANGE                 0x1F
#define IMEVK_SPACE                      0x20
#define IMEVK_PRIOR                      0x21
#define IMEVK_NEXT                       0x22
#define IMEVK_END                        0x23
#define IMEVK_HOME                       0x24
#define IMEVK_LEFT                       0x25
#define IMEVK_UP                         0x26
#define IMEVK_RIGHT                      0x27
#define IMEVK_DOWN                       0x28
#define IMEVK_SELECT                     0x29
#define IMEVK_PRINT                      0x2A
#define IMEVK_EXECUTE                    0x2B
#define IMEVK_SNAPSHOT                   0x2C
#define IMEVK_INSERT                     0x2D
#define IMEVK_DELETE                     0x2E
#define IMEVK_HELP                       0x2F
#define IMEVK_0                          0x30
#define IMEVK_1                          0x31
#define IMEVK_2                          0x32
#define IMEVK_3                          0x33
#define IMEVK_4                          0x34
#define IMEVK_5                          0x35
#define IMEVK_6                          0x36
#define IMEVK_7                          0x37
#define IMEVK_8                          0x38
#define IMEVK_9                          0x39
#define IMEVK_A                          0x41
#define IMEVK_B                          0x42
#define IMEVK_C                          0x43
#define IMEVK_D                          0x44
#define IMEVK_E                          0x45
#define IMEVK_F                          0x46
#define IMEVK_G                          0x47
#define IMEVK_H                          0x48
#define IMEVK_I                          0x49
#define IMEVK_J                          0x4A
#define IMEVK_K                          0x4B
#define IMEVK_L                          0x4C
#define IMEVK_M                          0x4D
#define IMEVK_N                          0x4E
#define IMEVK_O                          0x4F
#define IMEVK_P                          0x50
#define IMEVK_Q                          0x51
#define IMEVK_R                          0x52
#define IMEVK_S                          0x53
#define IMEVK_T                          0x54
#define IMEVK_U                          0x55
#define IMEVK_V                          0x56
#define IMEVK_W                          0x57
#define IMEVK_X                          0x58
#define IMEVK_Y                          0x59
#define IMEVK_Z                          0x5A
#define IMEVK_LWIN                       0x5B
#define IMEVK_RWIN                       0x5C
#define IMEVK_APPS                       0x5D
#define IMEVK_NUMPAD0                    0x60
#define IMEVK_NUMPAD1                    0x61
#define IMEVK_NUMPAD2                    0x62
#define IMEVK_NUMPAD3                    0x63
#define IMEVK_NUMPAD4                    0x64
#define IMEVK_NUMPAD5                    0x65
#define IMEVK_NUMPAD6                    0x66
#define IMEVK_NUMPAD7                    0x67
#define IMEVK_NUMPAD8                    0x68
#define IMEVK_NUMPAD9                    0x69
#define IMEVK_MULTIPLY                   0x6A
#define IMEVK_ADD                        0x6B
#define IMEVK_SEPARATOR                  0x6C
#define IMEVK_SUBTRACT                   0x6D
#define IMEVK_DECIMAL                    0x6E
#define IMEVK_DIVIDE                     0x6F
#define IMEVK_F1                         0x70
#define IMEVK_F2                         0x71
#define IMEVK_F3                         0x72
#define IMEVK_F4                         0x73
#define IMEVK_F5                         0x74
#define IMEVK_F6                         0x75
#define IMEVK_F7                         0x76
#define IMEVK_F8                         0x77
#define IMEVK_F9                         0x78
#define IMEVK_F10                        0x79
#define IMEVK_F11                        0x7A
#define IMEVK_F12                        0x7B
#define IMEVK_F13                        0x7C
#define IMEVK_F14                        0x7D
#define IMEVK_F15                        0x7E
#define IMEVK_F16                        0x7F
#define IMEVK_F17                        0x80
#define IMEVK_F18                        0x81
#define IMEVK_F19                        0x82
#define IMEVK_F20                        0x83
#define IMEVK_F21                        0x84
#define IMEVK_F22                        0x85
#define IMEVK_F23                        0x86
#define IMEVK_F24                        0x87
#define IMEVK_NUMLOCK                    0x90
#define IMEVK_SCROLL                     0x91
#define IMEVK_OEM_SHIFT                  0x92
#define IMEVK_OEM_0                      0xB9
#define IMEVK_OEM_1                      0xBA
#define IMEVK_OEM_PLUS                   0xBB
#define IMEVK_OEM_COMMA                  0xBC
#define IMEVK_OEM_MINUS                  0xBD
#define IMEVK_OEM_PERIOD                 0xBE
#define IMEVK_OEM_2                      0xBF
#define IMEVK_OEM_3                      0xC0
#define IMEVK_OEM_4                      0xDB
#define IMEVK_OEM_5                      0xDC
#define IMEVK_OEM_6                      0xDD
#define IMEVK_OEM_7                      0xDE
#define IMEVK_OEM_8                      0xDF
#define IMEVK_OEM_9                      0xE0
#define IMEVK_OEM_AX                     0xE1
#define IMEVK_OEM_BSLASH                 0xE2
#define IMEVK_PROCESSKEY                 0xE5
#define IMEVK_DBE_ALPHANUMERIC           0xf0
#define IMEVK_DBE_KATAKANA               0xf1
#define IMEVK_DBE_HIRAGANA               0xf2
#define IMEVK_DBE_SBCSCHAR               0xf3
#define IMEVK_DBE_DBCSCHAR               0xf4
#define IMEVK_DBE_ROMAN                  0xf5
#define IMEVK_DBE_NOROMAN                0xf6
#define IMEVK_DBE_ENTERWORDREGISTERMODE  0xf7
#define IMEVK_DBE_ENTERIMECONFIGMODE     0xf8
#define IMEVK_DBE_FLUSHSTRING            0xf9
#define IMEVK_DBE_CODEINPUT              0xfa
#define IMEVK_DBE_NOCODEINPUT            0xfb
#define IMEVK_DBE_DETERMINESTRING        0xfc
#define IMEVK_DBE_ENTERDLGCONVERSIONMODE 0xfd

#define IMEVK_LEFTALT                    0x1000
#define IMEVK_RIGHTSHIFT                 0x1001
#define IMEVK_RIGHTCONTROL               0x1002
#define IMEVK_RIGHTALT                   0x1003
#define IMEVK_SYSREQ                     0x1004
#define IMEVK_ALLCANDIDATE               0x1005
#define IMEVK_PREVCANDIDATE              0x1006
#define IMEVK_SBCSDBCSCHAR               0x1007
#define IMEVK_TSANGJYE                   0x1008
#define IMEVK_PHONETIC                   0x1009

#define IMEVK_UNDEF                      0xffff

#endif /* __OS2IME__ */

#ifdef __cplusplus
        }
#endif

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
/**************************** end of file **********************************/
