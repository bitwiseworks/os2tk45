#===================================================================
#
#   MAKEFILE  -  TCP/IP SENDFILE Sample Make file
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
CFLAGS    = $(CFLAGS) /Sm /Ss /Q /W2 /Gm /Gt

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

RECVOBJS = $(OBJDIR)\recv.obj
SENDFILEOBJS = $(OBJDIR)\sendfile.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.  The
#   makefile builds the executable
#-------------------------------------------------------------------
all: $(EXEDIR)\recv.exe $(EXEDIR)\sendfile.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\recv.obj: recv.c

$(OBJDIR)\sendfile.obj: sendfile.c

$(EXEDIR)\recv.exe: $(RECVOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(MTLIBS) + $(TCPIPLIBS) $(RECVOBJS)

$(EXEDIR)\sendfile.exe: $(SENDFILEOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(MTLIBS) + $(TCPIPLIBS) $(SENDFILEOBJS)


clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe

