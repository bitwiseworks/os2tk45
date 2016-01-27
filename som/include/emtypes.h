/*
 *   COMPONENT_NAME: somu
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
/* @(#) 2.7 src/somem/emtypes.h, somu, som2.1 12/26/95 16:28:27 [7/30/96 14:46:07] */

/*
 */

#ifndef SOM_EM_TYPES_h
#define SOM_EM_TYPES_h

#ifndef SOMEEMRegisterData_VA_STUBS
    #define SOMEEMRegisterData_VA_EXTERN
#endif /* SOMEEMRegisterData_VA_STUBS */

#ifndef __cplusplus
    #ifdef __PRIVATE__
    #include <private/emregdat.h>
    #else
    #include <emregdat.h>
    #endif

    #include <event.h>
    #include <clientev.h>
    #include <eventmsk.h>
#else
    #ifdef __PRIVATE__
    #include <private/emregdat.xh>
    #else
    #include <emregdat.xh>
    #endif

    #include <event.xh>
    #include <clientev.xh>
    #include <eventmsk.h>
#endif

#ifdef SOM_STRICT_IDL
typedef void EMRegProc(SOMEEvent , void *);
typedef void SOMLINK EMMethodProc(SOMObject , SOMEEvent , void *);
typedef void SOMLINK EMMethodProcEv(SOMObject , Environment *, SOMEEvent , void *);
#else
/*
 * EMRegProc
 *
 * Procedure registration callback interface
 */
typedef void EMRegProc(SOMEEvent *, void *);

/*
 * EMMethodProc
 *
 * Method registration callback interface
 */
typedef void SOMLINK EMMethodProc(SOMObject *, SOMEEvent *, void *);

/*
 * EMMethodProcEv
 *
 * Method registration callback interface
 */
typedef void SOMLINK EMMethodProcEv(SOMObject *, Environment *, SOMEEvent *, void *);
#endif

#ifdef __IBMC__
    #pragma linkage(EMRegProc, system)
    #pragma linkage(EMMethodProc, system)
    #pragma linkage(EMMethodProcEv, system)
#endif

#endif       /* SOM_EM_TYPES_h */
