.*/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
.*/*³                                                                          ³*/
.*/*³ PROGRAM NAME: PALETTE                                                    ³*/
.*/*³ -------------                                                            ³*/
.*/*³  This file defines the help panels for the palette sample program.       ³*/
.*/*³  These panels are defined using the Information Presentation Language    ³*/
.*/*³  markup tags.  This file is compiled with the IPFC conversion tool to    ³*/
.*/*³  create the .hlp file which becomes the help library file for the        ³*/
.*/*³  program.                                                                ³*/
.*/*³                                                                          ³*/
.*/*³ COPYRIGHT:                                                               ³*/
.*/*³ ----------                                                               ³*/
.*/*³  Copyright (C) International Business Machines Corp., 1992               ³*/
.*/*³                                                                          ³*/
.*/*³ DISCLAIMER OF WARRANTIES:                                                ³*/
.*/*³ -------------------------                                                ³*/
.*/*³  The following [enclosed] code is sample code created by IBM Corporation.³*/
.*/*³  This sample code is not part of any standard IBM product and is provided³*/
.*/*³  to you solely for the purpose of assisting you in the development of    ³*/
.*/*³  your applications.  The code is provided "AS IS", without warranty of   ³*/
.*/*³  any kind.  IBM shall not be liable for any damages arising out of your  ³*/
.*/*³  use of the sample code, even if they have been advised of the           ³*/
.*/*³  possibility of such damages.                                            ³*/
.*/*³                                                                          ³*/
.*/*³ For details on what this program does etc., please see the PMLINES.C     ³*/
.*/*³ file.                                                                    ³*/
.*/*³                                                                          ³*/
.*/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
.*
.*/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
.*/*³ PALETTE.IPF                                                              ³*/
.*/*³                                                                          ³*/
.*/*³ Palette Sample documentation (IPF) file                                  ³*/
.*/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
.*
:userdoc.
:title.Palette Manager
:body.
.*--------------------------------------------------------
:h1 res=503 . General Help
:p.This program is a sample program that demonstrates the Palette Manager.
Help is available any time you press the F1 key
from anywhere within the program.  Because this is context sensitive help,
the help information that is displayed depends on where in the program F1
was selected.
:p.For instance, you receive help for the :hp2.Options:ehp2. pull-down
on the action bar when you highlight :hp2.Options:ehp2. and press F1.
However, this is not the same help that is displayed when you press F1
while the :hp2.Speed:ehp2. choice in the :hp2.Options:ehp2. pull-down
is highlighted.
:p.Press Esc at any time to view the previous help window or to return to
the main Help window if there are no other help windows.
.*
.*--------------------------------------------------------
:h1 res=100 . Help for Options
:p.Use the :hp2.Options:ehp2. pull-down choices to select the speed of
animation and/or what will be displayed in the window.
.*
.*--------------------------------------------------------
:h2 res=101 . Help for Start/Stop
:p.Use this switch to start or stop palette animation.
.*
.*--------------------------------------------------------
:h2 res=102 . Help for Speed
:p.Use this submenu to set the speed of the animation.
.*
.*--------------------------------------------------------
:h3 res=103 . Help for Slow Speed
:p.Use this choice to slow the animation down.
.*
.*--------------------------------------------------------
:h3 res=104 . Help for Medium Speed
:p.Use this choice to restore the animation speed.
.*
.*--------------------------------------------------------
:h3 res=105 . Help for Fast Speed
:p.Use this choice to speed up the animation.
.*
.*--------------------------------------------------------
:h2 res=106 . Help for Logo Selection
:p.Use this submenu to select which logo to display.
.*
.*--------------------------------------------------------
:h3 res=107 . Help for IBM Logo
:p.Use this choice to display the IBM logo.
.*
.*--------------------------------------------------------
:h3 res=108 . Help for OS/2 Logo
:p.Use this choice to display the OS/2 logo.
.*
.*--------------------------------------------------------
:h1 res=200 . The Help Menu
:p.
The help menu contains choices for fast access to special sections of this
help information. The following choices are available:
:ul compact.
:li.
:link reftype=hd res=203 .
Help index...
:elink.
:li.
:link reftype=hd res=503 .
General help...
:elink.
:li.
:link reftype=hd res=204 .
Keys help...
:elink.
:li.
:link reftype=hd res=201 .
Using help...
:elink.
:li.
:link reftype=hd res=202 .
Product information...
:elink.
:eul.
.*--------------------------------------------------------
:h1 res=203 . Index to the Help Topics
:p.
Displays the index to the help topics
.*
.*--------------------------------------------------------
:h1 res=202 . About Palette Manager
:p.
This choice shows a panel with the samples&apos.s copyright, its
version and name.
.*
.*--------------------------------------------------------
:h1 res=201 . Using Help
:p.
Information on how to use help
.*
.*--------------------------------------------------------
:h1 res=204 . Help for Keys
:p.
:dl.
:dt.F1
:dd.Accelerator for :hp3.Get help about this program:ehp3.
:dt.F3
:dd.Accelerator for :hp3.Exit this program:ehp3.
:edl.
.*
:euserdoc.
.*--- end of palette.ipf ---------------------------------
