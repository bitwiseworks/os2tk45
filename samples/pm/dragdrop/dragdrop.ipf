.*==============================================================*\
.*                                                              *
.* Main.ipf - Information Tag Language file for the Drag/Drop   *
.*              sample application's help manager               *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.Drag/Drop Sample -- Help
:i1 id=aboutMain.About Sample
:p.The Drag/Drop program is a sample Presentation Manager application
which illustrates direct manipulation.
:p.To show direct manipulation, start two instances of the sample
in separate windows.  In one of the sessions, open a directory from which
you want to see a file list, then select names of files you want to move.
In the other Drap/Drop session, open a
directory to which you want to move the files.  Perform the drag and drop by
holding down mouse button 2 in the first window.  Drag the
mouse across to the target window and release it.  The files will be moved to
the target directory.

.*-- Import the File menu help file --*
.im dragfile.ipf

.*-- Import the Window menu help file --*
.im dragwnd.ipf

.*-- Import the Help menu help file --*
.im draghelp.ipf

.*-- Import the Application Menu help file --*
.im dragmenu.ipf

.*-- Import the Dialog boxes help file --*
.im dragdlg.ipf

:euserdoc.
