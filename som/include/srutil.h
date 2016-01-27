/*
 *   COMPONENT_NAME: somp
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.8 src/somp/srutil.h, somp, som2.1 12/26/95 17:02:26 [7/30/96 14:47:06] */

/*
 */

#ifndef srutil_h
  #define srutil_h

  #include <fmi.h>
  #include <nsma.h>
  #include <psma.h>

  /* Prototypes */


  long   SOMLINK sompFileOpen(string filename, int oflag, int shflag, int pmode);
  long   SOMLINK sompRaiseException(Environment *ev, long primary, long secondary);
  string SOMLINK sompExpandPidString (string rawpath, SOMPTError *rc);

  #ifdef SOM_STRICT_IDL
    SOMPTError SOMLINK sompInitPersistenceFW(SOMPPersistentStorageMgr *psm,
                                     SOMPNameSpaceMgr *nsm );
  #else
    SOMPTError SOMLINK sompInitPersistenceFW(SOMPPersistentStorageMgr **psm,
                                     SOMPNameSpaceMgr **nsm );
  #endif

#endif
