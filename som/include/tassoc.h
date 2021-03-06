
/*
 * This file was generated by the SOM Compiler.
 * FileName: tassoc.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.17
 *     SOM Emitter emith: 2.47
 */

/*
 *    COMPONENT_NAME: somuc
 * 
 *    ORIGINS: 82, 81, 27
 * 
 * 
 *     25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *    All Rights Reserved
 *    Licensed Materials - Property of IBM
 *    US Government Users Restricted Rights - Use, duplication or
 *    disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 * 
 *    Copyright � 1988 Apple Computer, Inc. All rights reserved.
 * 
 * CLASS_NAME: somf_TAssoc
 * 
 * DESCRIPTION: A somf_TAssoc is used to hold a pair of objects.  Typically, these
 *              structures are owned by some other higher level object
 *              (e.g. a dictionary) and are usually not returned to the user.
 * 
 *  This file was generated by the SOM Compiler.
 *  FileName: TAssoc.id2.
 *  Generated using:
 *      SOM Precompiler spc: 6.13
 *      SOM Emitter emitidl.dll: 6.18
 */


#ifndef SOM_somf_TAssoc_h
#define SOM_somf_TAssoc_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef somf_TAssoc
#define somf_TAssoc SOMObject
#endif
#include <mcollect.h>

/*
 * Start of bindings for IDL types
 */

#ifndef SOMClass
    #define SOMClass SOMObject
#endif /* SOMClass */

/*
 * End of bindings for IDL types.
 */

#define somf_TAssoc_MajorVersion 2
#define somf_TAssoc_MinorVersion 1

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define somf_TAssocNewClass c
#pragma linkage(c, system)
#define somf_TAssocClassData d
#define somf_TAssocCClassData x
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define somf_TAssoc_classObj somf_TAssocClassData.classObject
#define _somf_TAssoc somf_TAssoc_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK somf_TAssocNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(somf_TAssocNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct somf_TAssocClassDataStructure {
	SOMClass *classObject;
	somMToken somfGetKey;
	somMToken somfGetValue;
	somMToken somfSetKey;
	somMToken somfSetValue;
	somMToken somfTAssocInitMM;
	somMToken somfTAssocInitM;
} SOMDLINK somf_TAssocClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct somf_TAssocCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK somf_TAssocCClassData;

/*
 * New and Renew macros for somf_TAssoc
 */
#define somf_TAssocNew() \
   ( _somf_TAssoc ? \
	_somNew(_somf_TAssoc) \
	: ( somf_TAssocNewClass(\
		somf_TAssoc_MajorVersion, \
		somf_TAssoc_MinorVersion),\
	   _somNew(_somf_TAssoc)))
#define somf_TAssocRenew(buf) \
   ( _somf_TAssoc ? \
	_somRenew(_somf_TAssoc, buf) \
	: ( somf_TAssocNewClass(\
		somf_TAssoc_MajorVersion, \
		somf_TAssoc_MinorVersion),\
	   _somRenew(_somf_TAssoc, buf)))

/*
 * Override method: somInit
 */
#define somf_TAssoc_somInit(somSelf) \
	SOMObject_somInit(somSelf)

/*
 * Override method: somUninit
 */
#define somf_TAssoc_somUninit(somSelf) \
	SOMObject_somUninit(somSelf)

/*
 * New Method: somfTAssocInitMM
 */
typedef somf_TAssoc*   SOMLINK somTP_somf_TAssoc_somfTAssocInitMM(somf_TAssoc *somSelf, Environment *ev, 
		somf_MCollectible* k, 
		somf_MCollectible* v);
#pragma linkage(somTP_somf_TAssoc_somfTAssocInitMM, system)
typedef somTP_somf_TAssoc_somfTAssocInitMM *somTD_somf_TAssoc_somfTAssocInitMM;
/*
 *  Initialize somf_TAssoc to an existing key (k) and value (v).
 */
#define somMD_somf_TAssoc_somfTAssocInitMM "::somf_TAssoc::somfTAssocInitMM"
#define somf_TAssoc_somfTAssocInitMM(somSelf,ev,k,v) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfTAssocInitMM) \
	(somSelf,ev,k,v))
#ifndef SOMGD_somfTAssocInitMM
    #if (defined(_somfTAssocInitMM) || defined(__somfTAssocInitMM))
        #undef _somfTAssocInitMM
        #undef __somfTAssocInitMM
        #define SOMGD_somfTAssocInitMM 1
    #else
        #define _somfTAssocInitMM somf_TAssoc_somfTAssocInitMM
    #endif /* _somfTAssocInitMM */
#endif /* SOMGD_somfTAssocInitMM */

/*
 * New Method: somfTAssocInitM
 */
typedef somf_TAssoc*   SOMLINK somTP_somf_TAssoc_somfTAssocInitM(somf_TAssoc *somSelf, Environment *ev, 
		somf_MCollectible* k);
#pragma linkage(somTP_somf_TAssoc_somfTAssocInitM, system)
typedef somTP_somf_TAssoc_somfTAssocInitM *somTD_somf_TAssoc_somfTAssocInitM;
/*
 *  Initialize somf_TAssoc to an existing key (k).  The value is set to SOMF_NIL.
 */
#define somMD_somf_TAssoc_somfTAssocInitM "::somf_TAssoc::somfTAssocInitM"
#define somf_TAssoc_somfTAssocInitM(somSelf,ev,k) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfTAssocInitM) \
	(somSelf,ev,k))
#ifndef SOMGD_somfTAssocInitM
    #if (defined(_somfTAssocInitM) || defined(__somfTAssocInitM))
        #undef _somfTAssocInitM
        #undef __somfTAssocInitM
        #define SOMGD_somfTAssocInitM 1
    #else
        #define _somfTAssocInitM somf_TAssoc_somfTAssocInitM
    #endif /* _somfTAssocInitM */
#endif /* SOMGD_somfTAssocInitM */

/*
 * New Method: somfGetKey
 */
typedef somf_MCollectible*   SOMLINK somTP_somf_TAssoc_somfGetKey(somf_TAssoc *somSelf, Environment *ev);
#pragma linkage(somTP_somf_TAssoc_somfGetKey, system)
typedef somTP_somf_TAssoc_somfGetKey *somTD_somf_TAssoc_somfGetKey;
/*
 *  Get the key (fKey) to the associated pair.
 */
#define somMD_somf_TAssoc_somfGetKey "::somf_TAssoc::somfGetKey"
#define somf_TAssoc_somfGetKey(somSelf,ev) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfGetKey) \
	(somSelf,ev))
#ifndef SOMGD_somfGetKey
    #if (defined(_somfGetKey) || defined(__somfGetKey))
        #undef _somfGetKey
        #undef __somfGetKey
        #define SOMGD_somfGetKey 1
    #else
        #define _somfGetKey somf_TAssoc_somfGetKey
    #endif /* _somfGetKey */
#endif /* SOMGD_somfGetKey */

/*
 * New Method: somfGetValue
 */
typedef somf_MCollectible*   SOMLINK somTP_somf_TAssoc_somfGetValue(somf_TAssoc *somSelf, Environment *ev);
#pragma linkage(somTP_somf_TAssoc_somfGetValue, system)
typedef somTP_somf_TAssoc_somfGetValue *somTD_somf_TAssoc_somfGetValue;
/*
 *  Get the value (fValue) to the associated pair.
 */
#define somMD_somf_TAssoc_somfGetValue "::somf_TAssoc::somfGetValue"
#define somf_TAssoc_somfGetValue(somSelf,ev) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfGetValue) \
	(somSelf,ev))
#ifndef SOMGD_somfGetValue
    #if (defined(_somfGetValue) || defined(__somfGetValue))
        #undef _somfGetValue
        #undef __somfGetValue
        #define SOMGD_somfGetValue 1
    #else
        #define _somfGetValue somf_TAssoc_somfGetValue
    #endif /* _somfGetValue */
#endif /* SOMGD_somfGetValue */

/*
 * New Method: somfSetKey
 */
typedef void   SOMLINK somTP_somf_TAssoc_somfSetKey(somf_TAssoc *somSelf, Environment *ev, 
		somf_MCollectible* k);
#pragma linkage(somTP_somf_TAssoc_somfSetKey, system)
typedef somTP_somf_TAssoc_somfSetKey *somTD_somf_TAssoc_somfSetKey;
/*
 *  Set the key (fKey) to the associated pair.
 */
#define somMD_somf_TAssoc_somfSetKey "::somf_TAssoc::somfSetKey"
#define somf_TAssoc_somfSetKey(somSelf,ev,k) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfSetKey) \
	(somSelf,ev,k))
#ifndef SOMGD_somfSetKey
    #if (defined(_somfSetKey) || defined(__somfSetKey))
        #undef _somfSetKey
        #undef __somfSetKey
        #define SOMGD_somfSetKey 1
    #else
        #define _somfSetKey somf_TAssoc_somfSetKey
    #endif /* _somfSetKey */
#endif /* SOMGD_somfSetKey */

/*
 * New Method: somfSetValue
 */
typedef void   SOMLINK somTP_somf_TAssoc_somfSetValue(somf_TAssoc *somSelf, Environment *ev, 
		somf_MCollectible* v);
#pragma linkage(somTP_somf_TAssoc_somfSetValue, system)
typedef somTP_somf_TAssoc_somfSetValue *somTD_somf_TAssoc_somfSetValue;
/*
 *  Set the value (fValue) to the associated pair.
 */
#define somMD_somf_TAssoc_somfSetValue "::somf_TAssoc::somfSetValue"
#define somf_TAssoc_somfSetValue(somSelf,ev,v) \
    (SOM_Resolve(somSelf, somf_TAssoc, somfSetValue) \
	(somSelf,ev,v))
#ifndef SOMGD_somfSetValue
    #if (defined(_somfSetValue) || defined(__somfSetValue))
        #undef _somfSetValue
        #undef __somfSetValue
        #define SOMGD_somfSetValue 1
    #else
        #define _somfSetValue somf_TAssoc_somfSetValue
    #endif /* _somfSetValue */
#endif /* SOMGD_somfSetValue */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define somf_TAssoc_somfClone somf_MCollectible_somfClone
#define somf_TAssoc_somfClonePointer somf_MCollectible_somfClonePointer
#define somf_TAssoc_somfHash somf_MCollectible_somfHash
#define somf_TAssoc_somfIsEqual somf_MCollectible_somfIsEqual
#define somf_TAssoc_somfIsSame somf_MCollectible_somfIsSame
#define somf_TAssoc_somfIsNotEqual somf_MCollectible_somfIsNotEqual
#define somf_TAssoc_somDefaultInit SOMObject_somDefaultInit
#define somf_TAssoc_somDestruct SOMObject_somDestruct
#define somf_TAssoc_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define somf_TAssoc_somDefaultAssign SOMObject_somDefaultAssign
#define somf_TAssoc_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define somf_TAssoc_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define somf_TAssoc_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define somf_TAssoc_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define somf_TAssoc_somDefaultVAssign SOMObject_somDefaultVAssign
#define somf_TAssoc_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define somf_TAssoc_somFree SOMObject_somFree
#define somf_TAssoc_somGetClass SOMObject_somGetClass
#define somf_TAssoc_somGetClassName SOMObject_somGetClassName
#define somf_TAssoc_somGetSize SOMObject_somGetSize
#define somf_TAssoc_somIsA SOMObject_somIsA
#define somf_TAssoc_somIsInstanceOf SOMObject_somIsInstanceOf
#define somf_TAssoc_somRespondsTo SOMObject_somRespondsTo
#define somf_TAssoc_somDispatch SOMObject_somDispatch
#define somf_TAssoc_somClassDispatch SOMObject_somClassDispatch
#define somf_TAssoc_somCastObj SOMObject_somCastObj
#define somf_TAssoc_somResetObj SOMObject_somResetObj
#define somf_TAssoc_somDispatchV SOMObject_somDispatchV
#define somf_TAssoc_somDispatchL SOMObject_somDispatchL
#define somf_TAssoc_somDispatchA SOMObject_somDispatchA
#define somf_TAssoc_somDispatchD SOMObject_somDispatchD
#define somf_TAssoc_somPrintSelf SOMObject_somPrintSelf
#define somf_TAssoc_somDumpSelf SOMObject_somDumpSelf
#define somf_TAssoc_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_somf_TAssoc_h */
