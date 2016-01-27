#pragma title ("HHPHEAP.C - Dynamic Memory Management")

/************************ START OF SPECIFICATIONS ***************************/
/*                                                                          */
/* SOURCE FILE NAME:  HHPHEAP.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: HHP Memory Management                                  */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*                   Copyright (c) IBM Corporation 1991, 1992               */
/*                          All Rights Reserved                             */
/*                                                                          */
/* STATUS: OS/2 MM Release 1.00                                             */
/*                                                                          */
/* FUNCTION: Memory Management                                              */
/*                                                                          */
/* NOTES:                                                                   */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                  */
/*                                                                          */
/*    This is a Heap Manager that can manage heaps larger than 64K.  It     */
/*    can grow and shrink dynamically.  It is intended to hide some of the  */
/*    memory management differences between 16-bit and 32-bit.              */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*                                                                          */
/*   ROUTINES: HhpCreateHeap                                                */
/*             HhpAllocMem                                                  */
/*             HhpFreeMem                                                   */
/*             HhpDestroyHeap                                               */
/*             HhpAccessHeap                                                */
/*             HhpReleaseHeap                                               */
/*             HhpAllocBuffer                                               */
/*             HhpFreeBuffer                                                */
/*             HhpAccessBuffer                                              */
/*             HhpGetPID                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*                                                                          */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

/*
 *  Include files
 */

#define INCL_BASE           // include BASE OS/2 defines
#define INCL_NOPMAPI        // leave out the PM stuff
#include <os2.h>            // all-inclusive OS/2 include file
#include <string.h>         //dwk
#include <stdlib.h>         //dwk

#include <hhpheap.h>        // External function prototypes

/* Performance stub for pagetuned pragma header files */
#ifndef PERFSTUB
  #include "perfstub.h"
  #define  PERFSTUB
#endif

/*
 *  DLL global data
 */

#define PID_MAX    50

#define SEG_NONSHARED       0x0000    //dwk
#define SEG_GIVEABLE        0x0001    //dwk
#define SEG_GETTABLE        0x0002    //dwk

/*
 *  Local Symbols
 */

#define HEAP_MAGIC     0x55

typedef struct _PIDINFO {       /* pidi */  //dwk
    PID     pid;                            //dwk
    TID     tid;                            //dwk
    PID     pidParent;                      //dwk
} PIDINFO;                                  //dwk

typedef struct _HBLOCK {                   /* should be dword multiple */
   struct _HBLOCK FAR * pNext;             /* Next Heap Descriptor */
   struct _HBLOCK FAR * pPrev;             /* Prev Heap Descriptor */
   ULONG   lBlockSize;                     /* Block Size */
   UCHAR   fAlloc;                         /* Block Alloced */
   UCHAR   magic;                          /* Used for Magic Number */
   USHORT  usReserved;                     /* To DWORD align the struct */  //dwk
#ifdef MMDEBUG
   CHAR    szFile[16];
   ULONG   uLine;
#endif
} BLOCK;
typedef BLOCK FAR  * PBLOCK;


typedef struct _HHEAP {
   struct _HHEAP FAR * pNext;              /* Next Heap */
   PPID     ppidList;                      /* Pointer to pid List */
   PBLOCK   pBlock;                        /* Heap Block List */
   USHORT   uAllocCnt;                     /* Allocated Block Cnt */
   ULONG    lSize;                         /* Segment Size */
   USHORT   fFlags;                        /* Flags */
   PBLOCK   pLastSearchedBlock;          // BDN
   BLOCK    Block;                         /* first Block */
} HEAP;
typedef HEAP FAR * PHEAP;

/**********************************************************************\
 *  Local functions                                                   *
\**********************************************************************/

BOOL   LocateFreeMem (PHEAP pFirstHeap,
                      PHEAP FAR * ppHeap,
                      PBLOCK FAR * ppBlock,
                      ULONG cbLen);
USHORT CollapseFreeBlock (PBLOCK pBlock);
USHORT CollapseFreeHeap (PHEAP pFirstHeap, PHEAP pHeap);

#ifndef INCL_32
/* Reallocation of a segment is used to grow a heap in 16-bit OS/2 */
BOOL   ReallocHeap (PHEAP pFirstHeap,
                    PHEAP FAR * ppHeap,
                    PBLOCK FAR * ppBlock,
                    ULONG lSize);
#endif

USHORT AddPid (PHEAP pFirstHeap, PID pid, PHEAP pNewHeap);
PHEAP NewHeap (PHEAP pFirstHeap, ULONG lInitSize, USHORT fFlags);

/**********************************************************************\
 * HhpCreateHeap
 *
 * Create the First Heap
 *
 * History:
\**********************************************************************/
HHUGEHEAP APIENTRY HhpCreateHeap (ULONG lInitSize, USHORT fFlags)
{
   return ((HHUGEHEAP)NewHeap (NULL, lInitSize, fFlags));
}


/**********************************************************************\
 * NewHeap
 *
 * Create a New Heap (one segment)
 *
 * History:
\**********************************************************************/
PHEAP NewHeap (PHEAP pFirstHeap, ULONG lInitSize, USHORT fFlags)
{
   PHEAP  pHeap;
   PHEAP  pLastHeap;

   /******************************************************************/
   /* Allocate a Segment or block of memory into which to build heap */
   /******************************************************************/

   pHeap = (PHEAP) HhpAllocBuffer (lInitSize, fFlags);   /* Alloc Memory for Heap */
   if (!pHeap) return (NULL);

   /*
    * Zero the heap here.  This is more efficient than zeroing when
    * each piece is allocated.
    */

//   memset( pHeap, '\0', (ULONG)lInitSize );

   /*****************************************************************/
   /* Setup Heap Structures                                         */
   /*****************************************************************/
   pHeap->pNext = 0;                            /* Last in List */
   pHeap->pBlock = &pHeap->Block;               /* Point to first block */
   pHeap->uAllocCnt = 0;                        /* Number of Allocated Blocks */
   pHeap->lSize = lInitSize;                    /* Size of Segment */
   pHeap->fFlags = fFlags;                      /* Heap Flags */
   pHeap->ppidList = NULL;                      /* No Pid List */

   pHeap->pBlock->pNext = NULL;                 /* Only one block */
   pHeap->pBlock->pPrev = NULL;                 /* No back pointer */
   pHeap->pBlock->lBlockSize = (lInitSize - (ULONG) sizeof(HEAP));  /* Init Block Size */
   pHeap->pBlock->fAlloc = FALSE;               /* Free */
   pHeap->pBlock->magic = HEAP_MAGIC;           /* Initialize reserved word */

   pHeap->pLastSearchedBlock = &pHeap->Block;       /* Initialize reserved word */

   /******************************************************************/
   /* Chain Heaps                                                    */
   /******************************************************************/
   if (pFirstHeap) {
      for (pLastHeap=pFirstHeap; pLastHeap->pNext; pLastHeap=pLastHeap->pNext);
      pLastHeap->pNext = pHeap;
   }

   /******************************************************************/
   /* Create PID List or Give new Heap to all who Accessed Heap      */
   /******************************************************************/

   if (!pFirstHeap && pHeap->fFlags & HH_SHARED)
      {
      pHeap->ppidList = HhpIAllocMem ((HHUGEHEAP)pHeap, sizeof(PID)*PID_MAX);

      /*
       * Don't Give to anyone.
       */

      if (AddPid( pHeap, HhpGetPID(), NULL ))
         return (NULL);
      }
   else if (pHeap->fFlags & HH_SHARED)
      {

      /*
       * Give This Heap to Others.
       */

      if (AddPid( pFirstHeap, HhpGetPID(), pHeap ))
         return (NULL);
      }

   return (pHeap);                              /* return heap pointer */

}

/**********************************************************************\
 * HhpIAllocMem
 *
 * Allocate some memory
 *
 * History:
\**********************************************************************/

PVOID APIENTRY HhpIAllocMem (HHUGEHEAP hheap, ULONG cbLen)
{
   PBLOCK pBlock;
   PHEAP  pHeap;
   PBLOCK pBlockNew;
   PBLOCK pBlockNext;
   BOOL   fAlloc;
   ULONG  lNewBlockSize;
   PHEAP  pFirstHeap = (PHEAP)hheap;
   PCHAR  pData = NULL;

   if (cbLen & 0x00000003)                //Align to DWORD boundary if not already  //dwk
    {                                                                               //dwk
      cbLen=(cbLen & 0xFFFFFFFC) + 0x4;                                             //dwk
    }                                                                               //dwk


   if (!hheap) return (NULL);                   /* Bad handle */

   fAlloc = LocateFreeMem (pFirstHeap, &pHeap, &pBlock, cbLen); /* Locate a free Block */

   if (fAlloc)
      {

      /* Allocate the first cbLen bytes of Block */

      if (pBlock->lBlockSize - cbLen <= sizeof(BLOCK))
         {
         cbLen = pBlock->lBlockSize;          /* Round up the Request Size */
         }

      lNewBlockSize = pBlock->lBlockSize - cbLen;  /* New block size */
      pBlockNew = (PBLOCK)((PCHAR)pBlock + sizeof(BLOCK) + cbLen); /* New Block */

      pBlock->lBlockSize = cbLen;            /* Data space of the block */
      pBlock->fAlloc = TRUE;                 /* Allocated Block */
      pHeap->uAllocCnt++;                    /* Alloc'ed block count */
      pBlock->magic = HEAP_MAGIC;            /* Should already be there */

      if (lNewBlockSize)
         {

         /*
          * Create a Free Block for Fragment.
          * Point to next block.
          */

         pBlockNext = pBlock->pNext;
         pBlock->pNext = pBlockNew;
         pBlockNew->pNext = pBlockNext;
         if (pBlockNext) pBlockNext->pPrev = pBlockNew;
         pBlockNew->pPrev = pBlock;

         pBlockNew->lBlockSize = lNewBlockSize - (ULONG) sizeof(BLOCK);
         pBlockNew->fAlloc = FALSE;
         pBlockNew->magic = HEAP_MAGIC;
         }

      /*
       * Note:  No need to zero memory.  It is now done on heap creation and
       * freemem.
       * WRONG...  Backed out...
       * Zero now, fix actual bug later... It seems block control information
       * is not being zeroed on a free.
       */

      pData = (PCHAR)pBlock + (USHORT)sizeof(BLOCK);     /* Get Pointer */
      memset( pData, '\0', (ULONG)cbLen );
      }

   return (pData);                                    /* return pointer */
}

/**********************************************************************\
 * LocateFreeMem
 *
 * Locate a Free Block of Memory
 *
 * History:
\**********************************************************************/
BOOL LocateFreeMem (PHEAP pFirstHeap,
                    PHEAP FAR * ppHeap,
                    PBLOCK FAR * ppBlock,
                    ULONG cbLen)
{
   PBLOCK pBlock;                            /* block pointer */
   PHEAP  pHeap;                             /* heap pointer */
   ULONG  lSize;                             /* New Heap Size */

   /*****************************************************************/
   /* Locate an existing Block by searching list.  Since this is a  */
   /* huge heap, performance can be improved by saving the last     */
   /* searched node (speed) or a best-fit algorithm (overall size)  */
   /*****************************************************************/

   for (pHeap = pFirstHeap; pHeap; pHeap = pHeap->pNext) {
//    for (pBlock = pHeap->pBlock; pBlock; pBlock = pBlock->pNext) {
      for (pBlock = pHeap->pLastSearchedBlock; pBlock; pBlock = pBlock->pNext) { //BDN
           /* Is there an existing free block that is big enough */
           if (!pBlock->fAlloc && pBlock->lBlockSize >= cbLen) {
              *ppHeap = pHeap;
              *ppBlock = pBlock;
              pHeap->pLastSearchedBlock=pBlock;                    //BDN
              return (TRUE);
           }
      }
   }

   /*******************************************************************/
   /* Try Reallocing an Existing Heap to make room                    */
   /*******************************************************************/
   lSize = ((ULONG)cbLen+(ULONG)sizeof(HEAP)+
            (ULONG)sizeof(BLOCK)+0xfffL) & 0xfffff000L; /* Round to 4K */

#ifndef INCL_32
   if (ReallocHeap (pFirstHeap, ppHeap, ppBlock, lSize)) {
      return (TRUE);
   }
#endif

   /********************************************************************/
   /* Create a New Heap                                                */
   /********************************************************************/
   *ppHeap = NewHeap (pFirstHeap, lSize, pFirstHeap->fFlags);
   if (!*ppHeap) return (FALSE);                        /* No More Mem */

   *ppBlock = (*ppHeap)->pBlock;                        /* First Block Free */

   return (TRUE);

}

/*********************************************************************\
 * HhpFreeMem
 *
 * Free Memory
 *
 * History:
\**********************************************************************/
BOOL APIENTRY HhpFreeMem (HHUGEHEAP hheap, PVOID pData)
{
   PHEAP  pHeap;
   PBLOCK pBlock;
   PHEAP pFirstHeap = (PHEAP)hheap;

   /*****************************************************************/
   /* Validate the heap handle and the data pointer                 */
   /*****************************************************************/

   if (!hheap) return (TRUE);                         /* Bad handle */

   pBlock = (PBLOCK)((PCHAR)pData - sizeof(BLOCK));   /* get Block Pointer */
   if (pBlock->magic != HEAP_MAGIC) return (TRUE);    /* Can't Free anything */
   if (!pBlock->fAlloc) return (TRUE);

   /* Determine if the data is in the specified heap */

   for (pHeap = pFirstHeap; pHeap; pHeap = pHeap->pNext) {
      if ((PCHAR)pData >= (PCHAR)pHeap->pBlock &&
          (PCHAR)pData < ((PCHAR)pHeap + pHeap->lSize)) break;
   }

   /********************************************************************/
   /* Free the Block                                                   */
   /********************************************************************/

   if (pHeap)
      {
#ifdef MMDEBUG                                    //12170 4meg performance enhancement
    /*************************************************************
     * llp
     * Only do memset to help in debugging so that if someone is
     * corrupting data, the trap will happen faster
     * Don't want this in retail since it slows things down
     * anyways memset is still done on alloc which is what is important
     *********************************************************************/
      memset( pData, '\0', pBlock->lBlockSize );
#endif

      pBlock->fAlloc = FALSE;                   /* Free Block */
      pHeap->uAllocCnt--;                       /* Decrement Alloc Block Cnt */
      CollapseFreeBlock (pBlock);               /* Collect Garbage */

      pHeap->pLastSearchedBlock=&pHeap->Block;                    //BDN

   /*******************************************/
   /*  Brad Noe 3/7/93                        */
   /*                                         */
   /*  Commented out because it seems to cause*/
   /*  problems in multi-process case         */
   /*  Need to investigate and fix            */
   /*******************************************/
//    CollapseFreeHeap  (pFirstHeap, pHeap);    /* Collect Garbage */
      }

   return (FALSE);

}

/************************************************************************\
 * CollapseFreeBlock
 *
 *
 * History:
\************************************************************************/
USHORT CollapseFreeBlock (PBLOCK pBlock)
{
   register PBLOCK pPrevBlock;
   register PBLOCK pNextBlock;

   if (pBlock->fAlloc) return (FALSE);          /* This block is allocated */

   pPrevBlock = pBlock->pPrev;
   pNextBlock = pBlock->pNext;

   if (pNextBlock && !pNextBlock->fAlloc) {     /* Next Block */
      pBlock->pNext = pNextBlock->pNext;
      if (pNextBlock->pNext) pNextBlock->pNext->pPrev = pBlock;
      pBlock->lBlockSize += sizeof(BLOCK) + pNextBlock->lBlockSize;
   }

   if (pPrevBlock && !pPrevBlock->fAlloc) {    /* Prev Block */
      pPrevBlock->pNext = pBlock->pNext;
      if (pBlock->pNext) pBlock->pNext->pPrev = pPrevBlock;
      pPrevBlock->lBlockSize += sizeof(BLOCK) + pBlock->lBlockSize;
   }

   return (FALSE);

}

/************************************************************************\
 * CollapseFreeHeap
 *
 *
 * History:
\************************************************************************/
USHORT CollapseFreeHeap (PHEAP pFirstHeap, PHEAP pHeap)
{
   register PHEAP pTempHeap;

   if (pFirstHeap == pHeap) return (FALSE);     /* Never Free First Heap */

   if (pHeap->uAllocCnt) return (FALSE);        /* Allocated Blocks */

   for (pTempHeap = pFirstHeap; pTempHeap->pNext != pHeap;
        pTempHeap = pTempHeap->pNext);          /* Locate Previous Heap */

   pTempHeap->pNext = pHeap->pNext;             /* Take out of List */

   HhpFreeBuffer ((PBYTE)pHeap);            /* Free Segment */

   return (FALSE);

}

/**********************************************************************\
 * HhpDestroyHeap
 *
 *
\**********************************************************************/
BOOL APIENTRY HhpDestroyHeap (HHUGEHEAP hheap)
{
   PHEAP pHeap;
   PHEAP pFirstHeap = (PHEAP) hheap;
   PSZ   pszTemp;

#ifdef MMDEBUG
       if (!DosScanEnv(MM_HEAP_DEBUG_VAR, &pszTemp))
         HhpDumpHeap(hheap);
#endif

   while (pFirstHeap) {                   /* Free All Segments Of Heap */
      pHeap = pFirstHeap;
      pFirstHeap = pFirstHeap->pNext;
      HhpFreeBuffer ((PBYTE)pHeap);
   }

   return (FALSE);

}

/***********************************************************************\
 * HhpAllocBuffer  (Call DosAllocSeg or DosAllocMem)
 *
 * History:
\***********************************************************************/
PBYTE APIENTRY HhpAllocBuffer (ULONG lSize, USHORT fFlags)
{
   USHORT rc;
   USHORT fMem;

#ifndef INCL_32
   SEL sel;

   if (fFlags & HH_SHARED) {
      fMem = SEG_GIVEABLE | SEG_GETTABLE;
   } else {
      fMem = SEG_NONSHARED;
   }

   if (lSize > 0xffffL) return (NULL);              /* Limit to 64K-1 */

   rc = DosAllocSeg ((USHORT)lSize, &sel, fMem);    /* Allocate Heap Seg */
   if (rc) return (NULL);
   return (MAKEP(sel, 0));

#else

   PBYTE pBuffer;

   /*
    * Allocate private or shared memory.
    */

   if (fFlags & HH_SHARED)
      {
      fMem = fALLOCSHR & ~PAG_EXECUTE;

      rc = (USHORT) DosAllocSharedMem( (PPVOID)&pBuffer,
                                       NULL,
                                       (ULONG)lSize,
                                       (ULONG)fMem );
      }
   else
      {
      fMem = fALLOC & ~PAG_EXECUTE;

      rc = (USHORT) DosAllocMem( (PPVOID)&pBuffer,
                                 (ULONG)lSize,
                                 (ULONG)fMem );
      }

   if (rc) return (NULL);
   return (pBuffer);

#endif

}

/***********************************************************************\
 * HhpFreeBuffer    (Call DosFreeSeg or DosFreeMem)
 *
 *
 * History
\***********************************************************************/
USHORT APIENTRY HhpFreeBuffer (PBYTE pBuffer)
{
#ifndef INCL_32
   return (DosFreeSeg (SELECTOROF(pBuffer)));
#else
   return ((USHORT) DosFreeMem ((PVOID)pBuffer));
#endif
}

/**********************************************************************\
 * HhpAccessBuffer
 *
 *
 *
\**********************************************************************/
USHORT APIENTRY HhpAccessBuffer (PBYTE pBuffer)
{
#ifndef INCL_32
    if (DosGetSeg (SELECTOROF(pBuffer))) return (TRUE);
#else
    if (DosGetSharedMem (pBuffer, fPERM)) return (TRUE);
#endif
    return (FALSE);
}

/**********************************************************************\
 * HhpAccessHeap        (Access a shared Heap)
 *
 *
 * History:
\**********************************************************************/

USHORT APIENTRY HhpAccessHeap( HHUGEHEAP hheap, PID pid )
{
   PHEAP pFirstHeap = (PHEAP) hheap;
   PHEAP pHeap;

   if (!hheap) return (TRUE);

   /*********************************************************************/
   /* Get Access to All Existing Segments/Pages in the Heap             */
   /*********************************************************************/

   for (pHeap = pFirstHeap; pHeap; pHeap = pHeap->pNext) { /* Access All Segments Of Heap */
      if (HhpAccessBuffer ((PVOID)pHeap)) return (TRUE);
   }

   /********************************************************************/
   /* Put New Pid Into Pid List                                        */
   /********************************************************************/

   if (pFirstHeap->fFlags & HH_SHARED)
      {
      if (AddPid( pFirstHeap, pid, NULL ))
         return (TRUE);
      }

   return (FALSE);

}

/**********************************************************************\
 * HhpReleaseHeap
 *
 * History:
\**********************************************************************/
USHORT APIENTRY HhpReleaseHeap (HHUGEHEAP hheap, PID pid)
{
   PHEAP pHeap = (PHEAP) hheap;
   USHORT i;

   if (!(pHeap->fFlags & HH_SHARED)) return (FALSE);

   if (!pHeap->ppidList) return (FALSE);

   /* Remove Pid from List */
   for (i=0; i<PID_MAX && pHeap->ppidList[i] != pid; i++);   /* Locate Slot */

   if (i < PID_MAX)
      pHeap->ppidList[i] = (ULONG)NULL;

   return ((USHORT)NULL);

}

/*************************************************************************\
 * AddPid
 *
 *
\*************************************************************************/
USHORT AddPid (PHEAP pFirstHeap, PID pid, PHEAP pNewHeap)
{
   USHORT i;

   /* Save the Pid in the PidList of the First Heap only */

   for (i=0; i<PID_MAX && pFirstHeap->ppidList[i] != pid; i++); /* Existing */
   if (i == PID_MAX) {
      for (i=0; i<PID_MAX && pFirstHeap->ppidList[i] != 0; i++);/* Locate Slot */
      if (i == PID_MAX) return (TRUE);                     /* No Access */
      pFirstHeap->ppidList[i] = pid;                       /* store pid */
   }

   /* Give the New Memory Block to all who have accessed this heap */

   if (pNewHeap && pNewHeap != pFirstHeap) {      /* Give Seg to All Pids */
      for (i=0; i<PID_MAX; i++) {
         if (pFirstHeap->ppidList[i] != 0) {
#ifndef INCL_32
            SEL selNew;
            DosGiveSeg (SELECTOROF(pNewHeap), pFirstHeap->ppidList[i], &selNew);
#else
            DosGiveSharedMem (pNewHeap, pFirstHeap->ppidList[i], fGIVESHR);
#endif
         }
      }
   }

   return (FALSE);

}

#ifndef INCL_32
/**********************************************************************\
 * ReallocHeap
 *
 *
 * History
\**********************************************************************/
BOOL ReallocHeap (PHEAP pFirstHeap,
                  PHEAP FAR * ppHeap,
                  PBLOCK FAR * ppBlock,
                  ULONG lSize)
{
   USHORT rc;
   PHEAP  pHeap;
   PBLOCK pBlock;
   PBLOCK pNextBlock;

   for (pHeap = pFirstHeap; pHeap; pHeap = pHeap->pNext) {

      if ((LONG)0xffffL - (LONG)pHeap->lSize >= (LONG)lSize) { /* ReSize segment */
         /* Only Supported with 64K Segments */
         rc = DosReallocSeg ((USHORT)(pHeap->lSize+lSize), SELECTOROF(pHeap));
         if (rc) return (FALSE);                    /* No Memory */

         pHeap->lSize += lSize;                     /* Increase Heap Size */

         for (pBlock = pHeap->pBlock; pBlock->pNext; pBlock = pBlock->pNext); /* Last */
         if (pBlock->fAlloc) {                     /* Last Block Allocated */
            pNextBlock = (PBLOCK)((PCHAR)pBlock + sizeof(BLOCK) + pBlock->lBlockSize);
            pNextBlock->pPrev = pBlock;
            pNextBlock->pNext = NULL;
            pBlock->pNext = pNextBlock;
            pNextBlock->lBlockSize = (lSize - (ULONG)sizeof(BLOCK));
            pNextBlock->magic = HEAP_MAGIC;
            pNextBlock->fAlloc = FALSE;
            pBlock = pNextBlock;
         } else {
            pBlock->lBlockSize += lSize;
         }
         *ppHeap = pHeap;
         *ppBlock = pBlock;
         return (TRUE);                            /* success */

      }

   }
   return (FALSE);

}

#endif

/*************************************************************************\
 * HhpGetPID
 *
 *
\*************************************************************************/
PID APIENTRY HhpGetPID (VOID)
{
   USHORT rc;

#ifndef INCL_32

   PIDINFO pInfo;

   rc = DosGetPID (&pInfo);                  /* Get Pid Info */
   if (rc) return ((PID)NULL);               /* Bad News */

   return (pInfo.pid);                       /* Return Current PID */

#else

   PPIB ppib;
   PTIB ptib;

   rc = (USHORT) DosGetInfoBlocks (&ptib, &ppib);  /* Get Info Blocks */
   if (rc) return ((PID)NULL);

   return (ppib->pib_ulpid);                 /* return PID */

#endif

}


// Trace the file and line number for allocated blocks

PVOID APIENTRY HhpTAllocMem (HHUGEHEAP hHeap, ULONG cbLen, PSZ pszFile, ULONG uLine)
{
   PVOID  pData;

#ifdef MMDEBUG
   PBLOCK pBlock=NULL;
   ULONG  ulLength;    //strlen for debug
#endif

   pData = (PVOID) HhpIAllocMem (hHeap, cbLen);

#ifdef MMDEBUG
   if (pData)
   {
     if (pszFile == NULL)
     {
       pBlock = (PBLOCK)((PCHAR)pData - sizeof(BLOCK));   /* get Block Pointer */
       strncpy (pBlock->szFile, "Retail", 14);
       pBlock->szFile[14] = 0;
       pBlock->uLine = 0;
     }
     else
     {
       pBlock = (PBLOCK)((PCHAR)pData - sizeof(BLOCK));   /* get Block Pointer */
       ulLength = strlen(pszFile);                       //append last 15 chars so if
       if (ulLength > 15)                                //long path, file name is visible
        {
         strncpy (pBlock->szFile, (PSZ)((ULONG)pszFile + ulLength - 15L), 15);
        }
       else strncpy (pBlock->szFile, pszFile, 15);
       pBlock->szFile[14] = 0;
       pBlock->uLine = uLine;
     }
   }
#endif

   return (pData);

}


USHORT APIENTRY HhpDumpHeap( HHUGEHEAP hheap )
{
   PHEAP  pHeap = (PHEAP) hheap;
   PBLOCK pBlock;
   ULONG  cbytes;
   CHAR   szBuffer[CCHMAXPATH];

   for (; pHeap; pHeap = pHeap->pNext)
   {
      strcpy (szBuffer, "Heap: ");
      _ultoa ((ULONG)pHeap, &szBuffer[strlen(szBuffer)], 16);
      strcat (szBuffer, " AllocCnt: ");
      _ultoa ((ULONG)pHeap->uAllocCnt, &szBuffer[strlen(szBuffer)], 10);
      strcat (szBuffer, " Size: ");
      _ultoa ((ULONG)pHeap->lSize, &szBuffer[strlen(szBuffer)], 10);
      strcat (szBuffer, "\r\n");
      DosWrite (1, szBuffer, strlen(szBuffer), &cbytes);


      for (pBlock = pHeap->pBlock; pBlock; pBlock = pBlock->pNext)
      {
         strcpy (szBuffer, "   Block: ");
         _ultoa ((ULONG)pBlock, &szBuffer[strlen(szBuffer)], 16);
         strcat (szBuffer, " Size: ");
         _ultoa ((ULONG)pBlock->lBlockSize, &szBuffer[strlen(szBuffer)], 10);
         strcat (szBuffer, " fAlloc:");
         _ultoa ((ULONG)pBlock->fAlloc, &szBuffer[strlen(szBuffer)], 10);
#ifdef MMDEBUG
         strcat (szBuffer, "  File: ");
         strcat (szBuffer, pBlock->szFile);
         strcat (szBuffer, " Line: ");
         _ultoa ((ULONG)pBlock->uLine, &szBuffer[strlen(szBuffer)], 10);
#endif
         strcat (szBuffer, "\r\n");
         DosWrite (1, szBuffer, strlen(szBuffer), &cbytes);
      }
   }

   return (0);
}
