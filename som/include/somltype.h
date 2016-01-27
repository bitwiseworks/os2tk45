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
/* @(#) 2.20.2.7 src/somk/somltype.h, somk.api, som2.1 1/18/96 10:43:49 [7/30/96 14:46:42] */

/*
 */

/*
 *    SOMLTYPE.H
 *    Assure definitions for SOMEXTERN, SOMLINK, and SOMDLINK
 *
 *    These variables are used to control the register linkage conventions
 *    for all SOM functions, methods, and external data references.
 *
 *    All SOM functions and external data references are preceeded with a
 *    SOMEXTERN macro (SOM methods do not have external names).
 *
 *    All SOM functions and methods are prototyped with a SOMLINK keyword.
 *
 *    All SOM external data references are defined with a SOMDLINK keyword.
 *
 *    Users may supply (at their own risk) appropriate definitions for
 *    other compilers using "-DSOMEXTERN=x", "-DSOMLINK=y" or "-DSOMDLINK=z"
 *    arguments on the compiler's command line.  Any such definition is not
 *    disturbed by the following code.
 *
 *    assumptions:
 *      (1) __cplusplus is defined when compiling C++ code anywhere
 *      (2) __IBMCPP__ is defined by CSet/2 on OS/2 when compiling C++ code
 *      (3) _CL386 may be defined if not using an IBM compiler
 *      (4) neither _CL386 nor __IBMCPP__ are defined on AIX
 *      (5) For historical reasons, "#pragma linkage" statements are
 *          generated for __IBMC__ on OS/2 and SOMLINK is set to white space.
 */

#ifndef SOMEXTERN
  #ifdef __cplusplus
    #define SOMEXTERN extern "C"
    #ifdef __STR__
      #undef __STR__
    #endif
  #else
    #define SOMEXTERN extern
  #endif
#endif

#ifndef SOMLINK
  #if defined(_CL386)
    #define SOMLINK _syscall
  #elif defined(applec)
    #define SOMLINK
  #elif defined(__OS2__)
    #if defined(__IBMCPP__)
      #define SOMLINK _System
    #elif defined(__BCPLUSPLUS__) || (__BORLANDC__)
      #define SOMLINK _syscall
    #elif defined(__HIGHC__)
      #define SOMLINK _DCC(_DEFAULT_CALLING_CONVENTION &   \
                           ~_SHORT_STRUCTS_IN_REGS & ~_OVERLOADED)
    #else
      #define SOMLINK
    #endif
  #elif defined(_MSDOS) || defined(__MSDOS__) || defined(__SC__)
    #if defined(_WDOS) || defined(__SOMEMIT__)
      #ifdef __SC__
        #define SOMLINK
      #endif
    #else
      #define SOMLINK __loadds
      #define WIN16_SOMLINK __loadds
      #ifndef _WIN16
        #define _WIN16
      #endif
    #endif
  #else
    #define SOMLINK
  #endif
#endif

#ifndef SOMDLINK
  #if defined(_CL386)
    #define SOMDLINK
  #elif defined(applec)
    #define SOMDLINK
  #elif defined(__BCPLUSPLUS__) || (__BORLANDC__)
    #define SOMDLINK _syscall
  #elif defined(__OS2__)
    #define SOMDLINK
  #elif defined(_MSDOS) || defined(__MSDOS__) || defined(__SC__)
    #if (defined(_WDOS) || defined(__SOMEMIT__)) && defined(__SC__)
      #define SOMDLINK
    #else
      #define SOMDLINK __far
    #endif
  #else
    #define SOMDLINK
  #endif
#endif

/*
 * === Backwards Source Level Compatability for __IBMC__
 */
#if defined(__OS2__) && defined(__IBMC__)
  #pragma linkage(somAncestorResolve             , system)
  #pragma linkage(somApply                       , system)
  #pragma linkage(somAssert                      , system)
  #pragma linkage(somBeginPersistentIds          , system)
  #pragma linkage(somBuildClass                  , system)
  #pragma linkage(somCheckId                     , system)
  #pragma linkage(somClassResolve                , system)
  #pragma linkage(somCompareIds                  , system)
  #pragma linkage(somConstructClass              , system)
  #pragma linkage(somDataResolve                 , system)
  #pragma linkage(somDataResolveChk              , system)
  #pragma linkage(somEndPersistentIds            , system)
  #pragma linkage(somEnvironmentNew              , system)
  #pragma linkage(somEnvironmentEnd              , system)
  #pragma linkage(somGenericApply                , system)
  #pragma linkage(somGetClassFromMToken          , system)
  #pragma linkage(somIdFromString                , system)
  #pragma linkage(somIdMarshal                   , system)
  #pragma linkage(somIsObj                       , system)
  #pragma linkage(somLPrintf                     , system)
  #pragma linkage(somMainProgram                 , system)
  #pragma linkage(somParentResolve               , system)
  #pragma linkage(somParentNumResolve            , system)
  #pragma linkage(somPrefixLevel                 , system)
  #pragma linkage(somPrintf                      , system)
  #pragma linkage(somRegisterClassLibrary        , system)
  #pragma linkage(somRegisterId                  , system)
  #pragma linkage(somResolve                     , system)
  #pragma linkage(somResolveDLocator             , system)
  #pragma linkage(somResolveMLocator             , system)
  #pragma linkage(somResolveByName               , system)
  #pragma linkage(somResolveTerminal             , system) 
  #pragma linkage(somSetExpectedIds              , system)
  #pragma linkage(somStringFromId                , system)
  #pragma linkage(somTest                        , system)
  #pragma linkage(somTestCls                     , system)
  #pragma linkage(somTotalRegIds                 , system)
  #pragma linkage(somUniqueKey                   , system)
  #pragma linkage(somVprintf                     , system)
  #pragma linkage(somTD_classInitRoutine         , system)
  #pragma linkage(somTD_SOMCalloc                , system)
  #pragma linkage(somTD_SOMClassInitFuncName     , system)
  #pragma linkage(somTD_SOMCreateMutexSem        , system)
  #pragma linkage(somTD_SOMDeleteModule          , system)
  #pragma linkage(somTD_SOMDestroyMutexSem       , system)
  #pragma linkage(somTD_SOMError                 , system)
  #pragma linkage(somTD_SOMFree                  , system)
  #pragma linkage(somTD_SOMGetThreadId           , system)
  #pragma linkage(somTD_SOMLoadModule            , system)
  #pragma linkage(somTD_SOMMalloc                , system)
  #pragma linkage(somTD_SOMOutCharRoutine        , system)
  #pragma linkage(somTD_SOMRealloc               , system)
  #pragma linkage(somTD_SOMReleaseMutexSem       , system)
  #pragma linkage(somTD_SOMRequestMutexSem       , system)
  #pragma linkage(somcsr_Static_foreign_marshaler, system)
  #pragma linkage(somUnregisterClassLibrary      , system)
#endif

