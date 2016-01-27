:userdoc.
.************************************************
.*
.*   Copyright 1991 IBM Corporation
.*
.*   HELP FOR DIALOG BOX FIELDS
.*   OPEN A GRAPHIC METAFILE
.*
.*   OPEN FILENAME       - 20810
.*   TYPE OF FILE        - 20811
.*   DRIVE               - 20812
.*   FILE                - 20813
.*   DIRECTORY           - 20814
.*   OPEN                - 20816
.*   CANCEL              - 20817
.*
.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About Sample
:i1 id=aboutMain.About Sample
:p.The Sample application is a sample Presentation Manager application
which an application developer can use as a base for his own applications.
The sample program uses many of the standard menus and dialogs that
most applications would use.  The source is designed to serve as a
base for any Presentation Manager application and was written in such
a way that it can be easily modified to handle any application specific
commands.
.*
.*************************************************
.*
:h1 res=20801.Help for File
:p.
Use :hp2.File:ehp2. to open or print Metafile.
.*
.*************************************************
:h2 res=20803.Help for Open
:p.
Use :hp2.Open:ehp2. to display a window that enables you to choose a
Metafile.
.*
.*************************************************
.*
:h1 res=20804.Help for Print
:p.
Use :hp2.Print:ehp2. to select the displayed Metafile or GPI circles
.*
.*************************************************
.*
:h1 res=20805.Help for OK
:p.
Use :hp2.OK:ehp2. after reading :hp2.Product information:ehp2. to return to
:hp2.Graphic:ehp2..
.*
.************************************************
:h1 res=20809.Help for Open
:p.
Use :hp2.Open:ehp2. to load a Meta file, following these steps.
:ol compact.
:li.Select the down arrow to the right of the :hp2.Drive:ehp2. list to
display all the drives on your system.
:li.Select a drive from the :hp2.Drive:ehp2. list.
:li.Select a directory from the :hp2.Directory:ehp2. list.
:li.Select a file name from the :hp2.File:ehp2. list or type in a file
name.
:li.Select the :hp2.Open:ehp2. pushbutton to display the puzzle
you want to view.
:eol.
:p.
For a detailed explanation of each field, select from the list
below&colon.
:ul compact.
:li.:link res=20810 reftype=hd.Open file name:elink.
:li.:link res=20811 reftype=hd.Type of file:elink.
:li.:link res=20812 reftype=hd.Drive:elink.
:li.:link res=20813 reftype=hd.File:elink.
:li.:link res=20814 reftype=hd.Directory:elink.
:li.:link res=20816 reftype=hd.Open pushbutton:elink.
:li.:link res=20817 reftype=hd.Cancel pushbutton:elink.
:eul.
.*
.************************************************
.*   HELP FOR OPEN FILENAME
.************************************************
.*
:h1 res=20810.Help for Open Filename
:p.
Type the name of the file you want to open in the :hp2.Open
filename:ehp2. field and select the :hp2.Open:ehp2. pushbutton.
.*
.************************************************
.*   HELP FOR TYPE OF FILE
.************************************************
.*
:h1 res=20811.Help for Type of File
:p.
Select the down arrow to the right of the :hp2.Type of file:ehp2.
field to display the available file types.  The Trace Formatter has
set this field for all file types.
.*
.************************************************
.*   HELP FOR DRIVE
.************************************************
.*
:h1 res=20812.Help for Drive
:p.
The :hp2.Drive:ehp2. list displays the drives on your system.  Select
the drive that contains the files you want to view.
.*
.*--------------------------------------------------------------*\
.*  Help menu Save command help panel                           *
.*      res = PANEL_HELPKEYS                                    *
.*--------------------------------------------------------------*/
:h1 res=20818 name=PANEL_HELPKEYS.Keys
:i2 refid=Help.Keys
:p.Place information for the Help Keys menu item here.
.*
.************************************************
.*   HELP FOR FILE
.************************************************
.*
:h1 res=20813.Help for File
:p.
The :hp2.File:ehp2. list displays all the files in the directory you
selected from the :hp2.Directory:ehp2. list.  Select the file you want
to open.
.*
.************************************************
.* Help for DIRECTORY - File, New, Dialog
.************************************************
.*
:h1 res=20814.Help for Directory
:p.
The :hp2.Directory:ehp2. list displays the directories on the selected
drive.  Select a directory to display the list of files from that
directory in the :hp2.File:ehp2. list box.
.*
.************************************************
.*   HELP FOR OPEN
.************************************************
.*
:h1 res=20816.Help for Open
:p.
Select :hp2.Open:ehp2. to accept all information in the window,
display the puzzle and remove the window from the
screen.
.*
.************************************************
.*   HELP FOR CANCEL
.************************************************
.*
:h1 res=20817.Help for Cancel
:p.
Select :hp2.Cancel:ehp2. to remove the window from the screen without
saving any changes.
.************************************************
.* GENERAL HELP - HELP ACTION BAR ITEM PULL DOWN
.************************************************
:h1 res=20409.Help for Graphic Sample
:p.
Use :hp2.Graphic:ehp2. to display GPI Routine and Metafiles.
:p.
.************************************************
.*  KEYS HELP - HELP ACTION BAR ITEM PULL DOWN
.************************************************
:h1 res=20410.Help for Keys
:p.
.*:dl compact break=all.
.*:dt.:hp2.Ctrl+D:ehp2.
.*:dd.Use to display the Dates window.
.*:dt.:hp2.Ctrl+L:ehp2.
.*:dd.Use to display or hide the Legend.
.*:edl.
:p.
Place information for the Help Keys menu item here.
:euserdoc.
