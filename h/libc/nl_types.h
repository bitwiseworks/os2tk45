#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __nl_types_h
   #define __nl_types_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   #ifndef  _LNK_CONV
      #if __IBMC__ || __IBMCPP__ || defined(_OPTLINK_SUPPORTED)
         #define _LNK_CONV   _Optlink
      #else
         #define _LNK_CONV
      #endif
   #endif

   #ifndef _IMPORT
      #ifdef __IMPORTLIB__
         #define _IMPORT _Import
      #else
         #define _IMPORT
      #endif
   #endif

   /********************************************************************/
   /*  <nl_types.h> header file                                        */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   struct _catalog_descriptor {
           char            *_mem;
           char            *_name;
           struct __file   *_fd;
           struct _header  *_hd;
           struct _catset  *_set;
           int             _setmax;
           int             _count;
           int             _pid;
           int             _oflag;
   };

   typedef int nl_item;

   typedef struct _catalog_descriptor *nl_catd;

   #define NL_SETD         1
   #define NL_CAT_LOCALE   1

   #define CAT_MAGIC       505
   #define CATD_ERR        ((nl_catd) -1)
   #define NL_MAXOPEN      10

   int _IMPORT _LNK_CONV catclose(nl_catd catd);
   char * _IMPORT _LNK_CONV catgets(nl_catd catd, int set_id, 
                            int msg_id, const char *s);
   nl_catd _IMPORT _LNK_CONV catopen(const char *name, int oflag);

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

