#define INCL_DOS                       /* DosCall Definitions include     */
#define INCL_PM                        /* Presentation Manager include    */
#define INCL_WIN                       /* OS/2 2.x Win  include           */
#define INCL_OS2MM                     /* OS/2 2.x Win  include           */
#include <os2.h>                       /* OS/2 2.x System include         */
#include <string.h>                    /* C String Functions include      */
#include <stdio.h>                     /* C String Functions include      */
#include <stdlib.h>                    /* C String Functions include      */
#include <os2me.h>                     /* Multimedia System include       */
#include "mcispy.h"                    /* Component Private Definitions   */

extern   MCISPY_BLOCK      SpyBlock;
extern   MCIMessageTable   MCIMsgTbl[];   /* MCI Message Table       */

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   ViewInstDevDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN:     None
*
* DESCRIPTION: Command Monitor Window  Procedure
*
*****************************END OF SPECIFICATIONS ***************************/
MRESULT EXPENTRY ViewInstDevDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  SZ    szFontName[50];         /* Font Name Array   */
  SHORT sDataIndex;              /* Data Item Index   */
  SHORT k   = 0;                 /* Loop Indicies     */
  char  buffer[2048];            /* Temporary Storage */
  char space[] = "               ";/* Text Allignment */
  char tokensep[]=" \t";         /* Token Separotor   */

  switch(msg)
    {
    case WM_CONTROL:

      if ( SHORT2FROMMP ( mp1 ) ==  LN_SELECT )
       {
           WinSendDlgItemMsg(hwnd,LI_DEVNAMES, LM_SELECTITEM,MPFROMSHORT(LIT_NONE),0L);
       }
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:
          WinDismissDlg (hwnd,PB_OK);
        break;

        case DID_CANCEL:
          /* Dismiss the dialog box */
          WinDismissDlg (hwnd,DID_CANCEL);
        break;

        case PB_HELP:
          if ( SpyBlock.hwndHelpInstance )
              WinSendMsg (SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP,
                      MPFROM2SHORT(INSTDEVHLP, NULL),
                      MPFROMSHORT(HM_RESOURCEID));
        break;
        }
        break;
    case WM_INITDLG:
      /* Max no. of chars for Device Names list is 512 */
      WinSendDlgItemMsg(hwnd,LI_DEVNAMES, EM_SETTEXTLIMIT,MPFROMSHORT(16382),0L);
      WinSendDlgItemMsg(hwnd,LI_DEVNAMES, EM_SETREADONLY,MPFROMSHORT(TRUE),0L);

      strcpy (szFontName,"10.System Monospaced");
      WinSetPresParam (hwnd, PP_FONTNAMESIZE,(ULONG)(strlen(szFontName)+(ULONG)1),
                       (PSZ) szFontName);


      /* Query Installed devices and initialize */

      for (k = 0; k < SpyBlock.usNumDevices ; k++ )
        {
         if (SpyBlock.PhysicalDevData[k].szLogDevName == NULL)
            break;
         strcpy (buffer,SpyBlock.PhysicalDevData[k].szLogDevName);

         strncat (buffer, space,
                  20-strlen(SpyBlock.PhysicalDevData[k].szLogDevName));

         strcat (buffer,SpyBlock.PhysicalDevData[k].szPhysicalDevName);

         strncat (buffer, space,
                  15-strlen(SpyBlock.PhysicalDevData[k].szPhysicalDevName));

         strcat (buffer,SpyBlock.PhysicalDevData[k].szProductInfo);

         InsertListItem (LI_DEVNAMES,LIT_SORTASCENDING,buffer);
        }
      break;

    case WM_PAINT:
        /* Execute default processing */
        return(WinDefDlgProc(hwnd,msg,mp1,mp2));
        break;

    default:
        return(WinDefDlgProc(hwnd,msg,mp1,mp2));
    }
    return(FALSE);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   FilterDevTypDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN:     None
*
* DESCRIPTION: Message Filters Dialog  Procedure
*****************************END OF SPECIFICATIONS ***************************/
MRESULT EXPENTRY FilterDevTypDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  SZ      szFontName[124];         /* Font Name array   */
  static SHORT   sDataIndex = 0;   /* Data Item Index   */
  RC      ReturnCode;              /* API Return Code   */
  PCHAR   pchWord;                 /* Temporary pointer */
  USHORT  i = 0;                   /* Loop Index        */
  SZ      buffer [1024];           /* Char buffer       */
  SZ      TokenSep[]=" \t";        /* Token Delimiters  */
  SZ      TempBuffer[124];         /* Char buffer       */
  BOOL    fAlias = FALSE;          /* Alias or DeviceID */
  static  SHORT   sDataIndicies[20];/* Previous Filters */
  static  SHORT   sCount;           /* Previous Filters */

  MCIDevTypTable   MCIDevTbl[]={
  MCI_DEVTYPE_VIDEOTAPE,            "Videotape",
  MCI_DEVTYPE_VIDEODISC,            "Videodisc",
  MCI_DEVTYPE_CD_AUDIO,             "CDaudio",
  MCI_DEVTYPE_DAT,                  "DAT",
  MCI_DEVTYPE_AUDIO_TAPE,           "Audiotape",
  MCI_DEVTYPE_OTHER,                "Other",
  MCI_DEVTYPE_WAVEFORM_AUDIO,       "Waveaudio",
  MCI_DEVTYPE_SEQUENCER,            "Sequencer",
  MCI_DEVTYPE_AUDIO_AMPMIX,         "Ampmix",
  MCI_DEVTYPE_OVERLAY,              "Overlay",
  MCI_DEVTYPE_ANIMATION,            "Animation",
  MCI_DEVTYPE_DIGITAL_VIDEO,        "Digitalvideo",
  MCI_DEVTYPE_SPEAKER,              "Speaker",
  MCI_DEVTYPE_HEADPHONE,            "Headphone",
  MCI_DEVTYPE_MICROPHONE,           "Microphone",
  MCI_DEVTYPE_MONITOR,              "Monitor",
  MCI_DEVTYPE_CDXA,                 "CDXA"
  };

  switch(msg)
    {
    case WM_CONTROL:
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:
          sDataIndex = 0;
          SpyBlock.fDevTypFil = FALSE;
          SpyBlock.usNumFilDevs = 0;
          sCount = 0;
          /* Reset existing filters and refresh the list */
          memset(&SpyBlock.ulDevTypFil,0,sizeof(SpyBlock.ulDevTypFil));
          memset(&SpyBlock.szDevTypFil,0,sizeof(SpyBlock.szDevTypFil));
          memset(&sDataIndicies,0,sizeof(sDataIndicies));

          sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_FILTERDEVS,
                                        LM_QUERYSELECTION,(MPARAM)LIT_FIRST,
                                        (MPARAM) 0L ) );
          sDataIndicies[0] = sDataIndex;
          sCount++;
          SpyBlock.usNumFilDevs++;
          if (sDataIndex != LIT_NONE)
           {
              WinSendDlgItemMsg( hwnd, LI_FILTERDEVS, LM_QUERYITEMTEXT,
                                 MPFROM2SHORT(sDataIndex,80), TempBuffer );

              pchWord = strtok (TempBuffer,TokenSep);
              if (pchWord == NULL)
                 break;

              SpyBlock.ulDevTypFil[0]= atol(pchWord);

              /* Get The String Device Type */

              pchWord = strtok (NULL,TokenSep);

              strcpy (SpyBlock.szDevTypFil[0],pchWord);

              SpyBlock.fDevTypFil = TRUE;
           } /* No Slection */

           while (sDataIndex != LIT_NONE)
             {
               sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_FILTERDEVS,
                                             LM_QUERYSELECTION,(MPARAM)sDataIndex,
                                             (MPARAM) 0L ) );

               if (sDataIndex == LIT_NONE)
               break;

               sDataIndicies[sCount] = sDataIndex;
               WinSendDlgItemMsg( hwnd, LI_FILTERDEVS, LM_QUERYITEMTEXT,
                                  MPFROM2SHORT(sDataIndex,80), TempBuffer );

               pchWord = strtok (TempBuffer,TokenSep);

               SpyBlock.ulDevTypFil[sCount]= atol(pchWord);

              /* Get The String Device Type */

              pchWord = strtok (NULL,TokenSep);

              strcpy (SpyBlock.szDevTypFil[sCount],pchWord);
              i++;
              sCount++;
              SpyBlock.usNumFilDevs++;
              if (i > MAXDEVICES)
                 break;
             } /* While */
          WinDismissDlg (hwnd,PB_OK);
          break;

          case DID_CANCEL:
            /* Dismiss the dialog box */
            WinDismissDlg (hwnd,DID_CANCEL);
          break;

        case PB_HELP:
          if ( SpyBlock.hwndHelpInstance )
              WinSendMsg (SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP,
                      MPFROM2SHORT(FILTERDEVTYPHLP, NULL),
                      MPFROMSHORT(HM_RESOURCEID));
          break;
        }
      break;
    case WM_INITDLG:
      /* Max no. of chars for Device Names list is 512 */
      WinSendDlgItemMsg(hwnd,LI_FILTERDEVS,
                        EM_SETTEXTLIMIT,MPFROMSHORT(4096),0L);

      strcpy (szFontName,"10.System Monospaced");

      WinSetPresParam (hwnd,PP_FONTNAMESIZE,
                       (ULONG)((ULONG) strlen(szFontName)+(ULONG)1),
                       (PSZ) szFontName);

      for (i=0; i < 17; i++)
        {
           strcpy (buffer,(char *)LongToString(MCIDevTbl[i].usDevType));
           if (i <= 9)
              strcat (buffer,"         " );
           else
              strcat (buffer,"        ");
           strcat (buffer,MCIDevTbl[i].szDevType);
           InsertListItem (LI_FILTERDEVS,LIT_END,buffer);
        }

      if (SpyBlock.fDevTypFil)
       {
          for (i=0; i < sCount; i++)
            {
               WinSendDlgItemMsg(hwnd,LI_FILTERDEVS,LM_SELECTITEM,
                                 MPFROMSHORT(sDataIndicies[i]),MPFROMSHORT(TRUE));
            }
       }

      break;

    case WM_PAINT:
        /* Execute default processing */

        return(WinDefDlgProc(hwnd,msg,mp1,mp2));

        break;
    default:
        return(WinDefDlgProc(hwnd,msg,mp1,mp2));
  }
  return(FALSE);
}
/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   FilterDevIdDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN:     None
*
* DESCRIPTION: Message Filters Dialog  Procedure
*
*****************************END OF SPECIFICATIONS ***************************/

MRESULT EXPENTRY FilterDevIdDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  SZ      szFontName[124];         /* Font name Array   */
  SHORT   sDataIndex;              /* Data Item Index   */
  APIRET  rc;                      /* API Return Code   */
  PCHAR   pchWord;                 /* Temporary pointer */
  USHORT  i;                       /* Loop Index        */
  SZ      buffer[1024];            /* Temporary buffer  */
  SZ      TokenSep[]=" \t";        /* Token Delimiters  */
  SZ      TempBuffer[124];         /* Temporary buffer  */
  BOOL   fAlias = FALSE;           /* Alias or Device   */
  ULONG  ulTempId;                 /* Temp Device Id    */
  ULONG  ulPID;                    /* Temp Process Id   */
  ULONG  ulCount;                  /* Selection Count   */

  switch(msg)
    {
    case WM_CONTROL:
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:
          ulCount = 0L;
          sDataIndex = 0;

          /* Reset existing filters and refresh the list */

          for (i= 0; i < MAXOPENS; i++)
            {
               SpyBlock.OpenList.ulOpenList[i].fExclude = FALSE;
            }
          for (i= 0; i < MAXOPENS; i++)
            {
               SpyBlock.OpenList.szAlias[i].fExclude = FALSE;
            }

          sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_OPENINST,
                                       LM_QUERYSELECTION,(MPARAM)LIT_FIRST,
                                       (MPARAM) 0L ) );

          if (sDataIndex != LIT_NONE)
           {
              WinSendDlgItemMsg( hwnd, LI_OPENINST, LM_QUERYITEMTEXT,
                                 MPFROM2SHORT(sDataIndex,80), TempBuffer );

              /* This Will be the PID  */

              pchWord = strtok (TempBuffer,TokenSep);

              ulPID = atoi(pchWord);
              /* Second Token is the Device ID or Alias */

              pchWord = strtok (NULL,TokenSep);

              for (i=0; i < MAXOPENS; i++)
                {
                   if (SpyBlock.OpenList.szAlias[i].ulPID == ulPID)
                    {
                       if (stricmp (SpyBlock.OpenList.szAlias[i].szAlias,pchWord) == 0)
                        {
                          SpyBlock.OpenList.szAlias[i].fExclude = TRUE;
                          fAlias = TRUE;
                          SpyBlock.fDevIdFil = TRUE;
                          break;
                        } /* Found Alias */
                    } /* Found PID */
                } /* Max Opens */
              if (!fAlias)
               {
                  ulTempId = atoi(pchWord);
                  for (i=0; i < MAXOPENS; i++)
                    {
                       if (SpyBlock.OpenList.ulOpenList[i].ulDevId == ulTempId)
                        {
                          SpyBlock.OpenList.ulOpenList[i].fExclude = TRUE;
                          SpyBlock.fDevIdFil = TRUE;
                          break;
                        }
                    }
               } /* Not an Alias */

              while (sDataIndex != LIT_NONE)
                {
                  fAlias = FALSE;
                  ulCount++;
                  sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,
                                               LI_OPENINST, LM_QUERYSELECTION,
                                               (MPARAM)sDataIndex,(MPARAM) 0L ) );



                  if (sDataIndex != LIT_NONE)
                   {
                      WinSendDlgItemMsg( hwnd, LI_OPENINST, LM_QUERYITEMTEXT,
                                         MPFROM2SHORT(sDataIndex,80), TempBuffer );

                      pchWord = strtok (TempBuffer,TokenSep);

                      ulPID = atoi(pchWord);

                      /* Second Token is the Device ID or Alias */
                      pchWord = strtok (NULL,TokenSep);

                      for (i=0; i < MAXOPENS; i++)
                        {
                           if (SpyBlock.OpenList.szAlias[i].ulPID == ulPID)
                            {
                               if (strcmp (SpyBlock.OpenList.szAlias[i].szAlias,pchWord) == 0)
                                {
                                  SpyBlock.OpenList.szAlias[i].fExclude = TRUE;
                                  fAlias = TRUE;
                                  break;
                                } /* Found Alias */
                            } /* Found PID */
                        } /* Max Devices */
                      if (!fAlias)
                       {
                          ulTempId = atoi(pchWord);
                          for (i=0; i < MAXOPENS; i++)
                            {
                               if (SpyBlock.OpenList.ulOpenList[i].ulDevId == ulTempId)
                                {
                                  SpyBlock.OpenList.ulOpenList[i].fExclude = TRUE;
                                  break;
                                }
                            }
                       } /* Not an Alias */

                   } /* LIT_NONE */
                } /* While */
           } /* No Selection */
          WinDismissDlg (hwnd,PB_OK);
         break;

        case DID_CANCEL:
          /* Dismiss the dialog box */
          WinDismissDlg (hwnd,DID_CANCEL);
         break;

        case PB_HELP:
          if ( SpyBlock.hwndHelpInstance )
              WinSendMsg (SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP,
                      MPFROM2SHORT(FILTERDEVIDHLP, NULL),
                      MPFROMSHORT(HM_RESOURCEID));
         break;
        }
        break;

    case WM_INITDLG:
        /* Max no. of chars for Device Names list is 512 */
        strcpy (szFontName,"10.System Monospaced");
        WinSetPresParam (hwnd,
                         PP_FONTNAMESIZE,
                         (ULONG)((ULONG) strlen(szFontName)+(ULONG)1),
                         (PSZ) szFontName);

        /* Begin Insertion of Open Device IDs into the list */

        for (i= 0; i < MAXOPENS; i++)
          {
             if (SpyBlock.OpenList.ulOpenList[i].ulDevId != 0)
              {
                 strcpy (buffer,
                         (char *)LongToString(SpyBlock.OpenList.ulOpenList[i].ulPID));
                 strcat (buffer,"     ");
                 strcat (buffer,
                         (char *)LongToString(SpyBlock.OpenList.ulOpenList[i].ulDevId));
                 InsertListItem (LI_OPENINST,LIT_END,buffer);
              }
          }
        /* Begin Insertion of Open Device Alias(es) into the list box */

        for (i= 0; i < MAXOPENS; i++)
          {
             if (strlen(SpyBlock.OpenList.szAlias[i].szAlias) != 0)
              {
                 strcpy (buffer,
                         (char *)LongToString(SpyBlock.OpenList.szAlias[i].ulPID));
                 strcat (buffer,"     ");
                 strcat (buffer,SpyBlock.OpenList.szAlias[i].szAlias);
                 InsertListItem (LI_OPENINST,LIT_END,buffer);
              }
          }

        /* Identify The current Device Id Filters If any and Select them */

        for (i= 0; i < MAXOPENS; i++)
          {
             if (SpyBlock.OpenList.ulOpenList[i].fExclude)
                 WinSendDlgItemMsg(hwnd,LI_OPENINST,LM_SELECTITEM,
                                   MPFROMSHORT(i),MPFROMSHORT(TRUE));
          }

        /* Identify The current Device Alias(es)
        **  Filters If any and Select them .
        */

        for (i= 0; i < MAXOPENS; i++)
          {
             if (SpyBlock.OpenList.szAlias[i].fExclude)
                 WinSendDlgItemMsg(hwnd,LI_OPENINST,LM_SELECTITEM,
                                   MPFROMSHORT(i),MPFROMSHORT(TRUE));
          }

        break;

    case WM_PAINT:

          /* Execute default processing */
         return(WinDefDlgProc(hwnd,msg,mp1,mp2));
       break;

    default:
      return(WinDefDlgProc(hwnd,msg,mp1,mp2));
  }
    return(FALSE);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   AboutDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN: None
*
* DESCRIPTION: This routine brings up the dialog about.
*
*****************************END OF SPECIFICATIONS ***************************/

MRESULT EXPENTRY AboutDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_CONTROL:
      if ( SHORT2FROMMP ( mp1 ) ==  LN_ENTER )
            WinSendMsg ( hwnd, WM_COMMAND, MPFROMSHORT ( PB_OK ),
                      MPFROM2SHORT ( CMDSRC_PUSHBUTTON, TRUE ) );
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:
          WinDismissDlg (hwnd,PB_OK);
         break;
        }
        break;

    case WM_PAINT:
        /* Execute default processing */
         return(WinDefDlgProc(hwnd,msg,mp1,mp2));
        break;
    default:
        return(WinDefDlgProc(hwnd,msg,mp1,mp2));
    }
    return(FALSE);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   FilterDevTypDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN:     None
*
* DESCRIPTION: Message Filters Dialog  Procedure
*
* OS/2 API's USED: None
*
*****************************END OF SPECIFICATIONS ***************************/
MRESULT EXPENTRY ShowFlagDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  typedef struct _flagTable{
    USHORT    wMsg;
    SZ      szFlag[100];
    ULONG   dwFlag;
  }MCIFlagTbl;

  SHORT  sDataIndex;              /* Data Item Index   */
  RC     ReturnCode;              /* API Return Code   */
  static ULONG  dwFlag = 0;       /* Local Flag        */
  USHORT i,j;                     /* Loop Indices      */
  USHORT   wMsg;                  /* Local MCI Msg     */
  SZ     szTemp[10];              /* Temporary buffer  */
  SZ     szFontName[70];          /* Font Name Array   */
  SZ     TempBuffer[80];          /* Temporary buffer  */
  SZ     TokenSep[]=" \t";        /* Token Seperator   */
  PCHAR  pchWord;                 /* Temporary buffer  */
  PCHAR  stop;                    /* Temporary buffer  */
  BOOL   fCommon = FALSE;         /* Found Common Flag */
  BOOL   fMsg    = FALSE;         /* Found Msg         */

  MCIFlagTbl MCIFlagsTbl[]={
  0,  "MCI_NOTIFY",                                         0x00000001L,
  0,  "MCI_WAIT",                                           0x00000002L,
  0,  "MCI_FROM",                                           0x00000004L,
  0,  "MCI_TO",                                             0x00000008L,
  0,  "MCI_MILLISECONDS",                                   0x00000010L,
  0,  "MCI_TRACK",                                          0x00000020L,
  0,  "MCI_OVER",                                           0x00000040L,
  0,  "MCI_TEST",                                           0x00000080L,
  0,  "MCI_TO_BUFFER",                                      0x00000100L,
  0,  "MCI_FROM_BUFFER",                                    0x00000200L,
  0,  "MCI_CONVERT_FORMAT",                                 0x00000400L,
  MCI_ACQUIREDEVICE, "MCI_EXCLUSIVE",                       0x00000100L,
  MCI_ACQUIREDEVICE, "MCI_EXCLUSIVE_INSTANCE",              0x00000200L,
  MCI_ACQUIREDEVICE, "MCI_ACQUIRE_QUEUE",                   0x00000400L,
  MCI_CAPTURE,       "MCI_CAPTURE_RECT",                    0x00000100L,
  MCI_CAPTURE,        "MCI_CONVERT",                        0x00001000L,
  MCI_CONNECTION,  "MCI_ENABLE_CONNECTOR",                  0x00000100L,
  MCI_CONNECTION,  "MCI_DISABLE_CONNECTOR",                 0x00000200L,
  MCI_CONNECTION,  "MCI_QUERY_CONNECTOR_STATUS",            0x00000400L,
  MCI_CONNECTION,  "MCI_CONNECTOR_TYPE",                    0x00000800L,
  MCI_CONNECTION,  "MCI_CONNECTOR_INDEX",                   0x00001000L,
  MCI_CONNECTION,  "MCI_TO_CONNECTOR_INDEX",                0x00002000L,
  MCI_CONNECTION,  "MCI_TO_CONNECTOR_TYPE",                 0x00004000L,
  MCI_CONNECTION,  "MCI_QUERY_CONNECTION",                   0x00008000L,
  MCI_CONNECTION,  "MCI_MAKE_CONNECTION",                    0x00010000L,
  MCI_CONNECTION,  "MCI_BREAK_CONNECTION",                   0x00020000L,
  MCI_CONNECTION,  "MCI_ENUMERATE_CONNECTORS",               0x00040000L,
  MCI_CONNECTION,  "MCI_QUERY_CONNECTOR_TYPE",               0x00080000L,
  MCI_CONNECTION,  "MCI_QUERY_VALID_CONNECTION",             0x00100000L,
  MCI_CONNECTION,  "MCI_CONNECTOR_ALIAS",                    0x00200000L,
  MCI_CUE,         "MCI_CUE_INPUT",                         0x00000100L,
  MCI_CUE,         "MCI_CUE_OUTPUT",                        0x00000200L,
  MCI_CUE,         "MCI_WAVE_INPUT",                        0x00001000L,
  MCI_CUE,         "MCI_WAVE_OUTPUT",                       0x00002000L,
  MCI_ESCAPE,      "MCI_ESCAPE_STRING",                     0x00000100L,
  MCI_FREEZE,      "MCI_OVLY_FREEZE_RECT",                  0x00000100L,
  MCI_FREEZE,      "MCI_OVLY_FREEZE_RECT_OUTSIDE",          0x00000200L,
  MCI_GETDEVCAPS,  "MCI_GETDEVCAPS_MESSAGE",                0x00000100L,
  MCI_GETDEVCAPS,  "MCI_GETDEVCAPS_ITEM",                   0x00000200L,
  MCI_GETDEVCAPS,  "MCI_GETDEVCAPS_EXTENDED",               0x00000400L,
  MCI_MASTERAUDIO, "MCI_ON",                                0x00000100L,
  MCI_MASTERAUDIO, "MCI_OFF",                               0x00000200L,
  MCI_MASTERAUDIO, "MCI_HEADPHONES",                        0x00000400L,
  MCI_MASTERAUDIO, "MCI_SPEAKERS",                          0x00000800L,
  MCI_MASTERAUDIO, "MCI_MASTERVOL",                         0x00001000L,
  MCI_MASTERAUDIO, "MCI_SAVESETTING",                       0x00002000L,
  MCI_MASTERAUDIO, "MCI_QUERYSAVEDSETTING",                 0x00004000L,
  MCI_MASTERAUDIO, "MCI_QUERYCURRENTSETTING",               0x00008000L,
  MCI_INFO,        "MCI_INFO_PRODUCT",                      0x00000100L,
  MCI_INFO,        "MCI_INFO_FILE",                         0x00000200L,
  MCI_OPEN,        "MCI_OPEN_ELEMENT",                      0x00000100L,
  MCI_OPEN,        "MCI_OPEN_ALIAS",                        0x00000200L,
  MCI_OPEN,        "MCI_OPEN_ELEMENT_ID",                   0x00000400L,
  MCI_OPEN,        "MCI_OPEN_PLAYLIST",                     0x00000800L,
  MCI_OPEN,        "MCI_OPEN_TYPE_ID",                      0x00001000L,
  MCI_OPEN,        "MCI_OPEN_SHAREABLE",                    0x00002000L,
  MCI_OPEN,        "MCI_OPEN_MMIO",                         0x00004000L,
  MCI_OPEN,        "MCI_READONLY",                          0x00008000L,
  MCI_OPEN,        "MCI_VID_OPEN_PARENT",                   0x01000000L,
  MCI_SYSINFO,      "MCI_SYSINFO_QUANTITY",                0x00000100L,
  MCI_SYSINFO,      "MCI_SYSINFO_OPEN",                    0x00000200L,
  MCI_SYSINFO,      "MCI_SYSINFO_NAME",                    0x00000400L,
  MCI_SYSINFO,      "MCI_SYSINFO_INSTALLNAME",             0x00000800L,
  MCI_SYSINFO,      "MCI_SYSINFO_ITEM",                    0x00001000L,
  MCI_SYSINFO,      "MCI_SYSINFO_INI_LOCK",                0x00002000L,
  MCI_PLAY,         "MCI_VD_PLAY_REVERSE",                 0x00001000L,
  MCI_PLAY,         "MCI_VD_PLAY_FAST",                    0x00002000L,
  MCI_PLAY,         "MCI_VD_PLAY_SPEED",                   0x00004000L,
  MCI_PLAY,         "MCI_VD_PLAY_SCAN",                    0x00008000L,
  MCI_PLAY,         "MCI_VD_PLAY_SLOW",                    0x00010000L,
  MCI_PLAY,         "MCI_FROM",                            0x00000004L,
  MCI_PLAY,         "MCI_TO",                              0x00000008L,
  MCI_RECORD,        "MCI_RECORD_INSERT",                   0x00000100L,
  MCI_RECORD,        "MCI_RECORD_OVERWRITE",                0x00000200L,
  MCI_SAVE,          "MCI_SAVE_FILE",                       0x00000100L,
  MCI_SAVE,          "MCI_DGV_SAVE_VIDEO_FILE",             0x00001000L,
  MCI_SAVE,          "MCI_DGV_SAVE_IMAGE_FILE",             0x00002000L,
  MCI_SEEK,          "MCI_TO_START",                        0x00000100L,
  MCI_SEEK,          "MCI_TO_END",                          0x00000200L,
  MCI_SEEK,          "MCI_DGV_SEEK_TO_NEAREST_IFRAME",      0x00100000L,
  MCI_SET,           "MCI_SET_ITEM",                        0x00000100L,
  MCI_SET,           "MCI_SET_ON",                          0x00000200L,
  MCI_SET,           "MCI_SET_OFF",                         0x00000400L,
  MCI_SET,           "MCI_SET_VIDEO",                       0x00000800L,
  MCI_SET,           "MCI_SET_AUDIO",                       0x00001000L,
  MCI_SET,           "MCI_SET_DOOR_OPEN",                   0x00002000L,
  MCI_SET,           "MCI_SET_DOOR_CLOSED",                 0x00004000L,
  MCI_SET,           "MCI_SET_SPEED_FORMAT",                0x00008000L,
  MCI_SET,           "MCI_SET_TIME_FORMAT",                 0x00010000L,
  MCI_SET,           "MCI_SET_DOOR_LOCK",                   0x00020000L,
  MCI_SET,           "MCI_SET_DOOR_UNLOCK",                 0x00040000L,
  MCI_SET,           "MCI_SET_VOLUME",                      0x00080000L,
  MCI_SET,           "MCI_SET_ITEM_FOURCC",                 0x00100000L,
  MCI_SET,           "MCI_SET_REVERSE",                     0x00200000L

  };


  switch(msg)
    {
    case WM_CONTROL:
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:

           if (!(BOOL)WinSendMsg( WinWindowFromID(hwnd,EF_FLAG),
                                  EM_QUERYCHANGED,(MPARAM)0L,
                                  (MPARAM) 0L ))
            {
               if (dwFlag == 0)
                {
                   mprintf ("No Flag Value entered \n");
                   break;
                }
            }

          WinQueryWindowText ( WinWindowFromID(hwnd,EF_FLAG),
                             9, szTemp );

          dwFlag = strtol (szTemp,&stop,16);

          if ( dwFlag > 0x01111111L )
           {
               mprintf ("Flag Value out of Range \n");
               break;
           }

          sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_MSGS,
                                        LM_QUERYSELECTION,(MPARAM)LIT_FIRST,
                                        (MPARAM) 0L ) );

          if (sDataIndex == LIT_NONE)
           {
             mprintf ("No MCI Message Selected\n");
             break;
           }

          WinSendDlgItemMsg( hwnd, LI_MSGS, LM_QUERYITEMTEXT,
                             MPFROM2SHORT(sDataIndex,80), TempBuffer );

          pchWord = strtok (TempBuffer,TokenSep);
          /*
          ** Find this message in our global message table and
          */
          for (i=0; i <MAX_MCI_MSGS; i++)
            {
              if (strcmp (MCIMsgTbl[i].szMessage,pchWord) == 0)
               {
                 wMsg = MCIMsgTbl[i].usMsg;
                 fMsg = TRUE;
                 break;
               }
            }
         if (!fMsg)
          {
            mprintf ("Could not Find Message %s\n",pchWord);
            break;
          }
         for (i= 0; i < 92; i++)
           {
             if (i < NUM_COMMON_FLAGS )
              {
                if (!fCommon)
                 {
                    if (dwFlag & MCIFlagsTbl[i].dwFlag)
                     {
                        InsertListItem (LI_FLAGS,LIT_SORTASCENDING,MCIFlagsTbl[i].szFlag);
                        fCommon = TRUE;
                     }
                 }
              }
             if (wMsg == MCIFlagsTbl[i].wMsg)
              {
                if (dwFlag & MCIFlagsTbl[i].dwFlag)
                 {
                    InsertListItem (LI_FLAGS,LIT_SORTASCENDING,MCIFlagsTbl[i].szFlag);
                    fCommon = TRUE;
                 }
              }
           }
           if (!fCommon)
              mprintf ("Unknown Flag (Modify Source Provided) %d\n",dwFlag);

          break;

          case DID_CANCEL:
              /* Dismiss the dialog box */
              WinDismissDlg (hwnd,DID_CANCEL);
          break;

        case PB_HELP:
          if ( SpyBlock.hwndHelpInstance )
              WinSendMsg (SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP,
                      MPFROM2SHORT(SHOWFLAGHLP, NULL),
                      MPFROMSHORT(HM_RESOURCEID));
         break;

        case PB_CLEAR:
             WinSendDlgItemMsg (hwnd,LI_FLAGS,LM_DELETEALL,(MPARAM)0,(MPARAM)0);
         break;
        }
      break;

      case WM_INITDLG:
           /* Max no. of chars for Device Names list is 512 */
           WinSendDlgItemMsg(hwnd,LI_MSGS,EM_SETTEXTLIMIT,
                             MPFROMSHORT(8192),0L);


           strcpy (szFontName,"10.System Monospaced");
           WinSetPresParam (hwnd,PP_FONTNAMESIZE,
                            (ULONG)((ULONG) strlen(szFontName)+(ULONG)1),
                            (PSZ) szFontName);

           /* Max no. of chars for Device Names list is 512 */

           WinSendDlgItemMsg(hwnd,LI_FLAGS,EM_SETTEXTLIMIT,
                             MPFROMSHORT(8192),0L);


           for (i=0; i < MAX_MCI_MSGS; i++)
             {
                for (j=0; j < 90 ; j++)
                  {
                    if (MCIFlagsTbl[j].wMsg == MCIMsgTbl[i].usMsg)
                     {
                         InsertListItem (LI_MSGS,
                                         LIT_SORTASCENDING,MCIMsgTbl[i].szMessage);
                         break;
                     }
                  }
             }

           break;

      case WM_PAINT:

          /* Execute default processing */
           return(WinDefDlgProc(hwnd,msg,mp1,mp2));

        break;

      default:
          return(WinDefDlgProc(hwnd,msg,mp1,mp2));
  }
    return(FALSE);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:   FilterMsgDlgProc
*
* ARGUMENTS:  HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
* RETURN:     None
*
* DESCRIPTION: Message Filters Dialog  Procedure
*
* OS/2 API's USED: None
*
* C FUNCTION CALLS: memset
*
*****************************END OF SPECIFICATIONS ***************************/

MRESULT EXPENTRY FilterMsgDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  SHORT  sDataIndex;              /* Data Item Index   */
  RC     ReturnCode;              /* API Return Code   */
  PCHAR  pchWord;                 /* Temporary pointer */
  SZ     szFontName[60];          /* FontName Array    */
  ULONG  ulCount;                 /* Items Selected    */
  USHORT usIndex,i,j = 0;         /* Loop Indices      */
  SZ     TempBuffer[124];         /* Item text buffer  */
  SZ     TokenSep[]=" \t";        /* Token Seperator   */

  switch(msg)
    {
    case WM_CONTROL:
     break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case PB_OK:
          ulCount = 0L;
          sDataIndex = 0;

          /* Reset All the previous filter flags */
          for (i=0; i < MAX_MCI_MSGS; i++)
            {
              MCIMsgTbl[i].fExclude = FALSE;
            }

          sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_FILTERMSG,
                                      LM_QUERYSELECTION,(MPARAM)LIT_FIRST,
                                      (MPARAM) 0L ) );

          if (sDataIndex != LIT_NONE)
           {
              WinSendDlgItemMsg( hwnd, LI_FILTERMSG, LM_QUERYITEMTEXT,
                                 MPFROM2SHORT(sDataIndex,80), TempBuffer );

              pchWord = strtok (TempBuffer,TokenSep);
              /*
              ** Find this message in our global message table and
              ** Toggle the filter status to "include". Note that we
              ** Don't care what the previous state was. We always
              ** update the status to reflect current action.
              */
              for (i=0; i <MAX_MCI_MSGS; i++)
                {
                 if (strcmp (MCIMsgTbl[i].szMessage,pchWord) == 0)
                  {
                     MCIMsgTbl[i].fExclude = TRUE;
                     break;
                  }
                }
              while (sDataIndex != LIT_NONE)
                {
                  ulCount++;
                  sDataIndex = SHORT1FROMMR( WinSendDlgItemMsg( hwnd,LI_FILTERMSG,
                                                LM_QUERYSELECTION,(MPARAM)sDataIndex,
                                                (MPARAM) 0L ) );
                  if (sDataIndex != LIT_NONE)
                   {
                      WinSendDlgItemMsg( hwnd, LI_FILTERMSG, LM_QUERYITEMTEXT,
                                         MPFROM2SHORT(sDataIndex,80), TempBuffer );

                      pchWord = strtok (TempBuffer,TokenSep);

                      /*
                      ** Find this message in our global message table and
                      ** Toggle the filter status to "include". Note that we
                      ** Don't care what the previous state was. We always
                      ** update the status to reflect current action.
                      */

                      for (i=0; i <MAX_MCI_MSGS; i++)
                        {
                         if (strcmp (MCIMsgTbl[i].szMessage,pchWord) == 0)
                          {
                             MCIMsgTbl[i].fExclude = TRUE;
                             break;
                          } /* Found it */
                        } /* For Loop */
                   } /* LIT_NONE */
                } /* While */
           } /* No Selection */
          WinDismissDlg (hwnd,PB_OK);
         break;

        case PB_ALL:

          for (i=0; i < MAX_MCI_MSGS; i++)
            {
               /* Select all items in the list  */

               WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                 MPFROMSHORT(i),MPFROMSHORT(TRUE));

              MCIMsgTbl[i].fExclude = TRUE;
            }
         break;

        case PB_NONE:

          /* DeSelect all items in the list */

          WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                            MPFROMSHORT(LIT_NONE),0L);

          for (i=0; i < MAX_MCI_MSGS; i++)
            {
              MCIMsgTbl[i].fExclude = FALSE;
            }

         break;

        case PB_TIMERON:

          for (i=0; i < MAX_MCI_MSGS; i++)
            {
               if (MCIMsgTbl[i].usMsg ==MM_MCIPOSITIONCHANGE )
                {
                   /* Select  item in the list  */

                   WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                     MPFROMSHORT(i),MPFROMSHORT(TRUE));
                  MCIMsgTbl[i].fExclude = TRUE;
                }
            }
         break;

        case PB_TIMEROFF:
          for (i=0; i < MAX_MCI_MSGS; i++)
            {
               if (MCIMsgTbl[i].usMsg == MM_MCIPOSITIONCHANGE)
                {
                   /* Select  item in the list  */

                   WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                     MPFROMSHORT(i),MPFROMSHORT(FALSE));

                  MCIMsgTbl[i].fExclude = FALSE;
                }
            }
         break;

        case PB_ACTON:
          for (i=0; i < MAX_MCI_MSGS; i++)
            {

               if ((MCIMsgTbl[i].usMsg == MCI_PLAY) ||
                  (MCIMsgTbl[i].usMsg == MCI_PAUSE) ||
                  (MCIMsgTbl[i].usMsg == MCI_RECORD) ||
                  (MCIMsgTbl[i].usMsg == MCI_RESUME) ||
                  (MCIMsgTbl[i].usMsg == MCI_SEEK) ||
                  (MCIMsgTbl[i].usMsg == MCI_STOP) ||
                  (MCIMsgTbl[i].usMsg == MCI_CUE) ||
                  (MCIMsgTbl[i].usMsg == MCI_STEP) ||
                  (MCIMsgTbl[i].usMsg == MCI_SPIN) ||
                  (MCIMsgTbl[i].usMsg == MCI_REWIND))

                {
                   /* Select  item in the list  */

                  WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                    MPFROMSHORT(i),MPFROMSHORT(TRUE));
                  MCIMsgTbl[i].fExclude = TRUE;
                }
            }
         break;

        case PB_ACTOFF:
          for (i=0; i < MAX_MCI_MSGS; i++)
            {
               if ((MCIMsgTbl[i].usMsg == MCI_PLAY) ||
                  (MCIMsgTbl[i].usMsg == MCI_PAUSE) ||
                  (MCIMsgTbl[i].usMsg == MCI_RECORD) ||
                  (MCIMsgTbl[i].usMsg == MCI_RESUME) ||
                  (MCIMsgTbl[i].usMsg == MCI_SEEK) ||
                  (MCIMsgTbl[i].usMsg == MCI_STOP) ||
                  (MCIMsgTbl[i].usMsg == MCI_CUE) ||
                  (MCIMsgTbl[i].usMsg == MCI_STEP) ||
                  (MCIMsgTbl[i].usMsg == MCI_SPIN) ||
                  (MCIMsgTbl[i].usMsg == MCI_REWIND))

                {
                   /* Select  item in the list  */

                  WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                    MPFROMSHORT(i),MPFROMSHORT(FALSE));
                  MCIMsgTbl[i].fExclude = FALSE;
                }
            }
         break;


        case DID_CANCEL:
          /* Dismiss the dialog box */
          WinDismissDlg (hwnd,DID_CANCEL);
          break;

        case PB_HELP:
         if ( SpyBlock.hwndHelpInstance )
             WinSendMsg (SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP,
                     MPFROM2SHORT(FILTERMSGHLP, NULL),
                     MPFROMSHORT(HM_RESOURCEID));
         break;
        }
     break;

    case WM_INITDLG:
      /* Max no. of chars for Device Names list is 512 */
      WinSendDlgItemMsg(hwnd,LI_FILTERMSG,EM_SETTEXTLIMIT,
                        MPFROMSHORT(16654),0L);


      strcpy (szFontName,"10.System Monospaced");
      WinSetPresParam (hwnd,PP_FONTNAMESIZE,
                       (ULONG)(strlen(szFontName)+(ULONG)1),
                       (PSZ) szFontName);

      for (i=0; i < MAX_MCI_MSGS; i++)
        {
           InsertListItem (LI_FILTERMSG,
                           LIT_SORTASCENDING,MCIMsgTbl[i].szMessage);
        }
      for (i=0; i < MAX_MCI_MSGS; i++)
        {
           if (MCIMsgTbl[i].fExclude)
               WinSendDlgItemMsg(hwnd,LI_FILTERMSG,LM_SELECTITEM,
                                 MPFROMSHORT(i),MPFROMSHORT(TRUE));
        }

     break;

    case WM_PAINT:

      /* Execute default processing */

      return(WinDefDlgProc(hwnd,msg,mp1,mp2));
     break;

    default:
        return(WinDefDlgProc(hwnd,msg,mp1,mp2));
  }
    return(FALSE);
}

