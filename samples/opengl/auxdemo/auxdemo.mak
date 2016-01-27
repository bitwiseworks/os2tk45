#===================================================================
#
#   AUXDEMO.MAK - OpenGL AUX demo make file
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

#-------------------------------------------------------------------
#
#   OpenGL specific definitions
#
#-------------------------------------------------------------------

AUXLIB = libaux.lib
OGLLIB = opengl.lib
CFLAGS = $(CFLAGS) -Ss -W1 -Ms -Gm+ -DOS2

#-------------------------------------------------------------------
#
#   A list of all of the exe files
#
#-------------------------------------------------------------------

EXES = $(EXEDIR)\accanti.exe  \
       $(EXEDIR)\accnot.exe   \
       $(EXEDIR)\accpersp.exe \
       $(EXEDIR)\accum.exe    \
       $(EXEDIR)\aim.exe      \
       $(EXEDIR)\alpha.exe    \
       $(EXEDIR)\alpha3d.exe  \
       $(EXEDIR)\anti.exe     \
       $(EXEDIR)\antiindx.exe \
       $(EXEDIR)\antipidx.exe \
       $(EXEDIR)\antipoly.exe \
       $(EXEDIR)\antipt.exe   \
       $(EXEDIR)\bezcurve.exe \
       $(EXEDIR)\bezmesh.exe  \
       $(EXEDIR)\bezsurf.exe  \
       $(EXEDIR)\checker.exe  \
       $(EXEDIR)\checker2.exe \
       $(EXEDIR)\chess.exe    \
       $(EXEDIR)\clip.exe     \
       $(EXEDIR)\colormat.exe \
       $(EXEDIR)\cone.exe     \
       $(EXEDIR)\cube.exe     \
       $(EXEDIR)\curve.exe    \
       $(EXEDIR)\depthcue.exe \
       $(EXEDIR)\disk.exe     \
       $(EXEDIR)\dof.exe      \
       $(EXEDIR)\dofnot.exe   \
       $(EXEDIR)\double.exe   \
       $(EXEDIR)\drawf.exe    \
       $(EXEDIR)\feedback.exe \
       $(EXEDIR)\fog.exe      \
       $(EXEDIR)\fogindex.exe \
       $(EXEDIR)\font.exe     \
       $(EXEDIR)\light.exe    \
       $(EXEDIR)\linelist.exe \
       $(EXEDIR)\lines.exe    \
       $(EXEDIR)\list.exe     \
       $(EXEDIR)\list2.exe    \
       $(EXEDIR)\maplight.exe \
       $(EXEDIR)\material.exe \
       $(EXEDIR)\mipmap.exe   \
       $(EXEDIR)\model.exe    \
       $(EXEDIR)\mvlight.exe  \
       $(EXEDIR)\nurbs.exe    \
       $(EXEDIR)\pickdpth.exe \
       $(EXEDIR)\pickline.exe \
       $(EXEDIR)\picksqr.exe  \
       $(EXEDIR)\plane.exe    \
       $(EXEDIR)\planet.exe   \
       $(EXEDIR)\planetup.exe \
       $(EXEDIR)\polys.exe    \
       $(EXEDIR)\robot.exe    \
       $(EXEDIR)\scene.exe    \
       $(EXEDIR)\scnbamb.exe  \
       $(EXEDIR)\scnflat.exe  \
       $(EXEDIR)\scnlight.exe \
       $(EXEDIR)\select.exe   \
       $(EXEDIR)\simple.exe   \
       $(EXEDIR)\smooth.exe   \
       $(EXEDIR)\sphere.exe   \
       $(EXEDIR)\stencil.exe  \
       $(EXEDIR)\stroke.exe   \
       $(EXEDIR)\surface.exe  \
       $(EXEDIR)\tea.exe      \
       $(EXEDIR)\teaamb.exe   \
       $(EXEDIR)\teapots.exe  \
       $(EXEDIR)\texgen.exe   \
       $(EXEDIR)\texsurf.exe  \
       $(EXEDIR)\trim.exe

#-------------------------------------------------------------------
#   This section lists all files to be built by  make.  The
#   makefile builds the executable as well as its associated help
#   file.
#-------------------------------------------------------------------
all: $(EXES)

#-------------------------------------------------------------------
#   Dependencies
#     This section lists all object files needed to be built for the
#     application, along with the files it is dependent upon (e.g.
#     its source and any header files).
#-------------------------------------------------------------------

$(EXES): $(OBJDIR)\$(@B).obj auxdemo.def
   -$(CREATE_PATH)
   $(LINK) $@ auxdemo.def $(MTLIBS) + $(OGLLIB) + $(AUXLIB) $(OBJDIR)\$(@B).obj

clean :
        @if exist *.obj del *.obj
        @if exist *.exe del *.exe
