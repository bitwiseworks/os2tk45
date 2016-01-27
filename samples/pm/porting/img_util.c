/**************************************************************************
 *  File name  :  img_util.c
 *
 *  Description:  This module contains the code for allocating/freeing
 *                memory for the image data, and for image file-related
 *                options.  It contains these functions:
 *
 *                UtilMemoryAllocate(usBufSiz, ppByte)
 *                UtilMemoryFree(pByte)
 *                UtilFindFileSize(pszFilename)
 *                UtilGetFileHandle(pszFilename, phfile)
 *                UtilUpdateTitleText(hab, hwnd, pszFullFile)
 *
 *  Concepts   :  32-bit memory allocation
 *
 *  API's      :  DosAllocSeg  [in 16 bit]  or DosAllocMem  [in 32 bit]
 *                DosFreeSeg   [in 16 bit]  or DosFreeMem   [in 32 bit]
 *                DosFindFirst
 *                DosOpen
 *                WinLoadString
 *                WinSetWindowText
 *                WinWindowFromID
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, IMG_MAIN.H, IMG_XTRN.H
 *
 *  Copyright (C) 1991, 1994 IBM Corporation
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

#define INCL_DOSERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINSWITCHLIST
#define INCL_WINSTDFILE
#include <os2.h>
#include <string.h>

#include "img_main.h"
#include "img_xtrn.h"

#ifdef PORT_S116
LONG FAR pascal UtilFindFileSize(PSZ pszFilename);
BOOL FAR pascal UtilMemoryAllocate(USHORT cbSize, PBYTE FAR *ppByte);
BOOL FAR pascal UtilGetFileHandle(PSZ pszFilename, PHFILE phfile);
VOID FAR pascal UtilUpdateTitleText(HAB hab, HWND hwnd, PSZ pszFullFile);
VOID FAR pascal UtilMemoryFree(SEL sel);
#endif
/**************************************************************************
 *
 *  Name       : UtilMemoryAllocate(usBufSiz, ppByte)
 *
 *  Description: Allocates memory for the image
 *
 *  Concepts   : Routine is called whenever memory needs to be
 *               allocated to store a new image.
 *
 *  API's      :  DosAllocSeg  [in 16 bit]  or DosAllocMem  [in 32 bit]
 *
 *  Parameters : usBufSiz = how many bytes to allocate
 *               ppByte   = pointer to the first byte of the memory block
 *
 *  Return     : TRUE  - if memory successfully allocated
 *               FALSE - if it failed to allocate memory
 *
 *************************************************************************/
#if (defined(PORT_16) || defined(PORT_32))
BOOL UtilMemoryAllocate(USHORT usBufSiz, PBYTE FAR *ppByte)
#else
BOOL FAR pascal UtilMemoryAllocate(USHORT usBufSiz, PBYTE FAR *ppByte)
#endif
{
#if (defined(PORT_16) || defined(PORT_S116))
    SEL sel;

    if (DosAllocSeg(usBufSiz, &sel, SEG_NONSHARED) == NO_ERROR)
    {
        *ppByte = (BYTE *)MAKEP(sel, 0);
        return TRUE;
    }
    else
        return FALSE;
#else

    return (BOOL)(!DosAllocMem((PPVOID)ppByte,
                    (ULONG)usBufSiz, PAG_READ | PAG_WRITE | PAG_COMMIT));
#endif

}  /* End of UtilMemoryAllocate  */

/**************************************************************************
 *
 *  Name       : UtilMemoryFree(pByte)
 *
 *  Description: Frees memory of the image
 *
 *  Concepts   : Routine is called at cleanup time during the processing
 *               of a WM_DESTROY message, and before loading a new image
 *
 *  API's      :  DosFreeSeg  [in 16 bit]  or DosFreeMem  [in 32 bit]
 *
 *  Parameters : pByte   = pointer to the first byte of the memory block
 *
 *  Return     : [none]
 *
 *************************************************************************/
#ifdef PORT_32
VOID UtilMemoryFree(pByte)
PBYTE pByte;
{
   DosFreeMem(pByte);
}   /* End of UtilMemoryFree */
#else
#ifdef PORT_16
VOID UtilMemoryFree(SEL sel)
#else
VOID FAR pascal UtilMemoryFree(SEL sel)
#endif
{
   DosFreeSeg(sel);
}   /* End of UtilMemoryFree */
#endif

/**************************************************************************
 *
 *  Name       : UtilFindFileSize(pszFilename)
 *
 *  Description: Finds the size of a loaded image
 *
 *  Concepts   : Called to determine the size of the image in order
 *               to allocate sufficient storage for it
 *
 *  API's      : DosFindFirst
 *
 *  Parameters : pszFilename = a pointer to a string, the file name
 *
 *  Return     : filesize = max(0xFFFF, filesize)
 *
 *************************************************************************/
#if (defined(PORT_16) || defined(PORT_32))
LONG UtilFindFileSize(pszFilename)
#else
LONG FAR pascal UtilFindFileSize(pszFilename)
#endif
PSZ pszFilename;
{
    FILEFINDBUF ffb;           /* DOS file-search results buffer    */
    HDIR hdir = 0xffffffff;    /* allocate a handle to caller       */
#if (defined(PORT_16) || defined(PORT_S116))
    USHORT usAttrib  = 0;
    USHORT cSearch   = 1;
    if (DosFindFirst(pszFilename,  /* filename from Open Dialog */
                     &hdir,
                     usAttrib,
                     &ffb,
                     (USHORT)(sizeof(ffb) * cSearch),
                     &cSearch,
                     0L) == NO_ERROR)  /* reserved must be NULL */
#else
    ULONG ulAttrib  = 0;
    ULONG cSearch   = 1;
    if (DosFindFirst(pszFilename,  /* filename from Open Dialog */
                     &hdir,
                     ulAttrib,
                     &ffb,
                     sizeof(ffb) * cSearch,
                     &cSearch,
                     1L) == NO_ERROR)  /* number of files to locate */
#endif
       /*
        * Obtain the size of the image. The amount of storage
        * required depends on the size of the image, which is
        * recorded in the image-file header.
        * If the image is greater than 64KB, then only the first 64KB of
        * image data is loaded.
        */
        return (ffb.cbFileAlloc > 0xFFFFL) ? 0xFFFFL : ffb.cbFileAlloc;
    else
        return 0L;

}   /* End of UtilFindFileSize */

/**************************************************************************
 *
 *  Name       : UtilGetFileHandle(pszFilename, phfile)
 *
 *  Description: Opens the file passed in and returns its file handle
 *
 *  Concepts   : Called in order the read in the image data from a file
 *               so that it can be displayed
 *
 *  API's      : DosOpen
 *
 *  Parameters : pszFilename = a pointer to a string, the file name
 *               phfile      = a pointer to the file handle
 *
 *  Return     : the value of the DosOpen return
 *
 *************************************************************************/
#if (defined(PORT_16) || defined(PORT_32))
BOOL UtilGetFileHandle(pszFilename, phfile)
#else
BOOL FAR pascal UtilGetFileHandle(pszFilename, phfile)
#endif
PSZ pszFilename;
PHFILE phfile;
{
#if (defined(PORT_16) || defined(PORT_S116))
    USHORT usAction;

    return DosOpen(pszFilename,           /* filename from open dialog */
                   phfile,                     /* file handle returned */
                   (PUSHORT)&usAction,
                   0L,
                   FILE_NORMAL,
                   FILE_OPEN,
                   OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                   0L) == NO_ERROR;                    /* must be zero */
#else
    ULONG ulAction;

    return (BOOL)(!DosOpen(pszFilename,       /* filename from open dialog */
                   phfile,                    /* file handle returned */
                   &ulAction,
                   0L,
                   FILE_NORMAL,
                   FILE_OPEN,
                   OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                   (PEAOP2)NULL)) ;                    /* must be zero */
#endif

}   /* End of UtilGetFileHandle  */

/**************************************************************************
 *
 *  Name       : UtilUpdateTitleText(hab, hwnd, pszFullFile)
 *
 *  Description: Updates the text in the main window's title bar to
 *               display the app name, followed by the separator,
 *               followed by the file name
 *
 *  Concepts   : Called at init time and when the text file is changed
 *
 *  API's      : WinLoadString
 *               WinSetWindowText
 *               WinWindowFromID
 *
 *  Parameters : hab         = anchor block handle
 *               hwnd        = window handle
 *               pszFullFile = a string:  the path and filename
 *
 *  Return     : [none]
 *
 *************************************************************************/
#if (defined(PORT_16) || defined(PORT_32))
VOID UtilUpdateTitleText(hab, hwnd, pszFullFile)
#else
VOID FAR pascal UtilUpdateTitleText(hab, hwnd, pszFullFile)
#endif
HAB  hab;
HWND hwnd;
PSZ  pszFullFile;
{
    CHAR szBuf[MAXNAMEL];
    CHAR szSeparator[TITLESEPARATORLEN+1];
    CHAR szUntitled[MESSAGELEN];
    PSZ pszFileName;
#ifdef PORT_S116
    PCHAR pch;
    USHORT cch;
#endif

    WinLoadString(hab, (HMODULE)NULL, IDS_APPNAME, MAXNAMEL, szBuf);
    WinLoadString(hab, (HMODULE)NULL, IDS_TITLEBARSEPARATOR,
                  TITLESEPARATORLEN, szSeparator);

    strcat(szBuf, szSeparator);

#if (defined(PORT_16) || defined(PORT_32))
   /*
    * if filename is valid it will be F.Q so search for
    * final backslash, otherwise use the Untitled string.
    */
    if (NULL != (pszFileName = strrchr(pszFullFile, (int)'\\')))
        pszFileName++;
#else
    /* set pointer to end of string and count characters in string */
    pch = pszFullFile;
    cch = 0;
    while (*pch++)
        cch++;

    /* must cater to the null string in this test */
    if (cch > 0)
        pch--;

    /* search for final backslash in F.Q name if it exists */
    while (*--pch && *pch != '\\' && cch-- >= 0)  ;

   /*
    * if filename is valid (ie '\' is found in F.Q name,
    * then use filename, otherwise use the Untitled string.
    */
    if (*pch == '\\')
        pszFileName = ++pch;
#endif
    else
    {
        /* load "untitled" string */
        WinLoadString(hab, 0, IDS_UNTITLED, MESSAGELEN, szUntitled);
        pszFileName = (PSZ)szUntitled;
    }
    strcat(szBuf, pszFileName);  /* szBuf = Image Sample Program - IMAGE.IMG */
    WinSetWindowText(WinWindowFromID(hwnd, FID_TITLEBAR), szBuf);
}   /* End of UtilUpdateTitleText  */
/***************************  End of img_util.c  *************************/
