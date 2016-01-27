#===================================================================
#
#   CODEC.MAK - Make file for OS/2 Multimedia Pluggable Decompressor Codecs
#   Copyright  IBM Corporation 1992
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

HEADERS = sampdeco.h sampenco.h sampinst.h
#-------------------------------------------------------------------
#
#  Component specific requirements for compile flags
#
#-------------------------------------------------------------------

CC = $(CC) -Ge- -Sm -DINCL_32 -I.

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = \
$(OBJDIR)\sampmain.obj  \
$(OBJDIR)\sampdeco.obj  \
$(OBJDIR)\sampenco.obj


ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\sampcodc.dll

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

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\sampcodc.dll: $(OBJS) sampcodc.def
   -$(CREATE_PATH)
   $(LINK) $@ sampcodc.def $(MTLIBS) @<<$(OBJDIR)\sampcodc.lnk
$(OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
