/****************************************************************************/
/*                                                                          */
/* Module Name: SW.H                                                        */
/*                                                                          */
/* This is the multimedia include file that has the typedefs, defines and   */
/* function prototyes for Multimedia Applets.                               */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1990, 1991     */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
/* The folowing symbols are used in this file for conditional sections.     */
/*                                                                          */
/*   #define:                To include:                                    */
/*                                                                          */
/*   INCL_SECONDARYWINDOW     Secondary Window                              */
/*   INCL_GRAPHICBUTTON      Graphic Buttons                                */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif


#ifndef __SW_H
   #define __SW_H

   #ifdef INCL_SECONDARYWINDOW

      #define MAX_SMBDTEXT               35

      #define MB_ICONCUSTOM             0x0001

      #define SC_DEFAULTSIZE            0xc000          /* WM_COMMAND from SysMenu */

      #define WM_INITSECONDARYWINDOW    0x0519          /* MP1: NULL, MP2: CreateParams */

      #define QS_FRAME                  0x1             /* Flag set to query frame  */
      #define QS_DIALOG                 0x2             /* Flag set to query dialog */

      #pragma pack(4)

      typedef struct _SMBD
      {
         CHAR   achText[MAX_SMBDTEXT + 1]; /* Text of the button. eg. "~Cancel"    */
         ULONG  idButton;                  /* Button ID returned when user chooses */
         /*  button                              */
         LONG   flStyle;                   /* Button style or'ed with internal     */
         /*  styles                              */
      } SMBD;

      typedef SMBD * PSMBD;

      typedef struct _SMBINFO
      {
         HPOINTER hIcon;               /* Icon handle                              */
         ULONG    cButtons;            /* Number of buttons                        */
         ULONG    flStyle;             /* Icon style flags (MB_ICONQUESTION, etc...)*/
         HWND     hwndNotify;          /* Reserved                                 */
         PSMBD    psmbd;               /* Array of button definitions              */
      } SMBINFO;

      typedef SMBINFO * PSMBINFO;

      #pragma pack()

      /* Analogous to WinDlgBox */

      ULONG EXPENTRY  WinSecondaryWindow(HWND    hwndParent,
      HWND    hwndOwner,
      PFNWP   pfnDlgProc,
      HMODULE hmod,
      ULONG  idDlg,
      PVOID   pCreateParams);

      /* Analogous to WinLoadDlg */
      HWND EXPENTRY  WinLoadSecondaryWindow(HWND    hwndParent,
      HWND    hwndOwner,
      PFNWP   pfnDlgProc,
      HMODULE hmod,
      ULONG   idDlg,
      PVOID   pCreateParams);

      /* Analogous to WinProcessDlg */
      ULONG EXPENTRY  WinProcessSecondaryWindow(HWND hwndSW);

      /* Analogous to WinCreateDlg  */
      HWND EXPENTRY  WinCreateSecondaryWindow(HWND    hwndParent,
      HWND    hwndOwner,
      PFNWP   pfnDlgProc,
      PDLGTEMPLATE pdlgt,                       /* requires INCL_WINDIALOGS */
      PVOID   pCreateParams);

      BOOL EXPENTRY WinDefaultSize(HWND hwnd);

      BOOL EXPENTRY WinInsertDefaultSize(HWND hwnd, PSZ pszDefaultSize);

      HWND EXPENTRY WinQuerySecondaryHWND(HWND hwnd, ULONG ulFlag);

      /************************************************************************/
      /* WinSecondaryMessageBox                                               */
      /*                                                                      */
      /* Parameters: HWND   hwndParent   - handle of the parent window.       */
      /*             HWND   hwndOwner    - handle of the owner window.        */
      /*             PSZ    pszText      - message text.                      */
      /*             PSZ    pszCaption   - title of the message box.          */
      /*             ULONG  idWindow     - Message box id                     */
      /*             PSMBINFO psmbinfo   - pointer to button/icon info        */
      /************************************************************************/
      ULONG  EXPENTRY  WinSecondaryMessageBox(HWND     hwndParent,
      HWND     hwndOwner,
      PSZ      pszText,
      PSZ      pszCaption,
      ULONG    idWindow,
      PSMBINFO psmbinfo);

      /************************************************************************/
      /* WinDismissSecondaryWindow                                            */
      /*                                                                      */
      /* This function should be called from within the dlg proc. The hwnd    */
      /* passed in MUST be the handle to the actual dialog.                   */
      /************************************************************************/

      BOOL EXPENTRY WinDismissSecondaryWindow(HWND hwndDlg, ULONG ulResult);

      /************************************************************************/
      /* The parameter hwnd can be either the secondary window or the actual  */
      /* dialog.                                                              */
      /************************************************************************/

      BOOL EXPENTRY WinDestroySecondaryWindow(HWND hwnd);

      MRESULT EXPENTRY WinDefSecondaryWindowProc(HWND hwnd, ULONG msg, MPARAM mp1,
      MPARAM mp2);


   #endif /* ifdef INCL_SECONDARYWINDOW */

   #ifdef INCL_GRAPHICBUTTON

      /************************************************************************/
      /*                         Graphic Buttons                              */
      /************************************************************************/

      /************************************************************************/
      /* Notes on Using GraphicButtons                                        */
      /*                                                                      */
      /* GraphicButton CONTROL DATA                                           */
      /*                                                                      */
      /*  The format of the control data for GraphicButtons is                */
      /*         "button Text, number of bitmaps, bitmap resource id's ..."   */
      /*                                                                      */
      /*                                                                      */
      /*  Following are two example window templates of GraphicButtons:       */
      /*                                                                      */
      /*      1)  CONTROL  "", IDD_MP_REV, 120, 10, TS_PB_WIDTH, TS_PB_HEIGHT,*/
      /*                 WC_GRAPHICBUTTON,                                    */
      /*                 GBS_TWOSTATE | GBS_HILITEBITMAP |                    */
      /*                 WS_VISIBLE | WS_TABSTOP                              */
      /*                 CTLDATA GB_RESOURCE,"~REV", 2, ID_MP_REV1, ID_MP_REV0*/
      /*                                                                      */
      /*  The above graphicbutton has id IDD_MP_REV and is of type            */
      /*  GBS_TWOSTATE and GBS_HILITEBITMAP.  The GBS_HILITEBITMAP allows     */
      /*  a different bitmap to be displayed when the button is in the        */
      /*  hilite state. The graphicbutton will be displayed with text         */
      /*  "REV" and has "R" as the mnemonic.  It has 2 bitmaps associated     */
      /*  with it.  Their resource id are ID_MP_REV1 and ID_MP_REV0.          */
      /*                                                                      */
      /*                                                                      */
      /*      2)   CONTROL  "", IDD_MP_PLAY, 175, 10, TS_PB_WIDTH,            */
      /*                        TS_PB_HEIGHT, WC_GRAPHICBUTTON,               */
      /*                        GBS_AUTOTWOSTATE | GBS_AUTOANIMATION |        */
      /*                        WS_VISIBLE | WS_TABSTOP                       */
      /*                        CTLDATA GB_RESOURCE, "~PLAY", 9,              */
      /*                                ID_MP_STOP0, ID_MP_PLAY1, ID_MP_PLAY2,*/
      /*                                ID_MP_PLAY3, ID_MP_PLAY4, ID_MP_PLAY5,*/
      /*                                ID_MP_PLAY6, ID_MP_PLAY7, ID_MP_REV1  */
      /*                                                                      */
      /*  The above graphicbutton has id IDD_MP_PLAY and is of type           */
      /*  GBS_AUTOTWOSTATE and GBS_AUTOANIMATE.  When clicked upon,           */
      /*  the button will automatically toggle the state and animation.       */
      /*  The graphicbutton will be displayed with text "PLAY"                */
      /*  and mnemonic "P".  It has 9 bitmaps associated with it.             */
      /*                                                                      */
      /*                                                                      */
      /*                                                                      */
      /*  GraphicButton Painting                                              */
      /*                                                                      */
      /*  Due to the PM design, whenever a graphicbutton is clicked,          */
      /*  it is sent a BN_PAINT (to paint a non-hilite state) and then        */
      /*  BN_CLICKED.  Thus, for GBS_AUTO* style graphicbuttons, a paint      */
      /*  request is generated before the button has a chance to change       */
      /*  its state (BN_CLICKED).  To avoid this premature painting,          */
      /*  code was inserted to delay the painting of graphicbuttons           */
      /*  GB_PAINT_RESOLUTION milliseconds whenever the button is switching   */
      /*  FROM the hilite paint state.                                        */
      /*                                                                      */
      /************************************************************************/

      BOOL EXPENTRY WinRegisterGraphicButton(VOID);

      #define WC_GRAPHICBUTTON ((PSZ)0xffff0040L)

      /************************************************************************/
      /*             GraphicButton Animation/TwoState constants               */
      /************************************************************************/

      /************************************************************************/
      /*                     Graphic Button Style bits                        */
      /************************************************************************/

      #define GBS_TWOSTATE        0x1000  /* indicates TwoState button        */
      #define GBS_AUTOTWOSTATE    0X2000  /* will auto toggle state up/down   */
      #define GBS_ANIMATION       0x4000  /* indicates Animatable button      */
      #define GBS_AUTOANIMATION   0x8000  /* will auto toggle anim. on/off    */
      #define GBS_DISABLEBITMAP   0x0010  /* allows a diff. bitmap when       */
      /* disabled                         */
      #define GBS_HILITEBITMAP    0x0020  /* allows a diff. bitmap when       */
      /* hilited                          */
      #define GBS_3D_TEXTRECESSED 0x0040  /* display text in 3-D recessed     */
      #define GBS_3D_TEXTRAISED   0x0080  /* display text in 3-D raised       */
      #define GBS_MINIBUTTON      0x0001  /* mini button style                */

      /************************************************************************/
      /*                     Graphic Button User Constants                    */
      /************************************************************************/

      /************************************************************************/
      /* Codes to reference allowed GraphicButton states (or paint states)    */
      /************************************************************************/

      #define GB_ERROR           -1       /* GraphicButton Error              */
      #define GB_UP               1       /* GraphicButton up (and paint)     */
      /* state                            */
      #define GB_DOWN             2       /* GraphicButton down (and paint)   */
      /* state                            */
      #define GB_DISABLE          3       /* GraphicButton disabled state     */
      #define GB_HILITE           4       /* GraphicButton paint state only   */
      #define GB_OUTOFHILITE      5       /* Changing out of hilite paint     */
      /* state                            */

      /************************************************************************/
      /* Codes for various GraphicButton message function parameters          */
      /************************************************************************/

      #define GB_TOGGLE           10      /* GraphicButton toggle             */
      #define GB_CURRENTSTATE     11      /* GraphicButton's current state    */
      #define GB_ANIMATIONBEGIN   12      /* when refering to index of anim   */
      /* start                            */
      #define GB_ANIMATIONEND     13      /* when refering to index of anim   */
      /* end                              */
      #define GB_MAXINDEX         14      /* GraphicButton max. index         */

      /************************************************************************/
      /* Codes to set/query text position relative to the bitmap              */
      /************************************************************************/

      #define GB_TEXTBELOW        1       /* place text below bitmap          */
      #define GB_TEXTABOVE        2       /* place text above bitmap          */

      /************************************************************************/
      /* Codes used to set the animation frame with message GBM_SETBITMAPINDEX*/
      /************************************************************************/

      #define GB_INDEX_FORWARD   -1       /* advance one frame foward         */
      #define GB_INDEX_BACKWARD  -2       /* advance one frame backwards      */
      #define GB_INDEX_FIRST     -3       /* set to first frame of sequence   */
      #define GB_INDEX_LAST      -4       /* set to last frame of sequence    */

      #pragma pack(1)

      typedef struct _GBTNCDATA
      {
         USHORT  usReserved;
         PSZ     pszText;
         HMODULE hmod;
         USHORT  cBitmaps;
         USHORT  aidBitmap[1];
      } GBTNCDATA;

      typedef GBTNCDATA *PGBTNCDATA;

      #pragma pack()

      #define GB_RESOURCE  1
      #define GB_STRUCTURE 0


      /************************************************************************/
      /*          Notification Messages received by GraphicButton Parent      */
      /***********************************************************************/
      /************************************************************************/
      /* GBN_BUTTONDOWN, GBN_BUTTONUP, and GBN_BUTTONHILITE                   */
      /*                                                                      */
      /* The notification messages are passed as part of the WM_CONTROL       */
      /* message.                                                             */
      /*                                                                      */
      /* msg = WM_CONTROL                                                     */
      /* mp1 = MPFROM2SHORT(gpb_id, button_state)                             */
      /*         gpd_id       = identity of the displayed graphic pushbutton  */
      /*         button_state = GBN_BUTTONUP, GBN_BUTTONDOWN, or              */
      /*                        GBN_BUTTONHILITE                              */
      /*                                                                      */
      /************************************************************************/

      #define GBN_BUTTONUP     0x0524
      #define GBN_BUTTONDOWN   0x0525
      #define GBN_BUTTONHILITE 0x0526
      #define GBN_SETFOCUS     0x0527   /* mp2 TRUE for gaining focus */

      /************************************************************************/
      /*          Messages to GraphicButton Windows                           */
      /************************************************************************/

      /************************************************************************/
      /*                             GBM_SETGRAPHICDATA                       */
      /************************************************************************/
      /* mp1 = MPFROMP((PGBTNCDATA)&gbtncdata);    Graphic button control data*/
      /* mp2 = NULL;                               not used                   */
      /*                                                                      */
      /* WARNING: This message resets all button parameters.                  */
      /*                                                                      */
      /************************************************************************/

      #define GBM_SETGRAPHICDATA   0x052A


      /************************************************************************/
      /*                             GBM_ANIMATE                              */
      /************************************************************************/
      /* mp1 = MPFROMSHORT(fStart)      TRUE to start animation, FALSE to stop*/
      /* mp2 = MPFROMSHORT(fContinue)   TRUE continue anim. at currently      */
      /*                                displayed bitmap, FALSE restart at    */
      /*                                the beginning.                        */
      /*                                                                      */
      /* Returns TRUE on Success                                              */
      /*         FALSE on Failure                                             */
      /*                                                                      */
      /************************************************************************/

      #define GBM_ANIMATE 0x052B


      /************************************************************************/
      /*                             GBM_SETANIMATIONRATE                     */
      /************************************************************************/
      /* mp1 = MPFROMSHORT(ULmIL);      Animation rate in milliseconds        */
      /* mp2 = NULL                     not used                              */
      /*                                                                      */
      /*                                                                      */
      /* Returns TRUE on Success                                              */
      /*         FALSE on Failure                                             */
      /*                                                                      */
      /************************************************************************/

      #define GBM_SETANIMATIONRATE 0x052C


      /************************************************************************/
      /*                             GBM_QUERYANIMATIONACTIVE                 */
      /************************************************************************/
      /* mp1 = NULL;                    not used                              */
      /* mp2 = NULL;                    not used                              */
      /*                                                                      */
      /*                                                                      */
      /* Returns TRUE if animation is active, else GB_ERROR                   */
      /*                                                                      */
      /*                                                                      */
      /************************************************************************/

      #define GBM_QUERYANIMATIONACTIVE 0x052D


      /************************************************************************/
      /*                             GBM_QUERYANIMATIONRATE                   */
      /************************************************************************/
      /* mp1 = NULL;                    not used                              */
      /* mp2 = NULL;                    not used                              */
      /*                                                                      */
      /*                                                                      */
      /* Returns ULONG sepcifying animation rate in milliseconds              */
      /*                                                                      */
      /*                                                                      */
      /************************************************************************/

      #define GBM_QUERYANIMATIONRATE 0x052E


      /************************************************************************/
      /*                             GBM_SETBITMAPINDEX                       */
      /************************************************************************/
      /* mp1 = MPFROMSHORT(usGB_State)       Bitmap index to change           */
      /*             GB_UP,                                                   */
      /*             GB_DOWN,                                                 */
      /*             GB_DISABLE,                                              */
      /*             GB_HILITE,                                               */
      /*             GB_ANIMATIONBEGIN,                                       */
      /*             GB_ANIMATIONEND,                                         */
      /*          or GB_CURRENTSTATE    chng current state (up or down) bitmap*/
      /* mp2 = MPFROMSHORT(sFrameCode)  Set according to code or frame desire */
      /*             GB_INDEX_FORWARD,  chng to next bitmap in circular array */
      /*             GB_INDEX_BACKWARD, "   "  prev   "    "     "       "    */
      /*             GB_INDEX_FIRST,    "   "  first  "    "     "       "    */
      /*             GB_INDEX_LAST,     "   "  last   "    "     "       "    */
      /*          or desired_bitmap     otherwise desired bmp index specified */
      /*                                                                      */
      /* Returns TRUE on Success, otherwise FALSE                             */
      /*                                                                      */
      /************************************************************************/

      #define GBM_SETBITMAPINDEX 0x052F


      /************************************************************************/
      /*                             GBM_QUERYBITMAPINDEX                     */
      /************************************************************************/
      /*  mp1 = MPFROMSHORT(usGB_State)       Query bitmap index              */
      /*            GB_UP,                                                    */
      /*            GB_DOWN,                                                  */
      /*            GB_DISABLE,                                               */
      /*            GB_HILITE,                                                */
      /*            GB_ANIMATIONBEGIN,                                        */
      /*            GB_ANIMATIONEND,                                          */
      /*         or GB_CURRENTSTATE  query current state (up or down) bitmap  */
      /*  mp2 = NULL;                    not used                             */
      /*                                                                      */
      /*  Returns USHORT specifying the index                                 */
      /*                                                                      */
      /*                                                                      */
      /************************************************************************/

      #define GBM_QUERYBITMAPINDEX 0x0530

      /************************************************************************/
      /*                             GBM_SETSTATE                             */
      /************************************************************************/
      /* mp1 = MPFROMSHORT(usStateCode)   Set twostate button to specified    */
      /*                                  state                               */
      /*             GB_UP,                                                   */
      /*             GB_DOWN,                                                 */
      /*          or GB_TOGGLE          toggle (up/down) to (down/up)         */
      /* mp2 = MPFROMBOOL(fRepaint)     TRUE  - state changed and displayed   */
      /*                                FALSE - state changed, not displayed  */
      /*                                                                      */
      /* Returns TRUE on Success                                              */
      /*         FALSE on Failure                                             */
      /*                                                                      */
      /*                                                                      */
      /************************************************************************/

      #define GBM_SETSTATE 0x0531

      /************************************************************************/
      /*                             GBM_QUERYSTATE                           */
      /************************************************************************/
      /*                                                                      */
      /* mp1 = NULL                        not used                           */
      /* mp2 = NULL                        not used                           */
      /*                                                                      */
      /* Returns the state (GB_UP or GB_DOWN) else GB_ERROR.                  */
      /*                                                                      */
      /*                                                                      */
      /************************************************************************/

      #define GBM_QUERYSTATE 0x0532

      /************************************************************************/
      /*                             GBM_SETTEXTPOSITION                      */
      /************************************************************************/
      /* mp1 = MPFROMSHORT(usTextPos)   How to position text relative to      */
      /*                                  bitmap                              */
      /*               GB_TEXTBELOW,                                          */
      /*            or GB_TEXTABOVE                                           */
      /*   mp2 = NULL                     not used                            */
      /*                                                                      */
      /*   Returns TRUE on Success, otherwise FALSE                           */
      /*                                                                      */
      /************************************************************************/

      #define GBM_SETTEXTPOSITION 0x0533

      /************************************************************************/
      /*                             GBM_QUERYTEXTPOSITION                    */
      /************************************************************************/
      /*                                                                      */
      /*  mp1 = NULL                          not used                        */
      /*  mp2 = NULL                          not used                        */
      /*                                                                      */
      /*  Returns GB_TEXTBELOW, GB_TEXTABOVE, GB_TEXTRIGHT, GB_TEXTLEFT       */
      /*  on success, otherwise FALSE                                         */
      /*                                                                      */
      /************************************************************************/

      #define GBM_QUERYTEXTPOSITION 0x0534


      /************************************************************************/
      /*                             GraphicButton END                        */
      /************************************************************************/

   #endif /* ifdef INCL_GRAPHICBUTTON  */

   #ifdef INCL_STPMPAGE
      #define MM_TABHELP  0x054C
   #endif /* INCL_STPMPAGE */


#endif /* ifndef __SW_H  */

#ifdef __cplusplus
}
#endif

