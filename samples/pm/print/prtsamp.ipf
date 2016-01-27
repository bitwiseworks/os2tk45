:userdoc.
.*
.* name: PRTSAMP.IPF
.*
.* description: input file for the program help facility compiler, IPFC.EXE,
.*   to define help panels for the print sample application
.*
.*
.*
.*
.*
.*
.*
.* GENERAL HELP PANEL
.* -----------------------------------------------------------------
:h1 res=30003 name=ID_PRTSAMP.Help for the Print Sample
:i1.print sample,getting started
:p.
Use :hp2.Print Sample:ehp2. to display and print
:link reftype=fn refid=metafile.metafiles:elink.,
:link reftype=fn refid=text.text:elink.,
and
:link reftype=fn refid=bitmap.bitmaps:elink..
:p.
To begin&colon.
:ol compact.
:li.Select :hp2.File:ehp2. then :hp2.Open:ehp2.
:li.When the window appears, select a file. The Print Sample can
display&colon.
:ul compact.
:li.:hp2.Metafiles:ehp2.
:li.:hp2.Text:ehp2.
:li.:hp2.Bitmaps:ehp2.
:eul.
:li.Select :hp2.OK:ehp2..
.*
:li.Select :hp2.File:ehp2. then :hp2.Printer Setup:ehp2. to select a
printer, form, orientation, resolution and other settings for your
printed output.
.*
:li.Select :hp2.File:ehp2. then :hp2.Print:ehp2. to begin printing.
:eol.
.*
.*
.*
.*
.*
.*  HELP FOR MENU HEADINGS
.* -----------------------------------------------------------------
:h1 res=30050 name=PANEL_MENUFILE.Help for File
:p.
Use :hp2.File:ehp2. menu to choose dialogs for opening an object,
setting up your printer, and printing.
.*
:h1 res=30051 name=PANEL_MENUOPTIONS.Help for Options
:p.
Use :hp2.Options:ehp2. menu for functions which let you change the
presentation of the object being displayed, such as the page or the font.
.*
:h1 res=30052 name=PANEL_MENUHELP.Help for Help
:p.
Use :hp2.Help:ehp2. menu to choose additional help information.
.*
.*
.*
.*  HELP FOR PRINTER SETUP
.* -----------------------------------------------------------------
:h1 res=30060 name=PANEL_QUERYPRINT.Help for Printing options
:p.
Select from the listbox the printer you want to use for printing.
:p.
Use the :hp2.Job properties...:ehp2. pushbutton to start a dialog
from which you can control your printing configuration.
.*
.*
.*  HELP FOR PRINT
.* -----------------------------------------------------------------
:h1 res=30070 name=PANEL_PRINTDLG.Help for Print
:p.
Enter the number of copies you wish to print in the entry field provided.
The default is one copy.
.*
.*
.*  HELP FOR OPEN ON FILE PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30000 name=PANEL_OPEN.Help for Open
:p.
Use :hp2.Open:ehp2. to select and load a file to print.
.*
.*
.*  HELP FOR PRINTER SETUP ON FILE PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30001 name=PANEL_SETUP.Help for Printer Setup
:p.
Use :hp2.Printer Setup:ehp2. to select a printer.
.*
.*  HELP FOR PRINT ON FILE PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30002 name=PANEL_PRINT.Help for Print
:p.
Use :hp2.Print:ehp2. to print your object.
.*
.*  HELP FOR SET FONT ON OPTIONS PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30011 name=PANEL_SETFONT.Help for Set font
:p.
Use the :hp2.Set Font:ehp2. dialog
to set or change the font used for text display
and printing.
.*
.*
.* Help for Font Dialog
.* -----------------------------------------------------------------
:h1 res=30030 name=PANEL_FONTDIALOG.Help for Font Selection
:p.Different fonts can be selected by clicking on the arrrow
in the Name box.  The available fonts will be displayed, and
any font selected by clicking on it will immediately appear in the
Sample display.
.*
.*  HELP FOR NEXT PAGE ON OPTIONS PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30012 name=PANEL_PAGEDOWN.Help for Next page
:p.
Use :hp2.Next page:ehp2. to display the next page of the object.
.*
.*  HELP FOR VIEW HALF SIZE ON OPTIONS PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30013 name=PANEL_VIEWHALF.Help for View half size
:p.
Use :hp2.View half size:ehp2. to display the object at half size.
.*
.*  HELP FOR VIEW FULL SIZE ON OPTIONS PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30014 name=PANEL_VIEWFULL.Help for View full size
:p.
Use :hp2.View full size:ehp2. to display the object at full size.
.*
.*  HELP FOR HELP INDEX ON HELP PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30020 name=PANEL_HELPINDEX.Help for Help index
:p.
Use :hp2.Help index:ehp2. to display the index of the sample's help panels.
.*
.*  HELP FOR GENERAL HELP ON HELP PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30021 name=PANEL_HELPGENERAL.Help for General help
:p.
Use :hp2.General help:ehp2. to display a description of the overall
purpose of the sample.
.*
.*  HELP FOR USING HELP ON HELP PULLDOWN MENU
.* -----------------------------------------------------------------
:h1 res=30022 name=PANEL_USINGHELP.Help for Using help
:p.
Use :hp2.Using help:ehp2. to display advice for using the help
facility.
.*
.*
.*
.*
.* HELP FOR STANDARD FILE OPEN DIALOG
.* -----------------------------------------------------------------
:h1 res=30031 name=PANEL_FILEDIALOG.Help for Open File
:ol compact.
:li.Select the down arrow to the right of the drive list to see all
the drives on your system.
:li.Select a drive from the drive list.
:li.Select a directory from the directory list.
:li.Select a file name from the file list, or type a file name.
:li.Select :hp2.OK:ehp2. to display the file.
:eol.
:p.
For a detailed explanation of each field, select from the
list below:
:ul compact.
:li.:link refid=STANDARD_OPEN_FILE_NAME reftype=hd.Open File Name:elink.
:li.:link refid=STANDARD_TYPE_OF_FILE reftype=hd.Type of File:elink.
:li.:link refid=STANDARD_DRIVE reftype=hd.Drive:elink.
:li.:link refid=STANDARD_FILE reftype=hd.File:elink.
:li.:link refid=STANDARD_DIRECTORY reftype=hd.Directory:elink.
:li.:link refid=STANDARD_OK  reftype=hd.OK Button:elink.
:li.:link refid=STANDARD_CANCEL reftype=hd.Cancel Button:elink.
:eul.
.*
.*
:h2 id=STANDARD_OPEN_FILE_NAME.Help for Open File Name
:p.Type the name of the file to be opened.  You can leave this
field blank and select a file name from the file list.
.*
.*
:h2 id=STANDARD_TYPE_OF_FILE.Help for Type of File
:p.Select the down arrow to the right of the :hp2.Type of File:ehp2.
field to display the available file types.  The editor sets this field
for all file types.
.*
.*
:h2 id=STANDARD_DRIVE.Help for Drive
:p.Use the down arrow to see the drives on your system.  Select the drive
that has the file you want to open.
.*
.*
:h2 id=STANDARD_FILE.Help for File
:p.Use :hp2.File:ehp2. to display all files matching the file name and
file type in the selected drive and directory.
.*
.*
:h2 id=STANDARD_DIRECTORY.Help for Directory
:p.Select :hp2.Directory:ehp2. to view the files within a specific :link
reftype=fn refid=directory.directory:elink. of your hard disk.
.*
.*
:h2 id=STANDARD_OK.Help for OK Button
:p.Select :hp2.OK:ehp2. to retrieve the file currently selected in the
file list.
.*
.*
:h2 id=STANDARD_CANCEL.Help for Cancel Button
:p.Select :hp2.Cancel:ehp2. to dismiss the file open dialog without
opening a new file.
.*
.*------------------------------------------------------------------
.*
.*
.*
.*  HELP FOR FILETYPE DIALOG THAT FOLLOWS THE FILE OPEN DIALOG
.* -----------------------------------------------------------------
:h1 res=30040 name=PANEL_FILETYPE.Help for File Type Dialog
:p.Select the type of file. Types include
:link reftype=fn refid=metafile.metafiles:elink.,
:link reftype=fn refid=text.text:elink.,
and
:link reftype=fn refid=bitmap.bitmap:elink..
:p.The Print Sample tries to identify the file type for you. If in doubt,
just press OK.
.*
.*
.*
.*  HELP FOR PAGE SETUP
.* -----------------------------------------------------------------
:h1 res=30080 name=PANEL_QUERYFORM.Help for Page setup
:p.
Select from the listbox the form you want to use.
:p.
Use the :hp2.Margins:ehp2. fields to set the margins you
want to use. The minimum margins allowed corresponds to the
hardware clip limits of the printer. The maximum margins are
the size of the form.
.*
.*
.*
.*
.*
.*
.*
.* GLOSSARY OF TERMS
.* -------------------------------------------------------------------
.*
:fn id=directory.
:p.
A table of identifiers and references to the corresponding items of
data.  For example, a listing of the files stored on a diskette.
:efn.
.*
.*
:fn id=bitmap.
:p.
A representation of an image by an array of bits.  The OS/2 logo
displayed by this operating system is an example of a bitmap.
:efn.
.*
.*
:fn id=metafile.
:p.
A set of drawing instructions. When the instructions are "played,"
the result is graphical picture. The Print Sample can show metafiles,
but the OS/2 Picture Viewer is especially designed for viewing metafiles.
:efn.
.*
.*
:fn id=text.
:p.
An ASCII text file. Use the OS/2 System Editor to prepare plain text files.
:efn.
.*
.*
:euserdoc.
