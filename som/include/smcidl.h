/*
 *   COMPONENT_NAME: somc
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
/* @(#) somc/smcidl.h 2.7 12/26/95 15:53:19 [7/30/96 14:44:59] */

/*
 */

/*
 *         File:    smcidl.h.
 *     Contents:    Header file which contains special macros for
 *                  so that we can easily construct c and idl header
 *                  files and types.
 *
 *                  It can be include multiple times without damage!
 */

#undef _case
#undef _switch
#undef _struct
#undef _typedef
#undef _name

#ifdef __SOMIDL__
    /*
     * The _case and _switch macro are for IDL Unions.
     */
    #define _case(n)                       case n:
    #define _switch(t)                     switch(t)
    #define _struct                        /* Nothing */
    #define _typedef                       /* Nothing */
    #define _name(name)                    /* Nothing */
#else
    #define _case(n)                       /* Nothing */
    #define _switch(t)                     /* Nothing */
    #define _struct                        struct
    #define _typedef                       typedef
    #define _name(name)                    name
#endif
