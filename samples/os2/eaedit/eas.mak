#===================================================================
#
#   EAS.MAK -  EAs Application Make file
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

HEADERS = eas.h

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\eas.obj      $(OBJDIR)\easea.obj   $(OBJDIR)\easfile.obj  \
       $(OBJDIR)\easinit.obj  $(OBJDIR)\eashelp.obj

ALL_IPF = eas.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\eas.exe $(EXEDIR)\eas.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\eas.res: eas.rc $(HEADERS) eas.ico eas.dlg prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\eas.hlp:     $(ALL_IPF)

$(OBJDIR)\eas.obj:     eas.c     $(HEADERS)

$(OBJDIR)\easea.obj:   easea.c   $(HEADERS)

$(OBJDIR)\easfile.obj: easfile.c $(HEADERS)

$(OBJDIR)\eashelp.obj: eashelp.c $(HEADERS)

$(OBJDIR)\wrmhelp.obj: wrmhelp.c $(HEADERS)

$(EXEDIR)\eas.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
