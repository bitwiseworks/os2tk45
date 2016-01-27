#===================================================================
#
#   Image Make file - Porting Stage 3 : Pure 32 bit Application
#           and 16-bit application
#   Copyright 1991 IBM Corporation
#
#  The IMAGES1.MAK and .DEF files are provided for comparison purposes.
#  Together with the source files, they are not
#  sufficient to build a 16-bit executable.  They have a dependency on an
#  OS/2 1.3 toolkit being available locally, since they need 16-bit
#  compilers/linkers.  The sample source files contain conditional
#  compilation statements for PORT_16 and PORT_32,
#  which show some of the changes needed in making the port.
#===================================================================

#-------------------------------------------------------------------
#   .SUFFIXES line helps NMAKE recognize other file suffixes besides
#   .c, .exe, etc.
#-------------------------------------------------------------------
.SUFFIXES: .res .rc .hlp .ipf


#===================================================================
#
# Macro definitions
#
#   These macro definitions set are used to set up the options needed
#   to build the application.  Such options include the command line
#   options for the compiler and the libraries needed for the linker.
#   The flags are created as macros so that they can be easily modified
#   so they need to be changed.
#
#   The macros also allow you to have a different set of options
#   for each environment your application is built for.  The macros
#   below include the flags for compiling the application in the
#   32-bit and 16-bit versions.  You can also add flags for a
#   a retail version of the product, a version which does not contain
#   any debugger symbols and has the maximum optimization.
#
#===================================================================
CC32 = icc /c /Gd- /Se /Re /ss /Ms /Gm+
CCS1 = cl
CFLAGSS1 = -u -c -G2s -Oi -W3 -Zpei -AL -DLINT_ARGS -NT CODE16 -ND DATA16 -DPORT_S116 -DHELP_MANAGER_ENABLED
CFLAGS32 = -DLINT_ARGS -DHELP_MANAGER_ENABLED -DPORT_32
CINC32 =
CINCS1 =
LINK32 = link386
LIBS32 = os2386 libc
LFLAGS = /MAP/CO/NOD
RC = rc
HEADERS = img_main.h img_xtrn.h


#-------------------------------------------------------------------
#   This section is used to set which options macros are to be used
#   in the build.  By changing the macro names on the right, we can
#   change which environment the application will be built for.  This
#   application currently uses the flags for the 32-bit memory model.
#-------------------------------------------------------------------
LINK = $(LINK32)
CFLAGS = $(CFLAGS32)
LIBS = $(LIBS32)
CINC = $(CINC32)


#===================================================================
#
# Default inference rules
#
#   This section lists the command and flags to build each type of
#   of source file listed.
#
#===================================================================
.c.obj:
    $(CC32) $(CFLAGS) $(CINC) $*.c

.rc.res:
    $(RC) -r $*.rc

#-------------------------------------------------------------------
#   A list of all of the object files
#-------------------------------------------------------------------
ALL_OBJ1 = img_utS1.obj img_daS1.obj img_inS1.obj img_pnS1.obj img_viS1.obj
ALL_OBJ2 = img_dlS1.obj img_heS1.obj img_siS1.obj img_fiS1.obj img_meS1.obj img_mas1.obj

#-------------------------------------------------------------------
#   A list of all of the Help text files
#-------------------------------------------------------------------
ALL_IPF = img_main.ipf img_file.ipf img_view.ipf img_help.ipf img_dlg.ipf

#-------------------------------------------------------------------
#   This section lists all files to be built by the make.  This make
#   file builds the executable as well as its associated help file.
#-------------------------------------------------------------------
all: imageS1.exe image.hlp


#-------------------------------------------------------------------
#   This section creates the command file used by the linker.  This
#   command file is recreated automatically every time you change
#   the object file list, linker flags, or library list.
#-------------------------------------------------------------------
imageS1.lnk: imageS1.mak
    echo $(ALL_OBJ1) +           > imageS1.lnk
    echo $(ALL_OBJ2)            >> imageS1.lnk
    echo imageS1.exe            >> imageS1.lnk
    echo imageS1.map $(LFLAGS)  >> imageS1.lnk
    echo $(LIBS)                >> imageS1.lnk
    echo imageS1.def            >> imageS1.lnk



#===================================================================
#
# Dependencies
#
#   This section lists all object files needed to be built for the
#   application, along with the files it is dependent upon (e.g. its
#   source and any header files).
#
#===================================================================
img_maS1.res: img_main.rc img_main.h img_dlg.h image.dlg\
              img_help.rc img_help.h
    $(RC) -r img_main.rc img_maS1.res

img_maS1.obj: img_main.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_maS1.obj img_main.c

img_meS1.obj: img_menu.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_meS1.obj img_menu.c

img_siS1.obj: img_size.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_siS1.obj img_size.c

img_fiS1.obj: img_file.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_fiS1.obj img_file.c

img_viS1.obj: img_view.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_viS1.obj img_view.c

img_daS1.obj: img_data.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_daS1.obj img_data.c

img_inS1.obj: img_init.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_inS1.obj img_init.c

img_pnS1.obj: img_pnt.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_pnS1.obj img_pnt.c

img_dlS1.obj: img_dlg.c $(HEADERS)
    $(CC32) $(CFLAGS) $(CINC) -Foimg_dlS1.obj img_dlg.c

img_heS1.obj: img_help.c $(HEADERS) img_help.h
    $(CC32) $(CFLAGS) $(CINC) -Foimg_heS1.obj img_help.c

img_utS1.obj: img_util.c $(HEADERS)
    $(CCS1) $(CFLAGSS1) $(CINC) -Fa -Foimg_utS1.obj img_util.c
#-------------------------------------------------------------------
#   This section creates the help manager file.  The IPF compiler
#   creates a file called main.hlp which is renamed to image.hlp.
#-------------------------------------------------------------------
image.hlp: $(ALL_IPF)
     ipfc img_main.ipf /W3
     if exist image.hlp del image.hlp
     rename img_main.hlp image.hlp


#-------------------------------------------------------------------
#   This section links the object modules into the finished program
#   using the linker command file created earlier.  At the end, the
#   resource file is bound to the application.
#-------------------------------------------------------------------
imageS1.exe: $(ALL_OBJ1) $(ALL_OBJ2) imageS1.def imageS1.lnk img_maS1.res
    $(LINK) @imageS1.lnk
    $(RC) img_maS1.res imageS1.exe
