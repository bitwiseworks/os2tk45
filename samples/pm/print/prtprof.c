/****************************************************************************
 * OS/2 Sample Print Application PRTSAMP
 *
 * File name: prtprof.c
 *
 * Description:  In this module are all the functions needed to save and
 *               restore the application defaults to/from the PRTSAMP.INI.
 *
 *               This source file contains the following functions:
 *
 *               GetProfile(pmp)
 *               SaveProfile(pmp)
 *               SaveVersion(pmp)
 *               SaveProfileOnly(pmp)
 *               SaveDriverData(pmp)
 *               PutProfileInfo(pmp)
 *               GetProfileInfo(pmp)
 *
 * Concepts:   profile file
 *
 * API's:      PrfQueryProfileData
 *             PrfWriteProfileData
 *             PrfQueryProfileSize
 *             WinQueryWindowPos
 *             WinQueryWindowUShort
 *             WinSetWindowPos
 *
 *    Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PMASSERT.H
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
 ****************************************************************************/

/* os2 includes */
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
#define INCL_WINSHELLDATA
#define INCL_GPITRANSFORMS
#define INCL_DEV
#define INCL_SPL
#define INCL_SPLDOSPRINT
#include <os2.h>

/* c language includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <process.h>
#include <memory.h>
#include <sys\types.h>
#include <sys\stat.h>

/* application includes */
#include "prtsamp.h"
#include "prtsdlg.h"
#include "pmassert.h"

static const LONG lProgramVersion = 2100L;

static const PSZ pszApplication = "PRTSAMP";
static const PSZ pszProfileKey = "Profile";
static const PSZ pszVersionKey = "Version";
static const PSZ pszDriverDataKey = "DriverData";

VOID GetProfileInfo(PMAIN_PARM pmp);
VOID PutProfileInfo(PMAIN_PARM pmp);
BOOL SaveDriverData(PMAIN_PARM pmp);
BOOL SaveProfileOnly(PMAIN_PARM pmp);
BOOL SaveVersion(PMAIN_PARM pmp);

/***************************************************************************
 * Name    : GetDefaults
 *
 * Description:  Get the application defaults from OS2.INI (HINI_USERPROFILE)
 *
 * Concepts:  profile file
 *
 * API's:      PrfQueryProfileData
 *             PrfWriteProfileData
 *             PrfQueryProfileSize
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:   [none]
 *
 ***************************************************************************/
VOID GetProfile(PMAIN_PARM pmp)
{
   LONG    lCount, lVersion;
   BOOL    fNeedDefaults;

   fNeedDefaults = TRUE;
   lVersion = 0;
   pmp->pDriverData = (PDRIVDATA)NULL;
   pmp->cbDriverDataLen = 0;

   /*
    * First determine if the profile information in OS2.INI matches
    * the version number of this program.  If so, we will understand
    * the format of the data.  Otherwise, just use default values.
    */
   lCount = sizeof(LONG);

   if (!PrfQueryProfileData(
           HINI_USERPROFILE,
           pszApplication,
           pszVersionKey,
           (PVOID)&lVersion,
           (PULONG)&lCount) ||
       lCount != sizeof(LONG))
   {
       /*
        * The profile data for the version does not exist, so clean up
        * and other data. This deletes all keys under the application name.
        */
       PrfWriteProfileData(
           HINI_USERPROFILE,
           pszApplication,
           (PSZ)NULL,
           (PVOID)NULL,
           0L);
   }
   else if (lVersion != lProgramVersion)
   {
       /*
        * The profile data is the wrong version, so we need to clean
        * it up. This deletes all keys under the application name.
        */
       PrfWriteProfileData(
           HINI_USERPROFILE,
           pszApplication,
           (PSZ)NULL,
           (PVOID)NULL,
           0L);
   }
   else
   {
       /*
        * We have a matching version number, so we can proceed to
        * read the rest of the profile information.
        */

       /*
        * Look for the "Profile" information. We know the size of this
        * data so we're going to read as many bytes as we need. If for
        * some reason there's more info than we need that's all right
        * because we're relying on the lVersion to tell us we understand
        * the data.
        */
       lCount = sizeof(PRTSAMP_PROFILE);

       if (PrfQueryProfileData(
               HINI_USERPROFILE,
               pszApplication,
               pszProfileKey,
               (PVOID)&pmp->Profile,
               (PULONG)&lCount) &&
           lCount == sizeof(PRTSAMP_PROFILE))
       {
           fNeedDefaults = FALSE;

           /*
            * We found our profile data, which includes what
            * print object we want to use. Now look for the
            * "DriverData" data we want to use for this
            * print object.
            */
           if (PrfQueryProfileSize(
                   HINI_USERPROFILE,
                   pszApplication,
                   pszDriverDataKey,
                   (PULONG)&lCount) &&
               lCount >= sizeof(DRIVDATA))
           {
               /*
                * Found the size of the driver data. It can
                * only make sense if it is at least as big
                * as the DRIVDATA structure. Allocate some
                * space for it, and read it in.
                */
               pmp->pDriverData = (PDRIVDATA)malloc(lCount);
               if (pmp->pDriverData)
               {
                   pmp->cbDriverDataLen = lCount;

                   if (!PrfQueryProfileData(
                           HINI_USERPROFILE,
                           pszApplication,
                           pszDriverDataKey,
                           (PVOID)pmp->pDriverData,
                           (PULONG)&lCount) ||
                       lCount != pmp->cbDriverDataLen)
                   {
                       /*
                        * Didn't find any driver data or the size
                        * was wrong. Back out of our allocation.
                        */
                       free(pmp->pDriverData);
                       pmp->pDriverData = (PDRIVDATA)NULL;
                       pmp->cbDriverDataLen = 0;
                   }
               }
           }
       }

   } /* end handle correct profile version info */

   if (fNeedDefaults)
   {
       memset((PVOID)&pmp->Profile, 0, sizeof(PRTSAMP_PROFILE));
   }
   else
   {
       GetProfileInfo(pmp);
   }
}  /*  end of GetProfile() */

/**************************************************************************
 * Name:     SaveProfile
 *
 * Description: saves profile data PRTSAMP_PROFILE Application defaults
 *
 * API's:  [none]
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:  [none]
 *
 ***************************************************************************/
VOID SaveProfile(PMAIN_PARM pmp)
{
    PutProfileInfo(pmp);
    SaveVersion(pmp);
    if (SaveProfileOnly(pmp))
        SaveDriverData(pmp);

    return;
} /* end of SaveProfile() */

/**************************************************************************
 * Name:     SaveVersion
 *
 * Description: saves program version to user profile
 *
 * API's:  PrfWriteProfileData
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:  value from call to PrfWriteProfileData
 *
 ***************************************************************************/
BOOL SaveVersion(PMAIN_PARM pmp)
{
    return PrfWriteProfileData(
        HINI_USERPROFILE,
        pszApplication,
        pszVersionKey,
        (PVOID)&lProgramVersion,
        (ULONG)sizeof(lProgramVersion));
} /*  end of SaveVersion()  */

/**************************************************************************
 * Name :    SaveProfileOnly
 *
 * Description: saves program profile to user profile
 *
 * API's:  PrfWriteProfileData
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:  value from call to PrfWriteProfileData
 *
 ***************************************************************************/
BOOL SaveProfileOnly(PMAIN_PARM pmp)
{
    return PrfWriteProfileData(
        HINI_USERPROFILE,
        pszApplication,
        pszProfileKey,
        (PVOID)&pmp->Profile,
        (ULONG)sizeof(pmp->Profile));
} /* end of SaveProfileOnly()  */

/**************************************************************************
 * Name :    SaveDriverData
 *
 * Description: saves driver data to user profile
 *
 * API's:  PrfWriteProfileData
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:  value from call to PrfWriteProfileData
 *
 ***************************************************************************/
BOOL SaveDriverData(PMAIN_PARM pmp)
{
   return pmp->pDriverData == (PDRIVDATA)NULL ? FALSE :
        PrfWriteProfileData(
            HINI_USERPROFILE,
            pszApplication,
            pszDriverDataKey,
            (PVOID)pmp->pDriverData,
            (ULONG)pmp->cbDriverDataLen);
}  /*  end of SaveDriverData()  */

/**************************************************************************
 * Name :    PutProfileInfo
 *
 * Description:  writes data to the profile file
 *
 * API's:  WinQueryWindowPos
 *         WinQueryWindowUShort
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Return:  [none]
 *
 ***************************************************************************/
VOID PutProfileInfo(PMAIN_PARM pmp)
{
   SWP     swpPos;

   memset((PVOID)&pmp->Profile, 0, sizeof(PRTSAMP_PROFILE));

   /*
    * Query window position and size.
    */
   WinQueryWindowPos(pmp->hwndFrame, &swpPos);

   if ((swpPos.fl & SWP_MAXIMIZE) != 0)
   {
       pmp->Profile.flOptions = SWP_MAXIMIZE;
       pmp->Profile.cy = WinQueryWindowUShort(pmp->hwndFrame, QWS_CYRESTORE);
       pmp->Profile.cx = WinQueryWindowUShort(pmp->hwndFrame, QWS_CXRESTORE);
       pmp->Profile.y = WinQueryWindowUShort(pmp->hwndFrame, QWS_YRESTORE);
       pmp->Profile.x = WinQueryWindowUShort(pmp->hwndFrame, QWS_XRESTORE);
   }
   else if ((swpPos.fl & SWP_MINIMIZE) != 0)
   {
       pmp->Profile.flOptions = SWP_MINIMIZE;
       pmp->Profile.cy = WinQueryWindowUShort(pmp->hwndFrame, QWS_CYRESTORE);
       pmp->Profile.cx = WinQueryWindowUShort(pmp->hwndFrame, QWS_CXRESTORE);
       pmp->Profile.y = WinQueryWindowUShort(pmp->hwndFrame, QWS_YRESTORE);
       pmp->Profile.x = WinQueryWindowUShort(pmp->hwndFrame, QWS_XRESTORE);
   }
   else
   {
       pmp->Profile.flOptions = 0;
       pmp->Profile.cy = swpPos.cy;
       pmp->Profile.cx = swpPos.cx;
       pmp->Profile.y = swpPos.y;
       pmp->Profile.x = swpPos.x;
   }

   pmp->Profile.fxPointSize = pmp->fontdlg.fxPointSize;
   memcpy((PVOID)&pmp->Profile.fAttrs,
          (PVOID)&pmp->fontdlg.fAttrs,
          sizeof(pmp->Profile.fAttrs));

   memcpy((PVOID)&pmp->Profile.form,
          (PVOID)&pmp->form,
          sizeof(pmp->Profile.form));

   pmp->Profile.ulNextMode = pmp->ulMode;
   strcpy(pmp->Profile.szNextFilename, pmp->szFilename);

   strcpy(pmp->Profile.achQueueName, pmp->achQueueName);
   strcpy(pmp->Profile.achDriverName, pmp->achDriverName);

   pmp->Profile.usCopies = pmp->usCopies;
   pmp->Profile.fPrintAllPages = pmp->fPrintAllPages;
   pmp->Profile.usFirstPage = pmp->usFirstPage;
   pmp->Profile.usLastPage = pmp->usLastPage;

   return;
}  /*  end of PutProfileInfo()  */

/**************************************************************************
 * Function: GetProfileInfo
 *
 * Description:  reads data which had been saved in the profile
 *
 * API's:  WinSetWindowPos
 *
 * Parameters:  pmp = a pointer to the application main data structure
 *
 * Result: [none]
 *
 ***************************************************************************/
VOID GetProfileInfo(PMAIN_PARM pmp)
{
   /* restore Window position */
   WinSetWindowPos( pmp->hwndFrame, HWND_TOP, pmp->Profile.x, pmp->Profile.y,
                    pmp->Profile.cx, pmp->Profile.cy,
                    pmp->Profile.flOptions |
                              SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW );

   pmp->fontdlg.fxPointSize = pmp->Profile.fxPointSize;
   memcpy((PVOID)&pmp->fontdlg.fAttrs,
          (PVOID)&pmp->Profile.fAttrs,
          sizeof(pmp->Profile.fAttrs));

   memcpy((PVOID)&pmp->form,
          (PVOID)&pmp->Profile.form,
          sizeof(pmp->Profile.form));

   pmp->ulNextMode = pmp->Profile.ulNextMode;
   strcpy(pmp->szNextFilename, pmp->Profile.szNextFilename);
   strcpy(pmp->achQueueName, pmp->Profile.achQueueName);
   strcpy(pmp->achDriverName, pmp->Profile.achDriverName);

   pmp->usCopies = pmp->Profile.usCopies;
   pmp->fPrintAllPages = pmp->Profile.fPrintAllPages;
   pmp->usFirstPage = pmp->Profile.usFirstPage;
   pmp->usLastPage = pmp->Profile.usLastPage;

   return;
}  /*  end of GetProfileInfo()  */
/***************************  End of prtprof.c ****************************/
