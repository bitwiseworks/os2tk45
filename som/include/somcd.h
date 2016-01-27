/*
 *   COMPONENT_NAME: somc
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
/*
 *    @(#) 1.6 src/somc/comp/somcd.h, somc.sc, som2.1 3/22/96 15:34:19 [7/30/96 15:23:03]
 */

/*
 */

/*
 *         File:    somcd.h.
 *       Author:    Andy Martin [andy@austin.ibm.com]
 *     Contents:    Prototype for the dynamic versions of the
 *                  SOM Compiler
 *      History:    [03/22/94] Program created.
 */

#ifndef somcd_h
#define somcd_h
#ifdef _WIN32
#include <private/emitlib.h>
#else
#include <emitlib.h>
#endif
SOMEXTERN void *somtsc(int argc, char **argv);

#endif /* somcd_h */
