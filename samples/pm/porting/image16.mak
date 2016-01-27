#===================================================================
#
#    Make file for the 16-bit Image sample.
#   Copyright 1991 IBM Corporation
#
#  The IMAGE16.MAK and .DEF files are provided for comparison purposes.
#  Together with the source files, they are not
#  sufficient to build a 16-bit executable.  They have a dependency on an
#  OS/2 1.3 toolkit being available locally, since they need 16-bit
#  compilers/linkers.  The sample source files contain conditional
#  compilation statements for PORT_16 and PORT_32,
#  which show some of the changes needed in making the port.
#
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
#   below include the flags for compiling the application in
#   32-bit and 16-bit versions.  You can also add flags for a
#   a retail version of the product, a version which does not contain
#   any debugger symbols and has the maximum optimization.
#
#===================================================================
CC = cl -c
CFLAGS16 = -u -Gs -Od -W3 -Zpei -AL -DHELP_MANAGER_ENABLED -DPORT_16
CINC16 =
LINK16 = link
LIBS16 = os2286 llibcep
LFLAGS = /MAP/CO/NOD
RC = rc
HEADERS = img_main.h img_xtrn.h

#-------------------------------------------------------------------
#   This section is used to set which options macros are to be used
#   in the build.  By changing the macro names on the right, we can
#   change which environment the application will be built for.  This
#   make file has flags set for the 16-bit memory model.
#-------------------------------------------------------------------
LINK = $(LINK16)
CFLAGS = $(CFLAGS16)
LIBS = $(LIBS16)
CINC = $(CINC16)


#===================================================================
#
# Default inference rules
#
#   This section lists the command and flags to build each type of
#   of source file listed.
#
#===================================================================
.c.obj:
    $(CC) $(CFLAGS) $(CINC) $*.c

.rc.res:
    $(RC) -r $*.rc

#-------------------------------------------------------------------
#   A list of all of the object files
#-------------------------------------------------------------------
ALL_OBJ1 = img_ma16.obj img_da16.obj img_in16.obj img_pn16.obj img_vi16.obj
ALL_OBJ2 = img_dl16.obj img_he16.obj img_si16.obj img_fi16.obj img_me16.obj img_ut16.obj

#-------------------------------------------------------------------
#   A list of all of the Help text files
#-------------------------------------------------------------------
ALL_IPF = img_main.ipf img_file.ipf img_view.ipf img_help.ipf img_dlg.ipf

#-------------------------------------------------------------------
#   This section lists all files to be build my the make.  This make
#   file build the executible as well as its associated help file.
#-------------------------------------------------------------------
all: image16.exe image.hlp


#-------------------------------------------------------------------
#   This section creates the command file used by the linker.  This
#   command file is recreated automatically every time you change
#   the object file list, linker flags, or library list.
#-------------------------------------------------------------------
image16.lnk: image16.mak
    echo $(ALL_OBJ1) +             > image16.lnk
    echo $(ALL_OBJ2)              >> image16.lnk
    echo image16.exe              >> image16.lnk
    echo image16.map $(LFLAGS)    >> image16.lnk
    echo $(LIBS)                  >> image16.lnk
    echo image16.def              >> image16.lnk



#===================================================================
#
# Dependencies
#
#   This section lists all object files needed to be built for the
#   application, along with the files it is dependent upon (e.g. its
#   source and any header files).
#
#===================================================================

img_ma16.res: img_main.rc img_main.h img_dlg.h image.dlg \
              img_help.rc img_help.h
    copy ..\prodinfo.bmp
    $(RC) -r img_main.rc img_ma16.res
    del prodinfo.bmp

img_ma16.obj: img_main.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_ma16.obj img_main.c

img_me16.obj: img_menu.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_me16.obj img_menu.c

img_si16.obj: img_size.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_si16.obj img_size.c

img_fi16.obj: img_file.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_fi16.obj img_file.c

img_vi16.obj: img_view.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_vi16.obj img_view.c

img_da16.obj: img_data.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_da16.obj img_data.c

img_dl16.obj: img_dlg.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_dl16.obj img_dlg.c

img_in16.obj: img_init.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_in16.obj img_init.c

img_pn16.obj: img_pnt.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Foimg_pn16.obj img_pnt.c

img_he16.obj: img_help.c $(HEADERS) img_help.h
    $(CC) $(CFLAGS) $(CINC) -Foimg_he16.obj img_help.c

img_ut16.obj: img_util.c $(HEADERS)
    $(CC) $(CFLAGS) $(CINC) -Fa -Oi -Foimg_ut16.obj img_util.c
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
image16.exe: $(ALL_OBJ1) $(ALL_OBJ2) image16.def image16.lnk img_ma16.res
    $(LINK) @image16.lnk
    $(RC) img_ma16.res image16.exe
    mapsym image16
