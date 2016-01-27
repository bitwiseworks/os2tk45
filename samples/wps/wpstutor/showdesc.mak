#===================================================================
#
#   SHOWDESC.MAK - SHOWDESC Make file
#   Copyright  IBM Corporation 1995
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
#       SOMLIBS  - SOM Libraries
#===================================================================

!include ..\..\ibmsamp.inc

HEADERS = showdesc.h methodid.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\showdesc.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\showdesc.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\showdesc.res: showdesc.rc $(HEADERS) showdesc.ico
                $(RC) $(RCFLAGS) $(@B).rc $@

$(OBJDIR)\showdesc.obj: showdesc.c $(HEADERS)

$(EXEDIR)\showdesc.exe: $(OBJS) showdesc.def $(OBJDIR)\showdesc.res
   -$(CREATE_PATH)
   $(LINK) $@ showdesc.def $(MTLIBS) + $(SOMLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
