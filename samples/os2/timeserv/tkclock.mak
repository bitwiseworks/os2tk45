#===================================================================
#
#   TKCLOCK.MAK - Time Services Application Make file
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

HEADERS = clock.h clkdata.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\clock.obj   $(OBJDIR)\dialogs.obj   $(OBJDIR)\paint.obj  \
       $(OBJDIR)\wndproc.obj $(OBJDIR)\clkdata.obj   $(OBJDIR)\udtime.obj \
       $(OBJDIR)\digital.obj $(OBJDIR)\alarmthd.obj  $(OBJDIR)\help.obj

ALL_IPF = clock.ipf help.ipf dlg.ipf view.ipf settings.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\clock.exe $(EXEDIR)\clock.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\clock.res: $(HEADERS) clock.rc help.rc clock.ico clock.dlg \
                     product.dlg prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\clock.hlp:   $(ALL_IPF)

$(OBJDIR)\help.obj:    help.c    $(HEADERS)

$(OBJDIR)\clock.obj:   clock.c   $(HEADERS)

$(OBJDIR)\dialogs.obj: dialogs.c $(HEADERS)

$(EXEDIR)\clock.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) @<<$(OBJDIR)\tkclock.lnk
   $(OBJS)
<<
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
