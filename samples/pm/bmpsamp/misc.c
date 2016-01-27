/******************************************************************************
*
*  File Name   : MISC.C
*
*  Description : miscellaneous Jigsaw functions
*
*  Entry Points:
*                CreateBitmapHdcHps()
*                SegListUpdate()
*                SetDVTransform()
*                SetRect()
*                SegListGet()
*                DrawPiece()
*                DoDraw()
*                CalcBounds()
*                CalcTransform()
*                Redraw()
*                Correlate()
*                MarkIsland()
*
*  Copyright (C) 1992 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
******************************************************************************/

#include "jigsaw.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/******************************************************************************/
/*                                                                            */
/* Create a memory DC and an associated PS.                                   */
/*                                                                            */
/******************************************************************************/
BOOL CreateBitmapHdcHps( phdc, phps)
PHDC phdc;
PHPS phps;
{
  SIZEL    sizl;
  HDC      hdc;
  HPS      hps;

  hdc = DevOpenDC( habMain, OD_MEMORY, "*", 3L, (PDEVOPENDATA)&dop, NULLHANDLE);
  if( !hdc)
    return( FALSE);

  sizl.cx = sizl.cy = 1L;
  if ((hps = GpiCreatePS( habMain
                        , hdc
                        , &sizl
                        , PU_PELS | GPIA_ASSOC | GPIT_MICRO )) == GPI_ERROR)
             DispError(habMain,hwndClient);
  if( !hps)
    return( FALSE);

  *phdc = hdc;
  *phps = hps;
  return( TRUE);
}

/*****************************************************************************/
/*                                                                            */
/* Add (at head or tail) or delete a specified segment list member.           */
/*                                                                            */
/******************************************************************************/
PSEGLIST SegListUpdate(USHORT usOperation,PSEGLIST pslUpdate)
{
  PSEGLIST psl;

  switch( usOperation)
  {
    case ADD_HEAD_SEG:
      if( pslHead == NULL)
      {
        DosAllocMem((PPVOID)&pslHead,(ULONG)sizeof(SEGLIST),(ULONG)PAG_READ | PAG_WRITE | PAG_COMMIT);
        if( pslHead == NULL)
          return( NULL);
        *pslHead = *pslUpdate;
        pslHead->pslPrev = NULL;
        pslHead->pslNext = NULL;
        pslTail = pslHead;
      } else
      {
        DosAllocMem((PPVOID)&psl,(ULONG)sizeof(SEGLIST),(ULONG)PAG_READ | PAG_WRITE | PAG_COMMIT);
        if( psl == NULL)
          return( NULL);
        *psl = *pslUpdate;
        pslHead->pslPrev = psl;
        psl->pslNext = pslHead;
        psl->pslPrev = NULL;
        pslHead = psl;
      }
      return( pslHead);
      break;

    case ADD_TAIL_SEG:
      if( pslTail == NULL)
      {
        DosAllocMem((PPVOID)&pslHead,(ULONG)sizeof(SEGLIST),(ULONG)PAG_READ | PAG_WRITE |
                PAG_COMMIT);
        if( pslHead == NULL)
          return( NULL);
        *pslHead = *pslUpdate;
        pslHead->pslPrev = NULL;
        pslHead->pslNext = NULL;
        pslTail = pslHead;
      } else
      {
        DosAllocMem((PPVOID)&psl,(ULONG)sizeof(SEGLIST),(ULONG)PAG_READ | PAG_WRITE |
                PAG_COMMIT);
        if( psl == NULL)
          return( NULL);
        *psl = *pslUpdate;
        pslTail->pslNext = psl;
        psl->pslPrev = pslTail;
        psl->pslNext = NULL;
        pslTail = psl;
      }
      return( pslTail);
      break;

    case MAKE_TAIL_SEG:
      if( pslUpdate == pslTail)
        return( pslTail);
      if( pslUpdate == pslHead)
      {
        pslHead = pslHead->pslNext;
        pslHead->pslPrev = NULL;
      } else
      {
        pslUpdate->pslPrev->pslNext = pslUpdate->pslNext;
        pslUpdate->pslNext->pslPrev = pslUpdate->pslPrev;
      }
      pslTail->pslNext = pslUpdate;
      pslUpdate->pslPrev = pslTail;
      pslTail = pslUpdate;
      pslTail->pslNext = NULL;
      return( pslTail);
      break;

    case MAKE_HEAD_SEG:
      if( pslUpdate == pslHead)
        return( pslHead);
      if( pslUpdate == pslTail)
      {
        pslTail = pslTail->pslPrev;
        pslTail->pslNext = NULL;
      } else
      {
        pslUpdate->pslPrev->pslNext = pslUpdate->pslNext;
        pslUpdate->pslNext->pslPrev = pslUpdate->pslPrev;
      }
      pslHead->pslPrev = pslUpdate;
      pslUpdate->pslNext = pslHead;
      pslHead = pslUpdate;
      pslHead->pslPrev = NULL;
      return( pslHead);
      break;

    case DEL_SEG:
      for( psl = pslHead; psl != NULL; psl = psl->pslNext)
      {
        if( psl->lSegId == pslUpdate->lSegId)
        {
          if( psl == pslHead)
          {
            pslHead = psl->pslNext;
            if( pslHead == NULL)
              pslTail = NULL;
            else
              pslHead->pslPrev = NULL;
          }else if( psl == pslTail)
          {
            pslTail = psl->pslPrev;
            pslTail->pslNext = NULL;
          } else
          {
            (psl->pslPrev)->pslNext = psl->pslNext;
            (psl->pslNext)->pslPrev = psl->pslPrev;
          }
          DosFreeMem( psl);
          return( psl);
          break;
        }
      }
      return( NULL);
      break;

    default:
      return( NULL);
  }
}

/******************************************************************************/
/*                                                                            */
/* set the default viewing transform                                          */
/*                                                                            */
/******************************************************************************/
VOID SetDVTransform( fx11, fx12, fx21, fx22, l31, l32, lType)
FIXED fx11;
FIXED fx12;
FIXED fx21;
FIXED fx22;
FIXED l31;
FIXED l32;
FIXED lType;
{
  MATRIXLF  matlf;

  matlf.fxM11 = fx11;
  matlf.fxM12 = fx12;
  matlf.lM13  = 0L;
  matlf.fxM21 = fx21;
  matlf.fxM22 = fx22;
  matlf.lM23  = 0L;
  matlf.lM31  = l31;
  matlf.lM32  = l32;
  matlf.lM33  = 1L;
  if (GpiSetDefaultViewMatrix( hpsClient, 9L, &matlf, lType) == GPI_ERROR)
      DispError(habMain,hwndClient);
}

/******************************************************************************/
/*                                                                            */
/* Determine the bounding rect of a segment.                                  */
/*                                                                            */
/* Use special knowledge that the only non-identity part of the segment       */
/* transform is the translation part.                                         */
/******************************************************************************/
VOID SetRect( psl)
PSEGLIST psl;
{
  psl->rclCurrent         = psl->rclBitBlt;      /* world space bounding rect */
  psl->rclCurrent.xLeft   += psl->ptlModelXlate.x;
  psl->rclCurrent.yBottom += psl->ptlModelXlate.y;
  psl->rclCurrent.xRight  += psl->ptlModelXlate.x;
  psl->rclCurrent.yTop    += psl->ptlModelXlate.y;
}

/******************************************************************************/
/*                                                                            */
/* Return a pointer to a segment list member, based on segment id.            */
/*                                                                            */
/******************************************************************************/
PSEGLIST SegListGet( lSeg)
LONG lSeg;
{
  PSEGLIST  psl;

  for( psl = pslHead; psl != NULL; psl = psl->pslNext)
    if( psl->lSegId == lSeg)
      return( psl);
  return( NULL);
}


/******************************************************************************
*
*  Name        : DrawPiece
*
*  Description : Draw one piece.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : HPS      hps
*                PSEGLIST psl
*                BOOL     fFill
*
*  Return      : VOID
*
******************************************************************************/

VOID DrawPiece(HPS hps, PSEGLIST psl, BOOL fFill)
{
   POINTL  aptl[4];
   APIRET  rc;

   if(psl->fVisible)                          /* don't bother if not visible */
   {
      /*
       *  Setup source and target points based on model transform of this peice
       */

      aptl[2].x = aptl[2].y = 0L;                      /* source: lower left */
      aptl[3] = psl->aptlBitBlt[3];                   /* source: upper right */

                                                               /* target:    */
      aptl[0].x = psl->rclBitBlt.xLeft + psl->ptlModelXlate.x; /* lower left */
      aptl[0].y = psl->rclBitBlt.yBottom + psl->ptlModelXlate.y;

      rc =
      GpiConvert(hpsClient, CVTC_MODEL, CVTC_DEVICE, 1L, aptl);

      if (rc == GPI_ERROR)
         DispError(habMain,hwndClient);

      aptl[1].x = aptl[0].x + aptl[3].x;              /* target: upper right */
      aptl[1].y = aptl[0].y + aptl[3].y;           /* (equal to source size) */

      /*
       *  Punch a hole
       */

      rc =
      GpiBitBlt(
         hps,                            /* target presentation-space handle */
         psl->hpsHole,                   /* source presentation-space handle */
         4L,                             /* number of points in array        */
         aptl,                           /* address of array                 */
         ROP_SRCAND,                     /* mixing method                    */
         BBO_IGNORE);                    /* line/column-compression flag     */

      if (rc == GPI_ERROR)
         DispError(habMain,hwndClient);

      if (fFill)
      {
         /*
          *  Fill the hole
          */
         rc =
         GpiBitBlt(
            hps,                         /* target presentation-space handle */
            psl->hpsFill,                /* source presentation-space handle */
            4L,                          /* number of points in array        */
            aptl,                        /* address of array                 */
            ROP_SRCPAINT,                /* mixing method                    */
            BBO_IGNORE);                 /* line/column-compression flag     */

         if (rc == GPI_ERROR)
            DispError(habMain,hwndClient);

      }   /* end if (fFill) */

   }   /* end if(psl->fVisible) */

}   /* end DrawPiece() */

/******************************************************************************/
/*                                                                            */
/* Draw the picture, using the passed region for clipping.                    */
/* Intersect the bounding box used for the clip region with the client rect.  */
/* Test each segment to see if its bounding box intersects the bounding box   */
/* of the clipping region.  Draw only if there is an intersection.            */
/*                                                                            */
/******************************************************************************/
BOOL DoDraw(HPS hps,HRGN hrgn,BOOL fPaint)
{
  HRGN      hrgnOld;
  RECTL     rcl, rclRegion, rclDst, rclClient;
  PSEGLIST  psl;
  ULONG     ulPostCt;

  if( fPaint && !fFirstLoad)
  {
    if (GpiSetColor( hps, CLR_BACKGROUND) == GPI_ERROR)
        DispError(habMain,hwndClient);
    if (GpiPaintRegion( hps, hrgn) == GPI_ERROR)             /* erase region */
        DispError(habMain,hwndClient);
  }

  if (GpiQueryRegionBox( hps, hrgn, &rclRegion) == RGN_ERROR)
      DispError(habMain,hwndClient);

  WinQueryWindowRect( hwndClient, &rclClient);

  if( !WinIntersectRect( habAsync, &rclRegion, &rclRegion, &rclClient))
    return( FALSE);                                  /* not in client window */

  if (GpiSetClipRegion( hps, hrgn, &hrgnOld) == RGN_ERROR) /* make the clip region */
      DispError(habMain,hwndClient);

  for( psl = pslHead; psl != NULL; psl = psl->pslNext)    /* scan all pieces */
  {
    /**************************************************************************/
    /* get the piece bounding box in device coordinates                       */
    /**************************************************************************/
    rcl = psl->rclCurrent;
    if (GpiConvert( hpsClient, CVTC_MODEL, CVTC_DEVICE, 2L, (PPOINTL)&rcl) == GPI_ERROR)
        DispError(habMain,hwndClient);
    rcl.xRight++;
    rcl.yTop++;

    /**************************************************************************/
    /* if the piece might be visible, and drawing allowed, draw the piece     */
    /**************************************************************************/

    if( WinIntersectRect( habAsync, &rclDst, &rcl, &rclRegion))
       {
       DosQueryEventSem( hevDrawOn, &ulPostCt);          /* drawing enabled? */
         if( ulPostCt)                                                /* yes */
           DrawPiece( hps, psl, TRUE);
         else
           break;                   /* drawing is disabled; don't draw piece */
       }
  }

  if (GpiSetClipRegion( hps, NULLHANDLE, &hrgnOld) == RGN_ERROR)
      DispError(habMain,hwndClient);

  DosQueryEventSem( hevDrawOn, &ulPostCt);
  if( ulPostCt)                                        /* drawing is enabled */
    if (GpiSetRegion( hpsClient, hrgnInvalid, 0L, NULL) == RGN_ERROR)
        DispError(habMain,hwndClient);

  return( TRUE);

}   /* end DoDraw() */

/******************************************************************************/
/*                                                                            */
/* get bounding rect of whole picture in model coordinates                    */
/*                                                                            */
/******************************************************************************/
VOID CalcBounds(VOID)
{
  PSEGLIST  psl;
  RECTL     rclOld;
  ULONG     ulPostKillDraw, ulPostCt;

  if( !pslHead)
    return;
  rclBounds = rclOld = pslHead->rclCurrent;

  /*
   * get start killdraw post count
   */
  DosQueryEventSem(hevKillDraw, &ulPostKillDraw);

  for( psl = pslHead->pslNext; psl != NULL; psl = psl->pslNext) {
    DosQueryEventSem(hevKillDraw, &ulPostCt);
    if (ulPostKillDraw != ulPostCt) {
        rclBounds = rclOld;
        return;
        }
    WinUnionRect(habAsync, &rclBounds, &rclBounds, &(psl->rclCurrent));
    }
}

/******************************************************************************/
/*                                                                            */
/* Calculate and set the default viewing transform based on zoom and scroll   */
/*                                                                            */
/******************************************************************************/
VOID CalcTransform( hwnd)
HWND hwnd;
{
  RECTL     rclClient;
  POINTL    ptlCenter, ptlTrans, ptlScale, aptl[4], ptlOrg, aptlSides[12];
  PSEGLIST  psl;
  LONG      l;
  MATRIXLF  matlf;
  ULONG     ulPostCt, ulPostKillDraw;


  /*
   * get start killdraw post count
   */
  DosQueryEventSem(hevKillDraw, &ulPostKillDraw);


  /****************************************************************************/
  /* from bounding rect of picture get center of picture                      */
  /****************************************************************************/
  ptlCenter.x = (rclBounds.xLeft   + rclBounds.xRight) / 2;
  ptlCenter.y = (rclBounds.yBottom + rclBounds.yTop  ) / 2;

  /****************************************************************************/
  /* translate center of picture to origin                                    */
  /****************************************************************************/
  SetDVTransform( (FIXED)UNITY
                , (FIXED)0
                , (FIXED)0
                , (FIXED)UNITY
                , -ptlCenter.x
                , -ptlCenter.y
                , TRANSFORM_REPLACE);

  /****************************************************************************/
  /* scale down to 1:1 of bitmap in file                                      */
  /****************************************************************************/
  if (pbmp2BitmapFile->cbFix  == sizeof(BITMAPINFOHEADER))
      {
      ptlScale.x = UNITY * ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cx
              / (ptlTopRight.x - ptlBotLeft.x);
      ptlScale.y = UNITY * ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cy
              / (ptlTopRight.y - ptlBotLeft.y);
      }
  else
      {
      ptlScale.x = UNITY * pbmp2BitmapFile->cx
              / (ptlTopRight.x - ptlBotLeft.x);
      ptlScale.y = UNITY * pbmp2BitmapFile->cy
              / (ptlTopRight.y - ptlBotLeft.y);
      }

  /****************************************************************************/
  /* add in zoom scale                                                        */
  /****************************************************************************/
  ptlScale.x += ptlScale.x * lScale / (ZOOM_MAX + 1);
  ptlScale.y += ptlScale.y * lScale / (ZOOM_MAX + 1);

  SetDVTransform( (FIXED)ptlScale.x
                , (FIXED)0
                , (FIXED)0
                , (FIXED)ptlScale.y
                , 0L
                , 0L
                , TRANSFORM_ADD);

  /****************************************************************************/
  /* translate center of picture to center of client window                   */
  /****************************************************************************/
  WinQueryWindowRect( hwnd, &rclClient);
  ptlTrans.x = (rclClient.xRight - rclClient.xLeft)   / 2;
  ptlTrans.y = (rclClient.yTop   - rclClient.yBottom) / 2;

  /****************************************************************************/
  /* add in horizontal and vertical scrolling factors                         */
  /****************************************************************************/
  ptlTrans.x -= ptsScrollPos.x - ptsHalfScrollMax.x;
  ptlTrans.y += ptsScrollPos.y - ptsHalfScrollMax.y;
  SetDVTransform( (FIXED)UNITY
                , (FIXED)0
                , (FIXED)0
                , (FIXED)UNITY
                , ptlTrans.x
                , ptlTrans.y
                , TRANSFORM_ADD);

  if (GpiQueryDefaultViewMatrix( hpsClient, 9L, &matlf) == GPI_ERROR)
      DispError(habMain,hwndClient);
  if (GpiSetDefaultViewMatrix( hpsClient, 9L, &matlf, TRANSFORM_REPLACE) == GPI_ERROR)
      DispError(habMain,hwndClient);

  /****************************************************************************/
  /* create bitmaps for pieces                                                */
  /****************************************************************************/

  ptlOffset = ptlBotLeft;                  /* BottomLeft corner in dev coord  */
  if (GpiConvert( hpsClient, CVTC_WORLD, CVTC_DEVICE, 1L, &ptlOffset) == GPI_ERROR)
      DispError(habMain,hwndClient);
  if( (ptlScale.x != ptlScaleRef.x) || (ptlScale.y != ptlScaleRef.y))
  {
    ptlScaleRef = ptlScale;

    /**************************************************************************/
    /* create a shadow bitmap of the original, sized to current output size   */
    /**************************************************************************/
    aptl[0] = ptlBotLeft;                  /* current output rect, dev coord  */
    aptl[1] = ptlTopRight;
    if (GpiConvert( hpsClient, CVTC_WORLD, CVTC_DEVICE, 2L, aptl) == GPI_ERROR)
        DispError(habMain,hwndClient);
    aptl[0].x -= ptlOffset.x;              /* put bottom left at (0,0)        */
    aptl[0].y -= ptlOffset.y;
    aptl[1].x -= ptlOffset.x;
    aptl[1].y -= ptlOffset.y;
#ifdef fred
    aptl[1].x -= ptlOffset.x - 1;          /* correct for coordinate convert  */
    aptl[1].y -= ptlOffset.y - 1;
#endif
    aptl[2].x = 0L;
    aptl[2].y = 0L;
  if (pbmp2BitmapFile->cbFix  == sizeof(BITMAPINFOHEADER))
     {
    aptl[3].x = ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cx;  /*bitmap dimensions   */
    aptl[3].y = ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cy;
     }
  else
     {
    aptl[3].x = pbmp2BitmapFile->cx;        /* bitmap dimensions               */
    aptl[3].y = pbmp2BitmapFile->cy;
     }
    if (GpiSetBitmap( hpsBitmapSize, hbmBitmapSize) == (HBITMAP)BMB_ERROR)
        DispError(habMain,hwndClient);
    if (GpiBitBlt( hpsBitmapSize               /* copy the bitmap                 */
                 , hpsBitmapFile
                 , 4L
                 , aptl
                 , ROP_SRCCOPY
                 , BBO_IGNORE) == GPI_ERROR)
        DispError(habMain,hwndClient);

    for( psl = pslHead; psl != NULL; psl = psl->pslNext)
    {
      DosQueryEventSem( hevTerminate, &ulPostCt);
      if( ulPostCt) /* exit if quit   */
        break;

      DosQueryEventSem( hevKillDraw, &ulPostCt);
      if( ulPostCt != ulPostKillDraw) {
        break;
        }

      aptl[0].x = psl->rclBitBlt.xLeft;     /* bounding rect in world space   */
      aptl[0].y = psl->rclBitBlt.yBottom;
      aptl[1].x = psl->rclBitBlt.xRight;
      aptl[1].y = psl->rclBitBlt.yTop;
      aptl[2] = aptl[0];
      aptl[3] = aptl[1];
      if (GpiConvert( hpsClient, CVTC_WORLD, CVTC_DEVICE, 2L, &aptl[2]) == GPI_ERROR)
          DispError(habMain,hwndClient);
      ptlOrg = aptl[2];
      aptl[3].x -= ptlOrg.x - 1;            /* bitmap rect of piece           */
      aptl[3].y -= ptlOrg.y - 1;
      aptl[2].x = 0L;
      aptl[2].y = 0L;
      psl->aptlBitBlt[0] = aptl[0];
      psl->aptlBitBlt[1] = aptl[1];
      psl->aptlBitBlt[2] = aptl[2];
      psl->aptlBitBlt[3] = aptl[3];

      /************************************************************************/
      /* compute the piece control points                                     */
      /************************************************************************/
      for ( l = 0; l < 12; l++)
        aptlSides[l] = psl->aptlSides[l];
      if (GpiConvert( hpsClient, CVTC_WORLD, CVTC_DEVICE, 12L, aptlSides) == GPI_ERROR)
          DispError(habMain,hwndClient);
      for ( l = 0; l < 12; l++)
      {
        aptlSides[l].x -= ptlOrg.x;
        aptlSides[l].y -= ptlOrg.y;
      }

      /************************************************************************/
      /* prepare the mask to punch a hole in the output bitmap                */
      /************************************************************************/
      if (GpiSetClipPath( psl->hpsHole, 0L, SCP_RESET)== GPI_ERROR)/* no clip path          */
          DispError(habMain,hwndClient);
      if (GpiBitBlt( psl->hpsHole                        /* fill with 1's         */
                   , NULLHANDLE
                   , 2L
                   , &psl->aptlBitBlt[2]
                   , ROP_ONE
                   , BBO_IGNORE) == GPI_ERROR)
          DispError(habMain,hwndClient);

      if (GpiBeginPath( psl->hpsHole, 1L) == GPI_ERROR)  /* define a clip path    */
          DispError(habMain,hwndClient);
      if (GpiMove( psl->hpsHole, &aptlSides[11]) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiPolySpline( psl->hpsHole, 12L, aptlSides) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiEndPath( psl->hpsHole) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiSetClipPath( psl->hpsHole, 1L, SCP_AND) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiBitBlt( psl->hpsHole                        /* fill with 0's         */
                   , NULLHANDLE
                   , 2L
                   , &psl->aptlBitBlt[2]
                   , ROP_ZERO
                   , BBO_IGNORE) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiSetClipPath( psl->hpsHole, 0L, SCP_RESET) == GPI_ERROR)  /* clear the clip path   */
          DispError(habMain,hwndClient);

      /************************************************************************/
      /* prepare the mask to fill the hole in the output bitmap               */
      /************************************************************************/
      aptl[0] = psl->aptlBitBlt[2];
      aptl[1] = psl->aptlBitBlt[3];
      aptl[2] = aptl[0];
      if (GpiBitBlt( psl->hpsFill                        /* make inverse of hole  */
                   , psl->hpsHole
                   , 3L
                   , aptl
                   , ROP_NOTSRCCOPY
                   , BBO_IGNORE) == GPI_ERROR)
          DispError(habMain,hwndClient);

      aptl[0] = psl->aptlBitBlt[2];
      aptl[1] = psl->aptlBitBlt[3];
      aptl[2].x = ptlOrg.x - ptlOffset.x;            /* pick the right part   */
      aptl[2].y = ptlOrg.y - ptlOffset.y;            /* of the sized bitmap   */
      if (GpiBitBlt( psl->hpsFill                        /* fill with data        */
                   , hpsBitmapSize
                   , 3L
                   , aptl
                   , ROP_SRCAND
                   , BBO_IGNORE) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiSetClipPath( psl->hpsFill, 0L, SCP_RESET) == GPI_ERROR)  /* clear the clip path   */
          DispError(habMain,hwndClient);
      if (GpiSetColor( psl->hpsFill, CLR_RED) == GPI_ERROR)/* draw the outline      */
          DispError(habMain,hwndClient);
      if (GpiMove( psl->hpsFill, &aptlSides[11]) == GPI_ERROR)
          DispError(habMain,hwndClient);
      if (GpiPolySpline( psl->hpsFill, 12L, aptlSides) == GPI_ERROR)
          DispError(habMain,hwndClient);
      DrawPiece( hpsClient, psl, TRUE);
    }
  }
}

/******************************************************************************/
/*                                                                            */
/* Redraw the entire client window.                                           */
/*                                                                            */
/******************************************************************************/
VOID Redraw(VOID)
{
  RECTL   rclInvalid;
  HRGN    hrgnUpdt;
  POINTL  aptlUpdtNew[3];

  WinQueryWindowRect( hwndClient, &rclInvalid);
  hrgnUpdt = GpiCreateRegion( hpsBitmapBuff, 1L, &rclInvalid);
  DoDraw( hpsBitmapBuff, hrgnUpdt, TRUE);
  if (GpiDestroyRegion( hpsBitmapBuff, hrgnUpdt) == GPI_ERROR)
     DispError(habMain,hwndClient);
  aptlUpdtNew[0].x = rclInvalid.xLeft;
  aptlUpdtNew[0].y = rclInvalid.yBottom;
  aptlUpdtNew[1].x = rclInvalid.xRight;
  aptlUpdtNew[1].y = rclInvalid.yTop;
  ROUND_DOWN_MOD( aptlUpdtNew[0].x, lByteAlignX);         /* round down       */
  ROUND_DOWN_MOD( aptlUpdtNew[0].y, lByteAlignY);         /* round down       */
  ROUND_UP_MOD(   aptlUpdtNew[1].x, lByteAlignX);         /* round up         */
  ROUND_UP_MOD(   aptlUpdtNew[1].y, lByteAlignY);         /* round up         */
  aptlUpdtNew[2] = aptlUpdtNew[0];

  if (!fFirstLoad)
  {
     if (GpiBitBlt( hpsClient
                  , hpsBitmapBuff
                  , 3L
                  , aptlUpdtNew
                  , ROP_SRCCOPY
                  , BBO_IGNORE ) == GPI_ERROR)
         DispError(habMain,hwndClient);
  }   /* end if (!fFirstLoad) */
}

/******************************************************************************
*
*  Name        : CorrelatePPOINTL
*
*  Description : perform bitmap-based correlation
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : PPOINTL pptl
*
*  Return      : PSEGLIST
*
******************************************************************************/

PSEGLIST Correlate(PPOINTL pptl)
{
   PSEGLIST  psl;
   POINTL    aptl[2];
   LONG      lColor;
   RECTL     rcl;
   APIRET    rc;                                    /* return code from APIs */


   aptl[0] = aptl[1] = *pptl;                     /* target lower-left point */

   aptl[1].x++;                           /* target bitmap upper-right point */
   aptl[1].y++;

   /*
    *  Set all hpsBitmapSave pels to 1.  We're doing this so we can draw
    *  the correlated segment (if any) overtop of it.  Then, if the color
    *  didn't change, we know that we hit a clipped part of the segment
    *  (piece).
    */
   rc =
   GpiBitBlt(
      hpsBitmapSave,                     /* target presentation-space handle */
      NULLHANDLE,                        /* source presentation-space handle */
      2L,                                /* number of points in array        */
      aptl,                              /* address of array                 */
      ROP_ONE,                           /* mixing method                    */
      BBO_IGNORE);                       /* line/column-compression flag     */

   if (rc  == GPI_ERROR)
      DispError(habMain,hwndClient);

   /*
    *  Retrieve color attribute for correlation point
    */

   lColor =                                               /* should return 0 */
   GpiQueryPel(hpsBitmapSave, pptl);

   if (lColor == GPI_ALTERROR)
      DispError(habMain,hwndClient);

   /*
    *  Do a manual correlation by determining if the correlation point
    *  intersects the region occupied be each segment (piece).
    */

   for( psl = pslTail; psl != NULL; psl = psl->pslPrev)
   {
      /*
       *  get device coordinates for current segment (peice) bounding box
       */

      rcl = psl->rclCurrent;
      rc =
      GpiConvert(
         hpsClient,             /* presentation-space handle                 */
         CVTC_MODEL,            /* source coordinate space                   */
         CVTC_DEVICE,           /* target coordinate space                   */
         2L,                    /* number of coordinate pairs in structure   */
         (PPOINTL)&rcl);        /* address of structure for coordinate pairs */

      if (rc == GPI_ERROR)
         DispError(habMain,hwndClient);

      rcl.xRight++;
      rcl.yTop++;

      /*
       *  Does the point fall inside this segment's (peice's) bounding box?
       *  If so, did the color of the pel change when we drew the piece?  If
       *  so, we selected a point inside this piece (segment) which isn't
       *  clipped.  Return it.
       */

      if (WinPtInRect(habAsync, &rcl, pptl))
      {
          DrawPiece( hpsBitmapSave, psl, FALSE);      /* draw clipped bitmap */
          if(GpiQueryPel( hpsBitmapSave, pptl) != lColor)   /* color change? */
             break;                                             /* got a hit */
      }
   }   /* end for(all segments in our linked list) */

   return( psl);

}   /* end Correlate() */

/******************************************************************************/
/*                                                                            */
/* mark a whole island                                                        */
/*                                                                            */
/******************************************************************************/
VOID MarkIsland(PSEGLIST pslMark,BOOL fMark)
{
  PSEGLIST  psl;
  BOOL      fFirst;

  for( psl = pslMark, fFirst = TRUE
     ; (psl != pslMark) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
      psl->fIslandMark = fMark;               /* mark as island member        */
}

/********************************  END MISC.C  *******************************/
