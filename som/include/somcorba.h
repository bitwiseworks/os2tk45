/*
 *   COMPONENT_NAME: somk
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
/* @(#) somk/somcorba.h 2.10.1.3 12/26/95 16:48:02 [7/30/96 14:46:40] */

/*
 */

/*
 *    SOMCORBA.H
 *    Typedefs, structs, & prototypes in support of CORBA extensions
 *    to the SOM run-time
 */

#ifndef SOMCorba_h
#define SOMCorba_h
#include <string.h>  /* needed for memset, used below */

#ifdef CORBA_FUNCTION_NAMES
#define CORBAObject Object
#endif

/* in SOM, a CORBA object is a SOM object */
typedef SOMObject CORBAObject;

/* CORBA 5.7, p.89 */
#ifndef SOM_BOOLEAN
  #define SOM_BOOLEAN
  typedef unsigned char boolean;
#endif /* SOM_BOOLEAN */
typedef unsigned char octet;
typedef char *string;

/* CORBA 7.5.1, p. 129 */
typedef string Identifier;

/* CORBA 4.13, p. 80 */
/*
 * Generated SOM usage bindings for IDL enums start at 1, but
 * somcorba.h is not generated, and the original SOM 2.0 somcorba.h
 * used C enum typedefs to define the exception_type and
 * completion_status enumerations.  As a result, to maintain backwards
 * binary compatibility, the mapping for these enums starts at 0
 * (which is also the mapping specified by CORBA 2.0).
 *
 * The additional value enum_name_MAX is needed to ensure that all
 * compilers will allocate 4 bytes for these enums.  This technique
 * for representing IDL enums is used in the CORBA 2.0 * C++ mappings.
 */
typedef enum exception_type {NO_EXCEPTION, USER_EXCEPTION, SYSTEM_EXCEPTION,
    exception_type_MAX = 2147483647       /* ensure mapped as 4 bytes */
} exception_type;
typedef enum completion_status {YES, NO, MAYBE,
    completion_status_MAX = 2147483647    /* ensure mapped as 4 bytes */
} completion_status;

typedef struct StExcep {
    unsigned long minor;
    completion_status completed;
} StExcep;

typedef struct Environment {
    exception_type             _major;
    struct {
        char *                 _exception_name;
        void *                 _params;
    } exception;
    void *                     _somdAnchor;
} Environment;

/* CORBA 7.6.1, p.139 plus 5.7, p.89 enum Data Type Mapping */
typedef unsigned long TCKind;
  #define TypeCode_tk_null      1
  #define TypeCode_tk_void      2
  #define TypeCode_tk_short     3
  #define TypeCode_tk_long      4
  #define TypeCode_tk_ushort    5
  #define TypeCode_tk_ulong     6
  #define TypeCode_tk_float     7
  #define TypeCode_tk_double    8
  #define TypeCode_tk_boolean   9
  #define TypeCode_tk_char      10
  #define TypeCode_tk_octet     11
  #define TypeCode_tk_any       12
  #define TypeCode_tk_TypeCode  13
  #define TypeCode_tk_Principal 14
  #define TypeCode_tk_objref    15
  #define TypeCode_tk_struct    16
  #define TypeCode_tk_union     17
  #define TypeCode_tk_enum      18
  #define TypeCode_tk_string    19
  #define TypeCode_tk_sequence  20
  #define TypeCode_tk_array     21

  #define TypeCode_tk_pointer   101 /* SOM extension */
  #define TypeCode_tk_self      102 /* SOM extension */
  #define TypeCode_tk_foreign   103 /* SOM extension */

/* Short forms of tk_<x> enumerators */

  #define tk_null       TypeCode_tk_null
  #define tk_void       TypeCode_tk_void
  #define tk_short      TypeCode_tk_short
  #define tk_long       TypeCode_tk_long
  #define tk_ushort     TypeCode_tk_ushort
  #define tk_ulong      TypeCode_tk_ulong
  #define tk_float      TypeCode_tk_float
  #define tk_double     TypeCode_tk_double
  #define tk_boolean    TypeCode_tk_boolean
  #define tk_char       TypeCode_tk_char
  #define tk_octet      TypeCode_tk_octet
  #define tk_any        TypeCode_tk_any
  #define tk_TypeCode   TypeCode_tk_TypeCode
  #define tk_Principal  TypeCode_tk_Principal
  #define tk_objref     TypeCode_tk_objref
  #define tk_struct     TypeCode_tk_struct
  #define tk_union      TypeCode_tk_union
  #define tk_enum       TypeCode_tk_enum
  #define tk_string     TypeCode_tk_string
  #define tk_sequence   TypeCode_tk_sequence
  #define tk_array      TypeCode_tk_array
  #define tk_pointer    TypeCode_tk_pointer
  #define tk_self       TypeCode_tk_self
  #define tk_foreign    TypeCode_tk_foreign

#ifndef SOM_TYPECODE
  #define SOM_TYPECODE
  typedef void * TypeCode;
#endif /* SOM_TYPECODE */

/* CORBA 5.7, p.89 */
typedef struct any {
    TypeCode _type;
    void   * _value;
    } any;

/* Convenience macros for sequences */
#define sequence(type) _IDL_SEQUENCE_ ## type
#define SOM_SEQUENCE_NAME(name,type)\
    struct name {\
        unsigned long _maximum;\
        unsigned long _length;\
        type     *    _buffer;\
    }
#define SOM_SEQUENCE(type)\
    struct {\
        unsigned long _maximum;\
        unsigned long _length;\
        type     *    _buffer;\
    }

#define SOM_SEQUENCE_TYPEDEF(type) typedef SOM_SEQUENCE(type) sequence(type)
#define SOM_SEQUENCE_TYPEDEF_NAME(type, name) typedef SOM_SEQUENCE(type) name

/* per CORBA 5.10, p.91 */
#ifndef _IDL_SEQUENCE_void_defined
#define _IDL_SEQUENCE_void_defined
SOM_SEQUENCE_TYPEDEF (void);
#endif /* _IDL_SEQUENCE_void_defined */

/* SOM extensions for sequence manipulation */
#define GENERIC_SEQUENCE        sequence(void)
#define sequenceNew(type,max)   (*(sequence(type) *)tcSequenceNew(TC_##type,max))
/* Note that sequenceNew macro assumes that a TypeCode constant of the  */
/* form TC_xxx exists for any type xxx used as an argument              */
#define sequenceElement(s,elem) ((s)._buffer[elem])
#define sequenceLength(s)       ((s)._length)
#define sequenceMaximum(s)      ((s)._maximum)

#ifdef __IBMC__
  #pragma linkage (somExceptionId,             system)
  #pragma linkage (somExceptionValue,          system)
  #pragma linkage (somExceptionFree,           system)
  #pragma linkage (somSetException,            system)
  #pragma linkage (somGetGlobalEnvironment,    system)
#endif /* __IBMC__ */

SOMEXTERN char * SOMLINK somExceptionId    (Environment *ev);
SOMEXTERN void * SOMLINK somExceptionValue (Environment *ev);
SOMEXTERN void   SOMLINK somExceptionFree  (Environment *ev);
SOMEXTERN void   SOMLINK somSetException   (Environment *ev,
    exception_type major, char *exception_name, void *params);
SOMEXTERN Environment * SOMLINK somGetGlobalEnvironment (void);

/* Exception function names per CORBA 5.19, p.99 */
#define exception_id    somExceptionId
#define exception_value somExceptionValue
#define exception_free  somExceptionFree

#ifndef    TRUE
#define    TRUE 1
#endif  /* TRUE */
#ifndef    FALSE
#define    FALSE 0
#endif  /* FALSE */

#define SOM_InterfaceRepository\
    (__get_somInterfaceRepository(SOMClassMgrObject))

/*  Convenience macros for manipulating environment structures
 *
 *  SOM_CreateLocalEnvironment returns a pointer to an Environment.
 *  The other 3 macros all expect a single argument that is also
 *  a pointer to an Environment.  Use the create/destroy forms for
 *  a dynamic local environment and the init/uninit forms for a stack-based
 *  local environment.
 *
 *  For example
 *
 *      Environment *ev;
 *      ev = SOM_CreateLocalEnvironment ();
 *      ... Use ev in methods
 *      SOM_DestroyLocalEnvironment (ev);
 *
 *  or
 *
 *      Environment ev;
 *      SOM_InitEnvironment (&ev);
 *      ... Use &ev in methods
 *      SOM_UninitEnvironment (&ev);
 */
#define SOM_CreateLocalEnvironment()\
    ((Environment *) SOMCalloc (1, sizeof (Environment)))
#define SOM_DestroyLocalEnvironment(ev)\
    (somExceptionFree ((ev)), (somGetGlobalEnvironment() == (ev)) ?\
        (void) 0 : SOMFree ((ev)))
#define SOM_InitEnvironment(ev)\
    ((somGetGlobalEnvironment() == (ev)) ?\
        (void *) NULL : memset (((char *)((ev))), 0, sizeof (Environment)))
#define SOM_UninitEnvironment(ev)\
    (somExceptionFree ((ev)))

#endif /* SOMCorba_h */
