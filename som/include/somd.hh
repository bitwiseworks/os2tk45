/*
 *   COMPONENT_NAME: somd
 *
 *   ORIGINS: 27
 *
 *
 *   10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,
1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */


/*
 *  somd.hh - global DTS C++ include file for DSOM definitions
 */

#ifndef SOMD_HH_DTS_Included
#define SOMD_HH_DTS_Included

/*  --- base SOM and DSOM types ---  */

#pragma SOMAsDefault (off)
#define CORBA_FUNCTION_NAMES
#pragma SOMAsDefault (pop)

#include <som.hh>
#include <somtcnst.hh>
#include <somir.hh>


#include <somdtype.hh>

/*  --- DSOM error codes ---  */

#pragma SOMAsDefault (off)
#include <somderr.h>
#pragma SOMAsDefault (pop)


/*  --- DSOM externals --- */

#pragma SOMAsDefault (off)
#include <somdext.h>
#pragma SOMAsDefault (pop)



/* For DSOM users, CORBA's exception_free should be mapped to
 * somdExceptionFree rather than somExceptionFree, as in somcorba.h.
 */
#pragma SOMAsDefault (off)

#ifdef exception_free
 #undef exception_free
#endif
#define exception_free somdExceptionFree

#ifndef OBJECT_NIL
#define OBJECT_NIL ((SOMObject *) NULL)
#endif

#pragma SOMAsDefault (pop)

/*  --- DSOM global variables ---
 *
 *  Several global variables point to DSOM objects that are always or often
 *  used by DSOM applications.
 *
 *  SOMD_ObjectMgr, SOMD_ORBObject, and SOMD_ImplRepObject are initialized
 *  by SOMD_Init().
 *
 *  SOMD_ImplDefObject is initially NULL.   DSOM servers *MUST* set this to
 *  an ImplementationDef object which describes the server implementation.
 *  DSOM clients *MUST* leave this variable NULL.
 *
 *  SOMD_SOMOAObject is initially NULL.  DSOM servers should use the SOM
 *  Object Adapter can use this variable to refer to the instantiated SOMOA.
 *  It is not used by client processes.
 *
 *  The external references to these global variables are defined in the
 *  .hh files for their respective classes.
 */

#include <somdom.hh>            /* SOMD_ObjectMgr (incl. Factory) */
#include <orb.hh>               /* SOMD_ORBObject */
#include <implrep.hh>           /* SOMD_ImplRepObject */
#include <impldef.hh>           /* SOMD_ImplDefObject */
#include <somoa.hh>             /* SOMD_SOMOAObject */

/*  --- DSOM interfaces ---
 *
 *  Include the interfaces for other commonly used DSOM classes.
 */

#include <somdobj.hh>           /* SOMDObject */
#include <somdcprx.hh>          /* SOMDClientProxy */
#include <cntxt.hh>             /* Context */
#include <nvlist.hh>            /* NVList */
#include <request.hh>           /* Request */
// #include <stexcep.hh>           /* standard exceptions */
#pragma SOMAsDefault (off)
#include <stexcep.xh>
#pragma SOMAsDefault (pop)


#endif /* SOMD_HH_DTS_Included */
