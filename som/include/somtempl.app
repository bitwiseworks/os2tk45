:somapp_sccsid
@(#) 1.3 src/somc/somapp/somtempl.app, somtempl, som2.1 12/19/95 10:51:42 [7/30/96 15:26:41]

:symtest
        SOM_CLASS_NAME            "__SOM_CLASS_NAME__"
        SOM_META_FILES            "__SOM_META_FILES__"
        SOM_META_NAMES            "__SOM_META_NAMES__"
        SOM_PARENT_FILES          "__SOM_PARENT_FILES__"
        SOM_PARENT_NAMES          "__SOM_PARENT_NAMES__"
        SOM_STEM                  "__SOM_STEM__"
        SOM_OBJS                  "__SOM_OBJS__"
        SOM_OBJS_RESPONSE         "__SOM_OBJS_RESPONSE__"
        SOM_OBJS_C_DEPS           "__SOM_OBJS_C_DEPS__"
        SOM_OBJS_CPP_DEPS         "__SOM_OBJS_CPP_DEPS__"
        SOM_INITBODY              "__SOM_INITBODY__"
        SOM_C_HEADER_FILES        "__SOM_C_HEADER_FILES__"
        SOM_C_HEADER_DELETE_FILES "__SOM_C_HEADER_DELETE_FILES__"
        SOM_CPP_HEADER_FILES        "__SOM_CPP_HEADER_FILES__"
        SOM_CPP_HEADER_DELETE_FILES "__SOM_CPP_HEADER_DELETE_FILES__"
        SOM_EXPORT_C_NAMES        "__SOM_EXPORT_C_NAMES__"
        SOM_EXPORT_PASCAL_NAMES   "__SOM_EXPORT_PASCAL_NAMES__"
        SOM_META_CLASS_NAME       "__SOM_META_CLASS_NAME__"
        SOM_PARENT_CLASS_NAME     "__SOM_PARENT_CLASS_NAME__"
        SOM_DLLSTEM               "__SOM_DLLSTEM__"
:idl_dll_Makefile
# AIX xlc Makefile for __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__.dll
TARGETS    = $(TARGET) lib__SOM_STEM__.a
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

AR         = ar
CC         = cc
CDEBUG     = -g
LDEBUG     =
CFLAGS     = $(CDEBUG)
LDFLAGS    = -bM:SRE -T512 -H512
LIBLIST    = -lsomtk
LINKER     = ld
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh:ih
EMITC      = -sc
DELETE     = rm -f

.SUFFIXES: .idl .ih .h .sh .c

.c.o:
	$(CC) -I. -I$(SOMBASE)/include $(CFLAGS) -c $<

.idl.ih:
	$(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
	$(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
	$(SC) $(SCFLAGS) $(EMITC) $<

.sh:
	cp $? $@; chmod 0755 $@

all: $(TARGETS)

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__.exp
	$(LINKER) -o $@ -e SOMInitModule $(OBJS) -bE:__SOM_STEM__.exp -lc $(LDFLAGS) -L$(SOMBASE)/lib $(LIBLIST)

lib__SOM_STEM__.a: __SOM_STEM__.dll __SOM_STEM__.exp
	$(AR) ruv $@ __SOM_STEM__.exp

clean:
	$(DELETE) som.ir *.ih *.o $(TARGETS)
__SOM_C_HEADER_DELETE_FILES__
:idl_dll_makefile.32
# OS/2 C Set Makefile for __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__.dll
TARGETS    = $(TARGET) __SOM_STEM__.lib
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

CC         = icc
CDEBUG     = -Ti+
LDEBUG     = /debug
CFLAGS     = -W3 -c $(CDEBUG) -Q+ -Ss+ -Gd- -Gm+ -Gs+ -Sm
LDFLAGS    = /exepack /packd /packc /align:4 /stack:100000 /pm:vio /noe /noi /nol $(LDEBUG)
LIBLIST    = somtk.lib
LINKER     = link386
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
	$(CC) -fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -Ge- $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: $(TARGETS)

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__.def
	$(LINKER) $(LDFLAGS) @<<
$(OBJS: = +^
)
$@
nul
$(LIBLIST)
__SOM_STEM__.def
\<<

__SOM_STEM__.lib: __SOM_STEM__.dll __SOM_STEM__.def
	implib __SOM_STEM__.lib __SOM_STEM__.def

clean:
	-$(DELETE) som.ir
	-$(DELETE) $(TARGET)
	-$(DELETE) __SOM_STEM__.lib
	-$(DELETE) *.ih
	-$(DELETE) *.o
__SOM_C_HEADER_DELETE_FILES__
:idl_dll_makefile.w16
# Windows Visual C++ Makefile for __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__.dll
TARGETS    = $(TARGET) __SOM_STEM__.lib
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

CC         = cl
CDEBUG     = -Zi
LDEBUG     = /CO
CFLAGS     = -nologo -Zp1 -Oi $(CDEBUG)
LDFLAGS    = /nologo /ONERROR:NOEXE /nod $(LDEBUG)
LIBLIST    = llibcew libw $(SOMBASE)\lib\somtk
LIBLISTD   = ldllcew libw $(SOMBASE)\lib\somtk
LINKER     = link
SC         = somc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
	$(CC) -c -Fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -ALw $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: $(TARGETS)

som__SOM_STEM__.o: som__SOM_STEM__.c __SOM_STEM__.h
	$(CC) -Fo$@ -I. -I$(SOMBASE)\include $(CFLAGS) -c -AL som__SOM_STEM__.c

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__w16.def
	$(LINKER) $(LDFLAGS) @<<
$(OBJS: = +^
)
__SOM_STEM__.dll
nul
$(LIBLISTD)
__SOM_STEM__w16.def
\<<

__SOM_STEM__.lib: __SOM_STEM__.dll __SOM_STEM__w16.def
	implib /nologo __SOM_STEM__.lib __SOM_STEM__w16.def

clean:
	-$(DELETE) som.ir
	-$(DELETE) $(TARGET)
	-$(DELETE) __SOM_STEM__.lib
	-$(DELETE) __SOM_STEM__.dll
	-$(DELETE) *.ih
	-$(DELETE) *.o
__SOM_C_HEADER_DELETE_FILES__
:idl_dll___SOM_STEM__.def
LIBRARY __SOM_STEM__ INITINSTANCE
DESCRIPTION '__SOM_CLASS_NAME__ Class Library'
PROTMODE
DATA MULTIPLE NONSHARED LOADONCALL
EXPORTS
SOMInitModule
__SOM_EXPORT_PASCAL_NAMES__
:idl_dll___SOM_STEM__.exp
#! __SOM_STEM__.dll
SOMInitModule
__SOM_EXPORT_PASCAL_NAMES__
:idl_dll___SOM_STEM__.idl
#ifndef __SOM_STEM___idl
#define __SOM_STEM___idl

__SOM_PARENT_FILES__
__SOM_META_FILES__

interface __SOM_CLASS_NAME__ : __SOM_PARENT_NAMES__
{
  // Attributes filled in here:

  // Operations filled in here:

#ifdef __SOMIDL__
  implementation {
    releaseorder:;

    // Class Modifiers:
    align     = 4;
    dllname   = "__SOM_DLLSTEM__.dll";
    __SOM_META_NAMES__

    // Attribute Modifiers:

    // Overrides:
    somDefaultInit: override, init;     // Default object initializer
    somDestruct: override;              // Default object uninitializer
  };
#endif /* __SOMIDL__ */
};
#endif  /* __SOM_STEM___idl */
:idl_dll___SOM_STEM__init.c
/*
 * Module name: __SOM_STEM__init.c
 *
 * Description: This source file contains functions used during the loading
 *              and initialization of the __SOM_DLLSTEM__.dll class library.
 *
 *
 */
#ifdef __IBMC__
#pragma linkage(SOMInitModule, system)
#endif

__SOM_C_HEADER_FILES__

void SOMLINK SOMInitModule(integer4 majorVersion, integer4 minorVersion, string ignore)
{
__SOM_INITBODY__
}

#ifdef _WIN16
#include <windows.h>
int CALLBACK LibMain (HINSTANCE inst, WORD ds, WORD heapSize, LPSTR cmdLine)
{
    SOM_IgnoreWarning (inst);
    SOM_IgnoreWarning (ds);
    SOM_IgnoreWarning (heapSize);
    SOM_IgnoreWarning (cmdLine);

    SOM_ClassLibrary ("__SOM_STEM__.dll");
    return 1;
}
#endif
/*
 * 10H9767, 25H7912 (C) COPYRIGHT International Business Machines Corp. 1992,1996 
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:idl_dll___SOM_STEM__w16.def
LIBRARY         __SOM_STEM__
DESCRIPTION     '__SOM_CLASS_NAME__ Class Library'
EXETYPE         WINDOWS
PROTMODE
STUB            'WINSTUB.EXE'
DATA            MOVEABLE SINGLE SHARED LOADONCALL
CODE            MOVEABLE DISCARDABLE LOADONCALL
HEAPSIZE        4096

EXPORTS
__SOM_EXPORT_C_NAMES__
:dsom_README
This directory contains a simple template DSOM application composed of the
following files. See main.c for a description of the application.

Makefile      - Makefile for AIX
makefile.32   - Makefile for OS/2 (IBM C Set)
makefile.w16  - Makefile ofr DOS/Win (MSVC)

chk__SOM_STEM__.bat   - DOS/Win exec to check environment variables
chk__SOM_STEM__.cmd   - OS/2    "    "  "     "           "
chk__SOM_STEM__.sh    - AIX     "    "  "     "           "

local.bat     - DOS/Win exec to set up for objects local to the client process.
local.cmd     - OS/2    "    "  "   "  "   "       "     "  "   "      "
local.sh      - AIX     "    "  "   "  "   "       "     "  "   "      "

remote.bat    - DOS/Win exec to set up for remote DSOM objects.
remote.cmd    - OS/2    "    "  "   "  "   "      "    "
remote.sh     - AIX     "    "  "   "  "   "      "    "

som__SOM_STEM__.c     - Source of utility functions to aid local/remote transparency.
som__SOM_STEM__.h     - Header for utility functions.
main.c        - Main client program.
__SOM_STEM__cli.def   - DOS/Win module definition file for client program.

__SOM_STEM__.idl      - The class definition of the __SOM_CLASS_NAME__ class.
__SOM_STEM__.c        - The implementation of the __SOM_CLASS_NAME__ class.
__SOM_STEM__init.c    - Class library initialization module.
__SOM_STEM__.exp      - Class library export list for AIX.
__SOM_STEM__.def      - Class library export list for OS/2.
__SOM_STEM__w16.def   - Class library export list for DOS/Win.

To build this application, run make or nmake on the make file appropriate
for your system.

   On AIX:    make
   On OS/2:   nmake -f makefile.32
   On Win:    nmake -f makefile.w16

The make file will also run the chk__SOM_STEM__ command to check your environment
and report potential setup problems.

The make files produce two executables: __SOM_STEM__.dll and __SOM_STEM__cli[.exe] The dll
file is the class library.  __SOM_STEM__cli is the client program.  Following the
building of the executables and prior to running the client program, the
"local" or "remote" command should be run to set up the application to use
either local SOM objects or remote DSOM objects.  The application is written
such that the location is transparent to it.  The remote command registers
the object implementation in the Implementation Repository.  The remote
command takes one arguement, the remote host name.  If no argument is given,
the local system is assumed to be the system on which the DSOM server will
run.  The DSOM server is the process that contains the remote objects.  You
must ensure that the DSOM deamon somdd is running on the server system and
that the class library __SOM_STEM__.dll can be located via the LIBPATH (or PATH on
DOS/Win).

The local command reverses the operation done by the remote command by
removing the object implementation from the Implementation Repository.  The
remote command can not be run again without first running the local command.

To clean up the directory of everything but source files, run make and
specify the "clean" target:

   On AIX:    make clean
   On OS/2:   nmake -f makefile.32 clean
   On Win:    nmake -f makefile.w16 clean
:dsom_Makefile
# AIX xlc Makefile for DSOM __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__cli
TARGETS    = chk__SOM_STEM__ $(TARGET) __SOM_STEM__.dll lib__SOM_STEM__.a local remote
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

AR         = ar
CC         = cc
CDEBUG     = -g
LDEBUG     =
CFLAGS     = $(CDEBUG)
LDFLAGS    = -bM:SRE -T512 -H512
LIBLIST    = -lsomtk
LINKER     = ld
RUNSHELL   = ./chk__SOM_STEM__
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh:ih
EMITC      = -sc
DELETE     = rm -f

.SUFFIXES: .idl .ih .h .sh .c

.c.o:
	$(CC) -I. -I$(SOMBASE)/include $(CFLAGS) -c $<

.idl.ih:
	$(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
	$(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
	$(SC) $(SCFLAGS) $(EMITC) $<

.sh:
	cp $? $@; chmod 0755 $@

all: $(TARGETS) sregimpl

chk__SOM_STEM__: chk__SOM_STEM__.sh
	cp $? $@; chmod 0755 $@
	$(RUNSHELL)

sregimpl:
	@echo Run the 'remote' command to make use of non-local DSOM objects.
	@echo Or, run the 'local' command to make use of local SOM objects.
	@touch $@

$(TARGET): lib__SOM_STEM__.a main.o som__SOM_STEM__.o
	$(CC) $(LDFLAGS) -o $@ main.o som__SOM_STEM__.o -L$(SOMBASE)/lib lib__SOM_STEM__.a $(LIBLIST)
	@echo Run $(TARGET) to execute the client program.

main.o: main.c __SOM_STEM__.h

som__SOM_STEM__.o: som__SOM_STEM__.c som__SOM_STEM__.h __SOM_STEM__.h

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__.exp
	$(LINKER) -o $@ -e SOMInitModule $(OBJS) -bE:__SOM_STEM__.exp -lc $(LDFLAGS) -L$(SOMBASE)/lib $(LIBLIST)

lib__SOM_STEM__.a: __SOM_STEM__.dll __SOM_STEM__.exp
	$(AR) ruv $@ __SOM_STEM__.exp

clean: local.sh
	-local
	$(DELETE) *.ih *.o $(TARGETS)
	$(DELETE) sregimpl core
__SOM_C_HEADER_DELETE_FILES__
:dsom_makefile.32
# OS/2 C Set Makefile for DSOM __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__cli.exe
TARGETS    = $(TARGET) __SOM_STEM__.dll __SOM_STEM__.lib
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

CC         = icc
CDEBUG     = -Ti+
LDEBUG     = /debug
CFLAGS     = -W3 -c $(CDEBUG) -Q+ -Ss+ -Gd- -Gm+ -Gs+ -Sm
LDFLAGS    = /exepack /packd /packc /align:4 /stack:100000 /pm:vio /noe /noi /nol $(LDEBUG)
LIBLIST    = somtk.lib
LINKER     = link386
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
        $(CC) -fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -Ge- $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: runchk $(TARGETS) sregimpl

runchk: chk__SOM_STEM__.cmd
        chk__SOM_STEM__

sregimpl:
        @echo Run the 'remote' command to make use of non-local DSOM objects.
        @echo Or, run the 'local' command to make use of local SOM objects.
        echo $@ > $@

$(TARGET): __SOM_STEM__.lib main.o som__SOM_STEM__.o
        $(LINKER) $(LDFLAGS) main.o som__SOM_STEM__.o,$@,NUL,$(LIBLIST)+__SOM_STEM__;
        @echo Run $(TARGET) to execute the client program.

main.o: main.c __SOM_STEM__.h
        $(CC) -fo$@  $(CFLAGS) -Ge+ main.c

som__SOM_STEM__.o: som__SOM_STEM__.c __SOM_STEM__.h
        $(CC) -fo$@  $(CFLAGS) -Ge+ som__SOM_STEM__.c

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__.def
        $(LINKER) $(LDFLAGS) @<<
$(OBJS: = +^
)
$@
nul
$(LIBLIST)
__SOM_STEM__.def
\<<

__SOM_STEM__.lib: __SOM_STEM__.dll __SOM_STEM__.def
        implib /nologo __SOM_STEM__.lib __SOM_STEM__.def

clean:
        -local
        -$(DELETE) sregimpl
        -$(DELETE) $(TARGET)
        -$(DELETE) __SOM_STEM__.lib
        -$(DELETE) __SOM_STEM__.dll
        -$(DELETE) *.ih
        -$(DELETE) *.o
        __SOM_C_HEADER_DELETE_FILES__
:dsom_local.bat
@ECHO OFF
regimpl -D -i __SOM_STEM__Server
echo Ignore any "alias name not found" errors.
echo __SOM_CLASS_NAME__ objects are now local to the client process.
:dsom_local.cmd
@ECHO OFF
regimpl -D -i __SOM_STEM__Server
echo Ignore any "alias name not found" errors.
echo __SOM_CLASS_NAME__ objects are now local to the client process.
:dsom_local.sh
#!/bin/sh -f
regimpl -D -i __SOM_STEM__Server
echo Ignore any \"alias name not found\" errors.
echo __SOM_CLASS_NAME__ objects are now local to the client process.
:dsom_main.c
/*
 * Module name: main.c
 *
 * Description: This is a simple program that encompasses the major elements
 *              of a SOM/DSOM program. The program is written using a set
 *              of general purpose functions (in som__SOM_STEM__.c) such that the
 *              main program below can be run using either a local SOM object
 *              or a remote (DSOM) object.
 *
 *              In order to provide local/remote object transparency, objects
 *              used in the program are always created via a class object.
 *              The class object is either a local object or a proxy for a
 *              class object in the remote server process.
 *
 *              Because an object's creation (for DSOM) results in the creation
 *              of an object in another process, a DSOM program should not
 *              terminate without dealing with any non-local objects.
 *              This program tracks all application objects created and
 *              destroys them in the remote process prior to termination.
 *              (Note: this behavior would be inappropriate if the program
 *              was sharing its remote objects with other DSOM programs.)
 *
 */

#include <somd.h>
#include "__SOM_STEM__.h"
#include "som__SOM_STEM__.h"

#ifndef _WIN16
int main (int argc, char *argv[])
{
    SOMClass cls;
    __SOM_CLASS_NAME__ obj;
    string name;
    Environment *ev;
    int i, status = 0, max = (argc == 1) ? 1 : atoi(argv[1]);
    char buf[MAX_STRING_SIZE];

    somEnvironmentNew();
    ev = SOM_CreateLocalEnvironment();
    cls = __SOM_CLASS_NAME__Initialize(ev);

    if (EV_OK(ev)) {
       for (i = 0; i < max; i++) {
          obj = createObject(cls, ev);
          if (EV_OK(ev) && obj) {
             somPrintf("%s\n", name = __get_name(obj, ev));
             freeReturnedMem(name);
             sprintf(buf, "[%d] Hello", i);
             __set_name(obj, ev, buf);
             somPrintf("%s\n", name = __get_name(obj, ev));
             freeReturnedMem(name);
             _append(obj, ev, " World!");
             somPrintf("%s\n", name = __get_name(obj, ev));
             freeReturnedMem(name);
             freeObject(obj, ev);
          }
       }
    }
    else {
       somPrintf("Error creating __SOM_CLASS_NAME__ object.\n");
       status = -1;
    }

    SOM_DestroyLocalEnvironment(ev);
    __SOM_STEM__Shutdown();
    return (status);
}
#else
#include <windows.h>
#include <bccstruc.h>

long WINAPI WndProc(HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
    static __SOM_CLASS_NAME__ obj;
    static SOMClass cls;
    static Environment *ev;
    char *app_s = "__SOM_CLASS_NAME__ Application", *name;
    char *err_s = "Error creating __SOM_CLASS_NAME__ object.";

    if (message == WM_CREATE) {
        ev = SOM_CreateLocalEnvironment();
        cls = __SOM_CLASS_NAME__Initialize(ev);
        if (EV_OK(ev)) {
           obj = createObject(cls, ev);
           if (EV_OK(ev) && obj) {
              __set_name(obj, ev, "Hello");
              MessageBox(hwnd, name = __get_name(obj, ev), app_s, MB_OK);
              freeReturnedMem(name);
              _append(obj, ev, " World!");
              MessageBox(hwnd, name = __get_name(obj, ev), app_s, MB_OK);
              freeReturnedMem(name);
              MessageBox(hwnd, "Press ok to exit", app_s, MB_OK);
              freeObject(obj, ev);
           }
        }
        else {
           MessageBox(hwnd, err_s, app_s, MB_ICONEXCLAMATION);
        }

        SOM_DestroyLocalEnvironment(ev);
        __SOM_STEM__Shutdown();
        PostQuitMessage(0);
        return (0);
    }
    else
        return (DefWindowProc(hwnd, message, wParam, lParam));
}

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    static char szAppName[] = "__SOM_CLASS_NAME__";
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    SOM_MainProgram();
    if (!hPrevInstance) {
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon((HINSTANCE) NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
        wndclass.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        RegisterClass(&wndclass);
    }

    hwnd = CreateWindow(szAppName,           //window class name
                        "__SOM_CLASS_NAME__ Program",   //window caption
                        WS_OVERLAPPEDWINDOW, //window style
                        CW_USEDEFAULT,       //initial x position
                        CW_USEDEFAULT,       //initial y position
                        CW_USEDEFAULT,       //initial x size
                        CW_USEDEFAULT,       //initial y size
                        (HWND) NULL,         //parent window handle
                        (HMENU) NULL,        //window menu handle
                        hInstance,           //program instance handle
                        NULL);
    ShowWindow(hwnd, SW_SHOWMINIMIZED);

    while (GetMessage(&msg, (HWND) NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}
#endif
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:dsom_makefile.w16
# Windows Visual C++ Makefile for DSOM __SOM_STEM__
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__cli.exe
TARGETS    = $(TARGET) __SOM_STEM__.dll __SOM_STEM__.lib
OBJS       = __SOM_OBJS__ __SOM_STEM__init.o

CC         = cl
CDEBUG     = -Zi
LDEBUG     = /CO
CFLAGS     = -nologo -Zp1 -Oi $(CDEBUG)
LDFLAGS    = /nologo /ONERROR:NOEXE /nod $(LDEBUG)
LIBLIST    = llibcew libw $(SOMBASE)\lib\somtk
LIBLISTD   = ldllcew libw $(SOMBASE)\lib\somtk
LINKER     = link
SC         = somc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
        $(CC) -c -Fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -ALw $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: runchk $(TARGETS) sregimpl
        @echo Now go to windows to run $(TARGET) ...

runchk: chk__SOM_STEM__.bat
        chk__SOM_STEM__

sregimpl:
        @echo Run the 'remote' command to make use of non-local DSOM objects.
        @echo Or, run the 'local' command to make use of local SOM objects.
        echo $@ > $@

$(TARGET): __SOM_STEM__.lib main.o som__SOM_STEM__.o __SOM_STEM__cli.def
        $(LINKER) $(LDFLAGS) @<<
main.o som__SOM_STEM__.o
$@
NUL
$(LIBLIST) __SOM_STEM__.lib
__SOM_STEM__cli.def
\<<

main.o: main.c __SOM_STEM__.h
        $(CC) -Fo$@ -I. -I$(SOMBASE)\include $(CFLAGS) -c -AL main.c

som__SOM_STEM__.o: som__SOM_STEM__.c __SOM_STEM__.h
        $(CC) -Fo$@ -I. -I$(SOMBASE)\include $(CFLAGS) -c -AL som__SOM_STEM__.c

__SOM_OBJS_C_DEPS__
__SOM_STEM__init.o: __SOM_STEM__.h __SOM_STEM__init.c

__SOM_STEM__.dll: $(OBJS) __SOM_STEM__w16.def
        $(LINKER) $(LDFLAGS) @<<
$(OBJS: = +^
)
$@
nul
$(LIBLISTD)
__SOM_STEM__w16.def
\<<

__SOM_STEM__.lib: __SOM_STEM__.dll __SOM_STEM__w16.def
        implib /nologo __SOM_STEM__.lib __SOM_STEM__w16.def

clean:
        -$(DELETE) sregimpl
        -$(DELETE) $(TARGET)
        -$(DELETE) __SOM_STEM__.lib
        -$(DELETE) __SOM_STEM__.dll
        -$(DELETE) *.ih
        -$(DELETE) *.o
        __SOM_C_HEADER_DELETE_FILES__
:dsom___SOM_STEM__.c
/*
 * Module name: __SOM_STEM__.c
 *
 * Description: This source file contains the implementation of the
 *              __SOM_CLASS_NAME__ class.
 *
 *
 */

#ifndef SOM_Module___SOM_STEM___Source
#define SOM_Module___SOM_STEM___Source
#endif
#define __SOM_CLASS_NAME___Class_Source

#include <somd.h>
#include "__SOM_STEM__.ih"


/*
 *Method from the IDL attribute statement:
 *"attribute string name"
 */

SOM_Scope void  SOMLINK __SOM_STEM____set_name(__SOM_CLASS_NAME__ somSelf,
                                        Environment *ev, string name)
{
    __SOM_CLASS_NAME__Data *somThis = __SOM_CLASS_NAME__GetData(somSelf);
    __SOM_CLASS_NAME__MethodDebug("__SOM_CLASS_NAME__","__SOM_STEM____set_name");

    if (_name)
       SOMFree(_name);
    if (!name)
       name = "";
    _name = strcpy((string)SOMMalloc(strlen(name)+1), name);
}

/*
 *Method from the IDL attribute statement:
 *"attribute string name"
 */

SOM_Scope string  SOMLINK __SOM_STEM____get_name(__SOM_CLASS_NAME__ somSelf,
                                          Environment *ev)
{
    string name;
    __SOM_CLASS_NAME__Data *somThis = __SOM_CLASS_NAME__GetData(somSelf);
    __SOM_CLASS_NAME__MethodDebug("__SOM_CLASS_NAME__","__SOM_STEM____get_name");

    if (!_name)
       _name = "";
    name = strcpy((string)SOMMalloc(strlen(_name)+1), _name);
    return(name); /* caller frees storage */
}

/*
 * The append method concatenates the given <name> with this object's
 * name attribute.
 */

SOM_Scope void  SOMLINK __SOM_STEM___append(__SOM_CLASS_NAME__ somSelf,
                                     Environment *ev, string name)
{
    char buf[MAX_STRING_SIZE];
    string name1;
    __SOM_CLASS_NAME__Data *somThis = __SOM_CLASS_NAME__GetData(somSelf);
    __SOM_CLASS_NAME__MethodDebug("__SOM_CLASS_NAME__","__SOM_STEM___append");

    if (!name)
       name = "";
    if (!(name1 = __get_name(somSelf, ev)))
       name1 = "";

    strcpy(buf, name1);
    strcat(buf, name);
    __set_name(somSelf, ev, buf);
}


/*
 * Default object initializer
 */

SOM_Scope void SOMLINK __SOM_STEM___somDefaultInit(__SOM_CLASS_NAME__ somSelf,
                                           somInitCtrl* ctrl)
{
    __SOM_CLASS_NAME__Data *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    __SOM_CLASS_NAME__MethodDebug("__SOM_CLASS_NAME__","somDefaultInit");
    __SOM_CLASS_NAME___BeginInitializer_somDefaultInit;

    __SOM_CLASS_NAME___Init___SOM_PARENT_CLASS_NAME___somDefaultInit(somSelf, ctrl);

    /*
     * local __SOM_CLASS_NAME__ initialization code added by programmer
     */
    _name = (string)NULL;
}



/*
 * Default object uninitializer
 */

SOM_Scope void SOMLINK __SOM_STEM___somDestruct(__SOM_CLASS_NAME__ somSelf,
                                        octet doFree, somDestructCtrl* ctrl)
{
    __SOM_CLASS_NAME__Data *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    __SOM_CLASS_NAME__MethodDebug("__SOM_CLASS_NAME__","__SOM_STEM___somDestruct");
    __SOM_CLASS_NAME___BeginDestructor;

    /*
     * local __SOM_CLASS_NAME__ deinitialization code added by programmer
     */
    if (_name)
       SOMFree(_name);

    __SOM_CLASS_NAME___EndDestructor;
}
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:dsom___SOM_STEM__.def
LIBRARY __SOM_STEM__ INITINSTANCE
DESCRIPTION '__SOM_CLASS_NAME__ Class Library'
PROTMODE
DATA MULTIPLE NONSHARED LOADONCALL
EXPORTS
SOMInitModule
__SOM_EXPORT_PASCAL_NAMES__
:dsom___SOM_STEM__.exp
#! __SOM_STEM__.dll
SOMInitModule
__SOM_EXPORT_PASCAL_NAMES__
:dsom___SOM_STEM__.idl
#ifndef __SOM_STEM___idl
#define __SOM_STEM___idl

//
//  Interface : __SOM_CLASS_NAME__
//
//  Description : Simple distributed SOM object class definition.
//

__SOM_PARENT_FILES__
__SOM_META_FILES__

interface __SOM_CLASS_NAME__ : __SOM_PARENT_NAMES__
{
  const long MAX_STRING_SIZE = 1024;
  attribute string name;

  void append(in string name);
  // The append method concatenates the given <name> with this object's
  // name attribute.

#ifdef __SOMIDL__
  implementation {
    releaseorder: _get_name, _set_name, append;

    // Class Modifiers:
    align     = 4;               // instance data aligned on 4 byte boundary
    memory_management = corba;   // caller owns all returned memory
    dllname   = "__SOM_DLLSTEM__.dll";      // loadable library of this class
    functionprefix = __SOM_STEM___;      // unique method procedure names
    __SOM_META_NAMES__

    // Attribute Modifiers:
    name: noget,
          noset; // string attribute methods must be written by user
                 // for "corba" memory_management policy.

    // Overrides:
    somDefaultInit: override, init;     // Default object initializer
    somDestruct: override;              // Default object uninitializer

  };
#endif /* __SOMIDL__ */
};

#endif  /* __SOM_STEM___idl */
:dsom___SOM_STEM__cli.def
NAME           __SOM_CLASS_NAME__CLI
DESCRIPTION    '__SOM_CLASS_NAME__ Client Program'
EXETYPE        WINDOWS
STUB           'WINSTUB.EXE'
CODE           PRELOAD MOVEABLE DISCARDABLE
DATA           PRELOAD MOVEABLE MULTIPLE
HEAPSIZE       5000
STACKSIZE      20000
:dsom___SOM_STEM__init.c
/*
 * Module name: __SOM_STEM__init.c
 *
 * Description: This source file contains functions used during the loading
 *              and initialization of the __SOM_STEM__.dll class library.
 *
 *
 */
#ifdef __IBMC__
#pragma linkage(SOMInitModule, system)
#endif

__SOM_C_HEADER_FILES__

void SOMLINK SOMInitModule(integer4 majorVersion, integer4 minorVersion, string ignore)
{
__SOM_INITBODY__
}

#ifdef _WIN16
#include <windows.h>
int CALLBACK LibMain (HINSTANCE inst, WORD ds, WORD heapSize, LPSTR cmdLine)
{
    SOM_IgnoreWarning (inst);
    SOM_IgnoreWarning (ds);
    SOM_IgnoreWarning (heapSize);
    SOM_IgnoreWarning (cmdLine);

    SOM_ClassLibrary ("__SOM_STEM__.dll");
    return 1;
}
#endif
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:dsom___SOM_STEM__w16.def
LIBRARY         __SOM_STEM__
DESCRIPTION     '__SOM_CLASS_NAME__ Class Library'
EXETYPE         WINDOWS
PROTMODE
STUB            'WINSTUB.EXE'
DATA            MOVEABLE SINGLE SHARED LOADONCALL
CODE            MOVEABLE DISCARDABLE LOADONCALL
HEAPSIZE        4096

EXPORTS
__SOM_EXPORT_C_NAMES__
:dsom_remote.bat
@ECHO OFF
REM This program takes one argument, the name of the remote system of the
REM server process. The system defaults to the local host.
if "%1" == "" goto default
REM ------------- Non local system setup ----------------------------------
regimpl -A -h %1 -i __SOM_STEM__Server
if errorlevel 1 goto failed
echo Implementation registered for the %1 system.
echo Please verify that somdd is running on the %1 system.
goto continue
REM ------------- local system setup --------------------------------------
\:default
regimpl -A -i __SOM_STEM__Server
if errorlevel 1 goto failed
echo Implementation registered for this system.
echo Please verify that somdd is running on this system.
\:continue
regimpl -a -i __SOM_STEM__Server -c __SOM_CLASS_NAME__
regimpl -L -i __SOM_STEM__Server
regimpl -l -i __SOM_STEM__Server
goto done
\:failed
echo The object implementation registration failed.
\:done
:dsom_remote.cmd
@ECHO OFF
REM This program takes one argument, the name of the remote system of the
REM server process. The system defaults to the local host.
if "%1" == "" goto default
REM ------------- Non local system setup ----------------------------------
regimpl -A -h %1 -i __SOM_STEM__Server
if errorlevel 1 goto failed
echo Implementation registered for the %1 system.
echo Please verify that somdd is running on the %1 system.
goto continue
REM ------------- local system setup --------------------------------------
\:default
regimpl -A -i __SOM_STEM__Server
if errorlevel 1 goto failed
echo Implementation registered for this system.
echo Please verify that somdd is running on this system.
\:continue
regimpl -a -i __SOM_STEM__Server -c __SOM_CLASS_NAME__
regimpl -L -i __SOM_STEM__Server
regimpl -l -i __SOM_STEM__Server
goto done
\:failed
echo The object implementation registration failed.
\:done
:dsom_remote.sh
#!/bin/sh -f
# This program takes one argument, the name of the remote system of the
# server process. The system defaults to the local host.
sys=${1:-`hostname`}
if regimpl -A -h $sys -i __SOM_STEM__Server
then
    regimpl -a -i __SOM_STEM__Server -c __SOM_CLASS_NAME__
    echo Implementation registered for the $sys system.
    echo Please verify that somdd is running on the $sys system.
    regimpl -L -i __SOM_STEM__Server
    regimpl -l -i __SOM_STEM__Server
fi
:dsom_chk__SOM_STEM__.bat
@ECHO OFF
REM Quick check of a few important variables...
IF "%SOMBASE%" == "" echo SOMBASE must be set to SOM install directory. Usually c:\som.
IF "%SMINCLUDE%" == "" echo SMINCLUDE must be set to include %sombase%\include.
IF "%INCLUDE%" == "" echo INCLUDE must be set to include %sombase%\include.
IF "%LIB%" == "" echo LIB must be set to include %sombase%\lib.
IF "%SOMIR%" == "" echo SOMIR must be set for DSOM. Typically %sombase%\etc\som.ir;som.ir
IF "%SOMSOCKETS%" == "" echo SOMSOCKETS must be set for remote DSOM. Example: TCPIPSockets
IF "%USER%" == "" echo USER must be set for DSOM.
IF "%HOSTNAME%" == "" echo HOSTNAME must be set for DSOM.
IF "%SOMDDIR%" == "" echo SOMDDIR should be set for DSOM. Defaults to %sombase%\etc\dsom.
:dsom_chk__SOM_STEM__.cmd
@ECHO OFF
REM Quick check of a few important variables...
IF "%SOMBASE%" == "" echo SOMBASE must be set to SOM install directory. Usually c:\som. & goto pgmchk
IF "%SMINCLUDE%" == "" echo SMINCLUDE must be set to include %sombase%\include. & set chk__SOM_STEM__=1
IF "%INCLUDE%" == "" echo INCLUDE must be set to include %sombase%\include. & set chk__SOM_STEM__=1
IF "%LIB%" == "" echo LIB must be set to include %sombase%\lib. & set chk__SOM_STEM__=1
IF "%SOMIR%" == "" echo SOMIR must be set for DSOM. Typically %sombase%\etc\som.ir;som.ir & set chk__SOM_STEM__=1
IF "%SOMSOCKETS%" == "" echo SOMSOCKETS must be set for remote DSOM. Example: TCPIPSockets & set chk__SOM_STEM__=1
IF "%USER%" == "" echo USER must be set for DSOM. & set chk__SOM_STEM__=1
IF "%HOSTNAME%" == "" echo HOSTNAME must be set for DSOM. & set chk__SOM_STEM__=1
IF "%SOMDDIR%" == "" echo SOMDDIR should be set for DSOM. Defaults to %sombase%\etc\dsom. & set chk__SOM_STEM__=1
IF NOT "%CHK__SOM_STEM__%" == "1" goto done
\:pgmchk
echo You may want to run the SOMDCHK program to check your environment further.
\:done
SET CHK__SOM_STEM__=
:dsom_chk__SOM_STEM__.sh
#!/bin/sh -f
# Quick check of a few important variables...
chk__SOM_STEM__=0
if [ -z "$SOMBASE" ]
then
    echo SOMBASE must be set to SOM install directory. Usually /usr/lpp/som.
    chk__SOM_STEM__=1
else
    [ -z "$SMINCLUDE" ]  && echo SMINCLUDE must be set to include $SOMBASE/include. && chk__SOM_STEM__=1
    [ -z "$INCLUDE" ]    && echo INCLUDE must be set to include $SOMBASE/include. && chk__SOM_STEM__=1
    [ -z "$LIB" ]        && echo LIB must be set to include $SOMBASE/lib. && chk__SOM_STEM__=1
    [ -z "$SOMIR" ]      && echo SOMIR must be set for DSOM. Typically $SOMBASE'/etc/som.ir:som.ir' && chk__SOM_STEM__=1
    [ -z "$SOMSOCKETS" ] && echo SOMSOCKETS must be set for remote DSOM. Example: TCPIPSockets && chk__SOM_STEM__=1
    [ -z "$USER" ]       && echo USER must be set for DSOM. && chk__SOM_STEM__=1
    [ -z "$HOSTNAME" ]   && echo HOSTNAME must be set for DSOM. && chk__SOM_STEM__=1
    [ -z "$SOMDDIR" ]    && echo SOMDDIR should be set for DSOM. Defaults to $SOMBASE/etc/dsom. && chk__SOM_STEM__=1
fi
if [ $chk__SOM_STEM__ -eq 1 ]
then
   echo You may want to run the SOMDCHK program to check your environment further.
fi
:dsom_som__SOM_STEM__.h
#include <som.h>

/* Types */

/* For building a list of objects */
typedef
   struct objectList {
             SOMObject obj;
             struct objectList *next;
          } *objectList;

/* Globals */
SOMEXTERN boolean __SOM_STEM__isdsom;     /* == TRUE when process using DSOM */
SOMEXTERN boolean __SOM_STEM__inShutdown; /* == TRUE when process in shutdown */
SOMEXTERN objectList __SOM_STEM__Objs;   /* list head */
SOMEXTERN objectList __SOM_STEM__ObjsTl; /* list tail */

/* Functions */
void printDSOMEv(Environment *ev, boolean fatal);
SOMClass __SOM_CLASS_NAME__Initialize(Environment *ev);
void freeReturnedMem(void *p);
SOMObject createObject(SOMClass cls, Environment *ev);
void freeObject(SOMObject obj, Environment *ev);
void addObjectToShutdownList(SOMObject obj);
void removeObjectFromShutdownList(SOMObject obj);
void __SOM_STEM__SigHandler(int sig);
void __SOM_STEM__Init(Environment *ev);
void __SOM_STEM__Shutdown(void);

/* Macros */
#define SHUTDOWN (__SOM_STEM__inShutdown == TRUE)
#define USING_DSOM (__SOM_STEM__isdsom == TRUE)
#define FATAL ((boolean)1)
#define NOT_FATAL ((boolean)0)
#define EV_OK(ev) ((ev)->_major == NO_EXCEPTION)
#define EV_NOT_OK(ev) ((ev)->_major != NO_EXCEPTION)
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:dsom_som__SOM_STEM__.c
/*
 * Module name: som__SOM_STEM__.c
 *
 * Description: This source file contains serveral general purpose functions
 *              that can be used to program SOM objects without regard to
 *              whether they are local or remote.
 *
 *              Functions included are:
 *
 *              __SOM_CLASS_NAME__Initialize - init and return class
 *              printDSOMEv - prints a SOM or DSOM exception
 *              createObject - creates an object from a given class
 *              freeObject - frees an object (local and remote)
 *              freeReturnedMem - frees memory returned from a method call
 *              addObjectToShutdownList - adds to set of objects to shutdown
 *              removeObjectFromShutdownList - removes from set of " " "
 *              __SOM_STEM__Init - initialize application
 *              __SOM_STEM__Shutdown - shutdown the application, free app objects
 *              __SOM_STEM__SigHandler - handle termination signals.
 *
 */

#include <somd.h>
#include "__SOM_STEM__.h"
#include "som__SOM_STEM__.h"
#include <signal.h>

/* Globals */
SOMEXTERN boolean __SOM_STEM__isdsom = FALSE;
SOMEXTERN boolean __SOM_STEM__inShutdown = FALSE;
SOMEXTERN objectList __SOM_STEM__ShutdownList = (objectList)NULL;   /* list head */
SOMEXTERN objectList __SOM_STEM__ShutdownListTl = (objectList)NULL; /* list tail */

/*
 * __SOM_STEM__SigHandler handles signals for the application.
 *
 */
void __SOM_STEM__SigHandler(int sig)
{
    char buf[16];
    Environment ev;
    SOM_InitEnvironment(&ev);
    __SOM_STEM__Shutdown();
    exit(sig);
}

/*
 * __SOM_STEM__Init prepares the process to start running. It may be called
 * multiple times without side effects.
 *
 */
static boolean initDone = FALSE;
void __SOM_STEM__Init(Environment *ev)
{
    if (!initDone) {
       initDone = TRUE;
       atexit(__SOM_STEM__Shutdown);

#ifndef _WIN16
       #ifdef SIGABRT
       signal(SIGABRT,  __SOM_STEM__SigHandler);  /* abnormal termination */
       #endif

       #ifdef SIGBREAK
       signal(SIGBREAK, __SOM_STEM__SigHandler);  /* Ctrl-break */
       #endif

       #ifdef SIGINT
       signal(SIGINT,   __SOM_STEM__SigHandler);  /* Ctrl-c */
       #endif

       #ifdef SIGQUIT
       signal(SIGQUIT,  __SOM_STEM__SigHandler);  /* Quit */
       #endif

       #ifdef SIGIOT
       signal(SIGIOT,   __SOM_STEM__SigHandler);  /* End process */
       #endif
#endif
    }
}

/*
 * printDSOMEv prints the contents of a SOM/DSOM exception and then frees
 * the contents of the exeception. If this is indicated to be a fatal
 * exception, the process is shutdown and then terminated.
 *
 */
void printDSOMEv(Environment *ev, boolean fatal)
{
    string exId;           /* exception ID string */
    StExcep *params;       /* standard exception structure */

    exId = exception_id(ev);
    params = exception_value(ev);

    somPrintf("Exception Occurred\n");
    somPrintf("    exception ID : %s\n", exId);
    somPrintf("minor error code : %u\n", params->minor);
    somPrintf(" completion code : %s\n", (params->completed == YES ? "YES" :
                                      params->completed == NO ? "NO": "MAYBE"));

    somdExceptionFree(ev); /* Frees SOM or DSOM exception */

    if (fatal) {
       __SOM_STEM__Shutdown();
       exit((int)params->minor);
    }
}


/*
 * __SOM_CLASS_NAME__Initialize returns a __SOM_CLASS_NAME__ class object.
 * The class object returned can be used to transparently create instances of
 * __SOM_CLASS_NAME__. The class object returned may be either a local
 * class object or a proxy for a non-local class object, depending on
 * whether SOMD_Init successfully initializes DSOM.
 *
 * Note: In general, this function should be customized such that the
 * class object returned is always a local object or always a proxy.
 * That is, if this function must return a proxy class, any exception
 * encountered should be considered fatal.
 *
 */
SOMClass __SOM_CLASS_NAME__Initialize(Environment *ev)
{
    SOMDServer svr;              /* DSOM server object */
    SOMClass cls;                /* class object to return */
    SOMClass rcls;               /* class object to return */

    __SOM_STEM__Init(ev);

    /* Create the __SOM_CLASS_NAME__ class object, if it doesn't
       already exist.  Use this if a remote class object can't be returned.
     */
    cls = __SOM_CLASS_NAME__NewClass(__SOM_CLASS_NAME___MajorVersion,
                                     __SOM_CLASS_NAME___MinorVersion);

    /* Initialize DSOM framework */
    SOMD_Init(ev);

    /* If no exception was encountered during DSOM initialization,
       find a DSOM server that can be used to create a remote
       object instance and then get a class object proxy from
       that server. The class object proxy will be used to
       create remote object instances.

       If exceptions are encountered, during init or while finding
       the server, just use the local class object.
     */
    if (EV_OK(ev)) {
#ifdef FIND_BY_CLASSNAME
       svr = _somdFindAnyServerByClass(SOMD_ObjectMgr, ev, "__SOM_CLASS_NAME__");
#else
       svr = _somdFindServerByName(SOMD_ObjectMgr, ev, "__SOM_STEM__Server");
#endif
       if (svr && EV_OK(ev)) {
          rcls = _somdGetClassObj(svr, ev, "__SOM_CLASS_NAME__");
          if (rcls && EV_OK(ev)) {
             __SOM_STEM__isdsom = TRUE;
             cls = rcls;
          }
       }
    }

    if (EV_NOT_OK(ev)) {
       printDSOMEv(ev, NOT_FATAL); /* report any problems, but continue */
    }

    return (cls);
}


/*
 * __SOM_STEM__Shutdown prepares the process for termination.
 * All application objects are freed and DSOM is uninitialized.
 *
 */
void __SOM_STEM__Shutdown()
{
    Environment ev;
    objectList p;

    SOM_InitEnvironment(&ev);
    __SOM_STEM__inShutdown = TRUE;
    for (p=__SOM_STEM__ShutdownList; p; p=p->next)
       freeObject(p->obj, &ev);
    SOMD_Uninit(&ev);
    somEnvironmentEnd();
}

/*
 * freeReturnedMem frees the given block of memory using a procedure determined
 * by whether the process is using DSOM.
 *
 */
void freeReturnedMem(void *p)
{
    if (p)
       if (USING_DSOM)
           ORBfree(p);
       else
           SOMFree(p);
}


/*
 * Create an instance of the given SOM class. Add the
 * instance to a global list of all application objects.
 *
 * Note: when the class object is a proxy for remote
 *       class object, the instance is created in the remote
 *       process.
 *
 */
SOMObject createObject(SOMClass cls, Environment *ev)
{
    SOMObject obj;

    obj = _somNew(cls);
    if (obj)
       /* add the object to the list of objects to be cleaned up
          at shutdown.          */
       addObjectToShutdownList(obj);
    return(obj);
}


/*
 * Delete an instance of a SOM object and remove the object
 * from the global list of all application objects.
 * This function can delete local as well as remote instances.
 *
 */
void freeObject(SOMObject obj, Environment *ev)
{
    somId methId;

    if (obj) {
       methId = SOM_IdFromString("somdTargetFree");
       if (USING_DSOM && _somRespondsTo(obj, methId)) {
          _somdTargetFree(obj, ev); /* free at server */
          _somdProxyFree(obj, ev); /* free local proxy */
       } else {
          _somFree(obj);
       }
       SOMFree(methId);
       removeObjectFromShutdownList(obj);
    }
}

/* Add an object to the tail end of the global list of
 * all application objects.
 */
void addObjectToShutdownList(SOMObject obj)
{
    if (__SOM_STEM__ShutdownList) {
       __SOM_STEM__ShutdownListTl->next =
                          (objectList)SOMMalloc(sizeof(struct objectList));
       __SOM_STEM__ShutdownListTl = __SOM_STEM__ShutdownListTl->next;
    } else
       __SOM_STEM__ShutdownList = __SOM_STEM__ShutdownListTl =
                          (objectList)SOMMalloc(sizeof(struct objectList));
    __SOM_STEM__ShutdownListTl->next = (objectList)NULL;
    __SOM_STEM__ShutdownListTl->obj = obj;
}

/* Remove object from global list of all application objects and
 * fixup the list.
 */
void removeObjectFromShutdownList(SOMObject obj)
{
    objectList p, t;

    if (!SHUTDOWN) { /* only remove entries if we are not shutting down */
       for (p=__SOM_STEM__ShutdownList, t=NULL; p && (p->obj!=obj); t=p, p=p->next);
       if (p==__SOM_STEM__ShutdownListTl) __SOM_STEM__ShutdownListTl = t;
       if (p && !t) { /* object found in first node of list */
          __SOM_STEM__ShutdownList = p->next;
          SOMFree(p);
       } else if (p && t) { /* object found in some other node of list */
          t->next = p->next;
          SOMFree(p);
       }
    }
}
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
:gidl
#ifndef __SOM_STEM___idl
#define __SOM_STEM___idl

__SOM_PARENT_FILES__
__SOM_META_FILES__

interface __SOM_CLASS_NAME__ : __SOM_PARENT_NAMES__
{
  // Attributes filled in here:

  // Operations filled in here:

#ifdef __SOMIDL__
  implementation {
    releaseorder:;

    // Class Modifiers:
    align     = 4;
    dllname   = "__SOM_DLLSTEM__.dll";
    __SOM_META_NAMES__

    // Attribute Modifiers:

    // Overrides:
    somDefaultInit: override, init;     // Default object initializer
    somDestruct: override;              // Default object uninitializer
  };
#endif /* __SOMIDL__ */
};
#endif  /* __SOM_STEM___idl */
:gidl_meta
#ifndef __SOM_STEM___idl
#define __SOM_STEM___idl

#include <somcls.idl>

interface __SOM_CLASS_NAME__ : SOMClass
{
  //# Attributes filled in here:

  //# Operations filled in here:

#ifdef __SOMIDL__
  implementation {
    releaseorder:;

    //# Class Modifiers:
    align     = 4;
    dllname   = "__SOM_DLLSTEM__.dll";

    //# Attribute Modifiers:

    //# Overrides:
  };
#endif /* __SOMIDL__ */
};
#endif  /* __SOM_STEM___idl */
:gidl_parent
#ifndef __SOM_STEM___idl
#define __SOM_STEM___idl

#include <somobj.idl>

interface __SOM_CLASS_NAME__ : SOMObject
{
  // Attributes filled in here:

  // Operations filled in here:

#ifdef __SOMIDL__
  implementation {
    releaseorder:;

    // Class Modifiers:
    align     = 4;
    dllname   = "__SOM_DLLSTEM__.dll";

    // Attribute Modifiers:

    // Overrides:
  };
#endif /* __SOMIDL__ */
};
#endif  /* __SOM_STEM___idl */
:dsomsvr_Makefile
# AIX xlc Makefile for DSOM server __SOM_STEM__svr
# Author: SOMObjects Application Builder

TARGET     = __SOM_STEM__svr
TARGETS    = $(TARGET)
OBJS       = __SOM_STEM__svr.o

AR         = ar
CC         = cc
CDEBUG     = -g
LDEBUG     =
CFLAGS     = $(CDEBUG)
LDFLAGS    = -bM:SRE -T512 -H512
LIBLIST    = -lsomtk
LINKER     = ld
RUNSHELL   = ./chk__SOM_STEM__
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh:ih
EMITC      = -sc
DELETE     = rm -f

.SUFFIXES: .idl .ih .h .sh .c

.c.o:
	$(CC) -I. -I$(SOMBASE)/include $(CFLAGS) -c $<

.idl.ih:
	$(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
	$(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
	$(SC) $(SCFLAGS) $(EMITC) $<

.sh:
	cp $? $@; chmod 0755 $@

all: chk__SOM_STEM__ $(TARGETS)

chk__SOM_STEM__: chk__SOM_STEM__.sh
	cp $? $@; chmod 0755 $@
	$(RUNSHELL)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -L$(SOMBASE)/lib $(LIBLIST)
	@echo DSOM server $(TARGET) built.

__SOM_STEM__svr.o: __SOM_STEM__svr.c

clean:
	-$(DELETE) *.o $(TARGETS) chk__SOM_STEM__

:dsomsvr_makefile.32
# OS/2 C Set Makefile for DSOM server __SOM_STEM__svr
# Author: SOMObjects Template Builder

TARGET     = __SOM_STEM__svr.exe
TARGETS    = $(TARGET)
OBJS       = __SOM_STEM__svr.o

CC         = icc
CDEBUG     = -Ti+
LDEBUG     = /debug
CFLAGS     = -W3 -c $(CDEBUG) -Q+ -Ss+ -Gd- -Gm+ -Gs+ -Sm
LDFLAGS    = /exepack /packd /packc /align:4 /stack:100000 /pm:vio /noe /noi /nol $(LDEBUG)
LIBLIST    = somtk.lib
LINKER     = link386
RUNSHELL   = set__SOM_STEM__
SC         = sc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
        $(CC) -fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -Ge+ $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: runchk $(TARGETS)

runchk: chk__SOM_STEM__.cmd
        chk__SOM_STEM__

$(TARGET): $(OBJS)
        $(LINKER) $(LDFLAGS) $(OBJS),$@,NUL,$(LIBLIST);
        @echo DSOM server $(TARGET) built.

__SOM_STEM__svr.o: __SOM_STEM__svr.c

clean:
        -$(DELETE) $(TARGET)
        -$(DELETE) *.o

:dsomsvr_makefile.w16
# Windows Visual C++ Makefile for DSOM server __SOM_STEM__svr
# Author: SOMobjects Template Builder

TARGET     = __SOM_STEM__svr.exe
TARGETS    = $(TARGET)
OBJS       = __SOM_STEM__svr.o

CC         = cl
CDEBUG     = -Zi
LDEBUG     = /CO
CFLAGS     = -nologo -Zp1 -Oi $(CDEBUG)
LDFLAGS    = /nologo /ONERROR:NOEXE /nod $(LDEBUG)
LIBLIST    = llibcew libw $(SOMBASE)\lib\somtk
LIBLISTD   = ldllcew libw $(SOMBASE)\lib\somtk
LINKER     = link
SC         = somc
SCFLAGS    = -mnoaddstar -p -u
EMITH      = -sh
EMITIH     = -sh;ih
EMITC      = -sc
DELETE     = del

.SUFFIXES: .idl .ih .h .c

.c.o:
        $(CC) -c -D_WIN16 -Fo$*.o -I. -I$(SOMBASE)\include $(CFLAGS) -AL $<

.idl.ih:
        $(SC) $(SCFLAGS) $(EMITIH) $<

.idl.h:
        $(SC) $(SCFLAGS) $(EMITH) $<

.idl.c:
        $(SC) $(SCFLAGS) $(EMITC) $<

all: runchk $(TARGETS)
        @echo Now go to windows to run $(TARGET) ...

runchk: chk__SOM_STEM__.bat
        chk__SOM_STEM__

$(TARGET): $(OBJS) __SOM_STEM__svr.def
        $(LINKER) $(LDFLAGS) @<<
$(OBJS)
$@
NUL
$(LIBLIST)
__SOM_STEM__svr.def
\<<
        @echo DSOM server $(TARGET) built.

__SOM_STEM__svr.o: __SOM_STEM__svr.c

clean:
        -$(DELETE) $(TARGET)
        -$(DELETE) *.o

:dsomsvr___SOM_STEM__svr.def
NAME           __SOM_STEM__svr
DESCRIPTION    'DSOM Server Program'
EXETYPE        WINDOWS
STUB           'WINSTUB.EXE'
CODE           PRELOAD MOVEABLE DISCARDABLE
DATA           PRELOAD MOVEABLE MULTIPLE
HEAPSIZE       16384
STACKSIZE      32768
:dsomsvr___SOM_STEM__svr.c
/*
 *  This is a generic DSOM server which can use the shared server or persistent
 *  server activation policy.
 *
 *  Syntax:  __SOM_STEM__svr <implid> | -a <alias>
 *
 *  The implid or alias must be defined in the current Implementation
 *  Repository.
 *
 */

#ifdef __OS2__
  #define INCL_DOSPROCESS
  #include <os2.h>
#endif
#if !defined(_WIN16) && !defined(__WINDOWS__) && !defined(__OS2__)
  #ifndef _ALL_SOURCE
  #define _ALL_SOURCE
  #endif
#endif
#ifdef _WIN16
  #include <windows.h>
  #define LOADDS __loadds
  #define FAR    _far
  typedef void (__cdecl * atexitFunction )(void);
#elif !defined(__OS2__)
  typedef void *HWND; /* dummy for AIX */
#endif

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <somd.h>
#include <somoa.h>
#include <implrep.h>

/*
 *  Function Prototypes
 */
int serverMain(int argc, char **argv);
void printDSOMEv(Environment *ev, boolean fatal);
void shutDownServer();
void initializeServer(Environment **ev);
int defineExitHandler();
#ifdef __OS2__
  void APIENTRY serverExitHandler(ULONG termCode);
#elif defined(_WIN16)
  void serverExitHandler();
  int makeArgs(LPSTR lpszCmdLine, int *argc, char **argv, int argSize);
  long FAR PASCAL _export WndProc (HWND, UINT, UINT, LONG);
#else
  void serverExitHandler(int Signal);
#endif
void decodeArgs(Environment *ev, int argc, char **argv);
ORBStatus processRequests(Environment *ev);
void showMessage(HWND hwnd, char *sztext);

/*
 *  Globals
 */
boolean svrImplIsReady = FALSE; /* set true after ready for requests */
boolean shuttingDown   = FALSE;
Environment *gev;               /* the global SOM environment */
HWND hwndSvr = (HWND)NULL;      /* Windows server window handle */

/*
 *  Macro definitions
 */
#define FATAL ((boolean)1)
#define NOT_FATAL ((boolean)0)
#define EV_OK(ev) ((ev)->_major == NO_EXCEPTION)
#define EV_NOT_OK(ev) ((ev)->_major != NO_EXCEPTION)

#ifdef _WIN16
/*
 * Win globals
 */
char *gSzAppName = "xxxxsvr";
HANDLE gHInstance = NULL;
/* For handling command line arguments the same */
char **argv=NULL;
int argc=1;
#define MAX_ARGS 25
#define ARG_LENGTH 80
#define WM_SOMDSVR  (WM_USER+0)

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG nlsmsg;
    WNDCLASS wndclass;
    long i;

    gHInstance = hInstance;

    if (!hPrevInstance) {
        wndclass.style         = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc   = WndProc;
        wndclass.cbClsExtra    = 0;
        wndclass.cbWndExtra    = 0;
        wndclass.hInstance     = hInstance;
        wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName  = NULL;
        wndclass.lpszClassName = gSzAppName;
        RegisterClass(&wndclass);
    }

    hwnd = CreateWindow(gSzAppName, gSzAppName,
                        WS_OVERLAPPEDWINDOW | WS_MINIMIZE,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);

    argv = (char **) malloc (sizeof (char *)*MAX_ARGS);
    for (i=0; i<MAX_ARGS; ++i)
       argv[i] = (char *)calloc (ARG_LENGTH, sizeof(char *));

    makeArgs(lpszCmdLine, &argc, argv, MAX_ARGS);

    /* Start the server with Windows user message */
    SendMessage(hwnd, WM_SOMDSVR, 0, 0);

    /* Windows message loop */
    while (GetMessage(&nlsmsg, NULL, 0, 0)) {
        TranslateMessage(&nlsmsg);
        DispatchMessage(&nlsmsg);
    }

    return nlsmsg.wParam;

    return (0);
}                                      /* end main */

long FAR PASCAL _export WndProc(HWND hwnd, UINT message, UINT wParam,
                                 LONG lParam)
{
    switch (message) {
            case WM_SOMDSVR:
                hwndSvr = hwnd; /* set server main window */
                serverMain (argc, argv);
                return 0 ;

            case WM_DESTROY:
                 PostQuitMessage (0);
                 return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

int makeArgs(LPSTR lpszCmdLine, int *argc, char **argv, int argSize)
{
    char *c;
    int i;

    *argc = 1; /* lpszCmdLine does not have argv[0] */
    /* lpszCmdLine has command line arguments delimited by blanks */
    c = strtok ((char *)lpszCmdLine, " ");

    for (i=0; i<argSize; ++i) {
       if (c == NULL) break;
       strcpy ((char *)argv[*argc], (char *)c);
       c = strtok (NULL, " ");
       *argc += 1;
    }
    return *argc;
}

#else /* OS/2 and AIX */

int main(int argc, char **argv)
{
    ORBStatus rc;

    rc = serverMain(argc, argv);
    shutDownServer();
    exit(0);
}
#endif /* _WIN16 */

int serverMain(int argc, char **argv) /* generic server main program */
{
    Environment *ev;
    ORBStatus rc;

#ifndef __OS2__
    /*
     * Register an exit handler to handle termination.
     *
     * Note: this server registers an exit handler after the call
     * to impl_is_ready on OS/2. Moving the point of the registration is a
     * work-around to avoid a problem encountered on the OS/2 version
     * of DSOM when Ctrl-c is entered to kill this server process.
     */
    defineExitHandler();
#endif

    /*
     * Initialize local SOM environment and DSOM.
     * Terminates if initialization fails.
     */
    initializeServer(&ev);

    /*
     * Parse command line arguments to locate server ID or alias to start.
     * Terminates if command line incorrect.
     */
    decodeArgs(ev, argc, argv);

    /*
     * At this point, the implementation definition has been determined.
     * Inform DSOM deamon we're noew ready to accept requests.
     */
    _impl_is_ready(SOMD_SOMOAObject, ev, SOMD_ImplDefObject);
    if (EV_NOT_OK(ev)) {
       showMessage(hwndSvr, "impl_is_ready failed.");
       printDSOMEv(ev, FATAL);        /* print error and terminate   */
    } else {
       svrImplIsReady = TRUE;
    }
#ifdef __OS2__
    defineExitHandler();
#endif

    /*
     * Process requests until an exception is encountered.
     */
    rc = processRequests(ev);

    if (EV_NOT_OK(ev)) {
      showMessage(hwndSvr, "execute_request_loop failed.");
      printDSOMEv(ev, FATAL);       /* print error and terminate     */
    }
}

/*
 *  Process incoming requests.
 *
 *  This function does not return until termination.
 */
ORBStatus processRequests(Environment *ev)
{
    ORBStatus rc = 0;

    /*
     * Processing incoming requests and then wait for more.
     * This call continues to process requests and only returns
     * if an error is encountered. It is a blocking call.
     */
    rc = _execute_request_loop(SOMD_SOMOAObject, ev, SOMD_WAIT);
    return(rc);
}

/*
 *  Prepares the server for termination.
 *  Frees local and global DSOM objects and informs DSOM
 *  deamon the server is terminating.
 */
void shutDownServer()
{
    Environment *globalEv;
    globalEv = somGetGlobalEnvironment();

    if (!shuttingDown) {
       shuttingDown = TRUE;
#ifdef _WIN16
       if (globalEv->_somdAnchor != NULL) {
          if (svrImplIsReady) {
             _deactivate_impl(SOMD_SOMOAObject, globalEv, SOMD_ImplDefObject);
          }
          if (SOMD_ImplDefObject != NULL) {
             _somFree(SOMD_ImplDefObject);
             SOMD_ImplDefObject = NULL;
          }
          if (SOMD_SOMOAObject != NULL) {
             _somFree(SOMD_SOMOAObject);
             SOMD_SOMOAObject = NULL;
          }
          SOMD_Uninit(globalEv);
#else
       if (svrImplIsReady) {
          _deactivate_impl(SOMD_SOMOAObject, globalEv, SOMD_ImplDefObject);
       }
       if (SOMD_ImplDefObject != NULL) {
          _somFree(SOMD_ImplDefObject);
          SOMD_ImplDefObject = NULL;
       }
       if (SOMD_SOMOAObject != NULL) {
          _somFree(SOMD_SOMOAObject);
          SOMD_SOMOAObject = NULL;
       }
       SOMD_Uninit(globalEv);
#endif
#ifdef _WIN16
       }
#endif
       somEnvironmentEnd();
    } /* endif shuttingDown */
}

/*
 * serverExitHandler - Deactivate the impl, shutdown server and exit.
 *
 * This function is called when a server process is terminated.
 */
#ifdef __OS2__
void APIENTRY serverExitHandler(ULONG termCode)
{
    (void) termCode;    /* not currently used */
    shutDownServer();
    DosExitList(EXLST_EXIT, 0);
}
#elif defined(_WIN16)
void serverExitHandler()
{
    shutDownServer();
}
#else
void serverExitHandler(int Signal)
{
    sigset_t sigset;

    (void) Signal;   /* not used, reference to avoid warnings */

    /* Process is about to terminate, so disable further signals */
    sigfillset(&sigset);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    shutDownServer();
    exit(-1);
}

#endif

/*
 * defineExitHandler - Defines handler function for abnormal termination.
 *
 */
#ifdef __OS2__
int defineExitHandler()
{
     int rc;
     rc = DosExitList(EXLST_ADD | 0x0100, (PFNEXITLIST) serverExitHandler);
     return(rc);
}
#elif defined(_WIN16)
int defineExitHandler()
{
    SOM_MainProgramWithUserExit(((atexitFunction)serverExitHandler));
    return (0);
}
#else
/*
 *  Sets up the signal handler.
 */
int defineExitHandler()
{
    struct sigaction action;

    action.sa_handler = (void *) serverExitHandler;
    action.sa_flags = 0;
    sigfillset(&action.sa_mask);

    /*
     * Specify which signals to catch.
     */
    sigaction(SIGHUP, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGILL, &action, NULL);
    sigaction(SIGTRAP, &action, NULL);
    sigaction(SIGIOT, &action, NULL);
    sigaction(SIGEMT, &action, NULL);
    sigaction(SIGFPE, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGSYS, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGPWR, &action, NULL);
    sigaction(SIGDANGER, &action, NULL);
    return (0);
}
#endif


/*
 * printDSOMEv prints the contents of a SOM/DSOM exception and then frees
 * the contents of the exeception. If this is indicated to be a fatal
 * exception, the process is shutdown and then terminated.
 *
 */
void printDSOMEv(Environment *ev, boolean fatal)
{
    string exId;           /* exception ID string */
    StExcep *params;       /* standard exception structure */
    char msgBuf[128];


    exId = exception_id(ev);
    params = exception_value(ev);

#ifdef _WIN16
    sprintf(msgBuf, "Exception occurred: %s", exId);
    MessageBox(hwndSvr, msgBuf, gSzAppName, MB_ICONSTOP|MB_OK);
#else
    somPrintf("Exception Occurred\n");
    somPrintf("    exception ID : %s\n", exId);
    somPrintf("minor error code : %u\n", params->minor);
    somPrintf(" completion code : %s\n", (params->completed == YES ? "YES" :
                                      params->completed == NO ? "NO": "MAYBE"));
#endif

    somdExceptionFree(ev); /* Frees SOM or DSOM exception */

    if (fatal) {
       shutDownServer();
       exit((int)params->minor);
    }
}

/*
 *  Initialize the server process. If an error is encountered,
 *  the exception is printed and the process is terminated.
 *  On successful completion, a pointer to the global Environment
 *  structure (per-thread) is returned.
 */
void initializeServer(Environment **ev)
{
    somEnvironmentNew();
    gev = *ev = somGetGlobalEnvironment();
    SOMD_Init(*ev);
    if (EV_NOT_OK(*ev)) {
       printDSOMEv(*ev, FATAL);      /* print error and terminate     */
    } else {
       SOMD_SOMOAObject = SOMOANew();
    } /* endif */
}

/*
 *  This function processes the command line arguments.
 *  The command line should contain either a implementation ID
 *  or an implementation alias. If neither is specified, an
 *  error is displayed and the process is terminated.
 *
 *  The Implementation Repository is queried with the given ID or
 *  alias in order to set up the global ImplementationDef object
 *  for the server process.
 */
void decodeArgs(Environment *ev, int argc, char **argv)
{
    char msgBuf[80];

    switch (argc) {
    /*
     * Implementation ID passed
     */
    case 2:
        /*
         * Get the ImplementationDef object from the Implementation Repository
         * based on the given ID. The impldef object is stored in the process
         * global variable SOMD_ImplDefObject.
         */
        SOMD_ImplDefObject = _find_impldef(SOMD_ImplRepObject, ev, argv[1]);
        if (EV_NOT_OK(ev)) {
           showMessage(hwndSvr, "find_impldef failed.");
           printDSOMEv(ev, FATAL);  /* print error and terminate     */
        }
        break;

    /*
     * Alias passed
     */
    case 3:
        /*
         * Get the ImplementationDef object from the Implementation Repository
         * based on the given alias. The impldef object is stored in the process
         * global variable SOMD_ImplDefObject.
         *
         * The alias option can be given as either:
         *
         *  -a alias_name
         *     or
         *  -A alias_name
         *
         */
        if ((argv[1][0] == '-') &&
            ((argv[1][1] == 'a') || (argv[1][1] == 'A'))) {
           SOMD_ImplDefObject = _find_impldef_by_alias(SOMD_ImplRepObject, ev, argv[2]);
           if (EV_NOT_OK(ev)) {
              showMessage(hwndSvr, "find_impldef_by_alias failed.");
              printDSOMEv(ev, FATAL); /* print error and terminate   */
           }
        } else {
          sprintf(msgBuf, "Usage: %s -a <alias>", argv[0]);
          showMessage(hwndSvr, msgBuf);
          shutDownServer();
          exit(1);
        }
        break;

    /*
     * Invalid number of arguments.
     */
    default:
        sprintf(msgBuf, "Usage: %s <implid> | -a <alias>", argv[0]);
        showMessage(hwndSvr, msgBuf);
        shutDownServer();
        exit(1);
        break;
    }
}

void showMessage(HWND hwnd, char *sztext)
{
#ifdef _WIN16
    MessageBox(hwnd, sztext, gSzAppName, MB_ICONINFORMATION|MB_OK);
#else
    somPrintf("%s\n", sztext);
#endif
}
/*
 * 10H9767, 10H9769 (C) COPYRIGHT International Business Machines Corp. 1992,1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS".
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */
