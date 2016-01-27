#===================================================================
#
#   RPCGSAMP.MAK - TCP/IP RPCGEN Sample Make file
#   Copyright      IBM Corporation 1996
#
#===================================================================
#===================================================================
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

TCPIPLIBS = so32dll.lib tcp32dll.lib rpc32dll.lib
CFLAGS    = $(CFLAGS) /DTCPV40HDRS /Ss /Gm

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

RGSOBJS = $(OBJDIR)\rgs.obj $(OBJDIR)\rgus.obj
RGCOBJS = $(OBJDIR)\rgc.obj $(OBJDIR)\rguc.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: genfiles $(EXEDIR)\rgc.exe $(EXEDIR)\rgs.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\rgs.obj:  rgs.c

$(OBJDIR)\rgus.obj: rgus.c

$(OBJDIR)\rgc.obj:  rgc.c

$(OBJDIR)\rguc.obj: rguc.c

$(EXEDIR)\rgc.exe: $(RGCOBJS)  rpcg.def rpcgsamp.mak
   -$(CREATE_PATH)
   $(LINK) $@ rpcg.def $(MTLIBS) + $(TCPIPLIBS) $(RGCOBJS)

$(EXEDIR)\rgs.exe: $(RGSOBJS)  rpcg.def rpcgsamp.mak
   -$(CREATE_PATH)
   $(LINK) $@ rpcg.def $(MTLIBS) + $(TCPIPLIBS) $(RGSOBJS)

genfiles:
   rpcgen rg.x

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
