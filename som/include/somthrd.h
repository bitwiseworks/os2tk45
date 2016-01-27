/* @(#) 1.9 src/somu/somthrd.h, somu, som2.1 12/27/95 13:58:43 [7/30/96 15:26:34] */
/*
 *   COMPONENT_NAME: somd
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


/*
 *  somthrd.h - include file for Thread Service Provider Interfaces (SPIs)
**	10/26/95	cgt	DCR 15152- Additional thread SPIs.
*/

#ifndef SOM_somthrd_h
#define SOM_somthrd_h

#include <som.h>

#ifdef __IBMC__
 #pragma linkage(somTD_SOMStartThread     , system)
 #pragma linkage(somTD_SOMEndThread       , system)
 #pragma linkage(somTD_SOMKillThread      , system)
 #pragma linkage(somTD_SOMYieldThread     , system)
 #pragma linkage(somTD_SOMGetThreadHandle , system)
 #pragma linkage(somTD_SOMBeginThread     , system)
 #pragma linkage(somTD_SOMBeginProc       , system)

/* DCR 15152 additions follow */

 #pragma linkage(somTD_SOMThreadJoin, system)
 #pragma linkage(somTD_SOMThreadJoin , system )
 #pragma linkage(somTD_SOMGetMutexOwner , system )
 #pragma linkage(somTD_SOMCreateEventSem , system )
 #pragma linkage(somTD_SOMDestroyEventSem, system )
 #pragma linkage(somTD_SOMPostEventSem   , system )
 #pragma linkage(somTD_SOMWaitEventSem   , system )
 #pragma linkage(somTD_SOMCreateSharedEventSem   , system )
 #pragma linkage(somTD_SOMOpenSharedEventSem     , system )
 #pragma linkage(somTD_SOMCloseSharedEventSem, system )
 #pragma linkage(somTD_SOMDestroySharedEventSem  , system )
 #pragma linkage(somTD_SOMPostSharedEventSem  , system )
 #pragma linkage(somTD_SOMWaitSharedEventSem  , system )
 #pragma linkage(somTD_SOMCreateSharedMutexSem, system )
 #pragma linkage(somTD_SOMOpenSharedMutexSem  , system )
 #pragma linkage(somTD_SOMCloseSharedMutexSem    , system )
 #pragma linkage(somTD_SOMDestroySharedMutexSem  , system )
 #pragma linkage(somTD_SOMRequestSharedMutexSem, system )
 #pragma linkage(somTD_SOMReleaseSharedMutexSem, system )
 #pragma linkage(somTD_SOMThreadDetach, system )
/* End of 15152 */


#endif


#ifdef _AIX41   /* Defect 18173 */

 /* Pthreads allows thread functions to return status */
 typedef void *SOMLINK 	somTD_SOMBeginProc(void *data);
 typedef void *SOMLINK	somTD_SOMThreadProc(void * data);

#else

 /* SOMBeginThread supports thread procedure with system linkage */
 typedef void SOMLINK 	somTD_SOMBeginProc(void *data);
 typedef void SOMLINK	somTD_SOMThreadProc(void * data);

#endif

/* note: for OS/2 CSet++, somTD_SOMThreadProc must use _Optlink linkage */

typedef unsigned long SOMLINK somTD_SOMStartThread(OUT somToken * thrd,
						   IN somTD_SOMThreadProc proc,
						   IN void * data, 
						   IN unsigned long datasz,
						   IN unsigned long stacksz);

typedef unsigned long SOMLINK somTD_SOMBeginThread(OUT somToken * thrd,
					   IN somTD_SOMBeginProc proc,
					   IN void * data, 
					   IN unsigned long datasz,
					   IN unsigned long stacksz);

typedef unsigned long SOMLINK somTD_SOMEndThread(void);
typedef unsigned long SOMLINK somTD_SOMKillThread(IN somToken thrd);
typedef unsigned long SOMLINK somTD_SOMYieldThread(void);
typedef unsigned long SOMLINK somTD_SOMGetThreadHandle(OUT somToken * thrd);


/* DCR 15152 additions follow */
typedef unsigned long SOMLINK somTD_SOMThreadJoin (IN somToken tid, unsigned long *status);
typedef unsigned long SOMLINK somTD_SOMGetMutexOwner (IN somToken sem);

typedef unsigned long SOMLINK somTD_SOMCreateEventSem (OUT somToken *sem);
typedef unsigned long SOMLINK somTD_SOMDestroyEventSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMPostEventSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMWaitEventSem (IN somToken sem);

typedef unsigned long SOMLINK somTD_SOMCreateSharedEventSem (OUT somToken *sem,void *name);
typedef unsigned long SOMLINK somTD_SOMOpenSharedEventSem (OUT somToken *sem, void *name);
typedef unsigned long SOMLINK somTD_SOMCloseSharedEventSem (IN somToken sem );
typedef unsigned long SOMLINK somTD_SOMDestroySharedEventSem (IN somToken sem );
typedef unsigned long SOMLINK somTD_SOMWaitSharedEventSem (IN somToken sem );
typedef unsigned long SOMLINK somTD_SOMPostSharedEventSem (IN somToken sem );

typedef unsigned long SOMLINK somTD_SOMCreateSharedMutexSem (OUT somToken *sem,void *name);
typedef unsigned long SOMLINK somTD_SOMOpenSharedMutexSem (OUT somToken *sem,void *name);
typedef unsigned long SOMLINK somTD_SOMCloseSharedMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMDestroySharedMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMRequestSharedMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMReleaseSharedMutexSem (IN somToken sem);
typedef unsigned long SOMLINK somTD_SOMThreadDetach (IN somToken thread);

/* End of 15152 */

/* pointers to thread service functions */

SOMEXTERN somTD_SOMStartThread     	* SOMDLINK SOMStartThread;
SOMEXTERN somTD_SOMBeginThread     	* SOMDLINK SOMBeginThread;
SOMEXTERN somTD_SOMEndThread       	* SOMDLINK SOMEndThread;
SOMEXTERN somTD_SOMKillThread      	* SOMDLINK SOMKillThread;
SOMEXTERN somTD_SOMYieldThread     	* SOMDLINK SOMYieldThread;
SOMEXTERN somTD_SOMGetThreadHandle 	* SOMDLINK SOMGetThreadHandle;



/* DCR 15152 additions follow */

SOMEXTERN somTD_SOMThreadJoin                   * SOMDLINK SOMThreadJoin;
SOMEXTERN somTD_SOMGetMutexOwner                * SOMDLINK SOMGetMutexOwner;

SOMEXTERN somTD_SOMCreateEventSem               * SOMDLINK SOMCreateEventSem;
SOMEXTERN somTD_SOMDestroyEventSem              * SOMDLINK SOMDestroyEventSem ;
SOMEXTERN somTD_SOMPostEventSem                 * SOMDLINK SOMPostEventSem ;
SOMEXTERN somTD_SOMWaitEventSem                 * SOMDLINK SOMWaitEventSem;

SOMEXTERN somTD_SOMCreateSharedEventSem         * SOMDLINK SOMCreateSharedEventSem ;
SOMEXTERN somTD_SOMOpenSharedEventSem           * SOMDLINK SOMOpenSharedEventSem ;
SOMEXTERN somTD_SOMCloseSharedEventSem          * SOMDLINK SOMCloseSharedEventSem ;
SOMEXTERN somTD_SOMDestroySharedEventSem        * SOMDLINK SOMDestroySharedEventSem;
SOMEXTERN somTD_SOMPostSharedEventSem           * SOMDLINK SOMPostSharedEventSem ;
SOMEXTERN somTD_SOMWaitSharedEventSem           * SOMDLINK SOMWaitSharedEventSem;

SOMEXTERN somTD_SOMCreateSharedMutexSem         * SOMDLINK SOMCreateSharedMutexSem;
SOMEXTERN somTD_SOMOpenSharedMutexSem           * SOMDLINK SOMOpenSharedMutexSem ;
SOMEXTERN somTD_SOMCloseSharedMutexSem          * SOMDLINK SOMCloseSharedMutexSem;
SOMEXTERN somTD_SOMDestroySharedMutexSem        * SOMDLINK SOMDestroySharedMutexSem;
SOMEXTERN somTD_SOMRequestSharedMutexSem        * SOMDLINK SOMRequestSharedMutexSem;
SOMEXTERN somTD_SOMReleaseSharedMutexSem        * SOMDLINK SOMReleaseSharedMutexSem;
SOMEXTERN somTD_SOMThreadDetach                 * SOMDLINK SOMThreadDetach;


/* End of 15152 */


#endif /* SOM_somthrd_h */
