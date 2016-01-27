/*
 *
 *
 *   Module Name: PMBROWSE
 *
 *   OS/2 Work Place Shell Sample Program
 *
 *   Copyright (C) 1993 IBM Corporation
 *
 *       DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *       sample code created by IBM Corporation. This sample code is not
 *       part of any standard or IBM product and is provided to you solely
 *       for  the purpose of assisting you in the development of your
 *       applications.  The code is provided "AS IS", without
 *       warranty of any kind.  IBM shall not be liable for any damages
 *       arising out of your use of the sample code, even if they have been
 *       advised of the possibility of such damages.
 *
 */
#define  INCL_WIN
#define  INCL_DOS
#define  INCL_DOSNMPIPES
#define  INCL_DOSERRORS
#define  INCL_GPILCIDS
#define  INCL_GPIPRIMITIVES
#include <os2.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "browse.ih"
#include "pmbrowse.h"

/*********************/
/* Defined Constants */
/*********************/
#define  MAX_TITLE_LENGTH     128

/***********************/
/* Function Prototypes */
/***********************/
static VOID SetSysMenu( HWND hDlg);

/*
 *
 *  BrowseWndProc
 *
 *  DESCRIPTION:
 *
 *    Processes messages for the Browse-O-Matic client window.
 *
 */
MRESULT EXPENTRY BrowseWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch( msg){

      case WM_CREATE:
         {
            CHAR          pszTitle[ MAX_TITLE_LENGTH];
            PBROWSEDATA   pWindowData;

            pWindowData = (PBROWSEDATA)mp1;
            WinSetWindowPtr( hwnd, QWL_USER, pWindowData);

            /*******************/
            /* Load the object */
            /*******************/
            _LoadObject( pWindowData->somSelf,
                        pWindowData->pszObjectName, &pWindowData->ulFileLength,
                                   &pWindowData->pBuffer, &pWindowData->pLines,
                                             &pWindowData->usNumberOfTextLines,
                                          &pWindowData->usNumberOfTextColumns);

            _GetLinesAndColumns( pWindowData->somSelf, pWindowData->bDisplay,
                        &pWindowData->sTotalLines, &pWindowData->sTotalColumns,
                                             pWindowData->usNumberOfTextLines,
                                            pWindowData->usNumberOfTextColumns,
                                                    pWindowData->ulFileLength);

            /*********************************************/
            /* Get the window handle for the scroll bars */
            /*********************************************/
            pWindowData->hwndVscroll = WinWindowFromID( WinQueryWindow( hwnd,
                                                   QW_PARENT), FID_VERTSCROLL);
            pWindowData->hwndHscroll = WinWindowFromID( WinQueryWindow( hwnd,
                                                   QW_PARENT), FID_HORZSCROLL);

            /*************************/
            /* Save font information */
            /*************************/
            WinSendMsg( hwnd, IDL_SETFONTINFO, (MPARAM)0, (MPARAM)0);

            /*****************/
            /* Get the focus */
            /*****************/
            WinSetFocus( HWND_DESKTOP, hwnd);

            /*****************************************************/
            /* Create the title for the Browse-O-Matic title bar */
            /*****************************************************/
            if( pWindowData->bDisplay == PMBROWSE_HEX_DISPLAY_MODE)
               sprintf( pszTitle, "Object: %s    (%d Bytes)",
                        pWindowData->pszObjectName, pWindowData->ulFileLength);
            else
               sprintf( pszTitle, "Object: %s    (%hd Lines)",
                         pWindowData->pszObjectName, pWindowData->sTotalLines);

            /************************************************/
            /* Register the current open view of the object */
            /************************************************/
            _wpAddToObjUseList( pWindowData->somSelf, &(pWindowData->UseItem));
            _wpRegisterView( pWindowData->somSelf, WinQueryWindow( hwnd,
                                                         QW_PARENT), pszTitle);
         }
         break;

      case WM_CLOSE:
         {
            HWND        hwndFrame;
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr( hwnd, QWL_USER);
            hwndFrame = WinQueryWindow( hwnd, QW_PARENT);

            /*****************************************************/
            /* Save the window info - size, location, and fonts. */
            /*****************************************************/
            WinStoreWindowPos( APPL_NAME, pWindowData->pszApplKey, hwndFrame);

            /**********************************************************/
            /* Free the window data and unload the file system object */
            /**********************************************************/
            if( pWindowData){
               if( pWindowData->pszObjectName)
                  _wpFreeMem( pWindowData->somSelf,
                                           ( PBYTE)pWindowData->pszObjectName);
               _wpFreeMem( pWindowData->somSelf, ( PBYTE)pWindowData);
            }

            _UnloadObject( pWindowData->somSelf, pWindowData->pBuffer,
                                                          pWindowData->pLines);

            _wpDeleteFromObjUseList( pWindowData->somSelf,
                                                        &pWindowData->UseItem);
            /*******************/
            /* Kill the window */
            /*******************/
            WinPostMsg( hwnd, WM_QUIT, 0, 0 );
            WinDestroyWindow( hwndFrame);
         }
         break;

      /*******************************************************************/
      /* The font characteristics have changed. This happens when a font */
      /* from the font palette is dropped on the client window.          */
      /*******************************************************************/
      case WM_PRESPARAMCHANGED:
         {
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr( hwnd, QWL_USER);

            if( (ULONG)mp1 == PP_FONTNAMESIZE){

               /***********************************************/
               /* Get the new font information and redraw the */
               /* window with the new font/font size.         */
               /***********************************************/
               WinSendMsg( hwnd, IDL_SETFONTINFO, (MPARAM)0, (MPARAM)0);
               WinSendMsg( hwnd, WM_SIZE, 0, MPFROM2SHORT(
                       pWindowData->sClientWidth, pWindowData->sClientHeight));
            }
         }
         break;

      case WM_SIZE:
         {
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr( hwnd, QWL_USER);

            /***************************************/
            /* Save client window size information */
            /***************************************/
            pWindowData->sClientWidth = SHORT1FROMMP( mp2);
            pWindowData->sClientHeight = SHORT2FROMMP( mp2);
            pWindowData->sPageHeight = pWindowData->sClientHeight/
                                                      pWindowData->sCharHeight;
            pWindowData->sPageWidth = pWindowData->sClientWidth  /
                                                       pWindowData->sCharWidth;

            /**************************************/
            /* Set the ranges for the scroll bars */
            /**************************************/
            WinPostMsg( pWindowData->hwndVscroll, SBM_SETSCROLLBAR,
                         (MPARAM)pWindowData->sCurrentLine, MPFROM2SHORT( 0,
                         pWindowData->sTotalLines - pWindowData->sPageHeight));
            WinPostMsg( pWindowData->hwndHscroll, SBM_SETSCROLLBAR,
                        (MPARAM)pWindowData->sCurrentColumn, MPFROM2SHORT( 0,
                        pWindowData->sTotalColumns - pWindowData->sPageWidth));

            /*************************************/
            /* Set scroll bar thumb control size */
            /*************************************/
            WinPostMsg( pWindowData->hwndVscroll, SBM_SETTHUMBSIZE,
                                        MPFROM2SHORT( pWindowData->sPageHeight,
                                        pWindowData->sTotalLines), (MPARAM) 0);
            WinPostMsg( pWindowData->hwndHscroll, SBM_SETTHUMBSIZE,
                                      MPFROM2SHORT( pWindowData->sPageWidth,
                                      pWindowData->sTotalColumns), (MPARAM) 0);

            /************************************************************/
            /* If the all the lines and/or columns can fit in the new   */
            /* sized window, set the sCurrentLine and/or sCurrentColumn */
            /* to the top/right side.                                   */
            /************************************************************/
            if( pWindowData->sPageHeight >= pWindowData->sTotalLines)
               pWindowData->sCurrentLine = 0;

            if( pWindowData->sPageWidth >= pWindowData->sTotalColumns)
               pWindowData->sCurrentColumn = 0;

            /********************************************/
            /* Repaint window with new size information */
            /********************************************/
            WinInvalidateRect( hwnd, NULL, FALSE);
         }
         return( 0);

      case WM_PAINT:
         {
            HPS         hps;
            CHAR        pszText[255];
            UINT        nLength = 0;
            RECTL       rect;
            POINTL      point;
            SHORT       sLine;
            SHORT       sTopLine;
            SHORT       sBottomLine;
            SHORT       sLinesFromTop;
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr( hwnd, QWL_USER);

            hps = WinBeginPaint( hwnd, NULLHANDLE, &rect);

            /*******************************************/
            /* Clear the area that needs to be redrawn */
            /*******************************************/
            GpiErase( hps);

            /********************************/
            /* Determine first clipped line */
            /********************************/
            sLinesFromTop = ( pWindowData->sClientHeight - rect.yTop) /
                                                      pWindowData->sCharHeight;
            rect.yTop = pWindowData->sClientHeight -
                                   ( pWindowData->sCharHeight * sLinesFromTop);

            /***************************************************************/
            /* Determine the first position to write at. NOTE: PM handles  */
            /* clipping for point.x < 0. Any characters written before the */
            /* the first column or after the last column are clipped.      */
            /***************************************************************/
            point.y = rect.yTop - pWindowData->sCharHeight +
                                                   pWindowData->sCharDescender;
            point.x = pWindowData->sCurrentColumn*pWindowData->sCharWidth * -1;

            /***********************************************/
            /* Determine the top and bottom lines to write */
            /***********************************************/
            sTopLine    = rect.yTop    / pWindowData->sCharHeight;
            sBottomLine = rect.yBottom / pWindowData->sCharHeight;

            /*********************************************************/
            /* Make sure that we aren't trying to display more lines */
            /* then are available.                                   */
            /*********************************************************/
            if( pWindowData->sPageHeight - sBottomLine + 1 >
                          pWindowData->sTotalLines - pWindowData->sCurrentLine)
               sBottomLine=pWindowData->sPageHeight-(pWindowData->sTotalLines -
                                                pWindowData->sCurrentLine) + 1;

            /********************************/
            /* Redraw the invalid rectangle */
            /********************************/
            for( sLine = sTopLine; sLine >= sBottomLine; sLine--){

               /*******************************************/
               /* Get the line of text or hex information */
               /* that we are going to write.             */
               /*******************************************/
               if( pWindowData->bDisplay == PMBROWSE_HEX_DISPLAY_MODE)
                  nLength = _GetHexLine( pWindowData->somSelf,
                     pWindowData->sPageHeight-sLine+pWindowData->sCurrentLine
                           +1, pWindowData->pBuffer, pWindowData->ulFileLength,
                                                                      pszText);
               else
                  nLength = _GetTextLine( pWindowData->somSelf,
                       pWindowData->sPageHeight-sLine+pWindowData->sCurrentLine
                                             +1, pWindowData->pLines, pszText);

               /******************/
               /* Write the text */
               /******************/
               if( nLength != 0)
                  GpiCharStringAt( hps, &point, nLength, pszText);

               point.y -= pWindowData->sCharHeight;
            }

            WinEndPaint( hps);
         }
         return( 0);

      /*******************************************/
      /* Keyboard support for scrolling and help */
      /*******************************************/
      case WM_CHAR:

         /****************************************/
         /* Show general help if user selects F1 */
         /****************************************/
         if( CHARMSG( &msg)->vkey == VK_F1){
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr(hwnd,QWL_USER);
            _wpDisplayHelp( pWindowData->somSelf, PANEL_MAIN,
                            "browse.hlp");
            return 0;
         }

         if( (CHARMSG( &msg)->fs & KC_KEYUP))
            return 0;

         switch( CHARMSG( &msg)->vkey){

            case VK_UP:
               WinPostMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_LINEUP));
               return 0;

            case VK_DOWN:
               WinPostMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_LINEDOWN));
               return 0;

            case VK_RIGHT:
               /************************************************************/
               /* Ctrl-Right scrolls one page to the right. Right scrolls  */
               /* to the right.                                            */
               /************************************************************/
               if( (CHARMSG( &msg)->fs & KC_CTRL))
                  WinPostMsg( hwnd, WM_HSCROLL, 0,
                                                MPFROM2SHORT(0, SB_PAGERIGHT));
               else
                  WinPostMsg( hwnd, WM_HSCROLL, 0,
                                                MPFROM2SHORT(0, SB_LINERIGHT));
               return 0;

            case VK_LEFT:
               /*********************************************************/
               /* Ctrl-Left scrolls one page to the left. Left scrolls  */
               /* to the Left.                                          */
               /*********************************************************/
               if( (CHARMSG( &msg)->fs & KC_CTRL))
                  WinPostMsg( hwnd, WM_HSCROLL, 0,
                                                MPFROM2SHORT( 0, SB_PAGELEFT));
               else
                  WinPostMsg( hwnd, WM_HSCROLL, 0,
                                                MPFROM2SHORT( 0, SB_LINELEFT));
               return 0;

            case VK_PAGEUP:
               WinPostMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_PAGEUP));
               return 0;

            case VK_PAGEDOWN:
               WinPostMsg( hwnd, WM_VSCROLL, 0, MPFROM2SHORT( 0, SB_PAGEDOWN));
               return 0;

            case VK_HOME:
               {
                  PBROWSEDATA pWindowData;

                  pWindowData = (PBROWSEDATA) WinQueryWindowPtr(hwnd,QWL_USER);

                  /*********************************************************/
                  /* Ctrl-Home positions at the first line, Home positions */
                  /* at the beginning of the current line.                 */
                  /*********************************************************/
                  if( (CHARMSG( &msg)->fs & KC_CTRL)){
                     WinPostMsg( hwnd, WM_VSCROLL, (MPARAM)0,
                                          MPFROM2SHORT( 0, SB_SLIDERPOSITION));
                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                                                           MPFROMSHORT( 0), 0);
                  }
                  else{
                     WinPostMsg( hwnd, WM_HSCROLL, (MPARAM)0,
                                          MPFROM2SHORT( 0, SB_SLIDERPOSITION));
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                                           MPFROMSHORT( 0), 0);
                  }
                  return 0;
               }

            case VK_END:
               {
                  PBROWSEDATA pWindowData;

                  pWindowData = (PBROWSEDATA) WinQueryWindowPtr(hwnd,QWL_USER);

                  /*********************************************************/
                  /* Ctrl-End positions at the last line, End positions at */
                  /* the end of the current line.                          */
                  /*********************************************************/
                  if( (CHARMSG( &msg)->fs & KC_CTRL)){
                     WinPostMsg( hwnd, WM_VSCROLL, (MPARAM)0, MPFROM2SHORT(
                           pWindowData->sTotalLines - pWindowData->sPageHeight,
                                                           SB_SLIDERPOSITION));
                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                      MPFROMSHORT( pWindowData->sTotalLines -
                                                 pWindowData->sPageHeight), 0);
                  }
                  else{
                     WinPostMsg( hwnd, WM_HSCROLL, (MPARAM)0, MPFROM2SHORT(
                          pWindowData->sTotalColumns - pWindowData->sPageWidth,
                                                           SB_SLIDERPOSITION));
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                      MPFROMSHORT( pWindowData->sTotalColumns -
                                                  pWindowData->sPageWidth), 0);
                  }
                  return 0;
               }
         }
         break;

      /******************/
      /* Scroll Up/Down */
      /******************/
      case WM_VSCROLL:
         {
            PBROWSEDATA pWindowData;

            pWindowData = (PBROWSEDATA) WinQueryWindowPtr(hwnd,QWL_USER);

            switch( SHORT2FROMMP( mp2)){

               case SB_LINEUP:
                  if( pWindowData->sCurrentLine > 0){
                     pWindowData->sCurrentLine--;

                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                                         (MPARAM)pWindowData->sCurrentLine, 0);

                     WinScrollWindow( hwnd, 0, pWindowData->sCharHeight * -1,
                               NULL, NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
                  }
                  return( 0);

               case SB_LINEDOWN:
                  if( pWindowData->sCurrentLine < pWindowData->sTotalLines -
                                                     pWindowData->sPageHeight){
                     pWindowData->sCurrentLine++;

                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                                         (MPARAM)pWindowData->sCurrentLine, 0);

                     WinScrollWindow( hwnd, 0, pWindowData->sCharHeight, NULL,
                                     NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
                  }
                  return( 0);

               case SB_PAGEUP:
                  if( pWindowData->sCurrentLine > 0){
                     pWindowData->sCurrentLine = max( 0,
                         pWindowData->sCurrentLine - pWindowData->sPageHeight);
                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                                          (MPARAM)pWindowData->sCurrentLine,0);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);

               case SB_PAGEDOWN:
                  if( pWindowData->sCurrentLine < pWindowData->sTotalLines -
                                                     pWindowData->sPageHeight){
                     pWindowData->sCurrentLine= min( pWindowData->sTotalLines -
                          pWindowData->sPageHeight, pWindowData->sCurrentLine +
                                                     pWindowData->sPageHeight);
                     WinPostMsg( pWindowData->hwndVscroll, SBM_SETPOS,
                                          (MPARAM)pWindowData->sCurrentLine,0);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);

               case SB_SLIDERPOSITION:
                  /***********************************************************/
                  /* Can't change current line if all the text fits in the   */
                  /* window.                                                 */
                  /***********************************************************/
                  if( pWindowData->sTotalLines > pWindowData->sPageHeight){
                     pWindowData->sCurrentLine = SHORT1FROMMP( mp2);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);
            }
         }
         break;


      /*********************/
      /* Scroll Right/Left */
      /*********************/
      case WM_HSCROLL:
         {
            PBROWSEDATA pWindowData;

            pWindowData = ( PBROWSEDATA)WinQueryWindowPtr( hwnd, QWL_USER);

            switch( SHORT2FROMMP( mp2)){

               case SB_LINELEFT:
                  if( pWindowData->sCurrentColumn > 0){
                     pWindowData->sCurrentColumn = max( 0,
                                              pWindowData->sCurrentColumn - 1);
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                        (MPARAM)pWindowData->sCurrentColumn,0);
                     WinScrollWindow( hwnd, pWindowData->sCharWidth, 0, NULL,
                                     NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
                  }
                  return( 0);

               case SB_LINERIGHT:
                  if(pWindowData->sCurrentColumn < pWindowData->sTotalColumns -
                                                      pWindowData->sPageWidth){
                     pWindowData->sCurrentColumn=min(pWindowData->sTotalColumns,
                                              pWindowData->sCurrentColumn + 1);
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                       (MPARAM)pWindowData->sCurrentColumn, 0);
                     WinScrollWindow( hwnd, pWindowData->sCharWidth * -1, 0,
                               NULL, NULL, NULLHANDLE, NULL, SW_INVALIDATERGN);
                  }
                  return( 0);

               case SB_PAGELEFT:
                  if( pWindowData->sCurrentColumn > 0){
                     pWindowData->sCurrentColumn = max( 0,
                        pWindowData->sCurrentColumn - pWindowData->sPageWidth);
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                       (MPARAM)pWindowData->sCurrentColumn, 0);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);

               case SB_PAGERIGHT:
                  if(pWindowData->sCurrentColumn < pWindowData->sTotalColumns -
                                                      pWindowData->sPageWidth){
                     pWindowData->sCurrentColumn=min(pWindowData->sTotalColumns
                       - pWindowData->sPageWidth, pWindowData->sCurrentColumn +
                                                      pWindowData->sPageWidth);
                     WinPostMsg( pWindowData->hwndHscroll, SBM_SETPOS,
                                       (MPARAM)pWindowData->sCurrentColumn, 0);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);

               case SB_SLIDERPOSITION:
                  /***********************************************************/
                  /* Can't change current column if all the text fits in the */
                  /* window.                                                 */
                  /***********************************************************/
                  if( pWindowData->sTotalColumns > pWindowData->sPageWidth){
                     pWindowData->sCurrentColumn = SHORT1FROMMP( mp2);
                     WinInvalidateRect( hwnd, NULL, FALSE);
                  }
                  return( 0);
            }
         }
         break;

      /*****************************/
      /* Get font size information */
      /*****************************/
      case IDL_SETFONTINFO:
         {
            HPS         hps;
            FONTMETRICS fm;
            PBROWSEDATA pWindowData;

            pWindowData = ( PBROWSEDATA)WinQueryWindowPtr( hwnd, QWL_USER);

            hps = WinGetPS( hwnd);
            GpiQueryFontMetrics( hps, sizeof( fm), &fm);
            WinReleasePS( hps);

            /***********************************************************/
            /* The character width is average of the average uppercase */
            /* and average lower case widths.                          */
            /***********************************************************/
            pWindowData->sCharWidth     = (SHORT)fm.lMaxCharInc;
            pWindowData->sCharHeight    = (SHORT)fm.lMaxBaselineExt;
            pWindowData->sCharDescender = (SHORT)fm.lMaxDescender;
         }
         return( 0);

      /*********************************************/
      /* Display no help available message to user */
      /*********************************************/
      case HM_HELPSUBITEM_NOT_FOUND:
         return( ( MRESULT)FALSE);

      /**************************************/
      /* Return name of help for keys panel */
      /**************************************/
      case HM_QUERY_KEYS_HELP:
         return( ( MRESULT)PANEL_HELPKEYS);
   }

   /********************************************************/
   /* Let the default window procedure process the message */
   /********************************************************/
   return( WinDefWindowProc( hwnd, msg, mp1, mp2));
}

/*
 *
 *  OpenFilterProc
 *
 *  DESCRIPTION:
 *
 *    Processes messages for the open file dialog box
 *
 */
MRESULT EXPENTRY
OpenFilterProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   return( WinDefFileDlgProc( hwnd, msg, mp1, mp2));
}

/*
 *
 *  AboutBoxDlgProc
 *
 *  DESCRIPTION:
 *
 *    Processes messages for the product information dialog box
 *
 */
MRESULT EXPENTRY
AboutBoxDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
      case WM_INITDLG:
         SetSysMenu(hwnd);       /* system menu for this dialog  */
         return MRFROMSHORT(FALSE);

      case WM_COMMAND:
           /* no matter what the command, close the dialog */
         WinDismissDlg(hwnd, TRUE);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
   }

   return (MRESULT)NULL;
}

/*
 *
 *  SetSysMenu
 *
 *  DESCRIPTION:
 *
 *    Set dialog box so only move and close are active.
 *
 */
static VOID
SetSysMenu( HWND hDlg)
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
    cItems = (SHORT)WinSendMsg( Mi.hwndSubMenu, MM_QUERYITEMCOUNT,
                                (MPARAM)NULL, (MPARAM)NULL);
    while( cItems--){

        Id = WinSendMsg( Mi.hwndSubMenu, MM_ITEMIDFROMPOSITION
                          , MPFROMLONG(Pos), (MPARAM)NULL);
        switch (SHORT1FROMMR(Id))
        {
        case SC_MOVE:
        case SC_CLOSE:
            Pos++;  /* Don't delete that one. */
            break;
        default:
            WinSendMsg( Mi.hwndSubMenu, MM_DELETEITEM
                      , MPFROM2SHORT((USHORT)Id, TRUE), (MPARAM)NULL);
        }
    }
}

