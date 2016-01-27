#===================================================================
#
#   IMAGE32.MAK -  32-Bit Image Application Make file
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
#===================================================================

!include ..\..\ibmsamp.inc

HEADERS = img_main.h img_xtrn.h

ALL_IPF = img_main.ipf img_file.ipf img_view.ipf img_help.ipf
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\img_ma32.obj $(OBJDIR)\img_da32.obj $(OBJDIR)\img_in32.obj $(OBJDIR)\img_pn32.obj \
       $(OBJDIR)\img_vi32.obj $(OBJDIR)\img_dl32.obj $(OBJDIR)\img_he32.obj $(OBJDIR)\img_si32.obj \
       $(OBJDIR)\img_fi32.obj $(OBJDIR)\img_me32.obj $(OBJDIR)\img_ut32.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\image32.exe $(EXEDIR)\image.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\img_ma32.res: $(HEADERS) img_main.rc image.dlg img_help.rc prodinfo.bmp
                $(RC) $(RCFLAGS) img_main.rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\image.hlp: $(ALL_IPF)
        $(IPF) img_main.ipf $(EXEDIR)\$(@B).hlp /W3

$(OBJDIR)\img_ma32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_main.c

$(OBJDIR)\img_me32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_menu.c

$(OBJDIR)\img_si32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_size.c

$(OBJDIR)\img_fi32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_file.c

$(OBJDIR)\img_vi32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_view.c

$(OBJDIR)\img_da32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_data.c

$(OBJDIR)\img_in32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_init.c

$(OBJDIR)\img_pn32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_pnt.c

$(OBJDIR)\img_dl32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_dlg.c

$(OBJDIR)\img_he32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_help.c

$(OBJDIR)\img_ut32.obj:
   $(CC) -c $(CFLAGS) $(CINCLUDE) -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32 -Fo"$@" img_util.c


$(EXEDIR)\image32.exe: $(OBJS) $(@B).def $(OBJDIR)\img_ma32.res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) @<<$(OBJDIR)\image32.lnk
$(OBJS)
<<
   $(RC) -p -x $(OBJDIR)\img_ma32.res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
