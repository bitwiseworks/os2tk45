#===================================================================
#
#   ULIOT.MAK - Makefile for Toolkit AVI IO Procedure
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



HEADERS = \
checkmem.h \
ff.h       \
hhpheap.h  \
ioi.h      \
mmdsload.h \
mmiocrt.h  \
ulionls.h

#-------------------------------------------------------------------
#
#  Component specific requirements for compile flags
#
#-------------------------------------------------------------------

CC = $(CC) -Ge- -Sm -I. -DINCL_32

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = \
$(OBJDIR)\CHECKMEM.obj \
$(OBJDIR)\HHPHEAP.obj  \
$(OBJDIR)\IOCLOSE.obj  \
$(OBJDIR)\IOCODEC.obj  \
$(OBJDIR)\IODLL.obj    \
$(OBJDIR)\IOEDIT.obj   \
$(OBJDIR)\IOENTRY.obj  \
$(OBJDIR)\ioformat.obj \
$(OBJDIR)\IOHEADER.obj \
$(OBJDIR)\IOOPEN.obj   \
$(OBJDIR)\IOSEEK.obj   \
$(OBJDIR)\IOSET.obj    \
$(OBJDIR)\IOSRV.obj    \
$(OBJDIR)\ULCODEC.obj  \
$(OBJDIR)\ULGDAT.obj   \
$(OBJDIR)\ULIDENT.obj  \
$(OBJDIR)\ULOPEN.obj   \
$(OBJDIR)\ULREAD.obj   \
$(OBJDIR)\ULSRV.obj


ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\uliot.dll

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


$(EXEDIR)\uliot.dll: $(OBJS) uliot.def
   -$(CREATE_PATH)
   $(LINK) $@ uliot.def $(MTLIBS) $(MMLIBS) @<<$(OBJDIR)\uliot.lnk
$(OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
