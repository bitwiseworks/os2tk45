/*----------------------------------------------------------------------------*/
/* File Name   : Caption.c                                                    */
/*                                                                            */
/* Description : This file contains the C source code required for the        */
/*               Caption Creation Utility program.                            */
/*                                                                            */
/* Concepts    : This sample program is part of MMPM/2 sample closed          */
/*               captioning system.  This sample closed captioning system     */
/*               is composed of three parts: Caption Creation Utility (this   */
/*               (sample), Caption DLL, and Caption Sample Application.       */
/*               The Caption Creation Utility (this program) creates a        */
/*               "captioned" file. This is a text file with timing information*/
/*               relating to it's associated audio file.  The Caption DLL     */
/*               provides 3 API's that drive the display and management of a  */
/*               "caption window" in a PM application.  The caption sample    */
/*               illustrates how an application uses the 3 API's provided by  */
/*               the caption DLL to take advantage of it's services.          */
/*                                                                            */
/*               This sample captioning system is provided to illustrate one  */
/*               of the many ways that captioning may be implemented using    */
/*               MMPM/2. As with all MMPM/2 samples, this may be used as it   */
/*               is provided, or it may be modified in any way you prefer.    */
/*               Please refer to the Application Programmers Reference for    */
/*               more information on this sample captioning system.           */
/*                                                                            */
/* MMPM/2 API's :  List of all MMPM/2 API's that are used in                  */
/*                 this module                                                */
/*                                                                            */
/*                 mciSendCommand    MCI_OPEN                                 */
/*                                   MCI_PLAY                                 */
/*                                   MCI_CLOSE                                */
/*                                   MCI_SET                                  */
/*                                   MCI_STOP                                 */
/*                                   MCI_STATUS                               */
/*                                   MCI_PAUSE                                */
/*                                   MCI_LOAD                                 */
/*                                   MCI_SET_POSITION_ADVISE                  */
/*                                   MCI_ACQUIREDEVICE                        */
/*                 mciGetErrorString                                          */
/*                                                                            */
/* Required                                                                   */
/*    Files     :  caption.c        Source Code.                              */
/*                 caption.h        Include file.                             */
/*                 caption.dlg      Dialog definition.                        */
/*                 caption.rc       Resources.                                */
/*                 caption.ipf      Help text.                                */
/*                 makefile         Make file.                                */
/*                 caption.def      Linker definition file.                   */
/*                 caption.ico      Program icon.                             */
/*                 nextline.bmp     Next line pushbutton bitmap               */
/*                 rewind.bmp       Rewind pushbutton bitmap                  */
/*                 pause. BMP       Pause pushbutton bitmap                   */
/*                 nextbusy.bmp     Next line pushbutton bitmap               */
/*                 stop.bmp         Stop pushbutton bitmap                    */
/*                 paused.bmp       Pause pushbutton bitmap                   */
/*                 play0.bmp        Play pushbutton bitmap                    */
/*                 play1.bmp        Play pushbutton bitmap                    */
/*                 play2.bmp        Play pushbutton bitmap                    */
/*                 play3.bmp        Play pushbutton bitmap                    */
/*                 play4.bmp        Play pushbutton bitmap                    */
/*                                                                            */
/* Copyright (C) IBM 1993                                             */
/*----------------------------------------------------------------------------*/

#define INCL_GPIPRIMITIVES        /*------------------------------------------*/
#define INCL_GPIBITMAPS           /*                                          */
#define INCL_GPILCIDS             /*                                          */
#define INCL_WIN                  /*   Include appropriate sections           */
#define INCL_DOSERRORS            /*                                          */
#define INCL_WINHELP              /*                                          */
#define INCL_DOSPROCESS           /*                                          */
#define INCL_WINSTDSLIDER         /*                                          */
#define INCL_OS2MM                /*   Required for MCI and MMIO headers      */
#define INCL_SECONDARYWINDOW      /*                                          */
#define INCL_GRAPHICBUTTON        /*------------------------------------------*/

#include <os2.h>                  /*-------- OS/2 header files ---------------*/
#include <stdlib.h>               /*------------------------------------------*/
#include <string.h>               /*          c header files                  */
#include <stdio.h>                /*------------------------------------------*/

#include <sw.h>                   /*------------------------------------------*/
                                  /*        MMPM/2 header files               */
#include <os2me.h>                /*------------------------------------------*/
                                  /*------------------------------------------*/
#include "caption.h"              /*            Local header files            */
                                  /*------------------------------------------*/


/*---------------------- Function prototypes ---------------------------------*/

void             main           (void );
MRESULT EXPENTRY MainWindowProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY TextWindowProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY FileFilterProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ProductDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID    FormatFname     (CHAR szOutputName[], CHAR szInputName[] );
ULONG   LoadAudioFile   (HWND hwnd, CHAR szFilename[] );
ULONG   StartPlaying    (HWND hwnd );
VOID    StopPlaying     (HWND hwnd );
VOID    CloseAudioDevice(HWND hwnd );
VOID    ShareAudioDevice(void );
VOID    AudioError      (ULONG ulError );
APIRET  LoadTextFile    (CHAR szFilename[] );
VOID    PositionCaptions(SHORT sArmPos );
VOID    DisplayTextLine (void );
VOID    DisplayDosError (CHAR szTitle[], APIRET apiret );
USHORT  SaveChanges     (ULONG flStyle );
BOOL    CheckForTiming  (void );
APIRET  SaveCaptionFile (CHAR szFilename[], SHORT sMenuItem );
APIRET  WriteLine       (HFILE hfile, PSZ psz );
VOID    ResizeTextWindow(LONG lNewDisplayLines );
VOID    CreateTextBitmap(void );
LONG    DisplaySaveasDlg(SHORT sMenuItem);

/*------------------------ Global Variables ----------------------------------*/

HAB                hab;            /*      Anchor block handle                */

MCI_OPEN_PARMS     mop;            /*-----------------------------------------*/
MCI_GENERIC_PARMS  mgp;            /* Required for MMPM/2 calls.              */
MCI_STATUS_PARMS   msp;            /*                                         */
MCI_SET_PARMS      mspSet;         /*-----------------------------------------*/

                                   /*----------- Window handles --------------*/
                                   /*                                         */
HWND        hwndMainDlg;           /* Control window handle (dialog)          */
HWND        hwndMainFrame;         /* Control window handle (frame)           */
HWND        hwndMenu;              /* Menu bar in secondary frame window      */
HWND        hwndText;              /* Frame window for scrolling text display */
HWND        hwndHelp;              /* Help object window.                     */
HWND        hwndVolume;            /* Volume control circular slider.         */
HWND        hwndAudioSlider;       /* Audio slider in main dialog box.        */
HWND        hwndProductInfo = 0;   /* Handle to the product info. dialog box  */
                                   /*-----------------------------------------*/

RECTL       rclTextClient;         /* Dimensions of scrolling-text client area*/
RECTL       rclMain;               /* Dimensions of main window               */

PVOID       pvText = NULL;         /* Pointer to text file image              */
PVOID       pvLinedataTable;       /* Pointer to array of LINEDATA structures */
USHORT      usLineCount;           /* Number of structures in Line Data table */
USHORT      usDisplayLine;         /* Line displayed at the bottom of the txt */
SHORT       sTotalVolume;          /* Total volume level slider can have      */
SHORT       sCaptionArmRange;      /* Length of the caption slider control    */

int         iState    = ST_CLOSED; /* Initial state of the device             */
CHAR        cEOF      = CHAR_EOF;
ULONG       ulVolume  = 100;       /* Initial volume level                    */

FILEDLG     fdgAudio;
CHAR        szErrorMsgTitle    [TITLE_LENGTH];  /* Title for Error message box*/
CHAR        szAudioFile        [TITLE_LENGTH];  /* Store ".WAV" for open dlg  */
CHAR        szCC_FILE[TITLE_LENGTH];            /* First line of CC files     */
ULONG       ulAudioLength;                      /* Audio length in MM units   */
SHORT       sAudioArmRange;                     /* Slider arm range in pels   */

FILEDLG     fdgText;
CHAR        szTextOpen         [TITLE_LENGTH];  /* "Open Text File"           */
CHAR        szCaptionOpen      [TITLE_LENGTH];  /* "Open Caption File"        */
CHAR        szTextFile      [CCHMAXPATH] = "";  /* Text file full path        */
CHAR        szCaptionFile   [CCHMAXPATH] = "";  /* Caption file full path     */

FILEDLG     fdgSaveAs;
CHAR        szCapFileName      [TITLE_LENGTH];  /* Caption file name          */
CHAR        szTextFileName     [TITLE_LENGTH];  /* Text file name             */
CHAR        szAudioFileName    [TITLE_LENGTH];  /* Audio file name            */
CHAR        szNoneFileName     [TITLE_LENGTH];  /* for "[none]" file name  */

USHORT      usFiledlgHelpRes;    /*   Help resource for latest file dialog    */

LONG        lTitleY;             /* System values which affect the size       */

HPS         hpsText;             /*-------------------------------------------*/
HBITMAP     hbmText;             /*   Handles for text window backup bitmap   */
HDC         hdcText;             /*-------------------------------------------*/

LONG        lScrollOffset = 0;   /*-------------------------------------------*/
USHORT      usNextlineReq = 0;   /*       Variables used in scrolling         */
USHORT      usNextline;          /*-------------------------------------------*/

LONG        lLineSpacing;        /*      Height of each line in pixels        */
LONG        lLastLineY;          /*      Y coordinate of Bottom  line         */
LONG        lDisplayLines;       /*      Number of lines in display           */
LONG        lcyDialog;           /*      Dialog box cy value                  */

HELPINIT    helpinit;
CHAR        szHelpTitle[TITLE_LENGTH];  /* "Caption Creation Utility Help"    */
CHAR        szHelpLibrary[TITLE_LENGTH];/* Help library                       */

SHORT       sLatestPlayRequest;
BOOL        bAcceptPlayRequests = TRUE;

USHORT      usPositionUP = 0;    /* Current valid Position advise serial nmbr */

BOOL        bAudioLoaded    = FALSE;  /* TRUE when audio file specified       */
BOOL        bTextLoaded     = FALSE;  /* TRUE when text file specified        */
BOOL        bCaptionsLoaded = FALSE;  /* TRUE when caption data avail         */
BOOL        bUnsavedChanges = FALSE;  /* TRUE when changes have gone unsaved  */

BOOL        bMainActive     = FALSE;  /* TRUE when the main window is active  */
BOOL        bAcquired       = FALSE;  /* TRUE when we have access to audio dev*/

SWP         swpMain;

HPOINTER    hptrWait, hptrArrow;/* Pointer handles for use during long waits. */


/******************************************************************************
 * Name         : main
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a dialog box as a
 *                main window.  The message queue will be created, as well
 *                the dialog box.  When program is terminated, the code will
 *                destroy the help instance, message queue, and main window.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : mciSendString   MCI_GETDEVCAPS
 *
 * Parameters   : argc - Number of parameters passed into the program.
 *                argv - Command line parameters.
 *
 * Return       : TRUE is returned to the operating system.
 *
 ******************************************************************************/

void main(  )
{
  HMQ     hmq;                        /* Handle to the message queue          */
  QMSG    qmsg;                       /* Struc to hold a msg on the msg queue */
  LONG    lX;                         /* Text window position, x coordinate   */
  LONG    lY;                         /* Text window position, y coordinate   */
  LONG    lWidth;                     /* Text window width                    */
  LONG    lHeight;                    /* Text window height                   */
  ULONG   ulBytesReq;                 /* Memory required for Line data table  */
  CHAR    szError[80];                /* Buffer to hold help creation error   */
  CHAR    szTitle[TITLE_LENGTH];      /* Buffer to hold Default size          */
  BOOL    fHelpCreated = TRUE;        /* flag used for help creation          */
  CHAR    szMainTitle [TITLE_LENGTH]; /* "Caption Creation Utility"           */
  CHAR    szSave      [TITLE_LENGTH]; /* "Save as"                            */
  CHAR    szSaveAsOpen[TITLE_LENGTH]; /* "Save caption file"                  */
  CHAR    szAudioOpen [TITLE_LENGTH]; /* "Open Audio File"                    */
  LONG    lmciSendStringRC;           /* return code from mciSendString       */
/*----------------------- PM Initialization  ---------------------------------*/

  hab = WinInitialize ( 0 );
  hmq = WinCreateMsgQueue ( hab, 50 );

  ulBytesReq = (ULONG)(MAX_TEXT_LINES*sizeof(LINEDATA));/* Allocate memory for*/
  DosAllocMem ( &pvLinedataTable, ulBytesReq, fALLOC );/* the Line Data Table */

  hptrArrow = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE );
  hptrWait  = WinQuerySysPointer ( HWND_DESKTOP, SPTR_WAIT,  FALSE );

/*----------------------------------------------------------------------------*/
/*  Load frequently-used strings from the program's resource file and save    */
/*  them in static memory.  Strings which are seldom used will be loaded when */
/*  needed in to local memory.                                                */
/*----------------------------------------------------------------------------*/

  WinLoadString(hab, 0, STRID_OPEN_AUDIO,         TITLE_LENGTH,szAudioOpen    );
  WinLoadString(hab, 0, STRID_OPEN_TEXT,          TITLE_LENGTH,szTextOpen     );
  WinLoadString(hab, 0, STRID_OPEN_CAPTION,       TITLE_LENGTH,szCaptionOpen  );
  WinLoadString(hab, 0, STRID_SAVEAS_TITLE,       TITLE_LENGTH,szSaveAsOpen   );
  WinLoadString(hab, 0, STRID_SAVEAS_BUTTON,      TITLE_LENGTH,szSave         );
  WinLoadString(hab, 0, STRID_HELP_TITLE,         TITLE_LENGTH,szHelpTitle    );
  WinLoadString(hab, 0, STRID_DEFAULT_MAIN_TITLE, TITLE_LENGTH,szMainTitle    );
  WinLoadString(hab, 0, STRID_HELP_LIBRARY_NAME,  TITLE_LENGTH,szHelpLibrary  );
  WinLoadString(hab, 0, STRID_AUDIOFILE_EXTENTION,TITLE_LENGTH,szAudioFile    );
  WinLoadString(hab, 0, STRID_FIRSTLINE_OF_CCFILE,TITLE_LENGTH,szCC_FILE      );
  WinLoadString(hab, 0, STRID_ERROR_MESSAGE_TITLE,TITLE_LENGTH,szErrorMsgTitle);
  WinLoadString(hab, 0, STRID_CAPTION_FILE_NAME,  TITLE_LENGTH,szCapFileName  );
  WinLoadString(hab, 0, STRID_TEXT_FILE_NAME,     TITLE_LENGTH,szTextFileName );
  WinLoadString(hab, 0, STRID_AUDIO_FILE_NAME,    TITLE_LENGTH,szAudioFileName);
  WinLoadString(hab, 0, STRID_NONE_FILE_NAME,     TITLE_LENGTH,szNoneFileName );

/*----------- Create the backup bitmap for the text window -------------------*/

  lDisplayLines = CC_DEFAULT_LINES;
  CreateTextBitmap();



/*------- Open the main window ( which is a Secondary Window ) ---------------*/


  hwndMainFrame =
      WinLoadSecondaryWindow(
         HWND_DESKTOP,             /* Parent of the Secondary window.         */
         (HWND) NULL,              /* Owner of the Secondary window.          */
         (PFNWP) MainWindowProc,   /* Window procedure of Secondary window.   */
         (HMODULE) NULL,           /* Dialog template is found in EXE resource*/
         ID_MAIN_DLG,              /* Dialog template ID.                     */
         (PVOID) NULL);            /* Creation Parameters for the dialog.     */

  hwndMenu = WinWindowFromID ( hwndMainFrame, FID_MENU );

  WinSetWindowText( hwndMainFrame, szMainTitle );      /* Default window title*/

  WinLoadString( hab, 0, STRID_DEFAULT_SIZE,  TITLE_LENGTH, szTitle );
  WinInsertDefaultSize( hwndMainFrame, szTitle ); /*Add ~Default size to menu */

  hwndMainDlg = WinQuerySecondaryHWND( hwndMainFrame, QS_DIALOG );

/*-------- Find out the waveaudio on the system can play ---------------------*/

  lmciSendStringRC =
      mciSendString( (PSZ)"capability waveaudio can play wait",
                     (PSZ) NULL,
                     CCHMAXPATH,
                     (HWND) 0,
                     0 );

  if (lmciSendStringRC != 0)          /* if can not play disable the Audio... */
  {                                   /* menu item in the Open submenu        */
     WinSendMsg( hwndMenu,
                 MM_SETITEMATTR,
                 MPFROM2SHORT( IDM_OPEN_AUDIO, TRUE ),
                 MPFROM2SHORT( MIA_DISABLED, MIA_DISABLED));
  }

/*-------- Initialize the text window for scrolling captions -----------------*/
  WinRegisterClass(
     hab,
     "TextWindow",
     (PFNWP) TextWindowProc,
     CS_SYNCPAINT | CS_SIZEREDRAW,
     0 );
  rclTextClient.xLeft   = 0;
  rclTextClient.xRight  = CC_WIDTH;
  rclTextClient.yBottom = 0;

  hwndText =
      WinCreateWindow( (HWND)hwndMainDlg, "TextWindow", NULL, 0, 0, 0, 1,
                       1, (HWND)hwndMainDlg, HWND_TOP, 111, NULL, NULL );

  WinQueryWindowRect( hwndMainDlg, &rclMain );

  lTitleY   = WinQuerySysValue ( HWND_DESKTOP, SV_CYTITLEBAR );
  lWidth    = rclMain.xRight - 10;
  lHeight   = ( 2 * lTitleY + rclTextClient.yTop );
  lX        = 5;
  lY        = ( rclMain.yTop - (rclMain.yTop / 5) ) ;

  lcyDialog = lHeight;

  WinSetWindowPos ( hwndText, 0, lX, lY, lWidth, lHeight,
                                     SWP_SHOW | SWP_MOVE | SWP_SIZE );

  WinShowWindow( hwndMainFrame, TRUE );

/*------------- Initialize help for this application -------------------------*/
/*                                                                            */
/*   (1) Create a help instance.                                              */
/*   (2) Associate the help instance with the secondary window                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

  memset ( &helpinit, 0, sizeof(helpinit) );
  helpinit.cb                 = sizeof(helpinit);
  helpinit.fShowPanelId       = CMIC_HIDE_PANEL_ID;
  helpinit.fShowPanelId       = (HMODULE) 0;
  helpinit.pszHelpWindowTitle = (PSZ) szHelpTitle;
  helpinit.pszHelpLibraryName = (PSZ) szHelpLibrary;
  helpinit.phtHelpTable       = (PHELPTABLE) ( HELPTABLE_ID | 0xffff0000 );
  helpinit.phtHelpTable       = (PVOID) ( HELPTABLE_ID | 0xffff0000 );

  hwndHelp =
      WinCreateHelpInstance( hab, &helpinit ); /* (1) Create help instance    */

  if ( ! hwndHelp )
     fHelpCreated = FALSE;
  else
  if (! WinAssociateHelpInstance( hwndHelp, hwndMainFrame )) /*(2) Assoc. help*/
     fHelpCreated = FALSE;

  if ( ! fHelpCreated )
  {
     WinLoadString ( hab, (HMODULE)NULL, STRID_HELP_CREATION_FAILED,
                                                   ERROR_LENGTH, szError );
     WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError, szErrorMsgTitle,
                                       0, MB_OK | MB_ERROR | MB_MOVEABLE );
  }


/*----------- Initialize the File Dialog data structures ---------------------*/

   memset ( &fdgAudio, 0, sizeof(fdgAudio) );      /*-------------------------*/
   fdgAudio.cbSize   = sizeof(fdgAudio);           /*                         */
   fdgAudio.fl = FDS_HELPBUTTON | FDS_CENTER       /*                         */
                              | FDS_OPEN_DIALOG;   /*   Audio file            */
   fdgAudio.pszTitle = (PSZ) szAudioOpen;          /*                         */
   fdgAudio.pfnDlgProc = (PFNWP) FileFilterProc;   /*                         */
                                                   /*-------------------------*/

   memset ( &fdgText, 0, sizeof(fdgText) );        /*-------------------------*/
   fdgText.cbSize   = sizeof(fdgText);             /*                         */
   fdgText.fl = FDS_HELPBUTTON | FDS_CENTER        /*                         */
                              | FDS_OPEN_DIALOG;   /*    Text file            */
   fdgText.pfnDlgProc = (PFNWP) FileFilterProc;    /*                         */
                                                   /*                         */
                                                   /*-------------------------*/

   memset ( &fdgSaveAs, 0, sizeof(fdgSaveAs) );    /*-------------------------*/
   fdgSaveAs.cbSize   = sizeof(fdgSaveAs);         /*                         */
   fdgSaveAs.fl = FDS_HELPBUTTON | FDS_CENTER      /*                         */
                              | FDS_OPEN_DIALOG;   /*   Saveas file           */
   fdgSaveAs.pszTitle = (PSZ) szSaveAsOpen;        /*                         */
   fdgSaveAs.pfnDlgProc = (PFNWP) FileFilterProc;  /*                         */
   fdgSaveAs.pszOKButton = (PSZ) szSave;           /*-------------------------*/


/*----------- Initialize the Open parameter data structure--------------------*/

   mop.usDeviceID = (USHORT) NULL;

/*------------------------- Message Loop  ------------------------------------*/

  while( WinGetMsg( hab, &qmsg, 0, 0, 0 ) )
    WinDispatchMsg( hab, &qmsg );


/*---------------------------- Wrapup ----------------------------------------*/

  WinDestroyHelpInstance   ( hwndHelp );     /*-------------------------------*/
  WinDestroySecondaryWindow( hwndMainFrame );/*Destroy windows we have created*/
  WinDestroyWindow         ( hwndText );     /*-------------------------------*/

  GpiDestroyPS             ( hpsText );   /*----------------------------------*/
  DevCloseDC               ( hdcText );   /*Get rid of text wndw backup bitmap*/
  GpiDeleteBitmap          ( hbmText );   /*----------------------------------*/

  DosFreeMem               ( pvLinedataTable);  /* Free Line Data Tbl memory  */

  WinDestroyMsgQueue       ( hmq );             /* Destroy msg queue          */
  WinTerminate             ( hab );
}

/******************************************************************************
 * Name         : MainWindowProc
 *
 * Description  : This function controls the main dialog box.  It will handle
 *                received messages such as pushbutton notifications, timing
 *                events, etc.
 *
 * Concepts     : - How to participate in MMPM/2 device sharing scheme.
 *                - How to handle notification messages.
 *
 * MMPM/2 API's : mciSendCommand
 *                   MCI_STATUS
 *                   MCI_PAUSE
 *                   MCI_RESUME
 *                   MCI_SET
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY MainWindowProc( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2 )
{
  ULONG         ulError;                /* for return value                   */
  USHORT        usNotifyCode;           /* for notification code              */
  USHORT        usUserParm;             /* for user parameter                 */
  USHORT        usCommandMessage;       /* for command message                */
  APIRET        apiret;                 /* for api return code                */
  PLINEDATA     pld;                    /* Local pointer to the Line Data Tb  */
  ULONG         ulTime;                 /* Media position in mm units         */
  PSZ           psz, pszSlash;          /* pointers for temporary storage     */
  PSZ           pszFileName;            /* for file name                      */
  USHORT        usTestLine;             /* for text line in the text window   */
  PSWP          pswp;
  SHORT         sArmPosition;           /* Updated slider arm position        */
  SHORT         sControlID;             /* Control ID for WM_CONTROL messages */
  SHORT         sCommandID;             /* Command ID for WM_COMMAND messages */
  PCHAR         pcTextFile;             /* buffer for window title text       */
  CHAR          szBuffer[CCHMAXPATH];   /* temporary buffer to store file name*/
  MRESULT       mresult;                /* for return result                  */

/*----------------------------------------------------------------------------*/

  switch( msg )
  {
     /*---------------- Dialog initialization --------------------------------*/
     /*                                                                       */
     /* (1) Determine a number of window handles for dialog controls.  This   */
     /*     will save time ( and code lines ) later.                          */
     /* (2) Initialize the text in the files group box                        */
     /* (3) Set the scale and position of the Volume control.                 */
     /* (4) Set the animation rate for the Play and Start Timing pushbuttons. */
     /*-----------------------------------------------------------------------*/

     case WM_INITDLG:
        hwndAudioSlider = WinWindowFromID( hwnd, ID_AUDIO_SLIDER ); /* (1)    */
        hwndVolume      = WinWindowFromID( hwnd, ID_VOLUME       );

        strcpy(szBuffer, szTextFileName);                             /* (2)  */
        strcat(szBuffer, szNoneFileName);
        WinSetWindowText( WinWindowFromID( hwnd, ID_TEXTTXT), szBuffer );

        strcpy(szBuffer, szAudioFileName);
        strcat(szBuffer, szNoneFileName);
        WinSetWindowText( WinWindowFromID( hwnd, ID_AUDIOTXT), szBuffer );

        strcpy(szBuffer, szCapFileName);
        strcat(szBuffer, szNoneFileName);
        WinSetWindowText( WinWindowFromID( hwnd, ID_CAPTIONTXT), szBuffer );

        WinSendMsg( hwndVolume, SLM_SETTICKSIZE,                      /* (3)  */
                    (MPARAM)MPFROM2SHORT (0, 3),
                    (MPARAM)0 );

        WinSendMsg( hwndVolume, SLM_SETSCALETEXT,
                    (MPARAM)0, (MPARAM) "0%" );

        WinSendMsg( hwndVolume, SLM_SETTICKSIZE,
                    (MPARAM)MPFROM2SHORT (25, 3), (MPARAM)0 );

        WinSendMsg( hwndVolume, SLM_SETTICKSIZE,
                    (MPARAM)MPFROM2SHORT (50, 3), (MPARAM)0 );

        WinSendMsg( hwndVolume, SLM_SETSCALETEXT,
                    (MPARAM)50, (MPARAM) "50%" );

        WinSendMsg( hwndVolume, SLM_SETTICKSIZE,
                    (MPARAM)MPFROM2SHORT (75, 3), (MPARAM)0 );

        WinSendMsg( hwndVolume, SLM_SETTICKSIZE,
                    (MPARAM)MPFROM2SHORT (99, 3), (MPARAM)0 );

        WinSendMsg( hwndVolume, SLM_SETSCALETEXT,
                    (MPARAM)99, (MPARAM) "100%" );

        mresult =
          WinSendMsg( hwndVolume,
                      SLM_QUERYSLIDERINFO,
                      MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                      0 );
        sTotalVolume = SHORT2FROMMP( (MPARAM) mresult );

        WinSendMsg(
              hwndVolume,
              SLM_SETSLIDERINFO,
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
              MPFROMSHORT( sTotalVolume - 1 ));

        WinSendDlgItemMsg( hwnd, ID_SET_TIMING, GBM_SETANIMATIONRATE,  /* (4) */
                                                MPFROMLONG(100), 0 );
        WinSendDlgItemMsg( hwnd, ID_PLAY, GBM_SETANIMATIONRATE,
                                          MPFROMLONG(100), 0 );

        break;

     /*---------------------- Activate message -------------------------------*/
     /*                                                                       */
     /*   Keep up with the active status of the Main window.                  */
     /*   Whenever the main window is activated, we must:                     */
     /*                                                                       */
     /*  (1) See if we need to regain control of the audio device.            */
     /*                                                                       */
     /*-----------------------------------------------------------------------*/

     case WM_ACTIVATE:
        if ( SHORT1FROMMP ( mp1 ) )
        {
           bMainActive = TRUE;
           ShareAudioDevice();
        }
        else
           bMainActive = FALSE;

        break;

     case WM_COMMAND:
        sCommandID = SHORT1FROMMP(mp1);

        /*--------------------------------------------------------------------*/
        /*  The accelerator table will give us WM_COMMAND messages for        */
        /*  pushbuttons, even if the buttons are disabled.  If this message   */
        /*  originated from the accelerator table, make sure it is NOT coming */
        /*  from a disabled pushbutton.                                       */
        /*--------------------------------------------------------------------*/

        if ( ( SHORT1FROMMP(mp2) == CMDSRC_ACCELERATOR ) &&
             ( ( sCommandID == ID_SET_TIMING ) ||
               ( sCommandID == ID_PLAY       ) ||
               ( sCommandID == ID_NEXTLINE   ) ||
               ( sCommandID == ID_REWIND     ) ||
               ( sCommandID == ID_PAUSE      ) ||
               ( sCommandID == ID_STOP       ) ) )
           if ( ! WinIsWindowEnabled(
                     WinWindowFromID( hwnd, (ULONG) sCommandID )))
           {
              WinAlarm( HWND_DESKTOP, WA_WARNING );/* Disabled pushbutton give*/
              break;                               /* a warning beep and exit.*/
           }

        switch ( sCommandID )
        {
           /*---------------- Start Timing pushbutton ------------------------*/
           /*                                                                 */
           /* (1) Check to see if there is already a Play or Start Timing     */
           /*     operation in progress.  If so, chastise the user and exit.  */
           /*                                                                 */
           /* (2) Set switches to indicate a Start Timing operation is in     */
           /*     progress and initialize queued line count to zero.          */
           /*                                                                 */
           /* (3) Start playing the audio.                                    */
           /*-----------------------------------------------------------------*/

           case ID_SET_TIMING:
              if ( ! bAcceptPlayRequests )              /* (1) Alredy playing?*/
                 WinAlarm ( HWND_DESKTOP, WA_WARNING );
              else
              {
                 bAcceptPlayRequests = FALSE;        /* (2) Timing switch set */
                 sLatestPlayRequest  = ID_SET_TIMING;
                 usNextlineReq = 0;
                 WinSetPointer ( HWND_DESKTOP, hptrWait );
                 ulError = StartPlaying( hwnd );     /* (3) Start playing     */
                 WinSetPointer ( HWND_DESKTOP, hptrArrow );
                 if ( ulError ) AudioError ( ulError );
              }
              break;


           /*------------------- Play pushbutton -----------------------------*/
           /*                                                                 */
           /* (1) Check to see if there is already a Play or Start Timing n   */
           /*     operation in progress.  If so, chastise the user and exit.  */
           /*                                                                 */
           /* (2) Set switches to indicate a Play operation is in progress    */
           /*     and initialize queued line count to zero.                   */
           /*                                                                 */
           /* (3) Start playing the audio.                                    */
           /*-----------------------------------------------------------------*/

           case ID_PLAY:
              if ( ! bAcceptPlayRequests )              /* (1) Alredy playing?*/
                 WinAlarm ( HWND_DESKTOP, WA_WARNING );
              else
              {
                 bAcceptPlayRequests = FALSE;        /* (2) Play switch setup */
                 sLatestPlayRequest = ID_PLAY;
                 usNextlineReq = 0;
                 WinSetPointer ( HWND_DESKTOP, hptrWait );
                 ulError = StartPlaying( hwnd );     /* (3) Start playing     */
                 WinSetPointer ( HWND_DESKTOP, hptrArrow );
                 if ( ulError ) AudioError ( ulError );
              }
              break;


           /*--------------- Advance Line pushbutton -------------------------*/
           /*                                                                 */
           /* (1) Check variable usNextline for the index of the line to be   */
           /*     associated with this button push.                           */
           /*                                                                 */
           /* (2) If we have not reached the end of the text, then ask for the*/
           /*     current audio position ( in MM units ) and place it in the  */
           /*     LINEDATA entry for this text line.                          */
           /*                                                                 */
           /* (3) Next we check variable usNextlineReq. This gives us the     */
           /*     number of lines queued up to be scrolled into the text wnd. */
           /*                                                                 */
           /* (4) If this is the only line waiting, highlight the Advance Line*/
           /*     control and post a message to the text window, directing it */
           /*     to start a scroll operation; but if there are other lines   */
           /*     queued up to be scrolled, exit after incrementing           */
           /*     usNextlineReq.                                              */
           /*-----------------------------------------------------------------*/

           case ID_NEXTLINE:
              if ( usNextline < usLineCount )      /* (1) Check usNextline    */
              {
                 msp.hwndCallback = (HWND) NULL;    /* (2) Get audio position..*/
                 msp.ulItem     = MCI_STATUS_POSITION;
                 ulError = mciSendCommand ( mop.usDeviceID,
                                            MCI_STATUS,
                                            MCI_WAIT | MCI_STATUS_ITEM,
                                            (PVOID) &msp,
                                            (USHORT)  UP_STATUS );
                 if ( !ulError )
                    AudioError ( ulError );
                 else
                 {
                    pld = (PLINEDATA) pvLinedataTable + usNextline;
                    pld->ulTime = msp.ulReturn;
                    bUnsavedChanges = TRUE;        /*....and mark in LINEDATA */
                    usNextline++;
                 }
                 usNextlineReq++;
                 if ( usNextlineReq == 1 )      /* (3) Check for queued lines */
                 {
                    WinSendDlgItemMsg(                        /*--------------*/
                           hwnd, ID_NEXTLINE,                 /*              */
                           GBM_SETBITMAPINDEX,                /* (4) Hilite   */
                           MPFROMSHORT(GB_CURRENTSTATE),      /* adv line and */
                           MPFROMSHORT(1) );                  /* post message */
                    WinPostMsg( hwndText, UM_NEXTLINE, 0, 0 );/*--------------*/
                 }
              }
              break;

           case ID_PAUSE:
              switch ( iState )
              {
                 /*------------ Pause the current operation ------------------*/
                 /*                                                           */
                 /* (1) Pause the audio play.                                 */
                 /* (2) Update internal status.                               */
                 /* (3) Highlight the PAUSE graphic pushbutton.               */
                 /* (4) Enable the HELP submenu.                              */
                 /* (5) Freeze the animated graphic pushbutton for the latest */
                 /*     operation ( either Start timing or Play ).            */
                 /* (6) If the latest operation was Start timing, disable the */
                 /*     Advance Line pushbutton.                              */
                 /*-----------------------------------------------------------*/

                 case ST_PLAYING:
                    mgp.hwndCallback = (HWND) NULL;    /*---------------------*/
                    mciSendCommand ( mop.usDeviceID,   /*  (1) Pause audio    */
                                     MCI_PAUSE,        /*---------------------*/
                                     MCI_WAIT,
                                     (PVOID) &mgp,
                                     (USHORT) UP_PAUSE );

                    iState = ST_PAUSED;                  /* (2) Update state  */

                    WinSendDlgItemMsg ( hwnd, ID_PAUSE,  /* (3) Highlite pause*/
                                        GBM_SETBITMAPINDEX,
                                        MPFROMSHORT(GB_CURRENTSTATE),
                                        MPFROMSHORT(1) );

                    if (sLatestPlayRequest==ID_SET_TIMING)/*(4) Freeze anim.  */
                    {                                     /*(5) Dsabl Adv Line*/
                       WinEnableWindow ( WinWindowFromID( hwnd, ID_NEXTLINE ),
                                         FALSE );
                       WinSendDlgItemMsg ( hwndMainDlg, ID_SET_TIMING,
                                           GBM_ANIMATE,
                                           MPFROMSHORT((SHORT)FALSE),
                                           MPFROMSHORT((SHORT)FALSE) );
                    }
                    else
                       WinSendDlgItemMsg ( hwndMainDlg, ID_PLAY, GBM_ANIMATE,
                                           MPFROMSHORT((SHORT)FALSE),
                                           MPFROMSHORT((SHORT)FALSE) );
                    break;


                 /*-------------- Resume the current operation ---------------*/
                 /*                                                           */
                 /* (1) Resume the audio play.                                */
                 /* (2) Update internal status.                               */
                 /* (3) Unhighlight the PAUSE graphic pushbutton.             */
                 /* (4) Disable the HELP submenu.                             */
                 /* (5) Resume animation of the graphic pushbutton for the    */
                 /*     latest operation ( either Start timing or Play ).     */
                 /* (6) If the latest operation was Start timing, enable the  */
                 /*     Advance Line pushbutton.                              */
                 /*-----------------------------------------------------------*/

                 case ST_PAUSED:
                    mgp.hwndCallback = (HWND) NULL;  /*-----------------------*/
                    mciSendCommand ( mop.usDeviceID, /*  (1) Resume audio     */
                                     MCI_RESUME,     /*-----------------------*/
                                     MCI_WAIT,
                                     (PVOID) &mgp,
                                     (USHORT) UP_RESUME );

                    iState = ST_PLAYING;                 /* (2) Update state  */

                    WinSendDlgItemMsg ( hwnd, ID_PAUSE,  /* (3) Unhilite pause*/
                                        GBM_SETBITMAPINDEX,
                                        MPFROMSHORT(GB_CURRENTSTATE),
                                        MPFROMSHORT(0) );

                    if (sLatestPlayRequest==ID_SET_TIMING)/*(4) Animate       */
                    {                                     /*(5) Enabl Adv Line*/
                       WinEnableWindow ( WinWindowFromID ( hwnd, ID_NEXTLINE ),
                                         TRUE );
                       WinSendDlgItemMsg ( hwndMainDlg, ID_SET_TIMING,
                                           GBM_ANIMATE,
                                           MPFROMSHORT((SHORT)TRUE),
                                           MPFROMSHORT((SHORT)FALSE) );
                    }
                    else
                       WinSendDlgItemMsg ( hwndMainDlg, ID_PLAY, GBM_ANIMATE,
                                           MPFROMSHORT((SHORT)TRUE),
                                           MPFROMSHORT((SHORT)FALSE) );
                    break;
              }
              break;
                                                     /*-----------------------*/
                                                     /*        Stop           */
           case ID_STOP:                             /*-----------------------*/
              StopPlaying( hwnd );
              break;                                 /*-----------------------*/
                                                     /*        Rewind         */
                                                     /*-----------------------*/
           case ID_REWIND:
              StopPlaying( hwnd );
              WinSendMsg(hwndAudioSlider, SLM_SETSLIDERINFO,
                         MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                         MPFROMSHORT( 0 ) );
              PositionCaptions ( 0 );
              break;


           /*---------------- Open Audio... menu item ------------------------*/
           /*                                                                 */
           /*  (1) Set the appropriate help resource for this operation.      */
           /*  (2) Reformat the latest audio file name as:  x:\pathname\*.ext */
           /*  (3) Request the user to select a file                          */
           /*  (4) If the user selected a file, try to open it.               */
           /*-----------------------------------------------------------------*/

           case IDM_OPEN_AUDIO:
              usFiledlgHelpRes = PANEL_FILEDLG_AUDIO;     /*(1) Help resource */

              FormatFname( fdgAudio.szFullFile, szAudioFile );/*(2)Format name*/

              if (WinFileDlg(HWND_DESKTOP, hwnd, &fdgAudio))/*(3) Request file*/
                 if ( fdgAudio.lReturn == DID_OK )
                 {
                    if (iState == ST_PLAYING ) StopPlaying( hwnd );
                    WinSetPointer(HWND_DESKTOP, hptrWait);/*(4) Try opening it*/
                    ulError = LoadAudioFile ( hwnd, fdgAudio.szFullFile );
                    WinSetPointer ( HWND_DESKTOP, hptrArrow );
                    if ( ulError) AudioError ( ulError );
                 }
              break;


           /*----------------------- Open Text... menu item ------------------*/
           /*                                                                 */
           /* (1) Prompt user to save unsaved changes ( if any ).             */
           /* (2) Set the appropriate help resource for this operation.       */
           /* (3) Reformat the latest audio file name as:  x:\pathname\*.ext  */
           /* (4) Request the user to select a file                           */
           /* (5) If the user selected a file, try to open it.                */
           /*-----------------------------------------------------------------*/

           case IDM_OPEN_TEXT:
              if ( bUnsavedChanges )                     /* (1) Usaved changes*/
                 if ( SaveChanges( MB_YESNOCANCEL ) == MBID_CANCEL )
                    break;

              usFiledlgHelpRes = PANEL_FILEDLG_TEXT;     /* (2) Help resource */

              if ( szTextFile[0] )                       /* (3) Format filenam*/
                 FormatFname ( fdgText.szFullFile, szTextFile );
              else
                 strcpy ( fdgText.szFullFile, "*.TXT" );
              fdgText.pszTitle = szTextOpen;             /* Open dialog title */

              if (WinFileDlg(HWND_DESKTOP, hwnd, &fdgText))/*(4) Ask for name */
                 if ( fdgText.lReturn == DID_OK )          /* (5) Try loading */
                 {
                    if ( iState == ST_PLAYING ) StopPlaying( hwnd );
                    apiret = LoadTextFile( fdgText.szFullFile );
                    if ( apiret )
                       DisplayDosError ( szErrorMsgTitle, apiret );
                    else
                    {
                       strcpy(szBuffer, szCapFileName);
                       strcat(szBuffer, szNoneFileName);
                       WinSetWindowText( WinWindowFromID( hwnd, ID_CAPTIONTXT),
                                         szBuffer );

                       pcTextFile = strrchr( fdgText.szFullFile, '\\' );
                       pcTextFile++;
                       strcpy(szBuffer, szTextFileName);
                       strcat(szBuffer, pcTextFile);
                       WinSetWindowText( WinWindowFromID( hwnd, ID_TEXTTXT),
                                         szBuffer );
                    }
                 }
              break;


           /*------------ Open Captions... menu item -------------------------*/
           /*                                                                 */
           /* (1) Prompt user to save unsaved changes ( if any ).             */
           /* (2) Set the appropriate help resource for this operation.       */
           /* (3) Reformat the latest caption file name as:  x:\pathname\*.ext*/
           /* (4) Request the user to select a file                           */
           /* (5) If the user selected a file, try to open it.                */
           /*-----------------------------------------------------------------*/

           case IDM_OPEN_CAPTIONS:
              if ( bUnsavedChanges )                     /* (1) Unsaved chngs */
                 if ( SaveChanges( MB_YESNOCANCEL ) == MBID_CANCEL ) break;

              usFiledlgHelpRes = PANEL_FILEDLG_TEXT;     /* (2) Help resource */

              if ( szCaptionFile[0] )                    /* (3) Format filenam*/
                 FormatFname ( fdgText.szFullFile, szCaptionFile );
              else
                 strcpy ( fdgText.szFullFile, "*._CC" );
              fdgText.pszTitle = szCaptionOpen;          /* Open dialog title */

              if (WinFileDlg(HWND_DESKTOP, hwnd, &fdgText))/*(4) Ask for name */
                 if ( fdgText.lReturn == DID_OK )          /* (5) Try loading */
                 {
                    if ( iState == ST_PLAYING ) StopPlaying( hwnd );
                    apiret = LoadTextFile( fdgText.szFullFile );
                    if ( apiret )
                       DisplayDosError ( szErrorMsgTitle, apiret );
                    else
                    {
                       strcpy(szBuffer, szTextFileName);
                       strcat(szBuffer, szNoneFileName);
                       WinSetWindowText( WinWindowFromID( hwnd, ID_TEXTTXT),
                                         szBuffer );

                       pcTextFile = strrchr( fdgText.szFullFile, '\\' );
                       pcTextFile++;
                       strcpy(szBuffer, szCapFileName);
                       strcat(szBuffer, pcTextFile);
                       WinSetWindowText( WinWindowFromID( hwnd, ID_CAPTIONTXT),
                                         szBuffer );
                    }
                 }
              break;


           /*------------------ Save menu item -------------------------------*/
           /*                                                                 */
           /* (1) Check for any untimed lines. If any are found, ask the user */
           /*     if he is sure he wants to save.  Exit if user answers No.   */
           /* (2) If we successfully got through step one, save the file.     */
           /*---------------------------------------------------------------- */

           case IDM_SAVE:
              if ( CheckForTiming() )
                 SaveCaptionFile ( szCaptionFile, IDM_SAVE );
              break;


           /*--------------- Save as... menu item ---------------------------*/
           /*                                                                */
           /* (1) Check for any untimed lines. If any are found, ask the user*/
           /*     if he is sure he wants to continue. Exit if user answers NO*/
           /*                                                                */
           /* (2) Format a file name to prompt the user.  If there is a      */
           /*     caption file name (in szCaptionFile), use that name as the */
           /*     basis for the prompt.  If there is a text file name, use   */
           /*     that name as the basis.  The prompt will have the format   */
           /*     of:  x:\pathname\*._CC                                     */
           /*                                                                */
           /* (3) Set up the appropriate Help Resource for the file dialog   */
           /*     box.                                                       */
           /*                                                                */
           /* (4) Display the Save As dialog box to the user.                */
           /*----------------------------------------------------------------*/

           case IDM_SAVEAS:
              if ( ! CheckForTiming() ) break; /* (1) Check for untimed lines */

              if ( szCaptionFile [0] )         /* (2) Format prompt file name */
                 FormatFname ( fdgSaveAs.szFullFile, szCaptionFile );
              else
              {
                 strcpy ( fdgSaveAs.szFullFile, szTextFile );
                 psz  = (PSZ) strrchr ( fdgSaveAs.szFullFile, '.' );
                 pszSlash = (PSZ) strrchr ( fdgSaveAs.szFullFile, '\\' );
                 if ( pszSlash < psz )  strcpy  ( (char *)psz, "._CC" );
              }

              usFiledlgHelpRes = PANEL_FILEDLG_SAVEAS;/*(3)Setup Help Resource*/

              DisplaySaveasDlg(IDM_SAVEAS);                     /* (4) */
              break;


           /*------------------- Exit menu item ------------------------------*/
           /*                                                                 */
           /* (1) Check for any unsaved changes. If any exist, prompt the user*/
           /*     to save them or cancel the Exit request.                    */
           /*                                                                 */
           /* (2) If step 1 went OK, then post a WM_CLOSE message to yourself.*/
           /*-----------------------------------------------------------------*/

           case IDM_EXITPROG:
              if ( bUnsavedChanges )
                 if ( SaveChanges( MB_YESNOCANCEL ) == MBID_CANCEL ) break;

              bUnsavedChanges = FALSE;
              WinPostMsg( hwnd, WM_CLOSE, 0L, 0L );
              break;


           /*------------------- Help menu items -----------------------------*/
           /*                                                                 */
           /* For each of the Help items, send a message to the help instance */
           /* object window to display the information the user has requested.*/
           /*-----------------------------------------------------------------*/

           case IDM_GENERAL_HELP:
              WinSendMsg ( hwndHelp, HM_DISPLAY_HELP,
                       MPFROMSHORT( PANEL_OVERVIEW ),
                       MPFROMSHORT( HM_RESOURCEID ) );
              break;


           case IDM_KEYS_HELP:
              WinSendMsg ( hwndHelp, HM_DISPLAY_HELP,
                       MPFROMSHORT( PANEL_KEYS_HELP ),
                       MPFROMSHORT( HM_RESOURCEID ) );
              break;


           case IDM_USING_HELP:
              WinSendMsg ( hwndHelp, HM_DISPLAY_HELP, 0, 0 );
              break;


           case IDM_HELP_INDEX:
              WinSendMsg ( hwndHelp, HM_HELP_INDEX, 0, 0 );
              break;

           case IDM_PRODUCT_INFO:
              /*
               * See if the product information dialog box was created.
               */
              if (!hwndProductInfo)
              {
                 /*
                  * Product Window was not created, create and display the
                  * window.
                  */
                 hwndProductInfo =
                     WinLoadSecondaryWindow(
                        HWND_DESKTOP,              /* Parent of the dialog box*/
                        hwndMainFrame,             /* Owner of the dialog box.*/
                        (PFNWP)ProductDlgProc,     /* Dialog box procedure.   */
                        (HMODULE) NULL,            /* Dialog is where,EXE file*/
                        ID_DLG_PRODUCTINFO,        /* Dialog ID.              */
                        (PVOID) NULL);             /* Dialog Creation Params  */
              }
              else
              {
                 /*
                  * Product window was created earlier, reposition the window
                  * and give the focus.
                  */
                 WinSetWindowPos(
                    hwndProductInfo,
                    HWND_TOP,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    SWP_RESTORE | SWP_SHOW | SWP_ACTIVATE );

                 WinSetFocus( HWND_DESKTOP, hwndProductInfo );
              }
              break;


           /*------------- Caption lines... menu items -----------------------*/
           /*                                                                 */
           /* Since these menu items do essentially the same thing, they all  */
           /* invoke the same function ( ResizeTextWindow() ).  For details,  */
           /* see the ResizeTextWindow() function.                            */
           /*-----------------------------------------------------------------*/

           case IDM_2_LINES:
              ResizeTextWindow(2);
              break;

           case IDM_3_LINES:
              ResizeTextWindow(3);
              break;

           case IDM_4_LINES:
              ResizeTextWindow(4);
              break;

           case IDM_5_LINES:
              ResizeTextWindow(5);
              break;

           case IDM_6_LINES:
              ResizeTextWindow(6);
              break;
           }
        return 0;

     case WM_CONTROL:
        sControlID  = SHORT1FROMMP(mp1);
        usNotifyCode = (USHORT) SHORT2FROMMP(mp1);

        switch ( sControlID )
        {
           /*-------------------- Volume control -----------------------------*/
           /*                                                                 */
           /*  (1) Every time the volume control setting is changed, save the */
           /*      the value in a global variable:  ulVolume.                 */
           /*                                                                 */
           /*  (2) send an MCI_SET message to adjust the volume level         */
           /*-----------------------------------------------------------------*/

           case ID_VOLUME:
              if ( ( usNotifyCode == SLN_CHANGE ) ||
                   ( usNotifyCode == SLN_SLIDERTRACK ) )
              {

                 mresult = WinSendMsg( hwndVolume,
                                       SLM_QUERYSLIDERINFO,
                                       (MPARAM)MPFROM2SHORT( SMA_SLIDERARMPOSITION,
                                                             SMA_RANGEVALUE),
                                       0 );
                 ulVolume = (ULONG) SHORT1FROMMP( (MPARAM)mresult );/*(1)  */

                 mspSet.hwndCallback = (HWND) NULL;
                 mspSet.ulAudio    = MCI_SET_AUDIO_ALL;
                 mspSet.ulLevel    = (ULONG) ulVolume;
                 mciSendCommand ( mop.usDeviceID,      /*(2) Adj volume */
                                  MCI_SET,
                                  MCI_WAIT | MCI_SET_AUDIO | MCI_SET_VOLUME,
                                  (PVOID) &mspSet,
                                  (USHORT) UP_SET );
              }
              break;


           /*----------------- Audio position slider -------------------------*/
           /*                                                                 */
           /* (1) As soon as the user drags and/or changes the audio position */
           /*     slider, we must ignore further MM_MCIPOSITIONCHANGE messages*/
           /*     coming from the audio play.  We don't want the user and     */
           /*     MMPM/2 getting into a tug-of-war over the slider position.  */
           /*     We ignore further MM_MCIPOSITIONCHANGE messages by changing */
           /*     the value of usPositionUP. Every time an MM_MCIPOSITIONCHANGE*/
           /*     message is received, its user parameter is compared with    */
           /*     usPositionUP. If they don't match, the message is ignored.  */
           /*     By changing usPositionUP, we will cause the program to ignore*/
           /*     MM_MCIPOSITIONCHANGE messages until the next play is started*/
           /*                                                                 */
           /* (2) Now we need to determine if the SLN_CHANGE message came from*/
           /*     the user or from the program.  To do this, we manipulate the*/
           /*     window focus.  If the slider window has the focus, that     */
           /*     implies that the user updated the control.                  */
           /*                                                                 */
           /* (3) Set the focus to another window so we can continue to       */
           /*     distinguish between user and program updates.               */
           /*                                                                 */
           /* (4) What we do next depends on the play status of the audio:    */
           /*                                                                 */
           /*     (4a) If the audio is stopped, reposition the captions to    */
           /*          correspond to the new audio position.                  */
           /*                                                                 */
           /*     (4b) If the audio is paused, stop the audio and then        */
           /*          reposition the captions to correspond to the new audio */
           /*          position.                                              */
           /*                                                                 */
           /*     (4c) If the audio is playing, reposition the captions, stop */
           /*          the audio and then restart it in the new position.     */
           /*-----------------------------------------------------------------*/

           case ID_AUDIO_SLIDER:
              if ( usNotifyCode == SLN_SLIDERTRACK )
              {
                 usPositionUP++;
                 if ( usPositionUP > 32766 )/*(1)Ignore further position change*/
                    usPositionUP = 0;      /*   messages coming from MMPM/2   */
                 break;
              }

              if ( ( usNotifyCode != SLN_CHANGE ) ||
                   ( WinQueryFocus ( HWND_DESKTOP ) != hwndAudioSlider ) )
                 break;                    /*(2)Ignore SLN_CHANGE messages not*/
                                           /*   originated by the user.       */

              WinSetFocus(HWND_DESKTOP,hwndVolume);/*(3)Change focus so we can*/
                                                   /* continue to use test(2) */
              usPositionUP++;
              if ( usPositionUP > 32766 )/*Repeat step (1) for SLN_CHANGE msgs */
                 usPositionUP = 0;

              sArmPosition = SHORT1FROMMP(mp2);

              switch ( iState )
              {                           /* (4) Depending on audio state...  */
                 case ST_OPEN:
                    PositionCaptions ( sArmPosition );
                    break;               /*(4a)Reposition captions and exit or*/

                 case ST_PAUSED:
                    StopPlaying( hwnd ); /* (4b) Stop, then reposition or...  */
                    PositionCaptions ( sArmPosition );
                    break;

                 case ST_PLAYING:
                    PositionCaptions ( sArmPosition );
                    iState = ST_SEEKING;
                    StopPlaying( hwnd ); /*(4c)Reposition captions, stop curr */
                    StartPlaying( hwnd );/*    play and restart at new pos'n  */
                    break;
              }
              break;
        }
        break;

     /*---------------- MM_MCIPOSITIONCHANGE message -------------------------*/
     /*                                                                       */
     /* (1) Validate the message by comparing the user parameter with the     */
     /*     variable usPositionUP. This prevents queued messages from previous*/
     /*     plays from affecting the Audio Position slider.                   */
     /*                                                                       */
     /* (2) Update the position if the audio slider, if we are still playing. */
     /*                                                                       */
     /* (3) If we are in Play mode, scan through the line data table to see if*/
     /*     we need to scroll more lines into the text window.  The index of  */
     /*     the NEXT line to be displayed can be calculated as follows        */
     /*                                                                       */
     /*            Line currently at bottom of window:    usDisplayLine +     */
     /*            Lines queued to be displayed:          usNextlineReq +     */
     /*            The line after that one:               1                   */
     /*-----------------------------------------------------------------------*/

     case MM_MCIPOSITIONCHANGE:
        if ((USHORT) SHORT1FROMMP(mp1) != usPositionUP) /* (1) Validate message */
           break;

        ulTime = (ULONG) LONGFROMMP(mp2);

        if ( iState == ST_PLAYING )                 /* (2) Adjust audio slider*/
        {
           sArmPosition =
               (SHORT) ( ( ulTime * ( sAudioArmRange - 1) ) / ulAudioLength );
           WinSendMsg(
              hwndAudioSlider,
              SLM_SETSLIDERINFO,
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
              MPFROMSHORT( sArmPosition ));
        }

        if ( sLatestPlayRequest == ID_PLAY )       /* (3) Time for new line?  */
        {
           usTestLine = (USHORT) ( usDisplayLine + usNextlineReq + 1 );
           pld = (PLINEDATA) pvLinedataTable + usTestLine;
           while ( ( usTestLine < usLineCount ) && ( pld->ulTime <= ulTime ) )
           {
              usNextlineReq++;
              if ( usNextlineReq == 1 )
                 WinPostMsg ( hwndText, UM_NEXTLINE, 0, 0 );
              pld++;
              usTestLine++;
           }
        }
        return 0;

     case MM_MCINOTIFY:
        usNotifyCode    = (USHORT)  SHORT1FROMMP(mp1);
        usUserParm      = (USHORT)  SHORT2FROMMP(mp1);
        usCommandMessage = (USHORT) SHORT2FROMMP(mp2);

        switch ( usCommandMessage )
        {
           /*--------------- Play notification message -----------------------*/
           /*                                                                 */
           /*  We receive this message when a audio play has completed.       */
           /*                                                                 */
           /*  (1) Check to see if the Notification Code portion of the       */
           /*      message contained an error code.  Display the error code,  */
           /*      if present.                                                */
           /*                                                                 */
           /*  (2) If the notification code was MCI_NOTIFY_SUCCESSFUL, that   */
           /*      means that the audio has played to the end.  Invoke the    */
           /*      StopPlaying function to reset all dialog box controls.     */
           /*      Force the audio slider to its extreme right position, just */
           /*      in case the granularity of the MM_MCIPOSITIONCHANGE        */
           /*      messages left it a pixel or two short.                     */
           /*-----------------------------------------------------------------*/

           case MCI_PLAY:
              if ( ( usNotifyCode != MCI_NOTIFY_SUCCESSFUL ) &&
                   ( usNotifyCode != MCI_NOTIFY_SUPERSEDED ) &&
                   ( usNotifyCode != MCI_NOTIFY_ABORTED ) )
              {
                 ulError = (ULONG)usNotifyCode;/*(1)Display an audio error code*/
                 StopPlaying( hwnd );
                 AudioError ( ulError );
              }

              if ( usNotifyCode == MCI_NOTIFY_SUCCESSFUL )
              {                             /* (2) Audio has played to the end*/
                 StopPlaying( hwnd );
                 WinSendMsg( hwndAudioSlider, SLM_SETSLIDERINFO,
                             MPFROM2SHORT( SMA_SLIDERARMPOSITION,
                                           SMA_RANGEVALUE ),
                             MPFROMSHORT( (SHORT) ( sAudioArmRange - 1 )));
              }
        }
        return 0;


     /*-----------------------------------------------------------------------*/
     /* Every time we get an MM_MCIPASSDEVICE message, we save the state of   */
     /* the audio device in bAcquired. Whenever the Main window is activated, */
     /* we check this variable to see if we need to reacquire the audio device*/
     /*                                                                       */
     /* (1) If gaining control of the device:                                 */
     /*                                                                       */
     /*     (1a) Set bAcquired to FALSE.                                      */
     /*     (1b) If the latest request was to play start the Play button      */
     /*          animation.                                                   */
     /*     (1c) If the latest request was to set timing start the Set Timing */
     /*          button animation.                                            */
     /*                                                                       */
     /* (2) If loosing control of the device:                                 */
     /*                                                                       */
     /*     (2a) Set bAcquired to TRUE.                                       */
     /*     (2b) If the latest request was to play stop the Play button       */
     /*          animation.                                                   */
     /*     (2c) If the latest request was to set timing stop the Set Timing  */
     /*          button animation.                                            */
     /*                                                                       */
     /*-----------------------------------------------------------------------*/

     case MM_MCIPASSDEVICE:
        if ((USHORT)SHORT1FROMMP(mp2) == MCI_GAINING_USE) /*(1) gaining control */
        {
           bAcquired = TRUE;                         /* (1a) gained the device*/
           if ( (sLatestPlayRequest == ID_PLAY) &&   /* (1b) playing, start.. */
                (iState == ST_PLAYING) )
           {
              WinSendDlgItemMsg ( hwndMainDlg, ID_PLAY,      /* ...animation  */
                                  GBM_ANIMATE,
                                  MPFROMSHORT((SHORT)TRUE),
                                  MPFROMSHORT((SHORT)FALSE) );
           }
           else
           if ((sLatestPlayRequest==ID_SET_TIMING)&& /*(1c)set timing, start..*/
                     (iState == ST_PLAYING) )
           {
              WinSendDlgItemMsg ( hwndMainDlg, ID_SET_TIMING, /* ...animation*/
                                  GBM_ANIMATE,
                                  MPFROMSHORT((SHORT)TRUE),
                                  MPFROMSHORT((SHORT)FALSE) );
           }

        }
        else                                       /* (2) loosing control     */
        {
           bAcquired = FALSE;                      /* (2a) lost the device.   */
           if ( (sLatestPlayRequest == ID_PLAY) &&   /* (1b) playing, start.. */
                (iState == ST_PLAYING) )
           {
              WinSendDlgItemMsg( hwndMainDlg, ID_PLAY,       /* ...animation  */
                                 GBM_ANIMATE,
                                 MPFROMSHORT((SHORT)FALSE),
                                 MPFROMSHORT((SHORT)FALSE) );
           }
           else
           if ((sLatestPlayRequest==ID_SET_TIMING)&& /*(1c)set timing, start..*/
                     (iState == ST_PLAYING) )
           {
              WinSendDlgItemMsg( hwndMainDlg, ID_SET_TIMING, /* ...animation */
                                 GBM_ANIMATE,
                                 MPFROMSHORT((SHORT)FALSE),
                                 MPFROMSHORT((SHORT)FALSE) );
           }
        }
        return 0;

     /*-----------------------------------------------------------------------*/
     /* Before closing the Main window, check to see if the user wants to save*/
     /* any unsaved timing changes. Then close the device and return          */
     /* WinDefWindowProc to make sure a WM_QUITmessage is sent and we fall out*/
     /* of the message loop.                                                  */
     /*-----------------------------------------------------------------------*/

     case WM_CLOSE:
        if ( bUnsavedChanges )
        {
           if ( SaveChanges( MB_YESNO ) == MBID_CANCEL )
              return 0;
        }
        if (iState != ST_CLOSED)
        {
           CloseAudioDevice( hwnd );
        }
        return WinDefWindowProc( hwnd, msg, mp1, mp2 );
  }
  return WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 );
}

/******************************************************************************
 * Name         : TextWindowProc
 *
 * Description  : This function controls the text window.  It will handle
 *                received messages such as paint, timing events, etc.
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY TextWindowProc( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2 )
{
   HPS          hpsPaint;               /* Handle to Text window pres space   */
   RECTL        rclPaint;               /* Area of the Text window            */
   POINTL       aptlPaint[4];           /* for bitblt                         */


   switch ( msg )
   {
      /*--------------------- Paint text window ------------------------------*/
      /*                                                                      */
      /*  The text to be painted into the text window is held in a monochrome */
      /*  bitmap whose presentation space is hpsText.                         */
      /*                                                                      */
      /*  (1) Setup blitting rectangles based on the area to be painted and   */
      /*      the scroll offset.  These rectangles are identical, except for  */
      /*      the Y-coordinates of the source rectangle.  These are Scroll    */
      /*      Offset pixels greater than the destination.  While scrolling,   */
      /*      the Scroll Offset grows continually smaller, meaning that the   */
      /*      source blitting rectangle scrolls DOWN the bitmap, while the    */
      /*      text in the window appears to scroll UP.)                       */
      /*                                                                      */
      /*  (2) Set the foreground and background colors of the Paint           */
      /*      presentation space to control the color of the background and   */
      /*      the text as it is blitted to the window.                        */
      /*----------------------------------------------------------------------*/

      case WM_PAINT:
         hpsPaint = WinBeginPaint ( hwnd, 0, &rclPaint );

         aptlPaint[0].x = rclPaint.xLeft;    /* (1) Set blit rect coordinates */
         aptlPaint[0].y = rclPaint.yBottom;
         aptlPaint[1].x = rclPaint.xRight;
         aptlPaint[1].y = rclPaint.yTop;
         aptlPaint[2].x = rclPaint.xLeft;
         aptlPaint[2].y = rclPaint.yBottom + lScrollOffset;
         aptlPaint[3].x = rclPaint.xRight;
         aptlPaint[3].y = rclPaint.yTop + lScrollOffset;

         GpiSetColor( hpsPaint, CLR_WHITE );    /* (2) Set background color...*/
         GpiSetBackColor( hpsPaint, CLR_DARKBLUE ); /*  ... and Text color    */

         GpiBitBlt(
            hpsPaint,
            hpsText,
            4,
            aptlPaint,
            ROP_SRCCOPY,
            BBO_IGNORE );
         WinEndPaint( hpsPaint );
         return (MRESULT) 1;


      /*---------------------- WM_TIMER message ------------------------------*/
      /*                                                                      */
      /*  The text window receives a WM_TIMER message whenever it is necessary*/
      /*  to scroll one more scroll increment.  When it is not scrolling, the */
      /*  timer is stopped, and no messages are received.                     */
      /*                                                                      */
      /*  (1) Decrement the Scroll Offset by the Scroll Increment.            */
      /*                                                                      */
      /*  (2) Scroll the Text window UP by the Scroll Increment.              */
      /*                                                                      */
      /*  (3) Check to see if one complete text line has been scrolled.  This */
      /*      is true if the Scroll Offset is zero.  If so:                   */
      /*                                                                      */
      /*      (3a) Stop the timer for now.                                    */
      /*                                                                      */
      /*      (3b) Decrement the number of lines queued for scrolling         */
      /*           ( usNextlineReq ).                                         */
      /*                                                                      */
      /*      (3c) If lines remain to be scrolled, send a message to yourself,*/
      /*           requesting that another line be scrolled into the window.  */
      /*           If no lines remain to be scrolled AND we are in a Start    */
      /*           Timing operation, then unhighlight the Advance Line button.*/
      /*----------------------------------------------------------------------*/

      case WM_TIMER:
         lScrollOffset -= CC_SCROLL_INC;         /* (1) Decrement scroll offst*/

         WinScrollWindow(                        /* (2) Scroll window up      */
            hwnd,
            0,
            CC_SCROLL_INC,
            NULL,
            NULL,
            0,
            NULL,
            SW_INVALIDATERGN );

         if ( lScrollOffset == 0 )                /* (3) Finished line?  */
         {
            WinStopTimer ( hab, hwnd, CC_TIMER_ID );   /* (3a) Stop timer     */
            if ( usNextlineReq )                 /* (3b) Decrement queued lns */
               usNextlineReq--;
            if ( usNextlineReq )                 /* (3c) See if more lines    */
               WinSendMsg( hwnd, UM_NEXTLINE, 0, 0 ); /* need to be scrolled  */
            else
               if ( sLatestPlayRequest == ID_SET_TIMING )
                  WinSendDlgItemMsg (            /* Unhighlight Advance Line  */
                     hwndMainDlg,
                     ID_NEXTLINE,
                     GBM_SETBITMAPINDEX,
                     MPFROMSHORT(GB_CURRENTSTATE), MPFROMSHORT(0) );
         }
         break;


      /*------------------------ UM_NEXTLINE ---------------------------------*/
      /*                                                                      */
      /*  This message requests the Text window to scroll the next line of    */
      /*  text into the window.                                               */
      /*                                                                      */
      /*  (1) Increment the index of the bottom-most line which is to be      */
      /*      displayed in the text window. ( usDisplayLine )                 */
      /*                                                                      */
      /*  (2) Redraw the backup text bitmap with the new line of text at the  */
      /*      bottom.                                                         */
      /*                                                                      */
      /*  (3) Initialize the scroll offset to the Line Spacing.               */
      /*                                                                      */
      /*  (4) Start the timer.  As each WM_TIMER message is received, the text*/
      /*      window will scroll up by one Scroll Increment                   */
      /*----------------------------------------------------------------------*/

      case UM_NEXTLINE:
         usDisplayLine++;                       /* (1) Inc Display Line Index */
         DisplayTextLine();                     /* (2) Redraw backup bitmap   */
         lScrollOffset = lLineSpacing;          /* (3) Initialize scroll offst*/
         WinStartTimer( hab, hwnd, CC_TIMER_ID, 100 );/* (4) Start the timer. */
         return 0;

   }
   return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
}


/******************************************************************************
 * Name         : FileFilterProc
 *
 * Description  : This function filters the messages to the File dialog
 *                procedure.  It intercepts WM_HELP messages and invokes the
 *                help screen when it encounters.
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY FileFilterProc( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2 )
{
   CHAR szError[ERROR_LENGTH];


   if ( msg == WM_HELP )
   {
      if ( WinSendMsg ( hwndHelp, HM_DISPLAY_HELP,
                        MPFROMSHORT ( (SHORT) usFiledlgHelpRes ),
                        MPFROMSHORT ( HM_RESOURCEID ) ) )
      {
         WinLoadString ( hab, (HMODULE)NULL, STRID_HELP_CREATION_FAILED,
                         ERROR_LENGTH, szError );

         WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError, szErrorMsgTitle,
                                          0, MB_OK | MB_ERROR | MB_MOVEABLE);
      }
      return (MRESULT) FALSE;
   }
   return WinDefFileDlgProc( hwnd, msg, mp1, mp2 ) ;
}


/******************************************************************************
 * Name         : FormatFname
 *
 * Description  : This function format the file name for the dialog boxes.
 *                This function accepts a name of the from x:\path\file.ext
 *                and returns a name of the form x:\path\*.ext
 *
 * Parameters   : szInputName  - accepted file name
 *                szOutputName - returned file name
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID FormatFname ( CHAR szOutputName[], CHAR szInputName[] )
{
   CHAR   szWorkName[CCHMAXPATH];
   PCHAR  pcharStart, pcharEnd;


   strcpy ( szWorkName, szInputName );

   pcharEnd = strrchr ( szWorkName, (int) '\\' );
   if ( pcharEnd )
   {                                      /*----------------------------------*/
      *pcharEnd = 0;                      /* A directory name (and possibly a */
      strcpy ( szOutputName, szWorkName );/* drive letter) was present.  Copy */
      strcat ( szOutputName, "\\" );      /* these to the output name.        */
      pcharStart = pcharEnd + 1;          /*----------------------------------*/
   }
   else
   {
      pcharEnd = strrchr ( szWorkName, (int) ':' );
      if ( pcharEnd )
      {                                     /*--------------------------------*/
         *pcharEnd = 0;                     /*No directory found, but there IS*/
         strcpy ( szOutputName, szWorkName);/*a drive letter. Copy the drive  */
         strcat ( szOutputName, ":" );      /*letter to the output name.      */
         pcharStart = pcharEnd + 1;         /*--------------------------------*/
      }
      else
      {                                      /*-------------------------------*/
         szOutputName[0] = 0;                /* No directory or drive letter  */
         pcharStart = szWorkName;            /* was present.                  */
      }                                      /*-------------------------------*/
   }

   strcat ( szOutputName, "*" );        /* Now tack on the new file name: "*" */

   pcharEnd = strrchr ( pcharStart, '.' );   /*-------------------------------*/
   if ( pcharEnd )                           /*Tack on the original extension,*/
      strcat ( szOutputName, pcharEnd );     /*if present.                    */
}                                            /*-------------------------------*/

/******************************************************************************
 * Name         : LoadAudioFile
 *
 * Description  : This function will open the audio device with a selected
 *                audio file, if the audio device is not opened.  If the
 *                audio device is open, the selected audio file will be loaded
 *                onto it.  Also querys the length of the audio file, calibrates
 *                the scale on the audio slider and position the captions at
 *                the beginning of the caption file.
 *
 * Concepts     : - Open a device using the MCi interface.
 *                - Loading a file into an already open device.
 *
 * MMPM/2 API's : mciSendCommand  MCI_OPEN
 *                                MCI_LOAD
 *                                MCI_STATUS
 *
 * Parameters   : hwnd       - main window handle
 *                szFilename - selected audio file
 *
 * Return       : ulError    - if an error occured.
 *
 ******************************************************************************/
ULONG LoadAudioFile ( HWND hwnd, CHAR szFilename[] )
{
   ULONG   ulError;
   CHAR    szAudioPosSeconds[TITLE_LENGTH];     /* Title for Audio Slider     */
   CHAR    szAudioPosMinutes[TITLE_LENGTH];     /* Title for Audio Slider     */
   ULONG   ulAudioLengthMsec;                   /* For audio length in Msec   */
   ULONG   ulAudioLengthSec;                    /* For audio length in Sec    */
   ULONG   ulAudioLengthMin;                    /* For audio length in Minutes*/
   CHAR    szAudioLength[10];                   /* For audio length           */
   CHAR    szAudioMin[10];                      /* Audio length in ASCII      */
   CHAR    szAudioSec[10];                      /* Audio length in ASCII      */
   MRESULT mresult;                             /* for slider result          */
   PCHAR   pcAudioFile;                         /* audio file name            */
   CHAR    szBuffer[CCHMAXPATH];                /* buffer to hold file name   */
   static  MCI_LOAD_PARMS     mlp;              /* Load params                */


   /* If the audio device is closed, open it.  */
   if ( iState == ST_CLOSED )
   {
      mop.hwndCallback     = hwndMainDlg;         /*--------------------------*/
      mop.usDeviceID       = (USHORT)  NULL;      /*     Attempt to open      */
      mop.pszDeviceType    = (PSZ)NULL;           /*    the digital audio     */
      mop.pszElementName   = (PSZ)szFilename;     /*         device.          */
      ulError = mciSendCommand ( (USHORT) 0,      /*--------------------------*/
                                 MCI_OPEN,
                                 MCI_WAIT | MCI_OPEN_SHAREABLE |
                                 MCI_OPEN_ELEMENT | MCI_READONLY,
                                 (PVOID) &mop,
                                 (USHORT) UP_OPEN );

      if ( ulError )
      {
         CloseAudioDevice( hwnd );
         return ulError;
      }
   }
   else
   {
      mlp.hwndCallback     = (HWND) NULL;         /*--------------------------*/
      mlp.pszElementName   = (PSZ)szFilename;     /*     Attempt to load the  */
      ulError =                                   /*     requested audio file */
        mciSendCommand( (USHORT) mop.usDeviceID,  /*--------------------------*/
                        MCI_LOAD,
                        MCI_WAIT | MCI_READONLY,
                        (PVOID) &mlp,
                        (USHORT) UP_LOAD );

      if ( ulError )
      {
         CloseAudioDevice( hwnd );
         return ulError;
      }
   }
/*---------------  The load attempt was successful  --------------------------*/
/*                                                                            */
/* (1) Set the program state switches and the dialog-box status window to     */
/*     reflect the new state.                                                 */
/*                                                                            */
/* (2) Ask MMPM/2 how long the audio file is ( in MM units ) and use this     */
/*     number to calibrate the scale on the audio slider.  The scale is       */
/*     calibrated in seconds if the length is less than 60 seconds, and in    */
/*     minutes and seconds if the length is 60 seconds or greater.  Set the   */
/*     audio slider bar all the way to the left.                              */
/*                                                                            */
/* (3) Position the captions at the beginning of the caption file.            */
/*                                                                            */
/* (4) Enable the Rewind button and possibly the Start Timing and Play buttons*/
/*     in the dialog box.                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   iState       = ST_OPEN;                        /* (1) Set state switches.. */

   bAudioLoaded = TRUE;

   msp.hwndCallback = (HWND) NULL;                 /*-------------------------*/
   msp.ulItem     = MCI_STATUS_LENGTH;             /* (2)  Ascertain the      */
   mciSendCommand ( (USHORT) mop.usDeviceID,       /*    play length of the   */
                    MCI_STATUS,                    /*       audio file.       */
                    MCI_WAIT | MCI_STATUS_ITEM,    /*-------------------------*/
                    (PVOID) &msp,
                    (USHORT) UP_STATUS );

   ulAudioLength     = msp.ulReturn;                 /*-----------------------*/
                                                     /* (2) Initialize the    */
   ulAudioLengthMsec = MSECFROMMM ( ulAudioLength ); /*     audio position    */
   ulAudioLengthSec  = ulAudioLengthMsec / 1000;     /*     slider control    */
                                                     /*-----------------------*/
   if ( ulAudioLengthSec >= 60 )
   {
      ulAudioLengthSec %= 60;
      ulAudioLengthMin  = ( ulAudioLengthMsec / 60000 );
      sprintf(szAudioLength, "%01lu:%02lu", ulAudioLengthMin, ulAudioLengthSec);
      WinLoadString( hab, 0, STRID_AUDIO_POS_MINUTES,
                     TITLE_LENGTH, szAudioPosMinutes );
      WinSetWindowText( WinWindowFromID( hwnd, ID_AUDIO_POSITION ),
                        szAudioPosMinutes );
   }
   else
   {
      sprintf ( szAudioLength, "%lu", ulAudioLengthSec );
      WinLoadString( hab, 0, STRID_AUDIO_POS_SECONDS,TITLE_LENGTH,
                         szAudioPosSeconds );
      WinSetWindowText ( WinWindowFromID( hwnd, ID_AUDIO_POSITION ),
                         szAudioPosSeconds );
   }

   WinEnableWindow   ( hwndAudioSlider, TRUE );

   WinSendMsg ( hwndAudioSlider, SLM_SETSCALETEXT,
                MPFROMSHORT ( (SHORT) ( AUDIO_SLIDER_TICKS - 1 ) ),
                MPFROMP ( szAudioLength ) );
   WinSendMsg ( hwndAudioSlider, SLM_SETSCALETEXT, 0,
                MPFROMP ( "0" ) );
   WinSendMsg ( hwndAudioSlider, SLM_SETSLIDERINFO,
                MPFROM2SHORT ( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                MPFROMSHORT  ( 0 ) );
   mresult = WinSendMsg( hwndAudioSlider, SLM_QUERYSLIDERINFO,
                         MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_RANGEVALUE), 0);
   sAudioArmRange = SHORT2FROMMP ( (MPARAM) mresult );

   PositionCaptions ( 0 );                  /* (3) Position captions at start */

                                            /* (4) Enable dlg box pushbuttons */
   WinEnableWindow ( WinWindowFromID ( hwndMainDlg, ID_REWIND ), TRUE );

   if ( bTextLoaded )
      WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_SET_TIMING ), TRUE );
   if ( bCaptionsLoaded )
      WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_PLAY ), TRUE );

   pcAudioFile = strrchr( szFilename, '\\' );
   pcAudioFile++;
   strcpy(szBuffer, szAudioFileName);
   strcat(szBuffer, pcAudioFile);
   WinSetWindowText( WinWindowFromID( hwnd, ID_AUDIOTXT), szBuffer );

   return ulError;
}


/******************************************************************************
 * Name         : StartPlaying
 *
 * Description  : This function will begin the playing of an audio file from
 *                the location indicated by the audio slider and it will
 *                continue until the end of file. It is invoked when the user
 *                selects the Start timing or Play pushbutton on the Captions
 *                main window.
 *
 * Concepts     : Playing an audio file using MCI interface.
 *
 * MMPM/2 API's : mciSendCommand  MCI_PLAY
 *
 * Parameters   : hwnd       - main window handle
 *
 * Return       : ulError    - if an error occured.
 *
 ******************************************************************************/
ULONG StartPlaying ( HWND hwnd )
{
   static  MCI_PLAY_PARMS   mpp;       /* parms for MCI_PLAY                  */
   static  MCI_POSITION_PARMS mppPos;  /* parms for MCI_SET_POSITION_ADVISE   */
   ULONG   ulError;                    /* Return code                         */
   MRESULT mresultSlider; /*--------------------------------------------------*/
   SHORT   sArmPosition;  /*Read the slider position to determine the start of*/
   ULONG   ulStartPos;    /*the audio play. If the slider is at the extreme   */
                          /*right of its range, start over at the beginning.  */
                          /*--------------------------------------------------*/

   mresultSlider =
          WinSendMsg( hwndAudioSlider, SLM_QUERYSLIDERINFO,
                      MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE), 0 );
   sArmPosition = SHORT1FROMMP ( (MPARAM) mresultSlider );

   if ( sArmPosition >=  ( sAudioArmRange - 1 ) )
   {
      ulStartPos = 0;
      WinSendMsg( hwndAudioSlider, SLM_SETSLIDERINFO,
                  MPFROM2SHORT ( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                  MPFROMSHORT  ( 0 ) );
      PositionCaptions ( 0 );
   }
   else
      ulStartPos = (ULONG)((sArmPosition * ulAudioLength)/(sAudioArmRange - 1));


   mpp.hwndCallback = hwndMainDlg;
   mpp.ulFrom = ulStartPos;
   ulError =  mciSendCommand ( mop.usDeviceID,        /*--------------------*/
                               MCI_PLAY,              /*          Start     */
                               MCI_NOTIFY | MCI_FROM, /*         playback.  */
                               (PVOID) &mpp,          /*--------------------*/
                               (USHORT) UP_PLAY );

   if ( ulError )
   {
      StopPlaying( hwnd );
      return ulError;
   }


 /*------------------------------------------------------------------------*/
 /*                                                                        */
 /*         If there were no errors starting the play:                     */
 /*                                                                        */
 /*        (1) Request position advise messages.                           */
 /*        (2) Enable PAUSE and STOP controls.                             */
 /*        (3) Begin animating the button that started the play            */
 /*            ( either PLAY or SET TIMING )                               */
 /*                                                                        */
 /*------------------------------------------------------------------------*/

   iState = ST_PLAYING;                    /* Set state to reflect play mode */

   mppPos.hwndCallback = hwndMainDlg;      /*-----------------------------*/
   mppPos.ulUnits    = 1500;               /* (1) Request position advise */
   mppPos.usUserParm = usPositionUP;       /*             messages.       */
   mppPos.Reserved0  = 0;                  /*-----------------------------*/
   mciSendCommand    ( mop.usDeviceID,
                       MCI_SET_POSITION_ADVISE,
                       MCI_NOTIFY | MCI_SET_POSITION_ADVISE_ON,
                       (PVOID) &mppPos,
                       UP_POSITION );

   WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_PAUSE  ), TRUE  );/*(2) */
   WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_STOP   ), TRUE  );

   switch ( sLatestPlayRequest )        /* (3) Start animating the button that*/
      {                                 /*     initiated the play operation.  */
      case ID_SET_TIMING:
         WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_PLAY ), FALSE );
         WinSendDlgItemMsg(hwndMainDlg, ID_SET_TIMING, GBM_ANIMATE,
                           MPFROMSHORT((SHORT)TRUE), MPFROMSHORT((SHORT)FALSE));
         WinEnableWindow( WinWindowFromID ( hwndMainDlg, ID_NEXTLINE ), TRUE );
         WinSendDlgItemMsg(hwndMainDlg, ID_NEXTLINE, GBM_SETBITMAPINDEX,
                           MPFROMSHORT(GB_CURRENTSTATE), MPFROMSHORT(0) );
         break;

      case ID_PLAY:
         WinEnableWindow( WinWindowFromID( hwndMainDlg, ID_SET_TIMING ), FALSE);
         WinSendDlgItemMsg(hwndMainDlg, ID_PLAY, GBM_ANIMATE,
                           MPFROMSHORT((SHORT)TRUE), MPFROMSHORT((SHORT)FALSE));
   }
   return ulError;
}

/******************************************************************************
 * Name         : StopPlaying
 *
 * Description  : This function will stop the device that is playing.
 *
 * Concepts     : Stopping a device using MCI interface.
 *
 * MMPM/2 API's : mciSendCommand  MCI_STOP
 *
 * Parameters   : hwnd       - main window handle
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID StopPlaying ( HWND hwnd )
{
   /***************************************************************************/
   /*                                                                         */
   /* (1) Send a message to MMPM/2 requesting the current play to stop.       */
   /*                                                                         */
   /* (2) Update the internal status to indicate that the play has stopped.   */
   /*     Show the new state in the status line in the main dialog box.       */
   /*                                                                         */
   /* (3) Disable the Pause, Stop and Advance Line controls                   */
   /*                                                                         */
   /* (4) Stop animating the button which requested the audio play ( either   */
   /*     Play or Start Timing ).                                             */
   /*                                                                         */
   /* (5) Enable the File, Help and Options submenus.                         */
   /***************************************************************************/

   usNextlineReq  = 0;
   mgp.hwndCallback = 0;                /*-------------------------------------*/
   mciSendCommand ( mop.usDeviceID,     /*                                     */
                    MCI_STOP,           /*(1) Issue an MCI_STOP command for the*/
                    MCI_WAIT,           /*    audio file.                      */
                    (PVOID) &mgp,       /*                                     */
                    (USHORT) UP_STOP ); /*-------------------------------------*/

   iState = ST_OPEN;                   /*(2) Update internal & external status*/

   WinSendDlgItemMsg( hwndMainDlg, ID_PAUSE, GBM_SETBITMAPINDEX,
                                 MPFROMSHORT(GB_CURRENTSTATE), MPFROMSHORT(0) );
   WinSendDlgItemMsg( hwndMainDlg, ID_NEXTLINE, GBM_SETBITMAPINDEX,
                                 MPFROMSHORT(GB_CURRENTSTATE), MPFROMSHORT(0) );

   WinEnableWindow ( WinWindowFromID( hwndMainDlg, ID_PAUSE   ), FALSE );
   WinEnableWindow ( WinWindowFromID( hwndMainDlg, ID_STOP    ), FALSE );/*(3)*/
   WinEnableWindow ( WinWindowFromID( hwndMainDlg, ID_NEXTLINE), FALSE );

   switch ( sLatestPlayRequest )      /* (4) Stop animating the graphic button*/
   {                                  /*     which started the PLAY operation.*/
      case ID_SET_TIMING:
         WinEnableWindow ( WinWindowFromID ( hwndMainDlg, ID_PLAY ), TRUE );
         WinSendDlgItemMsg ( hwndMainDlg, ID_SET_TIMING, GBM_ANIMATE,
                             MPFROMSHORT((SHORT)FALSE),
                             MPFROMSHORT((SHORT)FALSE) );
         break;

      case ID_PLAY:
         WinEnableWindow( WinWindowFromID( hwndMainDlg, ID_SET_TIMING ), TRUE);
         WinSendDlgItemMsg ( hwndMainDlg, ID_PLAY, GBM_ANIMATE,
                             MPFROMSHORT((SHORT)FALSE),
                             MPFROMSHORT((SHORT)FALSE) );
   }
   bAcceptPlayRequests = TRUE;
}

/******************************************************************************
 * Name         : CloseAudioDevice
 *
 * Description  : This function will close the audio device.
 *
 * Concepts     : Closing a device using MCI interface.
 *
 * MMPM/2 API's : mciSendCommand  MCI_CLOSE
 *
 * Parameters   : hwnd       - main window handle
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID CloseAudioDevice ( HWND hwnd )
{
   mgp.hwndCallback = (HWND) NULL;       /*-----------------------------------*/
   mciSendCommand ( mop.usDeviceID,      /*                                   */
                    MCI_CLOSE,           /*                                   */
                    MCI_WAIT,            /* Close the digital audio device.   */
                    (PVOID) &mgp,        /*                                   */
                    (USHORT) UP_CLOSE ); /*                                   */
                                         /*-----------------------------------*/
   iState            = ST_CLOSED;
}


/******************************************************************************
 * Name         : ShareAudioDevice
 *
 * Description  : This function will control audio device sharing.
 *                Every time the activation status of the main window changes,
 *                this function will be invoked to see if we needed to regain
 *                the control of the audio device.
 *
 * Concepts     : Sharing a audio device using MCI interface.
 *
 * MMPM/2 API's : mciSendCommand  MCI_ACQUIREDEVICE
 *
 * Parameters   : none.
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID ShareAudioDevice ( void )
{

   if ( bMainActive && ( iState != ST_CLOSED ) && ! bAcquired )
   {
      mgp.hwndCallback = hwndMainDlg;     /*----------------------------------*/
      mciSendCommand( mop.usDeviceID,     /*Request the device if:            */
                      MCI_ACQUIREDEVICE,  /* (a) Either window active   AND...*/
                      (ULONG)MCI_NOTIFY,  /* (b) We have the device open AND..*/
                      (PVOID) &mgp,       /* (c) We don't own the device    . */
                      (USHORT)NULL);      /*----------------------------------*/
   }
}


/******************************************************************************
 * Name         : AudioError
 *
 * Description  : This function will display the MCI error message based upon
 *                the ulError return code.  The MCI function mciGetErrorString
 *                is used to convert the error code into a text string. The
 *                resulting string will displayed to the user.
 *
 * Concepts     : Using mciGetErrorString to convert an error code into a
 *                textual message.
 *
 * MMPM/2 API's : mciGetErrorString
 *
 * Parameters   : ulError  - MCI error code
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID AudioError ( ULONG ulError )
{
   CHAR  szErrorMessage [ERROR_LENGTH];


   mciGetErrorString ( ulError, (PSZ)szErrorMessage, ERROR_LENGTH );

   WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szErrorMessage, szErrorMsgTitle,
                                           0, MB_OK | MB_ERROR | MB_MOVEABLE );
}


/******************************************************************************
 * Name         : LoadTextFile
 *
 * Description  : This function will load the selected text or caption file
 *                for processing.
 *
 * Parameters   : szFilename - selected text file
 *
 * Return       : apiret     - if an error occured.
 *
 ******************************************************************************/
APIRET LoadTextFile ( CHAR szFilename[] )
{
   APIRET    apiret;         /* DOS return code                               */
   HFILE     hfile;          /* Text file handle                              */
   ULONG     ulAction;       /* Parameter returned by various DOS API func.   */
   ULONG     ulFileSize;     /* Expected file size                            */
   PLINEDATA pld;            /* Used in building the Line Data Table          */
   PSZ       psz;
   PLINEDATA pldIn, pldOut;
   PVOID     pvTextBackup;   /* Temporary holding area for pvText             */
   MRESULT   mresult;        /* Data returned for queries of slider arm info. */
   SHORT     sArmPosition;   /* Audio slider arm position                     */


/*----------------------- Open the text file ---------------------------------*/

   apiret = DosOpen ( (PSZ) szFilename, &hfile, &ulAction, 0,
                      FILE_NORMAL | FILE_READONLY, FILE_OPEN,
                      OPEN_ACCESS_READONLY | OPEN_SHARE_DENYREADWRITE, NULL );

   if ( apiret ) return apiret;


/*------------ Read the entire file into a memory area -----------------------*/

   pvTextBackup = pvText;                   /* Save the currently-loaded text */
                                            /* data in case the open fails.   */

   DosSetFilePtr( hfile, 0, FILE_END, &ulFileSize );/*Determine file size and */
   apiret = DosAllocMem(&pvText, ulFileSize, fALLOC);/* allocate memory for it*/

   if ( apiret)                 /*----------- Open failed --------------------*/
   {                            /*                                            */
      DosClose ( hfile );       /* Return the error code to the user and      */
      pvText = pvTextBackup;    /* restore the previous text data pointer.    */
      return apiret;            /*--------------------------------------------*/
   }

   DosSetFilePtr ( hfile, 0, FILE_BEGIN, &ulAction );  /* Read the file image */
   apiret = DosRead( hfile, pvText, ulFileSize, &ulAction );/* into the buffer*/
   DosClose ( hfile );
   if ( ! apiret && ( ulFileSize != ulAction ) ) apiret = ERROR_UNEXPECTED_EOF;

   if ( apiret )                /*----------- Read failed --------------------*/
   {                            /*                                            */
      DosFreeMem ( pvText );    /* Return the error code to the user and      */
      pvText = pvTextBackup;    /* restore the previous text data pointer.    */
      return apiret;            /*--------------------------------------------*/
   }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* At this point, the existing text or caption file must be unloaded.  Set the*/
/* dialog box controls to reflect this state, just in case something goes     */
/* wrong while setting up the new text or caption file.                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   DosFreeMem ( pvTextBackup );          /* Free previously-loaded text data. */

   bTextLoaded     = FALSE;              /* Set internal switches             */
   bCaptionsLoaded = FALSE;
   bUnsavedChanges = FALSE;
   szTextFile[0] = 0;
   szCaptionFile[0] = 0;

   GpiErase ( hpsText );                 /* Blankout text window              */
   WinInvalidateRect ( hwndText, NULL, FALSE );

   WinSendMsg ( hwndMenu, MM_SETITEMATTR,/* Disable Save and Save as... menus */
                MPFROM2SHORT ( IDM_SAVE, TRUE ),
                MPFROM2SHORT ( MIA_DISABLED, MIA_DISABLED ) );
   WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                MPFROM2SHORT ( IDM_SAVEAS, TRUE ),
                MPFROM2SHORT ( MIA_DISABLED, MIA_DISABLED ) );

   WinEnableWindow  ( WinWindowFromID ( hwndMainDlg, ID_SET_TIMING ), FALSE );
   WinEnableWindow  ( WinWindowFromID ( hwndMainDlg, ID_PLAY       ), FALSE );


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  Convert the file image to a series of NULL-terminated strings by changing */
/*  all carriage-returns to NULLs.  Build the Line Data Table with an entry   */
/*  pointing to each string.  When done, usLineCount will contain a count of  */
/*  the entries used in the Line Data Table.                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   pld = (PLINEDATA) pvLinedataTable;
   psz = (PSZ) pvText;

   for ( usLineCount = 1; usLineCount < MAX_TEXT_LINES; usLineCount++ )
   {
      pld->szText = psz;
      pld->ulTime = LINE_NOT_TIMED;
      psz = (PSZ) memchr ( pvText, CHAR_RETURN, (size_t) ulFileSize );
      if ( psz == NULL ) break;
      *psz = CHAR_NULL;
      psz += 2;
      if ( *psz == CHAR_EOF ) break;
      pld++;
   }
   if ( usLineCount == MAX_TEXT_LINES ) return (APIRET) ERROR_TOO_MANY_LINES;

   bTextLoaded = TRUE;


/*------- Put the length of each string in the Line Data Table ---------------*/

   pld = (PLINEDATA) pvLinedataTable;
   for ( usDisplayLine = 0; usDisplayLine < usLineCount; usDisplayLine++ )
   {
      pld->lTextLen = (LONG) strlen ( pld->szText );
      pld++;
   }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* If the user has loaded a text file that is already in Caption format,      */
/* build the Line Data Table from the information already present in the file.*/
/* If the file is plain text, mark the first line for immediate display.      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   pldOut = (PLINEDATA) pvLinedataTable;

   if ( ! strcmp ( (PCHAR) pldOut->szText, szCC_FILE ) )
   {
      bCaptionsLoaded = TRUE;
      WinSendMsg ( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT ( IDM_SAVE,   TRUE ),
                                             MPFROM2SHORT ( MIA_DISABLED, 0 ) );
      WinSendMsg ( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT ( IDM_SAVEAS, TRUE ),
                                             MPFROM2SHORT ( MIA_DISABLED, 0 ) );
      strcpy ( szCaptionFile, szFilename );
      pldIn = pldOut + 1;
      usLineCount--;
      for ( usDisplayLine = 0; usDisplayLine< usLineCount; usDisplayLine++ )
      {
         *pldOut = *pldIn;
         pldOut->szText[8] = CHAR_NULL;
         pldOut->ulTime    = (ULONG) atol ( (PCHAR) pldOut->szText );
         pldOut->szText   += 9;
         pldOut->lTextLen -= 9;
         pldIn++;
         pldOut++;
      }
   }
   else
   {
      bCaptionsLoaded = FALSE;
      WinSendMsg ( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT ( IDM_SAVE,   TRUE ),
                                 MPFROM2SHORT ( MIA_DISABLED, MIA_DISABLED ) );
      WinSendMsg ( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT ( IDM_SAVEAS, TRUE ),
                                 MPFROM2SHORT ( MIA_DISABLED, 0            ) );
      strcpy ( szTextFile, szFilename );
      szCaptionFile[0] = 0;
      pldOut = (PLINEDATA) pvLinedataTable;/*---------------------------------*/
      pldOut[0].ulTime = 0;                /*Mark the first line for immediate*/
                                           /*display                          */
   }                                       /*---------------------------------*/

   /*-------------------------------------------------------------------------*/
   /* Now set the initial caption position.  If there is an audio file loaded,*/
   /* set the position according to the position of the audio file.  If there */
   /* is no audio file loaded, just display the first line of text at the     */
   /* bottom of the text window                                               */
   /*-------------------------------------------------------------------------*/

   if ( iState != ST_CLOSED )
   {
      mresult =
          WinSendMsg( hwndAudioSlider, SLM_QUERYSLIDERINFO,
                      MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE), 0 );
      sArmPosition = SHORT1FROMMP ( (MPARAM) mresult );
      PositionCaptions ( sArmPosition );
   }
   else
   {
      usDisplayLine = 0;                           /*-------------------------*/
      DisplayTextLine();                           /* Display the first line  */
      WinInvalidateRect ( hwndText, NULL, FALSE ); /*-------------------------*/
      usNextline = 1;
   }


/*--- Set the Start Timing button, the Play button ---------------------------*/

   if ( bAudioLoaded )
   {
      WinEnableWindow ( WinWindowFromID ( hwndMainDlg, ID_SET_TIMING ), TRUE );
   }

   if ( bCaptionsLoaded && bAudioLoaded )
      WinEnableWindow ( WinWindowFromID ( hwndMainDlg, ID_PLAY ), TRUE );

   return 0;
}


/******************************************************************************
 * Name         : PositionCaptions
 *
 * Description  : This function will position the captions in the caption
 *                window according to the position of the audio slider.
 *
 * Parameters   : aArmPos   -  audio slider arm position
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID PositionCaptions ( SHORT sArmPos )
{
   PLINEDATA pld;
   ULONG     ulAudioPos;                        /* Audio position in MM units */
   SHORT     sLineIndex;


   if ( ! bTextLoaded ) return;
                                                  /*--------------------------*/
   WinStopTimer ( hab, hwndText, CC_TIMER_ID );   /*   Be sure to stop any    */
   usNextlineReq = 0;                             /*   scrolling in progress. */
   lScrollOffset = 0;                             /*--------------------------*/

   ulAudioPos = (ULONG) ( ulAudioLength * sArmPos / ( sAudioArmRange - 1 ) );
   pld        = (PLINEDATA) pvLinedataTable;

   for ( sLineIndex = 0; sLineIndex < (SHORT) usLineCount; sLineIndex++ )
   {
       if ( pld->ulTime == ulAudioPos ) break;  /*---------------------------*/
       if ( pld->ulTime > ulAudioPos )          /* Scan the Line Data Table  */
       {                                        /* until we reach a line     */
          sLineIndex--;                         /* to be shown at or later   */
          if ( sLineIndex < 0 ) sLineIndex = 0; /* than the current audio    */
          break;                                /* position, or until we run */
       }                                        /* off the end of the table. */
       pld++;                                   /*---------------------------*/
   }

   if ( sLineIndex == (SHORT) usLineCount ) sLineIndex--;

   if ( usDisplayLine != (USHORT) sLineIndex )
   {
      usDisplayLine     = (USHORT) sLineIndex;      /*------------------------*/
      DisplayTextLine   ();                         /*      Display the text  */
      WinInvalidateRect ( hwndText, NULL, FALSE );  /*------------------------*/
      usNextline        = usDisplayLine + 1;
   }
}


/******************************************************************************
 * Name         : DisplayTextLine
 *
 * Description  : This function will write a line of text to the backup bitmap
 *                of the text window to display it.
 *
 * Parameters   : none.
 *
 * Return       : none.
 *
 ******************************************************************************/
static VOID DisplayTextLine ( void )
{
   PLINEDATA pld;
   POINTL    ptl;
   USHORT    usTestLine;
   LONG      lLineCount;
   SHORT     sPosition;

/*-- Erase the pres. space and point ptl to the left end of the bottom line --*/

   GpiErase ( hpsText );
   pld        = (PLINEDATA) pvLinedataTable + usDisplayLine;
   ptl.x      = CC_XMARGIN;
   ptl.y      = lLastLineY;
   usTestLine = usDisplayLine;


/*----------------------------------------------------------------------------*/
/* Write lines from bottom to top until we have filled the window or run out  */
/* of lines.                                                                  */
/*----------------------------------------------------------------------------*/

   for ( lLineCount = 0; lLineCount < lDisplayLines; lLineCount++ )
   {
      if ( usTestLine < usLineCount )
         GpiCharStringAt ( hpsText, &ptl, pld->lTextLen, (PCH) pld->szText );
      if ( usTestLine == 0 ) break;
      ptl.y += lLineSpacing;
      pld--;
      usTestLine--;
   }
}


/******************************************************************************
 * Name         : SaveChanges
 *
 * Description  : This function will ask the user if he wants to save the
 *                changes made to the text or caption file.
 *
 * Parameters   : flStyle  - buttons style flags for the message box.
 *
 * Return       : usReturn - result of the WinMessageBox call.
 *
 ******************************************************************************/
USHORT SaveChanges ( ULONG flStyle )
{
   USHORT usReturn;
   PSZ    psz, pszSlash;
   CHAR   szMessage[100];


   WinLoadString ( hab, 0, STRID_SAVECHANGES_MESSAGE, 100,         szMessage );

   usReturn = WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szMessage,
                              szErrorMsgTitle, 0,
                              flStyle | MB_QUERY | MB_MOVEABLE);


/*---- If the user asks to save the file, give him a Save as dialog box ------*/

   if ( usReturn == MBID_YES )
   {
      if ( szCaptionFile [0] )
         FormatFname ( fdgSaveAs.szFullFile, szCaptionFile );
      else
      {
         FormatFname ( fdgSaveAs.szFullFile, szTextFile );
         psz      = (PSZ) strrchr ( fdgSaveAs.szFullFile, '.' );
         pszSlash = (PSZ) strrchr ( fdgSaveAs.szFullFile, '\\' );
         if ( pszSlash < psz )  strcpy  ( (char *)psz, "._CC" );
      }

      usFiledlgHelpRes = PANEL_FILEDLG_SAVEAS;
      if ( DisplaySaveasDlg(IDM_SAVE) == DID_CANCEL )
         usReturn = MBID_CANCEL;
   }

   return usReturn;
}


/******************************************************************************
 * Name         : CheckForTiming
 *
 * Description  : This function will check for untimed lines in the text file.
 *                This function is called just before a caption file is to be
 *                saved.  It scans the Line Data Table looking for lines which
 *                have been left untimed.  If an untimed line is found, the
 *                function then asks the user if he still wants to save.
 *
 * Parameters   : none.
 *
 * Return       : TRUE   -  if the file is to be saved (or if there are no
 *                          untimed lines).
 *                FALSE  -  if the user decides not to save the file.
 *
 ******************************************************************************/
BOOL CheckForTiming ( void )
{
   PLINEDATA pld;
   USHORT    usLineIndex, usReturn;
   CHAR      szError[ERROR_LENGTH];

   pld = (PLINEDATA) pvLinedataTable;
   for ( usLineIndex = 0; usLineIndex < usLineCount; usLineIndex++ )
   {
      if ( pld->ulTime == LINE_NOT_TIMED )
      {
         WinLoadString ( hab, 0, STRID_UNTIMED_MESSAGE, ERROR_LENGTH, szError );

         usReturn = WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError,
                                    szErrorMsgTitle, 0,
                                    MB_YESNO | MB_QUERY | MB_MOVEABLE);
         if ( usReturn == MBID_YES )
            return TRUE;
         else
            return FALSE;
      }
      pld++;
   }

   return TRUE;
}


/******************************************************************************
 * Name         : SaveCaptionFile
 *
 * Description  : This function will save the caption file that is currently
 *                loaded.
 *
 * Parameters   : szFilename  - use this file name to save
 *                sMenuItem   - menu selected like "Save" or "Save as"
 *
 * Return       : apiret   -  function return code.
 *
 ******************************************************************************/
APIRET SaveCaptionFile ( CHAR szFilename[], SHORT sMenuItem )
{
   HFILE      hfile;
   ULONG      ulAction;
   USHORT     usResponse;
   PLINEDATA  pld;
   USHORT     usLineIndex;
   CHAR       szOutput[MAX_OUTPUT_LENGTH];
   BOOL       bTruncated;
   APIRET     apiret;
   CHAR       szError[ERROR_LENGTH];
   USHORT     i;

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  Try to open the Caption File.  If the user is trying to "Save as..." a    */
/*  file which already exists, issue a warning and ask him if he wants to     */
/*  overwrite the file.                                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   apiret = DosOpen ( (PSZ) szFilename, &hfile, &ulAction, 0,
                      FILE_NORMAL, FILE_CREATE,
                      OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYREADWRITE, NULL );

   if ( ( apiret == ERROR_OPEN_FAILED ) &&
        ( sMenuItem == IDM_SAVE ) )
      apiret = DosOpen( (PSZ) szFilename, &hfile, &ulAction, 0,
                        FILE_NORMAL, FILE_TRUNCATE,
                        OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYREADWRITE, NULL);

                                                 /* Trap overwrites for the   */
   if ( apiret == ERROR_OPEN_FAILED )            /* SAVE AS... menu item      */
   {
      WinLoadString ( hab, 0, STRID_OVERWRITE_QUERY, ERROR_LENGTH, szError );
      usResponse =
        WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError, szFilename,
                                   0, MB_OKCANCEL | MB_QUERY | MB_MOVEABLE);

      if ( usResponse == MBID_CANCEL )
      {
        /*
         * If the user select's cancel, that means he wants to retry saving
         * the file using a different name.  So, go thru the same logic
         * again.
         */
        DisplaySaveasDlg(sMenuItem);
        return apiret;
      }
      /*
       * The user's response was OK to the "overwrite" warning message.
       * Go ahead and overwrite the file.
       */
      else
      {
         apiret =
           DosOpen( (PSZ) szFilename, &hfile, &ulAction, 0,
                    FILE_NORMAL, FILE_TRUNCATE,
                    OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYREADWRITE, NULL );
      }
   }

   if ( apiret )
   {
      DisplayDosError ( szErrorMsgTitle, apiret );
      return apiret;
   }


/*---------------- Write the Caption data to the file ------------------------*/

   bTruncated = FALSE;

   apiret = WriteLine ( hfile, (PSZ) szCC_FILE );
   if ( !apiret )
   {
      pld = (PLINEDATA) pvLinedataTable;
      for ( usLineIndex = 0; usLineIndex < usLineCount; usLineIndex++ )
      {
         sprintf ( szOutput, "%08lu ", pld->ulTime );
         if ( ( pld->lTextLen + 10 ) > MAX_OUTPUT_LENGTH ) bTruncated = TRUE;
         strncat(szOutput,(char *)pld->szText,(size_t)(MAX_OUTPUT_LENGTH - 10));
         apiret = WriteLine ( hfile, (PSZ) szOutput );
         if ( apiret ) break;
         pld++;
      }
      if ( ! apiret )
      {
         apiret = DosWrite ( hfile, &cEOF, 1, &ulAction );
         if ( ! apiret && ! ulAction ) apiret = ERROR_DISK_FULL;
         if ( apiret ) DisplayDosError ( szErrorMsgTitle, apiret );
      }
   }
   DosClose ( hfile );
   if ( bTruncated )
   {
      WinLoadString ( hab, 0, STRID_TRUNCATE_WARNING, ERROR_LENGTH, szError );
      WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError, szFilename, 0,
                                    MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
   }
   if ( ! apiret ) bUnsavedChanges = FALSE;
   return apiret;
}


/******************************************************************************
 * Name         : WriteLine
 *
 * Description  : This function will write a single line to the caption file.
 *                loaded.
 *
 * Parameters   : hfile  - Handle of output file
 *                psz    - Pointer to a NULL-terminated string to write to the
 *                         file.
 *
 * Return       : apiret -  function return code.
 *
 ******************************************************************************/
APIRET WriteLine ( HFILE hfile, PSZ psz )
{
   APIRET apiret;
   ULONG  ulBytesReq;
   ULONG  ulBytesWritten;
   CHAR   acNewline[] = { CHAR_RETURN, CHAR_LINEFEED };


   ulBytesReq = (ULONG) strlen ( (char *)psz );
   apiret = DosWrite ( hfile, psz, ulBytesReq, &ulBytesWritten );
   if ( ! apiret && ( ulBytesWritten < ulBytesReq ) ) apiret = ERROR_DISK_FULL;
   if ( ! apiret )
   {
      apiret = DosWrite ( hfile, acNewline, 2, &ulBytesWritten );
      if ( ! apiret && ( ulBytesWritten < 2 ) ) apiret = ERROR_DISK_FULL;
   }
   if ( apiret ) DisplayDosError ( szErrorMsgTitle, apiret );
   return apiret;
}


/******************************************************************************
 * Name         : DisplayDosError
 *
 * Description  : This function will display a Dos of local file error.
 *
 * Parameters   : szTitle - Error message-box title.
 *                apiret  - DOS or local error code.
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID DisplayDosError ( CHAR szTitle[], APIRET apiret )
{
   CHAR  szError[ERROR_LENGTH];
   CHAR  szFormat[ERROR_LENGTH];


   switch ( apiret )
   {
      case ERROR_TEXT_FILE_FORMAT:
         WinLoadString( hab, 0, STRID_TEXT_FILE_FORMAT, ERROR_LENGTH, szError );
         break;

      case ERROR_TOO_MANY_LINES:
         WinLoadString ( hab, 0, STRID_TOO_MANY_LINES, ERROR_LENGTH, szError );
         break;

      case ERROR_UNEXPECTED_EOF:
         WinLoadString ( hab, 0, STRID_UNEXPECTED_EOF, ERROR_LENGTH, szError );
         break;

      case ERROR_DISK_FULL:
         WinLoadString ( hab, 0, STRID_DISK_FULL, ERROR_LENGTH, szError );
         break;

      case ERROR_CANNOT_FIND_TEXTFILE:
         WinLoadString ( hab, 0, STRID_CANNOT_FIND_TEXTFILE, ERROR_LENGTH, szError );
         break;

      default:
         WinLoadString(hab, 0, STRID_GENERIC_DOS_ERROR, ERROR_LENGTH, szError );
   }

   WinMessageBox ( HWND_DESKTOP, hwndMainDlg, szError, szTitle, 0,
                                               MB_OK | MB_ERROR | MB_MOVEABLE);
}


/******************************************************************************
 * Name         : ResizeTextWindow
 *
 * Description  : This function will resize an existing text window to a
 *                different number of lines selected. Also adjust the main
 *                dialog window according to the text window size.
 *
 * Parameters   : lNewDisplayLines  - Number of lines desired.
 *
 * Return       : none.
 *
 ******************************************************************************/
static VOID ResizeTextWindow ( LONG lNewDisplayLines )
{
   LONG  lWidth;
   LONG  lHeight;
   LONG  lX;
   LONG  lY;
   SWP   swpFrame;
   SWP   swpDialog;
   SWP   swpText;

/*-------- Uncheck the menu item for previous display lines ------------------*/

   switch ( lDisplayLines )
   {
      case 2: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                                     MPFROM2SHORT ( IDM_2_LINES, TRUE ),
                                     MPFROM2SHORT ( MIA_CHECKED, 0 ) );
              break;

      case 3: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                                     MPFROM2SHORT ( IDM_3_LINES, TRUE ),
                                     MPFROM2SHORT ( MIA_CHECKED, 0 ) );
              break;

      case 4: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                                     MPFROM2SHORT ( IDM_4_LINES, TRUE ),
                                     MPFROM2SHORT ( MIA_CHECKED, 0 ) );
              break;

      case 5: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                                     MPFROM2SHORT ( IDM_5_LINES, TRUE ),
                                     MPFROM2SHORT ( MIA_CHECKED, 0 ) );
              break;

      case 6: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                                     MPFROM2SHORT ( IDM_6_LINES, TRUE ),
                                     MPFROM2SHORT ( MIA_CHECKED, 0 ) );
   }


/*-------- Check the menu item for the new display line count ----------------*/

   switch ( lNewDisplayLines )
   {
      case 2: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT ( IDM_2_LINES, TRUE ),
                               MPFROM2SHORT ( MIA_CHECKED, MIA_CHECKED ) );
              break;

      case 3: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT ( IDM_3_LINES, TRUE ),
                               MPFROM2SHORT ( MIA_CHECKED, MIA_CHECKED ) );
              break;

      case 4: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT ( IDM_4_LINES, TRUE ),
                               MPFROM2SHORT ( MIA_CHECKED, MIA_CHECKED ) );
              break;

      case 5: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT ( IDM_5_LINES, TRUE ),
                               MPFROM2SHORT ( MIA_CHECKED, MIA_CHECKED ) );
              break;

      case 6: WinSendMsg ( hwndMenu, MM_SETITEMATTR,
                               MPFROM2SHORT ( IDM_6_LINES, TRUE ),
                               MPFROM2SHORT ( MIA_CHECKED, MIA_CHECKED ) );
   }

   lDisplayLines = lNewDisplayLines;

/*---------------------- Resize the Window -----------------------------------*/
/*                                                                            */
/*  (1) Hide the Text window, so we don't get a WM_PAINT message while the    */
/*      backup bitmap is destroyed.                                           */
/*                                                                            */
/*  (2) Delete the present backup bitmap.                                     */
/*                                                                            */
/*  (3) Create a new backup bitmap having the desired dimensions.             */
/*                                                                            */
/*  (4) Calculate the new Text Window size.                                   */
/*                                                                            */
/*  (5) Calculate the new Main Window size.                                   */
/*                                                                            */
/*  (6) Show the new text window.                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   WinSetWindowPos( hwndText, 0, 0, 0, 0, 0, SWP_HIDE );/*(1) Hide text window*/

   GpiDestroyPS( hpsText );  /*-----------------------------------------------*/
   DevCloseDC  ( hdcText );  /*(2)Get rid of the old text window backup bitmap*/
   GpiDeleteBitmap( hbmText);/*-----------------------------------------------*/

   CreateTextBitmap ();   /* (3) Create a backup bitmap in the new dimensions */

   lWidth  = rclMain.xRight - 10;
   lHeight   = ( 2 * lTitleY + rclTextClient.yTop );
   lX = 5;
   lY = ( rclMain.yTop - (rclMain.yTop / 5) ) ;

   WinSetWindowPos( hwndText, 0, lX, lY, lWidth, lHeight, /*(4) Show new text */
                          SWP_SHOW | SWP_MOVE | SWP_SIZE );/*     window.  */

   WinQueryWindowPos   ( hwndMainFrame, &swpFrame ) ;      /* (5)             */

   /*
    * Query the window size and position of the Dialog window.
    */
   WinQueryWindowPos( hwndMainDlg, &swpDialog ) ;
   WinQueryWindowPos( hwndText, &swpText ) ;

   if ( lcyDialog > lHeight )
      swpDialog.cy -=  lcyDialog - lHeight;
   else
      swpDialog.cy +=  lHeight - lcyDialog;

   lcyDialog = lHeight;

   /*
    * Set the positioning of the Dialog window.
    */
   WinSetWindowPos(
         hwndMainDlg,                    /* Dialog window handle              */
         0L,                             /* Place hwnd on top of all siblings */
         0L,                             /* Window position, x coordinate     */
         0L,                             /* Window position, y coordinate     */
         swpDialog.cx,                   /* New window size                   */
         swpDialog.cy,                   /* New window size                   */
         SWP_SIZE ) ;                    /* Window positioning options        */

   WinUpdateWindow(hwndMainDlg);

   if ( bTextLoaded ) DisplayTextLine();

   WinInvalidateRect ( hwndText, 0, FALSE );
}


/******************************************************************************
 * Name         : CreateTextBitmap
 *
 * Description  : This function will create a backup bitmap for the text window.
 *
 * Parameters   : none.
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID CreateTextBitmap ( void )
{
   FONTMETRICS       fmText;
   LONG              lModulo;
   SIZEL             sizelText;
   BITMAPINFOHEADER2 bmh2;


/*--- Open a memory device context and create a Micro presentation space -----*/

   hdcText          = DevOpenDC ( hab, OD_MEMORY, "*", 0, NULL, 0 );

   sizelText.cx     = 0;
   sizelText.cy     = 0;
   hpsText          = GpiCreatePS ( hab, hdcText, &sizelText,
                            PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );


/*------------------- Calculate bitmap height --------------------------------*/
/*                                                                            */
/*  The height of the caption window and the line spacing within the window   */
/*  both depend on the height of the font being used.  (1) We will use the    */
/*  default font for the presentation space.  (2) The line spacing (in pels)  */
/*  will be AT LEAST:                                                         */
/*                                                                            */
/*      ( Max Baseline Extent ) + ( External Leading ) + ( Scroll Increment ) */
/*                                                                            */
/*  (3) There is one further condition: the line spacing must be an integer   */
/*  multiple of the scroll increment.  If necessary, increase the above line  */
/*  spacing so that this condition is met.                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   GpiQueryFontMetrics ( hpsText, sizeof(fmText), &fmText );/*(1) default font*/

   lLineSpacing  = fmText.lMaxBaselineExt          /* (2) Lower limit of line */
                 + fmText.lExternalLeading         /*     spacing.            */
                 + CC_SCROLL_INC;

   lModulo = lLineSpacing % CC_SCROLL_INC;
   if ( lModulo )                                /* (3) Adjust spacing to int */
      lLineSpacing += CC_SCROLL_INC - lModulo;   /*     mult of scroll incr.  */

   rclTextClient.yTop =
          lDisplayLines * lLineSpacing;          /* Now calculate height and  */
   lLastLineY = CC_SCROLL_INC + fmText.lMaxDescender;/*position of last line. */


/*-- Create a monochrome bitmap having the dimensions we just calculated -----*/

   bmh2.cbFix             = sizeof (BITMAPINFOHEADER2);
   bmh2.cx                = CC_WIDTH;
   bmh2.cy                = rclTextClient.yTop;
   bmh2.cPlanes           = 1;
   bmh2.cBitCount         = 1;                 /* This is a monochrome bitmap */
   bmh2.ulCompression     = BCA_UNCOMP;
   bmh2.cbImage           = 0;
   bmh2.cxResolution      = 30000;
   bmh2.cyResolution      = 30000;
   bmh2.cclrUsed          = 2;
   bmh2.cclrImportant     = 2;
   bmh2.usUnits           = BRU_METRIC;
   bmh2.usReserved        = 0;
   bmh2.usRecording       = BRA_BOTTOMUP;
   bmh2.usRendering       = BRH_NOTHALFTONED;
   bmh2.cSize1            = 0;
   bmh2.cSize2            = 0;
   bmh2.ulColorEncoding   = BCE_RGB;
   bmh2.ulIdentifier      = 0;
   hbmText                = GpiCreateBitmap ( hpsText, &bmh2, 0L, 0L, NULL );

   GpiSetBitmap   ( hpsText, hbmText );  /* Set the bitmap into the pres space*/
   GpiErase       ( hpsText );           /* Erase the bitmap.                 */
}

/******************************************************************************
 * Name        : ProcuctDlgProc
 *
 * Description : This function controls the product information dialog box.
 *
 * Parameters  : hwnd - Handle for the Include dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY ProductDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   switch( msg )
   {
     case WM_INITDLG :
         /*
          * Add Default Size menu item to system menu of the secondary window.
          */
         WinInsertDefaultSize(hwnd, "~DefaultSize");

         break;

     case WM_CLOSE :
         WinDestroySecondaryWindow( hwnd );         /* Close the Dialog box.          */
         return( 0 );
         break;

     case WM_DESTROY:
         hwndProductInfo = 0;
         break;

     case WM_COMMAND :
         switch( SHORT1FROMMP( mp1 ) )
         {
            case DID_OK :
            case DID_CANCEL:
               WinDestroySecondaryWindow( hwnd );   /* Close the Dialog box.          */
               return( (MRESULT)TRUE);
            break;
         }  /* End of Command Switch */

   }  /* End of Switch */

   return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

} /* End of FileDlgProc */
/******************************************************************************
 * Name         : DisplaySaveasDlg
 *
 * Description  : This function will display the "Save as" dialog box
 *
 * Parameters   : SHORT sMenuItem - menu item that caused this action
 *
 * Return       : return code from WinFileDlg
 *
 ******************************************************************************/
LONG DisplaySaveasDlg ( SHORT sMenuItem )
{

    /*  (a) Ask the user for a file name.  If he responds favorably:    */
    /*  (b) Save the file.                                              */
    /*  (c) Store the name in szCaptionFile.                            */
    /*  (d) Enable the Save menu item .                                 */

    if (WinFileDlg(HWND_DESKTOP, hwndMainDlg, &fdgSaveAs))       /* (a) */
    {
       if ( fdgSaveAs.lReturn == DID_OK )
       {
          if ( iState == ST_PLAYING ) StopPlaying( hwndMainDlg );
          if (!SaveCaptionFile(fdgSaveAs.szFullFile,sMenuItem))  /* (b) */
          {
             strcpy(szCaptionFile,fdgSaveAs.szFullFile);         /* (c) */
             WinSendMsg ( hwndMenu, MM_SETITEMATTR,              /* (d) */
                          MPFROM2SHORT ( IDM_SAVE, TRUE ),
                          MPFROM2SHORT ( MIA_DISABLED, 0 ) );
          }
       }
    }
    return fdgSaveAs.lReturn;
}
