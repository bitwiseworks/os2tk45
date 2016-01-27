#===================================================================
#
#   GLUTDEMO.MAK - OpenGL GLUT Demo Make file
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

GLUTLIB = glut.lib
OGLLIB  = opengl.lib
CFLAGS  = $(CFLAGS) -Ss -W1 -Gm+ -DOS2

#-------------------------------------------------------------------
#
#   A list of all of the object files
#
#-------------------------------------------------------------------

OBJS = $(OBJDIR)\atlantis.obj \
       $(OBJDIR)\whale.obj    \
       $(OBJDIR)\shark.obj    \
       $(OBJDIR)\dolphin.obj  \
       $(OBJDIR)\swim.obj

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXEDIR)\atlantis.exe $(EXEDIR)\test7.exe

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(OBJDIR)\atlantis.obj: atlantis.c atlantis.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\whale.obj:    whale.c    atlantis.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\shark.obj:    shark.c    atlantis.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\dolphin.obj:  dolphin.c  atlantis.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\swim.obj:     swim.c     atlantis.h
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(OBJDIR)\test7.obj:    test7.c
   $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

$(EXEDIR)\atlantis.exe: $(OBJS)   glutdemo.def
   -$(CREATE_PATH)
   $(LINK) $@ glutdemo.def $(MTLIBS) + $(OGLLIB) + $(GLUTLIB) $(OBJS)

$(EXEDIR)\test7.exe:    $(OBJDIR)\test7.obj glutdemo.def
   -$(CREATE_PATH)
   $(LINK) $@ glutdemo.def $(MTLIBS) + $(OGLLIB) + $(GLUTLIB) $(OBJDIR)\test7.obj

clean :
        @if exist *.obj del *.obj
        @if exist *.dll del *.dll
        @if exist *.exe del *.exe
