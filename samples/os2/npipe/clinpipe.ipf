.*==============================================================*\
.*                                                              *
.* Clinpipe.ipf - Information Tag Language file for the main    *
.*              sample pipe application's help manager          *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Sample
:i1 id=aboutMain.About Sample
:p.The Named pipe application is the client end of a sample
application of named pipes for communication between unrelated processes.

.*-- Import the File menu help file --*
.im clifile.ipf

.*-- Import the Option menu help file --*
.im clioptn.ipf

.*-- Import the Help menu help file --*
.im help.ipf

.*-- Import the About box help file --*
.im dlg.ipf

.*-- Import the Dialog box help file --*
.im clidlg.ipf

:euserdoc.
