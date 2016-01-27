#define INCL_DOSPROCESS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#include <os2.h>
#include <conio.h>
#include <stdio.h>

#include "tmr0_ioc.h"

#define NUM_THREADS     4

HFILE hfile;
volatile int fActive = TRUE;
volatile unsigned uNumThreads = 0;
ULONG *pulTimer;

ULONG ulCounter[NUM_THREADS];


VOID APIENTRY ThreadFunction(ULONG ulIndex)
{
   APIRET rc;
   ULONG ulDelay = (ulIndex + 1) * 500;
   ULONG ulSize = sizeof(ulDelay);

   rc = DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
   if (rc)
      return;

   uNumThreads++;    // This is an atomic operation.

   while (fActive) {
      rc = DosDevIOCtl(hfile, HRT_IOCTL_CATEGORY, HRT_BLOCKUNTIL, &ulDelay, ulSize, &ulSize, NULL, 0, NULL);
      if (rc)
         break;
      ulCounter[ulIndex]++;
   }

   uNumThreads--;    // This is an atomic operation.
}

int main(void)
{
   APIRET rc;
   TID tid;
   ULONG ulAction;
   ULONG ulOpenFlag = OPEN_ACTION_OPEN_IF_EXISTS;
   ULONG ulOpenMode = OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE;

   ULONG ulResolution = 1;
   ULONG ulSize1 = sizeof(ulResolution);

   ULONG * _Seg16 pulTimer16;                   // defines a 16:16 pointer
   ULONG ulSize2 = sizeof(pulTimer16);

   unsigned i;

   rc = DosOpen("TIMER0$  ", &hfile, &ulAction, 0, 0, ulOpenFlag, ulOpenMode, NULL);
   if (rc) {
      printf("Couldn't open TIMER0$.\n");
      return 1;
   }

   printf("TIMER0$ opened.  File Handle is %lu\n", hfile);

   rc = DosDevIOCtl(hfile, HRT_IOCTL_CATEGORY, HRT_SET_RESOLUTION,  &ulResolution, ulSize1, &ulSize1, NULL, 0, NULL);
   if (rc) {
      printf("Couldn't set resolution.\n");
      DosClose(hfile);
      return 2;
   }

   rc = DosDevIOCtl(hfile, HRT_IOCTL_CATEGORY, HRT_GET_POINTER, NULL, 0, NULL, (void *) &pulTimer16, ulSize2, &ulSize2);
   if (rc) {
      printf("Couldn't get pointer.\n");
      DosClose(hfile);
      return 3;
   }

   pulTimer = pulTimer16;    // converts a 16:16 pointer to a 0:32 pointer
   if (!pulTimer) {
      printf("NULL pointer.\n");
      DosClose(hfile);
      return 4;
   }

   for (i=0; i<NUM_THREADS; i++) {
      rc = DosCreateThread(&tid, ThreadFunction, i, 0, 8192);
      if (rc) {
         printf("Can't create thread %i, rc=%lu\n", i+1, rc);
         fActive = FALSE;
         while (uNumThreads);     // Wait until all threads have ended
         return 5;
      }
   }

   while (!kbhit()) {
      printf("Counter: %08lu  ", *pulTimer);
      for (i=0; i<NUM_THREADS; i++)
         printf("%i:%08lu ", i+1, ulCounter[i]);
      printf("\r");
      DosSleep(0);
   }
   fActive=FALSE;
   getch();
   while (uNumThreads);     // Wait until all threads have ended

   return 0;
}

