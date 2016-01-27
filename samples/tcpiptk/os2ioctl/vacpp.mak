#===================================================================
#
#   MAKEFILE  -  TCP/IP OS2IOCTL Sample Make file
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

OS2IOCTLOBJS = $(OBJDIR)\os2ioctl.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.  The
#   makefile builds the executable
#-------------------------------------------------------------------
all: $(EXEDIR)\os2ioctl.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\os2ioctl.obj: os2ioctl.c

$(EXEDIR)\os2ioctl.exe: $(OS2IOCTLOBJS)
   -$(CREATE_PATH)
   $(LINK) $@ + $(MTLIBS) + $(TCPIPLIBS) $(OS2IOCTLOBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe

