#===================================================================
#
#   Timer0.MAK -  High Resolution Timer Application Make File
#   Copyright     IBM Corporation 1996
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

HEADERS = tmr0_ioc.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\timer0.obj

ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\timer0.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\timer0.obj:   timer0.c   $(HEADERS)

$(EXEDIR)\timer0.exe: $(OBJS) $(@B).def
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) @<<$(OBJDIR)\timer0.lnk
   $(OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe
