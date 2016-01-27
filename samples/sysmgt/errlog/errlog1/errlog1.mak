#===================================================================
#
#   ERRLOG1.MAK - Error Logging Sample Make file
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
#       LFLIB    - Logging Framework libraries
#===================================================================

!include ..\..\..\ibmsamp.inc

CFLAGS=$(CFLAGS) /DNO_UNI_EAS /DUNI_NO_DOS
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\errlog1.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\errlog1.exe

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
$(OBJDIR)\errlog1.obj: errlog1.c

$(EXEDIR)\errlog1.exe: $(OBJS)
   -$(CREATE_PATH)
   $(LINK) $@ $(CFLAGS) $(MTLIBS) + $(LFLIBS) + $(ULSLIBS) $(OBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
