#===================================================================
#
#   VMM.MAK -    VMM Application Make file
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

HEADERS = vmm.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\vmm_main.obj $(OBJDIR)\vmm_init.obj $(OBJDIR)\vmm_dlg.obj \
       $(OBJDIR)\vmm_user.obj $(OBJDIR)\vmm_help.obj

ALL_IPF = vmm.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\vmm.exe $(EXEDIR)\vmm.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\vmm.res: $(HEADERS) vmm.dlg vmm.ico vmm.rc prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\vmm.hlp:      $(ALL_IPF)

$(OBJDIR)\vmm_main.obj: vmm_main.c $(HEADERS)

$(OBJDIR)\vmm_user.obj: vmm_user.c $(HEADERS)

$(OBJDIR)\vmm_init.obj: vmm_init.c $(HEADERS)

$(OBJDIR)\vmm_dlg.obj:  vmm_dlg.c  $(HEADERS)

$(OBJDIR)\vmm_help.obj: vmm_help.c $(HEADERS)

$(EXEDIR)\vmm.exe: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
