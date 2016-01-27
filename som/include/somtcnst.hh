/*
 *   COMPONENT_NAME: somc
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */


/*
 * SOMTCNST.HH : DTS C++ include
 * TypeCode constants as defined in CORBA for use with SOM
 */

#ifndef somtcnst_hh_DTS_INCLUDE
#define somtcnst_hh_DTS_INCLUDE

#include <som.hh>

#pragma SOMAsDefault (off)

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

SOMEXTERN somToken SOMDLINK TC__null;
SOMEXTERN somToken SOMDLINK TC__void;
SOMEXTERN somToken SOMDLINK TC__short;
SOMEXTERN somToken SOMDLINK TC__long;
SOMEXTERN somToken SOMDLINK TC__ushort;
SOMEXTERN somToken SOMDLINK TC__ulong;
SOMEXTERN somToken SOMDLINK TC__float;
SOMEXTERN somToken SOMDLINK TC__double;
SOMEXTERN somToken SOMDLINK TC__boolean;
SOMEXTERN somToken SOMDLINK TC__char;
SOMEXTERN somToken SOMDLINK TC__octet;
SOMEXTERN somToken SOMDLINK TC__any;
SOMEXTERN somToken SOMDLINK TC__TypeCode;
SOMEXTERN somToken SOMDLINK TC__Principal;
SOMEXTERN somToken SOMDLINK TC__Object;
SOMEXTERN somToken SOMDLINK TC__string;
SOMEXTERN somToken SOMDLINK TC__NamedValue;
SOMEXTERN somToken SOMDLINK TC__InterfaceDescription;
SOMEXTERN somToken SOMDLINK TC__OperationDescription;
SOMEXTERN somToken SOMDLINK TC__AttributeDescription;
SOMEXTERN somToken SOMDLINK TC__ParameterDescription;
SOMEXTERN somToken SOMDLINK TC__RepositoryDescription;
SOMEXTERN somToken SOMDLINK TC__ModuleDescription;
SOMEXTERN somToken SOMDLINK TC__ConstDescription;
SOMEXTERN somToken SOMDLINK TC__ExceptionDescription;
SOMEXTERN somToken SOMDLINK TC__TypeDescription;
SOMEXTERN somToken SOMDLINK TC__FullInterfaceDescription;

#pragma SOMAsDefault (pop)
#endif /* somtcnst_h */
