#===================================================================
#
#   MAKEFILE  -  TCP/IP ACCXRECV Sample Make file
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
TCPIPLIBS = tcpip32.lib
CFLAGS    = $(CFLAGS) /Mp /Sm /Ss /Q /W2 /Gm /Gt

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

ACCEPTOBJS = $(OBJDIR)\accept.obj
RECVOBJS = $(OBJDIR)\recv.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.  The
#   makefile builds the executable
#-------------------------------------------------------------------
all: $(EXEDIR)\accept.exe $(EXEDIR)\recv.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\accept.obj: accept.c

$(OBJDIR)\recv.obj: recv.c

$(EXEDIR)\accept.exe: $(ACCEPTOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(MTLIBS) + $(TCPIPLIBS) $(ACCEPTOBJS)

$(EXEDIR)\recv.exe: $(RECVOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(MTLIBS) + $(TCPIPLIBS) $(RECVOBJS)


clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe

