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

OBJS = $(OBJDIR)\carpp.obj $(OBJDIR)\horn.obj $(OBJDIR)\brake.obj $(OBJDIR)\rpm.obj

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
all: clean $(EXEDIR)\carpp.dll $(EXEDIR)\carpp.hlp

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------
$(OBJDIR)\carpp.res: carpp.rc carpp.xih
                $(RC) $(RCFLAGS) $(@B).rc $@

$(EXEDIR)\carpp.hlp: carpp.ipf

carpp.xih:   carpp.idl
   sc -r -I. -I$(R_OS2_IDL) -I$(SOM_COMP_HDR) -mnoint -mnotc -S 128000 -C 128000 -s"xih" $*.idl

carpp.def:   carpp.idl
   sc -r -I. -I$(R_OS2_IDL) -I$(SOM_COMP_HDR) -mnoint -mnotc -S 128000 -C 128000 -s"def" $*.idl

carpp.xh:    carpp.idl
   sc -r -I. -I$(R_OS2_IDL) -I$(SOM_COMP_HDR) -mnoint -mnotc -S 128000 -C 128000 -s"xh" $*.idl

$(OBJDIR)\horn.obj :    horn.cpp horn.h carpp.xih
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).cpp

$(OBJDIR)\brake.obj:    brake.cpp brake.h carpp.xih
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).cpp

$(OBJDIR)\rpm.obj  :    rpm.cpp rpm.h carpp.xih
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).cpp

$(OBJDIR)\carpp.obj: carpp.cpp carpp.xh carpp.xih carpp.idl horn.h brake.h rpm.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) $(DLLCFLAGS) -I. -Fo"$(OBJDIR)\$(@B).obj" $(@B).cpp

$(EXEDIR)\carpp.dll: carpp.def $(OBJS) $(OBJDIR)\carpp.res
   -$(CREATE_PATH)
   $(LINK) $@ $(DLLCFLAGS) $(@B).def $(DLLLIBS) + $(SOMLIBS) $(OBJS)
   $(RC) -p -x $(OBJDIR)\$(@B).res $@

clean:
        @if exist carpp.xh  del carpp.xh
        @if exist carpp.xih del carpp.xih
