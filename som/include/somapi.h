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

/* @(#) 2.44.2.7 src/somk/somapi.h, somk.api, som2.1 2/29/96 10:19:03 [7/30/96 14:46:36] */

/*
 *    SOMAPI.H
 *
 *    Note: Typedefs, function prototypes, and structure fields declared here
 *    explicitly use a star (*) to indicate references to objects that support
 *    IDL interfaces. For example, the first argument in the following
 *    function prototype is typed as pointer to an object that supports the
 *    SOMObject interface:
 *
 *    somMethodProc * SOMLINK somResolve(SOMObject *obj, somMToken mdata)
 *
 *    These types reflect the actual implementation of object references
 *    in SOM as pointers. The CORBA compliant C language bindings are
 *    designed so that programmers don't explicitly indicate pointers when
 *    declaring object reference types, but this header is included into
 *    the CORBA compliant C language binding files before this capability
 *    has been enabled. Thus, at this point in the processing of header files
 *    by a C compiler, it is appropriate that the explicit star appear in
 *    object reference types. You can view the header file som.h to see how
 *    the ability to omit the explicit star is enabled after this header is
 *    processed, but, in any case, you can be assured that the typedefs,
 *    function prototypes, and structure fields declared here all "work"
 *    with the CORBA compliant C language bindings.
 */

#ifndef somapi_h
#define somapi_h

/*  SOM Version Numbers  */
SOMEXTERN long SOMDLINK SOM_MajorVersion;
SOMEXTERN long SOMDLINK SOM_MinorVersion;

/*  SOM Thread Support  */
SOMEXTERN long SOMDLINK SOM_MaxThreads;

/*----------------------------------------------------------------------
 *  Typedefs for pointers to functions
 */

typedef void    SOMLINK somTD_classInitRoutine(SOMClass *, SOMClass *);
typedef int     SOMLINK somTD_SOMOutCharRoutine(char);
typedef int     SOMLINK somTD_SOMLoadModule(IN string /* className */,
                                        IN string     /* fileName */,
                                        IN string     /* functionName */,
                                        IN long    /* majorVersion */,
                                        IN long    /* minorVersion */,
                                        OUT somToken * /* modHandle */);
typedef int     SOMLINK somTD_SOMDeleteModule(IN somToken /* modHandle */);
typedef string SOMLINK somTD_SOMClassInitFuncName(void);
typedef somToken        SOMLINK somTD_SOMMalloc(IN size_t /* nbytes */);
typedef somToken        SOMLINK somTD_SOMCalloc(IN size_t /* element_count */,
                                        IN size_t /* element_size */);
typedef somToken        SOMLINK somTD_SOMRealloc(IN somToken /* memory */,
                                         IN size_t /* nbytes */);
typedef void    SOMLINK somTD_SOMFree(IN somToken /* memory */);
typedef void    SOMLINK somTD_SOMError(IN int      /* code */,
                                       IN string  /* fileName */,
                                       IN int      /* lineNum */);
typedef unsigned long SOMLINK somTD_SOMCreateMutexSem (OUT somToken *sem);
typedef unsigned long SOMLINK somTD_SOMRequestMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMReleaseMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMDestroyMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMGetThreadId (void);

/*----------------------------------------------------------------------
 *  Windows extra procedures:
 */

SOMEXTERN void SOMLINK somSetOutChar(somTD_SOMOutCharRoutine *outch);
SOMEXTERN SOMClassMgr * SOMLINK somMainProgram (void);
SOMEXTERN void SOMLINK somEnvironmentEnd (void);
SOMEXTERN boolean SOMLINK somAbnormalEnd (void);

/*--------------------------------------------------------*/


#ifndef SOM_MDT_DEFINED
/* -- Method/Data Tokens -- For locating methods and data members. */
typedef somToken somMToken;
typedef somToken somDToken;
#endif

/*----------------------------------------------------------------------
 *  The Method Table Structure
 */

#ifndef SOM_MTAB_DEFINED
/* -- to specify an embedded object (or array of objects). */
typedef struct {
    SOMClass     **copp; /* address of class of object ptr */
    long         cnt;    /* object count */
    long         offset; /* Offset to pointer (to embedded objs) */
} somEmbeddedObjStruct;

#ifndef SOM_CLASSINFO_DEFINED
typedef somToken somClassInfo;
#endif

typedef struct somMethodTabStruct {
    SOMClass        *classObject;
    somClassInfo    *classInfo;
    char            *className;
    long            instanceSize;
    long            dataAlignment;
    long            mtabSize;
    long            protectedDataOffset; /* from class's introduced data */
    somDToken       protectedDataToken;
    somEmbeddedObjStruct *embeddedObjs;
    /* remaining structure is opaque */
    somMethodProc* entries[1];
} somMethodTab, *somMethodTabPtr;
#endif


/* -- For building lists of method tables */
typedef
    struct somMethodTabList {
        somMethodTab             *mtab;
        struct somMethodTabList  *next;
    } somMethodTabList, *somMethodTabs;


/* -- For building lists of class objects */
typedef
    struct somClassList {
        SOMClass   *cls;
        struct somClassList *next;
    } somClassList, *somClasses;

/* -- For building lists of objects */
typedef struct somObjectList {
    SOMObject   *obj;
    struct somObjectList *next;
} somObjectList, *somObjects;

/*---------------------------------------------------------------------
 * C++-style constructors are called initializers in SOM. Initializers
 * are methods that receive a pointer to a somCtrlStruct as an argument.
 */

typedef  struct  {
    SOMClass      *cls;  /* the class whose introduced data is to be initialized */
    somMethodProc *defaultInit;
    somMethodProc *defaultCopyInit;
    somMethodProc *defaultConstCopyInit;
    somMethodProc *defaultNCArgCopyInit;
    long          dataOffset;
    somMethodProc *legacyInit;
} somInitInfo;

typedef struct {
    SOMClass      *cls;  /* the class whose introduced data is to be destroyed */
    somMethodProc *defaultDestruct;
    long          dataOffset;
    somMethodProc *legacyUninit;
} somDestructInfo;

typedef struct {
    SOMClass      *cls;  /* the class whose introduced data is to be assigned */
    somMethodProc *defaultAssign;
    somMethodProc *defaultConstAssign;
    somMethodProc *defaultNCArgAssign;
    somMethodProc *udaAssign;
    somMethodProc *udaConstAssign;
    long          dataOffset;
} somAssignInfo;

typedef octet *somBooleanVector;
#ifndef SOM_CTI_DEFINED
typedef somToken somCtrlInfo;
#endif

typedef struct somInitCtrlStruct {
    somBooleanVector mask; /* an array of booleans to control ancestor calls */
    somInitInfo     *info; /* an array of structs */
    int              infoSize; /* increment for info access */
    somCtrlInfo      ctrlInfo;
} somInitCtrl,som3InitCtrl;

typedef struct somDestructCtrlStruct {
    somBooleanVector  mask; /* an array of booleans to control ancestor calls */
    somDestructInfo  *info; /* an array of structs */
    int               infoSize; /* increment for info access */
    somCtrlInfo       ctrlInfo;
} somDestructCtrl,som3DestructCtrl;

typedef struct somAssignCtrlStruct {
    somBooleanVector  mask; /* an array of booleans to control ancestor calls */
    somAssignInfo    *info; /* an array of structs */
    int               infoSize; /* increment for info access */
    somCtrlInfo       ctrlInfo;
} somAssignCtrl,som3AssignCtrl;




/*----------------------------------------------------------------------
 *  The Class Data Structures -- these are used to implement static
 *  method and data interfaces to SOM objects.
 */

/* -- (Generic) Class data Structure */
typedef struct {
    SOMClass *classObject; /* changed by shadowing */
    somToken tokens[1];    /* method tokens, etc. */
} somClassDataStructure, *somClassDataStructurePtr;

/*
 * A special info access structure pointed to by
 * the parentMtab entry of somCClassDataStructure.
 */
typedef void somTP_somRenewNoInitNoZeroThunk(void*);
#if (defined(__IBMC__) && !defined(__cplusplus))
#pragma linkage(somTP_somRenewNoInitNoZeroThunk, system)
#endif
typedef somTP_somRenewNoInitNoZeroThunk *somTD_somRenewNoInitNoZeroThunk;

typedef struct {
    somMethodTab    *mtab; /* this class' mtab -- changed by shadowing */
    somMethodTabs   next; /* the parent mtabs -- unchanged by shadowing */
    SOMClass        *classObject; /* unchanged by shadowing */
    somTD_somRenewNoInitNoZeroThunk somRenewNoInitNoZeroThunk; /* changed by shadowing */
    long            instanceSize; /* changed by shadowing */
    somMethodProc   **initializers; /* resolved initializer array in releaseorder */
    somMethodProc   **resolvedMTokens;  /* resolved methods */
    somInitCtrl     initCtrl; /* these fields are filled in if somDTSClass&2 is on */
    somDestructCtrl destructCtrl;
    somAssignCtrl   assignCtrl;
    long            embeddedTotalCount;
    long            hierarchyTotalCount;
    long            unused;
} somParentMtabStruct, *somParentMtabStructPtr;

/*
 * (Generic) Auxiliary Class Data Structure
 */
typedef struct {
    somParentMtabStructPtr parentMtab;
    somDToken              instanceDataToken;
    somMethodProc          *wrappers[1]; /* for valist methods */
} somCClassDataStructure, *somCClassDataStructurePtr;


/*---------------------------------------------------------------------
 *  Offset-based method resolution.
 */

SOMEXTERN somMethodProc * SOMLINK somResolve(SOMObject *obj, somMToken mdata);
SOMEXTERN somMethodProc * SOMLINK somParentResolve(somMethodTabs parentMtabs,
                                         somMToken mToken);
SOMEXTERN somMethodProc * SOMLINK somParentNumResolve(somMethodTabs parentMtabs,
                                         int parentNum,
                                         somMToken mToken);
SOMEXTERN somMethodProc * SOMLINK somClassResolve(SOMClass *, somMToken mdata);
SOMEXTERN somMethodProc * SOMLINK somResolveTerminal(SOMClass *, somMToken mdata);
SOMEXTERN somMethodProc * SOMLINK somPCallResolve(SOMObject *obj,
                                                  SOMClass *callingCls,
                                                  somMToken method);
SOMEXTERN somMethodProc * SOMLINK somAncestorResolve(SOMObject *obj, /* the object */
                                           somCClassDataStructure *ccds, /* id the ancestor */
                                           somMToken mToken);
SOMEXTERN somMethodProc * SOMLINK somResolveByName(SOMObject *obj,
                                                   char *methodName);
/*------------------------------------------------------------------------------
 * Offset-based data resolution
 */
SOMEXTERN somToken SOMLINK somDataResolve(SOMObject *obj, somDToken dataId);
SOMEXTERN somToken SOMLINK somDataResolveChk(SOMObject *obj, somDToken dataId);


/*----------------------------------------------------------------------
 *  Misc. procedures:
 */

/*
 *  Create and initialize the SOM environment
 *
 *  Can be called repeatedly
 *
 *  Will be called automatically when first object (including a class
 *  object) is created, if it has not already been done.
 *
 *  Returns the SOMClassMgrObject
 */
SOMEXTERN SOMClassMgr * SOMLINK somEnvironmentNew (void);


/*
 * Test whether <obj> is a valid SOM object. This test is based solely on
 * the fact that (on this architecture) the first word of a SOM object is a
 * pointer to its method table. The test is therefore most correctly understood
 * as returning true if and only if <obj> is a pointer to a pointer to a
 * valid SOM method table. If so, then methods can be invoked on <obj>.
 */
SOMEXTERN boolean SOMLINK somIsObj(somToken obj);

/*
 * Return the class that introduced the method represented by a given method token.
 */
SOMEXTERN SOMClass* SOMLINK somGetClassFromMToken(somMToken mToken);


/*----------------------------------------------------------------------
 *  String Manager: stem <somsm>
 */
SOMEXTERN somId SOMLINK somCheckId (somId id);
/* makes sure that the id is registered and in normal form, returns */
/* the id */

SOMEXTERN int SOMLINK somRegisterId(somId id);
/* Same as somCheckId except returns 1 (true) if this is the first */
/* time the string associated with this id has been registered, */
/* returns 0 (false) otherwise */

SOMEXTERN somId SOMLINK somIdFromString (string aString);
/* caller is responsible for freeing the returned id with SOMFree */

SOMEXTERN string SOMLINK somStringFromId (somId id);

SOMEXTERN int SOMLINK somCompareIds(somId id1, somId id2);
/* returns true (1) if the two ids are equal, else false (0) */

SOMEXTERN unsigned long SOMLINK somTotalRegIds(void);
/* Returns the total number of ids that have been registered so far, */
/* you can use this to advise the SOM runtime concerning expected */
/* number of ids in later executions of your program, via a call to */
/* somSetExpectedIds defined below */

SOMEXTERN void SOMLINK somSetExpectedIds(unsigned long numIds);
/* Tells the SOM runtime how many unique ids you expect to use during */
/* the execution of your program, this can improve space and time */
/* utilization slightly, this routine must be called before the SOM */
/* environment is created to have any effect */

SOMEXTERN unsigned long SOMLINK somUniqueKey(somId id);
/* Returns the unique key for this id, this key will be the same as the */
/* key in another id if and only if the other id refers to the same */
/* name as this one */

SOMEXTERN void SOMLINK somBeginPersistentIds(void);
/* Tells the id manager that strings for any new ids that are */
/* registered will never be freed or otherwise modified. This allows */
/* the id manager to just use a pointer to the string in the */
/* unregistered id as the master copy of the ids string. Thus saving */
/* space */
/* Under normal use (where ids are static varibles) the string */
/* associated with an id would only be freed if the code module in */
/* which it occured was unloaded */

SOMEXTERN void SOMLINK somEndPersistentIds(void);
/* Tells the id manager that strings for any new ids that are */
/* registered may be freed or otherwise modified.  Therefore the id */
/* manager must copy the strings inorder to remember the name of an */
/* id. */

/*----------------------------------------------------------------------
 *  Class Manager: SOMClassMgr, stem <somcm>
 */

/* Global class manager object */
SOMEXTERN SOMClassMgr * SOMDLINK SOMClassMgrObject;

/* The somRegisterClassLibrary function is provided for use
 * in SOM class libraries on platforms that have loader-invoked
 * entry points associated with shared libraries (DLLs).
 *
 * This function registers a SOM Class Library with the SOM Kernel.
 * The library is identified by its file name and a pointer
 * to its initialization routine.  Since this call may occur
 * prior to the invocation of somEnvironmentNew, its actions
 * are deferred until the SOM environment has been initialized.
 * At that time, the SOMClassMgrObject is informed of all
 * pending library initializations via the _somRegisterClassLibrary
 * method.  The actual invocation of the library's initialization
 * routine will occur during the execution of the SOM_MainProgram
 * macro (for statically linked libraries), or during the _somFindClass
 * method (for libraries that are dynamically loaded).
 */
SOMEXTERN void SOMLINK somRegisterClassLibrary (string libraryName,
    somMethodProc *libraryInitRtn);
SOMEXTERN void SOMLINK somUnregisterClassLibrary (string libraryName);

/*----------------------------------------------------------------------
 * Method Stubs -- Signature Support
 *
 *
 * This section defines the structures used to pass method signature
 * ingo to the runtime. This supports selection of generic apply stubs
 * and runtime generation of redispatchstubs when these are needed. The
 * information is registered with the runtime when methods are defined.
 *
 * When calling somAddStaticMethod, if the redispatchStub is -1, then a
 * pointer to a struct of type somApRdInfo is passed as the applyStub.
 * Otherwise, the passed redispatchstub and applystub are taken as given.
 * When calling somAddDynamicMethod, an actual apply stub must be passed.
 * Redispatch stubs for dynamic methods are not available, nor is
 * automated support for dynamic method apply stubs. The following
 * atructures only appropriate in relation to static methods.
 *
 * In SOMr2, somAddStaticMethod can be called with an actual redispatchstub
 * and applystub *ONLY* if the method doesn't return a structure. Recall
 * that no SOMr1 methods returned structures, so SOMr1 binaries obey this
 * restriction. The reason for this rule is that SOMr2 *may* use thunks,
 * and thunks need to know if a structure is returned. We therefore assume
 * that if no signature information is provided for a method through the
 * somAddStaticMethod interface, then the method returns a scalar.
 *
 * If a structure is returned, then a -1 *must* be passed to
 * somAddStaticMethod as a redispatchstub. In any case, if a -1 is passed,
 * then this means that the applystub actually points to a structure of type
 * somApRdInfo. This structure is used to hold and access signature
 * information encoded as follows.
 *
 * If the somApRdInfo pointer is NULL, then, if the runtime was built with
 * SOM_METHOD_STUBS defined, a default signature is assumed (no arguments,
 * and no structure returned); otherwise, the stubs are taken as
 * somDefaultMethod (which produces a runtime error when used) if dynamic
 * stubs are not available.
 *
 * If the somApRdInfo pointer is not NULL, then the structure it points to can
 * either include (non-null) redispatch and applystubs (the method is then
 * assumed to return a structure), or null stubs followed by information needed
 * to generate necessary stubs dynamically.
 */

typedef unsigned long somRdAppType; /* method signature code -- see def below */
typedef unsigned long somFloatMap[13]; /* float map -- see def below */
typedef struct somMethodInfoStruct {
    somRdAppType       callType;
    long               va_listSize;
    somFloatMap        *float_map;
} somMethodInfo;

typedef struct somApRdInfoStruct {
    somMethodProc *rdStub;
    somMethodProc *apStub;
    somMethodInfo *stubInfo;
} somApRdInfo;

/*
 * Values for somRdAppType are generated by summing one from column A and one
 * from column B of the following constants:
 */
/* Column A: return type */
#define SOMRdRetsimple     0 /* Return type is a non-float fullword */
#define SOMRdRetfloat      2 /* Return type is (single) float */
#define SOMRdRetdouble     4 /* Return type is double */
#define SOMRdRetlongdouble 6 /* Return type is long double */
#define SOMRdRetaggregate  8 /* Return type is struct or union */
#define SOMRdRetbyte      10 /* Return type is a byte */
#define SOMRdRethalf      12 /* Return type is a (2 byte) halfword */
/* Column B: are there any floating point scalar arguments? */
#define SOMRdNoFloatArgs   0
#define SOMRdFloatArgs     1

/* A somFloatMap is only needed on RS/6000 */
/*
 * This is an array of offsets for up to the first 13 floating point arguments.
 * If there are fewer than 13 floating point arguments, then there will be
 * zero entries following the non-zero entries which represent the float args.
 * A non-zero entry signals either a single- or a double-precision floating point
 * argument. For a double-precision argument, the entry is the stack
 * frame offset. For a single-precision argument the entry is the stack
 * frame offset + 1. For the final floating point argument, add 2 to the
 * code that would otherwise be used.
 */
#define SOMFMSingle 1        /* add to indicate single-precision */
#define SOMFMLast   2        /* add to indicate last floating point arg */


/*----------------------------------------------------------------------
 * -- somApply --
 *
 * This routine replaces direct use of applyStubs in SOMr1. The reason
 * for the replacement is that the SOMr1 style of applyStub is not
 * generally available in SOMr2, which uses a fixed set of applyStubs,
 * according to method information in the somMethodData. In particular,
 * neither the redispatch stub nor the apply stub found in the method
 * data structure are necessarily useful as such. The method somGetRdStub
 * is the way to get a redispatch stub, and the above function is the
 * way to call an apply stub. If an appropriate apply stub for the
 * method indicated by md is available, then this is invoked and TRUE is
 * returned; otherwise FALSE is returned.
 *
 * The va_list passed to somApply *must* include the target object,
 * somSelf, as its first entry, and any single precision floating point
 * arguments being passed to the the method procedure must be
 * represented on the va_list using double precision values. retVal cannot
 * be NULL.
 */

#ifndef SOM_SMD_DEFINED
typedef somToken somSharedMethodData;
#endif

typedef struct somMethodDataStruct {
    somId id;
    long type;          /* 0=static, 1=dynamic 2=nonstatic */
    somId descriptor;        /* for use with IR interfaces */
    somMToken mToken;        /* NULL for dynamic methods */
    somMethodPtr method;     /* depends on resolution context */
    somSharedMethodData *shared;
} somMethodData, *somMethodDataPtr;


SOMEXTERN boolean SOMLINK somApply(SOMObject *somSelf,
                                somToken *retVal,
                                somMethodDataPtr mdPtr,
                                va_list ap);

/*---------------------------------------------------------------------
 * -- somBuildClass --
 *
 * This procedure automates construction of a new class object. A variety of
 * special structures are used to allow language bindings to statically define
 * the information necessary to specify a class. Pointers to these static
 * structures are accumulated into an overall "static class information"
 * structure or SCI, passed to somBuildClass. The SCI has evolved over time.
 * The current version is defined here.
 */

#define SOM_SCILEVEL 4


/* The SCI includes the following information:
 *
 * The address of a class's ClassData structure is passed.
 * This structure should have the external name,
 * <className>ClassData. The classObject field should be NULL
 * (if it is not NULL, then a new class will not be built). somBuildClass will
 * set this field to the address of the new class object when it is built.
 *
 * The address of the class's auxiliary ClassData structure is passed.
 * Thi structure should have the external name,
 * <className>CClassData. The parentMtab field will be set by somBuildClass.
 * This field often allows method calls to a class object to be avoided.
 *
 * The other structures referenced by the static class information (SCI)
 * are used to:
 */

/*
 * to specify a static method. The methodId used here must be
 * a simple name (i.e., no colons). In all other cases,
 * where a somId is used to identify a registered method,
 * the somId can include explicit scoping. An explicitly-scoped
 * method name is called a method descriptor. For example,
 * the method introduced by SOMObject as somGetClass has the
 * method descriptor "SOMObject::somGetClass". When a
 * class is contained in an IDL module, the descriptor syntax
 * <moduleName>::<className>::<methodName> can be used. Method
 * descriptors can be useful when a class supports different methods
 * that have the same name (note: IDL prevents this from occuring
 * statically, but SOM itself has no problems with this).
 */
typedef struct somStaticMethodStruct {
    somMToken *classData;
    somId *methodId; /* this must be a simple name (no colons) */
    somId *methodDescriptor;
    somMethodProc *method;
    somMethodProc *redispatchStub;
    somMethodProc *applyStub;
} somStaticMethod_t;

/* to specify an overridden method */
typedef struct somOverideMethodStruct {
    somId *methodId; /* this can be a method descriptor */
    somMethodProc *method;
} somOverrideMethod_t;

/* to inherit a specific parent's method implementation */
typedef struct somInheritedMethodStruct {
    somId     *methodId;  /* identify the method */
    long parentNum;       /* identify the parent */
    somMToken *mToken;    /* for parentNumresolve */
} somInheritedMethod_t;

/* to register a method that has been moved from this */
/* class <cls> upwards in the class hierachy to class <dest> */
typedef struct somMigratedMethodStruct {
    somMToken *clsMToken; /* points into the <cls> classdata structure */
                          /* the method token in <dest> will copied here */
    somMToken *destMToken;/* points into the <dest> classdata structure */
                          /* the method token here will be copied to <cls> */
} somMigratedMethod_t;

/* to specify non-internal data */
typedef struct somNonInternalDataStruct {
    somDToken *classData;
    char *basisForDataOffset;
} somNonInternalData_t;

/* to specify a "procedure" or "classdata" */
typedef struct somProcMethodsStruct {
    somMethodProc **classData, *pEntry;
} somProcMethods_t;

/* to specify a general method "action" using somMethodStruct */
/*
  the type of action is specified by loading the type field of the
  somMethodStruct. There are three bit fields in the overall type:

  action (in type & 0xFF)
   0: static -- (i.e., virtual) uses somAddStaticMethod
   1: dynamic -- uses somAddDynamicMethod (classData==0)
   2: nonstatic -- (i.e., nonvirtual) uses somAddMethod
   3: udaAssign -- registers a method as the udaAssign (but doesn't add the method)
   4: udaConstAssign -- like 3, this doesn't add the method
   5: somClassResolve Override (using the class pointed to by *classData)
   6: somMToken Override (using the method token pointed to by methodId)
                         (note: classData==0 for this)
   7: classAllocate -- indicates the default heap allocator for this class.
                       If classData == 0, then method is the code address (or NULL)
                       If classData != 0, then *classData is the code address.
                       No other info required (or used)
   8: classDeallocate -- like 7, but indicates the default heap deallocator.
   9: classAllocator -- indicates a non default heap allocator for this class.
                        like 7, but a methodDescriptor can be given.

   === the following is not currently supported ===
   binary data access -- in (type & 0x100), valid for actions 0,1,2,5,6
   0: the method procedure doesn't want binary data access
   1: the method procedure does want binary data access

   aggregate return -- in (type & 0x200), used when binary data access requested
   0: method procedure doesn't return a structure
   1: method procedure does return a structure
*/

typedef struct somMethodStruct {
    unsigned long type;
    somMToken *classData;
    somId *methodId;
    somId *methodDescriptor;
    somMethodProc *method;
    somMethodProc *redispatchStub;
    somMethodProc *applyStub;
} somMethods_t;

/* to specify a varargs function */
typedef struct somVarargsFuncsStruct {
    somMethodProc **classData, *vEntry;
} somVarargsFuncs_t;

/* to specify dynamically computed information (incl. embbeded objs) */
typedef struct {
    int   version;              /* 1 for now */
    long    instanceDataSize;   /* true size (incl. embedded objs) */
    long    dataAlignment;      /* true alignment */
    somEmbeddedObjStruct *embeddedObjs; /* array end == null copp */
} somDynamicSCI;


/*
   to specify a DTS class, use the somDTSClass entry in the following
   data structure. This entry is a bit vector interpreted as follows:

   (somDTSClass & 0x0001) == the class is a DTS C++ class
   (somDTSClass & 0x0002) == the class wants the initCtrl entries
                             of the somParentMtabStruct filled in.

*/



/*
 *  The Static Class Info Structure passed to somBuildClass
 */

typedef struct somStaticClassInfoStruct {
    unsigned long layoutVersion;  /* this struct defines layout version SOM_SCILEVEL */
    unsigned long numStaticMethods;   /* count of smt entries */
    unsigned long numStaticOverrides; /* count of omt entries */
    unsigned long numNonInternalData; /* count of nit entries */
    unsigned long numProcMethods;     /* count of pmt entries */
    unsigned long numVarargsFuncs;    /* count of vft entries */
    unsigned long majorVersion;
    unsigned long minorVersion;
    unsigned long instanceDataSize;   /* instance data introduced by this class */
    unsigned long maxMethods;         /* count numStaticMethods and numMethods */
    unsigned long numParents;
    somId    classId;
    somId    explicitMetaId;
    long implicitParentMeta;
    somId                  *parents;
    somClassDataStructure  *cds;
    somCClassDataStructure *ccds;
    somStaticMethod_t      *smt; /* basic "static" methods for mtab */
    somOverrideMethod_t    *omt; /* overrides for mtab */
    char                   *nitReferenceBase;
    somNonInternalData_t   *nit; /* datatokens for instance data */
    somProcMethods_t       *pmt; /* Arbitrary ClassData members */
    somVarargsFuncs_t      *vft; /* varargs stubs */
    somTP_somClassInitFunc *cif; /* class init function */
    /* end of layout version 1 */

    /* begin layout version 2 extensions */
    long dataAlignment; /* the desired byte alignment for instance data */
    /* end of layout version 2 */

#define SOMSCIVERSION 1

    /* begin layout version 3 extensions */
    long numDirectInitClasses;
    somId *directInitClasses;
    unsigned long numMethods; /* general (including nonstatic) methods for mtab */
    somMethods_t       *mt;
    unsigned long protectedDataOffset; /* access = resolve(instanceDataToken) + offset */
    unsigned long somSCIVersion;  /* used during development. currently = 1 */
    unsigned long numInheritedMethods;
    somInheritedMethod_t *imt; /* inherited method implementations */
    unsigned long numClassDataEntries; /* should always be filled in */
    somId *classDataEntryNames; /* either NULL or ptr to an array of somIds */
    unsigned long numMigratedMethods;
    somMigratedMethod_t *mmt; /* migrated method implementations */
    unsigned long numInitializers; /* the initializers for this class */
    somId *initializers;     /* in order of release */
    unsigned long somDTSClass; /* used to identify a DirectToSOM class */
    somDynamicSCI *dsci;  /* used to register dynamically computed info */
    /* end of layout version 3 */

} somStaticClassInfo, *somStaticClassInfoPtr;

SOMEXTERN SOMClass * SOMLINK somBuildClass (
                    long inherit_vars,
                    somStaticClassInfo *sci,
                    long majorVersion,
                    long minorVersion);

  /*
  The arguments to somBuildClass are as follows:

   inherit_vars: a bit mask used to control inheritance of implementation
   Implementation is inherited from parent i iff the bit 1<<i is on, or i>=32.

   sci: the somStaticClassInfo defined above.

   majorVersion, minorVersion: the version of the class implementation.

   */


/*---------------------------------------------------------------------
 *  Used by old single-inheritance emitters to make class creation
 *  an atomic operation. Kept for backwards compatability.
 */
SOMEXTERN void SOMLINK somConstructClass (
                    somTD_classInitRoutine *classInitRoutine,
                    SOMClass *parentClass,
                    SOMClass *metaClass,
                    somClassDataStructure *cds);


/*
 * Uses <SOMOutCharRoutine> to output its arguments under control of the ANSI C
 * style format.  Returns the number of characters output.
 */
SOMEXTERN int SOMLINK somPrintf (string fmt, ...);
/*
 * vprint form of somPrintf
 */
SOMEXTERN int SOMLINK somVprintf (string fmt, va_list ap);
/*
 * Outputs (via somPrintf) blanks to prefix a line at the indicated level
 */
SOMEXTERN void SOMLINK somPrefixLevel (long level);
/*
 * Combines somPrefixLevel and somPrintf
 */
SOMEXTERN int SOMLINK somLPrintf (int level, string fmt, ...);

/*
 *  Replaceable character output handler.
 *  Points to the character output routine to be used in development
 *  support.  Initialized to <somOutChar>, but may be reset at anytime.
 *  Should return 0 (false) if an error occurs and 1 (true) otherwise.
 */

SOMEXTERN somTD_SOMOutCharRoutine * SOMDLINK SOMOutCharRoutine;

/*----------------------------------------------------------------------
 * Pointers to routines used to do dynamic code loading and deleting
 */

SOMEXTERN somTD_SOMLoadModule     * SOMDLINK SOMLoadModule;
SOMEXTERN somTD_SOMDeleteModule   * SOMDLINK SOMDeleteModule;
SOMEXTERN somTD_SOMClassInitFuncName * SOMDLINK SOMClassInitFuncName;

/*----------------------------------------------------------------------
 *  Replaceable SOM Memory Management Interface
 *
 *  External procedure variables SOMCalloc, SOMFree, SOMMalloc, SOMRealloc
 *  have the same interface as their standard C-library analogs.
 */

SOMEXTERN somTD_SOMCalloc  * SOMDLINK SOMCalloc;
SOMEXTERN somTD_SOMFree    * SOMDLINK SOMFree;
SOMEXTERN somTD_SOMMalloc  * SOMDLINK SOMMalloc;
SOMEXTERN somTD_SOMRealloc * SOMDLINK SOMRealloc;

/*----------------------------------------------------------------------
 *  Replaceable SOM Error handler
 */

SOMEXTERN somTD_SOMError * SOMDLINK SOMError;

/*----------------------------------------------------------------------
 *  Replaceable SOM Semaphore Operations
 *
 *  These operations are used by the SOM Kernel to make thread-safe
 *  state changes to internal resources.
 */
SOMEXTERN somTD_SOMCreateMutexSem  * SOMDLINK SOMCreateMutexSem;
SOMEXTERN somTD_SOMRequestMutexSem * SOMDLINK SOMRequestMutexSem;
SOMEXTERN somTD_SOMReleaseMutexSem * SOMDLINK SOMReleaseMutexSem;
SOMEXTERN somTD_SOMDestroyMutexSem * SOMDLINK SOMDestroyMutexSem;

/*----------------------------------------------------------------------
 *  Replaceable SOM Thread Identifier Operation
 *
 *  This operation is used by the SOM Kernel to index data unique to the
 *  currently executing thread.  It must return a small integer that
 *  uniquely represents the current thread within the current process.
 */
SOMEXTERN somTD_SOMGetThreadId * SOMDLINK SOMGetThreadId;

/*----------------------------------------------------------------------
 * Externals used in the implementation of SOM, but not part of the
 * SOM API.
 */

SOMEXTERN SOMObject * SOMLINK somTestCls(SOMObject *obj, SOMClass *classObj,
                                         string fileName, int lineNumber);
SOMEXTERN void SOMLINK somTest(int condition, int severity, string fileName,
                               int lineNum, string msg);
SOMEXTERN void SOMLINK somAssert(int condition, int ecode,
                                 string fileName, int lineNum, string msg);


#endif /* somapi_h */
