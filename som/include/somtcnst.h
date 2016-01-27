/*
 *   COMPONENT_NAME: somi
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.10 src/somc/tc/somtcnst.h, somtc.const, som2.1 3/12/96 17:55:37 [7/30/96 14:45:07] */

/*
 */

/*
 * SOMTCNST.H
 * TypeCode constants as defined in CORBA for use with SOM
 */

#ifndef somtcnst_h
#define somtcnst_h

#ifdef somtcnst_xh
  #include <som.xh>
#else
  #include <som.h>
#endif /* somtcnst_xh */

/* CORBA 7.6.2, p.141 */
#define TC_null 		    ((TypeCode) &TC__null)
#define TC_void 		    ((TypeCode) &TC__void)
#define TC_short		    ((TypeCode) &TC__short)
#define TC_long 		    ((TypeCode) &TC__long)
#define TC_ushort		    ((TypeCode) &TC__ushort)
#define TC_ulong		    ((TypeCode) &TC__ulong)
#define TC_float		    ((TypeCode) &TC__float)
#define TC_double		    ((TypeCode) &TC__double)
#define TC_boolean		    ((TypeCode) &TC__boolean)
#define TC_char 		    ((TypeCode) &TC__char)
#define TC_octet		    ((TypeCode) &TC__octet)
#define TC_any			    ((TypeCode) &TC__any)
#define TC_TypeCode		    ((TypeCode) &TC__TypeCode)
#define TC_Principal		    ((TypeCode) &TC__Principal)
#define TC_Object		    ((TypeCode) &TC__Object)
#define TC_string		    ((TypeCode) &TC__string)
#define TC_NamedValue		    ((TypeCode) &TC__NamedValue)
#define TC_InterfaceDescription     ((TypeCode) &TC__InterfaceDescription)
#define TC_OperationDescription     ((TypeCode) &TC__OperationDescription)
#define TC_AttributeDescription     ((TypeCode) &TC__AttributeDescription)
#define TC_ParameterDescription     ((TypeCode) &TC__ParameterDescription)
#define TC_RepositoryDescription    ((TypeCode) &TC__RepositoryDescription)
#define TC_ModuleDescription	    ((TypeCode) &TC__ModuleDescription)
#define TC_ConstDescription	    ((TypeCode) &TC__ConstDescription)
#define TC_ConstantDescription	    ((TypeCode) &TC__ConstDescription)
#define TC_ExceptionDescription     ((TypeCode) &TC__ExceptionDescription)
#define TC_TypeDescription	    ((TypeCode) &TC__TypeDescription)
#define TC_FullInterfaceDescription ((TypeCode) &TC__FullInterfaceDescription)

/* To compile and link somtc.c into a DLL, or an exe, use the 
following flag: -DSOM_DONTIMPORT_SOMTC. To import from a DLL that
already contains somtc.o, do not use the above flag.
*/
#if !defined(_WIN32) || defined(SOM_DONTIMPORT_SOMTC)
#define somtcImportExport
#else
#define somtcImportExport SOMDLLIMPORT
#endif

SOMEXTERN somtcImportExport somToken SOMDLINK TC__null;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__void;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__short;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__long;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ushort;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ulong;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__float;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__double;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__boolean;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__char;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__octet;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__any;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__TypeCode;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__Principal;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__Object;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__string;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__NamedValue;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__InterfaceDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__OperationDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__AttributeDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ParameterDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__RepositoryDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ModuleDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ConstDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__ExceptionDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__TypeDescription;
SOMEXTERN somtcImportExport somToken SOMDLINK TC__FullInterfaceDescription;

#endif /* somtcnst_h */
