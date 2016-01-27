#===================================================================
#
#   FSSHT.MAK - Make file for Toolkit File System Stream Handler
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
#       MMLIBS   - Multimedia libraries
#===================================================================

!include ..\..\ibmsamp.inc



HEADERS = fssh.h hhpheap.h seekcalc.h shi.h shifwd.h
#-------------------------------------------------------------------
#
#  Component specific requirements for compile flags
#
#-------------------------------------------------------------------

CC = $(CC) -Ge- -Sm -DINCL_32 /DINCL_NEWSH -I.

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = \
$(OBJDIR)\fsshread.obj \
$(OBJDIR)\fsshgdat.obj \
$(OBJDIR)\fsshwrit.obj \
$(OBJDIR)\shioutil.obj \
$(OBJDIR)\shmisc.obj   \
$(OBJDIR)\shrouter.obj \
$(OBJDIR)\shstart.obj  \
$(OBJDIR)\shstop.obj   \
$(OBJDIR)\fsshseek.obj \
$(OBJDIR)\fsshass.obj  \
$(OBJDIR)\hhpheap.obj  \
$(OBJDIR)\seekcalc.obj \
$(OBJDIR)\fsshcrea.obj \
$(OBJDIR)\shnegot.obj  \
$(OBJDIR)\shdestry.obj \
$(OBJDIR)\shgprot.obj  \
$(OBJDIR)\sheprot.obj  \
$(OBJDIR)\shiprot.obj  \
$(OBJDIR)\shinit.obj


ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\fssht.dll

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


$(EXEDIR)\fssht.dll: $(OBJS) fssht.def
   -$(CREATE_PATH)
   $(LINK) $@ fssht.def $(MTLIBS) $(MMLIBS) @<<$(OBJDIR)\fssht.lnk
$(OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
