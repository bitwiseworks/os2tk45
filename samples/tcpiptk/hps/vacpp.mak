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
#   VACPP.MAK - TCP/IP Simple ECHO client/server sample makefile
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

!include ..\..\ibmsamp.inc

TCPIPLIBS = tcpip32.lib
CFLAGS=$(CFLAGS) /Ss  /Wall+ppt-uni-ppc-got-trd-ext-
STACKSIZE=20000
HFILES  = cliserv.h

!if "$(DEBUG)" != ""
CFLAGS=/Ti /DDEBUG $(CFLAGS)
!endif

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

ECHOCOBJS = $(OBJDIR)\echoc.obj
ECHOSOBJS = $(OBJDIR)\echos.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.
#-------------------------------------------------------------------
all: $(EXEDIR)\echoc.exe $(EXEDIR)\echos.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\echoc.obj: echoc.c

$(OBJDIR)\echos.obj: echos.c


$(EXEDIR)\echoc.exe: $(ECHOCOBJS) makefile
   -$(CREATE_PATH)
   $(LINK) $@ hps.def $(MTLIBS) + $(TCPIPLIBS) $(ECHOCOBJS)

$(EXEDIR)\echos.exe: $(ECHOSOBJS)  makefile
   -$(CREATE_PATH)
   $(LINK) $@ hps.def $(MTLIBS) + $(TCPIPLIBS) $(ECHOSOBJS)


clean:
          del *.obj *.exe

