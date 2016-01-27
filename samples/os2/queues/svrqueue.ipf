.*==============================================================*\
.*                                                              *
.* svrqueue.ipf - Information Tag Language file for the main    *
.*              sample application's help manager               *
.*                                                              *
.*  Copyright 1991, IBM Corp.                                   *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Sample
:i1 id=aboutMain.About Sample
:p.This sample application demonstrates the use of an IPC queue.
Messages entered in multiple client processes are displayed
by the server process, having been passed through a queue.

.*-- Import the File menu help file --*
.im file.ipf

.*-- Import the Option menu help file --*
.im svroptn.ipf

.*-- Import the Help menu help file --*
.im help.ipf

.*-- Import the Dialog boxes help file --*
.im dlg.ipf

:euserdoc.
