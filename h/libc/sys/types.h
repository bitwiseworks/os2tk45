#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __types_h
   #define __types_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   /********************************************************************/
   /*  <sys\types.h> header file                                       */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef __ANSI__

      #ifndef __SAA_L2__

         #if defined(__EXTENDED__)

            #ifndef __ssize_t
               typedef signed int ssize_t;  
               #define __ssize_t
            #endif

            #ifndef __ino_t
               typedef unsigned short ino_t;
               #define __ino_t
            #endif

            #ifndef __time_t
               #define __time_t
               typedef long time_t;
            #endif

            #ifndef __dev_t
               typedef short dev_t;
               #define __dev_t
            #endif

            #ifndef __off_t
               typedef long off_t;
               #define __off_t
            #endif

         #endif

      #endif

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

