#===================================================================
#
#   TEMPLATE.MAK - Template Application Make file
#   Copyright      IBM Corporation 1996
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

HEADERS = main.h xtrn.h help.c
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\main.obj $(OBJDIR)\user.obj $(OBJDIR)\init.obj \
       $(OBJDIR)\pnt.obj  $(OBJDIR)\dlg.obj  $(OBJDIR)\help.obj \
       $(OBJDIR)\file.obj $(OBJDIR)\edit.obj $(OBJDIR)\thrd.obj

ALL_IPF = template.ipf file.ipf edit.ipf help.ipf dlg.ipf menu.ipf
#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\template.exe $(EXEDIR)\template.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\template.res: $(HEADERS) main.rc main.ico template.dlg help.rc prodinfo.bmp
                $(RC) $(RCFLAGS) main.rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\template.hlp: $(ALL_IPF)

$(OBJDIR)\main.obj:     main.c $(HEADERS)

$(OBJDIR)\file.obj:     file.c $(HEADERS)

$(OBJDIR)\edit.obj:     edit.c $(HEADERS)

$(OBJDIR)\user.obj:     user.c $(HEADERS)

$(OBJDIR)\init.obj:     init.c $(HEADERS)

$(OBJDIR)\pnt.obj:      pnt.c  $(HEADERS)

$(OBJDIR)\dlg.obj:      dlg.c  $(HEADERS)

$(OBJDIR)\help.obj:     help.c $(HEADERS)

$(OBJDIR)\thrd.obj:     thrd.c $(HEADERS)

$(EXEDIR)\template.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) @<<$(OBJDIR)\template.lnk
   $(OBJS)
<<
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
