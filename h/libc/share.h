#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __share_h
   #define __share_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   /********************************************************************/
   /*  <share.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if (defined( __SPC__ ) || defined(__EXTENDED__))

      /*  This file defines the file sharing modes for _sopen().       */

      #define SH_DENYRW       0x10    /* deny read/write mode */
      #define SH_DENYWR       0x20    /* deny write mode */
      #define SH_DENYRD       0x30    /* deny read mode */
      #define SH_DENYNO       0x40    /* deny none mode */

   #endif

   #ifdef __cplusplus
      }
   #endif

#endif

#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( restore )
#endif
#pragma info( restore )
#endif

