/**************************************************************************
 *  File name  :  img_file.c
 *
 *  Description:  This module contains the code for handling the functions
 *                offered on the File submenu. These functions are called
 *                from the MainCommand() routine.
 *
 *                FileOpen(mp2)
 *                FileLoadImage()
 *
 *  Concepts   :  file input and output
 *
 *  API's      :  WinLoadString
 *                WinFileDlg
 *                WinSetPointer
 *                WinSendMsg
 *                WinInvalidateRect
 *                DosRead
 *                DosClose
 *                WinIsChild
 *                WinSetParent
 *                WinDefFileDlgProc
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDLIB.H, IMG_MAIN.H, IMG_XTRN.H,
 *                IMG_HELP.H
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/
/*
 *  Include files, macros, defined constants, and externs
 */
#define INCL_WINSCROLLBARS
#define INCL_WINFRAMEMGR
#define INCL_WINPOINTERS
#define INCL_WINSTDFILE
#include <os2.h>
#include <string.h>
#include <stdlib.h>

#include "img_main.h"
#include "img_help.h"
#include "img_xtrn.h"

/*
 *  Static Variables & local defines
 */
#define CCHEXTENSION 6
#define MAX_IMAGE_SIZE 0xFFFF

/* fully-qualified filename from open dialog */
CHAR szFullFile[CCHMAXPATH] = {0};

/*
 *  Entry point declarations
 */
BOOL             FileLoadImage(VOID);
MRESULT EXPENTRY ImageOpenFilterProc(HWND hwnd, ULONG message,
                  MPARAM mp1, MPARAM mp2 );
#ifdef PORT_32
/**************************************************************************
 *
 *  Name       : FileOpen(mp2)
 *
 *  Description: Processes the File menu's Open item.
 *
 *  Concepts   : Called whenever New from the File menu is selected.
 *               Calls the standard file open dialog to get the
 *               file name.  The file name is passed onto DosOpen
 *               which returns the handle to the file.  The file
 *               input procedure is called and then the file handle
 *               is closed.
 *
 *  API's      : WinLoadString
 *               WinFileDlg
 *               WinSetPointer
 *               WinSendMsg
 *               WinInvalidateRect
 *
 *  Parameters : mp2  = message parameter 2
 *
 *  Return     : TRUE  - if file successfully loaded
 *               FALSE - if it failed to load the file
 *
 *************************************************************************/
BOOL FileOpen(MPARAM mp2)
{
    BOOL fSuccess = FALSE;
    PCH  pch;
    CHAR szTitle[MESSAGELEN], szButton[MESSAGELEN];
    CHAR szExtension[CCHEXTENSION];                       /* e.g.: *.IMG */

   /*
    * set up structure to pass to standard file dialog
    */
    vfdg.cbSize = sizeof(FILEDLG);
    vfdg.fl = FDS_CENTER | FDS_HELPBUTTON | FDS_OPEN_DIALOG;

    if (!WinLoadString(vhab,
                       0,
                       IDS_OPEN,
                       MESSAGELEN,
                       szTitle))
    {
       MessageBox(vhwndFrame, IDMSG_CANNOTLOADSTRING, 0, MB_OK | MB_ERROR,
                  TRUE);
       return FALSE;
    }
    vfdg.pszTitle = szTitle;                            /* dialog title */

    if (!WinLoadString(vhab,
                       (HMODULE)0,
                       IDS_OPEN,
                       MESSAGELEN,
                       szButton))
    {
        MessageBox(vhwndFrame, IDMSG_CANNOTLOADSTRING, 0, MB_OK | MB_ERROR,
                   TRUE);
        return FALSE;
    }
    vfdg.pszOKButton = szButton;                 /* <Enter> button text */

    vfdg.ulUser = 0L;
    vfdg.pfnDlgProc = ImageOpenFilterProc;
    vfdg.lReturn = 0L;
    vfdg.lSRC = 0L;
    vfdg.hMod = (HMODULE)NULL;
    vfdg.usDlgId = FILEOPEN;
    vfdg.x = (SHORT)0;
    vfdg.y = (SHORT)0;

    /* set up EA type list & initial type */
    vfdg.pszIType = (PSZ)NULL;
    vfdg.papszITypeList = (PAPSZ)NULL;

    /* set up drive list & initial drive */
    vfdg.pszIDrive = (PSZ)NULL;
    vfdg.papszIDriveList = (PAPSZ)NULL;
    vfdg.sEAType = 0;
    vfdg.papszFQFilename = (PAPSZ)NULL;
    vfdg.ulFQFCount = 0L;

   /*
    * get the default file extension (i.e. *.IMG)
    */
    if (!WinLoadString(vhab, 0, IDS_FILEOPENEXTENSION,
                       CCHEXTENSION, szExtension))
    {
        MessageBox(vhwndFrame, IDMSG_CANNOTLOADSTRING, 0,
                   MB_OK | MB_ERROR, TRUE);
        return FALSE;
    }

   /*
    * if dialog has been previously invoked, then ensure the
    * dialog is brought with the last user-selected directory
    */
    if (*szFullFile)
    {
        pch = strrchr(szFullFile, '\\');
        strcpy(++pch, szExtension);
        strcpy(vfdg.szFullFile, szFullFile);
    }
    else
        strcpy(vfdg.szFullFile, szExtension);

   /*
    * Invoke the standard file dialog and get a file.
    */
    if (!WinFileDlg(HWND_DESKTOP, vhwndFrame, (PFILEDLG)&vfdg))
    {
        MessageBox(vhwndFrame, IDMSG_CANNOTRUNFILEOPEN, 0,
                            MB_OK | MB_ERROR, TRUE);
        return FALSE;
    }
    WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);

   /*
    *  Upon successful return of a file, open it for reading.
    */
    if (vfdg.lReturn == DID_OK)
    {
        strcpy(szFullFile, vfdg.szFullFile);

       /*
        * attempt to load selected image
        */
        WinSetPointer(HWND_DESKTOP, vhptrWait);
        fSuccess = FileLoadImage();
        WinSetPointer(HWND_DESKTOP, vhptrArrow);

       /*
        * Report error if image cannot be loaded.
        */
        if (!fSuccess)
        {
            MessageBox(vhwndFrame, IDMSG_CANNOTOPENINPUTFILE, 0,
                       MB_OK | MB_ERROR, FALSE);
            return FALSE;
        }

        /* Update title with new filename.     */
        UtilUpdateTitleText(vhab, vhwndFrame, szFullFile);

        /* reset scrollbars */
        WinSendMsg(vhwndVScroll, SBM_SETPOS, (MPARAM)NULL, (MPARAM)NULL);
        WinSendMsg(vhwndHScroll, SBM_SETPOS, (MPARAM)NULL, (MPARAM)NULL);

        WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);

       /* This routine currently doesn't use the mp2 parameter but       *\
        *  it is referenced here to prevent an 'Unreferenced Parameter'
       \*  warning at compile time.                                      */
        mp2;
    }
}   /*  End of FileOpen */

/**************************************************************************
 *
 *  Name       : FileLoadImage()
 *
 *  Description: Loads an image into storage
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window as a result of
 *               selecting the 'Open...' item on the 'File'
 *               pulldown.  It calls SizeCalculateMaxWindow() and
 *               DrawImage().
 *
 *                - determine the size of the image to be loaded
 *                - allocate allocation storage to contain the
 *                  image
 *                - load image into allocated storage
 *
 *  API's      : WinSendMsg
 *               DosRead
 *               DosClose
 *               WinIsChild
 *               WinSetParent
 *
 *  Parameters : [none]
 *
 *  Return     : TRUE  - if image successfully loaded
 *               FALSE - if it failed to load the image
 *
 *************************************************************************/
BOOL FileLoadImage(void)
{
   ULONG  ulBufSiz;   /* size of buffer required for the image */
   HFILE  hfile;
#if (defined(PORT_S132) || defined(PORT_32))
   ULONG cByte;
#else
   USHORT cByte;
#endif
   BOOL fFailed = FALSE;
   CHAR szTmp[10];        /* to hold image file header         */

   /*
    * Locate the selected file and determine its size, so that enough
    * storage can be allocated.
    */
   if (!(ulBufSiz = UtilFindFileSize(szFullFile)))
      return FALSE;

   /*
    * free any image storage buffer already in use
    */
   if (vfImgLoaded)
#if (defined(PORT_16) || defined(PORT_S132))
      UtilMemoryFree(SELECTOROF(vpbImgBuf));
#else
      UtilMemoryFree(vpbImgBuf);
#endif
    /* allocate memory for image */
   if (!UtilMemoryAllocate((USHORT)ulBufSiz, &vpbImgBuf))
      return FALSE;

   /*
    * open the image-data file
    */
   if (!UtilGetFileHandle(szFullFile, &hfile))
   {
      vfDetail = FALSE;
      vfImgLoaded = FALSE;
#if (defined(PORT_16) || defined(PORT_S132))
      UtilMemoryFree(SELECTOROF(vpbImgBuf));
#else
      UtilMemoryFree(vpbImgBuf);
#endif
      return FALSE;
   }
   /*
    * read the image width and height values from the image-file header
    * note: the image width is in bytes & the image height is in pels
    */
   DosRead(hfile, (PVOID)szTmp, 4L, &cByte);
   szTmp[4] = '\0';
   vsizlImg.cx = atol(szTmp);

   /* check for comma in header */
   DosRead(hfile, (PVOID)szTmp, 1L, &cByte);
   szTmp[1] = '\0';

   if (strcmp(szTmp, ","))
       fFailed = TRUE;
   else
   {
       /* read the image height field */
       DosRead(hfile, (PVOID)szTmp, 4L, &cByte);
       szTmp[4] = '\0';
       vsizlImg.cy = atol(szTmp);
   }

   /*
    * if header not correct format, fail the load
    */
   if (fFailed || vsizlImg.cx == 0L || vsizlImg.cy == 0L)
   {
       DosClose(hfile);
       vfDetail = FALSE;
       vfImgLoaded = FALSE;
#if (defined(PORT_16) || defined(PORT_S132))
       UtilMemoryFree(SELECTOROF(vpbImgBuf));
#else
       UtilMemoryFree(vpbImgBuf);
#endif
       return FALSE;
   }

   /*
    * adjust the image height (if necessary) to keep within 64KB limit
    */
   if (ulBufSiz >= MAX_IMAGE_SIZE)
      vsizlImg.cy = MAX_IMAGE_SIZE / (vsizlImg.cx + 3);

   /*
    * convert the image-width value to pels
    */
   vsizlImg.cx *= 8;

   /*
    * transfer the image data to application storage
    */
   DosRead(hfile, (PVOID)vpbImgBuf, ulBufSiz, &cByte);

   DosClose(hfile);

   /*
    * Invoke the DrawImage function to define a bitmap containing the
    * image.
    */
   PaintDrawImage();
   vfDetail = FALSE;
   vfImgLoaded = TRUE;

   /*
    * As 'Non-Detail' mode is set, scrolling should be disabled. The scroll
    * bars are made invisible by ensuring that they are no longer
    * object windows.
    */
   if (WinIsChild(vhwndVScroll, vhwndFrame))
   {
       WinSetParent(vhwndVScroll, HWND_OBJECT, FALSE);
       WinSetParent(vhwndHScroll, HWND_OBJECT, FALSE);
       WinSendMsg(vhwndFrame,
                  WM_UPDATEFRAME,
                  MPFROMLONG(FCF_VERTSCROLL | FCF_HORZSCROLL),
                  (MPARAM)NULL);
   }
   return TRUE;

}  /* End of FileLoadImage */

/**************************************************************************
 *
 *  Name       : ImageOpenFilterProc
 *
 *  Description: Filters all help messages to the open dialog
 *
 *  Concepts   : Needed to display help from the Open dialog
 *
 *  API's      : WinDefFileDlgProc
 *
 *  Parameters : hwnd   = window handle
 *               msg    = message code
 *               mp1    = first message parameter
 *               mp2    = second message parameter
 *
 *  Return     : same return as WinDefFileDlgProc()
 *
 *************************************************************************/
MRESULT EXPENTRY ImageOpenFilterProc(HWND hwnd,
                                     ULONG message,
                                     MPARAM mp1,
                                     MPARAM mp2 )
{
   if(message == WM_HELP)
   {
       DisplayHelpPanel(HID_FS_OPEN_DLG_HELP_PANEL);
       return (MRESULT)FALSE ;
   }
   return WinDefFileDlgProc( hwnd, message, mp1, mp2 ) ;
} /* End of ImageOpenFilterProc  */
#endif  /*  PORT_32  */
#ifdef PORT_16  /*  This is a dummy function for the 16-bit compile.  */
BOOL FileOpen(MPARAM mp2)
{
   return FALSE;
   mp2;
}  /* End of dummy FileOpen  */
#endif
/***************************  End of img_file.c  *************************/
