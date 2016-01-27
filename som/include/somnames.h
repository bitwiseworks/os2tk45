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
/* @(#) somk/somnames.h 2.15.1.1 12/26/95 16:50:09 [7/30/96 14:46:43] */

/*
 */

/*
 *    SOMNAMES.H
 *    System Object Model run-time external names
 */

#ifndef extnames_h
#define extnames_h

/*
 * Short forms of SOM_METHOD_THUNKS, SOM_DATA_THUNKS and SOM_METHOD_STUBS *
 */

#if (defined(_SOM_ALL) || defined(_SOM_MT)) && ! defined(SOM_METHOD_THUNKS)
    #define SOM_METHOD_THUNKS   1
#endif
#if (defined(_SOM_ALL) || defined(_SOM_DT)) && ! defined(SOM_DATA_THUNKS)
    #define SOM_DATA_THUNKS	1
#endif
#if (defined(_SOM_ALL) || defined(_SOM_MS)) && ! defined(SOM_METHOD_STUBS)
    #define SOM_METHOD_STUBS	1
#endif

#ifdef SOM_USE_SHORT_EXTERNAL_NAMES

#define SOM_AssertLevel            somasl
#define SOM_MajorVersion           sommav
#define SOM_MaxThreads             sommxt
#define SOM_MinorVersion           sommiv
#define SOM_TraceLevel             somtrl
#define SOM_WarnLevel              somwnl

#define SOMCalloc                  somcll
#define SOMClassMgrObject          somcmo
#define SOMCreateMutexSem          somcms
#define SOMDeleteModule            somdm
#define SOMDestroyMutexSem         somdms
#define SOMError                   somerr
#define SOMFree                    somfre
#define SOMGetThreadId             somgti
#define SOMOutCharRoutine          somout
#define SOMLoadModule              somlm
#define SOMMalloc                  sommll
#define SOMRealloc                 somrll
#define SOMReleaseMutexSem         somsms
#define SOMRequestMutexSem         somrms 

#define somAssert                  somass
#define somBeginPersistentIds      sombis
#define somBuildClass              sombcs
#define somCheckArgs               somcka
#define somCheckId                 somcki
#define somClassResolve            somcrs
#define somAncestorResolve         somars
#define somCompareIds              someqi
#define somConstructClass          somccl
#define somCreateDynamicClass      somcdy
#define somDataResolve             somdrs
#define somDataResolveChk          somdrc
#define somEndPersistentIds        someis
#define somEnvironmentEnd          somene
#define somEnvironmentNew          somen
#define somExceptionId             somxni
#define somExceptionValue          somxnv
#define somExceptionFree           somxnf
#define somGenericApply            somga
#define somGetClassFromMToken      somgct
#define somGetCurrentContextObject somgco
#define somGetCurrentEnvironment   somgce
#define somIdFromString            somifs
#define somIsObj                   somiso
#define somLPrintf                 somlpn
#define somMainProgram             sommp
#define somParentResolve           somprs
#define somParentNumResolve        sompnr
#define somPrefixLevel             somplv
#define somPrintf                  sompnt
#define somRegisterClassLibrary    somrcl
#define somRegisterId              somrgi
#define somResolve                 somres
#define somResolveByName           somrbn
#define somSetCurrentContextObject somsco
#define somSetCurrentEnvironment   somsce
#define somSetException            somsxn
#define somSetExpectedIds          somsei
#define somStringFromId            somsfi
#define somTest                    somtst
#define somTestCls                 somtcl
#define somTotalRegIds             somtri
#define somUniqueKey               somuky
#define somVprintf                 somvpt
#define somApply                   somapl

#ifdef __IBMC__
  #pragma linkage(somars, system)
  #pragma linkage(somapl, system)
  #pragma linkage(somass, system)
  #pragma linkage(sombis, system)
  #pragma linkage(somcdy, system)
  #pragma linkage(somcka, system)
  #pragma linkage(somcki, system)
  #pragma linkage(someqi, system)
  #pragma linkage(somccl, system)
  #pragma linkage(someis, system)
  #pragma linkage(somen , system)
  #pragma linkage(somene, system)
  #pragma linkage(somga , system)
  #pragma linkage(somgce, system)
  #pragma linkage(somgcl, system)
  #pragma linkage(somgco, system)
  #pragma linkage(somifs, system)
  #pragma linkage(somiso, system)
  #pragma linkage(somlpn, system)
  #pragma linkage(sommp,  system)
  #pragma linkage(somprs, system)
  #pragma linkage(somplv, system)
  #pragma linkage(sompnt, system)
  #pragma linkage(somrbn, system)
  #pragma linkage(somrcl, system)
  #pragma linkage(somres, system)
  #pragma linkage(somrgi, system)
  #pragma linkage(somsce, system)
  #pragma linkage(somsco, system)
  #pragma linkage(somsei, system)
  #pragma linkage(somsfi, system)
  #pragma linkage(somsxn, system)
  #pragma linkage(somtst, system)
  #pragma linkage(somtcl, system)
  #pragma linkage(somtri, system)
  #pragma linkage(somuky, system)
  #pragma linkage(somvpt, system)
  #pragma linkage(somxnf, system)
  #pragma linkage(somxni, system)
  #pragma linkage(somxnv, system)
#endif /* __IBMC__ */

#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */




/*
 * The following synonyms provide OS/2-style typedefs
 */

#ifndef FLOAT4
#define FLOAT4                  float4
#endif

#ifndef FLOAT8
#define FLOAT8                  double
#endif

#ifndef SOMID
#define SOMID                   somId
#endif

#ifndef SOMTOKEN
#define SOMTOKEN                somToken
#endif

#ifndef SOMANY
#define SOMANY                  SOMAny
#endif

#ifndef SOMCLASS
#define SOMCLASS                SOMClass
#endif

#ifndef SOMOBJECT
#define SOMOBJECT               SOMObject
#endif

#ifndef SOMMETHODTAB
#define SOMMETHODTAB            somMethodTab
#endif

#ifndef SOMMETHODDATA
#define SOMMETHODDATA           somMethodData
#endif

#ifndef SOMMETHODPTR
#define SOMMETHODPTR            somMethodPtr
#endif

#ifndef SOMCLASSDATASTRUCT
#define SOMCLASSDATASTRUCT      somClassDataStructure
#endif


#endif /* extnames_h */
