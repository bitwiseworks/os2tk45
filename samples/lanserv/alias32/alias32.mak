# ALIAS32 LAN Requester Sample Application
#
# (C) COPYRIGHT International Business Machines Corp. 1996
# All Rights Reserved
# Licensed Materials - Property of IBM
#
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#----------------------------------------------------------------------------

#===================================================================
# Include the file ibmsamp.inc which defines the various macros
# depending on the target specified.
#===================================================================

!include ..\..\ibmsamp.inc

all:  $(EXEDIR)\alias32.exe

CFLAGS=$(CFLAGS) -DPURE_32

#====================================================================
# alias32.obj dependencies
#====================================================================
$(OBJDIR)\alias32.obj: $(SRCDIR)\alias32.c

#====================================================================
# errmsg.obj dependencies
#====================================================================
$(OBJDIR)\errmsg.obj: $(SRCDIR)\errmsg.c

OBJS=$(OBJDIR)\alias32.obj $(OBJDIR)\errmsg.obj

#====================================================================
# alias32.exe
#   Define the dependencies for alias32.exe and how to build it
#====================================================================
$(EXEDIR)\alias32.exe : $(OBJS) $(SRCDIR)\sample32.def
  -$(CREATE_PATH)
  $(LINK) $@ $(SRCDIR)\sample32.def $(MTLIBS) + $(LANLIBS) $(OBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
