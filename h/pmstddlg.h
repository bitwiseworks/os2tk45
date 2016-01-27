/****************************** Module Header ******************************\
*
* Module Name: PMSTDDLG.H
*
* OS/2 Presentation Manager CUA controls and dialogs declarations
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_WINSTDDLGS         include all dialogs/controls
*   INCL_WINSTDBOOK         book control class
*   INCL_WINSTDCNR          container control class
*   INCL_WINSTDDRAG         standard drag dll
*   INCL_WINSTDFILE         standard file dialog
*   INCL_WINSTDFONT         standard font dialog
*   INCL_WINSTDSLIDER       slider control class
*   INCL_WINCIRCULARSLIDER  circular slider control class
*   INCL_WINSTDSPIN         spin button control class
*   INCL_WINSTDVALSET       value set control class
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

#define STDDLG_INCLUDED


#ifdef INCL_WINSTDDLGS /* enable everything */
   #define INCL_WINSTDFILE
   #define INCL_WINSTDFONT
   #define INCL_WINSTDSPIN
   #define INCL_WINSTDDRAG
   #define INCL_WINSTDCNR
   #define INCL_WINSTDSLIDER
   #define INCL_WINCIRCULARSLIDER
   #define INCL_WINSTDVALSET
   #define INCL_WINSTDBOOK
#endif /* INCL_WINSTDDLGS */

#if (defined (INCL_WINSTDCNR) || defined (INCL_WINSTDVALSET))
   #define INCL_WINSTDDRAG
#endif /* INCL_WINSTDCNR || INCL_WINSTDVALSET */

#if !(defined (INCL_WINSTDFILE_INCLUDED) || !defined (INCL_WINSTDFILE))
   #define INCL_WINSTDFILE_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                     F I L E    D I A L O G                         */
   /*                                                                    */
   /**********************************************************************/
   
   /*--------------------------------------------------------------------*/
   /* File Dialog Invocation Flag Definitions.                           */
   /*--------------------------------------------------------------------*/
   #define FDS_CENTER           0x00000001L /* Center within owner wnd   */
   #define FDS_CUSTOM           0x00000002L /* Use custom user template  */
   #define FDS_FILTERUNION      0x00000004L /* Use union of filters      */
   #define FDS_HELPBUTTON       0x00000008L /* Display Help button       */
   #define FDS_APPLYBUTTON      0x00000010L /* Display Apply button      */
   #define FDS_PRELOAD_VOLINFO  0x00000020L /* Preload volume info       */
   #define FDS_MODELESS         0x00000040L /* Make dialog modeless      */
   #define FDS_INCLUDE_EAS      0x00000080L /* Always load EA info       */
   #define FDS_OPEN_DIALOG      0x00000100L /* Select Open dialog        */
   #define FDS_SAVEAS_DIALOG    0x00000200L /* Select SaveAs dialog      */
   #define FDS_MULTIPLESEL      0x00000400L /* Enable multiple selection */
   #define FDS_ENABLEFILELB     0x00000800L /* Enable SaveAs Listbox     */
   #define FDS_NATIONAL_LANGUAGE 0x80000000L/* Reserved for bidirectional*/
   /* language support.         */
   
   /*--------------------------------------------------------------------*/
   /* File Dialog Selection returned attribute                           */
   /*--------------------------------------------------------------------*/
   #define  FDS_EFSELECTION    0
   #define  FDS_LBSELECTION    1
   
   /*--------------------------------------------------------------------*/
   /* Error Return Codes from dialog (self defining)                     */
   /*--------------------------------------------------------------------*/
   #define  FDS_SUCCESSFUL                            0
   #define  FDS_ERR_DEALLOCATE_MEMORY                 1
   #define  FDS_ERR_FILTER_TRUNC                      2
   #define  FDS_ERR_INVALID_DIALOG                    3
   #define  FDS_ERR_INVALID_DRIVE                     4
   #define  FDS_ERR_INVALID_FILTER                    5
   #define  FDS_ERR_INVALID_PATHFILE                  6
   #define  FDS_ERR_OUT_OF_MEMORY                     7
   #define  FDS_ERR_PATH_TOO_LONG                     8
   #define  FDS_ERR_TOO_MANY_FILE_TYPES               9
   #define  FDS_ERR_INVALID_VERSION                   10
   #define  FDS_ERR_INVALID_CUSTOM_HANDLE             11
   #define  FDS_ERR_DIALOG_LOAD_ERROR                 12
   #define  FDS_ERR_DRIVE_ERROR                       13
   
   /*--------------------------------------------------------------------*/
   /* File Dialog Messages.                                              */
   /*--------------------------------------------------------------------*/
   #define FDM_FILTER       WM_USER+40  /* mp1 = PSZ pszFileName         */
   /*                                      mp2 = PSZ EA .TYPE value      */
   /*                                      mr  = TRUE -> keep file.      */
   #define FDM_VALIDATE     WM_USER+41  /* mp1 = PSZ pszPathName         */
   /*                                      mp2 = USHORT Field name id    */
   /*                                      mr  = TRUE -> Valid name      */
   #define FDM_ERROR        WM_USER+42  /* mp1 = USHORT Error message id */
   /*                                      mp2 = NULL   reserved         */
   /*                                      mr  = NULL -> Use default msg */
   
   /*--------------------------------------------------------------------*/
   /* Define the type that is a pointer to an array of pointers.         */
   /*     Hence: pointer to an array of Z string pointers.               */
   /*--------------------------------------------------------------------*/
   typedef  PSZ       APSZ[1];
   typedef  APSZ *PAPSZ;          /* papsz */
   
   /*--------------------------------------------------------------------*/
   /* File Dialog application data structure.                            */
   /*--------------------------------------------------------------------*/
   typedef struct _FILEDLG     /* fildlg */
   {
      ULONG    cbSize;            /* Size of FILEDLG structure.         */
      ULONG    fl;                /* FDS_ flags. Alter behavior of dlg. */
      ULONG    ulUser;            /* User defined field.                */
      LONG     lReturn;           /* Result code from dialog dismissal. */
      LONG     lSRC;              /* System return code.                */
      PSZ      pszTitle;          /* String to display in title bar.    */
      PSZ      pszOKButton;       /* String to display in OK button.    */
      PFNWP    pfnDlgProc;        /* Entry point to custom dialog proc. */
      PSZ      pszIType;          /* Pointer to string containing       */
      /*                               initial EA type filter. Type     */
      /*                               does not have to exist in list.  */
      PAPSZ    papszITypeList;    /* Pointer to table of pointers that  */
      /*                                point to null terminated Type   */
      /*                                strings. End of table is marked */
      /*                                by a NULL pointer.              */
      PSZ      pszIDrive;         /* Pointer to string containing       */
      /*                               initial drive. Drive does not    */
      /*                               have to exist in drive list.     */
      PAPSZ    papszIDriveList;   /* Pointer to table of pointers that  */
      /*                                point to null terminated Drive  */
      /*                                strings. End of table is marked */
      /*                                by a NULL pointer.              */
      HMODULE  hMod;              /* Custom File Dialog template.       */
      CHAR     szFullFile[CCHMAXPATH]; /* Initial or selected fully     */
      /*                                  qualified path and file.      */
      PAPSZ    papszFQFilename;   /* Pointer to table of pointers that  */
      /*                                point to null terminated FQFname*/
      /*                                strings. End of table is marked */
      /*                                by a NULL pointer.              */
      ULONG    ulFQFCount;        /* Number of files selected           */
      USHORT   usDlgId;           /* Custom dialog id.                  */
      SHORT    x;                 /* X coordinate of the dialog         */
      SHORT    y;                 /* Y coordinate of the dialog         */
      SHORT    sEAType;           /* Selected file's EA Type.           */
   } FILEDLG;
   typedef FILEDLG *PFILEDLG;
   
   /*--------------------------------------------------------------------*/
   /* File Dialog - Function Prototype                                   */
   /*--------------------------------------------------------------------*/
   HWND    APIENTRY WinFileDlg(HWND hwndP,
                               HWND hwndO,
                               PFILEDLG pfild);
   MRESULT APIENTRY WinDefFileDlgProc(HWND hwnd,
                                      ULONG msg,
                                      MPARAM mp1,
                                      MPARAM mp2);
   
   BOOL    APIENTRY WinFreeFileDlgList(PAPSZ papszFQFilename);
   
   #ifndef FILEDLG_DID_INCLUDED
      #define FILEDLG_DID_INCLUDED
      /*--------------------------------------------------------------------*/
      /* File Dialog - dialog and control ids                               */
      /*--------------------------------------------------------------------*/
      #define   DID_FILE_DIALOG             256
      #define   DID_FILENAME_TXT            257
      #define   DID_FILENAME_ED             258
      #define   DID_DRIVE_TXT               259
      #define   DID_DRIVE_CB                260
      #define   DID_FILTER_TXT              261
      #define   DID_FILTER_CB               262
      #define   DID_DIRECTORY_TXT           263
      #define   DID_DIRECTORY_LB            264
      #define   DID_FILES_TXT               265
      #define   DID_FILES_LB                266
      #define   DID_HELP_PB                 267
      #define   DID_APPLY_PB                268
      #define   DID_READ_ONLY               269
      #define   DID_DIRECTORY_SELECTED      270
      #define   DID_OK_PB                   DID_OK
      #define   DID_CANCEL_PB               DID_CANCEL
   #endif /* FILEDLG_DID_INCLUDED */
   
   #define   IDS_FILE_ALL_FILES_SELECTOR      1000
   #define   IDS_FILE_BACK_CUR_PATH           1001
   #define   IDS_FILE_BACK_PREV_PATH          1002
   #define   IDS_FILE_BACK_SLASH              1003
   #define   IDS_FILE_BASE_FILTER             1004
   #define   IDS_FILE_BLANK                   1005
   #define   IDS_FILE_COLON                   1006
   #define   IDS_FILE_DOT                     1007
   #define   IDS_FILE_DRIVE_LETTERS           1008
   #define   IDS_FILE_FWD_CUR_PATH            1009
   #define   IDS_FILE_FWD_PREV_PATH           1010
   #define   IDS_FILE_FORWARD_SLASH           1011
   #define   IDS_FILE_PARENT_DIR              1012
   #define   IDS_FILE_Q_MARK                  1013
   #define   IDS_FILE_SPLAT                   1014
   #define   IDS_FILE_SPLAT_DOT               1015
   #define   IDS_FILE_SAVEAS_TITLE            1016
   #define   IDS_FILE_SAVEAS_FILTER_TXT       1017
   #define   IDS_FILE_SAVEAS_FILENM_TXT       1018
   #define   IDS_FILE_DUMMY_FILE_NAME         1019
   #define   IDS_FILE_DUMMY_FILE_EXT          1020
   #define   IDS_FILE_DUMMY_DRIVE             1021
   #define   IDS_FILE_DUMMY_ROOT_DIR          1022
   #define   IDS_FILE_PATH_PTR                1023
   #define   IDS_FILE_VOLUME_PREFIX           1024
   #define   IDS_FILE_VOLUME_SUFFIX           1025
   #define   IDS_FILE_PATH_PTR2               1026
   #define   IDS_FILE_INVALID_CHARS           1027
   #define   IDS_FILE_ETC_BACK_SLASH          1028
   #define   IDS_FILE_OPEN_PARENTHESIS        1029
   #define   IDS_FILE_CLOSE_PARENTHESIS       1030
   #define   IDS_FILE_SEMICOLON               1031
   
   #define   IDS_FILE_BAD_DRIVE_NAME          1100
   #define   IDS_FILE_BAD_DRIVE_OR_PATH_NAME  1101
   #define   IDS_FILE_BAD_FILE_NAME           1102
   #define   IDS_FILE_BAD_FQF                 1103
   #define   IDS_FILE_BAD_NETWORK_NAME        1104
   #define   IDS_FILE_BAD_SUB_DIR_NAME        1105
   #define   IDS_FILE_DRIVE_NOT_AVAILABLE     1106
   #define   IDS_FILE_FQFNAME_TOO_LONG        1107
   #define   IDS_FILE_OPEN_DIALOG_NOTE        1108
   #define   IDS_FILE_PATH_TOO_LONG           1109
   #define   IDS_FILE_SAVEAS_DIALOG_NOTE      1110
   
   #define   IDS_FILE_DRIVE_DISK_CHANGE       1120
   #define   IDS_FILE_DRIVE_NOT_READY         1122
   #define   IDS_FILE_DRIVE_LOCKED            1123
   #define   IDS_FILE_DRIVE_NO_SECTOR         1124
   #define   IDS_FILE_DRIVE_SOME_ERROR        1125
   #define   IDS_FILE_DRIVE_INVALID           1126
   #define   IDS_FILE_INSERT_DISK_NOTE        1127
   #define   IDS_FILE_OK_WHEN_READY           1128
   
#endif  /* !INCL_WINSTDFILE_INCLUDED && INCL_WINSTDFILE */


#if !(defined (INCL_WINSTDFONT_INCLUDED) || !defined (INCL_WINSTDFONT))
   #define INCL_WINSTDFONT_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                     F O N T    D I A L O G                         */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Font Dialog Creation Structure                                     */
   /**********************************************************************/
   typedef struct _FONTDLG     /* fntdlg */
   {
      ULONG   cbSize;                /* sizeof(FONTDLG)                 */
      HPS     hpsScreen;             /* Screen presentation space       */
      HPS     hpsPrinter;            /* Printer presentation space      */
      PSZ     pszTitle;              /* Application supplied title      */
      PSZ     pszPreview;            /* String to print in preview wndw */
      PSZ     pszPtSizeList;         /* Application provided size list  */
      PFNWP   pfnDlgProc;            /* Dialog subclass procedure       */
      PSZ     pszFamilyname;         /* Family name of font             */
      FIXED   fxPointSize;           /* Point size the user selected    */
      ULONG   fl;                    /* FNTS_* flags - dialog styles    */
      ULONG   flFlags;               /* FNTF_* state flags              */
      ULONG   flType;                /* Font type option bits           */
      ULONG   flTypeMask;            /* Mask of which font types to use */
      ULONG   flStyle;               /* The selected style bits         */
      ULONG   flStyleMask;           /* Mask of which style bits to use */
      LONG    clrFore;               /* Selected foreground color       */
      LONG    clrBack;               /* Selected background color       */
      ULONG   ulUser;                /* Blank field for application     */
      LONG    lReturn;               /* Return Value of the Dialog      */
      LONG    lSRC;                  /* System return code.             */
      LONG    lEmHeight;             /* Em height of the current font   */
      LONG    lXHeight;              /* X height of the current font    */
      LONG    lExternalLeading;      /* External Leading of font        */
      HMODULE hMod;                  /* Module to load custom template  */
      FATTRS  fAttrs;                /* Font attribute structure        */
      SHORT   sNominalPointSize;     /* Nominal Point Size of font      */
      USHORT  usWeight;              /* The boldness of the font        */
      USHORT  usWidth;               /* The width of the font           */
      SHORT   x;                     /* X coordinate of the dialog      */
      SHORT   y;                     /* Y coordinate of the dialog      */
      USHORT  usDlgId;               /* ID of a custom dialog template  */
      USHORT  usFamilyBufLen;        /* Length of family buffer provided*/
      USHORT  usReserved;            /* reserved                        */
   } FONTDLG;
   typedef FONTDLG *PFONTDLG;
   
   /**********************************************************************/
   /* Font Dialog Style Flags                                            */
   /**********************************************************************/
   #define FNTS_CENTER              0x00000001L /* Center in owner dialog*/
   #define FNTS_CUSTOM              0x00000002L /* Use custom template   */
   #define FNTS_OWNERDRAWPREVIEW    0x00000004L /* Allow app to draw     */
   #define FNTS_HELPBUTTON          0x00000008L /* Display Help button   */
   #define FNTS_APPLYBUTTON         0x00000010L /* Display Apply button  */
   #define FNTS_RESETBUTTON         0x00000020L /* Display Reset button  */
   #define FNTS_MODELESS            0x00000040L /* Make dialog modeless  */
   #define FNTS_INITFROMFATTRS      0x00000080L /* Initialize from FATTRs*/
   #define FNTS_BITMAPONLY          0x00000100L /* Only allow bitmap font*/
   #define FNTS_VECTORONLY          0x00000200L /* Only allow vector font*/
   #define FNTS_FIXEDWIDTHONLY      0x00000400L /* Only allow monospaced */
   #define FNTS_PROPORTIONALONLY    0x00000800L /* Only proportional font*/
   #define FNTS_NOSYNTHESIZEDFONTS  0x00001000L /* Don't synthesize fonts*/
   #define FNTS_NATIONAL_LANGUAGE   0x80000000L /* Reserved: bidirectio- */
   /* -nal language support.*/
   
   /**********************************************************************/
   /* Font Dialog Flags                                                  */
   /**********************************************************************/
   #define FNTF_NOVIEWSCREENFONTS      1L
   #define FNTF_NOVIEWPRINTERFONTS     2L
   #define FNTF_SCREENFONTSELECTED     4L
   #define FNTF_PRINTERFONTSELECTED    8L
   
   /**********************************************************************/
   /* Color code definitions                                             */
   /**********************************************************************/
   #define CLRC_FOREGROUND             1L
   #define CLRC_BACKGROUND             2L
   
   /**********************************************************************/
   /* Filter List message string identifiers                             */
   /**********************************************************************/
   #define FNTI_BITMAPFONT         0x0001
   #define FNTI_VECTORFONT         0x0002
   #define FNTI_FIXEDWIDTHFONT     0x0004
   #define FNTI_PROPORTIONALFONT   0x0008
   #define FNTI_SYNTHESIZED        0x0010
   #define FNTI_DEFAULTLIST        0x0020
   #define FNTI_FAMILYNAME         0x0100
   #define FNTI_STYLENAME          0x0200
   #define FNTI_POINTSIZE          0x0400
   
   /**********************************************************************/
   /* Error Return Codes from dialog (self defining)                     */
   /**********************************************************************/
   #define  FNTS_SUCCESSFUL                           0
   #define  FNTS_ERR_INVALID_DIALOG                   3
   #define  FNTS_ERR_ALLOC_SHARED_MEM                 4
   #define  FNTS_ERR_INVALID_PARM                     5
   #define  FNTS_ERR_OUT_OF_MEMORY                    7
   #define  FNTS_ERR_INVALID_VERSION                 10
   #define  FNTS_ERR_DIALOG_LOAD_ERROR               12
   
   /**********************************************************************/
   /* Font Dialog Messages                                               */
   /**********************************************************************/
   #define FNTM_FACENAMECHANGED   WM_USER+50  /* mp1 = PSZ pszFacename   */
   #define FNTM_POINTSIZECHANGED  WM_USER+51  /* mp1 = PSZ pszPointSize, */
   /*                                            mp2 = FIXED fxPointSize */
   #define FNTM_STYLECHANGED      WM_USER+52  /* mp1 = PSTYLECHANGE pstyc*/
   #define FNTM_COLORCHANGED      WM_USER+53  /* mp1 = LONG clr          */
   /*                                            mp2 = USHORT codeClr    */
   #define FNTM_UPDATEPREVIEW     WM_USER+54  /* mp1 = HWND hWndPreview  */
   #define FNTM_FILTERLIST        WM_USER+55  /* mp1 = PSZ pszFacename   */
   /*                                            mp2 = USHORT usStrStyle */
   /*                                            mr=TRUE(Add),FALSE(Dont)*/
   
   /**********************************************************************/
   /* Stylechange message parameter structure                            */
   /**********************************************************************/
   typedef struct _STYLECHANGE     /* stylc */
   {
      USHORT      usWeight;
      USHORT      usWeightOld;
      USHORT      usWidth;
      USHORT      usWidthOld;
      ULONG       flType;
      ULONG       flTypeOld;
      ULONG       flTypeMask;
      ULONG       flTypeMaskOld;
      ULONG       flStyle;
      ULONG       flStyleOld;
      ULONG       flStyleMask;
      ULONG       flStyleMaskOld;
   } STYLECHANGE;
   typedef STYLECHANGE *PSTYLECHANGE;
   
   /**********************************************************************/
   /* Font Dialog Function Prototypes                                    */
   /**********************************************************************/
   HWND    APIENTRY WinFontDlg(HWND hwndP,
                               HWND hwndO,
                               PFONTDLG pfntd);
   MRESULT APIENTRY WinDefFontDlgProc(HWND hwnd,
                                      ULONG msg,
                                      MPARAM mp1,
                                      MPARAM mp2);
   
   #ifndef FONTDLG_DID_INCLUDED
      #define FONTDLG_DID_INCLUDED
      /**********************************************************************/
      /* font dialog and control id's                                       */
      /**********************************************************************/
      #define DID_FONT_DIALOG          300
      #define DID_NAME                 301
      #define DID_STYLE                302
      #define DID_DISPLAY_FILTER       303
      #define DID_PRINTER_FILTER       304
      #define DID_SIZE                 305
      #define DID_SAMPLE               306
      #define DID_OUTLINE              307
      #define DID_UNDERSCORE           308
      #define DID_STRIKEOUT            309
      #define DID_HELP_BUTTON          310
      #define DID_APPLY_BUTTON         311
      #define DID_RESET_BUTTON         312
      #define DID_OK_BUTTON            DID_OK
      #define DID_CANCEL_BUTTON        DID_CANCEL
      #define DID_NAME_PREFIX          313
      #define DID_STYLE_PREFIX         314
      #define DID_SIZE_PREFIX          315
      #define DID_SAMPLE_GROUPBOX      316
      #define DID_EMPHASIS_GROUPBOX    317
      #define DID_FONT_ISO_SUPPORT     318
      #define DID_FONT_ISO_UNTESTED    319
   #endif /* FONTDLG_DID_INCLUDED */
   
   /**********************************************************************/
   /* Stringtable id's                                                   */
   /**********************************************************************/
   #define IDS_FONT_SAMPLE            350
   #define IDS_FONT_BLANK             351
   #define IDS_FONT_KEY_0             352
   #define IDS_FONT_KEY_9             353
   #define IDS_FONT_KEY_SEP           354
   #define IDS_FONT_DISP_ONLY         355
   #define IDS_FONT_PRINTER_ONLY      356
   #define IDS_FONT_COMBINED          357
   #define IDS_FONT_WEIGHT1           358
   #define IDS_FONT_WEIGHT2           359
   #define IDS_FONT_WEIGHT3           360
   #define IDS_FONT_WEIGHT4           361
   #define IDS_FONT_WEIGHT5           362
   #define IDS_FONT_WEIGHT6           363
   #define IDS_FONT_WEIGHT7           364
   #define IDS_FONT_WEIGHT8           365
   #define IDS_FONT_WEIGHT9           366
   #define IDS_FONT_WIDTH1            367
   #define IDS_FONT_WIDTH2            368
   #define IDS_FONT_WIDTH3            369
   #define IDS_FONT_WIDTH4            370
   #define IDS_FONT_WIDTH5            371
   #define IDS_FONT_WIDTH6            372
   #define IDS_FONT_WIDTH7            373
   #define IDS_FONT_WIDTH8            374
   #define IDS_FONT_WIDTH9            375
   #define IDS_FONT_OPTION0           376
   #define IDS_FONT_OPTION1           377
   #define IDS_FONT_OPTION2           378
   #define IDS_FONT_OPTION3           379
   #define IDS_FONT_POINT_SIZE_LIST   380
   #define IDS_FONT_COLOR_NAME_BASE   381
   #define IDS_FONT_COLOR_BLACK       381
   #define IDS_FONT_COLOR_MAROON      382
   #define IDS_FONT_COLOR_GREEN       383
   #define IDS_FONT_COLOR_OLIVE       384
   #define IDS_FONT_COLOR_NAVY        385
   #define IDS_FONT_COLOR_PURPLE      386
   #define IDS_FONT_COLOR_TEAL        387
   #define IDS_FONT_COLOR_GRAY        388
   #define IDS_FONT_COLOR_SILVER      389
   #define IDS_FONT_COLOR_RED         390
   #define IDS_FONT_COLOR_LIME        391
   #define IDS_FONT_COLOR_YELLOW      392
   #define IDS_FONT_COLOR_BLUE        393
   #define IDS_FONT_COLOR_FUSCIA      394
   #define IDS_FONT_COLOR_AQUA        395
   #define IDS_FONT_COLOR_WHITE       396
   #define IDS_FONT_COLOR_NAME_LAST   397
   #define IDS_FONT_COLORS            (IDS_FONT_COLOR_NAME_LAST-IDS_FONT_COLOR_NAME_BASE)
   #define IDS_FONT_NAME_LENGTH       32
   
#endif  /* !INCL_WINSTDFONT_INCLUDED && INCL_WINSTDFONT */


#if !(defined (INCL_WINSTDSPIN_INCLUDED) || !defined (INCL_WINSTDSPIN))
   #define INCL_WINSTDSPIN_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                          S P I N    B U T T O N                    */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* SPINBUTTON Creation Flags                                          */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Character Acceptance                                               */
   /**********************************************************************/
   #define SPBS_ALLCHARACTERS 0x00000000L /* Default: All chars accepted */
   #define SPBS_NUMERICONLY   0x00000001L /* Only 0 - 9 accepted & VKeys */
   #define SPBS_READONLY      0x00000002L /* No chars allowed in entryfld*/
   
   /**********************************************************************/
   /* Type of Component                                                  */
   /**********************************************************************/
   #define SPBS_MASTER        0x00000010L
   #define SPBS_SERVANT       0x00000000L /* Default: Servant            */
   
   /**********************************************************************/
   /* Type of Justification                                              */
   /**********************************************************************/
   #define SPBS_JUSTDEFAULT  0x00000000L /* Default: Same as Left        */
   #define SPBS_JUSTLEFT     0x00000008L
   #define SPBS_JUSTRIGHT    0x00000004L
   #define SPBS_JUSTCENTER   0x0000000CL
   
   /**********************************************************************/
   /* Border or not                                                      */
   /**********************************************************************/
   #define SPBS_NOBORDER     0x00000020L /* Borderless SpinField         */
   /*                                       Default is to have a border. */
   
   /**********************************************************************/
   /* Fast spin or not                                                   */
   /**********************************************************************/
   #define SPBS_FASTSPIN     0x00000100L /* Allow fast spinning.  Fast   */
   /*                                       spinning is performed by     */
   /*                                       skipping over numbers        */
   
   /**********************************************************************/
   /* Pad numbers on front with 0's                                      */
   /**********************************************************************/
   #define SPBS_PADWITHZEROS 0x00000080L /* Pad the number with zeroes   */
   
   /**********************************************************************/
   /* SPINBUTTON Messages                                                */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Notification from Spinbutton to the application is sent in a       */
   /* WM_CONTROL message.                                                */
   /**********************************************************************/
   #define SPBN_UPARROW       0x20A      /* up arrow button was pressed  */
   #define SPBN_DOWNARROW     0x20B      /* down arrow button was pressed*/
   #define SPBN_ENDSPIN       0x20C      /* mouse button was released    */
   #define SPBN_CHANGE        0x20D      /* spinfield text has changed   */
   #define SPBN_SETFOCUS      0x20E      /* spinfield received focus     */
   #define SPBN_KILLFOCUS     0x20F      /* spinfield lost focus         */
   
   /**********************************************************************/
   /* Messages from application to Spinbutton                            */
   /**********************************************************************/
   #define SPBM_OVERRIDESETLIMITS 0x200  /* Set spinbutton limits without*/
   /*                                        resetting the current value */
   #define SPBM_QUERYLIMITS       0x201  /* Query limits set by          */
   /*                                        SPBM_SETLIMITS              */
   #define SPBM_SETTEXTLIMIT      0x202  /* Max entryfield characters    */
   #define SPBM_SPINUP            0x203  /* Tell entry field to spin up  */
   #define SPBM_SPINDOWN          0x204  /* Tell entry field to spin down*/
   #define SPBM_QUERYVALUE        0x205  /* Tell entry field to send     */
   /*                                        current value               */
   
   /**********************************************************************/
   /* Query Flags                                                        */
   /**********************************************************************/
   #define SPBQ_UPDATEIFVALID    0       /* Default                      */
   #define SPBQ_ALWAYSUPDATE     1
   #define SPBQ_DONOTUPDATE      3
   
   /**********************************************************************/
   /* Return value for Empty Field.                                      */
   /*    If ptr too long, variable sent in query msg                     */
   /**********************************************************************/
   #define SPBM_SETARRAY          0x206  /* Change the data to spin      */
   #define SPBM_SETLIMITS         0x207  /* Change the numeric Limits    */
   #define SPBM_SETCURRENTVALUE   0x208  /* Change the current value     */
   #define SPBM_SETMASTER         0x209  /* Tell entryfield who master is*/
   
   
   /**********************************************************************/
   /* Spin Button control data structure                                  */
   /**********************************************************************/
   typedef struct _SPBCDATA     /* spbcd */
   {
      ULONG   cbSize;                /* Size of control block             */
      ULONG   ulTextLimit;           /* Entryfield text limit             */
      LONG    lLowerLimit;           /* Spin lower limit (numeric only)   */
      LONG    lUpperLimit;           /* Spin upper limit (numeric only)   */
      ULONG   idMasterSpb;           /* ID of the servant's master spb    */
      PVOID   pHWXCtlData;           /* reserved for Pen CtlData (penpm.h)*/
   } SPBCDATA;
   typedef SPBCDATA *PSPBCDATA;
   
   
#endif  /* !INCL_WINSTDSPIN_INCLUDED && INCL_WINSTDSPIN */


#if !(defined (INCL_WINSTDDRAG_INCLUDED) || !defined (INCL_WINSTDDRAG))
   #define INCL_WINSTDDRAG_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                D I R E C T   M A N I P U L A T I O N               */
   /*                                                                    */
   /**********************************************************************/
   
   #define PMERR_NOT_DRAGGING     0x1f00
   #define PMERR_ALREADY_DRAGGING 0x1f01
   
   #define MSGF_DRAG              0x0010    /* message filter identifier */
   
   #define WM_DRAGFIRST           0x0310
   #define WM_DRAGLAST            0x032f
   
   #define DM_DROP                0x032f
   #define DM_DRAGOVER            0x032e
   #define DM_DRAGLEAVE           0x032d
   #define DM_DROPHELP            0x032c
   #define DM_ENDCONVERSATION     0x032b
   #define DM_PRINT               0x032a
   #define DM_RENDER              0x0329
   #define DM_RENDERCOMPLETE      0x0328
   #define DM_RENDERPREPARE       0x0327
   #define DM_DRAGFILECOMPLETE    0x0326
   #define DM_EMPHASIZETARGET     0x0325
   #define DM_DRAGERROR           0x0324
   #define DM_FILERENDERED        0x0323
   #define DM_RENDERFILE          0x0322
   #define DM_DRAGOVERNOTIFY      0x0321
   #define DM_PRINTOBJECT         0x0320
   #define DM_DISCARDOBJECT       0x031f
   #define DM_DROPNOTIFY          0x031e
   
   #define DRT_ASM                "Assembler Code"   /* drag type constants  */
   #define DRT_BASIC              "BASIC Code"
   #define DRT_BINDATA            "Binary Data"
   #define DRT_BITMAP             "Bitmap"
   #define DRT_C                  "C Code"
   #define DRT_COBOL              "COBOL Code"
   #define DRT_DLL                "Dynamic Link Library"
   #define DRT_DOSCMD             "DOS Command File"
   #define DRT_EXE                "Executable"
   #define DRT_FORTRAN            "FORTRAN Code"
   #define DRT_ICON               "Icon"
   #define DRT_LIB                "Library"
   #define DRT_METAFILE           "Metafile"
   #define DRT_OS2CMD             "OS/2 Command File"
   #define DRT_PASCAL             "Pascal Code"
   #define DRT_RESOURCE           "Resource File"
   #define DRT_TEXT               "Plain Text"
   #define DRT_UNKNOWN            "Unknown"
   
   #define DOR_NODROP             0x0000   /* DM_DRAGOVER response codes */
   #define DOR_DROP               0x0001
   #define DOR_NODROPOP           0x0002
   #define DOR_NEVERDROP          0x0003
   
   #define DO_COPYABLE            0x0001   /* supported operation flags  */
   #define DO_MOVEABLE            0x0002
   #define DO_LINKABLE            0x0004
   #define DO_CREATEABLE          0x0008
   #define DO_CREATEPROGRAMOBJECTABLE  0x0010
   
   #define DC_OPEN                0x0001   /* source control flags       */
   #define DC_REF                 0x0002
   #define DC_GROUP               0x0004
   #define DC_CONTAINER           0x0008
   #define DC_PREPARE             0x0010
   #define DC_REMOVEABLEMEDIA     0x0020
   
   #define DO_DEFAULT             0xBFFE   /* Default operation          */
   #define DO_UNKNOWN             0xBFFF   /* Unknown operation          */
   #define DO_COPY                0x0010
   #define DO_MOVE                0x0020
   #define DO_LINK                0x0018
   #define DO_CREATE              0x0040
   #define DO_CREATEPROGRAMOBJECT 0x0080
   
   /* Drag Status Flags */
   #define DGS_DRAGINPROGRESS         0x0001  /* Standard Drag in Progress. */
   #define DGS_LAZYDRAGINPROGRESS     0x0002  /* Lazy Drag in Progress.     */
   
   #define DMFL_TARGETSUCCESSFUL  0x0001   /* transfer reply flags       */
   #define DMFL_TARGETFAIL        0x0002
   #define DMFL_NATIVERENDER      0x0004
   #define DMFL_RENDERRETRY       0x0008
   #define DMFL_RENDEROK          0x0010
   #define DMFL_RENDERFAIL        0x0020
   
   #define DRG_ICON          0x00000001L   /* drag image manipulation    */
   #define DRG_BITMAP        0x00000002L   /*   flags                    */
   #define DRG_POLYGON       0x00000004L
   #define DRG_STRETCH       0x00000008L
   #define DRG_TRANSPARENT   0x00000010L
   #define DRG_CLOSED        0x00000020L
   #define DRG_MINIBITMAP    0x00000040L
   
   #define DME_IGNOREABORT        1        /* DM_DRAGERROR return values */
   #define DME_IGNORECONTINUE     2
   #define DME_REPLACE            3
   #define DME_RETRY              4
   
   #define DF_MOVE                0x0001   /* DM_DRAGFILECOMPLETE flags  */
   #define DF_SOURCE              0x0002
   #define DF_SUCCESSFUL          0x0004
   
   #define DRR_SOURCE            1L
   #define DRR_TARGET            2L
   #define DRR_ABORT             3L
   
   #define DFF_MOVE               1        /* DM_DRAGERROR operation IDs */
   #define DFF_COPY               2
   #define DFF_DELETE             3
   
   
   typedef LHANDLE HSTR;  /* hstr */
   
   typedef struct _DRAGITEM     /* ditem */
   {
      HWND    hwndItem;                  /* conversation partner          */
      ULONG   ulItemID;                  /* identifies item being dragged */
      HSTR    hstrType;                  /* type of item                  */
      HSTR    hstrRMF;                   /* rendering mechanism and format*/
      HSTR    hstrContainerName;         /* name of source container      */
      HSTR    hstrSourceName;            /* name of item at source        */
      HSTR    hstrTargetName;            /* suggested name of item at dest*/
      SHORT   cxOffset;                  /* x offset of the origin of the */
      /*                                      image from the mouse hotspot*/
      SHORT   cyOffset;                  /* y offset of the origin of the */
      /*                                      image from the mouse hotspot*/
      USHORT  fsControl;                 /* source item control flags     */
      USHORT  fsSupportedOps;            /* ops supported by source       */
   } DRAGITEM;
   typedef DRAGITEM *PDRAGITEM;
   
   typedef struct _DRAGINFO     /* dinfo */
   {
      ULONG    cbDraginfo;               /* Size of DRAGINFO and DRAGITEMs*/
      USHORT   cbDragitem;               /* size of DRAGITEM              */
      USHORT   usOperation;              /* current drag operation        */
      HWND     hwndSource;               /* window handle of source       */
      SHORT    xDrop;                    /* x coordinate of drop position */
      SHORT    yDrop;                    /* y coordinate of drop position */
      USHORT   cditem;                   /* count of DRAGITEMs            */
      USHORT   usReserved;               /* reserved for future use       */
   } DRAGINFO;
   typedef DRAGINFO *PDRAGINFO;
   
   typedef struct _DRAGIMAGE     /* dimg */
   {
      USHORT  cb;                        /* size control block            */
      USHORT  cptl;                      /* count of pts, if DRG_POLYGON  */
      LHANDLE hImage;                    /* image handle passed to DrgDrag*/
      SIZEL   sizlStretch;               /* size to strecth ico or bmp to */
      ULONG   fl;                        /* flags passed to DrgDrag       */
      SHORT   cxOffset;                  /* x offset of the origin of the */
      /*                                      image from the mouse hotspot*/
      SHORT   cyOffset;                  /* y offset of the origin of the */
      /*                                      image from the mouse hotspot*/
   } DRAGIMAGE;
   typedef DRAGIMAGE *PDRAGIMAGE;
   
   typedef struct _DRAGTRANSFER     /* dxfer */
   {
      ULONG      cb;                     /* size of control block         */
      HWND       hwndClient;             /* handle of target              */
      PDRAGITEM  pditem;                 /* DRAGITEM being transferred    */
      HSTR       hstrSelectedRMF;        /* rendering mech & fmt of choice*/
      HSTR       hstrRenderToName;       /* name source will use          */
      ULONG      ulTargetInfo;           /* reserved for target's use     */
      USHORT     usOperation;            /* operation being performed     */
      USHORT     fsReply;                /* reply flags                   */
   } DRAGTRANSFER;
   typedef DRAGTRANSFER *PDRAGTRANSFER;
   
   typedef struct _RENDERFILE     /* rndf */
   {
      HWND   hwndDragFiles;              /* conversation window           */
      HSTR   hstrSource;                 /* handle to source file name    */
      HSTR   hstrTarget;                 /* handle to target file name    */
      USHORT fMove;                      /* TRUE - move, FALSE - copy     */
      USHORT usRsvd;                     /* reserved                      */
   } RENDERFILE;
   typedef RENDERFILE *PRENDERFILE;
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgAcceptDroppedFiles(HWND hwnd,
                                            PCSZ  pszPath,
                                            PCSZ  pszTypes,
                                            ULONG ulDefaultOp,
                                            ULONG ulRsvd);
#else
   BOOL      APIENTRY DrgAcceptDroppedFiles(HWND hwnd,
                                            PSZ pszPath,
                                            PSZ pszTypes,
                                            ULONG ulDefaultOp,
                                            ULONG ulRsvd);
#endif
   
   PDRAGINFO APIENTRY DrgAllocDraginfo(ULONG cditem);
   
   PDRAGTRANSFER APIENTRY DrgAllocDragtransfer(ULONG cdxfer);
   
   HWND      APIENTRY DrgDrag(HWND hwndSource,
                              PDRAGINFO pdinfo,
                              PDRAGIMAGE pdimg,
                              ULONG cdimg,
                              LONG vkTerminate,
                              PVOID pRsvd);
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgDragFiles(HWND hwnd,
                                   PCSZ  *apszFiles,
                                   PCSZ  *apszTypes,
                                   PCSZ  *apszTargets,
                                   ULONG cFiles,
                                   HPOINTER hptrDrag,
                                   ULONG vkTerm,
                                   BOOL fSourceRender,
                                   ULONG ulRsvd);
#else
   BOOL      APIENTRY DrgDragFiles(HWND hwnd,
                                   PSZ *apszFiles,
                                   PSZ *apszTypes,
                                   PSZ *apszTargets,
                                   ULONG cFiles,
                                   HPOINTER hptrDrag,
                                   ULONG vkTerm,
                                   BOOL fSourceRender,
                                   ULONG ulRsvd);
#endif
   
   BOOL      APIENTRY DrgPostTransferMsg(HWND hwnd,
                                         ULONG msg,
                                         PDRAGTRANSFER pdxfer,
                                         ULONG fl,
                                         ULONG ulRsvd,
                                         BOOL fRetry);
   
   BOOL      APIENTRY DrgQueryDragitem(PDRAGINFO pdinfo,
                                       ULONG cbBuffer,
                                       PDRAGITEM pditem,
                                       ULONG iItem);
   
   ULONG     APIENTRY DrgQueryDragitemCount(PDRAGINFO pdinfo);
   
   PDRAGITEM APIENTRY DrgQueryDragitemPtr(PDRAGINFO pdinfo,
                                          ULONG i);
   
   BOOL      APIENTRY DrgQueryNativeRMF(PDRAGITEM pditem,
                                        ULONG cbBuffer,
                                        PCHAR pBuffer);
   
   ULONG     APIENTRY DrgQueryNativeRMFLen(PDRAGITEM pditem);
   
#ifdef __cplusplus
   ULONG     APIENTRY DrgQueryStrName(HSTR hstr,
                                      ULONG cbBuffer,
                                      PCSZ  pBuffer);
#else
   ULONG     APIENTRY DrgQueryStrName(HSTR hstr,
                                      ULONG cbBuffer,
                                      PSZ pBuffer);
#endif
   
   ULONG     APIENTRY DrgQueryStrNameLen(HSTR hstr);
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgQueryTrueType(PDRAGITEM pditem,
                                       ULONG cbBuffer,
                                       PCSZ  pBuffer);
#else
   BOOL      APIENTRY DrgQueryTrueType(PDRAGITEM pditem,
                                       ULONG cbBuffer,
                                       PSZ pBuffer);
#endif
   
   ULONG     APIENTRY DrgQueryTrueTypeLen(PDRAGITEM pditem);
   
   MRESULT   APIENTRY DrgSendTransferMsg(HWND hwnd,
                                         ULONG msg,
                                         MPARAM mp1,
                                         MPARAM mp2);
   
   BOOL      APIENTRY DrgSetDragitem(PDRAGINFO pdinfo,
                                     PDRAGITEM pditem,
                                     ULONG cbBuffer,
                                     ULONG iItem);
   
   BOOL      APIENTRY DrgSetDragImage(PDRAGINFO pdinfo,
                                      PDRAGIMAGE pdimg,
                                      ULONG cdimg,
                                      PVOID pRsvd);
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgVerifyTypeSet(PDRAGITEM pditem,
                                       PCSZ  pszType,
                                       ULONG cbMatch,
                                       PCSZ  pszMatch);
#else
   BOOL      APIENTRY DrgVerifyTypeSet(PDRAGITEM pditem,
                                       PSZ pszType,
                                       ULONG cbMatch,
                                       PSZ pszMatch);
#endif
   
   BOOL      APIENTRY DrgAccessDraginfo(PDRAGINFO pdinfo);
   
#ifdef __cplusplus
   HSTR      APIENTRY DrgAddStrHandle(PCSZ  psz);
#else
   HSTR      APIENTRY DrgAddStrHandle(PSZ psz);
#endif
   
   BOOL      APIENTRY DrgDeleteDraginfoStrHandles(PDRAGINFO pdinfo);
   
   BOOL      APIENTRY DrgDeleteStrHandle(HSTR hstr);
   
   BOOL      APIENTRY DrgFreeDraginfo(PDRAGINFO pdinfo);
   
   BOOL      APIENTRY DrgFreeDragtransfer(PDRAGTRANSFER pdxfer);
   
   HPS       APIENTRY DrgGetPS(HWND hwnd);
   
   BOOL      APIENTRY DrgPushDraginfo(PDRAGINFO pdinfo,
                                      HWND hwndDest);
   
   BOOL      APIENTRY DrgReleasePS(HPS hps);
   
   BOOL      APIENTRY DrgSetDragPointer(PDRAGINFO pdinfo,
                                        HPOINTER hptr);
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgVerifyNativeRMF(PDRAGITEM pditem,
                                         PCSZ  pszRMF);
#else
   BOOL      APIENTRY DrgVerifyNativeRMF(PDRAGITEM pditem,
                                         PSZ pszRMF);
#endif
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgVerifyRMF(PDRAGITEM pditem,
                                   PCSZ  pszMech,
                                   PCSZ  pszFmt);
#else
   BOOL      APIENTRY DrgVerifyRMF(PDRAGITEM pditem,
                                   PSZ pszMech,
                                   PSZ pszFmt);
#endif
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgVerifyTrueType(PDRAGITEM pditem,
                                        PCSZ  pszType);
#else
   BOOL      APIENTRY DrgVerifyTrueType(PDRAGITEM pditem,
                                        PSZ pszType);
#endif
   
#ifdef __cplusplus
   BOOL      APIENTRY DrgVerifyType(PDRAGITEM pditem,
                                    PCSZ  pszType);
#else
   BOOL      APIENTRY DrgVerifyType(PDRAGITEM pditem,
                                    PSZ pszType);
#endif
   
   /**************************************************/
   /* Lazy Drag API's.                               */
   /**************************************************/
   BOOL APIENTRY DrgLazyDrag( HWND hwndSource,
                             PDRAGINFO pdinfo,
                             PDRAGIMAGE pdimg,
                             ULONG cdimg,
                             PVOID pRsvd );
   
   BOOL APIENTRY DrgCancelLazyDrag( VOID );
   
   BOOL APIENTRY DrgLazyDrop( HWND hwndTarget,
                             ULONG ulOperation,
                             PPOINTL pptlDrop );
   
   PDRAGINFO APIENTRY DrgQueryDraginfoPtr( PDRAGINFO pRsvd );
   
   PDRAGINFO APIENTRY DrgQueryDraginfoPtrFromHwnd( HWND hwndSource );
   
   PDRAGINFO APIENTRY DrgQueryDraginfoPtrFromDragitem( PDRAGITEM pditem );
   
   ULONG APIENTRY DrgQueryDragStatus(VOID);
   
   PDRAGINFO APIENTRY DrgReallocDraginfo (PDRAGINFO pdinfoOld,
                                          ULONG cditem);
   
#ifdef __cplusplus
   ULONG APIENTRY DrgQueryFormat(PDRAGITEM pditem,
                                 PCSZ  pszAppMech,
                                 ULONG cbBuffer,
                                 PCSZ  pBuffer,
                                 ULONG ulFMTIndex);
#else
   ULONG APIENTRY DrgQueryFormat(PDRAGITEM pditem,
                                 PSZ pszAppMech,
                                 ULONG cbBuffer,
                                 PSZ pBuffer,
                                 ULONG ulFMTIndex);
#endif
   
   
#endif  /* !INCL_WINSTDDRAG_INCLUDED && INCL_WINSTDDRAG */


#if !(defined (INCL_WINSTDCNR_INCLUDED) || !defined (INCL_WINSTDCNR))
   #define INCL_WINSTDCNR_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                         C O N T A I N E R                          */
   /*                                                                    */
   /**********************************************************************/
   
   #define CFA_IGNORE      0x80000000L  /* flData bit for FieldInfo        */
   #define CRA_IGNORE      0x00008000L  /* flRecordAttr bit for Recordcore */
   #define CRA_SOURCE      0x00004000L  /* flRecordAttr bit for Recordcore */
   #define CRA_DISABLED    0x00001000L  /* flRecordAttr bit for Recordcore */
   #define CRA_OWNERFREE   0x00010000L  /* flRecordAttr bit for Recordcore */
   #define CRA_OWNERDRAW   0x00020000L  /* flRecordAttr bit for Recordcore */
   #define CV_EXACTLENGTH  0x10000000L  /* Exact match for SearchString    */
   #define CMA_FILTER      0x1000       /* Filtering through invalidation  */
   #define CCS_MINIICONS    0x00000800L  /* Style to have container support */
   /* mini icons with the minirecord. */
   #define CCS_NOCONTROLPTR 0x00001000L  /* don't send WM_CONTROLPOINTER on */
   /* WM_MOUSEMOVE                    */
   
   /**********************************************************************/
   /* Error constants                                                    */
   /**********************************************************************/
   #define PMERR_NOFILTERED_ITEMS          0x1f02
   #define PMERR_COMPARISON_FAILED         0x1f03
   #define PMERR_RECORD_CURRENTLY_INSERTED 0x1f04
   #define PMERR_FI_CURRENTLY_INSERTED     0x1f05
   
   /**********************************************************************/
   /* Container control styles.                                          */
   /**********************************************************************/
   #define CCS_EXTENDSEL             0x00000001L
   #define CCS_MULTIPLESEL           0x00000002L
   #define CCS_SINGLESEL             0x00000004L
   #define CCS_AUTOPOSITION          0x00000008L
   #define CCS_VERIFYPOINTERS        0x00000010L
   #define CCS_READONLY              0x00000020L
   #define CCS_MINIRECORDCORE        0x00000040L
   /* Style to have container support mini icons with the minirecord. */
   #define CCS_MINIICONS             0x00000800L
   /* don't send WM_CONTROLPOINTER on WM_MOUSEMOVE                    */
   #define CCS_NOCONTROLPTR          0x00001000L
   
   /**********************************************************************/
   /* view identifiers           (flWindowAttr)                          */
   /**********************************************************************/
   #define CV_TEXT                  0x00000001L  /* text view            */
   #define CV_NAME                  0x00000002L  /* name view            */
   #define CV_ICON                  0x00000004L  /* icon view            */
   #define CV_DETAIL                0x00000008L  /* detail view          */
   #define CV_FLOW                  0x00000010L  /* flow items           */
   #define CV_MINI                  0x00000020L  /* use mini icon        */
   #define CV_TREE                  0x00000040L  /* tree view            */
   #define CV_GRID                  0x00000080L  /* gridded icon view    */
   #define CV_EXACTLENGTH           0x10000000L  /* Exact match for SearchString */
   
   /**********************************************************************/
   /* Container Attributes        (flWindowAttr)                         */
   /**********************************************************************/
   #define CA_CONTAINERTITLE        0x00000200L
   #define CA_TITLESEPARATOR        0x00000400L
   #define CA_TITLELEFT             0x00000800L
   #define CA_TITLERIGHT            0x00001000L
   #define CA_TITLECENTER           0x00002000L
   #define CA_OWNERDRAW             0x00004000L
   #define CA_DETAILSVIEWTITLES     0x00008000L
   #define CA_ORDEREDTARGETEMPH     0x00010000L
   #define CA_DRAWBITMAP            0x00020000L
   #define CA_DRAWICON              0x00040000L
   #define CA_TITLEREADONLY         0x00080000L
   #define CA_OWNERPAINTBACKGROUND  0x00100000L
   #define CA_MIXEDTARGETEMPH       0x00200000L
   #define CA_TREELINE              0x00400000L
   
   /**********************************************************************/
   /* child window IDs                                                   */
   /**********************************************************************/
   #define CID_LEFTCOLTITLEWND     0x7FF0   /* column title (left)       */
   #define CID_RIGHTCOLTITLEWND    0x7FF1   /* right column title        */
   #define CID_BLANKBOX            0x7FF2   /* blank box at bottom right */
   #define CID_HSCROLL             0x7FF3   /* horizontal scroll bar     */
   #define CID_RIGHTHSCROLL        0x7FF4   /* right horz scroll bar     */
   #define CID_CNRTITLEWND         0x7FF5   /* Container title window    */
   #define CID_LEFTDVWND           0x7FF7   /* Left Details View window  */
   #define CID_RIGHTDVWND          0x7FF8   /* Right Details View window */
   #define CID_VSCROLL             0x7FF9   /* vertical scroll bar       */
   #define CID_MLE                 0x7FFA   /* MLE window for direct edit*/
   
   /**********************************************************************/
   /* Bitmap descriptor array element.                                   */
   /**********************************************************************/
   typedef struct _TREEITEMDESC     /* treend */
   {
      HBITMAP       hbmExpanded;
      HBITMAP       hbmCollapsed;
      HPOINTER      hptrExpanded;
      HPOINTER      hptrCollapsed;
   } TREEITEMDESC;
   typedef TREEITEMDESC *PTREEITEMDESC;
   
   /**********************************************************************/
   /* Field Info data structure, attribute and data types, CV_DETAIL     */
   /**********************************************************************/
   typedef struct _FIELDINFO      /* fldinfo */
   {
      ULONG      cb;                  /* size of FIELDINFO struct       */
      ULONG      flData;              /* attributes of field's data     */
      ULONG      flTitle;             /* attributes of field's title    */
      PVOID      pTitleData;          /* title data (default is string) */
      /*                                 If CFT_BITMAP, must be HBITMAP */
      ULONG      offStruct;           /* offset from RECORDCORE to data */
      PVOID      pUserData;           /* pointer to user data           */
      struct _FIELDINFO *pNextFieldInfo; /* pointer to next linked  */
      /*                                          FIELDINFO structure   */
      ULONG      cxWidth;             /* width of field in pels         */
   } FIELDINFO;
   typedef FIELDINFO *PFIELDINFO;
   
   /**********************************************************************/
   /* RECORDCORE data structure , attribute values                       */
   /**********************************************************************/
   typedef struct _RECORDCORE     /* recc */
   {
      ULONG       cb;
      ULONG       flRecordAttr;        /* record attributes             */
      POINTL      ptlIcon;             /* Position of CV_ICON item      */
      struct _RECORDCORE *preccNextRecord;   /* ptr to next record  */
      PSZ         pszIcon;             /* Text for CV_ICON view         */
      HPOINTER    hptrIcon;            /* Icon to display for ~CV_MINI  */
      HPOINTER    hptrMiniIcon;        /* Icon to display for CV_MINI   */
      HBITMAP     hbmBitmap;           /* Bitmap to display for ~CV_MINI*/
      HBITMAP     hbmMiniBitmap;       /* Bitmap to display for CV_MINI */
      PTREEITEMDESC pTreeItemDesc;     /* Icons for the tree view       */
      PSZ         pszText;             /* Text for CV_TEXT view         */
      PSZ         pszName;             /* Text for CV_NAME view         */
      PSZ         pszTree;             /* Text for CV_TREE view         */
   } RECORDCORE;
   typedef RECORDCORE *PRECORDCORE;
   
   /**********************************************************************/
   /* MINIRECORDCORE data structure, attribute values                    */
   /**********************************************************************/
   typedef struct _MINIRECORDCORE     /* minirec */
   {
      ULONG       cb;
      ULONG       flRecordAttr;        /* record attributes             */
      POINTL      ptlIcon;             /* Position of CV_ICON item      */
      struct _MINIRECORDCORE *preccNextRecord; /*ptr to next record */
      PSZ         pszIcon;             /* Text for CV_ICON view         */
      HPOINTER    hptrIcon;            /* Icon to display for ~CV_MINI  */
   } MINIRECORDCORE;
   typedef MINIRECORDCORE *PMINIRECORDCORE;
   
   /**********************************************************************/
   /* Data structure for moving nodes in the tree to a new parent.       */
   /**********************************************************************/
   typedef struct _TREEMOVE          /* treemv */
   {
      PRECORDCORE preccMove;         /* Record to be moved.        */
      PRECORDCORE preccNewParent;    /* New parent for preccMove.  */
      PRECORDCORE pRecordOrder;      /* Record order for siblings. */
      BOOL        flMoveSiblings;    /* TRUE: Siblings after preccMove */
      /* move to new parent as well.    */
   } TREEMOVE;
   typedef TREEMOVE *PTREEMOVE;
   
   /**********************************************************************/
   /* CNRINFO data structure, describes the container control.           */
   /**********************************************************************/
   typedef struct _CNRINFO     /* ccinfo */
   {
      ULONG       cb;                  /* size of CNRINFO struct        */
      PVOID       pSortRecord;         /* ptr to sort function,         */
      /*                                    RECORDCORE                  */
      PFIELDINFO  pFieldInfoLast;      /* pointer to last column in     */
      /*                                    left pane of a split window.*/
      PFIELDINFO  pFieldInfoObject;    /* Pointer to a column to        */
      /*                                  represent an object.  This is */
      /*                                  the column which will receive */
      /*                                  IN-USE emphasis.              */
      PSZ         pszCnrTitle;         /* text for container title. One */
      /*                                    string separated by line    */
      /*                                    separators for multi-lines  */
      ULONG       flWindowAttr;        /* container attrs - CV_*, CA_*  */
      POINTL      ptlOrigin;           /* lower-left origin in virtual  */
      /*                                    coordinates. CV_ICON view   */
      ULONG       cDelta;              /* Application defined threshold */
      /*                                    or number of records from   */
      /*                                    either end of the list.     */
      ULONG       cRecords;            /* number of records in container*/
      SIZEL       slBitmapOrIcon;      /* size of bitmap in pels        */
      SIZEL       slTreeBitmapOrIcon;  /* size of tree bitmaps in pels  */
      HBITMAP     hbmExpanded;         /* bitmap  for tree node         */
      HBITMAP     hbmCollapsed;        /* bitmap  for tree node         */
      HPOINTER    hptrExpanded;        /* icon    for tree node         */
      HPOINTER    hptrCollapsed;       /* icon    for tree node         */
      LONG        cyLineSpacing;       /* space between two rows        */
      LONG        cxTreeIndent;        /* indent for children           */
      LONG        cxTreeLine;          /* thickness of the Tree Line    */
      ULONG       cFields;             /* number of fields  in container*/
      LONG        xVertSplitbar;       /* position relative to the      */
      /*                                    container (CV_DETAIL); if   */
      /*                                    0xFFFF then unsplit         */
   } CNRINFO;
   typedef CNRINFO *PCNRINFO;
   
   typedef struct _GRIDSQUARE       /* gridsquare */
   {
      ULONG ulNumber;                /* Number of this grid square.   */
      ULONG ulState;                 /* State: CMA_AVAIL, CMA_UNAVAIL */
      RECTL rctlSquare;              /* Rectangle of grid square.     */
   } GRIDSQUARE;
   typedef GRIDSQUARE *PGRIDSQUARE;
   
   typedef struct _GRIDINFO         /* gridinfo */
   {
      ULONG cb;                      /* sizeof gridinfo                     */
      SHORT cxGrid;                  /* width of grid squares.              */
      SHORT cyGrid;                  /* height of grid squares.             */
      SHORT sGridRows;               /* total rows in the grid.             */
      SHORT sGridCols;               /* total cols in the grid.             */
      LONG  cGridSquares;            /* total grid squares (rows*cols).     */
      PGRIDSQUARE pGrid;             /* pointer to array of grid squares.   */
   } GRIDINFO;
   typedef GRIDINFO *PGRIDINFO;
   
   typedef struct _CDATE     /* cdate */
   {
      UCHAR   day;                         /* current day               */
      UCHAR   month;                       /* current month             */
      USHORT  year;                        /* current year              */
   } CDATE;
   typedef CDATE *PCDATE;
   
   typedef struct _CTIME     /* ctime */
   {
      UCHAR   hours;                       /* current hour              */
      UCHAR   minutes;                     /* current minute            */
      UCHAR   seconds;                     /* current second            */
      UCHAR   ucReserved;                  /* reserved                  */
   } CTIME;
   typedef CTIME *PCTIME;
   
   /**********************************************************************/
   /* attribute and type values for flData and flTitle members of        */
   /* FIELDINFO, CFA_ (attributes), CFT_ (types)                         */
   /**********************************************************************/
   #define CFA_LEFT            0x00000001L /* left align text            */
   #define CFA_RIGHT           0x00000002L /* right align text           */
   #define CFA_CENTER          0x00000004L /* center text                */
   #define CFA_TOP             0x00000008L /* top-align text             */
   #define CFA_VCENTER         0x00000010L /* vertically center text     */
   #define CFA_BOTTOM          0x00000020L /* bottom-align text          */
   #define CFA_INVISIBLE       0x00000040L /* Specify invisible column.  */
   #define CFA_BITMAPORICON    0x00000100L /* field title is bitmap      */
   #define CFA_SEPARATOR       0x00000200L /* vert sep, right of fld     */
   #define CFA_HORZSEPARATOR   0x00000400L /* horz sep, bottom of fld    */
   
   #define CFA_STRING          0x00000800L /* string of characters       */
   #define CFA_OWNER           0x00001000L /* ownerdraw field            */
   #define CFA_DATE            0x00002000L /* date structure             */
   #define CFA_TIME            0x00004000L /* time structure             */
   #define CFA_FIREADONLY      0x00008000L /* Column is read-only.       */
   #define CFA_FITITLEREADONLY 0x00010000L /* Column Title is read-only  */
   #define CFA_ULONG           0x00020000L /* Column is number format    */
   
   #define CFA_RANGE           0x00040000L
   #define CFA_NEWCOMP         0x00080000L
   #define CFA_OBJECT          0x00100000L
   #define CFA_LIST            0x00200000L
   #define CFA_CLASS           0x00400000L
   
   #define CFA_IGNORE          0x80000000L  /* flData bit for FieldInfo */
   
   /**********************************************************************/
   /* attribute values for flRecordAttr member of RECORDCORE             */
   /**********************************************************************/
   #define CRA_SELECTED        0x00000001L /* record is selected         */
   #define CRA_TARGET          0x00000002L /* record has target emphasis */
   #define CRA_CURSORED        0x00000004L /* cursor is on the record    */
   #define CRA_INUSE           0x00000008L /* record has in-use emphasis */
   #define CRA_FILTERED        0x00000010L /* record has been filtered   */
   #define CRA_DROPONABLE      0x00000020L /* record can be dropped on   */
   #define CRA_RECORDREADONLY  0x00000040L /* record is read-only        */
   #define CRA_EXPANDED        0x00000080L /* record is expanded         */
   #define CRA_COLLAPSED       0x00000100L /* record is collapsed        */
   #define CRA_PICKED          0x00000200L /* record picked (Lazy Drag). */
   #define CRA_LOCKED          0x00000400L /* record is locked.          */
   #define CRA_DISABLED        0x00001000L /* record is disabled.        */
   #define CRA_SOURCE          0x00004000L /* record has source emph.    */
   #define CRA_IGNORE          0x00008000L /* record is to be ignored    */
   #define CRA_OWNERFREE       0x00010000L /* owner must free record.    */
   #define CRA_OWNERDRAW       0x00020000L /* owner must draw record.    */
   
   /**********************************************************************/
   /* Container messages                                                 */
   /**********************************************************************/
   #define CM_ALLOCDETAILFIELDINFO       0x0330
   #define CM_ALLOCRECORD                0x0331
   #define CM_ARRANGE                    0x0332
   #define CM_ERASERECORD                0x0333
   #define CM_FILTER                     0x0334
   #define CM_FREEDETAILFIELDINFO        0x0335
   #define CM_FREERECORD                 0x0336
   #define CM_HORZSCROLLSPLITWINDOW      0x0337
   #define CM_INSERTDETAILFIELDINFO      0x0338
   #define CM_INSERTRECORD               0x0339
   #define CM_INVALIDATEDETAILFIELDINFO  0x033a
   #define CM_INVALIDATERECORD           0x033b
   #define CM_PAINTBACKGROUND            0x033c
   #define CM_QUERYCNRINFO               0x033d
   #define CM_QUERYDETAILFIELDINFO       0x033e
   #define CM_QUERYDRAGIMAGE             0x033f
   #define CM_QUERYRECORD                0x0340
   #define CM_QUERYRECORDEMPHASIS        0x0341
   #define CM_QUERYRECORDFROMRECT        0x0342
   #define CM_QUERYRECORDRECT            0x0343
   #define CM_QUERYVIEWPORTRECT          0x0344
   #define CM_REMOVEDETAILFIELDINFO      0x0345
   #define CM_REMOVERECORD               0x0346
   #define CM_SCROLLWINDOW               0x0347
   #define CM_SEARCHSTRING               0x0348
   #define CM_SETCNRINFO                 0x0349
   #define CM_SETRECORDEMPHASIS          0x034a
   #define CM_SORTRECORD                 0x034b
   #define CM_OPENEDIT                   0x034c
   #define CM_CLOSEEDIT                  0x034d
   #define CM_COLLAPSETREE               0x034e
   #define CM_EXPANDTREE                 0x034f
   #define CM_QUERYRECORDINFO            0x0350
   #define CM_INSERTRECORDARRAY          0x0351
   #define CM_MOVETREE                   0x0352
   #define CM_SETTEXTVISIBILITY          0x0353
   #define CM_SETGRIDINFO                0x0354
   #define CM_QUERYGRIDINFO              0x0355
   #define CM_SNAPTOGRID                 0x0356
   
   /**********************************************************************/
   /* Container notifications                                            */
   /**********************************************************************/
   #define CN_DRAGAFTER                  101
   #define CN_DRAGLEAVE                  102
   #define CN_DRAGOVER                   103
   #define CN_DROP                          104
   #define CN_DROPHELP                   105
   #define CN_ENTER                         106
   #define CN_INITDRAG                   107
   #define CN_EMPHASIS                   108
   #define CN_KILLFOCUS                  109
   #define CN_SCROLL                        110
   #define CN_QUERYDELTA                 111
   #define CN_SETFOCUS                   112
   #define CN_REALLOCPSZ                 113
   #define CN_BEGINEDIT                  114
   #define CN_ENDEDIT                    115
   #define CN_COLLAPSETREE               116
   #define CN_EXPANDTREE                 117
   #define CN_HELP                       118
   #define CN_CONTEXTMENU                119
   #define CN_VERIFYEDIT                 134
   #define CN_PICKUP                     135
   #define CN_DROPNOTIFY                 136
   #define CN_GRIDRESIZED                137
   
   /**********************************************************************/
   /*   Data Structures for Message Parameters                           */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Container Direct Manipulation structures                           */
   /**********************************************************************/
   typedef struct _CNRDRAGINIT     /* cdrginit */
   {
      HWND        hwndCnr;                 /* Container window handle   */
      PRECORDCORE pRecord;                 /* record under mouse ptr    */
      LONG        x;                       /* x coordinate of mouse ptr */
      LONG        y;                       /* y coordinate of mouse ptr */
      LONG        cx;                      /* x offset from record      */
      LONG        cy;                      /* y offset from record      */
   } CNRDRAGINIT;
   typedef CNRDRAGINIT *PCNRDRAGINIT;
   
   /**********************************************************************/
   /* Data structure for CM_INSERTDETAILFIELDINFO                        */
   /* This structure is used by the application to specify the position  */
   /* of the FieldInfo structures they are inserting.                    */
   /**********************************************************************/
   typedef struct _FIELDINFOINSERT     /* fiins */
   {
      ULONG       cb;                  /* Size of structure.             */
      PFIELDINFO  pFieldInfoOrder;     /* Specifies the order of the     */
      /*                                  FieldInfo structures.          */
      ULONG       fInvalidateFieldInfo;/* Invalidate on Insert.          */
      ULONG       cFieldInfoInsert;    /* The number of FieldInfo        */
      /*                                  structures to insert.          */
   } FIELDINFOINSERT;
   typedef FIELDINFOINSERT *PFIELDINFOINSERT;
   
   /**********************************************************************/
   /* Data structure for CM_INSERTRECORD                                 */
   /**********************************************************************/
   typedef struct _RECORDINSERT     /* recins */
   {
      ULONG       cb;
      PRECORDCORE pRecordOrder;
      PRECORDCORE pRecordParent;
      ULONG       fInvalidateRecord;
      ULONG       zOrder;
      ULONG       cRecordsInsert;
   } RECORDINSERT;
   typedef RECORDINSERT *PRECORDINSERT;
   
   /**********************************************************************/
   /* Data structure for CM_QUERYRECORDFROMRECT                          */
   /**********************************************************************/
   typedef struct _QUERYRECFROMRECT     /* qrecfrct */
   {
      ULONG   cb;
      RECTL   rect;
      ULONG   fsSearch;
   } QUERYRECFROMRECT;
   typedef QUERYRECFROMRECT *PQUERYRECFROMRECT;
   
   /**********************************************************************/
   /* Data structure for CM_QUERYRECORDRECT                              */
   /**********************************************************************/
   typedef struct _QUERYRECORDRECT     /* qrecrct */
   {
      ULONG        cb;
      PRECORDCORE  pRecord;
      ULONG        fRightSplitWindow;
      ULONG        fsExtent;
   } QUERYRECORDRECT;
   typedef QUERYRECORDRECT *PQUERYRECORDRECT;
   
   /**********************************************************************/
   /* Data structure for CM_SEARCHSTRING                                 */
   /**********************************************************************/
   typedef struct _SEARCHSTRING     /* srchstr */
   {
      ULONG  cb;
      PSZ    pszSearch;
      ULONG  fsPrefix;
      ULONG  fsCaseSensitive;
      ULONG  usView;
   } SEARCHSTRING;
   typedef SEARCHSTRING *PSEARCHSTRING;
   
   #ifdef INCL_WINSTDDRAG
      /*******************************************************************/
      /* Data Structure for CN_DRAGLEAVE,CN_DRAGOVER,CN_DROP,CN_DROPHELP */
      /*******************************************************************/
      typedef struct _CNRDRAGINFO     /* cdrginfo */
      {
         PDRAGINFO   pDragInfo;
         PRECORDCORE pRecord;
      } CNRDRAGINFO;
      typedef CNRDRAGINFO *PCNRDRAGINFO;
      
      /*******************************************************************/
      /* Data Structure for CN_DROPNOTIFY                                */
      /*******************************************************************/
      typedef struct _CNRLAZYDRAGINFO     /* clzdrginfo */
      {
         PDRAGINFO pDragInfo;
         PRECORDCORE pRecord;
         HWND hwndTarget;
      } CNRLAZYDRAGINFO;
      typedef CNRLAZYDRAGINFO *PCNRLAZYDRAGINFO;
      
   #endif
   
   /**********************************************************************/
   /* Data structure for CN_EMPHASIS                                     */
   /**********************************************************************/
   typedef struct _NOTIFYRECORDEMPHASIS     /* nrecemph */
   {
      HWND        hwndCnr;
      PRECORDCORE pRecord;
      ULONG       fEmphasisMask;
   } NOTIFYRECORDEMPHASIS;
   typedef NOTIFYRECORDEMPHASIS *PNOTIFYRECORDEMPHASIS;
   
   /**********************************************************************/
   /* Data structure for CN_ENTER                                        */
   /**********************************************************************/
   typedef struct _NOTIFYRECORDENTER     /* notrecen */
   {
      HWND        hwndCnr;
      ULONG       fKey;
      PRECORDCORE pRecord;
   } NOTIFYRECORDENTER;
   typedef NOTIFYRECORDENTER *PNOTIFYRECORDENTER;
   
   /**********************************************************************/
   /* Data structure for CN_QUERYDELTA                                   */
   /**********************************************************************/
   typedef struct _NOTIFYDELTA     /* notdlt */
   {
      HWND        hwndCnr;
      ULONG       fDelta;
   } NOTIFYDELTA;
   typedef NOTIFYDELTA *PNOTIFYDELTA;
   
   /**********************************************************************/
   /* Data structure for CN_SCROLL                                       */
   /**********************************************************************/
   typedef struct _NOTIFYSCROLL     /* notscr */
   {
      HWND        hwndCnr;
      LONG        lScrollInc;
      ULONG       fScroll;
   } NOTIFYSCROLL;
   typedef NOTIFYSCROLL *PNOTIFYSCROLL;
   
   /**********************************************************************/
   /* Data structure for CN_REALLOCPSZ                                   */
   /**********************************************************************/
   typedef struct _CNREDITDATA     /* cnredat */
   {
      ULONG        cb;
      HWND         hwndCnr;
      PRECORDCORE  pRecord;
      PFIELDINFO   pFieldInfo;
      PSZ *ppszText;                   /* address of PSZ        */
      ULONG        cbText;                     /* size of the new text  */
      ULONG        id;
   } CNREDITDATA;
   typedef CNREDITDATA *PCNREDITDATA;
   
   /**********************************************************************/
   /* Data structure for CM_PAINTBACKGROUND                              */
   /**********************************************************************/
   typedef struct _OWNERBACKGROUND     /* ownbckg */
   {
      HWND    hwnd;
      HPS     hps;
      RECTL   rclBackground;
      LONG    idWindow;
   } OWNERBACKGROUND;
   typedef OWNERBACKGROUND *POWNERBACKGROUND;
   
   /**********************************************************************/
   /* Data structure used as part of WM_DRAWITEM                         */
   /**********************************************************************/
   typedef struct _CNRDRAWITEMINFO     /* cnrdrwii */
   {
      PRECORDCORE  pRecord;
      PFIELDINFO   pFieldInfo;
   } CNRDRAWITEMINFO;
   typedef CNRDRAWITEMINFO *PCNRDRAWITEMINFO;
   
   /**********************************************************************/
   /* Message parameter flags                                            */
   /**********************************************************************/
   #define CMA_TOP              0x0001       /* Place at top of zorder   */
   #define CMA_BOTTOM           0x0002       /* Place at bottom of zorder*/
   #define CMA_LEFT             0x0004
   #define CMA_RIGHT            0x0008
   #define CMA_PERIMETER        0x0010       /* additional arrange flags. */
   #define CMA_USER             0x0020
   
   #define CMA_FIRST            0x0010       /* Add record as first      */
   #define CMA_LAST             0x0020
   #define CMA_END              0x0040       /* Add record to end of list*/
   #define CMA_PREV             0x0080
   #define CMA_NEXT             0x0100
   
   #define CMA_HORIZONTAL       0x0200
   #define CMA_VERTICAL         0x0400
   #define CMA_ICON             0x0800
   #define CMA_TEXT             0x1000
   #define CMA_PARTIAL          0x2000
   #define CMA_COMPLETE         0x4000
   
   #define CMA_PARENT           0x0001
   #define CMA_FIRSTCHILD       0x0002
   #define CMA_LASTCHILD        0x0004
   
   #define CMA_CNRTITLE         0x0001       /* Container title          */
   #define CMA_DELTA            0x0002       /* Application defined      */
   #define CMA_FLWINDOWATTR     0x0004       /* Container attributes     */
   #define CMA_LINESPACING      0x0008
   #define CMA_PFIELDINFOLAST   0x0010       /* Ptr to last column in    */
   
   #define CMA_PSORTRECORD      0x0020       /* Pointer to sort function */
   #define CMA_PTLORIGIN        0x0040       /* Lower left origin        */
   #define CMA_SLBITMAPORICON   0x0080       /* Size  of bitmap          */
   #define CMA_XVERTSPLITBAR    0x0100       /* Splitbar position        */
   #define CMA_PFIELDINFOOBJECT 0x0200       /* Pointer to IN-USE        */
   /*                                             emphasis column.       */
   
   #define CMA_TREEICON           0x0400     /* Icon for tree node      */
   #define CMA_TREEBITMAP         0x0800     /* bitmap for tree node    */
   #define CMA_CXTREEINDENT       0x1000     /* indent for children     */
   #define CMA_CXTREELINE         0x2000     /* thickness of tree line  */
   #define CMA_SLTREEBITMAPORICON 0x4000     /* size of icon of tree node */
   
   #define CMA_ITEMORDER        0x0001       /* QueryRecord search flags */
   #define CMA_WINDOW           0x0002
   #define CMA_WORKSPACE        0x0004
   #define CMA_ZORDER           0x0008
   
   #define CMA_DELTATOP         0x0001       /* Industrial - top delta   */
   #define CMA_DELTABOT         0x0002       /* Industrial - bottom delta*/
   #define CMA_DELTAHOME        0x0004       /* Industrial - top of list */
   #define CMA_DELTAEND         0x0008       /* Industrial - end of list */
   
   #define CMA_NOREPOSITION     0x0001    /* InvalidateRecord flags      */
   #define CMA_REPOSITION       0x0002
   #define CMA_TEXTCHANGED      0x0004
   #define CMA_ERASE            0x0008
   #define CMA_NOTEXTCHANGED    0x0010
   #define CMA_FILTER           0x1000    /* Filtering through invalidation  */
   
   #define CMA_FREE             0x0001
   #define CMA_INVALIDATE       0x0002
   
   #define CMA_ARRANGESTANDARD  0x0000
   #define CMA_ARRANGEGRID      0x0001
   #define CMA_ARRANGESELECTED  0x0002
   
   #define CMA_AVAIL            0x0001  /* Grid square is available.   */
   #define CMA_UNAVAIL          0x0002  /* Grid square is unavailable. */
   
#endif /* !INCL_WINSTDCNR_INCLUDED && INCL_WINSTDCNR */


#if !(defined (INCL_WINSTDSLIDER_INCLUDED) || !defined (INCL_WINSTDSLIDER))
   #define INCL_WINSTDSLIDER_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                            S L I D E R                             */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Define messages for the slider control                             */
   /**********************************************************************/
   #define SLM_ADDDETENT          0x0369    /* Add detent niche          */
   #define SLM_QUERYDETENTPOS     0x036a    /* Query position of detent  */
   #define SLM_QUERYSCALETEXT     0x036b    /* Query text at tick number */
   #define SLM_QUERYSLIDERINFO    0x036c    /* Query slider information  */
   #define SLM_QUERYTICKPOS       0x036d    /* Query position of tick    */
   #define SLM_QUERYTICKSIZE      0x036e    /* Query size of tick        */
   #define SLM_REMOVEDETENT       0x036f    /* Remove detent niche       */
   #define SLM_SETSCALETEXT       0x0370    /* Set text above tick       */
   #define SLM_SETSLIDERINFO      0x0371    /* Set slider parameters     */
   #define SLM_SETTICKSIZE        0x0372    /* Set size of tick          */
   #define SLN_CHANGE             1         /* Slider position changed   */
   #define SLN_SLIDERTRACK        2         /* Slider dragged by user    */
   #define SLN_SETFOCUS           3         /* Slider gaining focus      */
   #define SLN_KILLFOCUS          4         /* Slider losing focus       */
   
   /**********************************************************************/
   /* Slider control data structure                                      */
   /**********************************************************************/
   typedef struct _SLDCDATA     /* sldcd */
   {
      ULONG   cbSize;                /* Size of control block             */
      USHORT  usScale1Increments;    /* # of divisions on scale           */
      USHORT  usScale1Spacing;       /* Space in pels between increments  */
      USHORT  usScale2Increments;    /* # of divisions on scale           */
      USHORT  usScale2Spacing;       /* Space in pels between increments  */
   } SLDCDATA;
   typedef SLDCDATA *PSLDCDATA;
   
   /**********************************************************************/
   /* Slider control style flag definition                               */
   /**********************************************************************/
   #define SLS_HORIZONTAL       0x00000000L /* Orient slider horizontally*/
   #define SLS_VERTICAL         0x00000001L /* Orient slider vertically  */
   #define SLS_CENTER           0x00000000L /* Center shaft in window    */
   #define SLS_BOTTOM           0x00000002L /* Offset shaft to bottom (H)*/
   #define SLS_TOP              0x00000004L /* Offset shaft to top (H)   */
   #define SLS_LEFT             0x00000002L /* Offset shaft to left (V)  */
   #define SLS_RIGHT            0x00000004L /* Offset shaft to right (V) */
   #define SLS_SNAPTOINCREMENT  0x00000008L /* Snap to nearest increment */
   #define SLS_BUTTONSBOTTOM    0x00000010L /* Add buttons at shaft bot. */
   #define SLS_BUTTONSTOP       0x00000020L /* Add buttons at shaft top  */
   #define SLS_BUTTONSLEFT      0x00000010L /* Add buttons left of shaft */
   #define SLS_BUTTONSRIGHT     0x00000020L /* Add buttons right of shaft*/
   #define SLS_OWNERDRAW        0x00000040L /* Owner draw some fields    */
   #define SLS_READONLY         0x00000080L /* Provide a read only slider*/
   #define SLS_RIBBONSTRIP      0x00000100L /* Provide a ribbon strip    */
   #define SLS_HOMEBOTTOM       0x00000000L /* Set home position at bot. */
   #define SLS_HOMETOP          0x00000200L /* Set home position at top  */
   #define SLS_HOMELEFT         0x00000000L /* Set home position at left */
   #define SLS_HOMERIGHT        0x00000200L /* Set home position at right*/
   #define SLS_PRIMARYSCALE1    0x00000000L /* Scale 1 is primary scale  */
   #define SLS_PRIMARYSCALE2    0x00000400L /* Scale 2 is primary scale  */
   
   /**********************************************************************/
   /* Message attributes for setting and querying slider components      */
   /**********************************************************************/
   #define SMA_SCALE1               0x0001
   #define SMA_SCALE2               0x0002
   #define SMA_SHAFTDIMENSIONS      0x0000
   #define SMA_SHAFTPOSITION        0x0001
   #define SMA_SLIDERARMDIMENSIONS  0x0002
   #define SMA_SLIDERARMPOSITION    0x0003
   #define SMA_RANGEVALUE           0x0000
   #define SMA_INCREMENTVALUE       0x0001
   #define SMA_SETALLTICKS          0xFFFF
   
   /**********************************************************************/
   /* Ownerdraw flag definitions                                         */
   /**********************************************************************/
   #define SDA_RIBBONSTRIP          0x0001
   #define SDA_SLIDERSHAFT          0x0002
   #define SDA_BACKGROUND           0x0003
   #define SDA_SLIDERARM            0x0004
   
   /**********************************************************************/
   /* Error return codes                                                 */
   /**********************************************************************/
   #define PMERR_UPDATE_IN_PROGRESS        0x1f06
   #define SLDERR_INVALID_PARAMETERS     -1
   
#endif /* !INCL_WINSTDSLIDER_INCLUDED && INCL_WINSTDSLIDER */

#if (defined (INCL_CIRCULARSLIDER))
   #define INCL_WINCIRCULARSLIDER
#endif

#if !(defined (INCL_WINCIRCULARSLIDER_INCLUDED) || !defined (INCL_WINCIRCULARSLIDER))
   #define INCL_WINCIRCULARSLIDER_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*           C I R C U L A R   S L I D E R                            */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Define messages for the circular slider control                    */
   /**********************************************************************/
   #define CSM_QUERYRANGE      0x053D      /* Query value range of control  */
   #define CSM_SETRANGE        0x053E      /* Set the range of values       */
   #define CSM_QUERYVALUE      0x053F      /* Query the current value       */
   #define CSM_SETVALUE        0x0540      /* Set the value for the control */
   #define CSM_QUERYRADIUS     0x0541      /* Query the current radious     */
   #define CSM_SETINCREMENT    0x0542      /* Set scroll/tick mark increment*/
   #define CSM_QUERYINCREMENT  0x0543      /* Query scroll/tick mark incr   */
   #define CSM_SETBITMAPDATA   0x0544      /* Change bitmap for +/-         */
   #define CSN_SETFOCUS        0x0548      /* Notify owner: gaining/losing  */
   #define CSN_CHANGED         0x0549      /* Notify owner: value changed   */
   #define CSN_TRACKING        0x054A      /* Notify owner: mouse tracking  */
   #define CSN_QUERYBACKGROUNDCOLOR 0x054B /* Notify owner: sets bckgrnd clr*/
   
   
   /****************************************************************************/
   /* Circular slider structure. Replaceable bit maps plus/minus buttons       */
   /****************************************************************************/
   typedef struct _CSBITMAPDATA   /* csbitmap */
   {
      HBITMAP hbmLeftUp;
      HBITMAP hbmLeftDown;
      HBITMAP hbmRightUp;
      HBITMAP hbmRightDown;
   } CSBITMAPDATA;
   typedef CSBITMAPDATA *PCSBITMAPDATA;
   
   /**********************************************************************/
   /* Circular Slider control style flag definition                      */
   /**********************************************************************/
   #define CSS_NOBUTTON             0x0001
   #define CSS_NOTEXT               0x0002
   #define CSS_NONUMBER             0x0004
   #define CSS_POINTSELECT          0x0008
   #define CSS_360                  0x0010
   #define CSS_MIDPOINT             0x0020
   #define CSS_PROPORTIONALTICKS    0x0040
   #define CSS_NOTICKS              0x0080
   #define CSS_CIRCULARVALUE        0x0100
   
#endif /* !INCL_WINCIRCULARSLIDER_INCLUDED && INCL_WINCIRCULARSLIDER */

#if !(defined (INCL_WINSTDVALSET_INCLUDED) || !defined (INCL_WINSTDVALSET))
   #define INCL_WINSTDVALSET_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                         V A L U E   S E T                          */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Define messages for the value set control                          */
   /**********************************************************************/
   #define VM_QUERYITEM           0x0375    /* Query item at location    */
   #define VM_QUERYITEMATTR       0x0376    /* Query item attributes     */
   #define VM_QUERYMETRICS        0x0377    /* Query metrics of control  */
   #define VM_QUERYSELECTEDITEM   0x0378    /* Query selected item       */
   #define VM_SELECTITEM          0x0379    /* Set selected item         */
   #define VM_SETITEM             0x037a    /* Set item at location      */
   #define VM_SETITEMATTR         0x037b    /* Set item attributes       */
   #define VM_SETMETRICS          0x037c    /* Set metrics of control    */
   
   #define VN_SELECT              120       /* Item selected by user     */
   #define VN_ENTER               121       /* Item entered by user      */
   #define VN_DRAGLEAVE           122       /* Drag left control         */
   #define VN_DRAGOVER            123       /* Drag is over item         */
   #define VN_DROP                124       /* Drop occurred on item     */
   #define VN_DROPHELP            125       /* Request help for drop     */
   #define VN_INITDRAG            126       /* Drag initiated on item    */
   #define VN_SETFOCUS            127       /* Value set gaining focus   */
   #define VN_KILLFOCUS           128       /* Value set losing focus    */
   #define VN_HELP                129       /* Help requested by user    */
   
   /**********************************************************************/
   /* Value set control data structure                                   */
   /**********************************************************************/
   typedef struct _VSCDATA     /* vscd */
   {
      ULONG   cbSize;                /* Size of control block             */
      USHORT  usRowCount;            /* Number of rows in value set       */
      USHORT  usColumnCount;         /* Number of columns in value set    */
   } VSCDATA;
   typedef VSCDATA *PVSCDATA;
   
   /**********************************************************************/
   /* Value set drag initialization structure                            */
   /**********************************************************************/
   typedef struct _VSDRAGINIT     /* vsdinit */
   {
      HWND    hwnd;                  /* Window handle of value set control*/
      LONG    x;                     /* X coordinate of pointer on desktop*/
      LONG    y;                     /* Y coordinate of pointer on desktop*/
      LONG    cx;                    /* X offset from pointer hot spot    */
      LONG    cy;                    /* Y offset from pointer hot spot    */
      USHORT  usRow;                 /* Number of rows in value set       */
      USHORT  usColumn;              /* Number of columns in value set    */
   } VSDRAGINIT;
   typedef VSDRAGINIT *PVSDRAGINIT;
   
   #ifdef INCL_WINSTDDRAG
      /*******************************************************************/
      /* Value set drag information structure                            */
      /*******************************************************************/
      typedef struct _VSDRAGINFO     /* vsdinfo */
      {
         PDRAGINFO pDragInfo;         /* Pointer to a drag info structure */
         USHORT    usRow;             /* Number of rows in value set      */
         USHORT    usColumn;          /* Number of columns in value set   */
      } VSDRAGINFO;
      typedef VSDRAGINFO *PVSDRAGINFO;
   #endif
   
   /**********************************************************************/
   /* Value set query item text structure                                */
   /**********************************************************************/
   typedef struct _VSTEXT     /* vstext */
   {
      PSZ       pszItemText;      /* Pointer to string for item text      */
      ULONG     ulBufLen;         /* Length of buffer to copy string into */
   } VSTEXT;
   typedef VSTEXT *PVSTEXT;
   
   /**********************************************************************/
   /* Value set control style flag definition                            */
   /**********************************************************************/
   #define VS_BITMAP       0x0001   /* Default all items to bitmaps      */
   #define VS_ICON         0x0002   /* Default all items to icons        */
   #define VS_TEXT         0x0004   /* Default all items to text strings */
   #define VS_RGB          0x0008   /* Default all items to color info   */
   #define VS_COLORINDEX   0x0010   /* Default all items to color indices*/
   #define VS_BORDER       0x0020   /* Add a border around the control   */
   #define VS_ITEMBORDER   0x0040   /* Add a border around each item     */
   #define VS_SCALEBITMAPS 0x0080   /* Scale bitmaps to cell size        */
   #define VS_RIGHTTOLEFT  0x0100   /* Support right to left ordering    */
   #define VS_OWNERDRAW    0x0200   /* Owner draws value set background  */
   
   /**********************************************************************/
   /* Value set item attribute definition                                */
   /**********************************************************************/
   #define VIA_BITMAP       0x0001  /* If set, item contains a bitmap    */
   #define VIA_ICON         0x0002  /* If set, item contains an icon     */
   #define VIA_TEXT         0x0004  /* If set, item contains text string */
   #define VIA_RGB          0x0008  /* If set, item contains color value */
   #define VIA_COLORINDEX   0x0010  /* If set, item contains color index */
   #define VIA_OWNERDRAW    0x0020  /* If set, item is ownerdraw         */
   #define VIA_DISABLED     0x0040  /* If set, item is unselectable      */
   #define VIA_DRAGGABLE    0x0080  /* If set, item can be source of drag*/
   #define VIA_DROPONABLE   0x0100  /* If set, item can be target of drop*/
   
   /**********************************************************************/
   /* Message parameter attributes for sizing and spacing of items       */
   /**********************************************************************/
   #define VMA_ITEMSIZE             0x0001
   #define VMA_ITEMSPACING          0x0002
   
   /**********************************************************************/
   /* Ownerdraw flag definitions                                         */
   /**********************************************************************/
   #define VDA_ITEM                 0x0001
   #define VDA_ITEMBACKGROUND       0x0002
   #define VDA_SURROUNDING          0x0003
   #define VDA_BACKGROUND           0x0004
   
   /**********************************************************************/
   /* Error return codes                                                 */
   /**********************************************************************/
   #define VSERR_INVALID_PARAMETERS     -1
   
#endif /* !INCL_WINSTDVALSET_INCLUDED && INCL_WINSTDVALSET */


#if !(defined (INCL_WINSTDBOOK_INCLUDED) || !defined (INCL_WINSTDBOOK))
   #define INCL_WINSTDBOOK_INCLUDED
   /**********************************************************************/
   /*                                                                    */
   /*                          N O T E B O O K                           */
   /*                                                                    */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Message ids                                                        */
   /**********************************************************************/
   #define BKM_CALCPAGERECT         0x0353  /* Calc book/page rectangle  */
   #define BKM_DELETEPAGE           0x0354  /* Delete page(s)            */
   #define BKM_INSERTPAGE           0x0355  /* Insert page               */
   #define BKM_INVALIDATETABS       0x0356  /* Invalidate tab area       */
   #define BKM_TURNTOPAGE           0x0357  /* Turn to page              */
   #define BKM_QUERYPAGECOUNT       0x0358  /* Query number of pages     */
   #define BKM_QUERYPAGEID          0x0359  /* Query page identifier     */
   #define BKM_QUERYPAGEDATA        0x035a  /* Query page user data      */
   #define BKM_QUERYPAGEWINDOWHWND  0x035b  /* Query page window handle  */
   #define BKM_QUERYTABBITMAP       0x035c  /* Query tab bitmap handle   */
   #define BKM_QUERYTABTEXT         0x035d  /* Query tab text pointer    */
   #define BKM_SETDIMENSIONS        0x035e  /* Set tab/dogear dimensions */
   #define BKM_SETPAGEDATA          0x035f  /* Set page user data        */
   #define BKM_SETPAGEWINDOWHWND    0x0360  /* Set page window handle    */
   #define BKM_SETSTATUSLINETEXT    0x0361  /* Set status line text      */
   #define BKM_SETTABBITMAP         0x0362  /* Set tab bitmap            */
   #define BKM_SETTABTEXT           0x0363  /* Set tab text              */
   #define BKM_SETNOTEBOOKCOLORS    0x0364  /* Set Notebook colors       */
   #define BKM_QUERYPAGESTYLE       0x0365  /* Query page style          */
   #define BKM_QUERYSTATUSLINETEXT  0x0366  /* Query status line text    */
   #define BKM_SETPAGEINFO          0x0367   /* Set page information     */
   #define BKM_QUERYPAGEINFO        0x0368   /* Query page information   */
   #define BKM_SETTABCOLOR          0x0374  /* Set book tab color        */
   #define BKM_SETNOTEBOOKBUTTONS   0x0375  /* Set common pushbuttons    */
   
   #define BKN_PAGESELECTED         130     /* New page selected by user */
   #define BKN_NEWPAGESIZE          131     /* App page size changed     */
   #define BKN_HELP                 132     /* Help notification         */
   #define BKN_PAGEDELETED          133     /* Page deleted notification */
   #define BKN_PAGESELECTEDPENDING  134     /* New page selected by user */
   
   /**********************************************************************/
   /* Page deletion flags (usDeleteFlag)                                 */
   /**********************************************************************/
   #define BKA_ALL                  0x0001  /* all pages                 */
   #define BKA_SINGLE               0x0002  /* single page               */
   #define BKA_TAB                  0x0004  /* minor/major section       */
   
   /**********************************************************************/
   /* Page insertion/query order (usPageOrder, usQueryOrder)             */
   /**********************************************************************/
   #define BKA_LAST                 0x0002  /* Insert/Query last page    */
   #define BKA_FIRST                0x0004  /* Insert/Query first page   */
   #define BKA_NEXT                 0x0008  /* Insert/Query after page   */
   #define BKA_PREV                 0x0010  /* Insert/Query before page  */
   #define BKA_TOP                  0x0020  /* Query topmost page        */
   
   /**********************************************************************/
   /* Notebook region types (usBookRegion, usType)                       */
   /**********************************************************************/
   #define BKA_MAJORTAB             0x0001  /* Major Tab                 */
   #define BKA_MINORTAB             0x0002  /* Minor Tab                 */
   #define BKA_PAGEBUTTON           0x0100  /* Page Turning Button       */
   
   /**********************************************************************/
   /* Page insertion/query styles (usPageStyle,usQueryEnd)               */
   /**********************************************************************/
   #define BKA_STATUSTEXTON         0x0001  /* status area text          */
   #define BKA_MAJOR                0x0040  /* Major Tab                 */
   #define BKA_MINOR                0x0080  /* Minor Tab                 */
   #define BKA_AUTOPAGESIZE         0x0100  /* Page window position/size */
   #define BKA_END                  0x0200  /* Query to end of book      */
   
   /**********************************************************************/
   /* Tab window contents (usTabDisplay)                                 */
   /**********************************************************************/
   #define BKA_TEXT                 0x0400  /* text data                 */
   #define BKA_BITMAP               0x0800  /* bitmap                    */
   
   /**********************************************************************/
   /* Tabbed dialog-specific constants                                   */
   /**********************************************************************/
   #define BKA_AUTOCOLOR            (-1)    /* Automatically color tabs  */
   #define BKA_MAXBUTTONID          7999    /* Max id for common buttons */
   
   /**********************************************************************/
   /* Notebook window styles (ulNotebookStyles)                          */
   /**********************************************************************/
   
   /**********************************************************************/
   /* Backpage Orientation                                               */
   /**********************************************************************/
   #define BKS_BACKPAGESBR          0x00000001  /* Bottom right          */
   #define BKS_BACKPAGESBL          0x00000002  /* Bottom left           */
   #define BKS_BACKPAGESTR          0x00000004  /* Top right             */
   #define BKS_BACKPAGESTL          0x00000008  /* Top left              */
   
   /**********************************************************************/
   /* Major Tab Side                                                     */
   /**********************************************************************/
   #define BKS_MAJORTABRIGHT        0x00000010  /* Major tabs right      */
   #define BKS_MAJORTABLEFT         0x00000020  /* Major tabs left       */
   #define BKS_MAJORTABTOP          0x00000040  /* Major tabs top        */
   #define BKS_MAJORTABBOTTOM       0x00000080  /* Major tabs bottom     */
   
   /**********************************************************************/
   /* Tab Type                                                           */
   /**********************************************************************/
   #define BKS_SQUARETABS           0x00000000  /* Square edged tabs     */
   #define BKS_ROUNDEDTABS          0x00000100  /* Round edged tabs      */
   #define BKS_POLYGONTABS          0x00000200  /* Polygon edged tabs    */
   
   /**********************************************************************/
   /* Binding type                                                       */
   /**********************************************************************/
   #define BKS_SOLIDBIND            0x00000000  /* Solid binding         */
   #define BKS_SPIRALBIND           0x00000400  /* Spiral binding        */
   
   /**********************************************************************/
   /* Status line text justification                                     */
   /**********************************************************************/
   #define BKS_STATUSTEXTLEFT       0x00000000  /* Left justify text     */
   #define BKS_STATUSTEXTRIGHT      0x00001000  /* Right justify text    */
   #define BKS_STATUSTEXTCENTER     0x00002000  /* Center text           */
   
   /**********************************************************************/
   /* Tab text justification                                             */
   /**********************************************************************/
   #define BKS_TABTEXTLEFT          0x00000000  /* Left justify tab text */
   #define BKS_TABTEXTRIGHT         0x00004000  /* Right justify tab text*/
   #define BKS_TABTEXTCENTER        0x00008000  /* Center tab text       */
   
   /**********************************************************************/
   /* Tabbed dialog styles                                               */
   /**********************************************************************/
   #define BKS_TABBEDDIALOG         0x00000800  /* Tabbed dialog         */
   #define BKS_BUTTONAREA           0x00000200  /* Reserve space for     */
   
   /**********************************************************************/
   /* Notebook color presentation param attributes                       */
   /**********************************************************************/
   #define BKA_BACKGROUNDPAGECOLORINDEX  0x0001 /* Page Background       */
   #define BKA_BACKGROUNDPAGECOLOR       0x0002
   #define BKA_BACKGROUNDMAJORCOLORINDEX 0x0003 /* Major Tab Background  */
   #define BKA_BACKGROUNDMAJORCOLOR      0x0004
   #define BKA_BACKGROUNDMINORCOLORINDEX 0x0005 /* Minor Tab Background  */
   #define BKA_BACKGROUNDMINORCOLOR      0x0006
   #define BKA_FOREGROUNDMAJORCOLORINDEX 0x0007 /* Major Tab Text        */
   #define BKA_FOREGROUNDMAJORCOLOR      0x0008
   #define BKA_FOREGROUNDMINORCOLORINDEX 0x0009 /* Minor Tab Text        */
   #define BKA_FOREGROUNDMINORCOLOR      0x000A
   
   /**********************************************************************/
   /* Error message ids                                                  */
   /**********************************************************************/
   #define BOOKERR_INVALID_PARAMETERS   -1L /* Invalid parameters        */
   
   /**********************************************************************/
   /* BKM_SETPAGINFO flags for bSetPageInfoFlags.                        */
   /**********************************************************************/
   #define BFA_PAGEDATA          0x00000001L /* Set page data            */
   #define BFA_PAGEFROMHWND      0x00000002L /* Set page window hwnd     */
   #define BFA_PAGEFROMDLGTEMPLATE 0x00000004L /* Set page window hwnd   */
   /* from dialog template     */
   #define BFA_PAGEFROMDLGRES    0x00000008L /* Set page window hwnd     */
   /* from dialog resource     */
   #define BFA_STATUSLINE        0x00000010L /* Set status text          */
   #define BFA_MAJORTABBITMAP    0x00000020L /* Set major tab bitmap     */
   #define BFA_MINORTABBITMAP    0x00000040L /* Set minor tab bitmap     */
   #define BFA_MAJORTABTEXT      0x00000080L /* Set major tab text       */
   #define BFA_MINORTABTEXT      0x00000100L /* Set minor tab text       */
   #define BFA_BIDIINFO          0x00000200L /* Reserved: bidirectional  */
   /* languages support.       */
   /**********************************************************************/
   /* BKM_SETPAGINFO message structure.                                  */
   /**********************************************************************/
   typedef struct _BOOKPAGEINFO     /* bkpginfo */
   {
      ULONG     cb;                         /* Page flags - BFA_         */
      ULONG     fl;                         /* Page flags - BFA_         */
      BOOL      bLoadDlg;                   /* TRUE: Load dialog now     */
      /* FALSE: Load dialog on turn*/
      ULONG     ulPageData;                 /* data to associate w/page  */
      HWND      hwndPage;                   /* hwnd to associate w/ page */
      PFN       pfnPageDlgProc;             /* auto load of dialogs for  */
      ULONG     idPageDlg;                  /* the application.          */
      HMODULE   hmodPageDlg;                /* Resource info used for    */
      PVOID     pPageDlgCreateParams;
      PDLGTEMPLATE pdlgtPage;
      ULONG     cbStatusLine;               /* Page flags - BFA_         */
      PSZ       pszStatusLine;              /* Status line text string   */
      HBITMAP   hbmMajorTab;                /* Major tab bitmap handle   */
      HBITMAP   hbmMinorTab;                /* Minor tab bitmap handle   */
      ULONG     cbMajorTab;                 /* Page flags - BFA_         */
      PSZ       pszMajorTab;                /* Major tab text string     */
      ULONG     cbMinorTab;                 /* Page flags - BFA_         */
      PSZ       pszMinorTab;                /* Minor tab text string     */
      PVOID     pBidiInfo;                  /* Reserved: Bidirectional   */
      /* language support.         */
   } BOOKPAGEINFO;
   typedef BOOKPAGEINFO *PBOOKPAGEINFO;
   
   /**********************************************************************/
   /* BKM_QUERYTABTEXT and BKM_QUERYSTATUSLINETEXT message structure     */
   /**********************************************************************/
   typedef struct _BOOKTEXT     /* booktxt */
   {
      PSZ    pString;                        /* ptr to string buffer      */
      ULONG  textLen;                        /* length of string to query */
   } BOOKTEXT;
   typedef BOOKTEXT *PBOOKTEXT;
   
   /**********************************************************************/
   /* BKM_SETNOTEBOOKBUTTONS message structure                           */
   /**********************************************************************/
   typedef struct _NOTEBOOKBUTTON   /* nbbutton */
   {
      PSZ     pszText;                /* Button text                     */
      ULONG   idButton;               /* Button id ( < BKA_MAXBUTTONID)  */
      LHANDLE hImage;                 /* Handle for bitmap / icon        */
      LONG    flStyle;                /* Button style                    */
   } NOTEBOOKBUTTON;
   typedef NOTEBOOKBUTTON *PNOTEBOOKBUTTON;
   
   /**********************************************************************/
   /* BKN_PAGEDELETED notify message structure                           */
   /**********************************************************************/
   typedef struct _DELETENOTIFY     /* delntfy */
   {
      HWND    hwndBook;                      /* Notebook window handle    */
      HWND    hwndPage;                      /* App. page window handle   */
      ULONG   ulAppPageData;                 /* App. page data            */
      HBITMAP hbmTab;                        /* App. tab bitmap handle    */
   } DELETENOTIFY;
   typedef DELETENOTIFY *PDELETENOTIFY;
   
   /**********************************************************************/
   /* BKN_PAGESELECTED notify message structure                          */
   /**********************************************************************/
   typedef struct _PAGESELECTNOTIFY     /* pgsntfy */
   {
      HWND    hwndBook;                      /* Notebook window handle    */
      ULONG   ulPageIdCur;                   /* Previous top page id      */
      ULONG   ulPageIdNew;                   /* New top Page id           */
   } PAGESELECTNOTIFY;
   typedef PAGESELECTNOTIFY *PPAGESELECTNOTIFY;
   
#endif /* !INCL_WINSTDBOOK_INCLUDED && INCL_WINSTDBOOK */

/**********************************************************************/
/* The following range of messages is reserved for Bidirectional      */
/* Languages support:                                                 */
/**********************************************************************/
#define  CTLS_WM_BIDI_FIRST   0x390
#define  CTLS_WM_BIDI_LAST    0x39F

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
