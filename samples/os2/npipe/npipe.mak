#===================================================================
#
#   NPIPE.MAK -  Named Pipe Application Make file
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

SVR_HEADERS = svrpmain.h svrpxtrn.h svrpglbl.h svrphelp.h namepipe.h
CLI_HEADERS = clnpmain.h clnpxtrn.h clnpglbl.h clnphelp.h namepipe.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

SVR_OBJS = $(OBJDIR)\svrpmain.obj $(OBJDIR)\svrpthrd.obj $(OBJDIR)\svrppnt.obj  \
           $(OBJDIR)\svrpgame.obj $(OBJDIR)\svrphelp.obj
CLI_OBJS = $(OBJDIR)\clnpmain.obj $(OBJDIR)\clnpthrd.obj $(OBJDIR)\clnphelp.obj

SVR_IPF = svrnpipe.ipf file.ipf    svroptn.ipf help.ipf dlg.ipf
CLI_IPF = clinpipe.ipf clifile.ipf clioptn.ipf help.ipf clidlg.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\svrnpipe.exe $(EXEDIR)\svrnpipe.hlp \
     $(EXEDIR)\clinpipe.exe $(EXEDIR)\clinpipe.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\svrnpipe.res: svrnpipe.rc namepipe.ico svrpmain.h \
                        svrphelp.rc svrphelp.h   prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

$(OBJDIR)\clinpipe.res: clinpipe.rc namepipe.ico clnpmain.h \
                        clnphelp.rc clnphelp.h
                copy ..\..\prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\svrnpipe.hlp: $(SVR_IPF)

$(EXEDIR)\clinpipe.hlp: $(CLI_IPF)

$(OBJDIR)\svrpmain.obj: svrpmain.c $(SVR_HEADERS)

$(OBJDIR)\svrpthrd.obj: svrpthrd.c $(SVR_HEADERS)

$(OBJDIR)\svrpgame.obj: svrpgame.c $(SVR_HEADERS)

$(OBJDIR)\svrppnt.obj:  svrppnt.c  $(SVR_HEADERS)

$(OBJDIR)\svrphelp.obj: svrphelp.c $(SVR_HEADERS)

$(OBJDIR)\clnpmain.obj: clnpmain.c $(CLI_HEADERS)

$(OBJDIR)\clnpthrd.obj: clnpthrd.c $(CLI_HEADERS)

$(OBJDIR)\clnphelp.obj: clnphelp.c $(CLI_HEADERS)

$(EXEDIR)\svrnpipe.exe: $(SVR_OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(SVR_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

$(EXEDIR)\clinpipe.exe: $(CLI_OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(CLI_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
