#===================================================================
#
#   QUEUES.MAK - Queues Application Make file
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

SVR_HEADERS = svrqmain.h svrqxtrn.h svrqglbl.h svrqhelp.h queue.h
CLI_HEADERS = cliqmain.h cliqxtrn.h cliqglbl.h cliqhelp.h queue.h
#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

SVR_OBJS = $(OBJDIR)\svrqmain.obj $(OBJDIR)\svrqthrd.obj $(OBJDIR)\svrqpnt.obj  \
           $(OBJDIR)\svrqhelp.obj
CLI_OBJS = $(OBJDIR)\cliqmain.obj $(OBJDIR)\cliqthrd.obj $(OBJDIR)\cliqhelp.obj

SVR_IPF = svrqueue.ipf file.ipf svroptn.ipf  help.ipf dlg.ipf
CLI_IPF = cliqueue.ipf file.ipf clientry.ipf help.ipf dlg.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\svrqueue.exe $(EXEDIR)\svrqueue.hlp \
     $(EXEDIR)\cliqueue.exe $(EXEDIR)\cliqueue.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\svrqueue.res: svrqueue.rc queue.ico   svrqmain.h \
                        svrqhelp.rc svrqhelp.h  prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

$(OBJDIR)\cliqueue.res: cliqueue.rc queue.ico cliqmain.h \
                        cliqhelp.rc cliqhelp.h
                copy ..\..\prodinfo.bmp
                $(RC) $(RCFLAGS) $(@B).rc $@
                @del prodinfo.bmp

prodinfo.bmp :
           copy ..\..\prodinfo.bmp $@

$(EXEDIR)\svrqueue.hlp: $(SVR_IPF)

$(EXEDIR)\cliqueue.hlp: $(CLI_IPF)

$(OBJDIR)\svrqmain.obj: svrqmain.c $(SVR_HEADERS)

$(OBJDIR)\svrqthrd.obj: svrqthrd.c $(SVR_HEADERS)

$(OBJDIR)\svrqpnt.obj:  svrqpnt.c  $(SVR_HEADERS)

$(OBJDIR)\svrqhelp.obj: svrqhelp.c $(SVR_HEADERS)

$(OBJDIR)\cliqmain.obj: cliqmain.c $(CLI_HEADERS)

$(OBJDIR)\cliqthrd.obj: cliqthrd.c $(CLI_HEADERS)

$(OBJDIR)\cliqhelp.obj: cliqhelp.c $(CLI_HEADERS)

$(EXEDIR)\svrqueue.exe: $(SVR_OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(SVR_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

$(EXEDIR)\cliqueue.exe: $(CLI_OBJS) $(@B).def $(OBJDIR)\$(@B).res
   -$(CREATE_PATH)
   $(LINK) $@ $(@B).def $(MTLIBS) $(CLI_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
