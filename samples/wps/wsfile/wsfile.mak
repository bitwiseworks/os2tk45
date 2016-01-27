#****************************************************************************
#
#  Work Place Shell - car Sample makefile - SOM 2.1 / IDL version
#
#  Copyright (C) 1993, 1994 IBM Corporation
#
#      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
#      sample code created by IBM Corporation. This sample code is not
#      part of any standard or IBM product and is provided to you solely
#      for  the purpose of assisting you in the development of your
#      applications.  The code is provided "AS IS", without
#      warranty of any kind.  IBM shall not be liable for any damages
#      arising out of your use of the sample code, even if they have been
#      advised of the possibility of such damages.
#
#      NOTE: you will have to modify the environment variables to suit
#            your environment
#
#****************************************************************************

!include ..\..\ibmsamp.inc

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

FILE_OBJS   = $(OBJDIR)\wsfile.obj
FOLDER_OBJS = $(OBJDIR)\wsfolder.obj

SOMTEMP = .\somtemp

!if [set SMTMP=$(SOMTEMP)] || \
    [set SMEMIT=ih;h;c] || \
    [set SMADDSTAR=1] || \
    [set SMNOTC=1]
!endif

!if [cd $(SOMTEMP)]
!  if [md $(SOMTEMP)]
!    error error creating $(SOMTEMP) directory
!  endif
!else
!  if [cd ..]
!    error - Couldn't cd .. from $(SOMTEMP) directory
!  endif
!endif

FILE_IPF   = wsfile.ipf
FOLDER_IPF = wsfolder.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: clean $(EXEDIR)\wsfile.dll $(EXEDIR)\wsfolder.dll  \
           $(EXEDIR)\wsfile.hlp $(EXEDIR)\wsfolder.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\wsfile.res: wsfile.rc wsfile.ih
                $(RC) $(RCFLAGS) $(@B).rc $@

$(OBJDIR)\wsfolder.res: wsfolder.rc wsfolder.ih
                $(RC) $(RCFLAGS) $(@B).rc $@

$(EXEDIR)\wsfile.hlp:   $(FILE_IPF)

$(EXEDIR)\wsfolder.hlp: $(FOLDER_IPF)

wsfile.h:     wsfile.idl

wsfile.def:   wsfile.idl

wsfile.ih:    wsfile.idl

wsfile.c:     wsfile.idl

wsfolder.h:   wsfolder.idl

wsfolder.def: wsfolder.idl

wsfolder.ih:  wsfolder.idl

wsfolder.c:   wsfolder.idl

$(OBJDIR)\wsfile.obj:    wsfile.h   wsfile.c    wsfile.ih   wsfile.idl
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -DSOM_DONT_USE_INHERITED_MACROS -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\wsfolder.obj:  wsfolder.h wsfolder.c  wsfolder.ih wsfolder.idl
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -DSOM_DONT_USE_INHERITED_MACROS -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\wsfile.dll: wsfile.def $(FILE_OBJS) $(OBJDIR)\wsfile.res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(DLLLIBS) + $(SOMLIBS) $(FILE_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

$(EXEDIR)\wsfolder.dll: wsfolder.def $(FOLDER_OBJS) $(OBJDIR)\wsfolder.res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(DLLLIBS) + $(SOMLIBS) $(FOLDER_OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean:
        @if exist wsfile.h   del wsfile.h
        @if exist wsfile.ih  del wsfile.ih
        @if exist wsfile.def del wsfile.def
        @if exist wsfolder.h   del wsfolder.h
        @if exist wsfolder.ih  del wsfolder.ih
        @if exist wsfolder.def del wsfolder.def
