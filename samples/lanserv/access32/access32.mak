# ACCESS32 LAN Requester Sample Application
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

all:  $(EXEDIR)\access32.exe

CFLAGS=$(CFLAGS) -DPURE_32

#====================================================================
# access32.obj dependencies
#====================================================================
$(OBJDIR)\access32.obj: $(SRCDIR)\access32.c

#====================================================================
# errmsg.obj dependencies
#====================================================================
$(OBJDIR)\errmsg.obj: $(SRCDIR)\errmsg.c

OBJS=$(OBJDIR)\access32.obj $(OBJDIR)\errmsg.obj

#====================================================================
# access32.exe
#   Define the dependencies for access32.exe and how to build it
#====================================================================
$(EXEDIR)\access32.exe : $(OBJS)
  -$(CREATE_PATH)
  $(LINK) $@ $(MTLIBS) + $(LANLIBS) $(OBJS)

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
