#===================================================================
#
#   DLLAPI.MAK - DLL Application Make file
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

HEADERS = dllapi.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

EXE_OBJS = $(OBJDIR)\dllapi.obj   $(OBJDIR)\dlapi_in.obj $(OBJDIR)\dlapi_hp.obj \
           $(OBJDIR)\dlapi_dg.obj

DLL_OBJS = $(OBJDIR)\dllib.obj

ALL_IPF = dllapi.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\dllapi.exe $(EXEDIR)\dllapi.hlp $(EXEDIR)\dllib.dll

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\dllapi.res: $(HEADERS) dllapi.rc dllapi.dlg dllapi.ico prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\dllapi.hlp:    $(ALL_IPF)

$(OBJDIR)\dllapi.obj:    dllapi.c    $(HEADERS)

$(OBJDIR)\dlapi_dg.obj:  dlapi_dg.c  $(HEADERS)

$(OBJDIR)\dlapi_in.obj:  dlapi_in.c  $(HEADERS)

$(OBJDIR)\dlapi_hp.obj:  dlapi_hp.c  $(HEADERS)

$(OBJDIR)\dllib.obj:     dllib.c     $(HEADERS)
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\dllapi.exe: $(EXE_OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(EXE_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

$(EXEDIR)\dllib.dll: $(DLL_OBJS) $(@B).def
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(MTLIBS) $(DLL_OBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
