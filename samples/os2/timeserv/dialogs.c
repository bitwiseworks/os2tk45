/*static char *SCCSID = "@(#)dialogs.c  6.6 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE:dialogs.c                                                    *
 *                                                                         *
 *       DESCRIPTION:    Dialog procedures for PM clock                    *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1989, 1990, 1992, 1996 IBM Corp.                         *
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
 *
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *  ClkTicksDlgProc()
 *  AlarmDialogsProc()
 *  AboutBoxDialogProc()
 *  SetAlarmField()
 *  GetAlarmTime()
 *  AlarmVScrolls()
 *  EntryFieldSubProc()
 *  AlarmSetTimer()
 *  ClkColorsDlgProc()
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_WININPUT
#define INCL_WINFRAMEMGR
#define INCL_WINTRACKRECT
#define INCL_WINMENUS
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINMESSAGEMGR
#define INCL_WINDIALOGS
#define INCL_WINBUTTONS
#define INCL_WINENTRYFIELDS
#define INCL_WINWINDOWMGR
#define INCL_WINSCROLLBARS
#define INCL_GPITRANSFORMS
#define INCL_GPIPRIMITIVES
#define INCL_GPILOGCOLORTABLE
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME

#include <os2.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "res.h"
#include "clkdata.h"
/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

VOID InitAlarm    (HWND);
VOID AlarmSet     (HWND);
VOID SetAlarmField(HWND, UCHAR, UCHAR);
VOID GetAlarmTime (HWND, UCHAR *, UCHAR *);
VOID AlarmVScrolls(HWND, USHORT, MPARAM);
VOID AlarmSetTimer(UCHAR uchHour, UCHAR uchMinutes);

MRESULT EXPENTRY AlarmDlgProc(HWND hwnd, ULONG usItem, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY EntryFieldSubProc(HWND hwnd, ULONG usItem, MPARAM mp1, MPARAM mp2);

/**************************************************************************\
*                                                                          *
*       ROUTINE:    ClkTicksDlgProc ( )                                    *
*                                                                          *
*       COMMENT:    "Ticks..." dialog                                      *
*                                                                          *
*       RETURNS:    MRESULT, 0 or return value from WinDefDlgProc          *
*                                                                          *
\**************************************************************************/
MRESULT EXPENTRY ClkTicksDlgProc(HWND hwnd, ULONG usMsg, MPARAM mp1, MPARAM mp2)
{
   static USHORT usMajorTickSel;
   static USHORT usMinorTickSel;

   switch (usMsg)
   {
   case WM_INITDLG:

      /* show the current major tick preference */
      WinSendMsg(WinWindowFromID(hwnd,
                                 (CLKTM_MAJOR | cp.usMajorTickPref)),
                                 BM_SETCHECK,
                                 MPFROM2SHORT(TRUE, NULL),
                                 MPVOID);

      /* show the current minor tick preference */
      WinSendMsg(WinWindowFromID(hwnd,
                                 (CLKTM_MINOR | cp.usMinorTickPref)),
                                 BM_SETCHECK,
                                 MPFROM2SHORT(TRUE, 0),
                                 MPVOID);

      /* load the selection values from the preferences */
      usMajorTickSel = cp.usMajorTickPref;
      usMinorTickSel = cp.usMinorTickPref;

      /* let the default dialog procedure handle anything else */
      break;

   case WM_COMMAND:
       switch (LOUSHORT(mp1))
       {
       case DID_OK:
           /* store away selections as preferences */
          cp.usMajorTickPref = usMajorTickSel;
          cp.usMinorTickPref = usMinorTickSel;

          /* repaint with the new preferences */
          WinInvalidateRect(hwndFrame, NULL, TRUE);

       case DID_CANCEL:
          WinDismissDlg(hwnd, TRUE);
       }

       return NULL;

   case WM_CONTROL:
      if (SHORT2FROMMP(mp1) == BN_CLICKED)
      {
         USHORT usButtonID = SHORT1FROMMP(mp1);

         switch (usButtonID & 0xff00)
         {
         case CLKTM_MAJOR:
            usMajorTickSel = LOBYTE(usButtonID);
            break;

         case CLKTM_MINOR:
            usMinorTickSel = LOBYTE(usButtonID);
            break;
         }
      }
      /* fall through to the default control processing */
   }

   return WinDefDlgProc(hwnd, usMsg, mp1, mp2);
}
/**************************************************************************\
*                                                                          *
*       ROUTINE:    AlarmDlgProc ( )                                       *
*                                                                          *
*       COMMENT:    "Alarm..." dialog                                      *
*                                                                          *
*       RETURNS:    MRESULT, 0 or return value from WinDefDlgProc          *
*                                                                          *
\**************************************************************************/
MRESULT EXPENTRY AlarmDlgProc(HWND hwnd, ULONG usMsg, MPARAM mp1, MPARAM mp2)
{
   switch (usMsg)
   {
   case WM_INITDLG:
      hAlarmTime = hwnd; /* Put in a global variable so all functions know
                             we have already have an opened dialog box.  */
      InitAlarm(hwnd);
      break;

   case WM_COMMAND:
      switch (LOUSHORT(mp1))
      {
      case DID_OK:
         AlarmSet(hwnd);

      case DID_CANCEL:
         hAlarmTime = NULLHANDLE;
         WinDismissDlg(hwnd, TRUE);
      }
      return NULL;

   case WM_CONTROL:
      break;

   case WM_VSCROLL:
      AlarmVScrolls(hwnd, HIUSHORT(mp2), mp1);
      break;
   }
   return WinDefDlgProc(hwnd, usMsg, mp1, mp2);
}
/**********************************************************************
* InitAlarm
*
************************************************************************/
void InitAlarm(HWND hwnd)
{
   USHORT usItem;
   MPARAM Message;

   WinSendMsg(WinWindowFromID(hwnd,
              SCROLL_HOURS),
              SBM_SETSCROLLBAR,
              MPFROMLONG(2),
              MPFROM2SHORT(1, 3));
   WinSendMsg(WinWindowFromID(hwnd,SCROLL_MINUTES),
              SBM_SETSCROLLBAR,
              MPFROMLONG(2),
              MPFROM2SHORT(1, 3));
   WinSendDlgItemMsg(hwnd,
                     IDC_HOURMINUTES,
                     EM_SETTEXTLIMIT,
                     MPFROMLONG(5),
                     MPVOID);
   SetAlarmField(hwnd, cp.alarm.uchHour, cp.alarm.uchMinutes);
   WinSendDlgItemMsg(hwnd,
                     IDC_HOURMINUTES,
                     EM_SETSEL,
                     MPFROM2SHORT(0, 0),
                     MPVOID);
   pfnwpEntryField = WinSubclassWindow(WinWindowFromID(hwnd,
                                                       IDC_HOURMINUTES),
                                                       EntryFieldSubProc);
   usItem = (USHORT)(cp.alarm.usMode & AM_ACTIVE ? RB_ALARMON : RB_ALARMOFF);
   WinSendDlgItemMsg(hwnd,
                     usItem,
                     BM_SETCHECK,
                     MPFROMLONG(1),
                     MPVOID);
   Message = (cp.alarm.usMode & AM_AUDIO)? MPFROMLONG(1) : MPFROMLONG(0);
   WinSendDlgItemMsg(hwnd,
                      IDC_BEEP,
                      BM_SETCHECK,
                      Message,
                      MPVOID);

   Message = (cp.alarm.usMode & AM_MSGBOX) ? MPFROMLONG(1) : MPFROMLONG(0);
   WinSendDlgItemMsg(hwnd,
                     IDC_MSG,
                     BM_SETCHECK,
                     Message,
                     MPVOID);
}

/**************************************************************************
* AlarmSet
***************************************************************************/
VOID AlarmSet(HWND hwnd)
{
   GetAlarmTime(hwnd, &cp.alarm.uchHour, &cp.alarm.uchMinutes);
   cp.alarm.usMode = (AM_ACTIVE * ((ULONG)WinSendDlgItemMsg(hwnd,
                                                    RB_ALARMON,
                                                    BM_QUERYCHECK,
                                                    MPVOID,
                                                    MPVOID)))
                      |
                      (AM_AUDIO  * ((ULONG)WinSendDlgItemMsg(hwnd,
                                                    IDC_BEEP,
                                                    BM_QUERYCHECK,
                                                    MPVOID,
                                                    MPVOID)))
                      |
                      (AM_MSGBOX * ((ULONG)WinSendDlgItemMsg(hwnd,
                                                    IDC_MSG,
                                                    BM_QUERYCHECK,
                                                    MPVOID,
                                                    MPVOID)));
   AlarmSetTimer(cp.alarm.uchHour, cp.alarm.uchMinutes);
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:       AlarmSetTimer
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
 *  Returns:
 *          1 - if successful execution completed
 *          0 - if error
\****************************************************************/
VOID AlarmSetTimer(UCHAR uchHour, UCHAR uchMinutes)
{
   DATETIME DateTime;
   time_t  UTCCurrentTime, UTCAlarmTime;
   struct tm tmTime;

   memset(&DateTime, 0, sizeof(DATETIME));
   memset(&tmTime, 0, sizeof(struct tm));

   /*
    *      Get our current time then use mktime to get the UTC time.
    */
   DosGetDateTime(&DateTime);
   tmTime.tm_min   = DateTime.minutes;
   tmTime.tm_hour  = DateTime.hours;
   tmTime.tm_mday  = DateTime.day;
   tmTime.tm_sec   = DateTime.seconds;
   tmTime.tm_mon   = --DateTime.month;
   tmTime.tm_year  = (DateTime.year - 1900);

   UTCCurrentTime =  mktime(&tmTime);

    /*
     *  If the alarm time is less than the current time
     *  then the alarm must be for tomorrow.
     */
   if(uchHour < DateTime.hours  ||
              (DateTime.hours == cp.alarm.uchHour
              && uchMinutes < DateTime.minutes))
   {
      DateTime.day++;
   }
    /*
     *    Get the time in UTC format for the timer to expire.
     */
   tmTime.tm_min   = uchMinutes;
   tmTime.tm_hour  = uchHour;
   tmTime.tm_mday  = DateTime.day;
   tmTime.tm_sec   = 0;
   tmTime.tm_isdst = 0;

    /*
     * Set the timer value for the
     * difference between our  current time
     * and the alarm time (in milliseconds).
     */
   UTCAlarmTime = mktime(&tmTime);
   TimerResources.ulTimerValue  = (UTCAlarmTime  - UTCCurrentTime) * 1000L;

    /*
     *      Post the timer.
     */
   DosPostEventSem(TimerResources.hTimerDev);
}


/**************************************************************************\
*                                                                          *
*       ROUTINE:    ClkColorsDlgProc ( )                                   *
*                                                                          *
*       COMMENT:    "Clock Color Preferences" Dialog                       *
*                                                                          *
*       RETURNS:    MRESULT, 0 or return value from WinDefDlgProc          *
*                                                                          *
\**************************************************************************/
MRESULT EXPENTRY ClkColorsDlgProc(HWND hwnd, ULONG usMsg, MPARAM mp1, MPARAM mp2)
{
   static USHORT usCheckedPartID = 0, usCheckedColorID = 0;
   static LONG clrBackgroundNew;
   static LONG clrFaceNew;
   static LONG clrHourHandNew;
   static LONG clrMinuteHandNew;
   static LONG *pclrNew;

   switch (usMsg)
   {
      USHORT usButtonID;
      HWND hwndButton;
      HPS hpsT;
      RECTL rclButton, rclButtonInterior;

   case WM_INITDLG:
      hpsT = WinGetPS(hwnd);

      GpiCreateLogColorTable(hpsT, LCOL_RESET, LCOLF_RGB, 0L, 0L, NULL);

      /* Load the new values from the current ones.    */
      clrBackgroundNew = GpiQueryColorIndex(hpsT, 0L, cp.clrBackground);

      clrFaceNew       = GpiQueryColorIndex(hpsT, 0L, cp.clrFace      );
      clrHourHandNew   = GpiQueryColorIndex(hpsT, 0L, cp.clrHourHand  );
      clrMinuteHandNew = GpiQueryColorIndex(hpsT, 0L, cp.clrMinuteHand);

      WinReleasePS(hpsT);

      /* click the "Background" radio button */
      WinPostMsg(WinWindowFromID(hwnd,
                                 CLKCLR_BACKGROUND),
                                 BM_CLICK,
                                 MPFROMSHORT(TRUE),
                                 MPVOID);

      /* Let the default dialog procedure handle anything else.   */
      break;

   case WM_COMMAND:
      switch (LOUSHORT(mp1))
      {
         case DID_OK:
            hpsT = WinGetPS(hwnd);

            GpiCreateLogColorTable(hpsT, LCOLF_RGB, 0L, 0L, 0L, NULL);

              /*
               * Uupdate the one the user has selected.
               */
            switch(usCheckedPartID)
            {
            case CLKCLR_BACKGROUND:
               cp.clrBackground = GpiQueryRGBColor(hpsT, 0L, clrBackgroundNew);
               break;

            case CLKCLR_FACE:
               cp.clrFace = GpiQueryRGBColor(hpsT, 0L, clrFaceNew);
               break;

            case CLKCLR_HOURHAND:
               cp.clrHourHand = GpiQueryRGBColor(hpsT, 0L, clrHourHandNew);
               break;

            case CLKCLR_MINUTEHAND:
               cp.clrMinuteHand = GpiQueryRGBColor(hpsT, 0L, clrMinuteHandNew);
               break;
            }
            WinReleasePS(hpsT);

              /* repaint with the new colors */
            WinInvalidateRect(hwndFrame, NULL, TRUE);

         case DID_CANCEL:
            WinDismissDlg(hwnd, TRUE);
      }
      return NULL;

   case WM_CONTROL:
      usButtonID = SHORT1FROMMP(mp1);

      /* selecting a new object */
      if (usButtonID & CLKCLR_OBJECTS)
      {
         MRESULT rc;

         switch (usButtonID)
         {
            case CLKCLR_BACKGROUND:
               pclrNew = &clrBackgroundNew;
               break;
            case CLKCLR_FACE:
               pclrNew = &clrFaceNew;
               break;
            case CLKCLR_HOURHAND:
               pclrNew = &clrHourHandNew;
               break;
            case CLKCLR_MINUTEHAND:
               pclrNew = &clrMinuteHandNew;
         }
         usCheckedPartID = usButtonID;

          /* click the button for the new object's current color */
         rc = WinSendMsg(WinWindowFromID(hwnd,
                                         (CLKCLR_BUTTONSHIFT + *pclrNew)),
                                         BM_CLICK,
                                         MPFROMSHORT(TRUE), MPVOID);
         break;
      }

      switch (SHORT2FROMMP(mp1))
      {
      case BN_CLICKED:
         *pclrNew = (LONG)usButtonID - CLKCLR_BUTTONSHIFT;

           /* Turn off the check state of the previously checked
            * button and turn on the check state of the button
            * just clicked.
            */
         WinCheckButton(hwnd, usCheckedColorID, FALSE);
         WinCheckButton(hwnd, usButtonID, TRUE);
         usCheckedColorID = usButtonID;
         break;

      case BN_PAINT:

              /* Fill only the interior of the button, so we don't
               * conflict with the focus indicator */
         hwndButton = ((PUSERBUTTON) mp2) -> hwnd;
         WinQueryWindowRect(hwndButton, &rclButton);
         rclButton.xLeft++;
         rclButton.yBottom++;
         rclButton.xRight--;
         rclButton.yTop--;
         WinFillRect(((PUSERBUTTON) mp2)->hps,
                       &rclButton,
                       (LONG)usButtonID - CLKCLR_BUTTONSHIFT);

         /* Hollow out those buttons which aren't checked */
         if (!WinQueryButtonCheckstate(hwnd, usButtonID))
         {
            rclButtonInterior.xLeft   = rclButton.xLeft   + 4;
            rclButtonInterior.yBottom = rclButton.yBottom + 4;
            rclButtonInterior.xRight  = rclButton.xRight  - 4;
            rclButtonInterior.yTop    = rclButton.yTop    - 4;
            WinFillRect(((PUSERBUTTON)mp2)->hps,
                          &rclButtonInterior, SYSCLR_WINDOW);
         }
         break;
      }
      /* fall through to the default control processing */
   }
   return WinDefDlgProc(hwnd, usMsg, mp1, mp2);
}


/**************************************************************************\
*                                                                          *
*       ROUTINE:    EntryFieldSubProc                                      *
*                                                                          *
*       COMMENT:    "Alarm..." dialog                                      *
* This procedure filter messages for the entry field and make it a time    *
* entry field. It doesn't allow for illigal characters according to the    *
* current selection. It doesn't allow modification of the time separator.  *
* It works only for hours and minutes. The entry field has to be limited   *
* to 5 characters.                                                         *
*                                                                          *
*                                                                          *
*       RETURNS:    MRESULT, 0 or return value from WinDefDlgProc          *
*                                                                          *
\**************************************************************************/
MRESULT EXPENTRY EntryFieldSubProc(HWND hwnd, ULONG usMsg, MPARAM mp1, MPARAM mp2)
{
   USHORT usSelect, fsKeyFlags, usLast;
   UCHAR usChr1;
   UCHAR uchHourMinutes[6];
   BOOL bValid;

   if ((usMsg > WM_MOUSEFIRST) && (usMsg < WM_MOUSELAST))
   {
      MRESULT mresult = pfnwpEntryField(hwnd, usMsg, mp1, mp2);
      MRESULT mresultSel = pfnwpEntryField(hwnd, EM_QUERYSEL, (PLONG)NULL, (PLONG)NULL);

      usSelect = LOUSHORT(mresultSel);
      usLast   = HIUSHORT(mresultSel);
      if ((usSelect == 2))
         pfnwpEntryField(hwnd, EM_SETSEL, MPFROM2SHORT(3, 3), NULL);
      if ((usSelect == 5))
         pfnwpEntryField(hwnd, EM_SETSEL, MPFROM2SHORT(4, 4), NULL);
      return mresult;
   }

   switch (usMsg)
   {
   case WM_SETFOCUS:
      pfnwpEntryField(hwnd, usMsg, mp1, mp2);
      if (SHORT1FROMMP(mp2))
          pfnwpEntryField(hwnd,
                          EM_SETINSERTMODE,
                          MPFROMSHORT(FALSE),
                          (MPARAM)NULL);
      return MPVOID;

   case WM_CHAR:
      fsKeyFlags = SHORT1FROMMP(mp1);    /* key flags             */
      if (!(fsKeyFlags & KC_KEYUP))
      {
         usSelect = (USHORT)WinSendMsg(hwnd, EM_QUERYSEL, MPVOID, MPVOID);
         if (fsKeyFlags & KC_VIRTUALKEY)
         {
            switch(SHORT2FROMMP(mp2))
            {
            case VK_UP:
            case VK_DOWN:
               break;

            case VK_LEFT:
               if (usSelect == 3)
                   pfnwpEntryField(hwnd, usMsg, mp1, mp2);
               return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
               break;

            case VK_RIGHT:
               if (usSelect == 4)
               {
                  return MPVOID;
               }
               if (usSelect == 1)
                  pfnwpEntryField(hwnd, usMsg, mp1, mp2);
               return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
               break;

            case VK_DELETE:
            case VK_BACKSPACE:
            case VK_INSERT:
               return MPVOID;

            case VK_SPACE:
               switch (usSelect)
               {
               case 0:   /*  Hours most significant digit   */
                  if (bTwelveHourFormat)
                  {
                     return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                  }

               default:
                  return MPVOID;
               }

            default:
               return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
               break;
            }
         }
         else
         {
            if (fsKeyFlags & KC_CHAR)
            {
               bValid = FALSE;
               WinQueryWindowText(hwnd, sizeof(uchHourMinutes), uchHourMinutes);
               usChr1 = (UCHAR)CHAR1FROMMP(mp2);
               switch (usSelect)
               {
               case 0:   /* Hours most significant digit */
                  if (bTwelveHourFormat)
                  {
                     if ((usChr1 > '1') || (usChr1 < '0'))
                     {
                         return MPVOID;
                     }
                     else
                     {
                         pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                         bValid = TRUE;
                     }
                  }
                  else
                  {
                     if ((usChr1 > '2') || (usChr1 < '0'))
                     {
                         return MPVOID;
                     }
                     else
                     {
                         pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                         bValid = TRUE;
                     }
                  }
                  if (bValid)
                  {
                     uchHourMinutes[0] = usChr1;
                     uchHourMinutes[1] = '0';
                     WinSetWindowText(hwnd, uchHourMinutes);
                     pfnwpEntryField(hwnd, EM_SETSEL, MPFROM2SHORT(1,1),(PLONG)NULL);
                  }
                  return MPVOID;
                  break;

               case 1:
                  if (bTwelveHourFormat)
                  {
                     if ((uchHourMinutes[0] == '0') || (uchHourMinutes[0] == ' '))
                     {
                        if ((usChr1 > '9') || (usChr1 < '0'))
                        {
                           return MPVOID;
                        }
                        else
                        {
                           pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                           bValid = TRUE;
                        }
                   }
                   else
                   {    /* probably 1  */
                      if ((usChr1 > '2') || (usChr1 < '0'))
                      {
                         return MPVOID;
                      }
                      else
                      {
                         pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                         bValid = TRUE;
                      }
                   }
               }
                else
                {
                   if (uchHourMinutes[0] < '2')
                   {
                      if ((usChr1 > '9') || (usChr1 < '0'))
                      {
                         return MPVOID;
                      }
                      else
                      {
                         pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                         bValid = TRUE;
                      }
                   }
                   else
                   {
                      if ((usChr1 > '4') || (usChr1 < '0'))
                      {
                         return MPVOID;
                      }
                      else
                      {
                         pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                         bValid = TRUE;
                      }
                   }
                }
                /*  Fill the entry field as if a time separator is in also */
                if (bValid)
                {
                   mp2 = (MPARAM)(((ULONG)mp2 & 0xffffff00) | szTimeSep[0]);
                   return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                }
                break;

            case 3:   /* minutes left digit, 0-5 */
               if ((usChr1 > '5') || (usChr1 < '0'))
               {
                  return MPVOID;
               }
               else
                  return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                  break;

            case 4:   /*minutes right digit, 0-9*/
               if ((usChr1 > '9') || (usChr1 < '0'))
               {
                  return MPVOID;
               }
               else
               {
                  pfnwpEntryField(hwnd, usMsg, mp1, mp2);
                  return pfnwpEntryField(hwnd, EM_SETSEL, MPFROM2SHORT(4,4), MPVOID);
               }
               break;
            }
         }
      }
   }

   default:
      return pfnwpEntryField(hwnd, usMsg, mp1, mp2);
   }
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:AlarmScrolls()
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
 *  Returns:
 *          1 - if successful execution completed
 *          0 - if error
\****************************************************************/
VOID  AlarmVScrolls(HWND hwnd, USHORT ScrollMsg, MPARAM idScroll)
{
   UCHAR uchHours, uchMinutes;

   GetAlarmTime(hwnd, &uchHours, &uchMinutes);

   switch(ScrollMsg)
   {
   case SB_LINEUP:
   case SB_PAGEUP:
      if (idScroll == (MPARAM)SCROLL_HOURS)
         if (uchHours == 23)
            uchHours = 0;
         else uchHours++;
      else
         if (uchMinutes == 59)
            uchMinutes = 0;
         else
            uchMinutes++;
         break;


   case SB_LINEDOWN:
   case SB_PAGEDOWN:
      if (idScroll == (MPARAM)SCROLL_HOURS)
         if (uchHours == 0)
            uchHours = 23;
         else uchHours--;
      else
         if (uchMinutes == 00)
            uchMinutes = 59;
         else
            uchMinutes--;
      break;


    }
    SetAlarmField(hwnd, uchHours, uchMinutes);
    WinSendDlgItemMsg(hwnd,
                        IDC_HOURMINUTES,
                        EM_SETSEL,
                        (idScroll == (MPARAM)SCROLL_HOURS) ?MPFROM2SHORT(0,0):MPFROM2SHORT(3,3),
                        MPVOID);
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:        GetAlarmTime()
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
 *  Returns:
 *
 *          VOID
\****************************************************************/
VOID GetAlarmTime(HWND hwnd,UCHAR *npuchHours,UCHAR *npuchMinutes)
{
   UCHAR achHourMinutes[6];
   UCHAR uchHours;

   WinQueryDlgItemText(hwnd,
                       IDC_HOURMINUTES,
                       sizeof(achHourMinutes),
                       achHourMinutes);
   uchHours = (UCHAR)atoi((const char *)achHourMinutes);

   if (bTwelveHourFormat)
   {
      if (WinSendDlgItemMsg(hwnd,
                            RB_PM,
                            BM_QUERYCHECK,
                            MPVOID,
                            MPVOID))
      {
         if(uchHours != 12)
            uchHours += 12;
      }
      else
      {
         if(uchHours == 12)
            uchHours = 0;
      }
   }
   *npuchHours = uchHours;
   *npuchMinutes = (UCHAR)atoi((const char *)&achHourMinutes[3]);
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:     SetAlarmField
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
 *  Returns:
 *          1 - if successful execution completed
 *          0 - if error
\****************************************************************/
VOID SetAlarmField(HWND hwnd,UCHAR uchHours,UCHAR uchMinutes)
{
   UCHAR achHourMinutes[6];

   if (bTwelveHourFormat)
   {
      if (uchHours >= 12)
      {
         uchHours -= 12;
         WinSendDlgItemMsg(hwnd,
                           RB_PM,
                           BM_SETCHECK,
                           MPFROMLONG(1),
                           MPVOID);
        if (uchHours == 0)
           uchHours = 12;
      }
      else
      {
         if (uchHours == 0)
            uchHours = 12;
         WinSendDlgItemMsg(hwnd,
                           RB_AM,
                           BM_SETCHECK,
                           MPFROMLONG(1),
                           MPVOID);
      }
   }
   else
   {
       WinShowWindow(WinWindowFromID(hwnd,RB_PM),FALSE);
       WinShowWindow(WinWindowFromID(hwnd,RB_AM),FALSE);
   }
   _itoa(uchHours, achHourMinutes,10);
   _itoa(uchMinutes, &achHourMinutes[3],10);
   achHourMinutes[2] = szTimeSep[0];
   if (achHourMinutes[1] == 0)
   {
      achHourMinutes[1] = achHourMinutes[0];
      achHourMinutes[0] = ' ';
   }
   if (achHourMinutes[4] == 0)
   {
      achHourMinutes[4] = achHourMinutes[3];
      achHourMinutes[3] = '0';
   }
   WinSetDlgItemText(hwnd, IDC_HOURMINUTES, achHourMinutes);
}

/****************************************************************\
 *  Dialog procedure for the About dialog box
 *--------------------------------------------------------------
 *
 *  Name:   AboutBoxDlgProc(hwnd, msg, mp1, mp2)
 *
 *  Purpose: Processes all messages sent to the About Box
 *
 *  Usage:  Called for each message sent to the About Box
 *          dialog box.
 *
 *  Method: the about box only has a button control so this
 *          routine only processes WM_COMMAND messages.  Any
 *          WM_COMMAND posted must have come from the Ok
 *          button so we dismiss the dialog upon receiving it.
 *
 *  Returns: Dependent upon message sent
 *
\****************************************************************/
MRESULT EXPENTRY AboutBoxDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch(msg)
    {
        case WM_COMMAND:
            /* no matter what the command, close the dialog */
            WinDismissDlg(hwnd, TRUE);
            break;

        default:
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
            break;
    }

    return MPVOID;
}   /* AboutBoxWndProc() */

/*--------------------------------------------------------------*\
 *  End of file: dialogs.c
\*--------------------------------------------------------------*/
