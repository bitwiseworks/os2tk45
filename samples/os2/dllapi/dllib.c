/*static char *SCCSID = "@(#)dllib.c	6.9 92/02/18";*/
/*==============================================================*\
 *  DLLIB.C - dynamic linking library procedures for the window *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the Display file Procedures for the    *
 *  user defined DOS called as well as any code they need       *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *    SearchFile  (pObj, pFile)                                 *
 *    ReadFileProc(pFileText)                                   *
 *    CalCulProc  (pMathEl)                                     *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "dllib.h"

/****************************************************************\
 *                                                              *
 * Name:    SearchFile(pObj, pFile)                             *
 *                                                              *
 * Purpose: This routine searches files which are matching the  *
 *          pattern.                                            *
 *                                                              *
 * Usage:   After files searching, this module will create a    *
 *          linking structure to store the matched files.       *
 *                                                              *
 * Returns: Dependent upon API return code.                     *
 *                                                              *
\****************************************************************/
APIRET SearchFile (POBJSTRUCT pObj, PPFILELIST pFile)
{
   FILEFINDBUF3 findbuf;
   HDIR         hDir = HDIR_CREATE;
   ULONG        ulSearchCount = 1;
   CHAR         szBuf[CCHMAXPATH];
   APIRET       Ret=0;
   PVOID        pvAddress=0;
   PFILELIST    pTail=NULL, pNext=NULL;         /* The last file of list */
                                             /* Change to selected drive */
   Ret = DosSetDefaultDisk((ULONG)(*(pObj->szNewDriver) - '@'));
   if(Ret)
        return (Ret);
                                         /* Change to selected directory */
   memset(szBuf, 0, sizeof(szBuf));
   strcpy(szBuf, pObj->szNewPath);
   if(strlen(szBuf)!= 1 && *(szBuf+strlen(szBuf)-1) == '\\')
       *(szBuf+strlen(szBuf)-1) = '\000';              /* Erase last '\' */
   Ret = DosSetCurrentDir((PSZ)szBuf);
   if(Ret)
         return (Ret);

   DosSetMem(*pFile, sizeof(PFILELIST), PAG_READ|PAG_WRITE|PAG_EXECUTE);

   while(*pFile != NULL)                 /* Free the File list structure */
   {
      pTail = *pFile;
      *pFile = (*pFile)->pNextFile;
      DosFreeMem(pTail);
   }
   Ret = DosFindFirst((PSZ)pObj->szNewPattern, &hDir, FILE_NORMAL,
                &findbuf, sizeof(findbuf), &ulSearchCount, FIL_STANDARD);

   if(Ret)
   {
      if(Ret == ERROR_NO_MORE_FILES)                    /* No file found */
          WinAlarm(HWND_DESKTOP, WA_ERROR);

          WinMessageBox(HWND_DESKTOP,
                   HWND_DESKTOP,
                   "No file is matched this pattern,\ncheck pattern or path.",
                   (PSZ)"Warning !",
                   IDM_MSGBOX,
                   MB_OK | MB_INFORMATION | MB_MOVEABLE);
      return (Ret);
   }
   else
   {
      if(!DosAllocMem(&pvAddress,sizeof(PFILELIST),
                     PAG_READ|PAG_WRITE|PAG_COMMIT))
      {                                      /* Store 1'st file into list */
         memset(szBuf, 0, sizeof(szBuf));
         strcat(szBuf, pObj->szNewDriver);
         strcat(szBuf, pObj->szNewPath);
         strcat(szBuf, findbuf.achName);
         *pFile = (PFILELIST)pvAddress;
         strcpy((*pFile)->szFileName, szBuf);
         (*pFile)->pNextFile = NULL;
      }
   }
   pTail = *pFile;
   while(ulSearchCount)
   {
      Ret=DosFindNext(hDir, &findbuf, sizeof(findbuf), &ulSearchCount);
      if(Ret)                           /* Ret != 0, DosFindNext is fail */
      {
        pNext = *pFile;               /* Protect the memory of file list */
        if(pNext != NULL)
        {
          DosSetMem(pNext,sizeof(PFILELIST), PAG_GUARD|PAG_READ|PAG_WRITE);
        }
        if (Ret != ERROR_NO_MORE_FILES)
           return Ret;                             /* Have error occured */
        else
           return 0;
      }
      memset(szBuf, 0, sizeof(szBuf));        /* More files are searched */
      strcat(szBuf, pObj->szNewDriver);
      strcat(szBuf, pObj->szNewPath);
      strcat(szBuf, findbuf.achName);
                            /* Store all found files to structure pFiles */
      if(!DosAllocMem(&pvAddress,sizeof(PFILELIST),
                     PAG_READ|PAG_WRITE|PAG_COMMIT))
      {                                          /* Store file into list */
         pTail->pNextFile = (PFILELIST)pvAddress;
         pTail = pTail->pNextFile;
         strcpy(pTail->szFileName, szBuf);
         pTail->pNextFile = NULL;
      }
   }
   pNext = *pFile;                    /* Protect the memory of file list */
   if(pNext != NULL)
   {
      DosSetMem(pNext,sizeof(PFILELIST), PAG_GUARD|PAG_READ|PAG_WRITE);
   }
   return 0;                                           /* RETURN_SUCCESS */
}

/****************************************************************\
 *                                                              *
 * Name:    ReadFileProc(pFileText)                             *
 *                                                              *
 * Purpose: This routine open and read file which are selected. *
 *                                                              *
 * Usage:   After files read, this module will store data into  *
 *          a buffer.                                           *
 *                                                              *
 * Returns: Dependent upon API return code.                     *
 *                                                              *
\****************************************************************/
APIRET ReadFileProc(PFILEINFO pFileText)
{
   HFILE    FileHandle=0;                               /* Opened file handle */
   ULONG    ulAct=0L;
   APIRET   Ret;
   ULONG    ulRdByte=0L;                                   /* Bytes be read */

   Ret = DosOpen((PSZ)pFileText->szFileName,
                 (PHFILE)&FileHandle,
                 &ulAct,
                 0L,                   /* Specify file new size for created */
                 FILE_NORMAL,        /* File attributes for create new file */
                 FILE_OPEN,                        /* Open an existing file */
           OPEN_SHARE_DENYWRITE,/* Open modes: others can open to read only */
                 NULL);                    /* Extended attributes structure */
   if (Ret)
      return (Ret);                                    /* RETURN_ERROR code */
   else
   {
      DosRead(FileHandle, (PVOID)&(pFileText->szBuffer), BUFF_SIZE, &ulRdByte);
      if (ulRdByte == 0L)
         return (APIRET)1;                                  /* RETURN_ERROR */
   }
   if(DosClose(FileHandle))
     return (APIRET)1;                                      /* RETURN_ERROR */
   return (APIRET)0;                                      /* RETURN_SUCCESS */
}

/****************************************************************\
 *                                                              *
 * Name:    CalCulProc(pMathEl)                                 *
 *                                                              *
 * Purpose: This routine calculates the result from given       *
 *            operands which are input by user.                 *
 *                                                              *
 * Usage:   After calculated, store the result to Math structure*
 *                                                              *
 * Returns: Dependent upon API return code.                     *
 *                                                              *
\****************************************************************/
APIRET CalCulProc(PMATHELEMENT pMathEl)
{
   double fResultVal=0.0;

   switch (*pMathEl->pszOperation) {
   case '+':
      fResultVal = pMathEl->fOperand1 + pMathEl->fOperand2;
      break;
   case '-':
      fResultVal = pMathEl->fOperand1 - pMathEl->fOperand2;
      break;
   case '*':
      fResultVal = pMathEl->fOperand1 * pMathEl->fOperand2;
      break;
   case '/':
      fResultVal = (double)pMathEl->fOperand1 / (double)pMathEl->fOperand2;
      break;
   }                                                           /* endswitch */
   sprintf(pMathEl->szResult, "%f", fResultVal);
   return (APIRET)0;                                      /* RETURN_SUCCESS */
}
