/*static char *SCCSID = "@(#)easea.c	6.7 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  EASEA.C - sample PM application                             *
 *    (C) Copyright IBM Corporation 1992.                       *
 *--------------------------------------------------------------*
 *    This module contains subroutines for eas.c that           *
 *    specifically deal with the manipulation of EAs.           *
 *                                                              *
 *--------------------------------------------------------------*
 *  Procedures in this file:                                    *
 *   AddEA()           Handles the Add button press             *
 *   QueryEAs()        Reads in all the EAs associated with a file
 *   CheckEAIntegrity()Checks an EA buffer to see if it is valid*
 *   Free_FEAList()    Deallocates memory associated with EA list
 *   LookupEAType()    Gets an offset into table for an EA type *
 *   DeleteCurEA()     Deletes the highlighted EA               *
 *   CurEAType()       Returns EA Type given a HoldFEA ptr      *
 *   GetUSHORT()       Returns nth USHORT in ->aValue           *
 *   WriteEAs()        Updates EAs state on the disk            *
 *   EditEAValue()     Handles editing a given EA ( m-m EAs)    *
 *   EAExists()        Determines if the given EA Name exists   *
 *   ChangeName()      Handles the change of an EA's name       *
 *   MultiTypeIndex()  Gets a specific field in a m-m structure *
 *   EAValueString()   Returns a representational string for  EA*
 *   MultiAdd()        Handles addition of a field for m-m      *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "eas.h"

extern EADATA ConvTable[EATABLESIZE];

/****************************************************************\
 *                                                              *
 *  Name:    AddEA(hwnd)                                        *
 *                                                              *
 *  Purpose: This routine handles the addition of a new EA to   *
 *           the linked list.                                   *
 *                                                              *
 *  Usage  :                                                    *
 *                                                              *
 *  Method : Routine does NOT do full memory error trapping and *
 *           the insert message to l-box is not error checked.  *
 *                                                              *
 *  Returns: TRUE if the EA is successfully added.              *
 *                                                              *
\****************************************************************/
BOOL AddEA(HWND hwnd)
{
   HOLDFEA  *pFEA=pHoldFEA;       /* Points to the beginning of the EA list */
   HOLDFEA  *pNewFEA=NULL;        /* Used to temporarily hold the new EA    */
   PASSDATA PData;

   if(!FILE_ISOPEN)
   {
      MessageBox(hwnd, IDMSG_NOFILEOPEN, "Error !",
                                MB_OK | MB_ERROR, TRUE);
      return(FALSE);
   }
   if(!WinDlgBox(HWND_DESKTOP,                  /* get new EA name and type */
                 hwnd,
                 AddEAProc,
                 '\0',
                 IDD_ADDEA,
                 '\0'))
      return(FALSE);                                    /* they said cancel */

   GetMem((PPVOID)&pNewFEA, sizeof(HOLDFEA));/* Allocate space for new EA struct */

   pNewFEA->cbName = (CHAR) strlen(szEAName);      /* Fill in new structure */
   pNewFEA->cbValue= 0;
   pNewFEA->fEA    = 0;                                 /* Need bit NOT set */
                                               /* Name returned in szEAName */
   GetMem((PPVOID)&pNewFEA->szName, pNewFEA->cbName+1);
   strcpy(pNewFEA->szName,strupr(szEAName));
   pNewFEA->aValue = '\0';
   pNewFEA->next   = '\0';

   if(pHoldFEA == '\0')                   /* It's the first EA for the file */
      pHoldFEA = pNewFEA;
   else                             /* Add EA to the end of the linked list */
   {
      while(pFEA->next)
         pFEA = pFEA->next;
      pFEA->next = pNewFEA;
   }
   PData.Point         = (CHAR *) pNewFEA;      /* Setup user data for call */
   PData.cbMulti       = 0;                     /* to edit of the name and  */
   PData.usMultiOffset = 0;                     /* EA Value                 */

   if(!EditEAValue(hwnd,&PData))                  /* They canceled the edit */
   {
      if(pFEA)                                      /* It's not the only EA */
         pFEA->next = '\0';                /* Disconnect the partial new EA */
      else
         pHoldFEA = '\0';                  /* The new EA was the first one  */

      FreeMem(pNewFEA->szName);
      FreeMem(pNewFEA);

      return(FALSE);
   }
   WinSendDlgItemMsg(hwnd, IDD_WINLBOX, LM_INSERTITEM,/* Insert name in L-Box */
                     MPFROM2SHORT(LIT_END,0),
                     MPFROMP(pNewFEA->szName));
   return(TRUE);
}

/****************************************************************\
 *                                                              *
 *  Name:    QueryEAs(pszPath)                                  *
 *                                                              *
 *  Purpose: Call DOS to Query a file's EA names and values.    *
 *                                                              *
 *  Usage:                                                      *
 *                                                              *
 *  Method : Routine does NOT do full memory error trapping.    *
 *           NOTE:  This routine does NOT prevent other         *
 *           processes from accessing the file's EAs while it is*
 *           reading them in, or while the program is editing   *
 *           them.                                              *
 *                                                              *
 *  Returns: TRUE if it successfully reads in the EAs. Upon exit*
 *           , globalp HoldFEA points to a linked list of the   *
 *           EAs for the current file.                          *
 *                                                              *
\****************************************************************/
BOOL QueryEAs(CHAR *pszPath)
{
   CHAR *pAllocc=NULL; /* Holds the FEA struct returned by DosEnumAttribute */
                       /*  used to create the GEA2LIST for DosQueryPathInfo */
   CHAR *pBigAlloc=NULL; /* Temp buffer to hold each EA as it is read in    */
   USHORT cbBigAlloc=0;  /* Size of buffer                                  */

   ULONG ulEntryNum = 1; /* count of current EA to read (1-relative)        */
   ULONG ulEnumCnt;      /* Number of EAs for Enum to return, always 1      */

   HOLDFEA *pLastIn=0;    /* Points to last EA added, so new EA can link    */
   HOLDFEA *pNewFEA=NULL; /* Struct to build the new EA in                  */

   FEA2 *pFEA;           /* Used to read from Enum's return buffer          */
   GEA2LIST *pGEAList;/*Ptr used to set up buffer for DosQueryPathInfo call */
   EAOP2  eaopGet;       /* Used to call DosQueryPathInfo                   */
                                   /* Allocate enough room for any GEA List */
   GetMem((PPVOID)&pAllocc, MAX_GEA);
   pFEA = (FEA2 *) pAllocc;               /* pFEA always uses pAlloc buffer */

   pHoldFEA = '\0';            /* Reset the pointer for the EA linked list  */

   while(TRUE)                /* Loop continues until there are no more EAs */
   {
      ulEnumCnt = 1;                   /* Only want to get one EA at a time */
      if(DosEnumAttribute(Ref_ASCIIZ,            /* Read into pAlloc Buffer */
                          pszPath,               /* Note that this does not */
                          ulEntryNum,            /* get the aValue field,   */
                          pAllocc,               /* so DosQueryPathInfo must*/
                          MAX_GEA,               /* be called to get it.    */
                          &ulEnumCnt,
                          (LONG) GetInfoLevel1))
        break;                              /* There was some sort of error */

      if(ulEnumCnt != 1)                      /* All the EAs have been read */
         break;

      ulEntryNum++;

      GetMem((PPVOID)&pNewFEA, sizeof(HOLDFEA));

      if (pNewFEA == '\0')                                 /* Out of memory */
      {
         FreeMem(pAllocc);
         Free_FEAList(pHoldFEA,pDelList);
         return (FALSE);
      }

      pNewFEA->cbName = pFEA->cbName;      /* Fill in the HoldFEA structure */
      pNewFEA->cbValue= pFEA->cbValue;
      pNewFEA->fEA    = pFEA->fEA;
      pNewFEA->next = '\0';

      GetMem((PPVOID)&pNewFEA->szName, pFEA->cbName +1); /* Allocate for 2 arrays */
      GetMem((PPVOID)&pNewFEA->aValue, pFEA->cbValue);

      if (!pNewFEA->szName || !pNewFEA->aValue)            /* Out of memory */
      {
         if(pNewFEA->szName)
            FreeMem(pNewFEA->szName);
         if(pNewFEA->aValue)
            FreeMem(pNewFEA->aValue);

         FreeMem(pAllocc);
         FreeMem(pNewFEA);

         Free_FEAList(pHoldFEA,pDelList);
         return (FALSE);
      }
      strcpy(pNewFEA->szName,pFEA->szName);              /* Copy in EA Name */

      cbBigAlloc = sizeof(FEA2LIST) + pNewFEA->cbName+1 +
                              pNewFEA->cbValue;
      GetMem((PPVOID)&pBigAlloc, cbBigAlloc);
      if (pBigAlloc == '\0')
      {
         FreeMem(pNewFEA->szName);
         FreeMem(pNewFEA->aValue);
         FreeMem(pAllocc);
         FreeMem(pNewFEA);
         Free_FEAList(pHoldFEA,pDelList);
         return (FALSE);
      }

      pGEAList = (GEA2LIST *) pAllocc;          /* Set up GEAList structure */

      pGEAList->cbList = sizeof(GEA2LIST) + pNewFEA->cbName; /* +1 for NULL */
      pGEAList->list[0].oNextEntryOffset = 0L;
      pGEAList->list[0].cbName = pNewFEA->cbName;
      strcpy(pGEAList->list[0].szName,pNewFEA->szName);

      eaopGet.fpGEA2List = (GEA2LIST FAR *) pAllocc;
      eaopGet.fpFEA2List = (FEA2LIST FAR *) pBigAlloc;

      eaopGet.fpFEA2List->cbList = cbBigAlloc;

      DosQueryPathInfo(pszPath,                     /* Get the complete EA info */
                       FIL_QUERYEASFROMLIST,
                      (PVOID) &eaopGet,
                       sizeof(EAOP2));
      memcpy(pNewFEA->aValue,                  /* Copy the value to HoldFEA */
             pBigAlloc+sizeof(FEA2LIST)+pNewFEA->cbName,
             pNewFEA->cbValue);

      FreeMem(pBigAlloc);                   /* Release the temp Enum buffer */

      if(!CheckEAIntegrity(pNewFEA->aValue,pNewFEA->cbValue))     /* Bad EA */
      {
         FreeMem(pNewFEA->szName);
         FreeMem(pNewFEA->aValue);
         FreeMem(pNewFEA);
         continue;                      /* Don't add this EA to linked list */
      }

      if(pHoldFEA == '\0')                     /* If first EA, set pHoldFEA */
         pHoldFEA = pNewFEA;
      else                                 /* Otherwise, add to end of list */
         pLastIn->next = pNewFEA;

      pLastIn = pNewFEA;                      /* Update the end of the list */
   }
   FreeMem(pAllocc);                      /* Free up the GEA buf for DosEnum */
   return (TRUE);
}


/****************************************************************\
 *                                                              *
 *  Function name: CheckEAIntegrity(aBuf, cbBuf)                *
 *                                                              *
 *  Returns: TRUE if the buffer is a valid EA structure.        *
 *                                                              *
 *  Purpose: This routine checks the integrity of the passed EA *
 *           buffer by seeing if there are any non-standard EA  *
 *           types, or bad data that isn't sized correctly.     *
 *  Usage  :                                                    *
 *  Method : Routine uses MultiTypeIndex() to check m-m type EAs*
 *           since they are potentially recursive. However, this*
 *           may not be a good idea if the m-m EA is severly    *
 *           corrupted because it may cause MTI() to attempt a  *
 *           read from protected memory. Routine does NOT modify*
 *           the buffer under any circumstance.                 *
 *                                                              *
 *  Returns: TRUE if the buffer is a valid EA structure.        *
 *                                                              *
\****************************************************************/
BOOL CheckEAIntegrity(CHAR *aBuf, ULONG cbBuf)
{
   USHORT *pusPtr = (USHORT *) aBuf;
   USHORT usOffset;
   CHAR   *aEndPtr;

   usOffset = LookupEAType(*pusPtr);                 /* Get the EA type */

   switch(ConvTable[usOffset].usFldType)
   {
      case IDD_LPDATA:
         pusPtr++;
         if(*pusPtr + 2*sizeof(USHORT) == cbBuf)
            return TRUE;
         else
            return FALSE;

      case IDD_MULTILIST:
         if(*pusPtr == EA_MVMT)
         {
            pusPtr += 2;

                /* This checks where the end of the m-m list ends to determine
                   the size of the EA.  This is probably not good if the EA is
                   badly corrupted and it points to protected memory        */

            aEndPtr = MultiTypeIndex(aBuf,*pusPtr);
            if(aEndPtr - aBuf == (CHAR) cbBuf)
               return TRUE;
            else
               return FALSE;
         }
         else            /* Single type, multi-value is not yet implemented */
         {
            return TRUE;
         }
      default:
         return FALSE;
   }
   return TRUE;
}

/****************************************************************\
 *                                                              *
 *  Name:    Free_FEAList(pFEA, pDList)                         *
 *                                                              *
 *  Purpose: This routine frees up the current list of EAs by   *
 *           deallocating the space used by the szName and      *
 *           aValue fields, then deallocating the HoldFEA struct*
 *           Next, it deletes the EAName space, then the        *
 *           DeleteList structure.                              *
 *  Usage :                                                     *
 *  Method:  Note that EAS always passes in pHoldFEA and        *
 *           pDelList which is unnecessary since they are global*
 *           pointers;however, this is done to make the routine *
 *           more flexible by allowing multiple linked lists to *
 *           exist.                                             *
 *                                                              *
 *  Returns: VOID.  The two linked lists passed in are cleaned  *
 *           out though.                                        *
 *                                                              *
\****************************************************************/
VOID Free_FEAList(HOLDFEA *pFEA, DELETELIST *pDList)
{
   HOLDFEA *next;       /* Holds the next field since we free the structure */
                        /* before reading the current next field            */
   DELETELIST *Dnext;                              /* Same purpose as *next */

   while(pFEA)
   {
      next = pFEA->next;
      if(pFEA->szName)                            /* Free if non-NULL name  */
         FreeMem(pFEA->szName);
      if(pFEA->aValue)                            /* Free if non-NULL value */
         FreeMem(pFEA->aValue);

      FreeMem(pFEA);                                 /* Free HoldFEA struct */
      pFEA = next;
   }
   while(pDList)
   {
      Dnext = pDList->next;
      if(pDList->EAName)
         FreeMem(pDList->EAName);
      FreeMem(pDList);
      pDList = Dnext;
   }
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    LookupEAType(usType)                               *
 *                                                              *
 *  Purpose: This routine takes EA type and returns an offset   *
 *           into ConvTable which points to an entry that       *
 *           describes the type passed in.                      *
 *                                                              *
 *  Usage:                                                      *
 *  Method:                                                     *
 *                                                              *
 *  Returns: An offset into the ConvTable to the appropriate    *
 *           entry. If no match is found, the return value      *
 *           points to the last entry, non-conventional format. *
 *                                                              *
\****************************************************************/
ULONG LookupEAType(ULONG usType)
{
   USHORT cnt;

   for(cnt=0;cnt<EATABLESIZE-1;cnt++)
      if(ConvTable[cnt].usPrefix == usType)
         return(cnt);
   return(cnt);
}

/****************************************************************\
 *                                                              *
 *  Name:    DeleteCurEA(hwnd)                                  *
 *                                                              *
 *  Purpose: This routine removes in memory the currently       *
 *           highlighted EA from the EA list.  It places the    *
 *           deleted EA in the global pDelList linked list.     *
 *                                                              *
 *  Usage:   This routine is called by MainDlgProc when DELETE  *
 *           button selected.                                   *
 *                                                              *
 *  Method:  The memory allocation routines are NOT fully error *
 *           trapped.                                           *
 *                                                              *
 *  Returns: VOID.  Removes one item from global pHoldFEA list  *
 *           and adds one to global pDelList.                   *
 *                                                              *
\****************************************************************/
VOID DeleteCurEA(HWND hwnd)
{
   HOLDFEA    *pFEA,*pFEAPrev;
   DELETELIST *pDL=NULL, *pDLcnt;/*Utility ptrs for manipulating the Del list*/
   SHORT      sOffset;
   SHORT      i;

   if(!FILE_ISOPEN)
   {
      MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                               MB_OK | MB_ERROR, TRUE);
      return;
   }
   sOffset = (SHORT) WinSendDlgItemMsg(hwnd, IDD_WINLBOX,
                                         LM_QUERYSELECTION,0,0);
   if(sOffset<0)                           /* No item is currently selected */
   {
      MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                                     MB_OK | MB_ERROR, TRUE);
      return;
   }
   pFEA = GetCurFEA(hwnd,pHoldFEA);     /* Gets a pointer to item to delete */
   if (pFEA == '\0')
      return;
               /* These two allocations should be checked for out of memory */
   GetMem((PPVOID)&pDL, sizeof(DELETELIST));     /* Add Name to Delete List */
   GetMem((PPVOID)&pDL->EAName, pFEA->cbName+1);
   strcpy(pDL->EAName,pFEA->szName);
   pDL->next = '\0';

   if(pDelList == '\0')               /* The del list was previously empty  */
      pDelList = pDL;
   else                               /* tack name onto the end of the list */
   {
      pDLcnt = pDelList;
      while(pDLcnt->next)
         pDLcnt = pDLcnt->next;
      pDLcnt->next = pDL;
   }
   WinSendDlgItemMsg(hwnd, IDD_WINLBOX,
                     LM_DELETEITEM,MPFROMSHORT(sOffset),0L);

   if(sOffset<1)                        /* Remove pFEA from the linked list */
   {
      pHoldFEA = pFEA->next;
   }
   else
   {
      pFEAPrev = pHoldFEA;

      --sOffset;
      while(sOffset)                                    /* Find previous EA */
      {
         pFEAPrev = pFEAPrev->next;
         --sOffset;
      }
      pFEAPrev->next = pFEA->next;
   }
                           /* Check Type Name is in the list of MVST or not */
   for (i=0; i<MSNUM && MSdata[i].szName!=NULL &&
                        strcmp(pFEA->szName,MSdata[i].szName)!=0;)
          i++;

   if(i<MSNUM && MSdata[i].szName!=NULL)         /* Found the EA type in MVST list */
   {
      FreeMem(MSdata[i].szName);
      for (; i<MSNUM && MSdata[i+1].szName!=NULL; i++)
      {
         MSdata[i].szName = MSdata[i+1].szName;
         MSdata[i].usType = MSdata[i+1].usType;
      }
   }
   FreeMem(pFEA->szName);                             /* Release the memory */
   FreeMem(pFEA->aValue);
   FreeMem(pFEA);

   FILE_CHANGED = TRUE;
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    CurEAType(pFEA)                                    *
 *                                                              *
 *  Purpose: Given an EA structure, this routine returns the    *
 *           Type of the EA which resides in the first USHORT of*
 *           the aValue member. This function is the same as    *
 *           GetUSHORT(pFEA,0).                                 *
 *  Usage  :                                                    *
 *  Method : Assumes a valid HoldFEA struct.                    *
 *                                                              *
 *  Returns: The EA type.                                       *
 *                                                              *
\****************************************************************/
ULONG CurEAType(HOLDFEA *pFEA)                   /* Same as GetUSHORT(,0); */
{
   USHORT *pusType;    /* EA Type is stored in first USHORT of aValue field */

   pusType = (USHORT *) pFEA->aValue;
   return(*pusType);
}

/****************************************************************\
 *                                                              *
 *  Name:    GetUSHORT(pFEA, index)                             *
 *                                                              *
 *  Purpose: This routine returns the nth USHORT value in the   *
 *           aValue member of pFEA using index as the offset.   *
 *  Usage  :                                                    *
 *  Method : Assumes a valid HoldFEA struct and that index      *
 *           doesn't point outside the aValue buffer.           *
 *                                                              *
 *  Returns: The appropriate USHORT from the aValue field       *
 *                                                              *
\****************************************************************/
USHORT GetUSHORT(HOLDFEA *pFEA, ULONG index)
{
   USHORT *pusType;

   pusType = (USHORT *) pFEA->aValue;
   while(index-- > 0)
      pusType++;
   return(*pusType);
}

/****************************************************************\
 *                                                              *
 *  Name:    WriteEAs()                                         *
 *                                                              *
 *  Purpose: This routine updates the EAs on disk to reflect    *
 *           their current condition in memory.  First, all EAs *
 *           in the delete list are removed from the disk, then *
 *           all EAs in the pHoldFEA list are written to disk.  *
 *  Usage  :                                                    *
 *  Method : This routine is not bulletproof as it does not get *
 *           exclusive access to the file EAs, nor does it      *
 *           handle out of disk space sort of errors. Also,     *
 *           memory fetches are not fully error trapped.        *
 *                                                              *
 *  Returns: VOID.  But cleans out the pDelList linked list.    *
 *                                                              *
\****************************************************************/
VOID WriteEAs()
{
   DELETELIST *pDL = pDelList,*pDLnext;
   HOLDFEA    *pHFEA= pHoldFEA;
   EAOP2       eaopWrite;
   CHAR       aBuf[MAX_GEA],*aPtr=NULL;
   FEA2        *pFEA = (FEA2 *) &aBuf[sizeof(ULONG)];
   USHORT     usMemNeeded, usRet;
   ULONG      *pulPtr=(ULONG *) aBuf;  /* Initally points to top of FEALIST */

   if(!FILE_ISOPEN)
   {
      MessageBox(HWND_DESKTOP, IDMSG_NOFILEOPEN, "Error !",
                                MB_OK | MB_ERROR, TRUE);
      return;
   }
   if (!FILE_CHANGED)                  /* Don't write unless it's necessary */
      return;
   eaopWrite.fpFEA2List = (FEA2LIST FAR *) aBuf; /* Setup fields that won't */
   pFEA->fEA     = 0;                            /* change for the delete   */
   pFEA->cbValue = 0;                            /* calls to DosSetPathInfo */

   while(pDL)                         /* Clean out all the deleted EA names */
   {
      pFEA->cbName = (UCHAR) strlen(pDL->EAName);
      *pulPtr      = sizeof(FEA2LIST) + pFEA->cbName;
      strcpy(pFEA->szName,pDL->EAName);
      pFEA->oNextEntryOffset = 0L;
                                         /* Delete EA's by saying cbValue=0 */
      DosSetPathInfo(szFileName,
                     SetInfoLevel2,
                     (PVOID) &eaopWrite,
                     sizeof(EAOP2),
                     DSPI_WRTTHRU);
      pDLnext = pDL->next;                            /* Temp hold next pDL */
      FreeMem(pDL->EAName);                   /* Free up current Del struct */
      FreeMem(pDL);
      pDL = pDLnext;                              /* Set pDL to saved value */
   }
   pDelList = '\0';                                 /* DelList is now empty */

   while(pHFEA)                                  /* Go through each HoldFEA */
   {
      usMemNeeded = sizeof(FEA2LIST) + pHFEA->cbName+1 +
                                pHFEA->cbValue;
      GetMem((PPVOID)&aPtr, usMemNeeded);

      eaopWrite.fpFEA2List = (FEA2LIST FAR *) aPtr;  /* Fill in eaop struct */
      eaopWrite.fpFEA2List->cbList = usMemNeeded;

      eaopWrite.fpFEA2List->list[0].fEA     = pHFEA->fEA;
      eaopWrite.fpFEA2List->list[0].cbName  = pHFEA->cbName;
      eaopWrite.fpFEA2List->list[0].cbValue = pHFEA->cbValue;

      strcpy(eaopWrite.fpFEA2List->list[0].szName, pHFEA->szName);
      memcpy(eaopWrite.fpFEA2List->list[0].szName + pHFEA->cbName+1,
             pHFEA->aValue, pHFEA->cbValue);

      usRet = DosSetPathInfo(szFileName,         /* Write out the EA */
                             FIL_QUERYEASIZE,
                             (PVOID) &eaopWrite,
                             sizeof(EAOP2),
                             DSPI_WRTTHRU);
      if (usRet)
      {
          MessageBox(HWND_DESKTOP, IDMSG_WRITEERROR, "Error !",
                                  MB_OK | MB_ERROR, TRUE);
          FreeMem(aPtr);                      /* Free up the FEALIST struct */
          FILE_CHANGED = FALSE;
          return;
      }
      FreeMem(aPtr);                          /* Free up the FEALIST struct */

      pHFEA = pHFEA->next;
   }
   FILE_CHANGED = FALSE;
   MessageBox(HWND_DESKTOP, IDMSG_WRITEOK, "Note !", MB_OK, FALSE);
   return;
}

/****************************************************************\
 *                                                              *
 *  Function name: EditEAValue(hwnd, pPDat)                     *
 *                                                              *
 *  Purpose: This routine allows the entry/edit of an EA value  *
 *           condition in memory.  First, all EAs in the delete *
 *           list are removed from the disk, then all EAs in the*
 *           pHoldFEA list are written out to disk.             *
 *  Usage  :                                                    *
 *  Method : Expects the PassData structure to tell it the      *
 *           HoldFEA to edit, and if it is a subfield of a multi*
 *           -multi EA, the rest of the PassData structure will *
 *           be filled into indicated which m-m is being edited.*
 *           Note that if this is a new edit, usRetEAType is    *
 *           expected to be set to the proper EA type upon entry*
 *    NOTE:  memory sizing  requests are not fully error trapped*
 *                                                              *
 *  Returns: TRUE if the edit was successful.                   *
 *                                                              *
\****************************************************************/
BOOL EditEAValue(HWND hwnd, PPASSDATA pPDat)
{
   USHORT usEAType;                    /* Holds the field type to be edited */
   USHORT *pusPtr;
   USHORT usSize;  /* Holds the delta difference of the old and new buffers */
   CHAR   *szNew=NULL,*szTrash;                            /* Temporary pointers */
   PASSDATA PDat;
   HOLDFEA *pFEA = (HOLDFEA *) pPDat->Point;         /* The EA to be edited */

                            /* Determine the type of EA that will be edited */
   if(pPDat->cbMulti)                             /* It's a multi-type job  */
   {
      pusPtr = (USHORT *) MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                         pPDat->usIndex);
      usEAType = *pusPtr;
   }
   else if(pFEA->cbValue)                         /* It isn't a new EA name */
   {
      pusPtr   = (USHORT *)pFEA->aValue;
      usEAType = *pusPtr;
   }
   else                                                    /* It's a new EA */
      usEAType = ConvTable[usRetEAType].usPrefix;

   PDat.Point   = pFEA->szName;          /* General setup for AsciiEditProc */
                                                  /* =1 if there is a multi */
   PDat.usIndex =(USHORT)( pPDat->cbMulti ? 1 : 0);
   PDat.fFlag   = (BYTE) ((pFEA->fEA & 0x80) ? TRUE : FALSE);

   switch(usEAType)
   {
      case EA_ASCIIZ:
      case EA_ASCIIZFN:
      case EA_ASCIIZEA:
      case EA_ASN1:
         if(pPDat->cbMulti)                     /* It is a multi-type field */
            szAscii=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                   pPDat->usIndex)
                    +sizeof(USHORT);
         else if(pFEA->cbValue)                 /* There is a current value */
            szAscii=pFEA->aValue+sizeof(USHORT);
         else                                              /* It's a new EA */
            szAscii='\0';

         if(!WinDlgBox(HWND_DESKTOP,               /* Do an ascii text edit */
                       hwnd,
                       AsciiEditProc,
                       '\0',
                       IDD_ASCIIEDIT,
                       &PDat))
            return(FALSE);                              /* They said cancel */

         if(PDat.fFlag)                         /* Handle the need/nice bit */
            PDat.fFlag = 0x80;
         if(PDat.fFlag != (BYTE)(PDat.fFlag & 0x80))
            FILE_CHANGED = TRUE;
         pFEA->fEA =(BYTE)( (pFEA->fEA & 0x7f) | PDat.fFlag );

         if(stricmp(strupr(szEAName),pFEA->szName))     /* The name changed */
            ChangeName(pFEA,szEAName);

         if(pFEA->cbValue)                      /* There is a current value */
         {
            if(!strcmp(szAscii,szScratch))             /* It hasn't changed */
               return(TRUE);

            if(pPDat->cbMulti)            /* Do the whole thing here if m-m */
            {                                             /* Change in size */
               usSize = (strlen(szScratch)-strlen(szAscii));

               if(usSize > 0)                   /* The new string is longer */
               {
                  ResizeMem((PPVOID)&pFEA->aValue,           /* Enlarge the EA size */
                            pFEA->cbValue,
                            pFEA->cbValue+usSize);
                  szTrash=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                               (pPDat->usIndex+1));
                  memmove(szTrash+usSize,    /* Move end of EA to make room */
                          szTrash,
                          pFEA->cbValue-(szTrash-pFEA->aValue));
               }
               else
               {
                  szTrash= MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                               (pPDat->usIndex+1));
                  memmove(szTrash+usSize,    /* Move back the end of the EA */
                          szTrash,
                          pFEA->cbValue-(szTrash-pFEA->aValue));
                  DosSetMem(pFEA->aValue, pFEA->cbValue+usSize, fPERM|PAG_COMMIT);
               }
               szTrash=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                      pPDat->usIndex);
               strcpy(szTrash+sizeof(USHORT),szScratch); /* Copy in new val */
               pFEA->cbValue+=usSize;              /* Change buffer count   */

               FILE_CHANGED = TRUE;
               return(FILE_CHANGED);                  /* Done with m-m edit */
            }
            else
            {
               FreeMem(pFEA->aValue);              /* Release old Value mem */
            }
         }
         GetMem((PPVOID)&szNew, strlen(szScratch)+3); /* +3 for Type & NULL */
         pusPtr = (USHORT *) szNew;
         *pusPtr= usEAType;                       /* Set type in new buffer */
         strcpy(szNew+2,szScratch);                /* Copy in the new value */
         pFEA->aValue = szNew;                      /* Fix up the structure */
         pFEA->cbValue= (USHORT)(strlen(szScratch)+3);

         FILE_CHANGED = TRUE;
         return(FILE_CHANGED);

      case EA_LPBITMAP:
      case EA_LPICON:
      case EA_LPBINARY:
      case EA_LPASCII:
      case EA_LPMETAFILE:
         if (usEAType==EA_LPBITMAP || usEAType==EA_LPICON)
         {
            WinDlgBox(HWND_DESKTOP,
                      hwnd,
                      IconDlgProc,
                      '\0',
                      IDD_ABOUTBOX,
                      NULL);
         }
         if(pPDat->cbMulti)                     /* It is a multi-type field */
         {   /* szTrash points to field to edit, pusPtr to the field length */
            szTrash=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                   pPDat->usIndex);
            pusPtr = (USHORT *) ((CHAR *) szTrash + sizeof(USHORT));
            usSize = *pusPtr;
            if(usSize)                                 /* It isn't a new EA */
            {                               /* Set up inital value for edit */
               GetMem((PPVOID)&szAscii, usSize+1);
               memcpy(szAscii,szTrash+2*sizeof(USHORT),usSize);
               *( ( CHAR *)szAscii + usSize ) ='\0';
            }
            else                                         /* No inital value */
               szAscii = '\0';
         }
         else if(pFEA->cbValue)    /* Single type && there is current value */
         {
            usSize=GetUSHORT(pFEA,1);      /* Get size and set inital value */
            if(usSize)
            {
               GetMem((PPVOID)&szTrash, usSize+1);           /* +1 for null */
               memcpy(szTrash,pFEA->aValue+4,usSize);
               szTrash[usSize]='\0';
               szAscii=szTrash;
            }
            else
               szAscii = '\0';
         }
         else
            szAscii = '\0';

         if(!WinDlgBox(HWND_DESKTOP,               /* Do an ascii text edit */
                       hwnd,
                       AsciiEditProc,
                       '\0',
                       IDD_ASCIIEDIT,
                       &PDat))
         {     /* Cancel, but check if memory needs to be freed before exit */
            if(pPDat->cbMulti || pFEA->cbValue)
               if(szAscii)                                 /* It's not NULL */
                  FreeMem(szAscii);                          /* +1 for NULL */
            return(FALSE);
         }
         if(PDat.fFlag)                         /* Handle the need/nice bit */
            PDat.fFlag = 0x80;
         if(PDat.fFlag != (BYTE)(PDat.fFlag & 0x80))
            FILE_CHANGED = TRUE;
         pFEA->fEA = (BYTE)((pFEA->fEA & 0x7f) | PDat.fFlag);

         if(stricmp(strupr(szEAName),pFEA->szName))     /* The name changed */
            ChangeName(pFEA,szEAName);

         if(pFEA->cbValue)                      /* There is a current value */
         {
            if(szAscii && !strcmp(szAscii,szScratch))  /* It hasn't changed */
            {
               if(szAscii)
                  FreeMem(szAscii);
               return(TRUE);
            }
            if(szAscii)                        /* Free default value buffer */
               FreeMem(szAscii);

            if(pPDat->cbMulti)     /* Do the whole thing here is multi-type */
            {                                              /* Change in len */
               USHORT usDelta = (USHORT)(strlen(szScratch) - usSize);
               if(usDelta > 0)    /* The new string is longer, resize first */
               {
                  ResizeMem((PPVOID)&pFEA->aValue, pFEA->cbValue, pFEA->cbValue+usDelta);
                  szTrash= MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                         (pPDat->usIndex+1));
                  memmove(szTrash+usDelta,szTrash,
                          pFEA->cbValue-(szTrash-pFEA->aValue));
               }
               else
               {
                  szTrash= MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                         (pPDat->usIndex+1));
                  memmove(szTrash+usDelta,szTrash,
                          pFEA->cbValue-(szTrash-pFEA->aValue));
                  ResizeMem((PPVOID)&pFEA->aValue, pFEA->cbValue+usDelta, fPERM|PAG_COMMIT);
               }
               szTrash=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                      pPDat->usIndex);
               memmove(szTrash+2*sizeof(USHORT),szScratch,strlen(szScratch));
               pusPtr = (USHORT *) ((CHAR *)szTrash + sizeof(USHORT));
               *pusPtr= strlen(szScratch); /* Set the length field */

               pFEA->cbValue += usDelta;         /* Adjust struct len field */

               FILE_CHANGED = TRUE;
               return(FILE_CHANGED);
            }
            FreeMem(pFEA->aValue);                     /* Free up old value */
         }
         GetMem((PPVOID)&szNew, strlen(szScratch)+4); /* Get space for new value */
         pusPtr = (USHORT *) szNew;
         *pusPtr= usEAType;                               /* Set type field */
         pusPtr++;
         *pusPtr= strlen(szScratch);            /* Set length field */
         memcpy(szNew+4,szScratch,*pusPtr);            /* Copy in new value */
         pFEA->aValue = szNew;                           /* Adjust pointers */
         pFEA->cbValue= strlen(szScratch)+4;/* +4 for type and LP cnt */

         FILE_CHANGED = TRUE;
         return(FILE_CHANGED);

      case EA_MVST:
      case EA_MVMT:                          /* It's multi-value multi-type */
         if(pFEA->cbValue == 0)                            /* It's a new EA */
         {                                         /* Allocate empty m-m EA */
            GetMem((PPVOID)&pFEA->aValue, 3*sizeof(USHORT));
            pFEA->cbValue = 3*sizeof(USHORT);
            pusPtr      = (USHORT *) pFEA->aValue;
            if(usEAType == EA_MVMT)
               *pusPtr = 0xffdf;                 /* Multi-value, multi-type */
            else
               *pusPtr = 0xffde;                /* Multi-value, single-type */
            pusPtr+=2;                               /* Skip type, codepage */
            *pusPtr = 0;                 /* No fields initially     */
            FILE_CHANGED = TRUE;
         }
                                                   /* Set up passed in data */
         if(pPDat->cbMulti)                       /* It's a multi-type job  */
         {
            szNew   = MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                     pPDat->usIndex);
            szTrash = MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                           (pPDat->usIndex+1));
            PDat.usMultiOffset = (USHORT)(szNew - pFEA->aValue);
            PDat.cbMulti       = (USHORT)(szTrash - szNew);
         }
         else
         {
            PDat.usMultiOffset = 0;
            PDat.cbMulti       = pFEA->cbValue;
         }
         PDat.Point = (CHAR *) pFEA;

         WinDlgBox(HWND_DESKTOP,                  /* Do the Multi-type edit */
                   hwnd,
                   MultiTypeProc,
                   '\0',
                   IDD_MULTIBOX,
                   &PDat);
         return(TRUE);

      default:
         return(FALSE);
   }
   return(FALSE);
}

/****************************************************************\
 *                                                              *
 *  Name:    EAExists(szEAName)                                 *
 *                                                              *
 *  Purpose: This routine goes through the linked list pointed  *
 *           to by global pHoldFEA and determines whether or not*
 *           an EA othe passed name already exists.             *
 *  Usage:                                                      *
 *  Method : The comparison is NOT case sensitive.              *
 *                                                              *
 *  Returns: TRUE if an EA with a name matching szEAName exists.*
 *                                                              *
\****************************************************************/
BOOL EAExists(CHAR *szEAName)
{
   HOLDFEA *phFEA=pHoldFEA;

   while(phFEA)
   {
      if(!stricmp(szEAName,phFEA->szName))
         return(TRUE);
      phFEA=phFEA->next;
   }
   return(FALSE);
}

/****************************************************************\
 *                                                              *
 *  Name:    ChangeName(pFEA, szName)                           *
 *                                                              *
 *  Purpose: This routine copies the current EA Name to the     *
 *           delete list, then allocates a new space, copies the*
 *           new name into it, and sets the FEA pointer to it.  *
 *  Usage :                                                     *
 *  Method : Not all the memory allocations are fully error     *
 *           trapped.                                           *
 *                                                              *
 *  Returns: VOID.  Fixes up the pFEA structure and global      *
 *           pDelList.                                          *
 *                                                              *
\****************************************************************/
VOID ChangeName(HOLDFEA *pFEA, CHAR *szName)
{
   CHAR *szTemp=NULL;
   DELETELIST *pDL=NULL;

   GetMem((PPVOID)&szTemp, strlen(szName+1));/* Allocate space for new name */
   if(!szTemp)
      return;
                                            /* Allocate a new delete struct */
   GetMem((PPVOID)&pDL,  sizeof(DELETELIST));
   pDL->EAName = pFEA->szName;                 /* Fill in DeleteList struct */
   pDL->next   = pDelList;
   pDelList    = pDL;

   strcpy(szTemp,szName);                  /* Copy name to permanent buffer */
   pFEA->szName = szTemp;                  /* Fix up struct                 */
   pFEA->cbName = (CHAR) strlen(szName);

   FILE_CHANGED = TRUE;
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    MultiTypeIndex(pMulti, usIndex)                    *
 *                                                              *
 *  Purpose: This routine takes a pointer to a Multi-Multi data *
 *           field and returns a pointer to the nth data field  *
 *           in this buffer.                                    *
 *  Usage:                                                      *
 *  Method:  Memory bounds are not checked and a corrupt EA field
 *           could cause unspecified results. Recursively calls *
 *           itself to handle nesting. Does not support multi-  *
 *           value single type fields.                          *
 *                                                              *
 *  Returns: a pointer to the field specified by the usIndex    *
 *           parameters.                                        *
 *                                                              *
 *  Calls: LookupEAType, MultiTypeIndex()                       *
 *                                                              *
\****************************************************************/
CHAR *MultiTypeIndex(CHAR *pMulti, ULONG usIndex)
{
   USHORT *pusPtr;
   USHORT usOffset;
                               /* skip over 0xffdf, codepage, and field cnt */
   pMulti += 3*sizeof(USHORT);

   while(usIndex--)                 /* loop to skip over correct # of flds  */
   {
      pusPtr   = (USHORT *) pMulti;   /* The address of EA data type stored */
      usOffset = LookupEAType(*pusPtr);         /* Get offset of field type */

      pMulti += sizeof(USHORT);   /* Skip over the type field to first data */

      switch(ConvTable[usOffset].usFldType)
      {
         case IDD_ASCIIZ:
            while(*pMulti)                 /* Increment to point after NULL */
                pMulti++;
            pMulti++;
            break;

         case IDD_LPDATA:
            pusPtr = (USHORT *) pMulti;         /* Get the length of a data */
            pMulti += *pusPtr + sizeof(USHORT);              /* skip to end */
            break;

         case IDD_MULTILIST:
            if(*pusPtr == EA_MVMT)  /* m-m, do a recursive call to skip fld */
            {
               pusPtr = (USHORT *) pMulti;           /* points to field cnt */
               pMulti = MultiTypeIndex(pMulti-sizeof(USHORT),*pusPtr);
               break;
            }
               /* implemented for Multi-valued single-type stuff... */
            pusPtr = (USHORT *) pMulti;         /* Get the length of a data */
            pMulti += *pusPtr + sizeof(USHORT);              /* skip to end */
            break;
      }
   }
   return(pMulti);
}

/****************************************************************\
 *                                                              *
 *  Name:    EAValueString(aEAVal)                              *
 *                                                              *
 *  Purpose: This routine takes a pointer to an EA Value (i.e.  *
 *           starting with $ffxx) and returns a pointer to a    *
 *           string representing the value of the EA. This string
 *           must be Freed by the user when finished with it.   *                          *
 *  Usage:                                                      *
 *  Method : Not all GetMem's are totally error trapped. The    *
 *           string returned is allocated in this procedure, but*
 *           it is the caller's responsibility to free the buffer
 *                                                              *
 *  Returns: a pointer to an ASCII description of the field value
 *                                                              *
\****************************************************************/
CHAR *EAValueString(CHAR *aEAVal)
{
   USHORT *pusPtr= (USHORT *) aEAVal;              /* Points to EA Type     */
   CHAR *szRet=NULL, *szTemp;                    /* szRet points to return string */

   switch(*pusPtr)
   {
      case EA_ASCIIZ:     /* For asciiz strings, return MAXSHOWSIZE-1 chars */
      case EA_ASCIIZFN:
      case EA_ASCIIZEA:
      case EA_ASN1:
         aEAVal += sizeof(USHORT);
         if(strlen(aEAVal)<MAXSHOWSIZE)
         {
            GetMem((PPVOID)&szRet, strlen(aEAVal)+1);
            strcpy(szRet,aEAVal);
         }
         else
         {
            GetMem((PPVOID)&szRet, MAXSHOWSIZE);
            strncpy(szRet,aEAVal,MAXSHOWSIZE-4);
            strcpy (szRet+MAXSHOWSIZE-4,"...");
            szRet[MAXSHOWSIZE-1]='\0';
         }
         return(szRet);

      case EA_LPASCII:           /* Display up to first MAXSHOWSIZE-1 chars */
      case EA_LPMETAFILE:
         pusPtr++;
         aEAVal += 2*sizeof(USHORT);
         if(*pusPtr < MAXSHOWSIZE)
         {
            GetMem((PPVOID)&szRet, *pusPtr +1);
            strncpy(szRet,aEAVal,*pusPtr);
            szRet[*pusPtr]='\0';
         }
         else
         {
            GetMem((PPVOID)&szRet, MAXSHOWSIZE);
            strncpy(szRet,aEAVal,MAXSHOWSIZE-4);
            strcpy (szRet+MAXSHOWSIZE-4,"...");
            szRet[MAXSHOWSIZE-1]='\0';
         }
         return(szRet);
                 /* For the rest of the types, just display the field type */
      case EA_LPBINARY:
         szTemp = "*** LP Binary ***";
         break;

      case EA_LPBITMAP:
         szTemp = "*** LP Bitmap ***";
         break;

      case EA_LPICON:
         szTemp = "*** LP Icon ***";
         break;

      case EA_MVMT:
         szTemp = "*** Multi-value Multi-type ***";
         break;

      case EA_MVST:
         szTemp = "*** Multi-value Single-type ***";
         break;

      default:
         szTemp = "*** Unknown EA type ***";
         break;

   }
   GetMem((PPVOID)&szRet, strlen(szTemp)+1); /* Copy string from static to dynamic */
   strcpy(szRet,szTemp);
   return(szRet);
}

/****************************************************************\
 *                                                              *
 *  Function name: MultiAdd(hwnd, pFEA, pPDat)                  *
 *                                                              *
 *  Purpose: This routine is called by MultiTypeProc and handles*
 *           the addition of a subvalue to a multi-value, multi-*
 *           type EA.                                           *
 *  Usage :                                                     *
 *  Method : Not all GetMem's are totally error trapped.  It is *
 *           possible that the add to the listbox could fail.   *
 *                                                              *
 *  Returns: VOID.  Modifies the current pFEA.                  *
 *                                                              *
\****************************************************************/
VOID MultiAdd(HWND hwnd, HOLDFEA *pFEA, PPASSDATA pPDat)
{
   USHORT   usSize, i;
   USHORT   *pusPtr;
   CHAR     aUtility[6];        /* Used to hold the header for all EA types */
   CHAR     *pInsert,*pValue;
   PASSDATA PDat;

   PDat.Point = pFEA->szName;
                           /* Check Type Name is in the list of MVST or not */
   for (i=0; i<MSNUM && MSdata[i].szName!=NULL &&
                             strcmp(PDat.Point,MSdata[i].szName)!=0;)
         i++;

   if(ConvTable[usRetEAType].usPrefix != EA_MVST && i==MSNUM)
   {
     if(!WinDlgBox(HWND_DESKTOP,                   /* Get the name and type */
                   hwnd,
                   AddEAProc,
                   '\0',
                   IDD_ADDEA,
                   &PDat))
        return;                                         /* They said cancel */
   }
   else                    /* MVST */
   {
     if (i < MSNUM && MSdata[i].szName!=NULL && MSdata[i].usType<10)
         usRetEAType = MSdata[i].usType;             /* EAname & type exist */
     else
        if(!WinDlgBox(HWND_DESKTOP,      /* EAname is new, get the new type */
                      hwnd,
                      AddEAProc,
                      '\0',
                      IDD_ADDEA,
                      &PDat))
           return;                                      /* They said cancel */
   }
   pusPtr = (USHORT *) aUtility;
   *pusPtr= ConvTable[usRetEAType].usPrefix;  /* Set the type in header buf */

   switch(ConvTable[usRetEAType].usFldType)
   {
      case IDD_ASCIIZ:           /* make buffer look like: xx FF 00, size 3 */
         usSize = 3;
         aUtility[2]=0;
         break;

      case IDD_LPDATA:    /* make the buffer look like: xx FF 00 00, size 4 */
         usSize = 4;
         pusPtr = (USHORT *) &aUtility[2];
         *pusPtr= 0;
         break;

      case IDD_MULTILIST:
         usSize = 6;
         pusPtr = (USHORT *) &aUtility[2];
         *pusPtr= 0;                                   /* Zero out codepage */
         pusPtr++;
         *pusPtr= 0;                                    /* Zero out fld cnt */
         break;
   }
                               /* Increase EA size to accomodate the header */
   ResizeMem((PPVOID)&pFEA->aValue, pFEA->cbValue, pFEA->cbValue+usSize);

   pusPtr  = (USHORT *) ((CHAR *) pFEA->aValue + pPDat->usMultiOffset);
   pusPtr+=2;                  /* Point to the current number of m-m fields */

        /* Get ptr to beginning of current EA, scoot the rest down and insert
         the 3-4 byte header at the end of the list.                        */
   pInsert = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset, *pusPtr);
   memmove(pInsert+usSize,pInsert, pFEA->cbValue-(pInsert-pFEA->aValue));
   memcpy(pInsert,aUtility,usSize);

   pFEA->cbValue += usSize;                            /* Fix up the counts */
   pPDat->cbMulti+= usSize;
                                                   /* Set the PDat for call */
   PDat.Point         = (CHAR *) pFEA;
   PDat.cbMulti       = pPDat->cbMulti;
   PDat.usMultiOffset = pPDat->usMultiOffset;
   PDat.usIndex       = *pusPtr;

   if(!EditEAValue(hwnd,&PDat))                   /* They canceled the edit */
   {          /* Move the EA's back to effectively kill the inserted header */
      memmove(pInsert,pInsert+usSize,pFEA->cbValue-(pInsert-pFEA->aValue));
      DosSetMem(pFEA->aValue, pFEA->cbValue-usSize, fPERM|PAG_COMMIT);
      pFEA->cbValue -= usSize;                           /* Adjust counters */
      pPDat->cbMulti-= usSize;

      return;
   }
        /* Reset pusPtr since EditEAValue could have moved the base address */
   pusPtr  = (USHORT *) ((CHAR *) pFEA->aValue + pPDat->usMultiOffset);
   pusPtr+=2;

   pInsert = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset, *pusPtr);

   *pusPtr += 1;                /* Field cnt incremented AFTER call to Edit */

   pValue = EAValueString(pInsert);        /* Add new field to the list box */

   WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_INSERTITEM,
                     MPFROM2SHORT(LIT_END,0),
                     MPFROMP(pValue));
   FreeMem(pValue);

   FILE_CHANGED = TRUE;
   return;
}
