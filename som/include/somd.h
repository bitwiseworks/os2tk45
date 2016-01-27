/*
 *   COMPONENT_NAME: somd
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
/* @(#) 2.9 src/somd/somd.h, somd, som2.1 9/16/94 12:25:03 [7/30/96 14:45:41] */

/*
 */


/*
 *  somd.h - global include file for DSOM definitions
 */

#ifndef somd_h
#define somd_h

/*  --- base SOM and DSOM types ---  */

#define CORBA_FUNCTION_NAMES
#include <som.h>

#include <somtcnst.h>
#include <somir.h>

#include <somdtype.h>

/*  --- DSOM error codes ---  */

#include <somderr.h>

/*  --- DSOM externals ---  */

#include <somdext.h>

/* For DSOM users, CORBA's exception_free should be mapped to 
 * somdExceptionFree rather than somExceptionFree, as in somcorba.h.
 */
#ifdef exception_free
 #undef exception_free
#endif
#define exception_free somdExceptionFree

#ifndef OBJECT_NIL
#define OBJECT_NIL NULL
#endif

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
 *  .h files for their respective classes.
 */

#include <somdom.h>		/* SOMD_ObjectMgr (incl. SOMDServer) */
#include <orb.h>		/* SOMD_ORBObject */
#include <implrep.h>		/* SOMD_ImplRepObject */
#include <impldef.h>		/* SOMD_ImplDefObject */
#include <somoa.h>		/* SOMD_SOMOAObject */

/*  --- DSOM interfaces ---
 *
 *  Include the interfaces for other commonly used DSOM classes.
 */

#include <somdobj.h>		/* SOMDObject */
#include <somdcprx.h>		/* SOMDClientProxy */
#include <cntxt.h>		/* Context */
#include <nvlist.h>		/* NVList */
#include <request.h>		/* Request */
#include <stexcep.h>		/* standard exceptions */
#include <principl.h>		/* Principal object */

#endif /* somd_h */
