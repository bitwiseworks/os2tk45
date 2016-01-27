#===================================================================
#
#   PROBE.MAK -    System Management FFST Probe Sample Make file
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
#       FFSTLIBS - FFST libraries
#===================================================================

!include ..\..\..\ibmsamp.inc

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\probe.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\probe.exe $(EXEDIR)\probe.msg $(EXEDIR)\probe.rep

#-------------------------------------------------------------------
#   This section creates the command file used by the linker.  This
#   command file is recreated automatically every time you change
#   the object file list, linker flags, or library list.
#-------------------------------------------------------------------
#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\probe.obj: probe.c

$(EXEDIR)\probe.msg:  probemsg.txt
   -$(CREATE_PATH)
   mkmsgf probemsg.txt $@

$(EXEDIR)\probe.rep:  proberep.txt
   mktmpf -O $@ -- proberep.txt

$(EXEDIR)\probe.exe: $(OBJS) $(@B).def
   -$(CREATE_PATH)
   $(LINK) $@ $(CFLAGS) $(@B).def $(MTLIBS) + $(FFSTLIBS) + $(ULSLIBS) @<<probe.lrf
$(OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
