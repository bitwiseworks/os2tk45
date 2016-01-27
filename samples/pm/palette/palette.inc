#ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
#³                                                                          ³
#³ PROGRAM NAME: PALETTE                                                    ³
#³  A PM program demonstrating vector fonts, palette manager APIs, and IPF  ³
#³  help.                                                                   ³
#³                                                                          ³
#³ COPYRIGHT:                                                               ³
#³ ----------                                                               ³
#³  Copyright (C) International Business Machines Corp., 1992               ³
#³                                                                          ³
#³ DISCLAIMER OF WARRANTIES:                                                ³
#³ -------------------------                                                ³
#³  The following [enclosed] code is sample code created by IBM Corporation.³
#³  This sample code is not part of any standard IBM product and is provided³
#³  to you solely for the purpose of assisting you in the development of    ³
#³  your applications.  The code is provided "AS IS", without warranty of   ³
#³  any kind.  IBM shall not be liable for any damages arising out of your  ³
#³  use of the sample code, even if they have been advised of the           ³
#³  possibility of such damages.                                            ³
#³                                                                          ³
#³ For details on what this program does etc., please see the PALETTE.C     ³
#³ file.                                                                    ³
#³                                                                          ³³
#³ Assumed INCLUDE environment variable path:                               ³
#³   C:\TOOLKT20\C\OS2H;                                                    ³
#³   C:\TOOLKT20\MASM\OS2INC;                                               ³
#³   C:\IBMC\INCLUDE;                                                       ³
#³   C:\CI\STD;                                                             ³
#³                                                                          ³³
#ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



PALETTE.RES:  PALETTE.RC  \
#  {.;$(INCLUDE)}OS2.H  \
   {.;$(INCLUDE)}PALETTE.H  \
   PALETTE.ICO  \
   PALETTE.H  \
   PALETTE.MAK

PALETTE.HLP:  PALETTE.IPF  \
   PALETTE.MAK

PALETTE.OBJ:  PALETTE.C  \
#  {$(INCLUDE);}os2.h  \
#  {$(INCLUDE);}string.h  \
#  {$(INCLUDE);}stdlib.h  \
#  {$(INCLUDE);}stdio.h  \
#  {$(INCLUDE);}stdarg.h  \
   {.;$(INCLUDE);}palette.h  \
   PALETTE.MAK
