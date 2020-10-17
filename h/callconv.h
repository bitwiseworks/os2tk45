/*
 * COMPONENT_NAME:
 *
 * FUNCTIONS:  Defines items for use with UniQueryLocaleItem - public
 *
 * ORIGINS: 27
 *
 * (C) COPYRIGHT International Business Machines Corp. 1989, 1992
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

#ifndef _ULS_CALLCONV_
#define _ULS_CALLCONV_

#ifdef __cplusplus
   extern "C" {
#endif

#if defined(__IBMCPP__) || defined(__IBMC__) || defined(__WATCOMC__)
   #define CALLCONV    _System
#else
   #define CALLCONV
#endif

#ifdef __cplusplus
   }
#endif

#endif
