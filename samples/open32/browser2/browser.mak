#===================================================================
#
#   BROWSER.MAK - Browser Application Make file
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
#       DAPIELIBS- DAPIE Libraries
#===================================================================

!include ..\..\ibmsamp.inc

HEADERS = browser.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------
OBJS = $(OBJDIR)\browser.obj $(OBJDIR)\main.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\browser.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\browser.res: $(HEADERS) browser.rc
                $(RC) $(RCFLAGS) $(@B).rc $@

$(OBJDIR)\browser.obj: browser.c $(HEADERS)

$(OBJDIR)\main.obj:    main.c $(HEADERS)

$(EXEDIR)\browser.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(DAPIELIBS) + $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
