/*static char *SCCSID = "@(#)dllib.h	6.5 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  DLLIB.H - Dynamic Linking Library header file               *
 *     (C) Copyright IBM Corporation 1992.                      *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the prototype of application.     *
 *                                                              *
\*==============================================================*/
#define  INCL_DOSFILEMGR
#define  INCL_DOSMEMMGR
#define  INCL_DOSERRORS
#define  INCL_DOSPROCESS
#define  INCL_WIN

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllcomm.h"

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define IDM_MSGBOX                         1020
