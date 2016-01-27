
#define INCL_BASE           // include BASE OS/2 defines
#define INCL_NOPMAPI        // leave out the PM stuff

#include <os2.h>
#include <os2medef.h>
#include <seekcalc.h>

/* Performance stub for pagetuned pragma header files */
#ifndef PERFSTUB
  #include "perfstub.h"
  #define  PERFSTUB
#endif


ULONG APIENTRY Bytes2Mmtime( ULONG   ulBytesPerUnit,
                             ULONG   lSeekPoint,
                             MMTIME  mmTimePerUnit,
                             PLONG   plByteSeekPoint )
{
  ULONG   ulTempHigh, ulTempLow, roundup=0, rc=0, negative=0;

  if (lSeekPoint & 0x80000000)
   {
     lSeekPoint--;
     lSeekPoint = ~lSeekPoint;
     negative = 1;
   }
  ulTempHigh = lSeekPoint & 0xFFFF0000;
  ulTempHigh = ulTempHigh >> 15;       // We will divide by 2 later

  ulTempLow = lSeekPoint & 0x0000FFFF;
  ulTempLow = ulTempLow * 2;           // We will divide by 2 later

  ulTempHigh = ulTempHigh * (ULONG)mmTimePerUnit;
  if (ulTempHigh & 0xFFFF0000) rc=1;

  ulTempHigh = ulTempHigh << 16;
  ulTempHigh = ulTempHigh / ulBytesPerUnit;

  ulTempLow = ulTempLow * (ULONG)mmTimePerUnit;
  ulTempLow = ulTempLow / ulBytesPerUnit;

  *plByteSeekPoint =  ulTempHigh + ulTempLow;

  if (*plByteSeekPoint & 0x00000001) roundup=1;

  *plByteSeekPoint = (*plByteSeekPoint/2) + roundup;

  if (negative)
   {
     *plByteSeekPoint = ~*plByteSeekPoint;
     (*plByteSeekPoint)++;
   }

  return(rc);
}



ULONG APIENTRY Mmtime2Bytes( ULONG   ulBytesPerUnit,
                             ULONG   lSeekPoint,
                             MMTIME  mmTimePerUnit,
                             PLONG   plByteSeekPoint )
{
  ULONG   ulTempHigh, ulTempLow, roundup=0, rc=0, negative=0;

  if (lSeekPoint & 0x80000000)
   {
     lSeekPoint--;
     lSeekPoint = ~lSeekPoint;
     negative = 1;
   }
  ulTempHigh = lSeekPoint & 0xFFFF0000;
  ulTempHigh = ulTempHigh >> 15;       // We will divide by 2 later

  ulTempLow = lSeekPoint & 0x0000FFFF;
  ulTempLow = ulTempLow * 2;           // We will divide by 2 later

  ulTempHigh = ulTempHigh * ulBytesPerUnit;
  if (ulTempHigh & 0xFFFF0000) rc=1;

  ulTempHigh = ulTempHigh << 16;
  ulTempHigh = ulTempHigh / mmTimePerUnit;

  ulTempLow = ulTempLow * ulBytesPerUnit;
  ulTempLow = ulTempLow / mmTimePerUnit;

  *plByteSeekPoint =  ulTempHigh + ulTempLow;

  if (*plByteSeekPoint & 0x00000001) roundup=1;

  *plByteSeekPoint = (*plByteSeekPoint/2) + roundup;

  if (negative)
   {
     *plByteSeekPoint = ~*plByteSeekPoint;
     (*plByteSeekPoint)++;
   }

  return(rc);
}


ULONG APIENTRY AddNCheck(ULONG ulAdd, ULONG ulSeekPoint, PLONG plSeekPoint)
{
   ULONG ulTempHigh, ulTempLow, carry=0;

   ulTempHigh = ulAdd & 0xFFFF0000;
   ulTempHigh = ulTempHigh >> 16;

   ulTempLow  = ulAdd & 0x0000FFFF;

   if ( (ulTempLow + (ulSeekPoint & 0x0000FFFF)) & 0xFFFF0000) carry=1;

   if ( (ulTempHigh + ((ulSeekPoint & 0xFFFF0000)>>16) + carry) & 0xFFFF0000) carry=1;
   else carry = 0;

   *plSeekPoint=ulAdd + ulSeekPoint;

   return(carry);
}
