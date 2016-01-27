/*static char *SCCSID = "@(#)bsememf.h  6.8 92/03/15";*/

 /******************************************************************************\
 *                                                                             *
 * Module Name: BSEMEMF.H                                                      *
 *                                                                             *
 * OS/2 Base MEMory Flags include file                                         *
 *                                                                             *
 * Copyright (c) 1989  - 1992 IBM Corporation                                  *
 *                                                                             *
 *******************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#ifndef __BSEMEMF__
/* NOINC */
#define __BSEMEMF__
/* INC */

 /* Access protection                                                          */

 #define PAG_READ          0x00000001U      /* read access                      */

 #define PAG_WRITE         0x00000002U      /* write access                     */

 #define PAG_EXECUTE       0x00000004U      /* execute access                   */

 #define PAG_GUARD         0x00000008U      /* guard protection                 */

 #define PAG_DEFAULT       0x00000400U      /* default (initial) access         */



 /* Commit                                                                     */

 #define PAG_COMMIT        0x00000010U      /* commit storage                   */

 #define PAG_DECOMMIT      0x00000020U      /* decommit storage                 */



 /* Allocation attributes                                                      */

 #define OBJ_TILE          0x00000040U      /* tile object                      */

 #define OBJ_PROTECTED     0x00000080U      /* protect object

                                            NOTE: This flag is NOT available at

                                            the api level */

 #define OBJ_GETTABLE      0x00000100U      /* gettable by other processes      */

 #define OBJ_GIVEABLE      0x00000200U      /* giveable to other processes      */

 #define OBJ_ANY           0x00000400U      /* allocate memory anywhere         */



 #define fPERM             (PAG_EXECUTE | PAG_READ | PAG_WRITE)

 #define fSHARE            (OBJ_GETTABLE | OBJ_GIVEABLE)



 /* DosAllocMem flags                                                          */

 #define fALLOC            (OBJ_TILE | PAG_COMMIT | fPERM)



 /* DosAllocSharedMem flags                                                    */

 #define fALLOCSHR         (OBJ_TILE | PAG_COMMIT | fSHARE | fPERM)



 /* DosGetNamedSharedMem flags                                                 */

 #define fGETNMSHR         (fPERM)



 /* DosGetSharedMem flags                                                      */

 #define fGETSHR           (fPERM)



 /* DosGiveSharedMem flags                                                     */

 #define fGIVESHR          (fPERM)



 /* DosSetMem flags                                                            */

 #define fSET              (PAG_COMMIT + PAG_DECOMMIT + PAG_DEFAULT + fPERM)



 /* Dos32SubSet flags                                                          */

 #define DOSSUB_INIT       0x01U           /* initialize memory object for     */

                                           /* suballocation                    */

 #define DOSSUB_GROW       0x02U           /* increase size of memory pool     */

                                           /* for suballocation                */

 #define DOSSUB_SPARSE_OBJ 0x04U           /* indicator for DosSub to          */

                                           /* manage the commitment of         */

                                           /* pages spanned by the memory      */

                                           /* pool                             */

 #define DOSSUB_SERIALIZE  0x08U           /* indicates that access to the     */

                                           /* memory pool is to be             */

                                           /* serialized by DosSub             */



 /* Allocation type (returned from DosQueryMem)                                */

 #define PAG_SHARED        0x00002000U     /* shared object                    */

 #define PAG_FREE          0x00004000U     /* pages are free                   */

 #define PAG_BASE          0x00010000U     /* first page in object             */

 /* for DosAllocMem and DosAliasMem */
 #define OBJ_SELMAPALL   0x00000800      /* First sel map all of object */

 /* for DosAliasMem */
 /* In addition to OBJ_TILE (defined in bsememf.h) and OBJ_SELMAPALL */
 #define SEL_CODE        0x00000001      /* Selector is code */
 #define SEL_USE32       0x00000002      /* Selector is USE32 */

 /* Page state (returned from DosQueryMemState) */
 #define PAG_NPOUT       0x00000000      /* page is not present, not in core */
 #define PAG_PRESENT     0x00000001      /* page is present */
 #define PAG_NPIN        0x00000002      /* page is not present, in core */
 #define PAG_PRESMASK    0x00000003      /* present state mask */
 #define PAG_INVALID     0x00000000      /* page is invalid */
 #define PAG_RESIDENT    0x00000010      /* page is resident */
 #define PAG_SWAPPABLE   0x00000020      /* page is swappable */
 #define PAG_DISCARDABLE 0x00000030      /* page is discardable */
 #define PAG_TYPEMASK    0x00000030      /* type mask */

#endif /* __BSEMEMF__ */

/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
