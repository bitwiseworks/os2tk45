#===================================================================
#
#   PRTSAMP.MAK - Printing Application Make file
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

HEADERS = pmassert.h prtsamp.h prtsdlg.h

ALL_IPF = prtsamp.ipf
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\prtsamp.obj  $(OBJDIR)\prtcreat.obj $(OBJDIR)\prtmenu.obj \
       $(OBJDIR)\prtprint.obj $(OBJDIR)\prtobj.obj   $(OBJDIR)\prtprof.obj \
       $(OBJDIR)\prtpage.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\prtsamp.exe $(EXEDIR)\prtsamp.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\prtsamp.res: $(HEADERS) prtsamp.rc prtsamp.ico prtsamp.dlg prodinfo.bmp
                $(RC) $(RCFLAGS) -D INCL_WINSTDDLGS $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\prtsamp.hlp: $(ALL_IPF)

$(OBJDIR)\prtcreat.obj: prtcreat.c  $(HEADERS)

$(OBJDIR)\prtmenu.obj:  prtmenu.c   $(HEADERS)

$(OBJDIR)\prtobj.obj:   prtobj.c    $(HEADERS)

$(OBJDIR)\prtpage.obj:  prtpage.c   $(HEADERS)

$(OBJDIR)\prtprof.obj:  prtprof.c   $(HEADERS)

$(OBJDIR)\prtprint.obj: prtprint.c  $(HEADERS)

$(OBJDIR)\prtsamp.obj: prtsamp.c    $(HEADERS)

prtsamp.def: prtsamp.mak
   @echo Creating .DEF file -- <<$(@B).def

NAME PRTSAMP WINDOWAPI

DESCRIPTION 'PM Print Sample Application'

CODE MOVEABLE
DATA MOVEABLE MULTIPLE

HEAPSIZE 18192
STACKSIZE 20384
EXPORTS
   WinProc
   ObjectWinProc
<<keep

$(EXEDIR)\prtsamp.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
