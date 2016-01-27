#===================================================================
#
#   WORMS.MAK - Console IO Make file
#   Copyright  IBM Corporation 1996
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

HEADERS = worms.h wrmthrd.h

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\worms.obj $(OBJDIR)\wrmthrd.obj $(OBJDIR)\init.obj  \
       $(OBJDIR)\disp.obj  $(OBJDIR)\wrmhelp.obj $(OBJDIR)\wmouse.obj

ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\worms.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\worms.obj:   worms.c $(HEADERS)

$(OBJDIR)\wrmthrd.obj: wrmthrd.c $(HEADERS)

$(OBJDIR)\init.obj:    init.c    $(HEADERS)

$(OBJDIR)\disp.obj:    disp.c    $(HEADERS)

$(OBJDIR)\wrmhelp.obj: wrmhelp.c $(HEADERS)

$(OBJDIR)\wmouse.obj:  wmouse.c  $(HEADERS)

$(EXEDIR)\worms.exe: $(OBJS) worms.def
   -$(CREATE_PATH)
   $(LINK) $@ worms.def $(MTLIBS) $(OBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
