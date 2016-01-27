.*==============================================================*\
.*                                                              *
.* HELP.IPF - Information Tag Language file for the Help menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel
.*      res = PANEL_MAIN
.*--------------------------------------------------------------*/
:h1 res=4110 name=PANEL_MAIN.About EAS
:i1 id=aboutMain.About EAS
:p.  The EAS application is a sample Presentation Manager program
which demonstrates the use of the extended attributes editor. It consists
of procedures for dialog boxes that provide some information about
a file's attributes.   The EA code revolves around the
HoldFEA structure.  This structure is a linked list which contains
one EA per structure.  Each HoldFEA structure is dynamically allocated,
as are the two string pointer members of the structure, szName and aValue.
This is done to make handling the EAs easier and more flexible.

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=4120 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Use the choices on the Help pull-down to display:
:dl tsize=5 break=all.
:dt.:hp2.Help index:ehp2.
:dd.Displays the index of the help menu.
:dt.:hp2.General Help:ehp2.
:dd.Displays some information about the EAS sample program.
:dt.:hp2.Keys Help:ehp2.
:dd.Displays a list of keys usage.
:dt.:hp2.Using Help:ehp2.
:dd.Provides detailed information on the kind of help available and how to
use Help.
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help for help... help panel                  *
.*      res = PANEL_GENERALHELP                                 *
.*--------------------------------------------------------------*/
:h1 res=4130 name=PANEL_GENERALHELP.General Help
:i2 refid=Help.Help for Help
:p.Use this choice to get general help for the program. General help
provides general information about the EAS sample program.

.*--------------------------------------------------------------*\
.*  Help menu item Extended help... help panel                  *
.*      res = PANEL_USINGHELP                                   *
.*--------------------------------------------------------------*/
:h1 res=4140 name=PANEL_USINGHELP.Extended
:i2 refid=Help.Using Help
:p.Use this choice to obtain information on how to
use the Help facility.

.*--------------------------------------------------------------*\
.*  Help menu item Keys help... help panel                      *
.*      res = PANEL_TUTORIAL                                    *
.*--------------------------------------------------------------*/
:h1 res=4150 name=PANEL_TUTORIAL.Tutorial of Keys
:i2 refid=Help.Keys Help
:p.Use this choice to see a list of keys and a description of the
functions of the keys.

.*--------------------------------------------------------------*\
.*  Keys help panel                                             *
.*      res = PANEL_KEYSHELP                                    *
.*--------------------------------------------------------------*/
:h2 res=4180 name=PANEL_KEYSHELP.Key Assigments
:p.The list of keys is arranged in groups.
:p.:hp2.ACCELERATOR KEYS:ehp2.
:dl tsize=2 break=all.
:p.:hp2.HELP KEYS:ehp2.
:dl tsize=5 break=all.
:dt.F1
:dd.Get help
:dt.Esc
:dd.Previous Help Panel, or End help if only one panel
:dt.Alt
:dd.Switch IN/OUT action bar
:dt.Alt+F4
:dd.End help or End program
:edl.
:p.:hp2.SYSTEM KEYS:ehp2.
:dl tsize=5 break=all.
:dt.Alt+Esc
:dd.Switch to the next program, including full-screen programs
:dt.Ctrl+Esc
:dd.Switch to the Task List
:dt.Alt+k
:dd.Call key help
:dt.F3
:dd.Exit immediately
:edl.
:p.:hp2.WINDOW KEYS:ehp2.
:dl tsize=5 break=all.
:dt.Underlined letter
:dd.Move among the choices on the action bar and pull-down submenu or
run the function chosen
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help index... help panel                     *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:edl.
:h1 res=4160 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Help Index
:p.Use this choice to display the help index.
:p.The help index lists the titles of the help information topics that 
are available.

.*--------------------------------------------------------------*\
.*  Help menu item Product... help panel                        *
.*      res = PANEL_HELPPRODUCTINFO                             *
.*--------------------------------------------------------------*/
:h1 res=4170 name=PANEL_HELPPRODUCTINFO.Information
:i2 refid=Help.PRODUCT Information
:p.This dialog displays information about the application, such as version
number and copyright notice.

.*--------------------------------------------------------------*\
.*  Help menu item ProductOK... help panel                      *
.*      res = PANEL_PRODUCTINFO_OK                              *
.*--------------------------------------------------------------*/
:h1 res=4242 name=PANEL_PRODUCTINFO_OK.Information
:i2 refid=HELP.OK Information
:p.The OK button closes the dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item About... help panel                          *
.*      res = PANEL_ABOUTBOX                                    *
.*--------------------------------------------------------------*/
:h1 res=4210 name=PANEL_ABOUTBOX.Information
:i2 refid=HELP.ABOUT Information
:p.This dialog displays information about the application and an example
bitmap.

.*--------------------------------------------------------------*\
.*  Help menu item AboutOK... help panel                        *
.*      res = PANEL_ABOUTBOX_OK                                 *
.*--------------------------------------------------------------*/
:h1 res=4220 name=PANEL_ABOUTBOX_OK.Information
:i2 refid=HELP.ABOUTOK Information
:p.The OK button closes the dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item Open... help panel                           *
.*      res = PANEL_OPENHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=4190 name=PANEL_OPENHELP.Information
:i2 refid=HELP.OPEN Information
:p.This menu item will bring up an Open-File dialog that contains the
directory list and file list of the current directory. The user can select
one file to open.  If the current file status has been changed or edited,
it will display a warning before the next dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item OpenOK... help panel                         *
.*      res = PANEL_OPENBOX_OK                                  *
.*--------------------------------------------------------------*/
:h1 res=4227 name=PANEL_OPENBOX_OK.Information
:i2 refid=HELP.OPENOK Information
:p.The OK button closes the dialog box and displays the attributes of the selected
file.

.*--------------------------------------------------------------*\
.*  Help menu item Save... help panel                           *
.*      res = PANEL_SAVEHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=4191 name=PANEL_SAVEHELP.Information
:i2 refid=HELP.SAVE Information
:p.This menu item will save all extended attributes to the current file.

.*--------------------------------------------------------------*\
.*  Help menu item Add... help panel                            *
.*      res = PANEL_ADDHELP                                     *
.*--------------------------------------------------------------*/
:h1 res=4193 name=PANEL_ADDHELP.Information
:i2 refid=HELP.ADD Information
:p.This menu item will bring up an Add dialog box which contains an EA
name field and a data type list. This allows users to add a new attribute
to the current file.

.*--------------------------------------------------------------*\
.*  Help menu item AddOK... help panel                          *
.*      res = PANEL_ADDBOX_OK                                   *
.*--------------------------------------------------------------*/
:h1 res=4232 name=PANEL_ADDBOX_OK.Information
:i2 refid=HELP.ADDNOK Information
:p.The OK button will close the current dialog box and bring up an editing
dialog box which allows the user to edit the EA value.
:p.If the multi-type attribute is selected, the OK button will bring up a
Multi-Type dialog box which contains four buttons - ADD, EDIT, DELETE and
DONE. The usage of these buttons is the same as those on the menu bar.

.*--------------------------------------------------------------*\
.*  Help menu item Delete... help panel                         *
.*      res = PANEL_DELETEHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=4194 name=PANEL_DELETEHELP.Information
:i2 refid=HELP.DELETE Information
:p.This menu item will delete the selected attribute.

.*--------------------------------------------------------------*\
.*  Help menu item Edit... help panel                           *
.*      res = PANEL_EDITHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=4195 name=PANEL_EDITHELP.Information
:i2 refid=HELP.EDIT Information
:p.This menu item will bring up an editing dialog box which allows the user to
edit or change the EA value of the selected attribute.

.*--------------------------------------------------------------*\
.*  Help menu item MultiOK... help panel                        *
.*      res = PANEL_MULTIBOX_OK                                 *
.*--------------------------------------------------------------*/
:h1 res=4237 name=PANEL_MULTIBOX_OK.Information
:i2 refid=HELP.MULTIOK Information
:p.The ADD button will bring up an Add dialog box which contains the type list.
:p.The DELETE button will delete the selected attribute.
:p.The EDIT button will bring up an editing dialog box which allows the user
to edit or to change the value of the selected attribute.
:p.The DONE button will close the current dialog box.

:euserdoc.
