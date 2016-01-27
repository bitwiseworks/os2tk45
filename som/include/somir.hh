/*
 *   COMPONENT_NAME: somk
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */



/*
 *    SOMIR.HH  DTS C++ include
 *    Typedefs, structs, & enums in support of the CORBA Interface Repository
 *    for use with SOM
 */

#ifndef SOMIR_hh_DTS_INCLUDE
#define SOMIR_hh_DTS_INCLUDE

#include <somtc.hh>

/*
 * The following structure is for a SOM-unique extension
 * to the CORBA Interface Repository
 */

#pragma SOMAsDefault (off)

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

#pragma SOMAsDefault (pop)
#endif /* SOMIR_h */
