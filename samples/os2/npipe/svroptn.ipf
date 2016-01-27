.*==============================================================*\
.*                                                              *
.* Svroption.ipf - Information Tag Language file for the Option *
.*              menu help panels.                               *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_OPTION                                      *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_OPTION.Option Menu
:i1 id=Option.Option Menu
:p.The Option menu contains commands that you use to manipulate
the display of pipe messages.
The following commands appear in the File menu:
:parml tsize=15 break=none.
:pt.Clear Messages
:pd.Clears the screen of messages.
:eparml.

.*--------------------------------------------------------------*\
.*  File menu New command help panel                            *
.*      res = PANEL_OPTIONPURGE                                 *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_OPTIONCLEAR.Clear
:i2 refid=Option.Purge
:p. You can empty the contents of the screen by
using the Clear Messages command.
