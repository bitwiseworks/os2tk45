/*static char *SCCSID = "@(#)digital.c	6.4 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE:digital.c                                                    *
 *                                                                         *
 *       DESCRIPTION: This file contains the routines necessary for        *
 *                    updating the digital clock face                      *
 *                                                                         *
 *                                                                         *
 *      Copyright 1989, 1990, 1992 IBM Corp.                               *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *  ClkDrawDigitalString()
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME
#include <os2.h>
#include <string.h>
#include "clock.h"
#include "res.h"
#include "clkdata.h"




/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/
POINTL aptlSeg1[] =
{
    {  2, 40 },
    { 18, 40 },
    { 20, 42 },
    {  0, 42 },
};
/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

/****************************************************************\
 *  Routine Name:
 *--------------------------------------------------------------
 *
 *  Name: ClkDrawDigitalString()
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:VOID
 *
 *
\****************************************************************/
VOID ClkDrawDigitalString(HPS hps, char *psz, RECTL *prcl,
                          LONG clrFore, LONG clrBack)
{

    RECTL rclDevice;
    RECTL rclChar;
    USHORT cch, cxChar, cyChar, cxSegment, cySegment, i;
    LONG lPSid;
    USHORT xRemainder;

    /*
     * Do things in device coordinates to avoid problems with rounding.
     */
    rclDevice = *prcl;
    GpiConvert(hps, CVTC_WORLD, CVTC_DEVICE, 2L, (PPOINTL)&rclDevice);

    /*
     * Save and reset the PS to the default state.
     */
    lPSid = GpiSavePS(hps);
    GpiResetPS(hps, GRES_ATTRS);

    /*
     * Go into RGB mode
     */
    GpiCreateLogColorTable(hps, 0L, LCOLF_RGB, 0L, 0L, NULL);

    cch = (USHORT)strlen(psz);

    /*
     * How wide and how tall will each character be?
     */
    cxChar = (USHORT)(rclDevice.xRight - rclDevice.xLeft) / cch;
    cyChar = (USHORT)(rclDevice.yTop   - rclDevice.yBottom);

    cxSegment = cxChar / (USHORT)5;

    /*
     * Find out how much, if any we'll need to center the
     * digits by.
     */
    xRemainder = (USHORT)(rclDevice.xRight - rclDevice.xLeft) % cch;

    WinSetRect(NULLHANDLE, &rclChar,
               (rclDevice.xLeft + (xRemainder / 2) ),
               (rclDevice.yBottom),
               ( (rclDevice.xLeft + (xRemainder / 2)) + cxChar - cxSegment),
               (rclDevice.yBottom + cyChar) );

    for (i = 0; i < cch; i++)
    {
        WinDrawBorder(hps, &rclChar, cxSegment, cySegment, clrFore, clrBack,
                DB_INTERIOR);
        WinOffsetRect(NULLHANDLE, &rclChar, cxChar, 0);
    }

    /*
     * Put it back like we found it.
     */
    GpiRestorePS (hps, lPSid);
}


/*--------------------------------------------------------------*\
 *  End of file : digital.c
\*--------------------------------------------------------------*/
