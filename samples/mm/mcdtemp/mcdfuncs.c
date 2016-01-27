/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCDFUNCS.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCD COMMON FUNCTIONS                                  */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains common functions used by the MCD template. */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       QMAudio() - Query master volume settings                           */
/*       GetINIInstallName() - Get the MMPM/2 install name for this device  */
/*       ConvertTime() - Convert time formats                               */
/*       GetDeviceInfo() - Get device info                                  */
/****************************************************************************/
#define INCL_BASE                    // Base OS2 functions
#define INCL_MCIOS2                  // use the OS/2 like MMPM/2 headers

#include <os2.h>                     // OS2 defines.
#include <string.h>                  // string prototypes
#include <stdlib.h>                  // standard C functions
#include <os2me.h>                   // MME includes files.
#include "mcdtemp.h"                 // MCD Function Prototypes and typedefs


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  QMAudio                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  Query Master Audio                                    */
/*                                                                          */
/* FUNCTION:  Query current volume and headphone/speaker settings from      */
/*            Master Audio.                                                 */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINSTANCE pInstance - pointer to device instance structure.         */
/*                                                                          */
/* EXIT CODES:   None                                                       */
/*                                                                          */
/****************************************************************************/
VOID QMAudio(PINSTANCE pInstance)
{
   ULONG ulrc;
   MCI_MASTERAUDIO_PARMS recMasterAudio;

   /*****************************/
   /* get current master volume */
   /*****************************/
   ulrc = mciSendCommand(pInstance->usDeviceID, MCI_MASTERAUDIO,
                      MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_MASTERVOL,
                      &recMasterAudio, 0);
   if (ulrc)
      {
      if (pInstance->ulMasterVolume == (ULONG) -1L)
         pInstance->ulMasterVolume = 75L;     //if first time error set to default
      }
   else
      pInstance->ulMasterVolume = recMasterAudio.ulReturn;

   /*****************************/
   /* get current speaker status */
   /*****************************/
   ulrc = mciSendCommand(pInstance->usDeviceID, MCI_MASTERAUDIO,
                      MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_SPEAKERS,
                      &recMasterAudio, 0);
   if (!ulrc)
      if (recMasterAudio.ulReturn)
         pInstance->Speaker = TRUE;      //SPEAKERS are turned on

   /*****************************/
   /* get current headphone status */
   /*****************************/
   ulrc = mciSendCommand(pInstance->usDeviceID, MCI_MASTERAUDIO,
                      MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_HEADPHONES,
                      &recMasterAudio, 0);

   if (!ulrc)
      if (recMasterAudio.ulReturn)
         pInstance->Headphone = TRUE;      //Headphones are turned on


}  /* of QMAudio() */




/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetINIInstallName                                      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get The INI install name for this device              */
/*                                                                          */
/* FUNCTION:  Get the INI install name for this device.  This install name  */
/*            will be used later to get additional information from the     */
/*            INI file about this device.                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINSTANCE pInstance -- pointer to instance.                         */
/*                                                                          */
/****************************************************************************/
VOID  GetINIInstallName(PINSTANCE pInstance)
{
   ULONG ulrc;
   MCI_SYSINFO_PARMS      recSysInfo;
   MCI_SYSINFO_QUERY_NAME recQueryName;

   /*****************************/
   /* clear input parameters    */
   /*****************************/
   memset(&recQueryName, 0, sizeof(MCI_SYSINFO_QUERY_NAME));

   /*********************************************************/
   /* Get install name for this device based on device type */
   /*   Use the MCI_SYSINFO_QUERY_NAMES to get the MMPM/2   */
   /*   INI file install name.                              */
   /*********************************************************/
   recSysInfo.ulItem       = MCI_SYSINFO_QUERY_NAMES;
   recSysInfo.usDeviceType = pInstance->usDeviceType;
   recSysInfo.pSysInfoParm = &recQueryName;
   recQueryName.usDeviceType = pInstance->usDeviceType;
   recQueryName.usDeviceOrd  = pInstance->usDeviceOrd;

   ulrc = mciSendCommand(0, MCI_SYSINFO, MCI_SYSINFO_ITEM | MCI_WAIT,
                       &recSysInfo, 0);

   if (!ulrc)
      strcpy(pInstance->szInstallName, recQueryName.szInstallName);

   return;

}  /* end of GetINIInstallName() */

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetDeviceInfo                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get The device info from INI file                     */
/*                                                                          */
/* FUNCTION:  Get the device info from INI file.                            */
/*            Product Info                                                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINSTANCE pInstance -- pointer to instance.                         */
/*                                                                          */
/****************************************************************************/
VOID  GetDeviceInfo(PINSTANCE pInstance)
{
   ULONG ulrc;
   MCI_SYSINFO_PARMS      recSysInfo;
   MCI_SYSINFO_LOGDEVICE  recLogDevice;

   /**************************/
   /* clear input parameters */
   /**************************/
   memset(&recLogDevice, 0, sizeof(MCI_SYSINFO_LOGDEVICE));

   /******************************************************/
   /* Get device info for this device                    */
   /*   this assumes that GetINIInstallName was already  */
   /*   called                                           */
   /******************************************************/
   recSysInfo.ulItem       = MCI_SYSINFO_QUERY_DRIVER;
   recSysInfo.usDeviceType = pInstance->usDeviceType;
   recSysInfo.pSysInfoParm = &recLogDevice;
   strcpy(recLogDevice.szInstallName, pInstance->szInstallName);

   ulrc = mciSendCommand(0, MCI_SYSINFO, MCI_SYSINFO_ITEM | MCI_WAIT,
                       &recSysInfo, 0);

   if (!ulrc)
      strcpy(pInstance->szProductInfo, recLogDevice.szProductInfo);

   return;

}  /* end of GetDeviceInfo() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ConvertTime                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Convert time from one format to another.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINSTANCE pInstance -- pointer to instance.                         */
/*                                                                          */
/* NOTES: This is just the start of a routine to convert from one time      */
/*        format to another.  You should add and delete to this routine     */
/*        as required by your MCD.                                          */
/*                                                                          */
/****************************************************************************/
ULONG  ConvertTime(ULONG ulTime, ULONG ulCurrentFormat, ULONG ulNewFormat)
{
   ULONG ulrc;
   ULONG ulNewTime=0;

   switch (ulCurrentFormat)
      {
      case MCI_FORMAT_MILLISECONDS:                // MILLISECONDS
        switch (ulNewFormat)
           {
           case MCI_FORMAT_MMTIME:                 // MMTIME
              ulNewTime = MSECTOMM(ulTime);
              break;
           default :
              ulNewTime = ulTime;
           }  /* on switch */

         break;
      case MCI_FORMAT_MMTIME:                      // MMTIME
        switch (ulNewFormat)
           {
           case MCI_FORMAT_MILLISECONDS:           // MILLISECONDS
              ulNewTime = MSECFROMMM(ulTime);
              break;
           }  /* on switch */
         break;

      case MCI_FORMAT_MSF:                    // MINUTE, SECONDS, FRAMES
      case MCI_FORMAT_TMSF:                   // TRACKS, MINUTE, SECONDS, FRAMES
      case MCI_FORMAT_CHAPTERS:               // CHAPTERS
      case MCI_FORMAT_FRAMES:                 // FRAMES
      case MCI_FORMAT_HMS:                    // HOURS, MINUTES, SECONDS
      case MCI_FORMAT_HMSF:                   // HOURS, MINUTES, SECONDS, FRAMES
      case MCI_FORMAT_TRACKS:                 // TRACKS
      case MCI_FORMAT_BYTES:                  // BYTES
      case MCI_FORMAT_SAMPLES:                // SAMPLES
      default :
         ulNewTime = ulTime;
      }  /* on switch */

   return(ulNewTime);

}  /* end of ConvertTime() */



