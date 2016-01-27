/*
 *   COMPONENT_NAME: somi
 *
 *   ORIGINS: 27
 *
 *
 *   10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.6.1.2 src/somc/tc/somtc.h, somtc.funcs, som2.1 11/28/95 13:35:00 [7/30/96 14:45:06] */

/*
 */

/*
 * SOMTC.H
 * Typedefs, structs, & enums in support of CORBA TypeCodes
 * and sequences for use with SOM
 */

#ifndef SOMTC_h
#define SOMTC_h

#ifdef SOMTC_xh
  #include <som.xh>
#else
  #include <som.h>
#endif /* SOMTC_xh */

/*
 * Operations supported by TypeCodes
 *
 */
#ifdef __IBMC__
  #pragma linkage (tcAlignment,	   system)
  #pragma linkage (tcCopy,	   system)
  #pragma linkage (tcEqual,	   system)
  #pragma linkage (tcFree,	   system)
  #pragma linkage (tcKind,	   system)
  #pragma linkage (tcNew,	   system)
  #pragma linkage (tcNewVL,	   system)
  #pragma linkage (tcParameter,    system)
  #pragma linkage (tcParmCount,    system)
  #pragma linkage (tcPrint,	   system)
  #pragma linkage (tcSetAlignment, system)
  #pragma linkage (tcSize,	   system)
  #pragma linkage (tcSequenceNew,  system)
  #pragma linkage (tcSeqFromListString, system)
  #pragma linkage (tcSetZeroOriginEnum, system)		
  #pragma linkage (tcGetZeroOriginEnum, system)	
#endif /* __IBMC__ */

SOMEXTERN short    SOMLINK tcAlignment (TypeCode t,Environment *ev);
SOMEXTERN TypeCode SOMLINK tcCopy      (TypeCode t,Environment *ev);
SOMEXTERN boolean  SOMLINK tcEqual     (TypeCode x,Environment *ev,TypeCode y);
SOMEXTERN void	   SOMLINK tcFree      (TypeCode t,Environment *ev);
SOMEXTERN TCKind   SOMLINK tcKind      (TypeCode t,Environment *ev);
SOMEXTERN long	   SOMLINK tcParmCount (TypeCode t,Environment *ev);
SOMEXTERN any	   SOMLINK tcParameter (TypeCode t,Environment *ev,long index);
SOMEXTERN void	   SOMLINK tcPrint     (TypeCode t,Environment *ev);
SOMEXTERN void	   SOMLINK tcSetAlignment (TypeCode t,Environment *ev, short a);
SOMEXTERN long	   SOMLINK tcSize      (TypeCode t,Environment *ev);

SOMEXTERN long     SOMLINK tcSetZeroOriginEnum (TypeCode t, Environment *ev, boolean value);
SOMEXTERN boolean  SOMLINK tcGetZeroOriginEnum (TypeCode t, Environment *ev);	


/*
 * Variable argument sequences for tcNew:
 * (based on CORBA 7.6.1 Table 16 pp 139-140)
 *
 *  char     *interfaceId, *name, *mbrName, *enumId;
 *  char     *structOrUnionName, *typeName, *context;
 *  long     maxLength, length, labelFlag, labelValue;
 *  TypeCode swTC, mbrTC, seqTC, arrayTC, ptrTC;
 *  TCKind   tk_<xxx>;
 *
 *  tcNew (tk_<xxx>);
 *  tcNew (tk_objref interfaceId);
 *  tcNew (tk_string, maxLength);
 *  tcNew (tk_sequence, seqTC, maxLength);
 *  tcNew (tk_array, arrayTC, length);
 *  tcNew (tk_pointer, ptrTC);                     <---[SOM extension]
 *  tcNew (tk_self, structOrUnionName);            <---[SOM extension]
 *  tcNew (tk_foreign, typeName, context, length); <---[SOM extension]
 *
 *  tcNew (tk_struct, name, mbrName, mbrTC, [...,] NULL);
 *      [mbrName & TypeCode repeat as needed]
 *
 *  tcNew (tk_union, name, swTC, 
 *      labelFlag, labelValue, mbrName, mbrTC, [...,] NULL);
 *      [labelFlag, labelValue, mbrName & TypeCode repeat as needed]
 *      labelFlag has the following meanings:
 *	    TCREGULAR_CASE - regular labelValue, cast as a long, follows,
 *	    TCDEFAULT_CASE - default case, with 0L for labelValue follows.
 *
 *  tcNew (tk_enum, name, enumId, [...,] NULL);
 *	[enumId repeats as needed]
 *
 *  NULL is returned if tcNew detects an error or an attempt
 *  to construct an illegal TypeCode
 *
 *  TypeCodes are assigned default alignment attributes appropriate 
 *  to their constituent types.  The default alignment associated with  
 *  the TypeCode as a whole is the greatest alignment associated with
 *  any of its subordinate TypeCodes or primitive elements.  Note that
 *  alignment information is platform-specific.  Default alignment
 *  information can be overridden using the tcSetAlignment function,
 *  where an argument value of zero means use default alignment, 
 *  1 = byte alignment, n = alignment to an address boundary divisible 
 *  by n.
 *
 *  TypeCodes created by tcNew do not hold references to any of the
 *  passed strings, which are assumed to be owned by the caller.
 *  This is not the case for the "TypeCode" arguments. They become
 *  directly imbedded in the new TypeCode.  If you need to retain
 *  ownership of passed TypeCodes, pass them using tcCopy.  Note that
 *  TypeCode constants (TC_<xxx>) can be freely passed as arguments to
 *  tcNew without using tcCopy.
 *
 *  All TypeCodes created by tcNew can be freed using tcFree.
 */
SOMEXTERN TypeCode SOMLINK tcNew (TCKind tag, ...);

/*  The following special variant on tcNew can be used when the
 *  arguments are not known at compile-time and must be constructed
 *  dynamically as a va_list.
 */
SOMEXTERN TypeCode SOMLINK tcNewVL (TCKind tag, va_list ap);

/*
 *  Following constants used with tcNew to create union TypeCodes.
 *  See calling sequences above.
 */
#define TCREGULAR_CASE 1L
#define TCDEFAULT_CASE 2L

SOMEXTERN GENERIC_SEQUENCE * SOMLINK tcSequenceNew (TypeCode t,
						    unsigned long max);
#ifndef _IDL_SEQUENCE_string_defined
#define _IDL_SEQUENCE_string_defined
SOM_SEQUENCE_TYPEDEF (string);
#endif /* _IDL_SEQUENCE_string_defined */

SOMEXTERN sequence(string) SOMLINK tcSeqFromListString (string s);

/* CORBA function names for TypeCodes, per CORBA 7.6.1, p.139 */
#define TypeCode_kind	      tcKind
#define TypeCode_equal	      tcEqual
#define TypeCode_param_count  tcParmCount
#define TypeCode_parameter    tcParameter
/*
 *  The following are IBM TypeCode extensions
 */
#define TypeCode_alignment    tcAlignment
#define TypeCode_copy	      tcCopy
#define TypeCode_free	      tcFree
#define TypeCode_print	      tcPrint
#define TypeCode_setAlignment tcSetAlignment
#define TypeCode_size	      tcSize
#define TypeCode_setZeroOriginEnum tcSetZeroOriginEnum
#define TypeCode_getZeroOriginEnum tcGetZeroOriginEnum

#define TypeCodeNew           tcNew
#define TypeCodeNewVL         tcNewVL

/* somVaBuf support */

#ifndef SOMVABUF
  #define SOMVABUF
  typedef somToken somVaBuf;

#ifdef __IBMC__
  #pragma linkage(somVaBuf_create,	system)
  #pragma linkage(somVaBuf_get_valist,	system)
  #pragma linkage(somVaBuf_destroy,	system)
  #pragma linkage(somVaBuf_add,		system)
  #pragma linkage(somvalistGetTarget,	system)
  #pragma linkage(somvalistSetTarget,	system)
#endif

SOMEXTERN void*  SOMLINK  somVaBuf_create(char *, int);
SOMEXTERN void SOMLINK    somVaBuf_get_valist(void *, va_list *);
SOMEXTERN void SOMLINK    somVaBuf_destroy(void *);
SOMEXTERN long SOMLINK    somVaBuf_add(void * , char *, int);
SOMEXTERN unsigned long SOMLINK   somvalistGetTarget(va_list);
SOMEXTERN void   SOMLINK  somvalistSetTarget(va_list, unsigned long);
#endif /* SOMVABUF */

#endif /* SOMTC_h */
