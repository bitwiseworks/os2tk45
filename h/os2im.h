/****************************** Module Header ******************************\
*
* Module Name: OS2IM.H
*
* OS/2 Input Method Definitions file
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_OS2IM              input method
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

#ifndef __OS2IM__
/* NOINC */
#define __OS2IM__
/* INC */

#define OS2IM_INCLUDED




/*----------------------------------------------------------------------
 *
 *  Handle related Definition
 *
 *----------------------------------------------------------------------*/

typedef LHANDLE     HIMI;
typedef HIMI      * PHIMI;



/*----------------------------------------------------------------------
 *
 *  Instance related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImAssociateInstance( HWND hwnd, HIMI himi, PHIMI phimiPrev );

APIRET APIENTRY ImCreateInstance( PHIMI phimi );

APIRET APIENTRY ImDestroyInstance( HIMI himi );

APIRET APIENTRY ImGetInstance( HWND hwnd, PHIMI phimi );

APIRET APIENTRY ImQueryDefaultIMWindow( HWND hwnd, PHWND phwnd );

APIRET APIENTRY ImReleaseInstance( HWND hwnd, HIMI himi );



/*----------------------------------------------------------------------
 *
 *  Register Word related Interface
 *
 *----------------------------------------------------------------------*/

typedef struct _REGISTERWORDHEADER {  /* imregw */
    ULONG   cbSize;
    ULONG   ulLengthReading;
    ULONG   ulOffsetReading;
    ULONG   ulLengthWord;
    ULONG   ulOffsetWord;
} REGISTERWORDHEADER;
typedef REGISTERWORDHEADER * PREGISTERWORDHEADER;



typedef ULONG (EXPENTRY REGISTERWORDENUMPROC)(PSZ, ULONG, PSZ, PVOID );


APIRET APIENTRY ImEnumRegisterWord( HIMI himi,
                                    REGISTERWORDENUMPROC pfnEnumProc,
                                    PSZ pReading,
                                    ULONG ulType,
                                    PSZ pRegister,
                                    PVOID pData );


    #pragma pack(1) /* force structure alignment packing */

typedef struct _WORDTYPE {  /* imwordt */
    ULONG   ulType;
    CHAR    szDescription[32];
} WORDTYPE;
typedef WORDTYPE * PWORDTYPE;

    #pragma pack()  /* restore packing to default */


APIRET APIENTRY ImQueryRegisterWordType( HIMI himi, PULONG pulCount,
                                         PWORDTYPE pWordType );


APIRET APIENTRY ImRegisterWord( HIMI himi,
                                PSZ pszReading,
                                ULONG ulType,
                                PSZ pszRegister );

APIRET APIENTRY ImDeregisterWord( HIMI himi,
                                  PSZ pszReading,
                                  ULONG ulType,
                                  PSZ pszDeregister );





/*----------------------------------------------------------------------
 *
 *  Dialog related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImShowIMEDlg( HIMI himi,
                             ULONG ulDlgType,
                             PREGISTERWORDHEADER pRegWord );

#define     IME_DLG_CONFIG      1
#define     IME_DLG_REGWORD     2
#define     IME_DLG_DICTIONARY  3
#define     IME_DLG_VERSION     4



/*----------------------------------------------------------------------
 *
 *  Escape Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImEscape( HIMI himi, ULONG ulEscaoe, PVOID pData );



/*----------------------------------------------------------------------
 *
 *  Candidate Window related Interface
 *
 *----------------------------------------------------------------------*/

typedef struct _CANDIDATELISTHEADER {     /* imcanl */
    ULONG   ulSize;
    ULONG   ulStyle;
    ULONG   ulCount;
    ULONG   ulSelection;
    ULONG   ulPageStart;
    ULONG   ulPageSize;
    ULONG   ulTitleLen;
    ULONG   ulTitleOffset;
    ULONG   aulOffset[1];
} CANDIDATELISTHEADER;
typedef CANDIDATELISTHEADER * PCANDIDATELISTHEADER;

/* Definitions for the ulStyle */
#define     CLS_UNKNOWN     0x00000000
#define     CLS_READING     0x00000001
#define     CLS_CODE        0x00000002
#define     CLS_MEANING     0x00000003
#define     CLS_RADICAL     0x00000004
#define     CLS_STROKES     0x00000005



APIRET APIENTRY ImQueryCandidateList( HIMI himi,
                                      ULONG ulIndex,
                                      PCANDIDATELISTHEADER pCandidateList,
                                      PULONG pulBuffer );


typedef struct _CANDIDATEPOS {      /* imcanp */
    ULONG   ulIndex;
    ULONG   ulStyle;
    POINTL  ptCurrentPos;
    RECTL   rcArea;
} CANDIDATEPOS;
typedef CANDIDATEPOS * PCANDIDATEPOS;

/* Definitions for the ulStyle */
#define     CPS_CANDIDATEPOS    1
#define     CPS_EXCLUDE         2


APIRET APIENTRY ImQueryCandidateWindowPos( HIMI himi,
                                           PCANDIDATEPOS pCandidatePos );

APIRET APIENTRY ImSetCandidateWindowPos( HIMI himi,
                                         PCANDIDATEPOS pCandidatePos );




/*----------------------------------------------------------------------
 *
 *  Conversion Window related Interface
 *
 *----------------------------------------------------------------------*/

typedef struct _CONVERSIONPOS {      /* imconvp */
    ULONG   ulStyle;
    POINTL  ptCurrentPos;
    RECTL   rcArea;
} CONVERSIONPOS;
typedef CONVERSIONPOS * PCONVERSIONPOS;

/* Definitions for the ulStyle */
#define     CPS_DEFAULT     0x00000000
#define     CPS_FORCE       0x00000001
#define     CPS_POINT       0x00000002
#define     CPS_RECT        0x00000004


APIRET APIENTRY ImQueryConversionFont( HIMI himi,
                                       PFATTRS pFontAttrs );

APIRET APIENTRY ImSetConversionFont( HIMI himi,
                                     PFATTRS pFontAttrs );

APIRET APIENTRY ImQueryConversionFontSize( HIMI himi,
                                           PSIZEF psizfxBox );

APIRET APIENTRY ImSetConversionFontSize( HIMI himi,
                                         PSIZEF psizfxBox );

APIRET APIENTRY ImGetConversionString( HIMI himi, ULONG ulIndex,
                                       PVOID pBuf, PULONG pulBufLen );

APIRET APIENTRY ImSetConversionString( HIMI himi, ULONG ulIndex,
                                         PVOID pConv, ULONG ulConvLen,
                                         PVOID pReading, ULONG ulReadingLen );

APIRET APIENTRY ImQueryConversionAngle( HIMI himi, PGRADIENTL pgradlAngle );

APIRET APIENTRY ImSetConversionAngle( HIMI himi, PGRADIENTL pgradlAngle );


#define     IME_SCS_STRING              9
#define     IME_SCS_ATTRIBUTE          18
#define     IME_SCS_CLAUSEINFO         36

/* Definition for the attribute of the conversion string */
#define     CP_ATTR_INPUT               0
#define     CP_ATTR_TARGET_CONVERTED    1
#define     CP_ATTR_CONVERTED           2
#define     CP_ATTR_TARGET_NOTCONVERTED 3
#define     CP_ATTR_INPUT_ERROR         4

/* Definition for the Cursor Attribute */
#define     CP_CURSORATTR_INSERT        0x00000001
#define     CP_CURSORATTR_INVISIBLE     0x00000002

APIRET APIENTRY ImQueryConversionWindowPos( HIMI himi,
                                            PCONVERSIONPOS pConvPos );

APIRET APIENTRY ImSetConversionWindowPos( HIMI himi,
                                          PCONVERSIONPOS pConvPos );




/*----------------------------------------------------------------------
 *
 *  Result Part related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImGetResultString( HIMI himi, ULONG ulIndex,
                                   PVOID pBuf, PULONG pulBufLen );



/*----------------------------------------------------------------------
 *
 *  Message Information Window related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImQueryInfoMsg( HIMI himi, ULONG ulIndex,
                                     PVOID pBuf, PULONG pulBufLen );

#define     IM_ERRLEVEL_NONE                0
#define     IM_ERRLEVEL_INFORMATION         1
#define     IM_ERRLEVEL_WARNING             2
#define     IM_ERRLEVEL_ERROR               3
#define     IM_ERRLEVEL_FATAL               4
#define     IM_ERRLEVEL_HELP                5

#define     IM_ERRLEVEL_LAST                IM_ERRLEVEL_HELP

#define     IM_INFOID_UNSAVABLE             1
#define     IM_INFOID_NOCONVERT             2
#define     IM_INFOID_NODICTIONARY          3
#define     IM_INFOID_NOMODULE              4
#define     IM_INFOID_INVALIDREADING        5
#define     IM_INFOID_TOOMANYSTROKES        6
#define     IM_INFOID_TYPINGERROR           7
#define     IM_INFOID_UNDEFINED             8
#define     IM_INFOID_INPUTREADING          9
#define     IM_INFOID_INPUTRADICAL          10
#define     IM_INFOID_INPUTCODE             11
#define     IM_INFOID_SELECTCANDIDATE       12
#define     IM_INFOID_REVERSECONVERSION     13

#define     IM_INFOID_LAST                  IM_INFOID_REVERSECONVERSION




/*----------------------------------------------------------------------
 *
 *  Status Window related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImQueryStatusWindowPos( HIMI himi, PPOINTL pptPos, PSIZEL pszlSize );

APIRET APIENTRY ImSetStatusWindowPos( HIMI himi, PPOINTL pptPos );

APIRET APIENTRY ImShowStatusWindow( HIMI himi, ULONG fShow );

#define     SSW_HIDE        0
#define     SSW_SHOW        1

APIRET APIENTRY ImGetStatusString( HIMI himi, ULONG ulIndex,
                                   PVOID pBuf, PULONG pulBufLen );


/*----------------------------------------------------------------------
 *
 *  Convert String Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImConvertString( HIMI himi,
                                 PSZ pSrc,
                                 PCANDIDATELISTHEADER pDst,
                                 PULONG pulBufLen,
                                 ULONG ulFlag );

#define     CNV_NORMAL      1
#define     CNV_REVERSE     2
#define     CNV_READLEN     3



/*----------------------------------------------------------------------
 *
 *  Input Method Mode Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImQueryIMMode( HIMI himi,
                               PULONG pulInputMode,
                               PULONG pulConversionMode );

APIRET APIENTRY ImSetIMMode( HIMI himi,
                             ULONG ulInputMode,
                             ULONG ulConversionMode );

/* input mode definition */
#define     IMI_IM_NLS_ALPHANUMERIC     0x00000000
#define     IMI_IM_NLS_HIRAGANA         0x00000001
#define     IMI_IM_NLS_TAIWAN           0x00000001
#define     IMI_IM_NLS_PRC              0x00000001
#define     IMI_IM_NLS_HANGEUL          0x00000001
#define     IMI_IM_NLS_KATAKANA         0x00000003
#define     IMI_IM_NLS                  0x0000001F

#define     IMI_IM_WIDTH_HALF           0x00000000
#define     IMI_IM_WIDTH_FULL           0x00000020

#define     IMI_IM_ROMAJI_OFF           0x00000000
#define     IMI_IM_ROMAJI_ON            0x00000040

#define     IMI_IM_SYSTEMROMAJI_ENABLE     0x00000000
#define     IMI_IM_SYSTEMROMAJI_DISABLE    0x00000080

#define     IMI_IM_IME_OFF              0x00000000
#define     IMI_IM_IME_ON               0x00000100

#define     IMI_IM_IME_ENABLE           0x00000000
#define     IMI_IM_IME_DISABLE          0x00000200

#define     IMI_IM_IME_SOFTKBD_OFF      0x00000000
#define     IMI_IM_IME_SOFTKBD_ON       0x00000400

#define     IMI_IM_IME_CSYMBOL_OFF      0x00000000
#define     IMI_IM_IME_CSYMBOL_ON       0x00000800

#define     IMI_IM_SOFTKBD              IMI_IM_IME_SOFTKBD_ON
#define     IMI_IM_SOFTKBD_OFF          IMI_IM_IME_SOFTKBD_OFF

#define     IMI_IM_2NDCONV              0x00001000
#define     IMI_IM_2NDCONV_OFF          0x00000000

/* conversion mode definition */
#define     IMI_CM_NONE                 0x00000000
#define     IMI_CM_PLURALCLAUSE         0x00000001
#define     IMI_CM_SINGLE               0x00000002
#define     IMI_CM_AUTOMATIC            0x00000004
#define     IMI_CM_PREDICT              0x00000008


typedef struct _IMMODE {    /* immode */
    ULONG   ulInputMode;
    ULONG   ulConversionMode;
} IMMODE;
typedef IMMODE * PIMMODE;



/*----------------------------------------------------------------------
 *
 *  Input Method Editor Information Interface
 *
 *----------------------------------------------------------------------*/

#define     IMENAMESIZE     32

typedef struct _IMEINFOHEADER {     /* imeinfhdr */
    ULONG   cbSize;
    CHAR    szIMEName[IMENAMESIZE];
    ULONG   ulCodePageCount;
    ULONG   offsetCodePage;
    ULONG   ulDescriptionLen;
    ULONG   offsetDescription;
    ULONG   ulFlag;
} IMEINFOHEADER;
typedef IMEINFOHEADER * PIMEINFOHEADER;

APIRET APIENTRY ImQueryIMEInfo( ULONG ImeId, PIMEINFOHEADER pImeInfoHeader,
                                PULONG pulBufLen );


APIRET APIENTRY ImQueryIMEProperty( HIMI himi, ULONG ulIndex, PULONG pulProp );

#define     QIP_PROPERTY        1
#define     QIP_INPUTMODE       2
#define     QIP_CONVERSIONMODE  3
#define     QIP_UI              4
#define     QIP_SETCONVSTR      5
#define     QIP_SELECT          6


#define     PRP_SPECIALUI       0x00000001
#define     PRP_UNICODE         0x00000002
#define     PRP_FORCEPOSITION   0x00000004

#define     UIC_270             1
#define     UIC_ANGLE90         2
#define     UIC_ANGLEANY        4

#define     SCSC_CONVSTR        0x00000001
#define     SCSC_MAKEREAD       0x00000002

#define     SLC_INPUTMODE       0x00000001
#define     SLC_CONVERSIONMODE  0x00000002



APIRET APIENTRY ImRequestIME( HIMI himi, ULONG ulAction,
                              ULONG ulIndex, ULONG ulValue );

#define     REQ_CHANGECANDIDATELIST         1
#define     REQ_HIDECANDIDATE               2
#define     REQ_CONVERSIONSTRING            3
#define     REQ_SHOWCANDIDATE               4
#define     REQ_SELECTCANDIDATE             5
#define     REQ_SETCANDIDATEPAGESIZE        6
#define     REQ_SETCANDIDATEPAGESTART       7

#define     CNV_CANCEL          1
#define     CNV_COMPLETE        2
#define     CNV_CONVERT         3
#define     CNV_REVERT          4



/*----------------------------------------------------------------------
 *
 *  Input Method Message related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImIsIMEMessage( HWND hwndIm,
                                ULONG msg,
                                MPARAM mp1,
                                MPARAM mp2,
                                PBOOL pfResult );


#define     WM_IMECONTROL       0x00c5

#define         IMC_HIDESTATUSWINDOW            1
#define         IMC_QUERYCANDIDATEPOS           2
#define         IMC_QUERYCONVERSIONANGLE        3
#define         IMC_QUERYCONVERSIONFONT         4
#define         IMC_QUERYCONVERSIONFONTSIZE     5
#define         IMC_QUERYCONVERSIONWINDOWPOS    6
#define         IMC_QUERYIMMODE                 7
#define         IMC_QUERYSTATUSWINDOWPOS        8
#define         IMC_QUERYSTATUSWINDOWSIZE       9
#define         IMC_SHOWSTATUSWINDOW            10
#define         IMC_SETCANDIDATEPOS             11
#define         IMC_SETCONVERSIONANGLE          12
#define         IMC_SETCONVERSIONFONT           13
#define         IMC_SETCONVERSIONFONTSIZE       14
#define         IMC_SETCONVERSIONWINDOWPOS      15
#define         IMC_SETIMMODE                   16
#define         IMC_SETSTATUSWINDOWPOS          17
#define         IMC_CHANGEINPUTLANG             18


#define     WM_IMEREQUEST       0x00c6

#define         IMR_INSTANCEACTIVATE            1

#define             IMR_IA_STATUS                   0x00000001
#define             IMR_IA_CONVERSION               0x00000002
#define             IMR_IA_CANDIDATE                0x00000004
#define             IMR_IA_INFOMSG                  0x00000008
#define             IMR_IA_REGWORD                  0x00000010

#define         IMR_STATUS                      2

#define             IMR_STATUS_SHOW                 0x00000001
#define             IMR_STATUS_HIDE                 0x00000002
#define             IMR_STATUS_INPUTMODE            0x00000004
#define             IMR_STATUS_CONVERSIONMODE       0x00000008
#define             IMR_STATUS_STATUSPOS            0x00000010
#define             IMR_STATUS_STRING               0x00000020
#define             IMR_STATUS_STRINGATTR           0x00000040
#define             IMR_STATUS_CURSORPOS            0x00000080
#define             IMR_STATUS_CURSORATTR           0x00000100

#define         IMR_CONVRESULT                  3

#define             IMR_CONV_CONVERSIONSTRING       0x00000001
#define             IMR_CONV_CONVERSIONATTR         0x00000002
#define             IMR_CONV_CONVERSIONCLAUSE       0x00000004
#define             IMR_CONV_READINGSTRING          0x00000008
#define             IMR_CONV_READINGATTR            0x00000010
#define             IMR_CONV_READINGCLAUSE          0x00000020
#define             IMR_CONV_CURSORPOS              0x00000040
#define             IMR_CONV_CURSORATTR             0x00000080
#define             IMR_CONV_CHANGESTART            0x00000100

#define             IMR_CONV_INSERTCHAR             0x00000200
#define             IMR_CONV_NOMOVECARET            0x00000400
#define             IMR_CONV_CONVERSIONFONT         0x00000800
#define             IMR_CONV_CONVERSIONPOS          0x00001000

#define             IMR_RESULT_RESULTSTRING         0x00010000
#define             IMR_RESULT_RESULTATTR           0x00020000
#define             IMR_RESULT_RESULTCLAUSE         0x00040000
#define             IMR_RESULT_READINGSTRING        0x00080000
#define             IMR_RESULT_READINGATTR          0x00100000
#define             IMR_RESULT_READINGCLAUSE        0x00200000

#define         IMR_CANDIDATE                   4

#define             IMR_CANDIDATE_SHOW              0x00000001
#define             IMR_CANDIDATE_HIDE              0x00000002
#define             IMR_CANDIDATE_CHANGE            0x00000004
#define             IMR_CANDIDATE_CANDIDATEPOS      0x00000008
#define             IMR_CANDIDATE_SELECT            0x00000010

#define         IMR_INFOMSG                     5

#define             IMR_INFOMSG_ERRLEVEL            0x00000001
#define             IMR_INFOMSG_ERRINDEX            0x00000002
#define             IMR_INFOMSG_ERRSTR              0x00000004
#define             IMR_INFOMSG_ERRTITLE            0x00000008
#define             IMR_INFOMSG_PRIVATE             0x00000010

#define         IMR_REGWORD                     6

#define             IMR_REGWORD_SHOW                0x00000001
#define             IMR_REGWORD_HIDE                0x00000002

#define         IMR_IMECHANGE                   7

#define         IMR_OTHERINFO                   8

#define             IMR_OTHERINFO_HWND              0x00000001

#define         IMR_CONFIG                      9

#define             IMR_CONFIG_SHOW                 0x00000001
#define             IMR_CONFIG_HIDE                 0x00000002

#define         IMR_DICTIONARY                  10

#define             IMR_DICTIONARY_SHOW             0x00000001
#define             IMR_DICTIONARY_HIDE             0x00000002

#define         IMR_SOFTKBD_TBLUPDATE           12              /*@ssc1*/


#define     WM_IMENOTIFY        0x00c7

#define         IMN_STARTCONVERSION             1
#define         IMN_ENDCONVERSION               2
#define         IMN_CONVERSIONFULL              3
#define         IMN_IMECHANGED                  4




/*----------------------------------------------------------------------
 *
 *  Input Method Editor Registration related Interface
 *
 *----------------------------------------------------------------------*/


APIRET APIENTRY ImQueryIMEList( ULONG ulCodepage,
                                PULONG aImeId,
                                PULONG pulCount );

APIRET APIENTRY ImQueryCurrentIME( HIMI himi, PULONG pImeId );

APIRET APIENTRY ImSetCurrentIME( HIMI himi,
                                 ULONG ulAction,
                                 ULONG ImeId );

#define     SCI_NOIME           1
#define     SCI_NEXT            2
#define     SCI_PREVIOUS        3
#define     SCI_SETBYID         4


APIRET APIENTRY ImQueryDefaultIME( ULONG ulCodepage, PULONG pImeId );

APIRET APIENTRY ImSetDefaultIME( ULONG ulCodepage, ULONG ImeId );


APIRET APIENTRY ImSetIMEListOrder( ULONG ulCodepage,
                                   ULONG ulAction,
                                   ULONG ImeIdTarget,
                                   ULONG ImeIdIndex );

#define     SLO_TOP         1
#define     SLO_BOTTOM      2
#define     SLO_SETAFTER    3
#define     SLO_SETBEFORE   4
#define     SLO_ROTATE      5

APIRET APIENTRY ImQueryIMEID( PSZ pszIMEName, PULONG pImeId );



/*----------------------------------------------------------------------
 *
 *  Input Method Editor Registration related Interface
 *
 *----------------------------------------------------------------------*/

APIRET APIENTRY ImQueryMsgQueueProperty( HMQ hmq, PULONG pulFlag );

APIRET APIENTRY ImSetMsgQueueProperty( HMQ hmq, ULONG ulFlag );

#define MQP_INSTANCE_PERMQ              0x00000001
#define MQP_INSTANCE_PERFRAME           0x00000000



#endif /* __OS2IM__ */

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
