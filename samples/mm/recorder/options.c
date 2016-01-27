/******************************************************************************
 * File Name    :  OPTIONS.C
 *
 * Description  :  This file contains the C source code required for the
 *                 Recorder sample program.
 *
 * MMPM/2 API's :  List of all MMPM/2 API's that are used in
 *                 this module
 *
 *                 mmioOpen
 *                 mmioGetHeader
 *                 mmioClose
 *
 * Copyright (C) IBM 1993
 ******************************************************************************/

#define  INCL_WIN                      /* required to use Win APIs.           */
#define  INCL_PM                       /* required to use PM APIs.            */
#define  INCL_OS2MM                    /* required for MCI and MMIO headers   */
#define  INCL_SW                       /* required for secondary window       */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <string.h>

#include "recorder.h"

/*
 * Procedure/Function Prototypes
 */
VOID    EnableTheOption( USHORT idOption );
VOID    DisableTheOption( HWND hwndMenu, USHORT idOption );

extern  USHORT MessageBox( USHORT usMessageID, ULONG  ulStyle );

extern  HWND hwndMenu;
extern  HWND hwndFrame;
extern  HAB  hab;
/******************************************************************************
 * Name         : SelectTheOption
 *
 * Description  : This procedure will select the menu pulldown option.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : idOption - Id for the selected menu pulldown option.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID  SelectTheOption( USHORT idOption )
{
   USHORT   usAttrib;

   /*
    * Query the selected menu pulldown to get their current state...
    */
   usAttrib = (USHORT) WinSendMsg( hwndMenu,
                                   MM_QUERYITEMATTR,
                                   MPFROM2SHORT(idOption, TRUE),
                                   MPFROMSHORT(MIA_CHECKED));
   if (!(usAttrib & MIA_CHECKED))
   {
      /*
       * Selected option was not in selected state, set the attribute and
       * select it.
       */
      usAttrib ^= MIA_CHECKED;
      WinSendMsg( hwndMenu,
                  MM_SETITEMATTR,
                  MPFROM2SHORT( idOption, TRUE),
                  MPFROM2SHORT( MIA_CHECKED, usAttrib ) );
   }
   return;
}

/******************************************************************************
 * Name         : DeselectTheOption
 *
 * Description  : This procedure will deselect the menu pulldown option.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : idOption - Id for the selected menu pulldown option.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID  DeselectTheOption( USHORT idOption )
{
   USHORT   usAttrib;

   /*
    * Query the selected menu pulldown to get their current state...
    */
   usAttrib = (USHORT) WinSendMsg( hwndMenu,
                                   MM_QUERYITEMATTR,
                                   MPFROM2SHORT(idOption, TRUE),
                                   MPFROMSHORT(MIA_CHECKED));
   if (usAttrib & MIA_CHECKED)
   {
      /*
       * Selected option was in selected state, set the attribute and
       * deselect it.
       */
      usAttrib ^= MIA_CHECKED;
      WinSendMsg( hwndMenu,
                  MM_SETITEMATTR,
                  MPFROM2SHORT( idOption, TRUE),
                  MPFROM2SHORT( MIA_CHECKED, usAttrib ) );
   }
   return;
}


/******************************************************************************
 * Name         : DisableTheOption
 *
 * Description  : This procedure will disable the menu pulldown option.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : hwndMenu - handle for the menu.
 *                idOption - Id for the selected menu pulldown option.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID  DisableTheOption( HWND hwndMenu, USHORT idOption )
{
   USHORT   usAttrib;

   /*
    * Query the selected menu pulldown to get their current state...
    */
   usAttrib = (USHORT) WinSendMsg( hwndMenu,
                                   MM_QUERYITEMATTR,
                                   MPFROM2SHORT(idOption, TRUE),
                                   MPFROMSHORT(MIA_DISABLED));
   if (!(usAttrib & MIA_DISABLED))
   {
      /*
       * Selected option was in enable state, set the attribute and
       * disable it.
       */
      usAttrib ^= MIA_DISABLED;
      WinSendMsg( hwndMenu,
                  MM_SETITEMATTR,
                  MPFROM2SHORT( idOption, TRUE ),
                  MPFROM2SHORT( MIA_DISABLED, usAttrib));
   }
   return;
}


/******************************************************************************
 * Name         : EnableTheOption
 *
 * Description  : This procedure will enable the menu pulldown option.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : idOption - Id for the selected menu pulldown option.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID  EnableTheOption( USHORT idOption )
{
   USHORT   usAttrib;

   /*
    * Query the selected menu pulldown to get their current state...
    */
   usAttrib = (USHORT) WinSendMsg( hwndMenu,
                                   MM_QUERYITEMATTR,
                                   MPFROM2SHORT(idOption, TRUE),
                                   MPFROMSHORT(MIA_DISABLED));
   if (usAttrib & MIA_DISABLED)
   {
      /*
       * Selected option was in disable state, set the attribute and
       * enable it.
       */
      usAttrib ^= MIA_DISABLED;
      WinSendMsg( hwndMenu,
                  MM_SETITEMATTR,
                  MPFROM2SHORT( idOption, TRUE ),
                  MPFROM2SHORT( MIA_DISABLED, usAttrib));
   }
   return;
}


/******************************************************************************
 *  Name          : SetTheOptions
 *
 *  Description   : If the user selected an existing file, this procedure
 *                  will do the following:
 *                    -  get the file header information.
 *                    -  sets checkmarks on the appropriate type menu options.
 *                    -  disable the other type menu options.
 *
 *                  If the user selected a new file, this procedure will do
 *                  the following:
 *                    -  Enable the disabled type menu options.
 *                    -  Deselect the selected type menu options.
 *                    -  Select the default type menu options.
 *
 *  MMPM/2 API's : mmioOpen
 *                 mmioGetHeader
 *
 *  Parameters   : fSelectedFileType :  Selected file type - new/existing.
 *
 * Return        :  TRUE  -  if there is no error
 *                  FALSE -  if there is an error
 *
\******************************************************************************/
BOOL SetTheOptions( BOOL fSelectedFileType )
{
    ULONG         rc;                   /* for return code                    */
    HMMIO         hmmio;                /* Handle to the currently loaded file*/
    ULONG         lHeaderLength;        /* for header length                  */
    LONG          lBytes;               /* used for mmioGetheader api         */
    MMAUDIOHEADER mmAudioHeader;        /* WAVE header structure              */

    extern CHAR szFileName[FILE_NAME_SIZE];/* for fully qualified file name   */
    extern BOOL    fMonoSupported;                /* Dev supports mono files   */
    extern BOOL    fStereoSupported;              /* Dev supports stereo files */
    extern BOOL    fVoiceSupported;               /* Dev supports 11 kHz files */
    extern BOOL    fMusicSupported;               /* Dev supports 22 kHz files */
    extern BOOL    fHiFiSupported;                /* Dev supports 44 kHz files */
    extern BOOL    fHighQualitySupported;         /* Dev supports 16 bps files */
    extern BOOL    fLoQualitySupported;           /* Dev supports 8 bps files  */


    if (fSelectedFileType != NEW_FILE)
    {
       /*
        * Selected file is an EXISTING file. Open the file get the header
        * information.
        */
       hmmio = mmioOpen( szFileName, (PMMIOINFO)NULL, MMIO_READWRITE );
       if (!hmmio)
       {
          MessageBox( IDS_CANNOT_OPEN_INPUT_FILE,   /* ID of the message          */
                      MB_OK | MB_INFORMATION  | MB_MOVEABLE);    /* style         */
          return( FALSE );
       }

       /*
        * Get the header information from the selected file.
        */
       lHeaderLength = sizeof(MMAUDIOHEADER);
       rc = mmioGetHeader( hmmio,
                           (PVOID)&mmAudioHeader,
                           lHeaderLength,
                           (PLONG)&lBytes,
                           (ULONG) NULL,
                           (ULONG) NULL );
       if (rc)
       {
          MessageBox( IDS_MMIO_GET_HEADER_FAIL,     /* ID of the message          */
                      MB_OK | MB_INFORMATION  | MB_MOVEABLE);    /* style         */
          return( FALSE );
       }
    }

    /*
     * Before setting the correct selctions, enable the disbled items
     * in the menu pulldown and deselect the selected items.
     */
    if (fMonoSupported)           EnableTheOption( IDM_MONO );
    if (fStereoSupported)         EnableTheOption( IDM_STEREO );
    if (fVoiceSupported)          EnableTheOption( IDM_VOICE );
    if (fMusicSupported)          EnableTheOption( IDM_MUSIC );
    if (fHiFiSupported)           EnableTheOption( IDM_HIGH_FIDELITY );
    if (fLoQualitySupported)      EnableTheOption( IDM_LOW_QUALITY );
    if (fHighQualitySupported)    EnableTheOption( IDM_HIGH_QUALITY );
    DeselectTheOption( IDM_MONO );
    DeselectTheOption( IDM_VOICE );
    DeselectTheOption( IDM_MUSIC );
    DeselectTheOption( IDM_HIGH_FIDELITY );
    DeselectTheOption( IDM_LOW_QUALITY );
    DeselectTheOption( IDM_STEREO );
    DeselectTheOption( IDM_HIGH_QUALITY );

    if (fSelectedFileType != NEW_FILE)
    {
       /*
        * Selected file is an EXISTING file. Check the header information
        * and check the appropriate item.
        */
       if ( mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels == 1 )
       {
          /*
           * Selected file is using only channel. Select the "Mono" menu option.
           * Disable the "Stereo" menu option.
           */
          SelectTheOption( IDM_MONO );
          DisableTheOption( hwndMenu, IDM_STEREO );
       }
       else
       if ( mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels == 2 )
       {
          /*
           * Selected file is using both the channel. Select the "Stereo" menu
           * option. Disable the "Mono" menu option.
           */
          SelectTheOption( IDM_STEREO );
          DisableTheOption( hwndMenu, IDM_MONO );
       }

       if (mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec == 11025)
       {
          /*
           * Selected file sampling rate is 11.025 kHz. Select "Voice (11 khz)"
           * menu option. Disable the "Music (22 khz)" and
           * "High fidelity (44 khz)" menu options.
           */
          SelectTheOption( IDM_VOICE );
          DisableTheOption( hwndMenu, IDM_MUSIC );
          DisableTheOption( hwndMenu, IDM_HIGH_FIDELITY );
       }
       else
       if (mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec == 22050)
       {
          /*
           * Selected file sampling rate is 22.050 kHz. Select "Music (22 khz)"
           * menu option. Disable the "Voice (11 khz)" and
           * "High fidelity (44 khz)" menu options.
           */
          SelectTheOption( IDM_MUSIC );
          DisableTheOption( hwndMenu, IDM_VOICE );
          DisableTheOption( hwndMenu, IDM_HIGH_FIDELITY );
       }
       else
       if (mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec == 44100)
       {
          /*
           * Selected file sampling rate is 44.100 kHz. Select "High fidelity
           * 44 khz" menu option. Disable the "Voice (11 khz)" and
           * "Music (22 khz)" menu options.
           */
          SelectTheOption( IDM_HIGH_FIDELITY );
          DisableTheOption( hwndMenu, IDM_VOICE );
          DisableTheOption( hwndMenu, IDM_MUSIC );
       }

       if (mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample == 8)
       {
          /*
           * Selected file bits per sample is 8. Select "Low quality (8-bit)"
           * menu option. Disable the "High quality (16-bit)" menu option.
           */
          SelectTheOption( IDM_LOW_QUALITY );
          DisableTheOption( hwndMenu, IDM_HIGH_QUALITY );
       }
       else
       if (mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample == 16)
       {
          /*
           * Selected file bits per sample is 16. Select "High quality (16-bit)"
           * menu option. Disable the "Low quality (8-bit)" menu option.
           */
          SelectTheOption( IDM_HIGH_QUALITY );
          DisableTheOption( hwndMenu, IDM_LOW_QUALITY );
       }
    }
    else
    {
       /*
        * Selected file is a NEW file. Select all the default menu options.
        */
       SelectTheOption( IDM_STEREO );
       SelectTheOption( IDM_MUSIC );
       SelectTheOption( IDM_LOW_QUALITY );
    }
    mmioClose( hmmio, 0 );
    return( TRUE );

}   /* end GetHeader() */

/******************************************************************************
 * Name         :  AdjustTheDlg
 *
 * Description  :  This fuction adjusts the main dialog window whenever the
 *                 status line is added or deleted.
 *
 * Concepts     :
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  fShow - flag for selection/deselection of the Status line.
 *
 * Return       :  none
 *
 ******************************************************************************/
VOID AdjustTheDlg( HWND hwnd, BOOL fShow )
{
   SWP    swpFrame;                /* frame window positioning.               */
   SWP    swpDefault;              /* default window positioning.             */
   SWP    swpDialog;               /* dialog window positioning.              */
   SWP    swpStatus;               /* status line window positioning.         */
   static ULONG ulcyDialog = 0 ;   /* height of the dialog window.            */
   BOOL   fAtDefaultSize = FALSE ; /* flag to determine the size of the window*/


   /*
    * Get the current frame window and the default frame window sizes.
    */
   WinQueryWindowPos( hwndFrame, &swpFrame ) ;
   WinQueryDefaultSize( hwndFrame, &swpDefault ) ;

   /*
    * Set the flag to determine whether we were at the default size.
    */
   if (swpFrame.cx==swpDefault.cx && swpFrame.cy==swpDefault.cy)
   {
      fAtDefaultSize = TRUE ;
   }

   /*
    * Query the window size and position of the Dialog window.
    */
   WinQueryWindowPos( hwnd, &swpDialog ) ;

   /*
    * Save the current height of the Dialog window for further use.
    */
   if (!ulcyDialog)
   {
      ulcyDialog = swpDialog.cy ;
   }

   /*
    * Check to see the user selecting or deselecting the "Show status line"
    * option.
    */
   if (fShow)
   {
      /*
       * Status line is desired, raise dialog window height to ensure status
       * is visible
       */
      swpDialog.cy = ulcyDialog ;
   }
   else
   {
      /*
       * Status line is not desired, query the size and position of the
       * "status text line" and lower dialog height to hide status line.
       */
      WinQueryWindowPos(
         WinWindowFromID(
            hwnd,                                 /* Dialog window handle     */
            ID_STATUSTXT),                        /* Status text ID           */
         &swpStatus );                            /* Return status text data  */
      swpDialog.cy = swpStatus.y ;
   }
   /*
    * Set the positioning of the Dialog window.
    */
   WinSetWindowPos(
         hwnd,                           /* Dialog window handle              */
         0L,                             /* Place hwnd on top of all siblings */
         0L,                             /* Window position, x coordinate     */
         0L,                             /* Window position, y coordinate     */
         swpDialog.cx,                   /* New window size                   */
         swpDialog.cy,                   /* New window size                   */
         SWP_SIZE ) ;                    /* Window positioning options        */

   if (fAtDefaultSize)
   {
      /*
       * Current window is at default size. Repaint the window with new
       * window size.
       */
      WinDefaultSize(hwnd) ;
   }
   else
   {
      /*
       * Current window is NOT at default size. Just update the window to
       * hide the status line.
       */
      WinUpdateWindow(hwnd);
   }
}  /* End AdjustTheDlg() */

/******************************************************************************
 * Name         :  UpdateTheStatusLine
 *
 * Description  :  This function update the status line text.
 *
 * Concepts     :  none
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  hwnd     -  Handle for the dialog window.
 *                 usStatus -  Status line text to display.
 *
 * Return       :  none
 *
 ******************************************************************************/
VOID UpdateTheStatusLine( HWND hwnd, USHORT usStatus )
{
   CHAR szStatus[50];                   /* to store string from the resource. */
   CHAR szTemp[50];                     /* to store string from the resource. */
   extern BOOL fPassedDevice;           /* for MCI_ACQUIRE to play the file.  */

   /*
    * Get the appropriate string from the Resource string table.
    */
   WinLoadString( hab, (HMODULE)NULL, usStatus, sizeof(szStatus), szStatus );

   /*
    * If lost the device, inform the user by appending "Suspended" to the
    * current string.
    */
   if ( fPassedDevice )
   {
      /*
       * Get the "Suspended" string from the Resource string table.
       */
      WinLoadString( hab, (HMODULE)NULL, IDS_PASSED, sizeof(szTemp), szTemp );

      strcat( szStatus, ", ");              /* append ", " to the status      */
      strcat( szStatus, szTemp );           /* Append "Suspended" at the end. */
   }
   /*
    * Update the status line.
    */
   WinSetWindowText (WinWindowFromID (hwnd, ID_STATUSTXT), szStatus);

}  /* End if status line should be shown */
