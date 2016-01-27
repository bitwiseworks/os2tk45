#****************************************************************************
#
#  Work Place Shell - Browse Sample makefile - SOM 2.1 / IDL version
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

OBJS = $(OBJDIR)\browse.obj $(OBJDIR)\pmbrowse.obj

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

ALL_IPF = browse.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  The
#   makefile builds the executible as well as its associated help
#   file.
#-------------------------------------------------------------------
all: clean $(EXEDIR)\browse.dll $(EXEDIR)\browse.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\browse.res: browse.rc browse.ih browse.ico pmbrowse.h
                $(RC) $(RCFLAGS) $(@B).rc $@

$(EXEDIR)\browse.hlp: $(ALL_IPF)

browse.h:   browse.idl

browse.def: browse.idl

browse.ih:  browse.idl

browse.c:   browse.idl

$(OBJDIR)\browse.obj:    browse.c    browse.ih  browse.h browse.idl
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\pmbrowse.obj:  pmbrowse.c  pmbrowse.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\browse.dll: browse.def $(OBJS) $(OBJDIR)\browse.res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(MTLIBS) + $(SOMLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean:
        @if exist browse.h  del browse.h
        @if exist browse.ih del browse.ih
