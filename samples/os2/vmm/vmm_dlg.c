/*static char *SCCSID = "@(#)vmm_dlg.c	6.12 92/03/18";*/
/*==============================================================*\
 *  VMM_DLG.C - window procedures for the dialog boxes as well  *
 *              as utility procedures used by them              *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the Dialog Procedures for the user     *
 *  defined dialogs as well as any support code they need       *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *           AllocMemDlgProc(hwnd, msg, mp1, mp2)               *
 *           SetMem1DlgProc( hwnd, msg, mp1, mp2)               *
 *           SetMem2DlgProc( hwnd, msg, mp1, mp2)               *
 *           ReadMemDlgProc( hwnd, msg, mp1, mp2)               *
 *           WriteMemDlgProc(hwnd, msg, mp1, mp2)               *
 *           FreeMemDlgProc( hwnd, msg, mp1, mp2)               *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "vmm.h"

/****************************************************************\
 *  Dialog procedure for the Allocate Memory dialog             *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    AllocMemDlgProc(hwnd, msg, mp1, mp2)               *
 *                                                              *
 *  Purpose: Processes all messages sent to the AllocMem dialog *
 *                                                              *
 *  Usage:   Called for each message sent to the dialog box.    *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed. Any messages *
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY AllocMemDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static POBJSTRUCT pObj;
    MRESULT sRC;

    switch (msg) {
        case WM_INITDLG:

            pObj = PVOIDFROMMP(mp2);
            FixSysMenu(hwnd);

            _ltoa(
                   (pObj->ulSize) ,(CHAR *) szBuffer, 10);  /* radix=10 */

            WinSetDlgItemText(hwnd, IDD_EDITSIZE, szBuffer);

            WinSendDlgItemMsg(hwnd, IDD_READ, BM_SETCHECK,
                   MPFROMSHORT((pObj->ulAttr & PAG_READ) ? 1 : 0), (MPARAM)0L);

            WinSendDlgItemMsg(hwnd, IDD_EXECUTE, BM_SETCHECK,
                 MPFROMSHORT((pObj->ulAttr & PAG_EXECUTE) ? 1 : 0),(MPARAM)0L);

            WinSendDlgItemMsg(hwnd, IDD_WRITE, BM_SETCHECK,
                 MPFROMSHORT((pObj->ulAttr & PAG_WRITE) ? 1 : 0),(MPARAM)0L);

            WinSendDlgItemMsg(hwnd, IDD_GUARD, BM_SETCHECK,
                 MPFROMSHORT((pObj->ulAttr & PAG_GUARD) ? 1 : 0),(MPARAM)0L);

            WinSendDlgItemMsg(hwnd, IDD_COMMIT, BM_SETCHECK,
                 MPFROMSHORT((pObj->ulAttr & PAG_COMMIT) ? 1 : 0),(MPARAM)0L);

            WinSendDlgItemMsg(hwnd, IDD_TILE, BM_SETCHECK,
                 MPFROMSHORT((pObj->ulAttr & OBJ_TILE) ? 1 : 0),(MPARAM)0L);

            return((MRESULT)FALSE);
                           /* Let the focus go to where the system puts it */

        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) {
                case DID_OK:
                    WinQueryDlgItemText(hwnd, IDD_EDITSIZE, MAX_EDIT_BUFF,
                                          szBuffer);    /* Get user's size */
                    pObj->ulSize = atol(szBuffer);
                    pObj->ulSize *= 1000L;
                    pObj->ulAttr =
                  ((WinSendDlgItemMsg(hwnd,IDD_READ,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)PAG_READ : 0L)    |
                  ((WinSendDlgItemMsg(hwnd,IDD_EXECUTE,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)PAG_EXECUTE : 0L) |
                  ((WinSendDlgItemMsg(hwnd,IDD_WRITE,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)PAG_WRITE : 0L)   |
                  ((WinSendDlgItemMsg(hwnd,IDD_GUARD,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)PAG_GUARD : 0L)   |
                  ((WinSendDlgItemMsg(hwnd,IDD_COMMIT,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)PAG_COMMIT : 0L)  |
                  ((WinSendDlgItemMsg(hwnd,IDD_TILE,BM_QUERYCHECK,(MPARAM)0L,
                       (MPARAM)0L) == (MRESULT)1L) ? (ULONG)OBJ_TILE : 0L);

                    if (((pObj->ulAttr & PAG_READ) == 0L)  /* Disable READ */
                         && ((pObj->ulAttr & PAG_EXECUTE) != 0L))
                    {
                       MessageBox(hwnd, IDMSG_UNSETREADERROR, "Error !",
                                    MB_OK | MB_INFORMATION, TRUE);
                       pObj->ulAttr = pObj->ulAttr | PAG_READ;
                    }
                    if (((pObj->ulAttr & PAG_READ) == 0L)  /* Disable READ */
                         && ((pObj->ulAttr & PAG_WRITE) != 0L))
                    {
                       MessageBox(hwnd, IDMSG_UNSETREADERROR2, "Error !",
                                    MB_OK | MB_INFORMATION, TRUE);
                       pObj->ulAttr = pObj->ulAttr | PAG_READ;
                    }
                    WinDismissDlg(hwnd, TRUE);
                    return((MRESULT)0L);

                case DID_CANCEL:
                    WinDismissDlg(hwnd, FALSE);
                    return((MRESULT)0L);

                default:
                    sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                    return sRC;
                }
            break;

        case WM_CLOSE:
           WinDismissDlg(hwnd, FALSE);
           return((MRESULT)0L);

        default:
            sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
            return sRC;
    }
    return((MRESULT)0L);
}

/****************************************************************\
 *  Dialog procedure for the Set Memory dialog                  *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    SetMem1DlgProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: Processes all messages sent to the SetMem1 dialog  *
 *                                                              *
 *  Usage:   Called for each message sent to the dialog box.    *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed.  Any messages*
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY SetMem1DlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static POBJSTRUCT pObj;
   ULONG  ulIndexLow;
   MRESULT sRC;

   switch (msg) {
       case WM_INITDLG:  /* address of buffer pointed to by pCreateParams */
           pObj = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);

           WinSetDlgItemText(hwnd, IDD_EDITSIZE, SZDEFAULT_SIZE);

           return((MRESULT)FALSE);
                         /* Let the focus go to where the system puts it */

       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                   WinQueryDlgItemText(hwnd, IDD_ADDRESS, MAX_EDIT_BUFF,
                                              szBuffer);
                   sscanf(szBuffer,"%p",&(pObj->pvAddress));

                   WinQueryDlgItemText(hwnd, IDD_EDITSIZE, MAX_EDIT_BUFF,
                                              szBuffer);
                   sscanf(szBuffer,"%p",&(pObj->ulSize));

                   ulIndexLow = 0;
                                            /* Look for the pointed page */
                   while((apgentry[ulIndexLow].pvAddress != pObj->pvAddress)
                                      && (ulIndexLow < ulFreePage))
                       ulIndexLow++;
                   if(ulIndexLow == ulFreePage)       /* Illeage address */
                   {
                      MessageBox(hwnd, IDMSG_ERRORADDRESS, "Error !",
                                       MB_OK | MB_MOVEABLE, TRUE);
                      return (MRESULT)0L;
                   }
                   WinDismissDlg(hwnd, TRUE);
                   return((MRESULT)0L);

               case DID_CANCEL:
                   WinDismissDlg(hwnd, FALSE);
                   return((MRESULT)0L);

               default:
                   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                   return sRC;
           }
           break;

        case WM_CLOSE:
           WinDismissDlg(hwnd, FALSE);
           return((MRESULT)0L);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
       }
       return((MRESULT)0L);
}

/****************************************************************\
 *  Dialog procedure for the Set Memory dialog                  *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    SetMem2DlgProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: Processes all messages sent to the SetMem2 dialog. *
 *                                                              *
 *  Usage:   Called for each message sent to the dialog box.    *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed.  Any messages*
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY SetMem2DlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static POBJSTRUCT pObj;
   ULONG ulRegionSize, ulFlags=0L;
   ULONG rc;
   MRESULT sRC;

   switch (msg) {
       case WM_INITDLG:
           pObj = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);

           rc = DosQueryMem(pObj->pvAddress, &ulRegionSize, &ulFlags);
           if (rc)                                      /* Query is fail */
           {
               VMM_Error("DosQueryMem()", rc);
               WinDismissDlg(hwnd, FALSE);
               return((MRESULT)FALSE);
           }
           memset(szBuffer,0,sizeof(szBuffer) );
           sprintf(szBuffer, "Address of memory =0x%p",pObj->pvAddress);
           WinSetDlgItemText(hwnd, IDD_ADDRESS, szBuffer);

           WinSendDlgItemMsg(hwnd, IDD_READ, BM_SETCHECK,
                 MPFROMSHORT((ulFlags & PAG_READ) ? 1 : 0),(MPARAM)0L);
           WinSendDlgItemMsg(hwnd, IDD_EXECUTE, BM_SETCHECK,
                 MPFROMSHORT((ulFlags & PAG_EXECUTE) ? 1 : 0),(MPARAM)0L);
           WinSendDlgItemMsg(hwnd, IDD_WRITE, BM_SETCHECK,
                 MPFROMSHORT((ulFlags & PAG_WRITE) ? 1 : 0),(MPARAM)0L);
           WinSendDlgItemMsg(hwnd, IDD_GUARD, BM_SETCHECK,
                 MPFROMSHORT((ulFlags & PAG_GUARD) ? 1 : 0),(MPARAM)0L);
           WinSendDlgItemMsg(hwnd, IDD_DEFAULT, BM_SETCHECK,
                 MPFROMSHORT(1), (MPARAM)0L);

           return((MRESULT)FALSE);
                           /* Let the focus go to where the system puts it */
       case WM_CONTROL:
           switch (SHORT1FROMMP(mp1)) {
               case IDD_DECOMMIT:
                            /* Can't change attributes when de-committing */
                   if (SHORT2FROMMP(mp1) == BN_CLICKED)
                   {
                    if((WinSendDlgItemMsg(hwnd,IDD_DECOMMIT,BM_QUERYCHECK,
                        (MPARAM)0L,(MPARAM)0L) == (MRESULT)1L) ? TRUE : FALSE)
                    {
            /* If we are decommitting memory, don't change any attributes */
        WinSendDlgItemMsg(hwnd, IDD_READ, BM_SETCHECK,(MPARAM)0L,(MPARAM)0L);
        WinSendDlgItemMsg(hwnd, IDD_EXECUTE, BM_SETCHECK,(MPARAM)0L,(MPARAM)0L);
        WinSendDlgItemMsg(hwnd, IDD_WRITE, BM_SETCHECK,(MPARAM)0L,(MPARAM)0L);
        WinSendDlgItemMsg(hwnd, IDD_GUARD, BM_SETCHECK,(MPARAM)0L,(MPARAM)0L);
                    }
                   }             /* tell it we didn't process the message */
                   return((MRESULT)TRUE);

               case IDD_DEFAULT:
                   if (SHORT2FROMMP(mp1) == BN_CLICKED)
                   {
                    if ((WinSendDlgItemMsg(hwnd,IDD_DEFAULT,BM_QUERYCHECK,
                          (MPARAM)0L,(MPARAM)0L) == (MRESULT)1L) ?
                          TRUE : FALSE)
                    {               /* Reset attributes to initial values */
                     WinSendDlgItemMsg(hwnd, IDD_READ, BM_SETCHECK,
                                            MPFROMLONG(1L),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_EXECUTE, BM_SETCHECK,
                                            MPFROMLONG(1L),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_WRITE, BM_SETCHECK,
                                            MPFROMLONG(1L),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_GUARD, BM_SETCHECK,
                                            (MPARAM)0L,(MPARAM)0L);
                    }
                   }             /* tell it we didn't process the message */
                   return((MRESULT)TRUE);

               case IDD_COMMIT:
                   /* For these cases, put the attributes back the way
                      they were, this is for whenthe user clicks on de-commit,
                      then, clicks back on default or commit since we cleared
                      the attribute flags when they clicked on decommit */

                   if (SHORT2FROMMP(mp1) == BN_CLICKED)
                   {
                      rc=DosQueryMem(pObj->pvAddress, &ulRegionSize, &ulFlags);
                      if (rc)
                      {
                          VMM_Error("DosQueryMem()", rc);

                          WinDismissDlg(hwnd, FALSE);
                          return((MRESULT)FALSE);
                      }
                     WinSendDlgItemMsg(hwnd, IDD_READ, BM_SETCHECK,
                      MPFROMSHORT((ulFlags & PAG_READ) ? 1 : 0),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_EXECUTE, BM_SETCHECK,
                      MPFROMSHORT((ulFlags & PAG_EXECUTE) ? 1 : 0),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_WRITE, BM_SETCHECK,
                      MPFROMSHORT((ulFlags & PAG_WRITE) ? 1 : 0),(MPARAM)0L);
                     WinSendDlgItemMsg(hwnd, IDD_GUARD, BM_SETCHECK,
                      MPFROMSHORT((ulFlags & PAG_GUARD) ? 1 : 0),(MPARAM)0L);
                   }
                   return((MRESULT)TRUE);
                                /* tell it we didn't process the message */
               default:
                   return((MRESULT)TRUE);
           }
           break;

       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                   pObj->ulAttr =
                 ( (WinSendDlgItemMsg(hwnd,IDD_READ,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_READ : 0L ) |
                 ( (WinSendDlgItemMsg(hwnd,IDD_EXECUTE,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_EXECUTE : 0L ) |
                 ( (WinSendDlgItemMsg(hwnd,IDD_WRITE,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_WRITE : 0L ) |
                 ( (WinSendDlgItemMsg(hwnd,IDD_GUARD,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_GUARD : 0L ) |
                 ( (WinSendDlgItemMsg(hwnd,IDD_COMMIT,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_COMMIT : 0L ) |
                 ( (WinSendDlgItemMsg(hwnd,IDD_DECOMMIT,BM_QUERYCHECK,(MPARAM)0L,
                    (MPARAM)0L) == (MRESULT) 1L) ? (ULONG)PAG_DECOMMIT : 0L );

                  if (((pObj->ulAttr & PAG_READ) == 0L)  /* Disable READ */
                       && ((pObj->ulAttr & PAG_EXECUTE) != 0L))
                  {
                     MessageBox(hwnd, IDMSG_UNSETREADERROR, "Error !",
                                  MB_OK | MB_INFORMATION, TRUE);
                     pObj->ulAttr = pObj->ulAttr | PAG_READ;
                  }
                  if (((pObj->ulAttr & PAG_READ) == 0L)  /* Disable READ */
                       && ((pObj->ulAttr & PAG_WRITE) != 0L))
                  {
                     MessageBox(hwnd, IDMSG_UNSETREADERROR2, "Error !",
                                  MB_OK | MB_INFORMATION, TRUE);
                     pObj->ulAttr = pObj->ulAttr | PAG_READ;
                  }
                /**************************************************************
                 *if ((pObj->ulAttr & PAG_EXECUTE) == 0L)  //Disable EXECUTE//*
                 *{                                                           *
                 *  NOTE :                                                    *
                 *  When user disables the EXECUTE and call DosSetMem() to    *
                 *  change attribute, this program doesn't return error       *
                 *  and still attach the EXECUTE attribute on it.             *
                 *  That is because there are a bug in Intel-80386 chip, it   *
                 *  makes the DosSetMem() not work normally.                  *
                 *}                                                           *
                 **************************************************************/

                  WinDismissDlg(hwnd, TRUE);
                  return((MRESULT)FALSE);
                                   /* tell system we did process the message */

               case DID_CANCEL:
                  WinDismissDlg(hwnd, FALSE);
                  return((MRESULT)FALSE);
                                /* tell system we did process the message */
               default:         /* tell it we didn't process the message */
                  sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                  return sRC;
           }
           break;

       case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)0L);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
       }
   return((MRESULT)TRUE);
}

/****************************************************************\
 *  Dialog procedure for the Read Memory dialog                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    ReadMemDlgProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: Processes all messages sent to the Read Memory     *
 *            dialog                                            *
 *                                                              *
 *  Usage:   Called for each message sent to the dialog box.    *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed.  Any messages*
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY ReadMemDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static POBJSTRUCT pObj;
   ULONG  ulIndexLow;
   MRESULT sRC;

   switch (msg) {
       case WM_INITDLG:
           pObj = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);

           return((MRESULT)FALSE);
                           /* Let the focus go to where the system puts it */

       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                   WinQueryDlgItemText(hwnd, IDD_ADDRESS, MAX_EDIT_BUFF,
                                           szBuffer);
                   sscanf(szBuffer,"%p",&(pObj->pvAddress));

                   ulIndexLow = 0;
                                            /* Look for the pointed page */
                   while((apgentry[ulIndexLow].pvAddress != pObj->pvAddress)
                                    && (ulIndexLow < ulFreePage))
                       ulIndexLow++;
                   if(ulIndexLow == ulFreePage)       /* Illeage address */
                   {
                      MessageBox(hwnd, IDMSG_ERRORADDRESS, "Error !",
                                       MB_OK | MB_MOVEABLE, TRUE);
                      return (MRESULT)0L;
                   }

                   WinDismissDlg(hwnd, TRUE);
                   return((MRESULT)TRUE);

               case DID_CANCEL:
                   WinDismissDlg(hwnd, FALSE);
                   return((MRESULT)TRUE);

               default:
                   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                   return sRC;
               }
           break;

       case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)0L);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
       }
   return (MRESULT)0L;
}

/****************************************************************\
 *  Dialog procedure for the Write Memory dialog                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    WriteMemDlgProc(hwnd, msg, mp1, mp2)               *
 *                                                              *
 *  Purpose: Processes all messages sent to the WriteMem dialog.*
 *                                                              *
 *  Usage:   Called for each message sent to the dialog box.    *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed.  Any messages*
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY WriteMemDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   PVOID   pvMemAddress=0;
   ULONG   ulIndexLow;
   MRESULT sRC;

   switch (msg) {
       case WM_INITDLG:
           FixSysMenu(hwnd);
           return((MRESULT)FALSE);
                       /* Let the the focus go to where the system puts it */
       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                   WinQueryDlgItemText(hwnd, IDD_ADDRESS, MAX_EDIT_BUFF,
                                           szBuffer);
                   sscanf(szBuffer,"%p",&pvMemAddress);

                   ulIndexLow = 0;
                                            /* Look for the pointed page */
                   while((apgentry[ulIndexLow].pvAddress != pvMemAddress)
                                    && (ulIndexLow < ulFreePage))
                       ulIndexLow++;
                   if(ulIndexLow == ulFreePage)       /* Illeage address */
                   {
                      MessageBox(hwnd, IDMSG_ERRORADDRESS, "Error !",
                                       MB_OK | MB_MOVEABLE, TRUE);
                      return (MRESULT)0L;
                   }
                   memset(szBuffer,0,sizeof(szBuffer) );
                   WinQueryDlgItemText(hwnd, IDD_DATA, MAX_EDIT_BUFF, szBuffer);
                   strcpy(pvMemAddress, szBuffer);  /* let 'em go for it.
                                                 it may cause a GP fault */
                   WinDismissDlg(hwnd, TRUE);
                   return((MRESULT)TRUE);

               case DID_CANCEL:
                   WinDismissDlg(hwnd, FALSE);
                   return((MRESULT)TRUE);

               default:
                   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                   return sRC;
               }
           break;

       case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)0L);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
       }
   return (MRESULT)0L;

}

/****************************************************************\
 *  Dialog procedure for the Free Memory dialog                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    FreeMemDlgProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: Processes all messages sent to the FreeMem dialog  *
 *                                                              *
 *  Usage:   Called for each message sent to the Free Memory    *
 *           dialog box.                                        *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed.  Any messages*
 *           not specifically processed are passed to the       *
 *           default window procedure WinDefDlgProc()           *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY FreeMemDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static POBJSTRUCT pObj;
   MRESULT sRC;
   ULONG  ulIndexLow;

   switch (msg) {
       case WM_INITDLG:
           pObj = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);
           return((MRESULT)FALSE);
                     /* Let the the focus go to where the system puts it */

       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                   WinQueryDlgItemText(hwnd, IDD_ADDRESS, MAX_EDIT_BUFF,
                                              szBuffer);
                   sscanf(szBuffer,"%p",&(pObj->pvAddress));

                   ulIndexLow = 0;
                                            /* Look for the pointed page */
                   while((apgentry[ulIndexLow].pvAddress != pObj->pvAddress)
                                    && (ulIndexLow < ulFreePage))
                       ulIndexLow++;
                   if(ulIndexLow == ulFreePage)       /* Illeage address */
                   {
                      MessageBox(hwnd, IDMSG_ERRORADDRESS, "Error !",
                                       MB_OK | MB_MOVEABLE, TRUE);
                      return (MRESULT)0L;
                   }
                   WinDismissDlg(hwnd, TRUE);
                   break;

               case DID_CANCEL:
                   WinDismissDlg(hwnd, FALSE);
                   break;
               }
           break;

       case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)0L);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
       }
   return (MRESULT)0L;
}
