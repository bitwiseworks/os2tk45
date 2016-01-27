/****************************** Module Header ******************************\
*
* Module Name: OS2CTLWX.H
*
* OS/2 Presentation Manager Window Manager include file for WINX applications
* to use the new styles added for the PM controls as well as the dialog id's
* and structures needed for WINX specific dialogs.
*
* Copyright (c) International Business Machines Corporation 1995
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_WINLISTBOXES       List box controls
*   INCL_WINSTDFIND         standard find/replace dialogs
*   INCL_WINSTDCOLOR        standard color dialog
*   INCL_WINSTDPRINT        standard print dialog
*   INCL_WINSTDMLE          Multiple Line Edit
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

#ifdef INCL_WINLISTBOXES

      /* List box styles for WinX */

      #define LS_TABSTOPS               0x00000040L
      #define LS_OWNERDRAWVARIABLE      0x00000080L
      #define LS_NOSTRINGS              0x00000100L                 /* @136183 */
      #define LS_NOCURSOR               0x00000200L                 /* @148054 */
      #define LS_NOBORDER               0x00000400L                 /* @148609 */
      #define LS_NOTIFY                 0x00000800L                 /* @152609 */

      /* Combobox styles for WinX */

      #define CBS_OWNERDRAWVARIABLE     0x00000080L
      #define CBS_NOSTRINGS             0x00000100L       /* @136183 */
      #define CBS_OWNERDRAW             0x00000200L       /* @136247 */

#endif

#ifdef  INCL_WINSTDMLE
      #define MLS_WANTRETURN            0x00000100L
      #define MLS_MATCHVSCROLL          0x00000200L
      #define MLS_SETZEROMARGIN         0x00002000L                              //177439
#endif

#ifndef FILEDLG_DID_INCLUDED
   #define FILEDLG_DID_INCLUDED
   /*--------------------------------------------------------------------*/
   /* File Dialog - dialog and control ids (BASE)                        */
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


#ifndef FONTDLG_DID_INCLUDED
   #define FONTDLG_DID_INCLUDED
   /**********************************************************************/
   /* Font dialog and control id's  (BASE)                               */
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


#ifndef INCL_WINDLGFONTWINX_INCLUDED
   #define INCL_WINDLGFONTWINX_INCLUDED
   /**********************************************************************/
   /* Font dialog and control id's  (WINX SPECIFIC)                      */
   /**********************************************************************/
   #define DID_WINXFONT_DIALOG      320
   #define DID_COLOR                321
   #define DID_COLOR_PREFIX         322
#endif /* INCL_WINDLGFONTWINX_INCLUDED */


#ifndef FINDDLG_DID_INCLUDED
   #define FINDDLG_DID_INCLUDED
   /**********************************************************************/
   /* Find dialog Resource IDs (BASE)                                    */
   /**********************************************************************/
   #define DID_FIND_DIALOG             500
   #define DID_REPLACE_DIALOG          501

   #define DID_FIND_REQUIRED           502
   #define DID_FINDNEXT_PUSH           DID_OK
   #define DID_CLOSE_PUSH              DID_CANCEL
   #define DID_FINDWHAT_ENTRY          502
   #define DID_WHOLEWORD_CHECK         503
   #define DID_MATCHCASE_CHECK         504
   #define DID_FORWARD_RADIO           505
   #define DID_BACKWARD_RADIO          506
   #define DID_FINDWHAT_STATIC         507
   #define DID_DIRECTION_GROUP         508
   #define DID_OPTIONS_GROUP           509
   #define DID_HELP_PUSH               510
   #define DID_FIND_REQUIRED_LAST      510

   #define DID_REPLACE_REQUIRED        520
   #define DID_REPLACEWITH_ENTRY       520
   #define DID_REPLACE_PUSH            521
   #define DID_REPLACEALL_PUSH         522
   #define DID_REPLACEWITH_STATIC      523
   #define DID_REPLACE_REQUIRED_LAST   523
#endif  /* FINDDLG_DID_INCLUDED */


#ifndef COLORDLG_DID_INCLUDED
   #define COLORDLG_DID_INCLUDED
   /**********************************************************************/
   /*        C o l o r   D i a l o g / c o n t r o l   I D s  (BASE)     */
   /**********************************************************************/

   #define DID_COLORDLG               1027    /* ID of the color dialog */

   #define DID_RB_RGB                  515
   #define DID_RB_HSB                  517
   #define DID_RB_CMYK                 518

   #define DID_SPBTN1                  511
   #define DID_SPBTN2                  512
   #define DID_SPBTN3                  513
   #define DID_SPBTN4                  514

   #define DID_TITLE1                  501
   #define DID_TITLE2                  502
   #define DID_TITLE3                  503
   #define DID_TITLE4                  504

   #define DID_COLORCIRCLE             106
   #define DID_PB_VALUES               110
   #define DID_PB_HELP                 113
   #define DID_PB_PURE                 114
   #define DID_PB_UNDO                 115
   #define DID_CUSTOM                  116
   #define DID_VS_BASIC                117
   #define DID_VS_CUSTOM               118
   #define DID_TXT_BASIC               119
   #define DID_TXT_CUSTOM              120

   #define DID_IGNORE_ABOVE_THIS       10000
   #define DID_MARKER_FIRST            (DID_IGNORE_ABOVE_THIS+1000)
   #define DID_MRI_FIRST               (DID_IGNORE_ABOVE_THIS+2000)
   #define DID_POSITION_FIRST          (DID_IGNORE_ABOVE_THIS+3000)

   #define DID_MK_CENTER               (DID_MARKER_FIRST+1)
   #define DID_MK_UPPERLEFT            (DID_MARKER_FIRST+2)
   #define DID_MK_UPPERRIGHT           (DID_MARKER_FIRST+3)
   #define DID_MK_LOWERLEFT            (DID_MARKER_FIRST+4)
   #define DID_MK_LOWERRIGHT           (DID_MARKER_FIRST+5)

   #define DID_MRI_RED                 (DID_MRI_FIRST+1)
   #define DID_MRI_GREEN               (DID_MRI_FIRST+2)
   #define DID_MRI_BLUE                (DID_MRI_FIRST+3)
   #define DID_MRI_HUE                 (DID_MRI_FIRST+4)
   #define DID_MRI_SATURATION          (DID_MRI_FIRST+5)
   #define DID_MRI_BRIGHT              (DID_MRI_FIRST+6)
   #define DID_MRI_CYAN                (DID_MRI_FIRST+7)
   #define DID_MRI_MAGENTA             (DID_MRI_FIRST+8)
   #define DID_MRI_YELLOW              (DID_MRI_FIRST+9)
   #define DID_MRI_BLACK               (DID_MRI_FIRST+10)
   #define DID_MRI_DEF_CUST_COLOR      (DID_MRI_FIRST+11)

   /**********************************************************************/
   /* Perfect save color wheel                                           */
   /**********************************************************************/
   #define DID_POS_UNDO1               (DID_POSITION_FIRST+12)
   #define DID_POS_OK1                 0
   #define DID_POS_CANCEL1             0
   #define DID_POS_VALUES1             (DID_POSITION_FIRST+15)
   #define DID_POS_HELP1               (DID_POSITION_FIRST+16)
   #define DID_POS_SETCOLOR1           0

   /**********************************************************************/
   /* OK/Cancel color wheel                                              */
   /**********************************************************************/
   #define DID_POS_UNDO2               0
   #define DID_POS_OK2                 (DID_POSITION_FIRST+22)
   #define DID_POS_CANCEL2             (DID_POSITION_FIRST+23)
   #define DID_POS_VALUES2             (DID_POSITION_FIRST+24)
   #define DID_POS_HELP2               (DID_POSITION_FIRST+25)
   #define DID_POS_SETCOLOR2           0

   /**********************************************************************/
   /* Perfect save palette                                               */
   /**********************************************************************/
   #define DID_POS_UNDO3               (DID_POSITION_FIRST+32)
   #define DID_POS_OK3                 0
   #define DID_POS_CANCEL3             0
   #define DID_POS_VALUES3             (DID_POSITION_FIRST+34)
   #define DID_POS_HELP3               (DID_POSITION_FIRST+35)
   #define DID_POS_SETCOLOR3           (DID_POSITION_FIRST+36)

   /**********************************************************************/
   /* OK/Cancel palette                                                  */
   /**********************************************************************/
   #define DID_POS_UNDO4               0
   #define DID_POS_OK4                 (DID_POSITION_FIRST+32)
   #define DID_POS_CANCEL4             (DID_POSITION_FIRST+33)
   #define DID_POS_VALUES4             (DID_POSITION_FIRST+34)
   #define DID_POS_HELP4               (DID_POSITION_FIRST+35)
   #define DID_POS_SETCOLOR4           (DID_POSITION_FIRST+36)
#endif  /* COLORDLG_DID_INCLUDED */


#ifndef PRINT_DID_INCLUDED
   #define PRINT_DID_INCLUDED
   /************************************************************************/
   /* PRINT DIALOG RESOURCE IDs                                            */
   /************************************************************************/
   #define DID_PRINT_DIALOG         600

   #define DID_PRINT_REQUIRED       601
   #define PRID_PRINT_PUSH          DID_OK
   #define PRID_CANCEL_PUSH         DID_CANCEL
   #define PRID_QUEUE_GROUP         601
   #define PRID_QUEUE_CNR           602
   #define PRID_JOBPROP_PUSH        603
   #define PRID_OPTIONS_GROUP       604
   #define PRID_COPIES_STATIC       605
   #define PRID_COPIES_SPIN         606
   #define PRID_OPTION1_CHECK       607
   #define PRID_OPTION2_CHECK       608
   #define PRID_OPTION3_CHECK       609
   #define PRID_OPTION4_CHECK       610
   #define PRID_RANGE_GROUP         611
   #define PRID_ALL_RADIO           612
   #define PRID_SELECTION_RADIO     613
   #define PRID_PAGES_RADIO         614
   #define PRID_FROM_STATIC         615
   #define PRID_FROM_SPIN           616
   #define PRID_TO_STATIC           617
   #define PRID_TO_SPIN             618
   #define PRID_PREVIEW_PUSH        619
   #define PRID_HELP_PUSH           620
   #define DID_PRINT_REQUIRED_LAST  620

   #define DID_PRINT_NO_QUEUES      630
#endif  /* PRINT_DID_INCLUDED */

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
