#===================================================================
#
#   MAKEFILE  -  TCP/IP SYSCTL Sample Make file
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

TCPIPLIBS = tcpip32.lib $(LIBS)
CFLAGS    = $(CFLAGS) /Sm /Ss /Q  /W2 /Gm /Gt

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

SYSCTLOBJS = $(OBJDIR)\sysctl.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable
#-------------------------------------------------------------------
all: $(EXEDIR)\sysctl.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\sysctl.obj: sysctl.c


$(EXEDIR)\sysctl.exe: $(SYSCTLOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(TCPIPLIBS) $(SYSCTLOBJS)


clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe

