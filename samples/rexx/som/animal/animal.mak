#****************************************************************************
#
#  REXX - Animal Sample makefile - SOM 2.1 / IDL version
#
#  Copyright (C) 1993, 1994, 1996 IBM Corporation
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

SCFLAGS = -E ORXSAMP.IR

!include ..\..\..\ibmsamp.inc

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\animal.obj $(OBJDIR)\dog.obj $(OBJDIR)\bdog.obj \
       $(OBJDIR)\ldog.obj   $(OBJDIR)\sominit.obj

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
all: clean $(EXEDIR)\animals.dll

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

animal.ih: animal.idl

animal.h:  animal.idl

dog.ih:    dog.idl  animal.idl

dog.h:     dog.idl  animal.idl

bdog.ih:   bdog.idl dog.idl

bdog.h:    bdog.idl

ldog.ih:   ldog.idl dog.idl

ldig.h:    ldog.idl

$(OBJDIR)\animal.obj: animal.c animal.ih animal.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\dog.obj:     dog.c     dog.ih    animal.ih dog.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\bdog.obj:    bdog.c    bdog.ih   dog.ih bdog.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\ldog.obj:    ldog.c    ldog.ih   dog.ih ldog.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\sominit.obj: sominit.c animal.ih dog.ih    bdog.ih ldog.ih
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\animals.dll: animals.def $(OBJS)
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(DLLLIBS) + $(SOMLIBS) $(OBJS)

clean:
        @if exist animal.h  del animal.h
        @if exist animal.ih del animal.ih
        @if exist bdog.ih   del bdog.ih
        @if exist bdog.h    del bdog.h
        @if exist ldog.ih   del ldog.ih
        @if exist ldog.h    del ldog.h
