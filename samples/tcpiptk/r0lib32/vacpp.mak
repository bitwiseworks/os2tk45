#==============================================================================
#
#   TCP/IP R0LIB32 Sample Make file
#
#==============================================================================
# Include the file ibmsamp.inc which defines the various macros
# depending on the target specified.
#
# The following macros are defined in ibmsamp.inc:
#
#    OBJDIR      - Directory where object files are created
#    EXEDIR      - Directory where executable files are created
#    LINK        - Linker
#    LFLAGS      - Linker flags
#    MTLIB       - Multithreaded versions of the compiler runtime libraries
#    MTLIBS      - Combination of $(MTLIB) $(OS2386_LIB)
#    OS2286_LIB  - Entry points for 16-bit OS/2 API's
#    OS2386_LIB  - Entry points for 32-bit OS/2 API's
#    OS2386P_LIB - 32-bit ring-0 entry points (from OS/2 DDK)
#    OS2_H16     - Definitions for 16-bit OS/2 API's (from OS/2 DDK)
#==============================================================================

!include ..\..\ibmsamp.inc

DEFINES  = -DSLBOOTP -DSOCKETS -DOS2 -DKERNEL -DNEWOS2 -DMULTICAST -DGARBAGE_COLLECT -DVACPP
CFLAGS   = /G4 /O+ /Op- /Sm /Ss /W3 /Gi /Ms /Op- /Su4 /Sp1 $(DEFINES) /Fa$*.cod
C6CFLAGS = -DKERNEL -DSPL16 -W3 -Gs -NTMAINSEG -Alfw -Os -Zpe -Fc -G2

#------------------------------------------------------------------------------
#  A list of all of the object files
#------------------------------------------------------------------------------
16OBJS =$(OBJDIR)\init16.obj $(OBJDIR)\dd.obj
32OBJS = $(OBJDIR)\driver.obj
TESTINIOBJS = $(OBJDIR)\testini.obj
CLIENTOBJS = $(OBJDIR)\client.obj

#------------------------------------------------------------------------------
#  Libraries
#------------------------------------------------------------------------------
TEST_LIBS = r0lib32.lib $(OS2386_LIB) $(OS2386P_LIB) tcpipdll.lib
TESTINI_LIBS = $(MTLIBS)
CLIENT_LIBS = $(MTLIB) $(OS2386_LIB) tcpip32.lib tcp32dll.lib

#------------------------------------------------------------------------------
#   This section lists all files to be built by make.
#------------------------------------------------------------------------------
all : $(EXEDIR)\test.sys $(EXEDIR)\testini.exe $(EXEDIR)\client.exe

#------------------------------------------------------------------------------
#   Executables
#------------------------------------------------------------------------------
$(EXEDIR)\testini.exe: $(TESTINIOBJS)
	-$(CREATE_PATH)
        $(LINK) $@ /B"/PM:VIO /MAP:FULL" $(TESTINI_LIBS) $(TESTINIOBJS)

$(EXEDIR)\client.exe: $(CLIENTOBJS)
	-$(CREATE_PATH)
        $(LINK) $@ /B"/PM:VIO /MAP:FULL" $(CLIENTOBJS) $(CLIENT_LIBS)

$(EXEDIR)\test.sys: $(16OBJS) $(32OBJS)
	-$(CREATE_PATH)
        $(LINK) $@ /B"/PDD /MAP:FULL" $(TEST_LIBS) $(16OBJS) $(32OBJS) test.def
        mapsym test

#------------------------------------------------------------------------------
#   Object modules
#------------------------------------------------------------------------------
$(OBJDIR)\client.obj: client.c
	-$(CREATE_PATH)
        $(CC) $(CFLAGS) $(CINCLUDE) -DTCPV40HDRS /c /Sm /Gm+ /Fo$(OBJDIR)\$(@B).obj client.c

$(OBJDIR)\init16.obj: init16.c
	-$(CREATE_PATH)
        cl -c $(C6CFLAGS) /Zc -I$(OS2_H16) -Fo$(OBJDIR)\$(@B).obj init16.c

$(OBJDIR)\dd.obj: dd.asm
	-$(CREATE_PATH)
        ml /c /Cx /W3 /Zm /Fo$(OBJDIR)\dd.obj dd.asm

$(OBJDIR)\driver.obj: driver.c
	-$(CREATE_PATH)
        $(CC) $(CFLAGS) $(CINCLUDE) /c /Rn /Gr+ /Fo$(OBJDIR)\$(@B).obj driver.c

$(OBJDIR)\testini.obj: testini.c
	-$(CREATE_PATH)
        $(CC) $(CFLAGS) $(CINCLUDE) /DKERNEL /W3 /DOS2 /c /Ss /Ms /G4 /Fa$*.cod /Fo$(OBJDIR)\$(@B).obj testini.c


clean :
        @if exist $(OBJDIR)\*.obj del $(OBJDIR)\*.obj
        @if exist $(EXEDIR)\*.sys del $(EXEDIR)\*.sys
        @if exist $(EXEDIR)\*.exe del $(EXEDIR)\*.exe
        @if exist *.cod del *.cod
        @if exist *.map del *.map
        @if exist *.sym del *.sym
