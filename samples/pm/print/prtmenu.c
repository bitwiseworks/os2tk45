/************************************************************************
 * OS/2 Sample Print Application PRTSAMP
 *
 *  File name  :  prtmenu.c
 *
 *  Description:  This file contains functions which respond to selections
 *                the user makes at the menu bar or the pulldown menus on it.
 *
 *                This source file contains the following functions:
 *
 *                Menu(hwnd, msg, mp1, mp2)
 *                GetEAFileType(hfile, hab)
 *                FileTypeDlgProc(hwnd, msg, mp1, mp2)
 *                MyFileDlgProc(hwnd, msg, mp1, mp2)
 *                MyFontDlgProc(hwnd, msg, mp1, mp2)
 *                ProdInfoDlgProc(hwnd, msg, mp1, mp2)
 *                SetSysMenu(hdlg)
 *                ValidateFilename(pmp, pszFilename, hwndOwner)
 *
 *  Concepts   :  standard dialogs
 *
 *  API's      :  WinQueryWindowULong
 *                WinFileDlg
 *                WinFontDlg
 *                WinDlgBox
 *                WinSendMsg
 *                WinPostMsg
 *                WinCheckMenuItem
 *                GpiSetDefaultViewMatrix
 *                WinAlarm
 *                DosQueryFileInfo
 *                WinCheckButton
 *                WinSetFocus
 *                WinQueryButtonCheckstate
 *                WinDismissDlg
 *                DosOpen
 *                DosRead
 *                DosClose
 *                WinLoadString
 *                WinMessageBox
 *                WinDefFileDlgProc
 *                WinDefFontDlgProc
 *                WinDefDlgProc
 *                WinEnableMenuItem
 *                WinWindowFromID
 *
 *    Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PRTSHLP.H, PMASSERT.H
 *
 *  Copyright (C) 1991-1993 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.
 *************************************************************************/

/* os2 includes */
#define INCL_DEV
#define INCL_DOSFILEMGR
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINBUTTONS
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
#include <os2.h>

/* c language includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <process.h>
#include <memory.h>
#include <sys\types.h>
#include <sys\stat.h>

/* application includes */
#include "prtsamp.h"
#include "prtsdlg.h"
#include "prtshlp.h"
#include "pmassert.h"

/**************************************************************************
 *
 *  Name       : Menu()
 *
 *  Description: Processes commands initiated from the menu bar.
 *
 *  Concepts:    pull-down menus
 *
 *  API's      :  WinQueryWindowULong
 *                WinFileDlg
 *                WinFontDlg
 *                WinDlgBox
 *                WinSendMsg
 *                WinPostMsg
 *                WinCheckMenuItem
 *                GpiSetDefaultViewMatrix
 *                WinAlarm
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  depends on message sent
 *************************************************************************/
MRESULT Menu( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   PMAIN_PARM pmp;
   ULONG      rc;
   BOOL       bOK;
   FIXED      fxWork;
   CHAR       szWork[ LEN_WORKSTRING ];

   /* obtain the main parameter pointer from window words */
   pmp = (PMAIN_PARM)WinQueryWindowULong( hwnd, QWL_USER );

   switch( msg )
   {
   case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
      {
      case IDM_OPEN:
        /* filedlg structure initialized in create.c.
         * See function MyFileDlgProc() which operates while file dialog is up.
         */
         WinFileDlg( HWND_DESKTOP, hwnd, &pmp->filedlg );
         return (MRESULT)NULL;

      case IDM_PAGESETUP:
         /*
          * Check if any printer details changed since the last time the
          * page setup dialog was displayed and then display the dialog.
          */
         QueryPrintQueue( pmp, FALSE );
         QueryForm( pmp, TRUE );
         return (MRESULT)NULL;

      case IDM_PAGEDOWN:
         /* preview the next page of text */
         WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0L, (MPARAM)0L );
         WinPostMsg( pmp->hwndObject, WM_USER_PAGINATE, (MPARAM)hwnd,
                     (MPARAM)FLAGS_SCREEN );
         return (MRESULT)NULL;

      case IDM_PRINT:
         Print(hwnd, pmp);
         return (MRESULT)NULL;

      case IDM_HELPINDEX:
         rc = (ULONG)WinSendMsg(pmp->hwndHelp, HM_HELP_INDEX, MPVOID, MPVOID);
         pmassert( pmp->hab, rc == 0L );
         break;

      case IDM_HELPEXTENDED:
         rc = (ULONG)WinSendMsg(pmp->hwndHelp, HM_EXT_HELP, MPVOID, MPVOID);
         pmassert( pmp->hab, rc == 0L );
         break;

      case IDM_USINGHELP:
         rc = (ULONG)WinSendMsg(pmp->hwndHelp, HM_DISPLAY_HELP, MPVOID, MPVOID);
         pmassert( pmp->hab, rc == 0L );
         break;

      case IDM_PRODINFO:
         rc = WinDlgBox( HWND_DESKTOP,
                        hwnd,
                        (PFNWP)ProdInfoDlgProc,
                        (HMODULE)NULLHANDLE,
                        IDD_PRODINFO,
                        (PVOID)pmp );
         pmassert( pmp->hab, rc != DID_ERROR );
         return (MRESULT)NULL;

      case IDM_SETFONT:
        /* let user pick a font */
        pmassert( pmp->hab, pmp->ulMode == MODE_TEXT );

        /* reassign hpsPrinter because it could have changed */
        pmp->fontdlg.hpsPrinter   = pmp->hpsPrinterInfo;


        WinFontDlg( HWND_DESKTOP, hwnd, &pmp->fontdlg );
        if( pmp->fontdlg.lReturn == DID_OK )
        {
           /*
            * Store an indicator that the font dialog was recently visited.
            * This indicator is referenced in PRTSAMP.C under the
            * WM_NACK_DEFAULT_FONT case where the user is warned that
            * the pagination routine is using a default font.
            */
           pmp->fVisitedFontDialog = TRUE;

           /* requested vector (outline) fonts only */
           pmassert( pmp->hab,
                     pmp->fontdlg.fAttrs.fsFontUse & FATTR_FONTUSE_OUTLINE )

           if( pmp->f )
           {
              /* seek top of file to display document in new font */
              fseek( pmp->f, 0, SEEK_SET );
              /* re-paginate for the screen */
              WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, 0, 0 );
              WinPostMsg( pmp->hwndObject, WM_USER_PAGINATE, (MPARAM)hwnd,
                          (MPARAM)FLAGS_SCREEN );
           }
        }

         return (MRESULT)NULL;


      case IDM_SETUP:
        /*
         * Present queue selection dialog to user (parm 2 == TRUE);
         * Returns TRUE if queue/printer options changed.
         * If changed, then check the form and reset the program with the
         * NEW_MODE processing. See * prtsamp.c for the WM_USER_NEW_MODE case.
         */
        if( QueryPrintQueue( pmp, TRUE ))
        {
            QueryForm( pmp, FALSE );
        }
        return (MRESULT)NULL;


      case IDM_VIEWHALF:
      case IDM_VIEWFULL:
         switch ( SHORT1FROMMP (mp1) )
         {
         case IDM_VIEWHALF:
            WinCheckMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, FALSE );
            WinCheckMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, TRUE  );
            fxWork = MAKEFIXED( 0, 32768 );
            pmp->floatScale = 0.50;
            break;

        case IDM_VIEWFULL:
            WinCheckMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, TRUE  );
            WinCheckMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, FALSE );
            fxWork = MAKEFIXED( 1, 0 );
            pmp->floatScale = 1.00;
            break;
        }

        if( pmp->matlfDefView.fxM11 != fxWork )
        {
          /* apply the new scaling with no Y translation for now */
          pmp->matlfDefView.fxM11 = fxWork;
          pmp->matlfDefView.fxM22 = fxWork;
          pmp->matlfDefView.lM31 = 0;
          pmp->matlfDefView.lM32 = 0;
          bOK = GpiSetDefaultViewMatrix( pmp->hpsClient, 9,
                                 &pmp->matlfDefView, TRANSFORM_REPLACE );
          pmassert( pmp->hab, bOK );

          /*
           * Still need x and Y translation to "home" the document in the
           * client window. "Home" means to position the document such that
           * the top-left corner of the document appears in the top-left
           * corner of the client window.
           *
           * The WM_SIZE case in prtsamp.c sets a proper Y translation so it
           * homes the document
           *
          */
          WinSendMsg( hwnd, WM_SIZE, (MPARAM)0L, (MPARAM)0L );
        }
        else
        {
          /* viewmatrix ok already */
          WinAlarm( HWND_DESKTOP, WA_NOTE );
        }
        return (MRESULT)NULL;

      case IDM_AUTHORS:
        /* display message box with list of program's authors */
        WinLoadString(pmp->hab, (HMODULE)NULLHANDLE,
                      PROGRAM_AUTHORS, LEN_WORKSTRING, szWork);
        WinMessageBox( HWND_DESKTOP,
                       HWND_DESKTOP,
                       szWork,
                       pmp->pszTitle,
                       (USHORT)0,
                       MB_OK | MB_MOVEABLE | MB_CUANOTIFICATION | MB_APPLMODAL);
        return (MRESULT)NULL;

      }
      break;

   }
   return (MRESULT)NULL;
}   /*  end of Menu()  */


/**************************************************************************
 *
 *  Name       : GetEAFileType
 *
 *  Description: Gets the value for the extended attribute called ".TYPE".
 *               For more information, see the EA Sample Program and
 *               BSEDOS.H.
 *
 *  Concepts:    extended attributes
 *
 *  API's      : DosQueryFileInfo
 *
 *  Parameters :  hfile = file handle to query
 *
 *  Return     :  the extended attribute file type MODE_* as a ULONG
 *************************************************************************/
ULONG GetEAFileType( HFILE hfile, HAB hab)
{
   ULONG       ulMode;
   EAOP2       eaop2;
   PGEA2LIST   pgea2list;
   PFEA2LIST   pfea2list;
   PGEA2       pgea2;
   PFEA2       pfea2;
   PBYTE       pEA;

   /* default to unknown */
   ulMode = MODE_UNKNOWN;

   /* create one list item in GEA list */
   pgea2 = (PGEA2) calloc ( 1, 10 );
   pgea2->oNextEntryOffset = 0;
   pgea2->cbName           = 5;
   memcpy( pgea2->szName, ".TYPE", 5 );

   /* create the list with one GEA item */
   pgea2list = (PGEA2LIST) calloc ( 1, 14 );
   pgea2list->cbList = 10;
   memcpy( &pgea2list->list, pgea2, 10 );

   /* create the list where DosQueryFileInfo places result */
   pfea2list = (PFEA2LIST) calloc ( 1, 64 );
   pfea2list->cbList = 64;

   /* put both lists in a eaop2 structure */
   eaop2.fpGEA2List = pgea2list;
   eaop2.fpFEA2List = pfea2list;

   /* get info */
   if( 0 == DosQueryFileInfo( hfile, FIL_QUERYEASFROMLIST, &eaop2, 64 ) )
   {
     /* is there a ".TYPE" EA? */
      pfea2 = pfea2list->list;
      if( pfea2->cbValue > 0 )
      {
         /* check that EA is .TYPE */
         pEA = pfea2->szName;
         pmassert(hab, !strcmp((PSZ)pEA, ".TYPE") );

         /* set pointer to EA value - increment over EA name, null and flags */
         pEA = pfea2->szName + pfea2->cbName + 1 + 10;

         /* try to set filemode from EA value */
         if (! strcmp( pEA, "Assembler Code" ))     ulMode = MODE_TEXT;
         if (! strcmp( pEA, "BASIC Code" ))         ulMode = MODE_TEXT;
         if (! strcmp( pEA, "Bitmap" ))             ulMode = MODE_BITMAP;
         if (! strcmp( pEA, "C Code" ))             ulMode = MODE_TEXT;
         if (! strcmp( pEA, "COBOL Code" ))         ulMode = MODE_TEXT;
         if (! strcmp( pEA, "DOS Command File" ))   ulMode = MODE_TEXT;
         if (! strcmp( pEA, "FORTRAN Code" ))       ulMode = MODE_TEXT;
         if (! strcmp( pEA, "Icon" ))               ulMode = MODE_BITMAP;
         if (! strcmp( pEA, "OS/2 Command File" ))  ulMode = MODE_TEXT;
         if (! strcmp( pEA, "Pascal Code" ))        ulMode = MODE_TEXT;
         if (! strcmp( pEA, "Pointer" ))            ulMode = MODE_BITMAP;
         if (! strcmp( pEA, "Metafile" ))           ulMode = MODE_METAFILE;
         if (! strcmp( pEA, "Plain Text" ))         ulMode = MODE_TEXT;
      }
   }
   free( pfea2list );
   free( pgea2list );
   free( pgea2 );
   return ulMode;
}     /*  end of GetEAFileType()  */


/**************************************************************************
 *
 *  Name       : FileTypeDlgProc
 *
 *  Description: Window procedure for the dialog that lets the user
 *               indicate the file type, presented as a child dialog
 *               of the standard file dialog.
 *
 *  Concepts:    window words
 *
 *  API's      : WinSetWindowULong
 *               WinCheckButton
 *               WinSetFocus
 *               WinQueryButtonCheckstate
 *               WinDismissDlg
 *               WinDefDlgProc
 *
 *  Parameters : sets check according to pmp->ulNextMode, defaults to
 *               text type
 *               pmp is passed to this dialog at WM_INITDLG time and stored
 *               in dialog window words.
 *
 *  Return     : sets pmp->ulNextMode according to user's selection
 *************************************************************************/
ULONG APIENTRY FileTypeDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   PMAIN_PARM  pmp;
   ULONG       id;

   switch( msg )
   {
   case WM_INITDLG:
      pmp = (PMAIN_PARM) mp2;
      WinSetWindowULong( hwnd, QWL_USER, (ULONG)mp2 );

    /* set check according to pmp->ulNextMode */
      switch( pmp->ulNextMode )
      {
      case MODE_BITMAP:
         id = IDC_CK_BITMAP;
         break;
      case MODE_METAFILE:
         id = IDC_CK_METAFILE;
         break;
      default:
         id = IDC_CK_TEXT;
         break;
      }

      WinCheckButton( hwnd, id, TRUE );
      WinSetFocus( HWND_DESKTOP, hwnd );
      return 1L;

   case WM_COMMAND:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      switch( SHORT1FROMMP( mp1 ))
      {
      case DID_OK:
      /* get checked button */
         if( WinQueryButtonCheckstate( hwnd, IDC_CK_TEXT ))
         {
            pmp->ulNextMode = MODE_TEXT;
         }
         if( WinQueryButtonCheckstate( hwnd, IDC_CK_BITMAP ))
         {
            pmp->ulNextMode = MODE_BITMAP;
         }
         if( WinQueryButtonCheckstate( hwnd, IDC_CK_METAFILE ))
         {
            pmp->ulNextMode = MODE_METAFILE;
         }
         WinDismissDlg( hwnd, DID_OK );
         break;

    case DID_CANCEL:
      /* return */
        WinDismissDlg( hwnd, DID_CANCEL );
        break;
      }
      return 0L;

   }
   return (ULONG)WinDefDlgProc( hwnd, msg, mp1, mp2 );
}  /*  end of FileTypeDlgProc()  */


/************************************************************************
 *
 * Name:     MyFileDlgProc
 *
 * Description: Window procedure that subclasses the WinFileDlg window
 *              procedure, processes/validates filenames the user picked
 *              from the WinFileDlg.
 *              The address of this function has been specified in the
 *              pmp->FILEDLG structure before invoking WinFileDlg().
 *              Handle WM_HELP from font dialog.
 *
 *  Concepts:    open/saveas file dialog
 *
 *  API's      : WinQueryWindowULong
 *               WinDlgBox
 *               WinPostMsg
 *               DosOpen
 *               DosRead
 *               DosClose
 *               WinLoadString
 *               WinMessageBox
 *               WinSendMsg
 *               WinDefFileDlgProc
 *
 * Parameters:  Regular window procedure arguments.
 *              Can get pmp from filedlg.ulUser.
 *              Can get to filedlg structure by getting its pointer
 *              from dialog window words.
 *
 * Result:      presents the filetype dialog as a child of the filedlg
**************************************************************************/
MRESULT APIENTRY MyFileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   ULONG       ulPanel;
   PMAIN_PARM  pmp;
   PFILEDLG    pfiledlg;

   switch( msg )
   {
   case FDM_VALIDATE:
      /* get pointer to fontdlg structure from window words of this dialog */
      pfiledlg = (PFILEDLG)WinQueryWindowULong( hwnd, QWL_USER );
      /* filedlg structure has a pmp pointer stored in the user dword */
      pmp = (PMAIN_PARM)pfiledlg->ulUser;
      /* validate filename and use if ok */
      return ( (MRESULT)ValidateFilename( pmp, (PSZ)mp1, hwnd) );


   case WM_HELP:
     /* get pointer to fontdlg structure from window words of this dialog */
      pfiledlg = (PFILEDLG) WinQueryWindowULong( hwnd, QWL_USER );
     /* filedlg structure has a pmp pointer stored in the user dword */
      pmp = (PMAIN_PARM) pfiledlg->ulUser;

      if( pmp->hwndHelp )
      {
         ulPanel = PANEL_FILEDIALOG;
         WinSendMsg( pmp->hwndHelp, HM_DISPLAY_HELP,
                   (MPARAM)&ulPanel, (MPARAM)HM_RESOURCEID );
         return (MRESULT)NULL;
      }
      break;

   default:
      break;
   }
   return WinDefFileDlgProc( hwnd, msg, mp1, mp2 );
}   /*  end of MyFileDlgProc()  */



/************************************************************************
*
* Name: MyFontDlgProc
*
* Description:  Window procedure that subclasses the WinFontDlg window
*               procedure.  Exists to handle WM_HELP message from the
*               standard font dialog.
*
* Parameters:   Regular window procedure arguments.
*               Can get pmp from fontdlg.ulUser.  Can get to fontdlg
*               structure by getting its pointer from dialog window words.
*
* returns: depends on message sent
*
**************************************************************************/
MRESULT APIENTRY MyFontDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   PMAIN_PARM  pmp;
   PFONTDLG    pfontdlg;
   ULONG       ulPanel;

   switch( msg )
   {
   case WM_HELP:
     /* get pointer to filedlg structure from window words of this dialog */
      pfontdlg = (PFONTDLG) WinQueryWindowULong( hwnd, QWL_USER );
     /* fontdlg structure has a pmp pointer stored in the user dword */
      pmp = (PMAIN_PARM) pfontdlg->ulUser;

      if( pmp->hwndHelp )
      {
         ulPanel = PANEL_FONTDIALOG;
         WinSendMsg( pmp->hwndHelp, HM_DISPLAY_HELP,
                   (MPARAM)&ulPanel, (MPARAM)HM_RESOURCEID );
         return (MRESULT)NULL;
      }
      break;
   }
   return WinDefFontDlgProc( hwnd, msg, mp1, mp2 );
}   /*  end of MyFontDlgProc()  */


/*********************************************************************
*  Name : ProdInfoDlgProc
*
*  Description : Processes all messages sent to the Product
*                Information dialog box.
*
*  Concepts : Called for each message sent to the Product
*             Information dialog box.  The Product
*             Information box only has a button control so
*             this routine only processes WM_COMMAND
*             messages.  Any WM_COMMAND posted must have come
*             from the Ok button so we dismiss the dialog
*             upon receiving it.
*
*  API's : WinDismissDlg
*          WinDefDlgProc
*
* Parameters   : hwnd - Window handle to which message is addressed
*                msg - Message type
*                mp1 - First message parameter
*                mp2 - Second message parameter
*
*  Returns : Dependent upon message sent
****************************************************************/
MRESULT EXPENTRY ProdInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
      case WM_INITDLG:
         SetSysMenu(hwnd);       /* system menu for this dialog  */
         return MRFROMSHORT(FALSE);

      case WM_COMMAND:
         /*
          * No matter what the command, close the dialog
          */
         WinDismissDlg(hwnd, TRUE);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
   }
   return (MRESULT)NULL;
}   /*  end of ProdInfoDlgProc()  */


/**************************************************************************
 *
 *  Name       : SetSysMenu(hDlg)
 *
 *  Description: Sets only the Move and Close items of the system menu
 *
 *  Concepts:  Any dialog box is free to call this routine, to edit
 *             which menu items will appear on its System Menu pulldown.
 *
 *  API's      :  WinWindowFromID
 *                WinSendMsg
 *
 *  Parameters :  hDlg     = window handle of the dialog
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SetSysMenu(HWND hDlg)
{
    HWND     hSysMenu;
    MENUITEM Mi;
    ULONG    Pos;
    MRESULT  Id;
    SHORT    cItems;

    /******************************************************************/
    /*  We only want Move and Close in the system menu.               */
    /******************************************************************/

    hSysMenu = WinWindowFromID(hDlg, FID_SYSMENU);
    WinSendMsg( hSysMenu, MM_QUERYITEM
              , MPFROM2SHORT(SC_SYSMENU, FALSE), MPFROMP((PCH) & Mi));
    Pos = 0L;
    cItems=(SHORT)WinSendMsg(Mi.hwndSubMenu, MM_QUERYITEMCOUNT, MPVOID, MPVOID);
    while (cItems--)
    {
        Id = WinSendMsg( Mi.hwndSubMenu, MM_ITEMIDFROMPOSITION
                          , MPFROMLONG(Pos), MPVOID);
        switch (SHORT1FROMMR(Id))
        {
        case SC_MOVE:
        case SC_CLOSE:
            Pos++;  /* Don't delete that one. */
            break;
        default:
            WinSendMsg( Mi.hwndSubMenu, MM_DELETEITEM
                      , MPFROM2SHORT((USHORT)Id, TRUE), MPVOID);
        }
    }
}   /*  End of SetSysMenu  */


/************************************************************************
 *
 * Name:     ValidateFilename
 *
 * Description: Validates a filename, gets the type and uses it
 *
 *  API's      : DosOpen
 *               DosRead
 *               DosClose
 *               WinLoadString
 *               WinMessageBox
 *               WinSendMsg
 *
 * Parameters:  PMAIN_PARM       pointer to global data structure
 *              PSZ              file name to validate
 *              HWND             owner window, if NULL do not show type dialog
 *
 * Result:      TRUE if file is ok and used, otherwise FALSE
**************************************************************************/
BOOL ValidateFilename( PMAIN_PARM pmp, PSZ pszFilename, HWND hwndOwner)
{
   BOOL        bOK, bResult;
   HFILE       hfile;
   ULONG       ulAction, rc, ulMode, ulBytes;
   PCHAR       pch;
   char        szWork[ LEN_WORKSTRING ];

   /* check file exists */
   rc = DosOpen(  pszFilename,
                  &hfile,
                  &ulAction,
                  0,
                  FILE_NORMAL,
                  FILE_OPEN,
                  OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                  NULL );
   if( rc == 0 )
   {
     /* file exists; what kind of file is this according to the EAs? */
     ulMode = GetEAFileType( hfile, pmp->hab );

     if( ulMode == MODE_UNKNOWN )
     {
       /* not sure. Look at file extension to determine file type. */
       pch = pszFilename + strlen( pszFilename );
       pch = max( pszFilename, pch-4 );
       if( 0 == stricmp( pch, ".BMP" )) ulMode = MODE_BITMAP;
       if( 0 == stricmp( pch, ".TXT" )) ulMode = MODE_TEXT;
       if( 0 == stricmp( pch, ".MET" )) ulMode = MODE_METAFILE;
       if( 0 == stricmp( pch, ".SPL" )) ulMode = MODE_METAFILE;
     }

     if( ulMode == MODE_UNKNOWN )
     {
       /* still not sure; peek at data in the file */
       if( 0 == DosRead( hfile, szWork, 16, &ulBytes ))
       {
         if( 0==strncmp(szWork,"BM",2 ) || 0==strncmp(szWork,"BA", 2 ))
         {
            ulMode = MODE_BITMAP;
         }
         else if( szWork[3] == 0xA8  &&  szWork[4] == 0xA8  )
         {
            ulMode = MODE_METAFILE;
         }
       }
     }

     /* set "best-guess" mode into main params for FileTypeDlgProc() */
     pmp->ulNextMode = ulMode;

     /* show dialog only if a owner window handle is given */
     rc = DID_OK;
     if (hwndOwner)
     {
        /* confirm file type with user via the file type dialog */
        rc = WinDlgBox( HWND_DESKTOP,
                        hwndOwner,
                        (PFNWP)FileTypeDlgProc,
                        (HMODULE)NULLHANDLE,
                        IDD_FILETYPE,
                        (PVOID)pmp );
        pmassert( pmp->hab, rc != DID_ERROR );
     }

     if( rc == DID_OK )
     {
        /* set up for new operating mode on new file */
        strcpy( pmp->szNextFilename, pszFilename );
        WinPostMsg( pmp->hwndClient, WM_USER_NEW_MODE, MPVOID, MPVOID);
        bResult = TRUE;
     }
     else
     {
        bResult = FALSE;
     }
     DosClose( hfile );
   }
   else
   {
     /* file not found */
     bOK = WinLoadString( pmp->hab, (HMODULE)NULLHANDLE,
                          ERRMSG_FILE_NOT_FOUND, LEN_WORKSTRING, szWork );
     pmassert( pmp->hab, bOK );

     WinMessageBox( HWND_DESKTOP,
                    pmp->hwndFrame,
                    szWork,
                    pmp->pszTitle,
                    (USHORT)0,
                    MB_OK | MB_MOVEABLE | MB_CUACRITICAL | MB_APPLMODAL);

     bResult = FALSE;
   }
   return bResult;
} /* end of ValidateFilename */

/***************************  End of prtmenu.c  ****************************/
