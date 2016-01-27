#--------------------------------------------------------------------------
#
#   Licensed Materials - Property of IBM
#   IBM TCP/IP for OS/2
#   (C) Copyright IBM Corporation. 1996-1998
#
#   All rights reserved.
#
#   US Government Users Restricted Rights -
#   Use, duplication or disclosure restricted by GSA ADP Schedule
#   Contract with IBM Corp.
#
#--------------------------------------------------------------------------
#
#  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
#  sample code created by IBM Corporation. This sample code is not
#  part of any standard or IBM product and is provided to you solely
#  for  the purpose of assisting you in the development of your
#  applications.  The code is provided "AS IS", without
#  warranty of any kind.  IBM shall not be liable for any damages
#  arising out of your use of the sample code, even if they have been
#  advised of the possibility of such damages.
#
#--------------------------------------------------------------------------
#
#   VACPP.MAK - TCP/IP ECHO Server sample makefile
#
#--------------------------------------------------------------------------
# Include the file ibmsamp.inc which defines the various macros
# depending on the target specified.
#
# The following macros are defined in ibmsamp.inc:
#
#       OBJDIR   - Directory where object files are created
#       EXEDIR   - Directory where executable files are created
#       RC       - Resource compiler
#       RCFLAGS  - Resource compiler flags
#       LINK     - Linker
#       LFLAGS   - Linker flags
#       MTLIBS   - Multithreaded versions of the runtime libraries
#===================================================================

!include ..\..\..\ibmsamp.inc

TCPIPLIBS = tcpip32.lib
CFLAGS    = $(CFLAGS) /DOS2 /Sm /Ss /Q  /W2 /Gm /Gt

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS1 = $(OBJDIR)\echosrv.obj

OBJS2 = $(OBJDIR)\echosmt.obj

OBJS3 = $(OBJDIR)\echostcp.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.
#-------------------------------------------------------------------
all: $(EXEDIR)\echosrv.exe $(EXEDIR)\echosmt.exe $(EXEDIR)\echostcp.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\echosmt.obj:     $(@B).c

$(OBJDIR)\echosrv.obj:     $(@B).c

$(OBJDIR)\echostcp.obj:    $(@B).c

$(EXEDIR)\echosrv.exe: $(OBJS1)  echo.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ echo.def $(MTLIBS) + $(TCPIPLIBS) $(OBJS1)

$(EXEDIR)\echosmt.exe: $(OBJS2)  echo.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ echo.def $(MTLIBS) + $(TCPIPLIBS) $(OBJS2)

$(EXEDIR)\echostcp.exe: $(OBJS3)  echo.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ echo.def $(MTLIBS) + $(TCPIPLIBS) $(OBJS3)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
