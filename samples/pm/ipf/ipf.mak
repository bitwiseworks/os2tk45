#===================================================================
#
#   IPF.MAK    - IPF Application Make file
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

HEADERS = ipf.h

ALL_IPF = ipf.ipf
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\ipf.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\ipf.dll $(EXEDIR)\ipf.inf

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\ipf.res: $(HEADERS) ipf.rc     ipf.ico    frame1.bmp frame2.bmp \
                              frame4.bmp frame5.bmp frame3.bmp prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\ipf.inf: $(ALL_IPF)
   $(IPF) /inf $(@B).ipf $@ /W3

$(OBJDIR)\ipf.obj: ipf.c $(HEADERS)
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\ipf.dll: $(OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(MTLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
