#===================================================================
#
#   CDMCT.MAK - Make file for CD Audio Media Component Interface Drivers
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



HEADERS = cdaudibm.h hhpheap.h ibmcdrom.h
#-------------------------------------------------------------------
#
#  Component specific requirements for compile flags
#
#-------------------------------------------------------------------

CC = $(CC) -Ge- -Sm -DINCL_32

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

IBMCDRT_OBJS = \
$(OBJDIR)\ibmcdrom.obj $(OBJDIR)\ibmcdpro.obj $(OBJDIR)\ibmcdmsc.obj

CDAUDIO_OBJS = \
$(OBJDIR)\cdaudio.obj $(OBJDIR)\cdaudpro.obj $(OBJDIR)\cdaudutl.obj

COMMON_OBJS = \
$(OBJDIR)\cdmccomn.obj $(OBJDIR)\cdmcinit.obj $(OBJDIR)\hhpheap.obj

OBJS = $(IBMCDRT_OBJS) $(CDAUDIO_OBJS) $(COMMON_OBJS)

ALL_IPF =

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\cdaudiot.dll $(EXEDIR)\ibmcdrt.dll

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


$(EXEDIR)\ibmcdrt.dll: $(IBMCDRT_OBJS) $(COMMON_OBJS) ibmcdrom.def
   -$(CREATE_PATH)
   $(LINK) $@ ibmcdrom.def $(MTLIBS) @<<$(OBJDIR)\ibmcdrom.lnk
$(IBMCDRT_OBJS) $(COMMON_OBJS)
<<

$(EXEDIR)\cdaudiot.dll: $(CDAUDIO_OBJS) $(COMMON_OBJS) cdaudio.def
   -$(CREATE_PATH)
   $(LINK) $@ cdaudio.def $(MTLIBS) @<<$(OBJDIR)\cdaudio.lnk
$(CDAUDIO_OBJS) $(COMMON_OBJS)
<<

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
