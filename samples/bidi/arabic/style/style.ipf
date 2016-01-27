.**************************************************************************
.*
.*  File Name   : STY_IPF.IPF
.*
.*  Description : Information Tag Language file for help on the help manager
.*
.*  Copyright (C) 1992 IBM Corporation
.*
.*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
.*      sample code created by IBM Corporation. This sample code is not
.*      part of any standard or IBM product and is provided to you solely
.*      for  the purpose of assisting you in the development of your
.*      applications.  The code is provided "AS IS", without
.*      warranty of any kind.  IBM shall not be liable for any damages
.*      arising out of your use of the sample code, even if they have been
.*      advised of the possibility of such damages.                                                    *
.*
.**************************************************************************

:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel
.*      res = PANEL_MAIN
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Style
:i1 id=aboutMain.About Style
:p.The Style application is a sample Presentation Manager application
which demonstrates the basic functionality of a PM application.  The
application allows a user to edit and save basic text files.
It also has some dialogs which demonstrate the various control windows
and message boxes.

.*-- Import the File menu help file --
.im sty_file.ipf

.*-- Import the Edit menu help file --
.im sty_edit.ipf

.*-- Import the Help menu help file --
.im sty_help.ipf

.*-- Import the Application Menu help file --
.im sty_menu.ipf

.*-- Import the Dialog boxes help file --
.im sty_dlg.ipf

:euserdoc.
