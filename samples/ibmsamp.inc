#---------------------------------------------------------------------------
#
#   Copyright 1996-1999, IBM Corporation
#
#===================================================================

#===================================================================
#This include file is included by the various sample makefiles.
#===================================================================

SRCDIR = .
OBJDIR = .
EXEDIR = .

.SUFFIXES:
.SUFFIXES: .rc .res .obj .lst .c .asm .hlp .itl .ipf .idl .ih .h .xih .xh

PATH_TO_DDK  = $$(PATH_TO_DDK)
CFLAGS       = -Gd- -Se -Re -Ss -Ms -Gm+
CINCLUDE     =
CC           = icc
DLLCFLAGS    = -Ge-
OS2_H16      = $(PATH_TO_DDK)\base\h
AS           = alp
AFLAGS       =
LFLAGS       = /NOE /NOD /ALIGN:16 /EXEPACK
LINK         = icc /B"$(LFLAGS)" $(CFLAGS) -Fe
RC           = rc
RCFLAGS      = -r
SOMCOMPILER  = sc
MSGBIND      = msgbind
MAPSYM       = mapsym
OS2286_LIB   = os2286.lib
OS2386_LIB   = os2386.lib
OS2386P_LIB  = $(PATH_TO_DDK)\base\lib\os2386p.lib
OS2LIB       = $(OS2386_LIB)
IMPLIB       = implib
IPF          = ipfc
STLIB        = cppos30.lib
MTLIB        = cppom30.lib
DLLLIB       = cppon30.lib
LIBS         = $(MTLIB) + $(OS2386_LIB)
STLIBS       = $(STLIB) + $(OS2386_LIB)
MTLIBS       = $(MTLIB) + $(OS2386_LIB)
DLLLIBS      = $(DLLLIB) + $(OS2386_LIB)
REXXLIBS     = rexx.lib
DMILIBS      = dmiapi.lib
TRACELIBS    = trace.lib
LFLIBS       = lfapi.lib
FFSTLIBS     = ffst.lib
MMLIBS       = mmpm2.lib
SOMLIBS      = somtk.lib
VLIBS        = $(DLLLIB) + vdh + $(OS2386_LIB)
ULSLIBS      = libuls.lib
LANLIBS      = netapi32.lib
UPMLIBS      = upm32.lib
DAPIELIBS    = pmwinx.lib

.c{$(O)}.lst:
    $(CC) -Fc"$(OBJDIR)\$(@B).lst -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

.c{$(O)}.obj :
        $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(@B).c

{$(SRCDIR)}.c{$(O)}.obj :
        $(CC) -c $(CFLAGS) $(CINCLUDE) -Fo"$(OBJDIR)\$(@B).obj" $(SRCDIR)\$(@B).c

.itl.hlp :
        cc -P $*.itl
        ipfc $*.i
        del $*.i

.ipf{$(REL)}.hlp :
        $(IPF) $(@B).ipf $(EXEDIR)\$(@B).hlp /W3

.rc{$(O)}.res :
        $(RC) $(RCFLAGS) $*.rc $(OBJDIR)\$(@B).res

.idl.h:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"h"   $(SCFLAGS) $*.idl

.idl.xh:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"xh"  $(SCFLAGS) $*.idl

.idl.ih:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"ih"  $(SCFLAGS) $*.idl

.idl.xih:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"xih" $(SCFLAGS) $*.idl

.idl.def:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"def" $(SCFLAGS) $*.idl

.idl.c:
        sc -p -mnotc -maddstar -mnoint -S128000 -C128000 -s"c"   $(SCFLAGS) $*.idl
