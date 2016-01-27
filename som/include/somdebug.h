/*
 *   COMPONENT_NAME: somk
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
/* @(#) /16 1.6.1.2 12/26/95 15:57:49 [7/30/96 14:50:38] */

/*
 *
 */

/*
 *	SOMDEBUG.H
 *      SOM Kernel Debugging Hooks
 *
 *      We assume a debugger that operates in its own process and
 *      has access to the memory in another process that is being
 *      debugged.  
 *
 *      The SOM Kernel Debugging Hook functions described here use
 *      a debugger-supplied function (prototyped below as SOMTD_SOMPeek)
 *      to reach into the process being debugged and compute various
 *      results useful to debuggers.  No side-effect are experienced
 *      in the debugged process as a result of invoking these functions.
 *
 *      To install the debugger-supplied SOMTD_SOMPeek function,
 *      simply place its address in the SOMPeek external variable.
 *
 *      All addresses returned from the SOM Kernel Debugging Hook
 *      functions refer to memory in the debugged process, with
 *      ONE exception.  The sequence returned from 
 *      somDbgGetInstanceDataOffsets has its buffer allocated in
 *      the debugger's process (to avoid side-effect in the debugged
 *      process).  The space for this buffer is obtained via "malloc".
 *      The debugger must free the buffer when finished with it, using
 *      code similar to the following:
 *
 *          SOMClass_somOffsets result;
 *          result = somDbgGetInstanceDataOffsets (cls);
 *          ...
 *          if (result._length)
 *              free (result._buffer);
 */

#ifndef somdebug_h
#define somdebug_h

#include <som.h>

/*
 * somDbgIdSequence type -- used as a return type for some Dbg functions.
 */
typedef struct {
    long   _maximum;
    long   _length;
    char **_buffer;
} somDbgIdSequence;

#if defined(__IBMC__) && defined(__OS2__)
  #pragma linkage(SOMTD_SOMPeek, system)
  #pragma linkage(somDbgDataResolve, system)
  #pragma linkage(somDbgGetAllMethodDescriptors, system)
  #pragma linkage(somDbgGetClass, system)
  #pragma linkage(somDbgGetClassName, system)
  #pragma linkage(somDbgGetInstanceDataOffsets, system)
  #pragma linkage(somDbgGetInstanceSize, system)
  #pragma linkage(somDbgGetIntroducedMethodDescriptors, system)
  #pragma linkage(somDbgGetName, system)
  #pragma linkage(somDbgGetParents, system)
  #pragma linkage(somDbgGetVersionNumbers, system)
  #pragma linkage(somDbgIsObj, system)
  #pragma linkage(somDbgProtectedDataOffset, system)
  #pragma linkage(somDbgResolveMethod, system)
#endif

/*
 *	The following debugger-supplied function extracts memory
 *      from the debugged process begining at the address specified
 *      by "startAddr" in the debugged process and copies it into the 
 *      address specified by "targetAddr" in the current process until
 *      the number of bytes specified by length have been transferred.
 *      If everything completes successfully the function must return
 *      the value TRUE, otherwise FALSE must be returned.
 */
typedef long SOMLINK SOMTD_SOMPeek (void *startAddr, long length, 
				    void *targetAddr);
SOMEXTERN SOMTD_SOMPeek *SOMPeek;

/*
 * somDbg functions
 */

SOMEXTERN void * SOMLINK
    somDbgDataResolve(SOMObject *obj, somDToken dtoken);

SOMEXTERN somDbgIdSequence SOMLINK
    somDbgGetAllMethodDescriptors(SOMClass *cls);

SOMEXTERN SOMClass * SOMLINK
    somDbgGetClass (SOMObject *obj);

SOMEXTERN string SOMLINK
    somDbgGetClassName (SOMObject *obj);

SOMEXTERN SOMClass_somOffsets SOMLINK
    somDbgGetInstanceDataOffsets(SOMClass *somSelf);

SOMEXTERN long SOMLINK
    somDbgGetInstanceSize(SOMClass *cls);

SOMEXTERN somDbgIdSequence SOMLINK
    somDbgGetIntroducedMethodDescriptors(SOMClass *cls);

SOMEXTERN string SOMLINK
    somDbgGetName (SOMClass *cls);

SOMEXTERN SOMClassSequence SOMLINK
    somDbgGetParents(SOMClass *cls);

SOMEXTERN void SOMLINK
    somDbgGetVersionNumbers(SOMClass *cls, long *major, long *minor);

SOMEXTERN boolean SOMLINK
    somDbgIsObj(SOMObject *obj);

SOMEXTERN unsigned long SOMLINK
    somDbgProtectedDataOffset(SOMClass *cls);

SOMEXTERN somMethodProc * SOMLINK
    somDbgResolveMethod (SOMObject *obj, somMToken mtoken);


#endif /* somdebug_h */
