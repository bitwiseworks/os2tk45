/*
 *    @(#) 2.21 src/somd/somdext.h, somd, som2.1 4/5/95 13:44:45 [7/30/96 14:45:43]
 */

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

/*
 *  somdext.h - miscellaneous external definitions for DSOM runtime
 *              (other external definitions are made in IDL files)
 *
 *  Change History:
 *  03/08/95  lea  D9759: add SOMD_RecvBufferSize global, OS/2 and AIX.
 *  03/22/95  lea  D9727: add somdCreateDynProxyClass.
 *  04/01/95  jrr  Defect 9772: added SOMD_YesORBfree and SOMD_QueryORBfree
 */

#ifndef somdext_h
#define somdext_h

/* DSOM runtime functions */

#ifdef __IBMC__
  #pragma linkage(SOMD_Init, system)
  #pragma linkage(SOMD_Uninit, system)
  #pragma linkage(SOMD_RegisterCallback, system)
  #pragma linkage(SOMD_Callback, system)
  #pragma linkage(SOMInitsomd, system)
  #pragma linkage(ORBfree, system)
  #pragma linkage(SOMD_NoORBfree, system)
  #pragma linkage(SOMD_YesORBfree, system)
  #pragma linkage(SOMD_QueryORBfree, system)
  #pragma linkage(somdExceptionFree, system)
  #pragma linkage(SOMD_FlushInterfaceCache, system)
  #pragma linkage(somdCreateDynProxyClass, system)
#endif

SOMEXTERN void SOMLINK SOMD_Init(Environment *);
SOMEXTERN void SOMLINK SOMD_Uninit(Environment *);

typedef void SOMD_Callback(void *, void *);

#ifdef SOM_STRICT_IDL
SOMEXTERN void SOMLINK SOMD_RegisterCallback(SOMObject emanObj, SOMD_Callback *func);
#else
SOMEXTERN void SOMLINK SOMD_RegisterCallback(SOMObject *emanObj, SOMD_Callback *func);
#endif /* SOM_STRICT_IDL */

SOMEXTERN void SOMLINK SOMInitsomd(void);
#ifndef _DOS_REGIMPL
SOMEXTERN void SOMLINK ORBfree(void *);
#endif
SOMEXTERN void SOMLINK SOMD_NoORBfree(void);
SOMEXTERN void SOMLINK SOMD_YesORBfree(void);
SOMEXTERN unsigned char SOMLINK SOMD_QueryORBfree(void);
SOMEXTERN void SOMLINK somdExceptionFree(Environment *ev);
SOMEXTERN void SOMLINK SOMD_FlushInterfaceCache(Environment *ev, string name);
SOMEXTERN SOMClass * SOMLINK somdCreateDynProxyClass(Environment *ev, string targetClassName, 
						long classMajorVersion, long classMinorVersion );

/* DSOM global variables */

#ifdef _WIN16
#define SOMD_TraceLevel (*SOMD_TraceLevelResolve())
SOMEXTERN long * SOMLINK SOMD_TraceLevelResolve(void);
#define SOMD_DebugFlag (*SOMD_DebugFlagResolve())
SOMEXTERN long * SOMLINK SOMD_DebugFlagResolve(void);
#else
SOMEXTERN long SOMDLINK SOMD_TraceLevel; 
SOMEXTERN long SOMDLINK SOMD_DebugFlag;
SOMEXTERN long SOMDLINK SOMD_RecvBufferSize;   /* d9759 */
#endif /* _WIN16 */

#endif /* somdext_h */
