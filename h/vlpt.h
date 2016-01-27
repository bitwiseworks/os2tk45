/***    VLPT.H
 *
 *
 *      MVDM Virtual Printer Exported Definitions
 ****************************************************************************
 *                                                                          *
 *                                                                          *
 *                 Copyright (c) IBM Corporation  1993                      *
 *                           All Rights Reserved                            *
 *                                                                          *
 ****************************************************************************
 *
 *      For include file hierarchy, see MVDM.H
 *
 *
 *
 */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */

#ifndef __VLPT__
#define __VLPT__


/***    VLPT constants
 */


/***    VLPT data types
 */


/***    VLPT function prototypes
 */
VOID VDHENTRY VDHPrintClose(HVDM);

#endif /* __VLPT__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
