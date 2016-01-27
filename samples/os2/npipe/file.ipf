.*==============================================================*\
.*                                                              *
.* File.ipf - Information Tag Language file for the File menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_FILE                                        *
.*--------------------------------------------------------------*/
:h1 res=2210 name=PANEL_FILE.File Menu
:i1 id=File.File Menu
:p.The File menu contains commands that you use to create, open,
and save data files. In addition, it also contains the
command that you use to exit the Pipes application. The following
commands appear in the File menu:
:parml tsize=15 break=none.
:pt.Exit
:pd.Quits the Sample application
:eparml.


.*--------------------------------------------------------------*\
.*  File menu Exit command help panel                           *
.*      res = PANEL_FILEEXIT                                    *
.*--------------------------------------------------------------*/
:h1 res=2290 name=PANEL_FILEEXIT.Exit
:i2 refid=File.Exit
:p.You quit the Sample Pipes application by using the Exit command. To
quit the Sample Pipes application, do the following:
:ol.
:li.Select the File menu and choose the Exit command.
:eol.
