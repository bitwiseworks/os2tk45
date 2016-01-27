
.*==============================================================*\
.*                                                              *
.* Main.ipf - Information Tag Language file for the             *
.*       clock  sample application's help manager               *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Sample
:i1 id=aboutMain.About Sample
:p.The Clock Sample application is a sample Presentation Manager
program that demonstates how to use the system and window timers
in a OS/2 Application.

.*-- Import the Settings menu help file --*
.im settings.ipf

.*-- Import the View menu help file --*
.im view.ipf

.*-- Import the Help menu help file --*
.im help.ipf

.*-- Import the Dialogs help file --*
.im dlg.ipf

:euserdoc.
