/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: FSSHSEEK.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Seek stream routine                    */
/*                                                                          */
/* FUNCTION: Seek a file system stream                                      */
/*                                                                          */
/* ENTRY POINTS: ShcSeek                                                    */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_DOSERRORS
#define  INCL_MMIOOS2
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>
#include <fssh.h>
#include <seekcalc.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcSeek                                                 */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Seek stream routine.            */
/*                                                                          */
/* FUNCTION: This seeks the specified stream to the position specified by   */
/*   lSeekPoint.  This seek point is qualified by the flags passed to       */
/*   indicate if the seek is from 1) the beginning of the file; 2) from the */
/*   current position in the file; or 3) from the end of the file.          */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcSeek                                                     */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc seek parameter block PARM_SEEK containing:         */
/*   ULONG   ulFunction  Handler command function SHC_SEEK                  */
/*   HID     hid         handler ID                                         */
/*   HSTREAM hstream     handle of stream instance                          */
/*   ULONG   ulFlags      Seek flag with following possible Flags:          */
/*                         SPI_SEEK_ABSOLUTE                                */
/*                         SPI_SEEK_RELATIVE                                */
/*                         SPI_SEEK_FROMEND                                 */
/*                         SPI_SEEK_SLAVES                                  */
/*                         SPI_SEEK_MMTIME                                  */
/*                         SPI_SEEK_BYTES                                   */
/*   LONG    lSeekPoint  Seek position. Signed. Seeking from end should be  */
/*                       negative.                                          */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_NOT_SEEKABLE_BY_TIME                                             */
/*   ERROR_DATA_ITEM_NOT_SEEKABLE                                           */
/*   ERROR_STREAM_NOT_STOP                                                  */
/*   Errors from the external routines:                                     */
/*     mmioSeek - the extended error code from mmioGetLastError             */
/*     mmioSendMessage (MMIOM_SEEKBYTIME) - extended error code from        */
/*                                          mmioGetLastError                */
/*     ShFindSib - ERROR_INVALID_STREAM, and errors from DosRequestMutexSem */
/*                                                                          */
/* SIDE EFFECTS: The mmio file pointer is positioned to the new location.   */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     mmioSeek                                                             */
/*     mmioSendMessage                                                      */
/*     mmioGetLastError                                                     */
/*     ShFindSib                                                            */
/*                                                                          */
/*   DATA STRUCTURES: None                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcSeek(pskparm)
PPARM_SEEK pskparm;

{ /* Start of ShcSeek */

RC rc = NO_ERROR;                       /* local return code */
PSIB psib;                              /* Stream instance block */
ULONG ulrc;                             /* temp rc from special seek */
LONG lByteSeekPoint;                    /* Seek point in bytes */
LONG lAbsFilePos;                       /* absolute file position from seek */
SHORT sTempFlags;                       /* Flags to call mmioseek */
BOOL bSeekTried = FALSE;                /* have we tried the seek yet? */

  /*
   * Find our Stream Instance Block
   */
  if (!(rc = ShFindSib(&psib, pskparm->hstream, pskparm->hid)))
    { /* We have SIB */

      /* We can only seek when stopped in ready to run state */

      if ((psib->ulStatus == SIB_RUNNING) &&
          !(psib->ulActionFlags & SIBACTFLG_THREAD_DEAD))
        { /* Good state */

          /* Check if this datatype is phsically seekable. If not just return.*/

          if (!(psib->espcb.spcb.ulHandFlags & SPCBHAND_PHYS_SEEK))
             {
             return(rc);
             }

          /* Check if this datatype is seekable */

          if (!(psib->espcb.spcb.ulDataFlags & SPCBDATA_NOSEEK))

            /* and the psib->espcb.spcb.ulHandFlags field has a value of ? */
            /* otherwise it this not a physically seekable target. */

            { /* datatype is seekable */

              /* If seek by bytes, just use what's passed.  If not, then it */
              /*  is in mmtime units.  So we must convert it to bytes first. */

              if (pskparm->ulFlags & SPI_SEEK_BYTES)
                {
                  lByteSeekPoint = pskparm->lSeekPoint;
                }
              else
                { /* in mmtime units, need to convert to bytes */

                  /* Check the bytes per unit and mmtime per unit values. */
                  /* If either are zero then we don't know how to convert the */
                  /* time units passed into bytes so just call mmioSendMessage */
                  /* with MMIOM_SEEKBYTIME, to see if the IOProc can figure */
                  /* it out. */

                  if (psib->espcb.spcb.ulBytesPerUnit && psib->espcb.spcb.mmtimePerUnit)
                    {
                      /* Call routine to do calc and check for overflow */

                     if (Mmtime2Bytes( psib->espcb.spcb.ulBytesPerUnit,
                                       pskparm->lSeekPoint,
                                       (LONG)psib->espcb.spcb.mmtimePerUnit,
                                       &lByteSeekPoint ))
                        {
                        rc = ERROR_LARGE_SEEK_BY_TIME;
                        }
                    }
                  else
                    { /* don't know how to convert time to bytes */

                      ulrc = mmioSendMessage((HMMIO)psib->ulAssocP1,
                                             MMIOM_SEEKBYTIME,
                                             pskparm->lSeekPoint,
                                             pskparm->ulFlags);

                      /* The error processing is a little tricky for this */
                      /* message since the IOProc will be handling the message */
                      /* just like a regular Seek it will return values just */
                      /* as it would for Seek. So the return value should be */
                      /* the new file position which could be 0.  If an error */
                      /* occurs then MMIO_ERROR is returned and we need */
                      /* to call getlasterror for the real error code. */
                      /* Now, since this is actually being called using */
                      /* mmioSendMessage it may return MMIO_ERROR for bad */
                      /* handle and the getlasterror would return */
                      /* MMIO_INVALID_HANDLE.  Also, if the SEEKBYTIME */
                      /* message is not supported then the mmioSendMessage */
                      /* rc will be 0, and the getlasterror will be */
                      /* MMIOERR_UNSUPPORTED_MESSAGE. */

                      if ((ulrc == MMIO_ERROR) || (ulrc == 0L))
                        { /* Seek failed */
                          rc = mmioGetLastError((HMMIO)psib->ulAssocP1);
                        } /* Seek failed */

                      if ((ulrc == 0L) && (rc == MMIOERR_UNSUPPORTED_MESSAGE))
                        rc = ERROR_NOT_SEEKABLE_BY_TIME;

                      bSeekTried = TRUE;

                      /* Set the absolute file position in mmtime units. */
                      /* NOTE: This is only implemented for SEEK_FROMEND now. */

                      if (!rc)
                        {
                        if (pskparm->ulFlags & SPI_SEEK_FROMEND)
                           pskparm->lSeekPoint = ulrc;
                        }
                    } /* don't know how to convert time to bytes */
                } /* in mmtime units, need to convert to bytes */

              if ((!rc) && (!bSeekTried))
                { /* ok to seek */

                  /* Get rid of all flags but seek origin */

                  sTempFlags = (SHORT)(pskparm->ulFlags &
                                      (SPI_SEEK_RELATIVE+SPI_SEEK_ABSOLUTE+SPI_SEEK_FROMEND));

                  lAbsFilePos = mmioSeek((HMMIO)psib->ulAssocP1,
                                         lByteSeekPoint,
                                         sTempFlags);

                  if (lAbsFilePos == -1L)
                    { /* Seek failed */
                      rc = mmioGetLastError((HMMIO)psib->ulAssocP1);
                    } /* Seek failed */
                  else
                     {
                     /* Seek was OK - set the absolute position in pksparm. */
                     /* Set the position in either bytes or time, depending */
                     /* on the seek mode that was requested. */
                     /* NOTE: This is only implemented for SEEK_FROMEND now. */

                     if (pskparm->ulFlags & SPI_SEEK_FROMEND)
                        {

                        if (pskparm->ulFlags & SPI_SEEK_BYTES)
                           {
                           pskparm->lSeekPoint = lAbsFilePos;
                           }
                        else
                           {

                           /* Check the bytes per unit and mmtime per unit */
                           /* values. If either are zero then we don't know how to */
                           /* convert bytes back into time units. */

                           if (psib->espcb.spcb.ulBytesPerUnit && psib->espcb.spcb.mmtimePerUnit)
                              {

                              /* Set the returned seek point to absolute offset in mmtime units. */


                              /* Set the returned seek point to absolute offset in mmtime units. */

                              if ( Bytes2Mmtime( psib->espcb.spcb.ulBytesPerUnit,
                                                 lAbsFilePos,
                                                 (LONG)psib->espcb.spcb.mmtimePerUnit,
                                                 &pskparm->lSeekPoint ))
                                 {
                                 rc = ERROR_LARGE_SEEK_BY_TIME;
                                 }
                              }
                           else
                              {
                              rc = ERROR_NOT_SEEKABLE_BY_TIME;
                              }
                           }
                        } /* Seek from END */
                     } /* Seek was OK */
                }  /* ok to seek */
            } /* datatype is seekable */
          else
            {
              rc = ERROR_DATA_ITEM_NOT_SEEKABLE;
            }
        } /* Good state */
      else
        {
          if (psib->ulStatus < SIB_RUNNING)
            rc = ERROR_DATA_ITEM_NOT_SPECIFIED;
          else
            rc = ERROR_INVALID_SEQUENCE;
        }
    } /* We have SIB */

  return(rc);

} /* End of ShcSeek */

