/****************************** Module Header ******************************\
*
* Module Name: PMWP.H
*
* OS/2 Presentation Manager Workplace include file.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_WINWORKPLACE       All of Workplace Shell
*   INCL_WPCLASS            Workplace object class API's
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between NOINC
* and INC comments.
*
\***************************************************************************/

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

/* NOINC */
#ifndef PMWP_INCLUDED  /* multiple include protection */
   /* INC */
   #define PMWP_INCLUDED
   
   #ifdef INCL_WINWORKPLACE
      #define INCL_WPCLASS
   #endif
   
   /*** Common types *******************************************************/
   
   typedef LHANDLE HOBJECT;
   
   /*** Object management calls ********************************************/
   #if (defined(INCL_WPCLASS) || !defined(INCL_NOCOMMON))
      /*** Standard object classes *****************************************/
      
      
      #define CCHMAXCLASS              3      /* Length of a classname     */
      
      #define QC_First                 0      /* Codes for OA_QueryContent */
      #define QC_Next                  1
      #define QC_Last                  2
      /* NOINC */
      #define QC_FIRST              QC_First
      #define QC_NEXT               QC_Next
      #define QC_LAST               QC_Last
      /* INC */
      
      #ifndef LOCATION_DESKTOP
         #define LOCATION_DESKTOP ((PSZ)0xFFFF0001) /* Current Desktop             */
         /* use instead of <WP_DESKTOP> */
      #endif
      
      /*** An object's appearance (icon or bitmap or outline) **************/
      
      typedef struct _OBJECTIMAGE     /* oimg */
      {
         HPOINTER hptrObject;
      } OBJECTIMAGE;
      typedef OBJECTIMAGE *POBJECTIMAGE;
      
      /*** Class info structure returned by WinEnumObjectClasses ***********/
      typedef struct _OBJCLASS         /* ocls */
      {
         struct _OBJCLASS *pNext;         /* Null for the last structure.. */
         PSZ               pszClassName;  /* Class name                    */
         PSZ               pszModName;    /* Module name                   */
      } OBJCLASS;
      typedef OBJCLASS *POBJCLASS;
      
      
      /*** Workplace object management functions ***************************/
      
#ifdef __cplusplus
      BOOL APIENTRY WinRegisterObjectClass(PCSZ  pszClassName,
                                           PCSZ  pszModName);
#else
      BOOL APIENTRY WinRegisterObjectClass(PSZ pszClassName,
                                           PSZ pszModName);
#endif
      
#ifdef __cplusplus
      BOOL APIENTRY WinDeregisterObjectClass(PCSZ  pszClassName);
#else
      BOOL APIENTRY WinDeregisterObjectClass(PSZ pszClassName);
#endif
      
#ifdef __cplusplus
      BOOL APIENTRY WinReplaceObjectClass(PCSZ  pszOldClassName,
                                          PCSZ  pszNewClassName,
                                          BOOL fReplace);
#else
      BOOL APIENTRY WinReplaceObjectClass(PSZ pszOldClassName,
                                          PSZ pszNewClassName,
                                          BOOL fReplace);
#endif
      
      BOOL APIENTRY WinEnumObjectClasses(POBJCLASS pObjClass,
                                         PULONG pulSize);
      
#ifdef __cplusplus
      HOBJECT APIENTRY WinCreateObject(PCSZ  pszClassName,
                                       PCSZ  pszTitle,
                                       PCSZ  pszSetupString,
                                       PCSZ  pszLocation,
                                       ULONG ulFlags );
#else
      HOBJECT APIENTRY WinCreateObject(PSZ pszClassName,
                                       PSZ pszTitle,
                                       PSZ pszSetupString,
                                       PSZ pszLocation,
                                       ULONG ulFlags );
#endif
      #define CO_FAILIFEXISTS     0
      #define CO_REPLACEIFEXISTS  1
      #define CO_UPDATEIFEXISTS   2
      
#ifdef __cplusplus
      BOOL APIENTRY WinSetObjectData(HOBJECT hObject,
                                     PCSZ  pszSetupString);
#else
      BOOL APIENTRY WinSetObjectData(HOBJECT hObject,
                                     PSZ pszSetupString);
#endif
      
      BOOL APIENTRY WinDestroyObject(HOBJECT hObject);
      
#ifdef __cplusplus
      HOBJECT APIENTRY WinQueryObject(PCSZ  pszObjectID);
#else
      HOBJECT APIENTRY WinQueryObject(PSZ pszObjectID);
#endif
      
      BOOL APIENTRY WinSaveObject(HOBJECT hObject,
                                  BOOL    fAsync);
      
      BOOL APIENTRY WinOpenObject(HOBJECT hObject,
                                  ULONG   ulView,
                                  BOOL    Flag);
      
      HOBJECT APIENTRY WinMoveObject(HOBJECT hObjectofObject,
                                     HOBJECT hObjectofDest,
                                     ULONG   ulReserved);
      
      HOBJECT APIENTRY WinCopyObject(HOBJECT hObjectofObject,
                                     HOBJECT hObjectofDest,
                                     ULONG   ulReserved);
      
      HOBJECT APIENTRY WinCreateShadow(HOBJECT hObjectofObject,
                                       HOBJECT hObjectofDest,
                                       ULONG   ulReserved);
#ifdef __cplusplus
      BOOL APIENTRY WinQueryActiveDesktopPathname(PCSZ  pszPathName,
                                                  ULONG ulSize);
#else
      BOOL APIENTRY WinQueryActiveDesktopPathname(PSZ pszPathName,
                                                  ULONG ulSize);
#endif
#ifdef __cplusplus
      BOOL APIENTRY WinQueryObjectPath(HOBJECT hobject,
                                       PCSZ  pszPathName,
                                       ULONG ulSize);
#else
      BOOL APIENTRY WinQueryObjectPath(HOBJECT hobject,
                                       PSZ pszPathName,
                                       ULONG ulSize);
#endif
      
      APIRET  APIENTRY WinRestartWPDServer( BOOL fState );
      BOOL    APIENTRY WinIsWPDServerReady( VOID );
      APIRET  APIENTRY WinRestartSOMDD( BOOL fState );
      BOOL    APIENTRY WinIsSOMDDReady( VOID );
      
   #endif  /*WPCLASS*/
   
   /*** Error codes ********************************************************/
   #define INCL_WPERRORS
   #include <pmerr.h>
   
   /*** Object settings notebook page insertion structure ******************/
   
   typedef struct _PAGEINFO     /* pginf */
   {
      ULONG   cb;
      HWND    hwndPage;
      PFNWP   pfnwp;
      ULONG   resid;
      PVOID   pCreateParams;
      USHORT  dlgid;
      USHORT  usPageStyleFlags;
      USHORT  usPageInsertFlags;
      USHORT  usSettingsFlags;
      PSZ     pszName;
      USHORT  idDefaultHelpPanel;
      USHORT  usReserved2;
      PSZ     pszHelpLibraryName;
      PUSHORT pHelpSubtable;   /* PHELPSUBTABLE when PMHELP.H is included*/
      HMODULE hmodHelpSubtable;
      ULONG   ulPageInsertId;
   } PAGEINFO;
   typedef PAGEINFO *PPAGEINFO;
   
   #define  SETTINGS_PAGE_NUMBERS     0x01
   
   /*** Utility apis +******************************************************/
   
   #if (!defined(INCL_NOCOMMON))
      typedef struct _ICONPOS     /* icp */
      {
         POINTL  ptlIcon;                    /* Location */
         CHAR    szIdentity[1];              /* Object identity string */
      } ICONPOS;
      typedef ICONPOS *PICONPOS;
      
      /*********************************************************************/
      #ifdef INCL_WINPOINTERS
#ifdef __cplusplus
         BOOL APIENTRY WinSetFileIcon(PCSZ  pszFileName,
                                      PICONINFO pIcnInfo);
#else
         BOOL APIENTRY WinSetFileIcon(PSZ pszFileName,
                                      PICONINFO pIcnInfo);
#endif
         
#ifdef __cplusplus
         BOOL APIENTRY WinSetFileIconN(PCSZ  pszFileName,
                                       PICONINFO pIcnInfo,
                                       ULONG ulIconIndex);
#else
         BOOL APIENTRY WinSetFileIconN(PSZ pszFileName,
                                       PICONINFO pIcnInfo,
                                       ULONG ulIconIndex);
#endif
      #endif
      BOOL APIENTRY WinFreeFileIcon(HPOINTER hptr);
      
#ifdef __cplusplus
      HPOINTER APIENTRY WinLoadFileIcon(PCSZ  pszFileName,
                                        BOOL fPrivate);
#else
      HPOINTER APIENTRY WinLoadFileIcon(PSZ pszFileName,
                                        BOOL fPrivate);
#endif
      
#ifdef __cplusplus
      HPOINTER APIENTRY WinLoadFileIconN(PCSZ  pszFileName,
                                         BOOL fPrivate,
                                         ULONG ulIconIndex);
#else
      HPOINTER APIENTRY WinLoadFileIconN(PSZ pszFileName,
                                         BOOL fPrivate,
                                         ULONG ulIconIndex);
#endif
      
#ifdef __cplusplus
      BOOL APIENTRY WinStoreWindowPos(PCSZ  pszAppName,
                                      PCSZ  pszKeyName,
                                      HWND hwnd);
#else
      BOOL APIENTRY WinStoreWindowPos(PSZ pszAppName,
                                      PSZ pszKeyName,
                                      HWND hwnd);
#endif
      
#ifdef __cplusplus
      BOOL APIENTRY WinRestoreWindowPos(PCSZ  pszAppName,
                                        PCSZ  pszKeyName,
                                        HWND hwnd);
#else
      BOOL APIENTRY WinRestoreWindowPos(PSZ pszAppName,
                                        PSZ pszKeyName,
                                        HWND hwnd);
#endif
      
      BOOL APIENTRY WinShutdownSystem(HAB hab,
                                      HMQ hmq);
   #endif
   
   /* NOINC */
#endif /* PMWP_INCLUDED */
/* INC */

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

/**************************** end of file **********************************/
