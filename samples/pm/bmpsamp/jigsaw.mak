#===================================================================
#
#   JIGSAW.MAK - Jigsaw Puzzle Application Make file
#   Copyright    IBM Corporation 1996
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

HEADERS = globals.h jigsaw.h jighelp.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\jigsaw.obj $(OBJDIR)\jighelp.obj $(OBJDIR)\procs.obj \
       $(OBJDIR)\misc.obj   $(OBJDIR)\globals.obj

ALL_IPF = jigsaw.ipf
#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\jigsaw.exe $(EXEDIR)\jigsaw.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\jigsaw.res: $(HEADERS) prodinfo.bmp jigsaw.rc jigsaw.ico jighelp.rc jigsaw.dlg
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\jigsaw.hlp:  $(ALL_IPF)

$(OBJDIR)\jigsaw.obj:  jigsaw.c  $(HEADERS)

$(OBJDIR)\jighelp.obj: jighelp.c $(HEADERS)

$(OBJDIR)\procs.obj:   procs.c   $(HEADERS)

$(OBJDIR)\misc.obj:    misc.c    $(HEADERS)

$(OBJDIR)\globals.obj: globals.c $(HEADERS)

$(EXEDIR)\jigsaw.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
