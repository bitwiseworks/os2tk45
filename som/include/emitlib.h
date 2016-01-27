/*
 *   COMPONENT_NAME: somc
 *
 *   ORIGINS: 27
 *
 *
 *   10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somc/emitlib.pub 2.6 9/2/94 01:19:30 [7/30/96 14:45:04] */


/*
 *         File:    emitlib.h.
 *     Contents:    General routines used by Emitter Framework.
 */

#ifndef emitlib_h
#define emitlib_h

#ifndef SOMEXTERN
    #ifdef __cplusplus
	#define SOMEXTERN extern "C"
    #else
	#define SOMEXTERN extern
    #endif /* __cplusplus */
#endif /* SOMEXTERN */

#include <stdio.h>
#include <stdlib.h>
    /* #ifdef __OS2__ */
        /* #include <io.h> */
    /* #else */
        /* #include <unistd.h> */
    /* #endif */
#include <sm.h>

SOMEXTERN FILE *somtopenEmitFile(char *file, char *ext);
SOMEXTERN int somtfclose(FILE *fp);
SOMEXTERN void somterror(char *file, long lineno, char *fmt, ...);
SOMEXTERN void somtfatal(char *file, long lineno, char *fmt, ...);
SOMEXTERN void somtinternal(char *file, long lineno, char *fmt, ...);
SOMEXTERN void somtmsg(char *file, long lineno, char *fmt, ...);
SOMEXTERN void somtresetEmitSignals();
SOMEXTERN void somtunsetEmitSignals(void);
SOMEXTERN void somtwarn(char *file, long lineno, char *fmt, ...);

#endif
