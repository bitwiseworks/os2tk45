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
:h1 res=2100 name=PANEL_MAIN.About Hanoi
:i1 id=aboutMain.About Hanoi
:p.  The Hanoi application is a sample Presentation Manager application
which demonstrates the use of a multithreaded program. The main thread
handles the PM interface, the second thread is started to do the
recursive execution of the Hanoi algorithm.

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
:dd.Displays some information about Hanoi sample program.
:dt.:hp2.Keys Help:ehp2.
:dd.Displays a list of keys usage.
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help for help... help panel                  *
.*      res = PANEL_HELPHELPFORHELP                             *
.*--------------------------------------------------------------*/
:h1 res=2920 name=PANEL_HELPHELPFORHELP.General Help
:i2 refid=Help.Help for Help
:p.Use this choice to obtain information on how to
use the Help facility.

.*--------------------------------------------------------------*\
.*  Help menu item Extended help... help panel                  *
.*      res = PANEL_HELPEXTENDED                                *
.*--------------------------------------------------------------*/
:h1 res=2930 name=PANEL_HELPEXTENDED.Extended
:i2 refid=Help.Using Help
:p.Use this choice to get general help for the program. General help provides
general information about the Hanoi sample program.

.*--------------------------------------------------------------*\
.*  Help menu item Keys help... help panel                      *
.*      res = PANEL_HELPKEYS                                    *
.*--------------------------------------------------------------*/
:h1 res=2940 name=PANEL_HELPKEYS.Tutorial of Keys
:i2 refid=Help.Keys Help
:p.Use this choice to see a list of keys and a description of the function of
the keys.

.*--------------------------------------------------------------*\
.*  Keys help panel                                             *
.*      res = PANEL_KEYSHELP                                    *
.*--------------------------------------------------------------*/
:h2 res=2970 name=PANEL_KEYSHELP.Key Assigments
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
:dd.Switch TO/OUT action bar
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
:dt.Alt+Underlined letter
:dd.Execute the function chosen immediately
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Help index... help panel                     *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:edl.
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

.*--------------------------------------------------------------*\
.*  Help menu item Start... help panel                          *
.*      res = PANEL_HELPSTART                                   *
.*--------------------------------------------------------------*/
:h1 res=2980 name=PANEL_HELPSTART.Information
:i2 refid=HELP.START Information
:p.This key will start to move the disk.

.*--------------------------------------------------------------*\
.*  Help menu item Stop... help panel                           *
.*      res = PANEL_HELPSTOP                                    *
.*--------------------------------------------------------------*/
:h1 res=2981 name=PANEL_HELPSTOP.Information
:i2 refid=HELP.STOP Information
:p.This key will stop the moving of disk.

.*--------------------------------------------------------------*\
.*  Help menu item Set... help panel                            *
.*      res = PANEL_HELPSET                                     *
.*--------------------------------------------------------------*/
:h1 res=2982 name=PANEL_HELPSET.Information
:i2 refid=HELP.SET Information
:p.This dialog displays the disk number which you can reset.

.*--------------------------------------------------------------*\
.*  Help menu item On... help panel                             *
.*      res = PANEL_HELPON                                      *
.*--------------------------------------------------------------*/
:h1 res=2983 name=PANEL_HELPON.Information
:i2 refid=HELP.VIEWON Information
:p.This menu item will enable the information area.

.*--------------------------------------------------------------*\
.*  Help menu item Off... help panel                            *
.*      res = PANEL_HELPOFF                                     *
.*--------------------------------------------------------------*/
:h1 res=2984 name=PANEL_HELPOFF.Information
:i2 refid=HELP.VIEWOFF Information
:p.This menu item will disable the information area.

:euserdoc.
