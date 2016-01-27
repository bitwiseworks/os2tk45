.*==============================================================*\
.*                                                              *
.* SEMAPH.IPF - Information Tag Language file for the Help menu *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel
.*      res = PANEL_MAIN
.*--------------------------------------------------------------*/
:h1 res=9110 name=PANEL_MAIN.About SEMAPH
:i1 id=aboutMain.About SEMAPH
:p.  The SEMAPH application is a sample Presentation Manager application
which demonstrates the use of Mutex, Event and Mux Semaphores.
In the application several threads are sharing access to the same
resource.  A mutex semaphore is used to guarantee that only one thread
has access to the resource at a time.  An event semaphore is used to
signal the thread to give up the resource.  The event semaphore can
by posted by the user or the application can be run in auto mode, in which
case the event semaphore will be posted at fixed time intervals.
A Mux semaphore is used to check for a stop event or a user signal to give
up the resource.  This application also serves as a template than can be
easily modified by an application developer.

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=9120 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Use the choices on the Help pull-down to display:
:dl tsize=5 break=all.
:dt.:hp2.Help index:ehp2.
:dd.Displays the index of help menu.
:dt.:hp2.General Help:ehp2.
:dd.Displays some information about Semaph sample program.
:dt.:hp2.Keys Help:ehp2.
:dd.Displays a list of keys usage.
:dt.:hp2.Using Help:ehp2.
:dd.Provides detailed information on the kinds of help available and how to
use help.
:edl.

.*--------------------------------------------------------------*\
.*  Help menu item Extended help... help panel                  *
.*      res = PANEL_GENERALHELP                                 *
.*--------------------------------------------------------------*/
:h1 res=9130 name=PANEL_GENERALHELP.General Help
:i2 refid=Help.Extended help
:p.Use this choice to get general help for the program. General help
provides general information about the Semaph sample program.
:p.Map above displays the number of hits for each thread.
:p.Squares below change color as are hit by a thread.

.*--------------------------------------------------------------*\
.*  Help menu item Help for help... help panel                  *
.*      res = PANEL_USINGHELP                                   *
.*--------------------------------------------------------------*/
:h1 res=9140 name=PANEL_USINGHELP.Extended
:i2 refid=Help.Help for Help
:p.Use this choice to obtain information on how to
use the Help facility.

.*--------------------------------------------------------------*\
.*  Help menu item Keys help... help panel                      *
.*      res = PANEL_TUTORIAL                                    *
.*--------------------------------------------------------------*/
:h1 res=9150 name=PANEL_TUTORIAL.Tutorial of Keys
:i2 refid=Help.Keys Help
:p.Use this choice to see a list of keys and a description of the
function of the keys.

.*--------------------------------------------------------------*\
.*  Keys help panel                                             *
.*      res = PANEL_KEYSHELP                                    *
.*--------------------------------------------------------------*/
:h2 res=9180 name=PANEL_KEYSHELP.Key Assigments
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
:h1 res=9160 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Help Index
:p.Use this choice to display the help index.
:p.The help index lists the titles of the help information that is available.

.*--------------------------------------------------------------*\
.*  Help menu item Product... help panel                        *
.*      res = PANEL_HELPPRODINFO                                *
.*--------------------------------------------------------------*/
:h1 res=9170 name=PANEL_HELPPRODINFO.Information
:i2 refid=Help.PRODUCTINFO Information
:p.This dialog displays information about the application, such as version
number and copyright notice.

.*--------------------------------------------------------------*\
.*  Help menu item ProductOK... help panel                      *
.*      res = PANEL_PRODUCTINFO_OK                              *
.*--------------------------------------------------------------*/
:h1 res=9211 name=PANEL_PRODUCTINFO_OK.Information
:i2 refid=HELP.PRODUCTOK Information
:p.The OK button closed dialog box.

.*--------------------------------------------------------------*\
.*  Help menu item NumberThread... help panel                   *
.*      res = PANEL_NTHRDSHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9181 name=PANEL_NTHRDSHELP.Information
:i2 refid=HELP.THREADS Information
:p.This menu item will bring up four choices that indicated the number
which thread will be created.

.*--------------------------------------------------------------*\
.*  Help menu item 2Thread... help panel                        *
.*      res = PANEL_THRDS2HELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9182 name=PANEL_THRDS2HELP.Information
:i2 refid=HELP.2THREADS Information
:p.This menu item indicated there are 2 threads will be created.

.*--------------------------------------------------------------*\
.*  Help menu item 2Thread... help panel                        *
.*      res = PANEL_THRDS4HELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9183 name=PANEL_THRDS4HELP.Information
:i2 refid=HELP.4THREADS Information
:p.This menu item indicated there are 4 threads will be created.

.*--------------------------------------------------------------*\
.*  Help menu item 8Thread... help panel                        *
.*      res = PANEL_THRDS8HELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9184 name=PANEL_THRDS8HELP.Information
:i2 refid=HELP.8THREADS Information
:p.This menu item indicated there are 8 threads will be created.

.*--------------------------------------------------------------*\
.*  Help menu item 16Thread... help panel                        *
.*      res = PANEL_THRDS16HELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9185 name=PANEL_THRDS16HELP.Information
:i2 refid=HELP.16THREADS Information
:p.This menu item indicated there are 16 threads will be created.

.*--------------------------------------------------------------*\
.*  Help menu item Start... help panel                          *
.*      res = PANEL_STARTHELP                                   *
.*--------------------------------------------------------------*/
:h1 res=9186 name=PANEL_STARTHELP.Information
:i2 refid=HELP.START Information
:p.This menu item starts the program.

.*--------------------------------------------------------------*\
.*  Help menu item AUTO Start... help panel                     *
.*      res = PANEL_AUTOHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=9187 name=PANEL_AUTOHELP.Information
:i2 refid=HELP.AUTO Information
:p.This menu item indicated the program will create semaphs and events
by program.

.*--------------------------------------------------------------*\
.*  Help menu item Manual Start... help panel                   *
.*      res = PANEL_MANUALHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9188 name=PANEL_MANUALHELP.Information
:i2 refid=HELP.MANUAL Information
:p.This menu item indicated the program will create semaphs and events
by user.

.*--------------------------------------------------------------*\
.*  Help menu item Time out... help panel                       *
.*      res = PANEL_TIMEOUTHELP                                 *
.*--------------------------------------------------------------*/
:h1 res=9189 name=PANEL_TIMEOUTHELP.Information
:i2 refid=HELP.TIMEOUT Information
:p.This menu item will bring up a dialog box which can let user change
minimum interval for posting event semaphore.

.*--------------------------------------------------------------*\
.*  Help menu item Stop... help panel                           *
.*      res = PANEL_STOPHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=9191 name=PANEL_STOPHELP.Information
:i2 refid=HELP.STOP Information
:p.This menu item will stop program running in automatic mode.

.*--------------------------------------------------------------*\
.*  Help menu item Resume automode... help panel                *
.*      res = PANEL_RESUMEHELP                                  *
.*--------------------------------------------------------------*/
:h1 res=9192 name=PANEL_RESUMEHELP.Information
:i2 refid=HELP.RESUME Information
:p.This menu item will resume program running in automatic mode.

.*--------------------------------------------------------------*\
.*  Help menu item Post... help panel                           *
.*      res = PANEL_POSTHELP                                    *
.*--------------------------------------------------------------*/
:h1 res=9193 name=PANEL_POSTHELP.Information
:i2 refid=HELP.POST Information
:p.This menu item will create and post an event and semaphore to a thread,
once each time.

:euserdoc.
