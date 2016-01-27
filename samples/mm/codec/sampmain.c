/*****************************************************************************/
/*                                                                           */
/* SAMPMAIN.C: Sample Codec Main entry point and instance handling.          */
/*                                                                           */
/* Copyright (c) IBM Corporation 1991,1994             All Rights Reserved   */
/*                                                                           */
/*****************************************************************************/

#define  INCL_WIN
#define  INCL_GPI
#define  INCL_DOS
#define  INCL_OS2MM
#define  INCL_MMIO_CODEC
#include <os2.h>
#include <os2me.h>
#include <string.h>
#include "sampinst.h"
#include "sampdeco.h"
#include "sampenco.h"

/*****************************************************************************/
/*                                                                           */
/* Below defines the DLL shared global data (only one copy of this exists... */
/* regardless of the number of processes or instances using it).             */
/* The data must be initialized or it won't be placed in this segment.       */
/* Note that if you have significant information in your instance array,     */
/* you may want to make this global array pointers and allocate the          */
/* instances on a required basis.  As it exists here, all 16 instances       */
/* fit in one 4KB page of memory (the minimum allocatable memory without     */
/* a heap manager).  It's also a good idea to put static conversion tables   */
/* here so that if more than one process is open at a time, there's only     */
/* one copy of the tables (see the bottom of sampenco.c).                    */
/*                                                                           */
/*****************************************************************************/

#pragma data_seg (PROCESS_SHARED_DATA_SEGMENT)

#define   MAX_INSTANCES 16

MAIN_INST amiInst[MAX_INSTANCES] = {0};

HMTX      hmtxInstanceArray = 0;

ULONG     ulInitCount = 0;

#pragma data_seg ()




/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: OpenCodecInstance                                        */
/*     This routine is called once per instance allocation.  Here is where   */
/*     you initialize either your compression parameters, your               */
/*     decompression parameters, or both (one instance can do both at        */
/*     once) based on the passed open flags.                                 */
/*                                                                           */
/*****************************************************************************/

LONG OpenCodecInstance ( PULONG pulInstNumber, PCODECOPEN pCodecOpen )
   {
   PMAIN_INST pMainInst;
   PCODECVIDEOHEADER pCodecDst;
   PCODECVIDEOHEADER pCodecSrc;
   ULONG ulInstIndex;

   /* Check to make sure that the instance pointer is non-zero.              */
   if ( !pulInstNumber )
      return ( MMIO_ERROR );

   /* Check to make sure that the instance isn't already allocated.          */
   if ( *pulInstNumber )
      return ( MMIO_ERROR );

   /* Search for a spot in the instance array for this pointer.              */
   ulInstIndex = 0;
   while ( ulInstIndex < MAX_INSTANCES ) {
      /* If we've found an open instance location, use it.                   */
      if ( !amiInst[ulInstIndex].fInstanceInUse )
         break;
      ulInstIndex ++;
      }

   /* Check if we are out of instance space.                                 */
   if ( ulInstIndex == MAX_INSTANCES )
      return ( ERROR_ALLOC_RESOURCES );

   /* Setup a pointer to point to the instance.                              */
   pMainInst = &amiInst[ulInstIndex];

   /* Initialize the instance to all zeros.                                  */
   memset ( pMainInst, 0, sizeof ( MAIN_INST ) );

   /* Grab local copys of the codec open dst and src structure pointers.     */
   pCodecDst = (PCODECVIDEOHEADER) pCodecOpen->pDstHdr;
   pCodecSrc = (PCODECVIDEOHEADER) pCodecOpen->pSrcHdr;

   /* Check if we are opening a decompression instance.                      */
   if ( pCodecOpen->ulFlags & CODEC_DECOMPRESS ) {

      /* Grab a copy of the width and height.                                */
      pMainInst->ulWidth = pCodecSrc->cx;
      pMainInst->ulHeight = pCodecSrc->cy;
      }

   /* Check if we are opening a compression instance.                        */
   if ( pCodecOpen->ulFlags & CODEC_COMPRESS ) {

      /* We don't do any scaling, make sure we're not asked to.  Also check  */
      /* the source color format to see if we support that, and check that   */
      /* the destination is actually supposed to be compressed.              */
      if ( pCodecSrc->cx != pCodecDst->cx || pCodecSrc->cy != pCodecDst->cy ||
           ( pCodecSrc->ulColorEncoding != MMIO_RGB_5_6_5 &&
             pCodecSrc->ulColorEncoding != MMIO_YUV_4_1_1 ) ||
             pCodecDst->ulColorEncoding != MMIO_COMPRESSED )
         return ( MMIO_ERROR );

      /* Grab a copy of the source width, height, and color encoding.        */
      pMainInst->ulWidth= pCodecSrc->cx;
      pMainInst->ulHeight= pCodecSrc->cy;
      pMainInst->ulColorEncoding= pCodecSrc->ulColorEncoding;
      }

   /* Mark that this instance is now in use.                                 */
   pMainInst->fInstanceInUse = TRUE;

   /* Return the instance number that we created, and we are done.  Note     */
   /* that we'll add 1 to it just so we don't return a 0 as a vaild instance */
   /* number.  Therefore we'll return instance numbers in the 1 to 16 range. */
   /* Remember that when they come back, they'll be one larger!              */
   *pulInstNumber = ulInstIndex + 1;

   return ( MMIO_SUCCESS );
   }




/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: CompressBuffer                                           */
/*     This routine will be called once per frame to compress.  This may     */
/*     be a symmetric call or an asymmetric call (or even an asymmetric      */
/*     call to a symmetric algorithm) if your compression has set the        */
/*     symmetric flag.  Otherwise the asymmetric flag must be set and        */
/*     the call to the compression can take as long as desired.              */
/*                                                                           */
/*****************************************************************************/

ULONG CompressBuffer ( PULONG pulInstNumber, PMMCOMPRESS pmmCompress )
   {
   PMAIN_INST pMainInst;

   /* Check to make sure that the instance pointer is valid                  */
   if ( !pulInstNumber )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Check to make sure that the instance number is valid.                  */
   if ( !*pulInstNumber || *pulInstNumber > MAX_INSTANCES )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Grab a pointer to the instance and make sure it's okay.                */
   pMainInst = &amiInst[*pulInstNumber-1];
   if ( !pMainInst->fInstanceInUse )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Check to see that the pmmCompress pointer is non-zero.                 */
   if ( !pmmCompress )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Call to the compressor.                                                */
   pmmCompress->ulDstBufLen = CompressFrame ( pMainInst,
                                 pmmCompress->pSrcBuf, pmmCompress->pDstBuf );

   /* If we actually compressed something, return that it worked.            */
   if ( pmmCompress->ulDstBufLen )
      return ( MMIO_SUCCESS );
   else
      return ( MMIO_ERROR );
   }




/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: DecompressBuffer                                         */
/*     This routine is called once per frame to be decompressed.             */
/*                                                                           */
/*****************************************************************************/

ULONG DecompressBuffer ( PULONG pulInstNumber, PMMDECOMPRESS pmmDecompress )
   {
   PMAIN_INST pMainInst;

   /* Check to make sure that the instance pointer is valid                  */
   if ( !pulInstNumber )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Check to make sure that the instance number is valid.                  */
   if ( !*pulInstNumber || *pulInstNumber > MAX_INSTANCES )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Grab a pointer to the instance and make sure it's okay.                */
   pMainInst = &amiInst[*pulInstNumber-1];
   if ( !pMainInst->fInstanceInUse )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Check the videodecompress parameter for validity.                      */
   if ( !pmmDecompress->pRunTimeInfo )
      return ( MMIOERR_INVALID_PARAMETER );

   /* Here is where we check to see if the software video stream handler     */
   /* is requesting that we drop this frame to keep synchronization with     */
   /* the audio.  It is not recommended that this flag be ignored.  If it    */
   /* is ignored, the audio stream could starve waiting on the video stream  */
   /* and the audio will "break up"-- seemingly stopping and starting.       */
   /* The MMIO_DROP_FRAME_DECODE means don't do the decompression on this    */
   /* frame (but we're getting called in case we need to do clean-up).       */
   if ( pmmDecompress->ulFlags & MMIO_DROP_DELTA_FRAME )
      pmmDecompress->ulFlags &= ~MMIO_DROP_DELTA_FRAME;
   else
      /* Decompress the frame.                                               */
      DecompressFrame ( pMainInst, pmmDecompress->pSrcBuf,
                                       pmmDecompress->pDstBuf );

   /* The frame has be completely decoded, tell the subsystem we're done.    */
   pmmDecompress->pSrcBuf = 0;
   pmmDecompress->ulSrcBufLen = 0;
   return ( MMIO_SUCCESS );
   }




/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: CloseCodecInstance                                       */
/*     This routine will be called once per instance at deallocation time.   */
/*     Be sure to deallocate any private RAM allocated from the heap or      */
/*     through DosAllocMem in either a compression instance, decompression   */
/*     instance, or both.                                                    */
/*                                                                           */
/*****************************************************************************/

LONG CloseCodecInstance ( PULONG pulInstNumber )
   {
   PMAIN_INST pMainInst;

   /* Check to make sure the instance pointer is non-zero.                   */
   if ( pulInstNumber )

      /* Check to make sure the instance number itself is valid.             */
      if ( *pulInstNumber && *pulInstNumber <= MAX_INSTANCES ) {

         /* Grab a pointer to the instance.                                  */
         pMainInst = &amiInst[*pulInstNumber-1];

         /* Check to see if the instance is actually in use.                 */
         if ( pMainInst->fInstanceInUse ) {

            /* Okay, everything checks out.  Mark it as no longer in use and */
            /* Free up the compression buffer if it was allocated.           */
            pMainInst->fInstanceInUse = FALSE;

            /* Null the instance data pointer.                               */
            *pulInstNumber = 0;

            /* Return that everything was okay.                              */
            return ( MMIO_SUCCESS );
            }
         }

   /* There was something wrong with the passed instance number pointer.     */
   return ( MMIOERR_INVALID_PARAMETER );
   }




/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: SampleCodecEntry                                         */
/*     This routine is where all messages to the codec come in.              */
/*     This is the routine name that goes into the MMPMMMIO.INI file,        */
/*     specified in the install scrupt.                                      */
/*                                                                           */
/*****************************************************************************/

LONG APIENTRY SampleCodecEntry ( PULONG pulInstNumber, LONG lMMIOmessage,
                                 LONG lParam1, LONG lParam2 )
   {
   /* Switch to the appropriate routine based on the message.                */
   switch ( lMMIOmessage )
      {
      case MMIOM_CODEC_OPEN:

         /* During an open call, pulInstNumber is the address of where       */
         /* to put the instance number.  This location must contain          */
         /* a zero or the call should fail.                                  */
         return ( OpenCodecInstance ( pulInstNumber, (PCODECOPEN)lParam1 ) );

      case MMIOM_CODEC_CLOSE:

         /* During a close call, pulInstNumber is the address of the         */
         /* instance number to close.                                        */
         return ( CloseCodecInstance ( pulInstNumber ) );

      case MMIOM_CODEC_COMPRESS:

         /* During a decompress call, pulInstNumber is actually the instance */
         /* number and NOT a pointer.  Do the appropriate cast here.         */
         return ( CompressBuffer ( pulInstNumber, (PMMCOMPRESS)lParam1 ) );

      case MMIOM_CODEC_DECOMPRESS:

         /* During a decompress call, pulInstNumber is actually the instance */
         /* number and NOT a pointer.  Do the appropriate cast here.         */
         return ( DecompressBuffer ( pulInstNumber, (PMMDECOMPRESS)lParam1 ) );

      case MMIOM_CODEC_QUERYNAMELENGTH:

         /* Put codec name length here, plus 1 for the terminating char.     */
         /* Example: "Sample Codec" is 6+1+5+1= 13;                          */
         *(PULONG)lParam1= 13L;
         return ( MMIO_SUCCESS );

      case MMIOM_CODEC_QUERYNAME:

         /* Use standard string function "strcpy" to copy over codec name.   */
         strcpy ( (PSZ)lParam1, (PSZ)"Sample Codec" );

         /* Put codec name length here, plus 1 for the terminating chars.    */
         /* Example: "Sample Codec" is 6+1+5+1= 13;                          */
         *(PULONG)lParam2= 13L;
         return ( MMIO_SUCCESS );

      /* The rest of the messages are unsupported.                           */
      default:
         return ( MMIOERR_UNSUPPORTED_MESSAGE );
      }
   }




/*****************************************************************************/
/*                                                                           */
/* This is called by the system when the .DLL is being accessed by a new     */
/* process for the first and last time.  Here is where we create a semaphore */
/* to protect instances and processes from each other.                       */
/*                                                                           */
/*****************************************************************************/

ULONG _CRT_init ( VOID );

ULONG _DLL_InitTerm ( ULONG hmod, ULONG fTerm )
   {
   /* Check if we are initializing or terminating.                           */
   if ( fTerm )
      /* We are terminating.                                                 */
      {
      ulInitCount--;
      if ( !ulInitCount )
         DosCloseMutexSem ( hmtxInstanceArray );
      return ( TRUE );
      }
   else
      {
      /* We are initializing.  Call to init the "C" run-time library,        */
      /* and create a mutex semaphore for accessing the global data.         */
      ulInitCount++;
      if ( !(_CRT_init()) )
         if ( !DosCreateMutexSem ( NULL, &hmtxInstanceArray,
                                             DC_SEM_SHARED, FALSE ) )
            return ( TRUE );
      }

   /* The initialization or termination has failed for some reason.          */
   return ( FALSE );
   }
