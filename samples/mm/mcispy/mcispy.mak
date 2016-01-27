#===================================================================
#
#   MCISPY.MAK - Make file for the OS/2 MMPM/2 MCISPY Application
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

HEADERS = mcispy.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

MCIOBJS = \
$(OBJDIR)\dlgprocs.obj \
$(OBJDIR)\mcispy.obj   \
$(OBJDIR)\mcisubs.obj  \
$(OBJDIR)\mcitbl.obj

MDMOBJS = $(OBJDIR)\mdm.obj

OBJS = $(MCIOBJS) $(MDMOBJS)


ALL_IPF = mcispy.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\mcispy.exe $(EXEDIR)\mdm.dll $(EXEDIR)\mcispy.hlp

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
$(OBJDIR)\mcispy.res: mcispy.rc $(HEADERS) mcispy.ico mcispy.dlg prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

$(EXEDIR)\mcispy.hlp: $(ALL_IPF)

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(OBJDIR)\mdm.obj: mdm.c mdm.h
        $(CC) -Ge- -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" -DINCL_32 -DINCL_OS2MM $(@B).c

$(EXEDIR)\mcispy.exe: $(MCIOBJS) mcispy.def $(OBJDIR)\mcispy.res
   -$(CREATE_PATH)
   $(LINK) $@ mcispy.def $(MTLIBS) $(MMLIBS) @<<$(OBJDIR)\mcispy.lnk
$(MCIOBJS)
<<
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

$(EXEDIR)\mdm.dll: $(MDMOBJS) mdmcmd.res mdm.def
   -$(CREATE_PATH)
   $(LINK) $@ mdm.def $(MTLIBS) $(MMLIBS) @<<$(OBJDIR)\mcispy.lnk
$(MDMOBJS)
<<
   $(RC) -p -x mdmcmd.res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
