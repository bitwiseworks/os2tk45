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
#   VACPP.MAK - TCP/IP Sockets sample makefile
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
CFLAGS    = $(CFLAGS) /DOS2 /Sm /Ss /Q  /W2 /Gm /Gt

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

UDPCOBJS   = $(OBJDIR)\udpc.obj
UDPSOBJS   = $(OBJDIR)\udps.obj
TCPCOBJS   = $(OBJDIR)\tcpc.obj
TCPSOBJS   = $(OBJDIR)\tcps.obj
SELECTOBJS = $(OBJDIR)\selects.obj
MSGSOBJS   = $(OBJDIR)\msgs.obj
MSGCOBJS   = $(OBJDIR)\msgc.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.
#-------------------------------------------------------------------
all: $(EXEDIR)\udpc.exe $(EXEDIR)\udps.exe    $(EXEDIR)\tcpc.exe \
     $(EXEDIR)\tcps.exe $(EXEDIR)\selects.exe $(EXEDIR)\msgs.exe \
     $(EXEDIR)\msgc.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\udpc.obj:    udpc.c

$(OBJDIR)\udps.obj:    udps.c

$(OBJDIR)\tcpc.obj:    tcpc.c

$(OBJDIR)\tcps.obj:    tcps.c

$(OBJDIR)\selects.obj: selects.c

$(OBJDIR)\msgs.exe:    msgs.c

$(OBJDIR)\msgc.exe:    msgc.c

$(EXEDIR)\udpc.exe: $(UDPCOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(UDPCOBJS)

$(EXEDIR)\udps.exe: $(UDPSOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(UDPSOBJS)

$(EXEDIR)\tcpc.exe: $(TCPCOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(TCPCOBJS)

$(EXEDIR)\tcps.exe: $(TCPSOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(TCPSOBJS)

$(EXEDIR)\selects.exe: $(SELECTOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(SELECTOBJS)

$(EXEDIR)\msgs.exe: $(MSGSOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(MSGSOBJS)

$(EXEDIR)\msgc.exe: $(MSGCOBJS)  sock.def vacpp.mak
   -$(CREATE_PATH)
   $(LINK) $@ sock.def $(MTLIBS) + $(TCPIPLIBS) $(MSGCOBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
