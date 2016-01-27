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
/* @(#) somk/somtypes.h 2.6.2.3 12/26/95 16:51:47 [7/30/96 14:46:45] */

/*
 */

/*
 *    SOMTYPES.H
 *    SOM types for C
 *    Multiple Inheritance Version
 */
#ifndef somtypes_h
#define somtypes_h

#include <stdarg.h>

/* SOM Base Types */
#include <sombtype.h>

/*  -- Object Instance Structure */
struct somMethodTabStruct;
typedef struct SOMAny_struct {
    struct somMethodTabStruct  *mtab;
    integer4 body[1];
} SOMAny;


/* SOM Primitive Classes */
/* primitive classes */
#define SOMObject SOMAny
#define SOMClass SOMAny
#define SOMClassMgr SOMAny


/* SOM Implementation Types */
#ifdef SOM_INCLUDE_PRIVATE_ITYPES
#include <private/somitype.h>
#else
#include <somitype.h>
#endif


#endif  /* somtypes_h */
