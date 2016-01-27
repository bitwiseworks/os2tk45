.*==============================================================*\
.*                                                              *
.* DLLAPI.IPF - Information Tag Language file for the Help menu *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel
.*      res = PANEL_MAIN
.*--------------------------------------------------------------*/
:h1 res=4110 name=PANEL_MAIN.About DLL
:i1 id=aboutMain.About DLL
:p.  The DLL application is a sample Presentation Manager application
which demonstrates the use of a dynamic linking library. It consists 
of a DLL file that provides a 32-bit API and an EXE sample program that
calls the API.  The DLL uses exception management to catch general
protection faults that occur when the API is called with invalid 
pointers. The EXE illustrates that it can't touch the protected data by
attempting to access it directly, causing an access fault. The EXE should
handle all faults it causes and print user messages to the screen.

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=4120 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Use the choices on the Help pull-down to display:
:dl tsize=5 break=all.
:dt.:hp2.Help index:ehp2.
:dd.Displays the index of help menu.
:dt.:hp2.General Help:ehp2.
:dd.Displays some information about DLLAPI sample program.
:dt.:hp2.Keys Help:ehp2.
:dd.Displays a list of keys usage.
:dt.:hp2.Using Help:ehp2.
:dd.Provides detailed information on the kinds of help available and how to
use help.
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help for help... help panel                  *
.*      res = PANEL_GENERALHELP                                 *
.*--------------------------------------------------------------*/
:h1 res=4130 name=PANEL_GENERALHELP.General Help
:i2 refid=Help.Help for Help
:p.Use this choice to get general help for the program. General help
provides general information about the DLLAPI sample program.

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
:edl.
:p.:hp2.WINDOW KEYS:ehp2.
:dl tsize=5 break=all.
:dt.Underlined letter
:dd.Move among the choices on the action bar and pull-downs submenu or
execute the function chosen
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help index... help panel                     *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:edl.
:h1 res=4160 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Help Index
:p.Use this choice to display the help index.
:p.The help index lists the titles of the help information that is available.

.*--------------------------------------------------------------*\
.*  Help menu item Product... help panel                        *
.*      res = PANEL_HELPPRODUCTINFO                             *
.*--------------------------------------------------------------*/
:h1 res=4170 name=PANEL_HELPPRODUCTINFO.Information
:i2 refid=Help.Information
:p.This dialog displays information about the application, such as version
number and copyright notice.

.*--------------------------------------------------------------*\
.*  Help menu item ProductOK... help panel                      *
.*      res = PANEL_PRODUCTINFO_OK                              *
.*--------------------------------------------------------------*/
:h1 res=4220 name=PANEL_PRODUCTINFO_OK.Information
:i2 refid=HELP.PRODUCTOK Information
:p.The OK button closes the dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item Search... help panel                         *
.*      res = PANEL_SEARCHHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=4190 name=PANEL_SEARCHHELP.Information
:i2 refid=HELP.SEARCH Information
:p.This menu item will bring up a File-Search dialog box which can read
the file path and searched pattern.

.*--------------------------------------------------------------*\
.*  Help menu item Open... help panel                           *
.*      res = PANEL_OPENHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=4191 name=PANEL_OPENHELP.Information
:i2 refid=HELP.OPEN Information
:p.This menu item will bring up a File-Open dialog box. User can input
the file name which will be opened and be read.

.*--------------------------------------------------------------*\
.*  Help menu item OpenOK... help panel                         *
.*      res = PANEL_OPENBOX_OK                                  *
.*--------------------------------------------------------------*/
:h1 res=4232 name=PANEL_OPENBOX_OK.Information
:i2 refid=HELP.OPENOK Information
:p.The OK button closes the dialog box and displays the next dialog box which
will display the first 200 bytes of the opened file.

.*--------------------------------------------------------------*\
.*  Help menu item Calculator... help panel                     *
.*      res = PANEL_CALCULHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=4192 name=PANEL_CALCULHELP.Information
:i2 refid=HELP.CALCULATOR Information
:p.This menu item will bring up a Calculator dialog box which contains the
operand and operation fields.

.*--------------------------------------------------------------*\
.*  Help menu item CalculatorOK... help panel                   *
.*      res = PANEL_CALCULBOX_OK                                *
.*--------------------------------------------------------------*/
:h1 res=4237 name=PANEL_CALCULBOX_OK.Information
:i2 refid=HELP.CALCULOK Information
:p.The OK button closes the dialog box and displays the next dialog box which
will display the result of the operation.

:euserdoc.
