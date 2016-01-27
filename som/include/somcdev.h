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
/* @(#) 2.22.1.2 src/somk/somcdev.h, somk.api, som2.1 1/3/96 11:26:42 [7/30/96 14:46:36] */

/*
 */

/*
 *    SOMCDEV.H
 *    System Object Model development support for ANSI C
 *    Multiple Inheritance Version
 */

#ifndef somcdev_h
#define somcdev_h

#include <somtypes.h>
#if !defined(__SOMDLL__)
  #include <stdlib.h>  /* needed for atexit, used below */
#endif /* __SOMDLL__ */


/*
 *  Method and Data Resolution macros
 */

/*
 *  Method Resolution. Methods are invoked on an object o of some
 *  object class oc, where oc has immediate ancestor classes
 *  called parent classes. Macro arguments include method names
 *  (e.g., mn), object class and parent class names (e.g., ocn, pcn)
 *  and parent class positions (e.g., pcp), expressed in terms of the
 *  left-to-right ordering (beginning with 1, for the first parent)
 *  used when declaring oc's parents. The choice of resolution
 *  macro determines the method table from which methods are selected.
 *
 *  Macros are available to select a method from ...
 */

/*
 * Default definition of somresolve_ to call the procedure, somResolve.
 * This may be be changed by emitters on systems for which method
 * tokens are thunks.
 */
#define somresolve_(obj,mToken) (somResolve(obj,mToken))

/* from oc's mtbl, with verification of o */
#define SOM_Resolve(o, ocn, mn) \
    (( somTD_ ## ocn ## _ ## mn ) \
     somresolve_(SOM_TestCls(o, ocn ## ClassData.classObject), \
                ocn ## ClassData.mn ))

/* from oc's mtbl, without verification of o */
#define SOM_ResolveNoCheck(o, ocn, mn) \
    (( somTD_ ## ocn ## _ ## mn ) \
     somresolve_(o, \
                ocn ## ClassData.mn ))

/* from the pcp'th element of oc's CClassData.parentMtab list */
#define SOM_ParentNumResolveCC(pcn, pcp, ocn, mn) \
    (( somTD_ ## pcn ## _ ## mn ) \
     somParentNumResolve( ocn ## CClassData.parentMtab, pcp, \
                         pcn ## ClassData.mn ))

/* from the pcp'th element of an argument mtab list */
#define SOM_ParentNumResolve(pcn, pcp, mtabs, mn) \
    (( somTD_ ## pcn ## _ ## mn ) \
     somParentNumResolve(mtabs, pcp, \
                         pcn ## ClassData.mn ))

/* from an argument class's method table */
#define SOM_ClassResolve(cn, class, mn) \
    (( somTD_ ## cn ## _ ## mn ) \
     somClassResolve(class, \
                     cn ## ClassData.mn ))


/* support reintroduction of methods */
/* tdc == typedef class name; cdc == classdata class name */
#define SOM_ResolveD(o, tdc, cdc, mn) \
	(( somTD_ ## tdc ## _ ## mn ) \
	  somresolve_(SOM_TestCls(o, cdc ## ClassData.classObject),\
	             cdc ## ClassData.mn))


/* from the first mtbl in an argument mtbl list ...

  This macro is here primarily to document the behavior
  of previously-compiled single inheritance code that used
  the macro of this name. The behavior is that of using
  SOM_ParentNumResolve with a pcp of 1.
*/
#define SOM_ParentResolveE(pcn, mtbls, mn) \
    (( somTD_ ## pcn ## _ ## mn ) \
     somParentResolve(mtbls, \
                      pcn ## ClassData.mn ))



/*
 * Data resolution macro
 */

#define SOM_DataResolve(obj, dataId) \
  (somDataResolve(obj, dataId))


/*  -------------------------------------------------------------------
 *  The following macros are for purposes of backward compatibility
 *  with prior versions of SOM.  There is no advantage to using them
 *  over a direct call to the corresponding routine.
 */

#define SOM_CompareValidIds(id1,id2) (somCompareIds(id1,id2))

#define SOM_CompareIds(id1,id2) (somCompareIds(id1,id2))

#define SOM_StringFromId(id) (somStringFromId(id))

#define SOM_IdFromString(str) (somIdFromString(str))

#define SOM_CheckId(id) (somCheckId(id))

/*
 * Convenience macro for somSubstituteClass method.
 */
#ifdef __cplusplus     
#define SOM_SubstituteClass(old,new) \
    (old##NewClass(old##_MajorVersion, old##_MinorVersion),\
     new##NewClass(new##_MajorVersion, new##_MinorVersion),\
	 SOMClassMgrObject->somSubstituteClass(_##old->somGetName(),\
					       _##new->somGetName()))


#else
#define SOM_SubstituteClass(old,new) \
    (old##NewClass(old##_MajorVersion, old##_MinorVersion),\
     new##NewClass(new##_MajorVersion, new##_MinorVersion),\
     _somSubstituteClass(SOMClassMgrObject,\
			 _somGetName(_##old),\
			 _somGetName(_##new)))
#endif


#if !defined(__SOMDLL__)

  /*
   * Main programs should register for SOM cleanup at exit
   */

   #define SOM_MainProgram()\
    (atexit(somEnvironmentEnd), somMainProgram())

  /*
   * somEnvironmentEnd may not have the right
   * linkage to be called by atexit. Allow a user to
   * create a routine that can be called by atexit
   * and which will call somEnvironmentEnd.
   */

   #define SOM_MainProgramWithUserExit( userExitFcn ) \
    (atexit( userExitFcn ), somMainProgram())

#endif /* __SOMDLL__ */

/*
 * Platform provided automatic class library initialization rtns
 * should use this macro to inform the SOM Class Manager that
 * they have been loaded. 
 */
#define SOM_ClassLibrary(name)\
    (somRegisterClassLibrary(name,\
        (somMethodProc *) &SOMInitModule))


/*
 *   Development support macros and globals
 */

#ifdef SOM_STRICT_IDL
#define SOMSTAR
#else
#define SOMSTAR *
#endif

/*
 * Macro to get class object
 */
#define SOM_GetClass(obj) (**(SOMClass SOMSTAR **)obj)

/*
 * This macro is used throughout the generated source
 * to prevent compiler warnings for unreferenced variables
 */
#ifndef SOM_IgnoreWarning
    #ifdef __xlC__
	#define SOM_IgnoreWarning(v)	
    #else
	#define SOM_IgnoreWarning(v)	(void) v
    #endif
#endif /* SOM_IgnoreWarning */

/* Check the validity of method resolution using the specified target  */
/* object.  Note: this macro makes programs bigger and slower.  After  */
/* you are confident that your program is running correctly you should */
/* turn off this macro by defining SOM_NoTest, or adding -DSOM_NoTest  */
/* to your makefile.                                                   */

#if !defined(SOM_NoTest) && defined(SOM_TestOn)
  #define SOM_TestCls(obj, class) (somTestCls(((SOMObject SOMSTAR)\
    ((void *)obj)), ((SOMClass SOMSTAR)((void *)class)), __FILE__, __LINE__))
  #define SOM_Measure
#else
  #define SOM_TestCls(obj, class) ((SOMObject SOMSTAR)((void *)obj))
#endif

/* Control the printing of method and procedure entry messages, */
/* 0-none, 1-user, 2-core&user */
SOMEXTERN int SOMDLINK SOM_TraceLevel;

/* Control the printing of warning messages, 0-none, 1-all */
SOMEXTERN int SOMDLINK SOM_WarnLevel;

/* Control the printing of successful assertions, 0-none, 1-user, */
/* 2-core&user */
SOMEXTERN int SOMDLINK SOM_AssertLevel;

/*
 *  Scans argv looking for flags -somt, -somtc, -soma -somac -somw setting
 *  SOM_TraceLevel, SOM_AssertLevel and SOM_WarnLevel as appropriate.
 *  argv is not modified
 */
#ifdef __IBMC__
  #pragma linkage(somCheckArgs, system)
#endif
SOMEXTERN void SOMLINK somCheckArgs(int argc, zString argv[]);

#define SOM_Error(c) ((*SOMError) (c,__FILE__, __LINE__))

#define SOM_NoTrace(c,m)

#ifdef _RETAIL
  #define SOM_Trace(c,m)
  #define SOM_TraceCore(c,m)
#else
  #define SOM_Trace(c,m) if (SOM_TraceLevel > 0) \
      somPrintf("\"%s\": %d:\tIn %s:%s \n", \
          __FILE__, __LINE__, c, m)

  #define SOM_TraceCore(c,m) if (SOM_TraceLevel > 1) \
      somPrintf("\"%s\": %d:\tIn %s:%s \n", \
          __FILE__, __LINE__, c, m)
#endif

#define SOM_Assert(condition,ecode) \
  (somAssert(condition, ecode, __FILE__, __LINE__, # condition))

#define SOM_AssertCore(condition,ecode) \
  (somAssertCore(condition, ecode, __FILE__, __LINE__, # condition))

#define SOM_Expect(condition) \
      somTest(condition, SOM_Warn, __FILE__, __LINE__, # condition)

#define SOM_WarnMsg(msg) \
  if (SOM_WarnLevel > 0) \
      somPrintf("\"%s\": %d:\tWarning: %s\n", __FILE__, __LINE__, msg)

#define SOM_Test(boolexp) \
    somTest(boolexp, SOM_Fatal, __FILE__, __LINE__, # boolexp)

#define SOM_TestC(boolexp) \
    somTest(boolexp, SOM_Warn, __FILE__, __LINE__, # boolexp)

/*
 *   Default method debug macro, can be overridden
 */
#ifndef SOMMethodDebug
#define SOMMethodDebug(c,m) SOM_Trace(c,m)
#endif

/*
 *  Error severity codes, these are added to the base error number to
 *  produce the full error code
 */

#define SOM_Ok        0x0
#define SOM_Warn      0x1
#define SOM_Ignore    0x2 /* don't do anything */
#define SOM_Fatal     0x9 /* terminate the program */
#define SOM_Template  0x5 /* use to identify msg templates */

#define SOM_EB 20000
#define SOM_FatalCode(code) (SOM_EB + (code)*10 + SOM_Fatal)
#define SOM_WarnCode(code) (SOM_EB + (code)*10 + SOM_Warn)
#define SOM_IgnoreCode(code) (SOM_EB + (code)*10 + SOM_Ignore)
#define SOM_OkCode(code) (SOM_EB + (code)*10 + SOM_Ok)
#define SOM_TemplateCode(code) (SOM_EB + (code)*10 + SOM_Template)
#define SOM_MsgCode(ecode) (((ecode)-SOM_EB)/10)

#define SOMERROR_MustOverride SOM_FatalCode(18)

#endif /* somcdev_h */
