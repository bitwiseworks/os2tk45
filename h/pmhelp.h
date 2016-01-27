/****************************** Module Header ******************************\
*
* Module Name: PMHELP.H
*
* OS/2 Presentation Manager Information Presentation Facility,
* Help Manager declarations.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1993
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_WINHELP            Help manager structures and APIs
*   INCL_DDF                Dynamic Data Formatting APIs
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between XLATOFF
* and XLATON comments.
*
\***************************************************************************/

/* XLATOFF */
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
/* XLATON */

/* XLATOFF */
#ifndef PMHELP_INCLUDED
   /* XLATON */
   #define PMHELP_INCLUDED
   
   
   
   /************************************************************************/
   /* HelpSubTable entry structure                                         */
   /************************************************************************/
   typedef USHORT HELPSUBTABLE;
   typedef HELPSUBTABLE *PHELPSUBTABLE;
   
   /************************************************************************/
   /* HelpTable entry structure                                            */
   /*                                                                      */
   /*  Pack the structure HELPTABLE so that it is identical in the 32-Bit  */
   /*  and 16-Bit world.  We have to do this because the HelpTable can     */
   /*  reside either in memory or in the application's resources.          */
   /************************************************************************/
   
   /* XLATOFF */
   #pragma pack(2)   /* pack on word boundary */
   /* XLATON */
   
   typedef struct _HELPTABLE       /* ht */
   {
      USHORT          idAppWindow;
      PHELPSUBTABLE   phstHelpSubTable;
      USHORT          idExtPanel;
   } HELPTABLE;
   typedef HELPTABLE *PHELPTABLE;
   
   /* XLATOFF */
   #pragma pack()    /* reset to default packing */
   /* XLATON */
   
   
   /************************************************************************/
   /* IPF Initialization Structure used on the                             */
   /* WinCreateHelpInstance() call.                                        */
   /************************************************************************/
   
   typedef struct _HELPINIT      /* hinit 32 bit */
   {
      ULONG        cb;
      ULONG        ulReturnCode;
      PSZ          pszTutorialName;
      PHELPTABLE   phtHelpTable;
      HMODULE      hmodHelpTableModule;
      HMODULE      hmodAccelActionBarModule;
      ULONG        idAccelTable;
      ULONG        idActionBar;
      PSZ          pszHelpWindowTitle;
      ULONG        fShowPanelId;
      PSZ          pszHelpLibraryName;
   } HELPINIT;
   typedef HELPINIT *PHELPINIT;
   
   
   /************************************************************************/
   /* Search parent chain indicator for HM_SET_ACTIVE_WINDOW message.      */
   /************************************************************************/
   #define HWND_PARENT         (HWND)NULL
   
   /************************************************************************/
   /* Constants used to define whether user wants to display panel using   */
   /* panel number or panel name.                                          */
   /************************************************************************/
   #define HM_RESOURCEID             0
   #define HM_PANELNAME              1
   
   #define HMPANELTYPE_NUMBER        0
   #define HMPANELTYPE_NAME          1
   
   /************************************************************************/
   /* Constants used to define how the panel IDs are displayed on          */
   /* help panels.                                                         */
   /************************************************************************/
   #define CMIC_HIDE_PANEL_ID        0x0000
   #define CMIC_SHOW_PANEL_ID        0x0001
   #define CMIC_TOGGLE_PANEL_ID      0x0002
   
   /************************************************************************/
   /* Window Help API declarations.                                        */
   /************************************************************************/
   BOOL APIENTRY  WinDestroyHelpInstance(HWND hwndHelpInstance);
   
   HWND APIENTRY  WinCreateHelpInstance(HAB hab,
                                        PHELPINIT phinitHMInitStructure);
   
   BOOL APIENTRY  WinAssociateHelpInstance(HWND hwndHelpInstance,
                                           HWND hwndApp);
   
   HWND APIENTRY  WinQueryHelpInstance(HWND hwndApp);
   
   BOOL APIENTRY  WinLoadHelpTable(HWND hwndHelpInstance,
                                   ULONG idHelpTable,
                                   HMODULE Module);
   
   BOOL APIENTRY  WinCreateHelpTable(HWND hwndHelpInstance,
                                     PHELPTABLE phtHelpTable);
   
   /************************************************************************/
   /* IPF message base.                                                    */
   /************************************************************************/
   #define HM_MSG_BASE               0x0220
   
   /************************************************************************/
   /* Messages applications can send to the IPF.                           */
   /************************************************************************/
   #define HM_DISMISS_WINDOW              HM_MSG_BASE+0x0001
   #define HM_DISPLAY_HELP                HM_MSG_BASE+0x0002
   #define HM_EXT_HELP                    HM_MSG_BASE+0x0003
   #define HM_GENERAL_HELP                HM_EXT_HELP
   #define HM_SET_ACTIVE_WINDOW           HM_MSG_BASE+0x0004
   #define HM_LOAD_HELP_TABLE             HM_MSG_BASE+0x0005
   #define HM_CREATE_HELP_TABLE           HM_MSG_BASE+0x0006
   #define HM_SET_HELP_WINDOW_TITLE       HM_MSG_BASE+0x0007
   #define HM_SET_SHOW_PANEL_ID           HM_MSG_BASE+0x0008
   #define HM_REPLACE_HELP_FOR_HELP       HM_MSG_BASE+0x0009
   #define HM_REPLACE_USING_HELP          HM_REPLACE_HELP_FOR_HELP
   #define HM_HELP_INDEX                  HM_MSG_BASE+0x000a
   #define HM_HELP_CONTENTS               HM_MSG_BASE+0x000b
   #define HM_KEYS_HELP                   HM_MSG_BASE+0x000c
   #define HM_SET_HELP_LIBRARY_NAME       HM_MSG_BASE+0x000d
   
   #define HM_SET_OBJCOM_WINDOW           HM_MSG_BASE+0x0018
   #define HM_UPDATE_OBJCOM_WINDOW_CHAIN  HM_MSG_BASE+0x0019
   #define HM_QUERY_DDF_DATA              HM_MSG_BASE+0x001a
   #define HM_INVALIDATE_DDF_DATA         HM_MSG_BASE+0x001b
   #define HM_QUERY                       HM_MSG_BASE+0x001c
   #define HM_SET_COVERPAGE_SIZE          HM_MSG_BASE+0x001d
   
   /************************************************************************/
   /* Constants used to query the info from IPF in HM_QUERY message        */
   /************************************************************************/
   
   /* Hi word in lParam 1 */
   #define HMQW_COVERPAGE           0x0001
   #define HMQW_INDEX               0x0002
   #define HMQW_TOC                 0x0003
   #define HMQW_SEARCH              0x0004
   #define HMQW_VIEWPAGES           0x0005
   #define HMQW_LIBRARY             0x0006
   #define HMQW_VIEWPORT            0x0007
   #define HMQW_OBJCOM_WINDOW       0x0008
   #define HMQW_INSTANCE            0x0009
   #define HMQW_ACTIVEVIEWPORT      0x000a
   #define CONTROL_SELECTED         0x000b
   
   #define HMQW_GROUP_VIEWPORT      0x00f1
   #define HMQW_RES_VIEWPORT        0x00f2
   #define USERDATA                 0x00f3
   
   /* Lo word in lParam1 of HMQW_VIEWPORT */
   #define HMQVP_NUMBER             0x0001
   #define HMQVP_NAME               0x0002
   #define HMQVP_GROUP              0x0003
   
   /************************************************************************/
   /* Predefined Control IDs                                               */
   /************************************************************************/
   #define CTRL_PREVIOUS_ID    ((USHORT)0x0001)
   #define CTRL_SEARCH_ID      ((USHORT)0x0002)
   #define CTRL_PRINT_ID       ((USHORT)0x0003)
   #define CTRL_INDEX_ID       ((USHORT)0x0004)
   #define CTRL_CONTENTS_ID    ((USHORT)0x0005)
   #define CTRL_BACK_ID        ((USHORT)0x0006)
   #define CTRL_FORWARD_ID     ((USHORT)0x0007)
   #define CTRL_TUTORIAL_ID    ((USHORT)0x00FF)
   
   #define CTRL_USER_ID_BASE   ((USHORT)257)
   
   /************************************************************************/
   /* Messages the IPF sends to the applications active window             */
   /* as defined by the IPF.                                               */
   /************************************************************************/
   
   #define HM_ERROR                       HM_MSG_BASE+0x000e
   #define HM_HELPSUBITEM_NOT_FOUND       HM_MSG_BASE+0x000f
   #define HM_QUERY_KEYS_HELP             HM_MSG_BASE+0x0010
   #define HM_TUTORIAL                    HM_MSG_BASE+0x0011
   #define HM_EXT_HELP_UNDEFINED          HM_MSG_BASE+0x0012
   #define HM_GENERAL_HELP_UNDEFINED      HM_EXT_HELP_UNDEFINED
   #define HM_ACTIONBAR_COMMAND           HM_MSG_BASE+0x0013
   #define HM_INFORM                      HM_MSG_BASE+0x0014
   #define HM_NOTIFY                      HM_MSG_BASE+0x0022
   #define HM_SET_USERDATA                HM_MSG_BASE+0x0023
   #define HM_CONTROL                     HM_MSG_BASE+0x0024
   
   /************************************************************************/
   /* notify information for HM_NOTIFY                                     */
   /************************************************************************/
   #define OPEN_COVERPAGE        0x0001
   #define OPEN_PAGE             0x0002
   #define SWAP_PAGE             0x0003
   #define OPEN_TOC              0x0004
   #define OPEN_INDEX            0x0005
   #define OPEN_HISTORY          0x0006
   #define OPEN_SEARCH_HIT_LIST  0x0007
   #define OPEN_LIBRARY          0x0008
   #define HELP_REQUESTED        0x0009
   
   /************************************************************************/
   /* HMERR_NO_FRAME_WND_IN_CHAIN - There is no frame window in the        */
   /* window chain from which to find or set the associated help           */
   /* instance.                                                            */
   /************************************************************************/
   #define  HMERR_NO_FRAME_WND_IN_CHAIN                0x00001001L
   
   /***********************************************************************/
   /* HMERR_INVALID_ASSOC_APP_WND - The application window handle         */
   /* specified on the WinAssociateHelpInstance() call is not a valid     */
   /* window handle.                                                      */
   /***********************************************************************/
   #define  HMERR_INVALID_ASSOC_APP_WND                0x00001002L
   
   /***********************************************************************/
   /* HMERR_INVALID_ASSOC_HELP_INST - The help instance handle specified  */
   /* on the WinAssociateHelpInstance() call is not a valid               */
   /* window handle.                                                      */
   /***********************************************************************/
   #define  HMERR_INVALID_ASSOC_HELP_INST              0x00001003L
   
   /***********************************************************************/
   /* HMERR_INVALID_DESTROY_HELP_INST - The window handle specified       */
   /* as the help instance to destroy is not of the help instance class.  */
   /***********************************************************************/
   #define  HMERR_INVALID_DESTROY_HELP_INST            0x00001004L
   
   /***********************************************************************/
   /* HMERR_NO_HELP_INST_IN_CHAIN - The parent or owner chain of the      */
   /* application window specified does not have a help instance          */
   /* associated with it.                                                 */
   /***********************************************************************/
   #define  HMERR_NO_HELP_INST_IN_CHAIN                0x00001005L
   
   /***********************************************************************/
   /* HMERR_INVALID_HELP_INSTANCE_HDL - The handle specified to be a      */
   /* help instance does not have the class name of a IPF                 */
   /* help instance.                                                      */
   /***********************************************************************/
   #define  HMERR_INVALID_HELP_INSTANCE_HDL            0x00001006L
   
   /***********************************************************************/
   /* HMERR_INVALID_QUERY_APP_WND - The application window specified on   */
   /* a WinQueryHelpInstance() call is not a valid window handle.         */
   /***********************************************************************/
   #define  HMERR_INVALID_QUERY_APP_WND                0x00001007L
   
   /***********************************************************************/
   /* HMERR_HELP_INST_CALLED_INVALID -  The handle of the help instance   */
   /* specified on an API call to the IPF does not have the               */
   /* class name of an IPF help instance.                                 */
   /***********************************************************************/
   #define  HMERR_HELP_INST_CALLED_INVALID             0x00001008L
   
   #define  HMERR_HELPTABLE_UNDEFINE                   0x00001009L
   #define  HMERR_HELP_INSTANCE_UNDEFINE               0x0000100aL
   #define  HMERR_HELPITEM_NOT_FOUND                   0x0000100bL
   #define  HMERR_INVALID_HELPSUBITEM_SIZE             0x0000100cL
   #define  HMERR_HELPSUBITEM_NOT_FOUND                0x0000100dL
   
   /***********************************************************************/
   /* HMERR_INDEX_NOT_FOUND - No index in library file.                   */
   /***********************************************************************/
   #define  HMERR_INDEX_NOT_FOUND                      0x00002001L
   
   /***********************************************************************/
   /* HMERR_CONTENT_NOT_FOUND - Library file does not have any contents.  */
   /***********************************************************************/
   #define  HMERR_CONTENT_NOT_FOUND                    0x00002002L
   
   /***********************************************************************/
   /* HMERR_OPEN_LIB_FILE     - Cannot open library file                  */
   /***********************************************************************/
   #define  HMERR_OPEN_LIB_FILE                        0x00002003L
   
   /***********************************************************************/
   /* HMERR_READ_LIB_FILE     - Cannot read library file                  */
   /***********************************************************************/
   #define  HMERR_READ_LIB_FILE                        0x00002004L
   
   /***********************************************************************/
   /* HMERR_CLOSE_LIB_FILE    - Cannot close library file                 */
   /***********************************************************************/
   #define  HMERR_CLOSE_LIB_FILE                       0x00002005L
   
   /***********************************************************************/
   /* HMERR_INVALID_LIB_FILE  - Improper library file provided            */
   /***********************************************************************/
   #define  HMERR_INVALID_LIB_FILE                     0x00002006L
   
   /***********************************************************************/
   /* HMERR_NO_MEMORY - Unable to allocate the requested amount of memory.*/
   /***********************************************************************/
   #define  HMERR_NO_MEMORY                            0x00002007L
   
   /***********************************************************************/
   /* HMERR_ALLOCATE_SEGMENT - Unable                                     */
   /* to allocate a segment of memory for memory allocation requested     */
   /* from the IPF.                                                       */
   /***********************************************************************/
   #define  HMERR_ALLOCATE_SEGMENT                     0x00002008L
   
   /***********************************************************************/
   /* HMERR_FREE_MEMORY - Unable to free allocated  memory                */
   /***********************************************************************/
   #define  HMERR_FREE_MEMORY                          0x00002009L
   
   /***********************************************************************/
   /* HMERR_PANEL_NOT_FOUND  - Unable                                     */
   /* to find a help panel requested to help manager                      */
   /***********************************************************************/
   #define  HMERR_PANEL_NOT_FOUND                      0x00002010L
   
   /***********************************************************************/
   /* HMERR_DATABASE_NOT_OPEN - Unable to read the unopened database      */
   /***********************************************************************/
   #define  HMERR_DATABASE_NOT_OPEN                    0x00002011L
   
   /***********************************************************************/
   /* HMERR_DDL_ERROR - Unable to load resource dll                       */
   /***********************************************************************/
   #define  HMERR_LOAD_DLL                              0x00002013L
   
   /***********************************************************************/
   /* AC Viewport stucture definitions                                    */
   /***********************************************************************/
   typedef struct _ACVP        /* acvp */
   {
      ULONG  cb;
      HAB    hAB;
      HMQ    hmq;
      ULONG  ObjectID;            /* object identifier */
      HWND   hWndParent;          /* IPF viewport client handle */
      HWND   hWndOwner;           /* IPF viewport client handle */
      HWND   hWndACVP;            /* applications frame window hwnd */
   } ACVP, *PACVP;
   
   /***********************************************************************/
   /* DDF defines and prototypes                                          */
   /***********************************************************************/
   #ifndef PMDDF_H
      #define PMDDF_H
   #endif /* PMDDF_H */
   
   #ifdef INCL_DDF
      /*********************************************************************/
      /* Define Handle to DDF                                              */
      /*********************************************************************/
      typedef VOID *HDDF;
      
      /* DdfHyperText Flags */
      #define REFERENCE_BY_ID     0L
      #define REFERENCE_BY_RES    1L
      
      /* DdfBeginList formatting flags */
      #define HMBT_NONE           1L
      #define HMBT_ALL            2L
      #define HMBT_FIT            3L
      
      #define HMLS_SINGLELINE     1L
      #define HMLS_DOUBLELINE     2L
      
      /* DdfBitmap alignment flags */
      #define ART_RUNIN           0x10L
      #define ART_LEFT            0x01L
      #define ART_RIGHT           0x02L
      #define ART_CENTER          0x04L
      
      /* DdfSetColor Color Flag */
      #define CLR_UNCHANGED      (-6L)
      
      /**********************************************************************/
      /* DDF API declarations.                                              */
      /**********************************************************************/
      
      HDDF APIENTRY  DdfInitialize (HWND hwndHelpInstance,
                                    ULONG cbBuffer,
                                    ULONG ulIncrement);
      
      BOOL APIENTRY  DdfPara (HDDF hddf);
      
      BOOL APIENTRY  DdfSetFormat (HDDF hddf,
                                   ULONG fFormatType);
      
      BOOL APIENTRY  DdfSetTextAlign (HDDF hddf,
                                      ULONG fAlign);
      
      BOOL APIENTRY  DdfSetColor (HDDF hddf,
                                  COLOR fBackColor,
                                  COLOR fForColor);
      
#ifdef __cplusplus
      BOOL APIENTRY  DdfInform (HDDF hddf,
                                PCSZ  pszText,
                                ULONG resInformNumber);
#else
      BOOL APIENTRY  DdfInform (HDDF hddf,
                                PSZ pszText,
                                ULONG resInformNumber);
#endif
      
      BOOL APIENTRY  DdfSetFontStyle (HDDF hddf,
                                      ULONG fFontStyle);
      
#ifdef __cplusplus
      BOOL APIENTRY  DdfHyperText (HDDF hddf,
                                   PCSZ  pszText,
                                   PCSZ  pszReference,
                                   ULONG fReferenceType);
#else
      BOOL APIENTRY  DdfHyperText (HDDF hddf,
                                   PSZ pszText,
                                   PSZ pszReference,
                                   ULONG fReferenceType);
#endif
      
      BOOL APIENTRY  DdfBeginList (HDDF hddf,
                                   ULONG ulWidthDT,
                                   ULONG fBreakType,
                                   ULONG fSpacing);
      
#ifdef __cplusplus
      BOOL APIENTRY  DdfListItem (HDDF hddf,
                                  PCSZ  pszTerm,
                                  PCSZ  pszDescription);
#else
      BOOL APIENTRY  DdfListItem (HDDF hddf,
                                  PSZ pszTerm,
                                  PSZ pszDescription);
#endif
      
      BOOL APIENTRY  DdfEndList (HDDF hddf);
      
      BOOL APIENTRY  DdfMetafile (HDDF hddf,
                                  HMF hmf,
                                  PRECTL prclRect);
      
#ifdef __cplusplus
      BOOL APIENTRY  DdfText (HDDF hddf,
                              PCSZ  pszText);
#else
      BOOL APIENTRY  DdfText (HDDF hddf,
                              PSZ pszText);
#endif
      
#ifdef __cplusplus
      BOOL APIENTRY  DdfSetFont (HDDF hddf,
                                 PCSZ  pszFaceName,
                                 ULONG ulWidth,
                                 ULONG ulHeight);
#else
      BOOL APIENTRY  DdfSetFont (HDDF hddf,
                                 PSZ pszFaceName,
                                 ULONG ulWidth,
                                 ULONG ulHeight);
#endif
      
      BOOL APIENTRY  DdfBitmap (HDDF hddf,
                                HBITMAP hbm,
                                ULONG fAlign);
      
      /*********************************************************************/
      /* error codes returned by DDF API functions                         */
      /*********************************************************************/
      #define HMERR_DDF_MEMORY                  0x3001
      #define HMERR_DDF_ALIGN_TYPE              0x3002
      #define HMERR_DDF_BACKCOLOR               0x3003
      #define HMERR_DDF_FORECOLOR               0x3004
      #define HMERR_DDF_FONTSTYLE               0x3005
      #define HMERR_DDF_REFTYPE                 0x3006
      #define HMERR_DDF_LIST_UNCLOSED           0x3007
      #define HMERR_DDF_LIST_UNINITIALIZED      0x3008
      #define HMERR_DDF_LIST_BREAKTYPE          0x3009
      #define HMERR_DDF_LIST_SPACING            0x300A
      #define HMERR_DDF_HINSTANCE               0x300B
      #define HMERR_DDF_EXCEED_MAX_LENGTH       0x300C
      #define HMERR_DDF_EXCEED_MAX_INC          0x300D
      #define HMERR_DDF_INVALID_DDF             0x300E
      #define HMERR_DDF_FORMAT_TYPE             0x300F
      #define HMERR_DDF_INVALID_PARM            0x3010
      #define HMERR_DDF_INVALID_FONT            0x3011
      #define HMERR_DDF_SEVERE                  0x3012
      
   #endif /* INCL_DDF */
   /* XLATOFF */
   
   
#endif /* PMHELP_INCLUDED */
/* XLATON */

/* XLATOFF */
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
/* XLATON */

/**************************** end of file **********************************/
