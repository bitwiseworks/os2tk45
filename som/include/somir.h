/*
 *   COMPONENT_NAME: somi
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.10 src/somi/ir/somir/somir.h, somi.common, som2.1 12/26/95 16:41:48 [7/30/96 14:46:29] */

/*
 */

/*
 *    SOMIR.H
 *    Typedefs, structs, & enums in support of the CORBA Interface Repository
 *    for use with SOM
 */

#ifndef SOMIR_h
#define SOMIR_h

#ifdef SOMIR_xh
  #include <somtc.xh>
#else
  #include <somtc.h>
#endif /* SOMIR_xh */

/*
 * The following structure is for a SOM-unique extension
 * to the CORBA Interface Repository
 */

typedef struct somModifier {
    string name;
    string value;
} somModifier;

/*
 * Miscellaneous "ir" helper functions
 */

#ifdef __IBMC__
  #pragma linkage(irGetFileNames, system)
  #pragma linkage(irGetLastFileName, system)
#endif /* __IBMC__ */

SOMEXTERN char * SOMLINK irGetFileNames (void);
SOMEXTERN char * SOMLINK irGetLastFileName (void);

#endif /* SOMIR_h */
