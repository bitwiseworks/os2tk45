/*
 *   COMPONENT_NAME: somp
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
/* @(#) 2.10 src/somp/timer.h, somp, som2.1 12/26/95 17:03:33 [7/30/96 14:47:07] */

/*
 */

/* File: timer.h */

#ifndef TIMER_H
#define TIMER_H 1

#include <som.h>

#if (defined __OS2__) || (defined _WIN16)
  #define INCL_DOSDATETIME
  #ifdef _WIN16
     #include <dos.h>
     #include <stdio.h>
     #include <string.h>
  #else
     #include <os2.h>
  #endif

  /* OS/2 doesn't have the same "gettimeofday" function that AIX has. */
  /* It is simulated (in timer.c) by a call to the OS.                */

  struct timeval {
        int             tv_sec;         /* seconds */
        int             tv_usec;        /* microseconds */
  };

  struct timezone {
        int             tz_minuteswest; /* minutes west of Greenwich */
        int             tz_dsttime;     /* type of dst correction */
  };

  int sompfGetTimeOfDay(struct timeval *tv, struct timezone *tz);
  #define GETTIMEOFDAY sompfGetTimeOfDay
#else
  #include <sys/time.h>
  #define GETTIMEOFDAY gettimeofday
#endif

#define SOMPC_TIMER_NAME_LENGTH 30
struct SOMPTimer_type {
  struct timeval startTime;
  struct timeval endTime;
  long int elapsedMicroSec;
  long int timesTurnedOn;
  long int timesTurnedOff;
  char timerName[SOMPC_TIMER_NAME_LENGTH];
};
typedef struct SOMPTimer_type SOMPTimer;

void SOMLINK sompfInitTimers(void);
SOMPTimer * SOMLINK sompfGetTimer(char *name);
void SOMLINK sompfPrintTimer(SOMPTimer *thistimer);
void SOMLINK sompfPrintTimers(void);

long int SOMLINK sompfGetElapsedMicroSec(SOMPTimer *thisTimer);
long int SOMLINK sompfGetTimesTurnedOn(SOMPTimer *thisTimer);
long int SOMLINK sompfGetTimesTurnedOff(SOMPTimer *thisTimer);
char * SOMLINK sompfGetTimerName(SOMPTimer *thisTimer);

/* #define TIMER_ON */

#ifdef TIMER_ON

#define INITTIMERS  sompfInitTimers();
#define DCLTIMER(t) SOMPTimer *t = (SOMPTimer *)0;
#define DCLSTATICTIMER(t) static SOMPTimer *t = (SOMPTimer *)0;
#define GETTIMER(t,s) if (!t) t = sompfGetTimer(s);
#define PRINTTIMERS sompfPrintTimers();

#define STARTTIMER(t) \
{ \
  t->timesTurnedOn++; \
  GETTIMEOFDAY(&(t->startTime), &sompgDummyTimeZone); \
}

#define sompfStartTimer(thisTimer) \
{ \
  thisTimer->timesTurnedOn++; \
  GETTIMEOFDAY(&(thisTimer->startTime), &sompgDummyTimeZone); \
}

#define STOPTIMER(t) \
{ \
  GETTIMEOFDAY(&(t->endTime), &sompgDummyTimeZone); \
  t->elapsedMicroSec += \
      ((long int) (t->endTime.tv_sec - t->startTime.tv_sec)) * 1000000; \
  if (t->endTime.tv_usec > t->startTime.tv_usec) \
       t->elapsedMicroSec += t->endTime.tv_usec - t->startTime.tv_usec; \
  else t->elapsedMicroSec += \
       (1000000 - t->startTime.tv_usec) + t->endTime.tv_usec - 1000000; \
  t->timesTurnedOff++; \
}

#define sompfStopTimer(thisTimer) \
{ \
  GETTIMEOFDAY(&(thisTimer->endTime), &sompgDummyTimeZone); \
  thisTimer->elapsedMicroSec += \
      ((long int) (thisTimer->endTime.tv_sec - thisTimer->startTime.tv_sec)) * 1000000; \
  if (thisTimer->endTime.tv_usec > thisTimer->startTime.tv_usec) \
       thisTimer->elapsedMicroSec += thisTimer->endTime.tv_usec - thisTimer->startTime.tv_usec; \
  else thisTimer->elapsedMicroSec += \
       (1000000 - thisTimer->startTime.tv_usec) + thisTimer->endTime.tv_usec - 1000000; \
  thisTimer->timesTurnedOff++; \
}

#ifdef _WIN16
  #define sompgDummyTimeZone (*sompgDummyTimeZoneResolve())
  SOMEXTERN struct timezone * SOMLINK  sompgDummyTimeZoneResolve(void);
#else
  SOMEXTERN struct timezone SOMDLINK sompgDummyTimeZone;
#endif

#else
#define INITTIMERS
#define DCLTIMER(t)
#define DCLSTATICTIMER(t)
#define GETTIMER(t,s)
#define STARTTIMER(t)
#define STOPTIMER(t)
#define PRINTTIMERS

#define sompfStartTimer(thisTimer) {}
#define sompfStopTimer(thisTimer) {}
#endif

#endif

