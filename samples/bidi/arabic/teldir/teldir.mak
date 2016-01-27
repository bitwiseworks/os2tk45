#===================================================================
#
#   TELDIR.MAK - Bidirectional Telephone Direcotory Sample Make file
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

HEADERS = teldir.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\teldir.obj

ALL_IPF = teldir.ipf teldira.ipf
#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\teldir.exe $(EXEDIR)\teldir.hlp $(EXEDIR)\teldira.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\teldir.res: $(HEADERS) teldir.rc
                $(RC) $(RCFLAGS) $(@B).rc $@

$(EXEDIR)\teldir.hlp:  $(ALL_IPF)

$(EXEDIR)\teldira.hlp: $(ALL_IPF)
   ipfc $(@B).ipf $@ /W3 /COUNTRY=785 /CODEPAGE=864

$(OBJDIR)\teldir.obj:  teldir.c $(HEADERS)

$(EXEDIR)\teldir.exe: $(OBJS) $(@B).def $(OBJDIR)\teldir.res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) + PMBIDI.LIB $(OBJS)
   $(RC) -p -x $(OBJDIR)\teldir.res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
