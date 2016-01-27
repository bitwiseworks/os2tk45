
/*
 * This file was generated by the SOM Compiler.
 * FileName: sinkev.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.17
 *     SOM Emitter emith: 2.47
 */

/*
 *    COMPONENT_NAME: somu
 * 
 *    ORIGINS: 27
 * 
 * 
 *    10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
 *    All Rights Reserved
 *    Licensed Materials - Property of IBM
 *    US Government Users Restricted Rights - Use, duplication or
 *    disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 * 
 */


#ifndef SOM_SOMESinkEvent_h
#define SOM_SOMESinkEvent_h


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
#ifndef SOMESinkEvent
#define SOMESinkEvent SOMObject
#endif
/*
 *  SOMESinkEvent is the class for generic sink events within the Event Manager.
 *  This class should be queried for events of the following type(s):
 *       EMSinkEvent (for files, pipes, and sockets)
 *       EMMsgQEvent (for AIX message queues)
 * 
 *  On AIX, an instance of this class is created, initialized and passed to the
 *  callback routine  for events (input/output/exception) associated with files,
 *  sockets, pipes and Message Queues. On OS/2, the same is done for sockets only.
 */
#include <event.h>

/*
 * Start of bindings for IDL types
 */

#ifndef SOMClass
    #define SOMClass SOMObject
#endif /* SOMClass */

/*
 * End of bindings for IDL types.
 */

#define SOMESinkEvent_MajorVersion 2
#define SOMESinkEvent_MinorVersion 1

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define SOMESinkEventNewClass c
#pragma linkage(c, system)
#define SOMESinkEventClassData d
#define SOMESinkEventCClassData x
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define SOMESinkEvent_classObj SOMESinkEventClassData.classObject
#define _SOMESinkEvent SOMESinkEvent_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK SOMESinkEventNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(SOMESinkEventNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct SOMESinkEventClassDataStructure {
	SOMClass *classObject;
	somMToken somevGetEventSink;
	somMToken somevSetEventSink;
} SOMDLINK SOMESinkEventClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct SOMESinkEventCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK SOMESinkEventCClassData;

/*
 * New and Renew macros for SOMESinkEvent
 */
#define SOMESinkEventNew() \
   ( _SOMESinkEvent ? \
	_somNew(_SOMESinkEvent) \
	: ( SOMESinkEventNewClass(\
		SOMESinkEvent_MajorVersion, \
		SOMESinkEvent_MinorVersion),\
	   _somNew(_SOMESinkEvent)))
#define SOMESinkEventRenew(buf) \
   ( _SOMESinkEvent ? \
	_somRenew(_SOMESinkEvent, buf) \
	: ( SOMESinkEventNewClass(\
		SOMESinkEvent_MajorVersion, \
		SOMESinkEvent_MinorVersion),\
	   _somRenew(_SOMESinkEvent, buf)))

/*
 * Override method: somInit
 */
#define SOMESinkEvent_somInit(somSelf) \
	SOMObject_somInit(somSelf)

/*
 * New Method: somevGetEventSink
 */
typedef long   SOMLINK somTP_SOMESinkEvent_somevGetEventSink(SOMESinkEvent *somSelf, Environment *ev);
#pragma linkage(somTP_SOMESinkEvent_somevGetEventSink, system)
typedef somTP_SOMESinkEvent_somevGetEventSink *somTD_SOMESinkEvent_somevGetEventSink;
/*
 *  Returns the sink, or source of file i/o, of the generic sink event.
 *  For message queues it is the queue id, for files it is the file descriptor,
 *  for sockets it is the socket id and for pipes it is the pipe descriptor.
 */
#define somMD_SOMESinkEvent_somevGetEventSink "::SOMESinkEvent::somevGetEventSink"
#define SOMESinkEvent_somevGetEventSink(somSelf,ev) \
    (SOM_Resolve(somSelf, SOMESinkEvent, somevGetEventSink) \
	(somSelf,ev))
#ifndef SOMGD_somevGetEventSink
    #if (defined(_somevGetEventSink) || defined(__somevGetEventSink))
        #undef _somevGetEventSink
        #undef __somevGetEventSink
        #define SOMGD_somevGetEventSink 1
    #else
        #define _somevGetEventSink SOMESinkEvent_somevGetEventSink
    #endif /* _somevGetEventSink */
#endif /* SOMGD_somevGetEventSink */

/*
 * New Method: somevSetEventSink
 */
typedef void   SOMLINK somTP_SOMESinkEvent_somevSetEventSink(SOMESinkEvent *somSelf, Environment *ev, 
		long sink);
#pragma linkage(somTP_SOMESinkEvent_somevSetEventSink, system)
typedef somTP_SOMESinkEvent_somevSetEventSink *somTD_SOMESinkEvent_somevSetEventSink;
/*
 *  Sets the sink, or source of file i/o, of the generic sink event.
 *  For message queues it is the queue id, for files it is the file descriptor,
 *  for sockets it is the socket id and for pipes it is the pipe descriptor.
 */
#define somMD_SOMESinkEvent_somevSetEventSink "::SOMESinkEvent::somevSetEventSink"
#define SOMESinkEvent_somevSetEventSink(somSelf,ev,sink) \
    (SOM_Resolve(somSelf, SOMESinkEvent, somevSetEventSink) \
	(somSelf,ev,sink))
#ifndef SOMGD_somevSetEventSink
    #if (defined(_somevSetEventSink) || defined(__somevSetEventSink))
        #undef _somevSetEventSink
        #undef __somevSetEventSink
        #define SOMGD_somevSetEventSink 1
    #else
        #define _somevSetEventSink SOMESinkEvent_somevSetEventSink
    #endif /* _somevSetEventSink */
#endif /* SOMGD_somevSetEventSink */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define SOMESinkEvent_somevGetEventTime SOMEEvent_somevGetEventTime
#define SOMESinkEvent_somevGetEventType SOMEEvent_somevGetEventType
#define SOMESinkEvent_somevSetEventTime SOMEEvent_somevSetEventTime
#define SOMESinkEvent_somevSetEventType SOMEEvent_somevSetEventType
#define SOMESinkEvent_somDefaultInit SOMObject_somDefaultInit
#define SOMESinkEvent_somDestruct SOMObject_somDestruct
#define SOMESinkEvent_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define SOMESinkEvent_somDefaultAssign SOMObject_somDefaultAssign
#define SOMESinkEvent_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define SOMESinkEvent_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define SOMESinkEvent_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define SOMESinkEvent_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define SOMESinkEvent_somDefaultVAssign SOMObject_somDefaultVAssign
#define SOMESinkEvent_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define SOMESinkEvent_somFree SOMObject_somFree
#define SOMESinkEvent_somUninit SOMObject_somUninit
#define SOMESinkEvent_somGetClass SOMObject_somGetClass
#define SOMESinkEvent_somGetClassName SOMObject_somGetClassName
#define SOMESinkEvent_somGetSize SOMObject_somGetSize
#define SOMESinkEvent_somIsA SOMObject_somIsA
#define SOMESinkEvent_somIsInstanceOf SOMObject_somIsInstanceOf
#define SOMESinkEvent_somRespondsTo SOMObject_somRespondsTo
#define SOMESinkEvent_somDispatch SOMObject_somDispatch
#define SOMESinkEvent_somClassDispatch SOMObject_somClassDispatch
#define SOMESinkEvent_somCastObj SOMObject_somCastObj
#define SOMESinkEvent_somResetObj SOMObject_somResetObj
#define SOMESinkEvent_somDispatchV SOMObject_somDispatchV
#define SOMESinkEvent_somDispatchL SOMObject_somDispatchL
#define SOMESinkEvent_somDispatchA SOMObject_somDispatchA
#define SOMESinkEvent_somDispatchD SOMObject_somDispatchD
#define SOMESinkEvent_somPrintSelf SOMObject_somPrintSelf
#define SOMESinkEvent_somDumpSelf SOMObject_somDumpSelf
#define SOMESinkEvent_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_SOMESinkEvent_h */