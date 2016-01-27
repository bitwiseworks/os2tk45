/*
 *   COMPONENT_NAME: somk
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somk/sombtype.h 2.10.1.4 3/25/96 19:45:45 [7/30/96 14:46:36] */

/*
 */

/*
 *    SOMBTYPE.H
 *    SOM Base Data Types
 */

#ifndef sombtype_h
#define sombtype_h

#include <stdarg.h>
#include <stddef.h>

/*--------------------------------------------------------------
 * Basic SOM data types for C and C++
 */
/* -- Generic SOM Method Procedure Pointers */
#if defined(__OS2__) && defined(__IBMC__)
  #ifndef __cplusplus
    #pragma linkage( somMethodProc, system )
    #pragma linkage( somTP_somClassInitFunc, system)
  #endif
#endif

typedef void* SOMLINK somMethodProc(void*);
typedef somMethodProc SOMDLINK *somMethodPtr;
typedef void SOMLINK somTP_somClassInitFunc(void *somSelf);

#ifndef NULL
  #ifdef __cplusplus
    #define NULL 0
  #else
    #define NULL ((void *) 0)
  #endif
#endif

typedef char integer1;
typedef short integer2;
typedef unsigned short uinteger2;
typedef long integer4;
typedef unsigned long uinteger4;
typedef float float4;
typedef double float8;
typedef char *zString;                 /* NULL terminated string */
typedef char *fString;                 /* non-terminated string  */
typedef char **somId;
typedef void *somToken;                /* Uninterpretted value   */
typedef signed char signed_char;

#ifndef SOM_BOOLEAN
  #define SOM_BOOLEAN
  typedef unsigned char boolean;
#endif /* SOM_BOOLEAN */

#define SOM_DynamicMethod  1
#define SOM_StaticMethod   0

/*
 *    Comment macros for arguments
 */
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif


#endif /* sombtype_h */
