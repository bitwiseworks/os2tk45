#===================================================================
#
#   SEMAPH.MAK - Semaphore Application Make file
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

HEADERS = semaph.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\sem_main.obj $(OBJDIR)\sem_user.obj $(OBJDIR)\sem_init.obj  \
       $(OBJDIR)\sem_pnt.obj  $(OBJDIR)\semaph.obj   $(OBJDIR)\sem_help.obj  \
       $(OBJDIR)\sem_dlg.obj

ALL_IPF = semaph.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\semaph.exe $(EXEDIR)\semaph.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\semaph.res: $(HEADERS) semaph.dlg semaph.ico prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\semaph.hlp:   $(ALL_IPF)

$(OBJDIR)\sem_main.obj: sem_main.c $(HEADERS)

$(OBJDIR)\sem_edit.obj: sem_edit.c $(HEADERS)

$(OBJDIR)\sem_user.obj: sem_user.c $(HEADERS)

$(OBJDIR)\semaph.obj:   semaph.c   $(HEADERS)

$(OBJDIR)\sem_help.obj: sem_help.c $(HEADERS)

$(OBJDIR)\sem_init.obj: sem_init.c $(HEADERS)

$(OBJDIR)\sem_pnt.obj:  sem_pnt.c  $(HEADERS)

$(OBJDIR)\sem_dlg.obj:  sem_dlg.c  $(HEADERS)


$(EXEDIR)\semaph.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) @<<semaph.lrf
$(OBJS)
<<
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
