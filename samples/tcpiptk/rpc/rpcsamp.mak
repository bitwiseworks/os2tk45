#===================================================================
#
#   RPCSAMP.MAK -  TCP/IP RPC Sample Make file
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
CFLAGS    = $(CFLAGS) /DTCPV40HDRS /Sm /Ss /Q  /W2 /Gm /Gt /Mp

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

GENESERVOBJS = $(OBJDIR)\geneserv.obj
GENESENDOBJS = $(OBJDIR)\genesend.obj
RAWEXOBJS    = $(OBJDIR)\rawex.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\geneserv.exe $(EXEDIR)\genesend.exe $(EXEDIR)\rawex.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\geneserv.obj: geneserv.c

$(OBJDIR)\genesend.obj: genesend.c

$(OBJDIR)\rawex.obj:    rawex.c

$(EXEDIR)\genesend.exe: $(GENESENDOBJS)  rpc.def rpcsamp.mak
   -$(CREATE_PATH)
   $(LINK) $@ rpc.def $(MTLIBS) + $(TCPIPLIBS) $(GENESENDOBJS)

$(EXEDIR)\geneserv.exe: $(GENESERVOBJS)  rpc.def rpcsamp.mak
   -$(CREATE_PATH)
   $(LINK) $@ rpc.def $(MTLIBS) + $(TCPIPLIBS) $(GENESERVOBJS)

$(EXEDIR)\rawex.exe:    $(RAWEXOBJS)     rpc.def rpcsamp.mak
   -$(CREATE_PATH)
   $(LINK) $@ rpc.def $(MTLIBS) + $(TCPIPLIBS) $(RAWEXOBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
