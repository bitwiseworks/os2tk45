#===================================================================
#
#   REXXCAL4.MAK - PM REXX Calculator Sample Makefile
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
#       REXXLIBS - REXX libraries
#===================================================================

!include ..\..\..\ibmsamp.inc

HEADERS = rexxcal4.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\rexxcal4.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\rexxcal4.exe

#-------------------------------------------------------------------
#   This section creates the command file used by the linker.  This
#   command file is recreated automatically every time you change
#   the object file list, linker flags, or library list.
#-------------------------------------------------------------------
#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\rexxcal4.res: rexxcal4.rc $(HEADERS)
                $(RC) $(RCFLAGS) $(@B).rc $@

$(OBJDIR)\rexxcal4.obj: rexxcal4.c $(HEADERS)

$(EXEDIR)\rexxcal4.exe: $(OBJS) rexxcal4.def $(OBJDIR)\rexxcal4.res
   -$(CREATE_PATH)
   $(LINK) $@ rexxcal4.def $(MTLIBS) + $(REXXLIBS) $(OBJS)
   $(RC) $(OBJDIR)\rexxcal4.res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
