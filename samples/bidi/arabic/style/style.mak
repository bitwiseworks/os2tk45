#===================================================================
#
#   STYLE.MAK -  Bidirectional Style Sample Make file
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

!include ..\..\..\ibmsamp.inc

HEADERS = sty_dlg.h sty_help.h sty_main.h sty_xtrn.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\sty_main.obj $(OBJDIR)\sty_user.obj $(OBJDIR)\sty_init.obj \
       $(OBJDIR)\sty_pnt.obj  $(OBJDIR)\sty_dlg.obj  $(OBJDIR)\sty_file.obj \
       $(OBJDIR)\sty_edit.obj $(OBJDIR)\sty_help.obj

ALL_IPF = style.ipf sty_file.ipf sty_edit.ipf sty_help.ipf sty_menu.ipf \
          sty_dlg.ipf
#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\style.exe $(EXEDIR)\style.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\sty_main.res: $(HEADERS)   sty_main.ico sty_main.rc  \
                        sty_main.dlg prodinfo.bmp sty_help.rc
                $(RC) $(RCFLAGS) $(@B).rc $@

prodinfo.bmp:
      copy ..\..\..\prodinfo.bmp $@

style.hlp: $(ALL_IPF)

$(OBJDIR)\sty_main.obj: sty_main.c $(HEADERS)

$(OBJDIR)\sty_file.obj: sty_file.c $(HEADERS)

$(OBJDIR)\sty_edit.obj: sty_edit.c $(HEADERS)

$(OBJDIR)\sty_user.obj: sty_user.c $(HEADERS)

$(OBJDIR)\sty_init.obj: sty_init.c $(HEADERS)

$(OBJDIR)\sty_pnt.obj: sty_pnt.c   $(HEADERS)

$(OBJDIR)\sty_dlg.obj: sty_dlg.c   $(HEADERS)

$(OBJDIR)\sty_help.obj: sty_help.c $(HEADERS)

$(EXEDIR)\style.exe: $(OBJS) $(@B).def $(OBJDIR)\sty_main.res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) + PMBIDI.LIB @<<$(OBJDIR)\$(@B).lnk
$(OBJS)
<<
   $(RC) -p -x $(OBJDIR)\sty_main.res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
