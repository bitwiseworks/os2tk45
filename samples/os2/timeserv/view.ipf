.*==============================================================*\
.*                                                              *
.* View.ipf - Information Tag Language file for the Help menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main View menu                                              *
.*      res = PANEL_VIEW                                        *
.*--------------------------------------------------------------*/
:h1 res=4000 name=PANEL_VIEW.View Menu
:i1 id=View.View Menu
:p.The following View commands adjust the display of the clock application.
:parml tsize=15 break=none.
:pt.Time
:pd.Selection causes the time to be displayed.
:pt.Date
:pd.Selection causes the date to be displayed.
:pt.Analog
:pd.A standard clock dial and hands is displayed.
:pt.Digital
:pd.Time and date will be displayed in numerical format.
:eparml.


.*--------------------------------------------------------------*\
.*  View menu Time command help panel                            *
.*      res = PANEL_VIEWTIME                                    *
.*--------------------------------------------------------------*/
:h1 res=4100 name=PANEL_VIEWTIME.Display Time
:i2 refid=View.Display Time
:p. Display the time.



.*--------------------------------------------------------------*\
.*  View menu Date command help panel                           *
.*      res = PANEL_VIEWDATE                                    *
.*--------------------------------------------------------------*/
:h1 res=4200 name=PANEL_VIEWDATE.Display Date
:i2 refid=View.Display Date
:p. Display the date.


.*--------------------------------------------------------------*\
.*  Help menu Analog command help panel                        *
.*      res = PANEL_VIEWANALOG                                  *
.*--------------------------------------------------------------*/
:h1 res=4300 name=PANEL_VIEWANALOG.Display Analog Clock
:i2 refid=View.Display Analog Clock
:p. Display clock dial with hands.


.*--------------------------------------------------------------*\
.*  Help menu Digital command help panel                          *
.*      res = PANEL_VIEWDIGITAL                                 *
.*--------------------------------------------------------------*/
:h1 res=4400 name=PANEL_VIEWDIGITAL.Display Digital Clock
:i2 refid=View.Display Digital Clock
:p. Display numerical read out.
