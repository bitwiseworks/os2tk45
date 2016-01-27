.*==============================================================*\
.*                                                              *
.* Main.ipf - Information Tag Language file for the main        *
.*              sample application's help manager               *
.* Copyright 1991 IBM Corporation                               *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Image
:i1 id=aboutMain.About Image
:p.The Image Sample Program will display a monochrome bitmap in a window.
First choose a file with extension .IMG from a disk directory, then open
it with the File Open dialog.  The View menu allows you to change the
appearance of the image in the display window.

.*-- Import the File menu help file --*
.im img_file.ipf

.*-- Import the View menu help file --*
.im img_view.ipf

.*-- Import the Help menu help file --*
.im img_help.ipf

:euserdoc.
