/*static char *SCCSID = "@(#)dlapi_dg.c	6.13 92/03/18";*/
/*==============================================================*\
 *  DLL_DLG.C - window procedures for the dialog boxes as well  *
 *              as utility procedures used by them              *
 *     (C) Copyright IBM Corporation 1992.                      *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the Dialog Procedures for the user     *
 *  defined dialogs as well as any support code they need       *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *    GetPatternDlgProc(hwnd, msg, mp1, mp2)                    *
 *    DispFileDlgProc  (hwnd, msg, mp1, mp2)                    *
 *    DisplayFile      (hwnd, rect, pFile)                      *
 *    OpenDlgProc      (hwnd, msg, mp1, mp2)                    *
 *    DisplayDlgProc   (hwnd, msg, mp1, mp2)                    *
 *    MathDlgProc      (hwnd, msg, mp1, mp2)                    *
 *    ResultDlgProc    (hwnd, msg, mp1, mp2)                    *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "dllapi.h"

/****************************************************************\
 *  Dialog procedure for the Path/Pattern dialog                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    GetPatternDlgProc(hwnd, msg, mp1, mp2)             *
 *                                                              *
 *  Purpose: Processes all messages sent to the Pattern dialog  *
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
MRESULT EXPENTRY GetPatternDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static POBJSTRUCT pObj=NULL;
   static CHAR   szBuffer[CCHMAXPATH];
   ULONG  ulDriveMap, ulDriveNum=0L, ulDirLen = CCHMAXPATH;
   SHORT  sIndex;
   MRESULT sRC;

   switch (msg)
   {
      case WM_INITDLG:

          pObj = PVOIDFROMMP(mp2);
          FixSysMenu(hwnd);
                                                  /* Get current drive */
          DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
          memset(szBuffer, 0, sizeof(szBuffer));
          szBuffer[0] = (CHAR)(ulDriveNum + '@');
          strcpy(pObj->szCurDriver, szBuffer);
          *(pObj->szCurDriver+1) = ':';
                                 /* Get current dir & attach disk num. */
          DosQueryCurrentDir(0L, pObj->szCurPath, &ulDirLen);
          if(*(pObj->szCurPath+strlen(pObj->szCurPath)-1) != '\\')
             strcat(pObj->szCurPath, "\\");     /* Add '\' at the tail */

          memset(szBuffer, 0, sizeof(szBuffer));
          strcpy(szBuffer, pObj->szCurDriver);
          if(*(pObj->szCurPath) != '\\')
                  strcat(szBuffer,"\\");
          strcat(szBuffer, pObj->szCurPath);/*Default path is current diver*/
          WinSendDlgItemMsg(hwnd, IDD_PATH, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(CCHMAXPATH, 0), NULL);
          WinSetDlgItemText(hwnd, IDD_PATH, szBuffer);

          if(*(pObj->szCurPattern) == '\000')
             strcpy(pObj->szCurPattern, "*.*");     /* Inition Pattern */
          WinSetDlgItemText(hwnd, IDD_PATTERN, pObj->szCurPattern);

          return((MRESULT)FALSE);
                        /* Let the focus go to where the system puts it */
      case WM_COMMAND:
          switch (SHORT1FROMMP(mp1)) {
              case DID_OK:
                                                   /* Get new pattern */
                  memset(szBuffer,0,sizeof(szBuffer));
                  WinQueryDlgItemText(hwnd, IDD_PATTERN, MAX_EDIT_BUFF,
                                        szBuffer);
                  for (sIndex=0; szBuffer[sIndex]!='\000' &&
                         sIndex<MAX_EDIT_BUFF; sIndex++)
                  {
                     if (szBuffer[sIndex]>='a' && szBuffer[sIndex]<='z')
                     {
                        szBuffer[sIndex] = szBuffer[sIndex] - (CHAR)' ';
                     }                                       /* endif */
                  }                                         /* endfor */
                  memset(pObj->szNewPattern,0,sizeof(pObj->szNewPattern));
                  if (*szBuffer == '\000')       /* No input new path */
                     strcpy(pObj->szNewPattern,pObj->szCurPattern);
                  else                          /* Store user pattern */
                     strcpy(pObj->szNewPattern,szBuffer);

                                                      /* Get new path */
                  memset(szBuffer,0,sizeof(szBuffer));
                  WinQueryDlgItemText(hwnd, IDD_PATH, CCHMAXPATH,
                                                 szBuffer);
                  for (sIndex=0; szBuffer[sIndex]!='\000' &&
                         sIndex<CCHMAXPATH; sIndex++)
                  {
                     if (szBuffer[sIndex]>='a' && szBuffer[sIndex]<='z')
                     {
                        szBuffer[sIndex] = szBuffer[sIndex] - (CHAR)' ';
                     }                                       /* endif */
                  }                                         /* endfor */

                  memset(pObj->szNewDriver,0,sizeof(pObj->szNewDriver));
                  memset(pObj->szNewPath,0,sizeof(pObj->szNewPath));

                  if(*(szBuffer+strlen(szBuffer)-1) != '\\')
                     strcat(szBuffer, "\\");   /* Add '\' at the tail */
                  if (*szBuffer == '\000')       /* No input new path */
                  {
                     strcpy(pObj->szNewDriver,pObj->szCurDriver);
                     strcpy(pObj->szNewPath,pObj->szCurPath);
                  }
                  else if(*(szBuffer+1) == ':') /* they specified a drive */
                  {
                     *(pObj->szNewDriver) = *szBuffer;/*Store disk number */
                     *((pObj->szNewDriver)+1) = *(szBuffer+1);
                     if(*(szBuffer+2) != '\\')             /* If Path= c: */
                     {
                        *(pObj->szNewPath) = '\\';         /* Set path= \ */
                     }
                                                        /*Store user path*/
                     strcat(pObj->szNewPath,(szBuffer+2));
                  }
                  else
                  {
                     strcpy(pObj->szNewDriver,pObj->szCurDriver);
                     strcpy(pObj->szNewPath,szBuffer);   /* Store user path */
                  }
                  WinDismissDlg(hwnd, TRUE);
                  return((MRESULT)TRUE);

              case DID_CANCEL:
                  WinDismissDlg(hwnd, FALSE);
                  return((MRESULT)TRUE);
          }
          break;

      case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)TRUE);

      default:
          sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
          return sRC;
   }
   return((MRESULT)TRUE);
}

/****************************************************************\
 *  Dialog procedure for the Display file dialog                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    DispFileDlgProc(hwnd, msg, mp1, mp2)               *
 *                                                              *
 *  Purpose: Processes all messages sent to the display dialog  *
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
MRESULT EXPENTRY DispFileDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   SHORT            Ret=-1;
   PVOID            pvAddress=0;
   PFILELIST        pTail=NULL, pNext=NULL;    /* The last file of list */
   MRESULT sRC;

   switch (msg)
   {
      case WM_INITDLG:
          FixSysMenu(hwnd);
          pBackup=NULL;
                                  /* Default click the buttom of backup */
          WinSendDlgItemMsg(hwnd, IDD_ACCESSBACKUP,
                              BM_CLICK, (MPARAM)0L, (MPARAM)0L);
          return((MRESULT)FALSE);
                        /* Let the focus go to where the system puts it */
      case WM_COMMAND:
          switch (SHORT1FROMMP(mp1)) {
             case DID_OK:
                 pNext = pFindFile;
                 DosSetMem(pNext, sizeof(PFILELIST),
                           PAG_READ|PAG_WRITE|PAG_EXECUTE);
                 while (pNext != NULL)
                 {
                    if(!DosAllocMem(&pvAddress,sizeof(PFILELIST),fALLOC))
                    {        /* Store file list to backup structure */
                       if (pBackup == NULL)
                       {                       /* Back up 1'st file */
                          pBackup = (PFILELIST)pvAddress;
                          strcpy(pBackup->szFileName,pNext->szFileName);
                          pBackup->pNextFile = NULL;
                          pTail = pBackup;
                       }
                       else
                       {
                          pTail->pNextFile = (PFILELIST)pvAddress;
                          pTail = pTail->pNextFile;
                          strcpy(pTail->szFileName, pNext->szFileName);
                          pTail->pNextFile = NULL;
                       }
                       pNext = pNext->pNextFile;
                    }
                 }                                    /* while loop */
                 pNext = pFindFile; /* Protect the memory of file list */
                 if(pNext != NULL)
                 {
                    DosSetMem(pNext,sizeof(PFILELIST),
                                      PAG_GUARD|PAG_READ|PAG_WRITE);
                 }
                 Ret = (SHORT)WinSendDlgItemMsg(hwnd, (ULONG)IDD_ACCESSBACKUP,
                            BM_QUERYCHECKINDEX, (MPARAM)0L, (MPARAM)0L);
                 if(Ret == 0)
                 {
                    pFlList = pBackup;
                 }
                 else
                 {
                    pFlList = pFindFile;
                 }
                 WinDismissDlg(hwnd, TRUE);
                 return((MRESULT)TRUE);

             case DID_CANCEL:
                 WinDismissDlg(hwnd, FALSE);
                 return((MRESULT)TRUE);
          }
          break;

      case WM_CLOSE:
          WinDismissDlg(hwnd, FALSE);
          return((MRESULT)TRUE);

      default:
          sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
          return sRC;
   }
   return((MRESULT)TRUE);
}

/****************************************************************\
 *                                                              *
 *  Name:    DisplayFile(hwnd, rect, pFile)                     *
 *                                                              *
 *  Purpose: This routine searches and display files which are  *
 *           matching the pattern.                              *
 *                                                              *
 *  Usage:   After files searching, this module will create a   *
 *           child window to display the matched files.         *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID DisplayFile (HWND hwnd, RECTL rect, PPFILELIST pFile)
{
   CHAR         szBuf[CCHMAXPATH];
   APIRET       sCount=0;
   PFILELIST    pTail=NULL;                         /* The last file of list */

   if(hwndChild)
     WinDestroyWindow(hwndChild);
   hwndChild = WinCreateWindow (hwnd,
                                WC_LISTBOX,
                                "File List...",
                                WS_VISIBLE | FCF_MINMAX,
                                rect.xLeft,
                                rect.yBottom,
                                rect.xRight-rect.xLeft,
                                rect.yTop-rect.yBottom-5,
                                hwnd,
                                HWND_TOP,
                                ID_CHILDWIN,
                                NULL,
                                (PVOID)NULL);
   pTail = *pFile;
   if(*pFile != NULL)
   {
      strcpy(szBuf, "The found file list ....");
      WinSendDlgItemMsg(hwnd, ID_CHILDWIN, LM_INSERTITEM,
                        MPFROM2SHORT(LIT_END, 0),
                        MPFROMP(szBuf));
   }
   while (pTail != NULL)
   {
      strcpy(szBuf, pTail->szFileName);
      WinSendDlgItemMsg(hwnd, ID_CHILDWIN, LM_INSERTITEM,
                        MPFROM2SHORT(LIT_END, 0),
                        MPFROMP(pTail->szFileName));
      sCount++;
      pTail = pTail->pNextFile;
   }                                                        /* endwhile */
   memset(szBuf, 0, sizeof(szBuf));
   sprintf(szBuf, "There are %d files found.", sCount);
   WinSendDlgItemMsg(hwnd, ID_CHILDWIN, LM_INSERTITEM,
                     MPFROM2SHORT(LIT_END, 0),
                     MPFROMP(szBuf));
   while(*pFile != NULL)                 /* Free the File list structure */
   {
      pTail = *pFile;
      *pFile = (*pFile)->pNextFile;
      DosFreeMem(pTail);
   }
   return;
}

/****************************************************************\
 *  Dialog procedure for the Open file dialog                   *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    OpenDlgProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: Processes all messages sent to the Open file dialog*
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
MRESULT EXPENTRY OpenDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    static PFILEINFO pFile;
    static CHAR   szBuffer[CCHMAXPATH];
    ULONG  ulDriveMap, ulDriveNum=0L, ulDirLen = CCHMAXPATH;
    SHORT  sIndex=0;
    MRESULT sRC;

    switch (msg)
    {
        case WM_INITDLG:

            pFile = PVOIDFROMMP(mp2);
            FixSysMenu(hwnd);
            memset(pFile, 0, sizeof(pFile));
                                                    /* Get current drive */
            DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
            *(pFile->szFileName) = (CHAR)(ulDriveNum + '@');
            *(pFile->szFileName+1) = ':';
                                   /* Get current dir & attach disk num. */
            memset(szBuffer, 0, sizeof(szBuffer));
            DosQueryCurrentDir(0L, szBuffer, &ulDirLen);

            if(*(szBuffer+strlen(szBuffer)-1) != '\\')
               strcat(szBuffer, "\\");            /* Add '\' at the tail */
            if(*(szBuffer) != '\\')       /* Add '\' at begining of path */
               strcat(pFile->szFileName,"\\");

            strcat(pFile->szFileName,szBuffer);  /* Display current path */
            WinSendDlgItemMsg(hwnd, IDD_PATH, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(CCHMAXPATH, 0), NULL);
            WinSetDlgItemText(hwnd, IDD_PATH, pFile->szFileName );

            return((MRESULT)FALSE);
                         /* Let the focus go to where the system puts it */
        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) {
               case DID_OK:
                  memset(szBuffer,0,sizeof(szBuffer));
                  WinQueryDlgItemText(hwnd, IDD_PATH, CCHMAXPATH,
                                        szBuffer);
                  memset(pFile->szFileName,0,sizeof(pFile->szFileName));
                  for (sIndex=0; szBuffer[sIndex]!='\000' &&
                        sIndex<CCHMAXPATH; sIndex++)
                  {
                     if (szBuffer[sIndex]>='a' && szBuffer[sIndex]<='z')
                     {
                        szBuffer[sIndex] = szBuffer[sIndex] - (CHAR)' ';
                     }                /* Changed lowercase to uppercase */
                  }
                  strcpy(pFile->szFileName, szBuffer);
                  WinDismissDlg(hwnd, TRUE);
                  break;

               case DID_CANCEL:
                  WinDismissDlg(hwnd, FALSE);
                  break;
               }
            break;

        case WM_CLOSE:
            WinDismissDlg(hwnd, FALSE);
            return((MRESULT)TRUE);

        default:
            sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
            return sRC;
    }
    return (MRESULT)RETURN_SUCCESS;
}

/****************************************************************\
 *  Dialog procedure for the Display text dialog                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    DisplayDlgProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: Processes all messages sent to the Display text    *
 *           dialog window.                                     *
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
MRESULT EXPENTRY DisplayDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   CHAR    szBuf[CCHMAXPATH];
   static PFILEINFO pFile;
   MRESULT sRC;

   switch (msg)
   {
      case WM_INITDLG:

          pFile = PVOIDFROMMP(mp2);
          FixSysMenu(hwnd);
          strcpy(szBuf,"File: ");          /* Display file path & name */
          strcat(szBuf,pFile->szFileName);
          WinSendDlgItemMsg(hwnd, IDD_PATH, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(CCHMAXPATH, 0), NULL);
          WinSetDlgItemText(hwnd, IDD_PATH, szBuf);
                                               /* Display file content */
          WinSendDlgItemMsg(hwnd, IDD_TEXT, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(BUFF_SIZE, 0), NULL);
          WinSetDlgItemText(hwnd, IDD_TEXT, pFile->szBuffer);

          return((MRESULT)FALSE);
                       /* Let the focus go to where the system puts it */
      case WM_COMMAND:
          switch (SHORT1FROMMP(mp1)) {
              case DID_OK:
                 WinDismissDlg(hwnd, TRUE);
                 break;
              }
          break;

      default:
          sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
          return sRC;
   }
   return (MRESULT)RETURN_SUCCESS;
}

/****************************************************************\
 *  Dialog procedure for the Calculator Entry dialog            *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MathDlgProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: Processes all messages sent to the Calculator input*
 *           dialog window.                                     *
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
MRESULT EXPENTRY MathDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static PMATHELEMENT pMath;
   static CHAR szBuffer[OP_SIZE];
   SHORT  sIndex;
   MRESULT sRC;

   switch (msg)
   {
       case WM_INITDLG:
           pMath = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);
           pMath->fOperand1 = 0;
           pMath->fOperand2 = 0;
           *pMath->pszOperation = '\000';
           memset(pMath->szResult, 0, sizeof(pMath->szResult) );
           WinSendDlgItemMsg(hwnd, IDD_OPERAND1, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(OP_SIZE, 0), NULL);
           WinSendDlgItemMsg(hwnd, IDD_OPERAND2, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(OP_SIZE, 0), NULL);
           WinSendDlgItemMsg(hwnd, IDD_OPERATION, EM_SETTEXTLIMIT,
                               MPFROM2SHORT(1L, 0), NULL);
           return((MRESULT)FALSE);
                         /* Let the focus go to where the system puts it */
       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1))
           {
              case DID_OK:
                 memset(szBuffer,0,sizeof(szBuffer));
                 WinQueryDlgItemText(hwnd, IDD_OPERAND1, OP_SIZE+1,
                                        szBuffer);
                 for (sIndex=0; sIndex<OP_SIZE && szBuffer[sIndex]!='\000';
                            sIndex++)
                 {
                    if( (szBuffer[sIndex]<'0' || szBuffer[sIndex]>'9')
                        && (szBuffer[sIndex] != '.') )
                    {
                       MessageBox(hwnd, IDMSG_ERROROPERAND, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       return((MRESULT)FALSE);          /* Illegal input */
                    }
                 }                                             /* endfor */
                 pMath->fOperand1 = atol(szBuffer);

                 memset(szBuffer,0,sizeof(szBuffer));
                 WinQueryDlgItemText(hwnd, IDD_OPERAND2, OP_SIZE+1,
                                        szBuffer);
                 for (sIndex=0; sIndex<OP_SIZE && szBuffer[sIndex]!='\000';
                             sIndex++)
                 {
                    if( (szBuffer[sIndex]<'0' || szBuffer[sIndex]>'9')
                        && (szBuffer[sIndex] != '.') )
                    {
                       MessageBox(hwnd, IDMSG_ERROROPERAND, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);

                       *pMath->pszOperation = '\000';
                       return((MRESULT)FALSE);          /* Illegal input */
                    }
                 }                                             /* endfor */
                 pMath->fOperand2 = atol(szBuffer);

                 memset(szBuffer,0,sizeof(szBuffer));
                 WinQueryDlgItemText(hwnd, IDD_OPERATION, OP_SIZE,
                                        pMath->pszOperation);
                 if(*pMath->pszOperation!='+' && *pMath->pszOperation!='-' &&
                    *pMath->pszOperation!='*' && *pMath->pszOperation!='/')
                 {
                     MessageBox(hwnd, IDMSG_ERROROPERAND, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);

                     *pMath->pszOperation = '\000';
                     return((MRESULT)FALSE);            /* Illegal input */
                 }
                 memset(pMath->szResult, 0, sizeof(pMath->szResult) );
                 WinDismissDlg(hwnd, TRUE);
                 break;

              case DID_CANCEL:
                 WinDismissDlg(hwnd, FALSE);
                 break;
           }
           break;

       case WM_CLOSE:
           WinDismissDlg(hwnd, FALSE);
           return((MRESULT)TRUE);

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
   }
   return (MRESULT)RETURN_SUCCESS;
}

/****************************************************************\
 *                                                              *
 *  Name:    ResultDlgProc(hwnd, msg, mp1, mp2)                 *
 *                                                              *
 *  Purpose: Processes all messages sent to the result display  *
 *           dialog window.                                     *
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
MRESULT EXPENTRY ResultDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static  PMATHELEMENT pResult;
   CHAR    szBuf[BUFF_SIZE];
   CHAR    szTemp[BUFF_SIZE];
   SHORT   i;
   MRESULT sRC;

   switch (msg)
   {
       case WM_INITDLG:

           pResult = PVOIDFROMMP(mp2);
           FixSysMenu(hwnd);
           memset(szBuf, 0, sizeof(szBuf));
           sprintf(szBuf,"%ld", pResult->fOperand1); /* szBuf= 'OP1'     */
           memset(szTemp, 0, sizeof(szTemp));
           sprintf(szTemp,"%ld", pResult->fOperand2);
           strcat(szBuf," ");                    /* szBuf= 'OP1 '        */
           strcat(szBuf,pResult->pszOperation);  /* szBuf= 'OP1 +'       */
           strcat(szBuf," ");                    /* szBuf= 'OP1 + '      */
           strcat(szBuf, szTemp);                /* szBuf= 'OP1 + OP2'   */
           strcat(szBuf, " ");                   /* szBuf= 'OP1 + OP2 '  */
           strcat(szBuf, "=");                   /* szBuf= 'OP1 + OP2 =' */
                                                 /* Display the formula  */
           WinSetDlgItemText(hwnd, IDD_TEXT, szBuf);
           for (i=BUFF_SIZE-1; i && pResult->szResult[i]!='.'; i--)
           {
              if (pResult->szResult[i] == '0')
                 pResult->szResult[i] = '\000';  /* Erase tail's zero    */
           }                                     /* Display the result   */
           WinSetDlgItemText(hwnd, IDD_VALUE, pResult->szResult);

           return((MRESULT)FALSE);
                         /* Let the focus go to where the system puts it */
       case WM_COMMAND:
           switch (SHORT1FROMMP(mp1))
           {
              case DID_OK:
                 WinDismissDlg(hwnd, TRUE);
                 break;
           }
           break;

       default:
           sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
           return sRC;
   }
   return (MRESULT)RETURN_SUCCESS;
}
