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
:h1 res=4110 name=PANEL_MAIN.About VMM
:i1 id=aboutMain.About VMM
:p.  The VMM application is a sample Presentation Manager application
which demonstrates the use of virtual memory management.

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
:dd.Displays some information about VMM sample program.
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
provides general information about the VMM sample program.

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
function of the keys.

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
execut the function chosen
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
:i2 refid=HELP.OK Information
:p.The OK button closed dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item Alloc... help panel                          *
.*      res = PANEL_HELPALLOC                                   *
.*--------------------------------------------------------------*/
:h1 res=4190 name=PANEL_HELPALLOC.Information
:i2 refid=HELP.ALLOC Information
:p.This key will display a memory-allocate dialog box, which you can
select attributes and size of allocated memory.

.*--------------------------------------------------------------*\
.*  Help menu item AllocOK... help panel                        *
.*      res = PANEL_ALLOCBOX_OK                                 *
.*--------------------------------------------------------------*/
:h1 res=4232 name=PANEL_ALLOCBOX_OK.Information
:i2 refid=HELP.ALLOCOK Information
:p.The OK button closed dialog box and allocated the virtual memory.

.*--------------------------------------------------------------*\
.*  Help menu item Free... help panel                           *
.*      res = PANEL_HELPFREE                                    *
.*--------------------------------------------------------------*/
:h1 res=4191 name=PANEL_HELPFREE.Information
:i2 refid=HELP.FREE Information
:p.This key will display a free-memory dialog box, which you can free the
allocated memory by typing the address.

.*--------------------------------------------------------------*\
.*  Help menu item Set... help panel                            *
.*      res = PANEL_HELPSET                                     *
.*--------------------------------------------------------------*/
:h1 res=4192 name=PANEL_HELPSET.Information
:i2 refid=HELP.SET Information
:p.This key will display a set-attributes dialog box, which you can change
the selected memory attributes by typing the address.

.*--------------------------------------------------------------*\
.*  Help menu item SetOK... help panel                          *
.*      res = PANEL_SETBOX_OK                                   *
.*--------------------------------------------------------------*/
:h1 res=4237 name=PANEL_SETBOX_OK.Information
:i2 refid=HELP.SETOK Information
:p.The OK button closed dialog box and display another dialog box which
contains memory attributes and access table.

.*--------------------------------------------------------------*\
.*  Help menu item Write... help panel                          *
.*      res = PANEL_HELPWRITE                                   *
.*--------------------------------------------------------------*/
:h1 res=4193 name=PANEL_HELPWRITE.Information
:i2 refid=HELP.WRITE Information
:p.This key will display a write-to-memory dialog box, which you can type
the memory address that you attempt to write to.

.*--------------------------------------------------------------*\
.*  Help menu item Read... help panel                           *
.*      res = PANEL_HELPREAD                                    *
.*--------------------------------------------------------------*/
:h1 res=4194 name=PANEL_HELPREAD.Information
:i2 refid=HELP.READ Information
:p.This key will display a read-from-memory dialog box, which you can type
the memory address that you attempt to read from.

:euserdoc.
