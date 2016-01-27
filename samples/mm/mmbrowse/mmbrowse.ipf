.*==============================================================*\
.*                                                              *
.* HELP.IPF - Information Tag Language file for the Help menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About MMBrowse
:i1 id=aboutMain.About MMBrowse
:p.
The MMBrowse application is a Presentation Manager application
which demonstrates the use of the Multimedia I/O subsystem (MMIO).
For more detail information about MMIO, please consult IBM Multimedia
Presentation Manager/2 Programming Reference.

:h1 res=3000 name=HELP_GET_FOURCC.Get FOURCC
:i1 id=GetFourCC.Get FOURCC
:p.
Every installed MMIO I/O Procedure must have a four character
code (FOURCC) associated with it. It must be at most four characters long.
For example, the OS/2 1.3 Bitmap IO Proc's FOURCC is OS13. If you are
unsure of what the FOURCC should be, please contact the author of
the IO Proc you are trying to install/remove.

:h1 res=3010 name=ID_HELP_VIEW_IMAGE.Open an image file
:i1 id=ViewImage.Open an image file
:p.
Use this choice to open an image file for viewing.

:h1 res=3011 name=ID_HELP_SAVING_IMAGE.Save an image file
:i1 id=SavingImage.Save an image file
:p.
Use this choice to save an image file. The file format of the
image depends on the image file extension,
and the default one is OS/2 2.0 bitmap.

:h1 res=3012 name=ID_HELP_INSTALL_IO_PROC.Install an I/O procedure
:i1 id=InstallI/OProc.Install an I/O procedure
:p.
Use this choice to install a multimedia I/O procedure.
For more detail information about MMIO, please consult IBM
Multimedia Presentation Manager/2 Programming Reference.
The default file extension for IOProc is DLL, and this sample
program requires entry point to an installed IOPROC to be
"IOProc_Entry".

:h1 res=3001 name=ID_HELP_REMOVE_PERM_LIST_BOX.Remove Permanent I/O Procedure(s)
:i1 id=RemPermIOProc.Remove Permanent I/O Procedure(s)
:p.
Click on any permanently installed I/O procedure(s) you wish to remove from
the INI file. After you're done, press OK to remove. You must exit the
MMBrowse application to have the removal of I/O procedures take effect.
Since this sample program requires an entry point to an installed IOPROC
to be "IOProc_Entry", the listed installed I/O procedures that can be removed
are those that can be reinstalled.

:h1 res=1004 name=QUERY_ALL_LIST_BOX_HELP.Query installed I/O Procedures
:i1 id=QueryIOProc.Query installed I/O Procedures
:p.
Use this choice for Querying an installed I/O procedure.
It provides the listing of installed I/O procedures for the users to select.
Click on any installed I/O procedures you wish to query.

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=2910 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Use the choices on the Help pull-down to display:
:dl tsize=5 break=all.
:dt.:hp2.Help index:ehp2.
:dd.Displays the index of help menu.
:dt.:hp2.General Help:ehp2.
:dd.Provides detailed information on the kinds of help available and how to
use help.
:dt.:hp2.Using Help:ehp2.
:dd.Displays some information about MMBrowse application.
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help for help... help panel                  *
.*      res = PANEL_HELPHELPFORHELP                             *
.*--------------------------------------------------------------*/
:h1 res=2920 name=PANEL_HELPUSINGHELP.Help For Help
:i2 refid=Help.Using Help
:p.Use this choice to obtain information on how to
use the Help facility.

.*--------------------------------------------------------------*\
.*  Help menu item Extended help... help panel                  *
.*      res = PANEL_HELPGENERAL                               *
.*--------------------------------------------------------------*/
:h1 res=2930 name=PANEL_HELPGENERAL.General Help
:i1 id=PANEL_HELPGENERAL.General Help
:i2 refid=Help.General Help
:p.
General help provides general information about the MMBrowse application.
The MMBROWSE program demonstrates the Multimedia  I/O concept by
serving as a utility program for viewing and saving an image file.
The program can also be used to install or remove temporary or permanent I/O procedures.
Although this program needs an entry point to an installed IOPROC to be "IOProc_Entry",
new I/O procedures can be added by using the techniques of this program.


.*--------------------------------------------------------------*\
.*  Help menu item Help index... help panel                     *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:h1 res=2950 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Index Help
:p.Use this choice to display the help index.
:p.The help index lists the titles of the help information that is available.

.*--------------------------------------------------------------*\
.*  Help menu item Product... help panel                        *
.*      res = PANEL_HELPPRODUCTINFO                             *
.*--------------------------------------------------------------*/
:h1 res=2960 name=PANEL_HELPPRODUCTINFO.Information
:i2 refid=Help.PRODUCT Information
:p.This dialog displays information about the application, such as version
number and copyright notice.

.*--------------------------------------------------------------*\
.*  Help menu item ProductOK... help panel                      *
.*      res = PANEL_PRODUCTINFO_OK                              *
.*--------------------------------------------------------------*/
:h1 res=3110 name=PANEL_PRODUCTINFO_OK.Information
:i2 refid=HELP.OK Information
:p.The OK button closed dialog box.

:euserdoc.
