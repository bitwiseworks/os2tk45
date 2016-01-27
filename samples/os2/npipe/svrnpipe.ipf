.*==============================================================*\
.*                                                              *
.* Svrmain.ipf - Information Tag Language file for the main     *
.*              sample application's help manager               *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Sample
:i1 id=aboutMain.About Sample
:p.The Sample pipe application is the server end of a tic-tac-toe
game.  It will display all messages received from and sent to th
client processes.

.*-- Import the File menu help file --*
.im file.ipf

.*-- Import the Option menu help file --*
.im svroptn.ipf

.*-- Import the Help menu help file --*
.im help.ipf

.*-- Import the Dialog boxes help file --*
.im dlg.ipf

:euserdoc.
