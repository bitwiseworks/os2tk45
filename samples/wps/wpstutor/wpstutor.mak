#****************************************************************************
#
#  Work Place Shell - WPS Tutorial Sample makefile - SOM 2.1 / IDL version
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

OBJS = $(OBJDIR)\wpstutor.obj

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

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: clean $(EXEDIR)\wpstutor.dll

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\wpstutor.res: wpstutor.rc wpstutor.ih wpstutor.ico
                $(RC) $(RCFLAGS) $(@B).rc $@

wpstutor.h:   wpstutor.idl

wpstutor.def: wpstutor.idl

wpstutor.ih:  wpstutor.idl

wpstutor.c:   wpstutor.idl

$(OBJDIR)\wpstutor.obj:    wpstutor.c    wpstutor.ih  wpstutor.h wpstutor.idl
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\wpstutor.dll: wpstutor.def $(OBJS) $(OBJDIR)\wpstutor.res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(DLLLIBS) + $(SOMLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean:
        @if exist wpstutor.h  del wpstutor.h
        @if exist wpstutor.ih del wpstutor.ih
