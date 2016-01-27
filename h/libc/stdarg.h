#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __stdarg_h
   #define __stdarg_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   /********************************************************************/
   /*  <stdarg.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if __THW_PPC__

      /* WPOS ABI compliant definition */
      #if __IBMC__ || __IBMCPP__

         #ifndef __def_va_list
         #define __def_va_list
         typedef struct __va_list_struct {
            char gpr;
            char fpr;
            char reserved[2];
            char *input_arg_area;
            char *reg_save_area;
         } __va_list[1];
         #endif

         extern void _Builtin __vastart(void *);
         extern void* __va_arg(void*, int);

         #define __va_start(ap,fmt) __vastart(&ap)
         #define __va_arg(ap,t)  (*((t*)__va_arg(ap,__builtin_va_arg_incr((t *)0))))
         #define __va_end(ap)

         #define va_start __va_start
         #define va_arg   __va_arg
         #define va_end   __va_end

         typedef __va_list va_list;
      #endif

   #else /* __THW_INTEL__ */

      typedef char *va_list;

      #define __nextword(base)   (((unsigned)(sizeof(base))+3U)&~(3U))

      #ifdef __cplusplus
         #define va_start(ap, last) ap = __vastart(last);
      #else
         #define va_start(ap, last) ap = ((va_list)&last) + __nextword(last)
      #endif
      #define va_arg(ap, type) ((type *) ((ap += (int) __nextword(type)) - __nextword(type)))[0]
      #define va_end(ap)       ap = 0
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


