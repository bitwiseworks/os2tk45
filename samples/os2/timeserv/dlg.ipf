.*==============================================================*\
.*                                                              *
.* Dlg.ipf - Information Tag Language file for the Dialogs      *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_ALARM_TIME                                  *
.*--------------------------------------------------------------*/
:h1 res=8100 name=PANEL_ALARM_TIME.Alarm Time
:i1 id=Alarm.Alarm Time
:p.Use scroll bars on the sides of the display to change the alarm
time, or select the time field and enter it by the keyboard.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                              *
.*      res = PANEL_ALARM_AMPM                                  *
.*--------------------------------------------------------------*/
:h1 res=8200 name=PANEL_ALARM_AMPM.AM/PM
:i2 refid=Alarm.AM/PM
:p.Select AM or PM for the alarm time.



.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_ALARM_TOGGLE                                *
.*--------------------------------------------------------------*/
:h1 res=8300 name=PANEL_ALARM_TOGGLE.Alarm On/Off
:i2 refid=Alarm.Alarm On/Off
:p.The alarm is turned On or Off with this control.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_ALARM_AUDIBLE                               *
.*--------------------------------------------------------------*/
:h1 res=8400 name=PANEL_ALARM_AUDIBLE.Audio Alarm
:i2 refid=Alarm.Audio Alarm
:p.Checking this box will cause an audible alarm to ring at the alarm time.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_ALARM_MESSAGE                               *
.*--------------------------------------------------------------*/
:h1 res=8500 name=PANEL_ALARM_MESSAGE.Message Box
:i2 refid=Alarm.Message Box
:p.Checking this box will cause a message to popup at the alarm time.




.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_TICKS_MAJOR                                 *
.*--------------------------------------------------------------*/
:h1 res=6100 name=PANEL_TICKS_MAJOR.Major Tick Marks
:i2 refid=Tick-Marks.Major Tick Marks
:p.Selecting Always will cause a mark to be placed at the position of each
dial number.  The marks will not be displayed on the icon if Except When
Iconic is selected.  And selecting Never will never display the hour marks.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_TICKS_MINOR                                 *
.*--------------------------------------------------------------*/
:h1 res=6200 name=PANEL_TICKS_MINOR.Minor Tick Marks
:i2 refid=Tick-Marks.Minor Tick Marks
:p.Selecting Always will cause a mark to be placed between each hour mark to
mark each minute.  The marks will not be displayed on the icon if Except When
Iconic is selected.  And selecting Never will never display the minute marks.




.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_SET_TIME                                    *
.*--------------------------------------------------------------*/
:h1 res=7100 name=PANEL_SET_TIME.Time
:i2 refid=Set-Time/Date.Time
:p.Select the portion of the time you wish to change (hour, minute, second)
and either enter the new value, or use the adjacent scroll bar to adjust.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_SET_DATE                                    *
.*--------------------------------------------------------------*/
:h1 res=7200 name=PANEL_SET_DATE.Date
:i2 refid=Set-Time/Date.Date
:p.Select the portion of the date you wish to change (month, day, year)
and either enter the new value, or use the adjacent scroll bar to adjust.




.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_COLOR_PARTS                                 *
.*--------------------------------------------------------------*/
:h1 res=9100 name=PANEL_COLOR_PARTS.Parts of the Clock
:i2 refid=Colors.Parts of the Clock
:p.Select the clock part you wish to change the color of.


.*--------------------------------------------------------------*\
.*  Alarm dialog command help panel                             *
.*      res = PANEL_COLOR_BUTTONS                               *
.*--------------------------------------------------------------*/
:h1 res=9200 name=PANEL_COLOR_BUTTONS.GPI Colors
:i2 refid=Colors.GPI Colors
:p.Select the color you want for the clock part.
