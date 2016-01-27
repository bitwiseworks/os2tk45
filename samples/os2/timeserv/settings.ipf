.*==================================================================*\
.*                                                                  *
.* Settings.ipf - Information Tag Language file for the Help menu   *
.*                help panels.                                      *
.*                                                                  *
.*==================================================================*/

.*--------------------------------------------------------------*\
.*  Main Settings menu                                          *
.*      res = PANEL_SETTINGS                                    *
.*--------------------------------------------------------------*/
:h1 res=2200 name=PANEL_SETTINGS.Settings
:i1 id=Settings.Settings Menu
:p.The Settings menu contains commands you use to change the appearance of
and the time displayed by the clock.  It also contains the commands used to
control the alarm.  The following commands appear in the Settings menu.
:parml tsize=15 break=none.
:pt.Colors
:pd.Changes the clocks colors.
:pt.Tick Marks
:pd.Controls the drawing of the hour and minute marks on the dial.
:pt.Date/Time
:pd.Sets the date and time.
:pt.Alarm
:pd.Sets the alarm.
:pt.Second Hand
:pd.Toggles the display of the second hand.
:pt.Toggle Frame Controls
:pd.Controls whether or not the menu bar and other controls are displayed.
:eparml.


.*--------------------------------------------------------------*\
.*  Settings menu Colors command help panel                     *
.*      res = PANEL_SETTINGSCOLORS                              *
.*--------------------------------------------------------------*/
:h1 res=2300 name=PANEL_SETTINGSCOLORS.Colors
:i2 refid=Settings.Colors
:p.Displays a dialog box for the customization of the clocks colors.



.*--------------------------------------------------------------*\
.*  Settings menu Tick mark command help panel                  *
.*      res = PANEL_SETTINGSTICKS                               *
.*--------------------------------------------------------------*/
:h1 res=2400 name=PANEL_SETTINGSTICKS.Tick Marks
:i2 refid=Settings.Tick Marks
:p.Displays a dialog box for setting the display of the hour and minute marks.


.*--------------------------------------------------------------*\
.*  Settings menu Date/Time command help panel                  *
.*      res = PANEL_SETTINGSDATETIME                            *
.*--------------------------------------------------------------*/
:h1 res=2500 name=PANEL_SETTINGSDATETIME.Date/Time
:i2 refid=Settings.Date/Time
:p.Displays a dialog box for setting the time and date.


.*--------------------------------------------------------------*\
.*  Settings menu Alarm command help panel                      *
.*      res = PANEL_SETTINGSALARM                               *
.*--------------------------------------------------------------*/
:h1 res=2600 name=PANEL_SETTINGSALARM.Alarm
:i2 refid=Settings.Alarm
:p.Displays a dialog box for the setting of the alarm.


.*--------------------------------------------------------------*\
.*  Settings menu Second Hand command help panel                *
.*      res = PANEL_SETTINGSSECONDHAND                          *
.*--------------------------------------------------------------*/
:h1 res=2700 name=PANEL_SETTINGSSECONDHAND.Second Hand
:i2 refid=Settings.Second Hand
:p.Selection causes the second hand to appear, deselection hides it.


.*--------------------------------------------------------------*\
.*  Settings menu Toggle command help panel                     *
.*      res = PANEL_SETTINGSTOGGLECONTROLS                      *
.*--------------------------------------------------------------*/
:h1 res=2800 name=PANEL_SETTINGSTOGGLECONTROLS.Toggle Frame Controls
:i2 refid=Settings.Toggle Frame Controls
:p.Selecting this control causes the various control windows at the top
of the clock to disappear.
