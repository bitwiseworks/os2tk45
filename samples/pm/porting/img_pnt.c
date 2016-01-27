/**************************************************************************
 *  File name  :  img_pnt.c
 *
 *  Description:  This module contains the code for the main client window
 *                painting.  It contains these functions:
 *
 *                PaintUnSizedImage()
 *                PaintSizedImage()
 *                PaintDrawImage()
 *
 *  Concepts   :  bitmaps
 *
 *  API's      :  GpiDeleteBitmap
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *                DevOpenDC
 *                GpiCreatePS
 *                GpiCreateBitmap
 *                GpiSetBitmap
 *                GpiErase
 *                GpiSetBackColor
 *                GpiSetColor
 *                GpiSetBackMix
 *                GpiSetCurrentPosition
 *                WinMessageBox
 *                GpiImage
 *                WinInvalidateRect
 *                WinSetPointer
 *                WinQueryWindowRect
 *                WinDrawBitmap
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDIO.H, IMG_MAIN.H, IMG_XTRN.H
 *
 *  Copyright (C) 1991-1993 IBM Corporation
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
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_WINMENUS
#define INCL_WINFRAMEMGR
#define INCL_WINPOINTERS
#define INCL_WINSTDFILE
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS

#include <os2.h>
#include <string.h>
#include <stdio.h>

#include "img_main.h"
#include "img_xtrn.h"

/**************************************************************************
 *
 *  Name       : PaintDrawImage()
 *
 *  Description: Draws the image data into the bitmap
 *
 *  Concepts   : Routine is called from LoadImage()
 *               - creates a memory device context
 *               - defines a memory presentation space
 *               - associates the memory DC with the memory PS
 *               - creates a bitmap
 *               - Selects bitmap into memory DC
 *               - draws the image data into the bitmap
 *
 *  API's      :  GpiDeleteBitmap
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *                DevOpenDC
 *                GpiCreatePS
 *                GpiCreateBitmap
 *                GpiSetBitmap
 *                GpiErase
 *                GpiSetBackMix
 *                GpiSetBackColor
 *                GpiSetColor
 *                GpiSetCurrentPosition
 *                WinMessageBox
 *                GpiImage
 *                WinInvalidateRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID PaintDrawImage(void)
{
   POINTL ptl;
   DEVOPENSTRUC dop;
#ifdef PORT_16
   BITMAPINFOHEADER  BmapInfo;
#endif
#ifdef PORT_32
   BITMAPINFOHEADER2 BmapInfo;
#endif

   if (vfImgLoaded)
   {
       /*
        * The memory presentation space is defined to accommodate a
        * particular file of image data. If an image has already been
        * loaded, its resources must be deleted before a new set can be
        * defined.
        */
        GpiDeleteBitmap(vhbm);
        GpiAssociate(vhpsMem, (HDC)NULLHANDLE);
        GpiDestroyPS(vhpsMem);
        DevCloseDC(vhdcMem);
   }

   /*
    * define the memory device context
    */
   dop.pszLogAddress = (PSZ)NULL;
   dop.pszDriverName = "DISPLAY";
   vhdcMem = DevOpenDC(vhab,
                       OD_MEMORY,
                       "*",
                       2L, /* number of valid elements */
                       (PDEVOPENDATA)&dop,
                       (HDC)NULLHANDLE);

   /*
    * The memory presentation space is defined and associated with the
    * memory device context. The presentation page is the same size in
    * pels as the image.
    */
   vhpsMem = GpiCreatePS(vhab,
                         vhdcMem,
                         &vsizlImg,
                         (LONG)PU_PELS | GPIT_NORMAL | GPIA_ASSOC);

   /*
    * Create a bit map in a format compatible with the memory device
    * context.
    */

   /*
    * Define the bitmap information header.
    */
   BmapInfo.cbFix     = 16;                /* bit-map header length   */
   BmapInfo.cx        = vsizlImg.cx;
   BmapInfo.cy        = vsizlImg.cy;
   BmapInfo.cPlanes   = 1L;                /* number of bit planes    */
   BmapInfo.cBitCount = 1L;                /* number of bits per pel  */


   /*
    * The bit map is defined and selected into the memory device
    * context. The output display is cleared.
    */
   vhbm = GpiCreateBitmap(vhpsMem,
                          &BmapInfo,
                          0L,                     /* no options      */
                          (PBYTE)NULL,            /* no initial data */
                          (PBITMAPINFO2)NULL);

   GpiSetBitmap(vhpsMem, vhbm);
   GpiErase(vhpsMem);
   GpiSetBackColor(vhpsMem, vlBackClr);
   GpiSetColor(vhpsMem, vlForeClr);
   GpiSetBackMix(vhpsMem, BM_OVERPAINT);

   /*
    * The current position is set to the top-left corner of the memory
    * presentation page before GpiImage is issued to draw the image
    * into it.
    */
   ptl.x = 0L;
   ptl.y = vsizlImg.cy;
   GpiSetCurrentPosition(vhpsMem, &ptl);
#ifdef PORT_32
   {
      BYTE     acInform[50];
      PBYTE    pbFileName;

      WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);
      sprintf (acInform, "Press OK to display this image.");
      pbFileName = strrchr(vfdg.szFullFile, '\\');  /* the last backslash */
      if(pbFileName)
         pbFileName++;
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)acInform, (PSZ)pbFileName,
              0L, MB_OK | MB_INFORMATION | MB_APPLMODAL | MB_MOVEABLE);
   }
#endif
   WinEnableWindowUpdate(vhwndClient, TRUE);

   GpiImage(vhpsMem,
            0L,
            &vsizlImg,                             /* image size           */
            (LONG)((vsizlImg.cx * vsizlImg.cy)/8), /* data length in bytes */
            vpbImgBuf);                            /* image data           */

   GpiSetBitmap(vhpsMem, (HBITMAP)NULLHANDLE); /* free bit map in memory DC */
   WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);
}   /* End of PaintDrawImage */


/**************************************************************************
 *
 *  Name       : PaintUnSizedImage()
 *
 *  Description: Paints the image in the client window
 *
 *  Concepts   : Routine is called from MainWndProc()
 *
 *  API's      :  WinSetPointer
 *                GpiImage
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID PaintUnSizedImage(void)
{
   WinSetPointer(HWND_DESKTOP, vhptrWait);

   GpiImage(vhps,
            0L,
            &vsizlImg,                           /* size in pels         */
            (LONG)(vsizlImg.cx * vsizlImg.cy/8), /* data length in bytes */
            vpbImgBuf);                          /* image data           */

   WinSetPointer(HWND_DESKTOP, vhptrArrow);
}   /* End of PaintUnSizedImage */

/**************************************************************************
 *
 *  Name       : PaintSizedImage()
 *
 *  Description: Paints the image in the client window, sizes
 *               it so that the whole of the image is visible
 *               (i.e. scaled as necessary)
 *
 *  Concepts   : Routine is called from MainWndProc()
 *
 *  API's      :  WinQueryWindowRect
 *                WinSetPointer
 *                WinDrawBitmap
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID PaintSizedImage(void)
{
   RECTL  rcl;                   /* client window rectangle    */
   POINTL aptl[5];               /* array of x,y pairs         */

   /*
    * The dimensions of the target rectangle are obtained using the
    * WinQueryWindowRect() call.
    */
   WinQueryWindowRect(vhwndClient, &rcl); /* query client window size */
   aptl[0].x = 0;                         /* bottom left, in          */
   aptl[0].y = 0;                         /*  device coordinates      */
   aptl[1].x = rcl.xRight - rcl.xLeft;    /* top right, in            */
   aptl[1].y = rcl.yTop - rcl.yBottom;    /*  device coordinates      */

   /*
    * The dimensions of the source rectangle are those of the source
    * image.
    */
   aptl[2].x = 0;                         /* bottom left, in          */
   aptl[2].y = 0;                         /*  window coordinates      */
   aptl[3].x = vsizlImg.cx;               /* top right, in            */
   aptl[3].y = vsizlImg.cy;               /*  device coordinates      */

   WinSetPointer(HWND_DESKTOP, vhptrWait);

   /*
    * Draw the image.
    */
   WinDrawBitmap(vhps,
                 vhbm,
                 (PRECTL)NULL,                /* draw whole bit map   */
                 (PPOINTL)&rcl,               /* bit-map destination  */
                                              /*  is client window    */
                                              /*  rectangle.          */
                 0L,
                 0L,
                 DBM_STRETCH | DBM_IMAGEATTRS);
                                              /* stretch or compress  */
                                              /*  to suit, and use    */
                                              /*  image attributes    */
                                              /*  for color.          */

   WinSetPointer(HWND_DESKTOP, vhptrArrow);
}   /* End of PaintSizedImage */
/***************************  End of img_pnt.c   *************************/
