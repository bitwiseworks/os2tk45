/******************************************************************************
*
*  Module Name: CARPP.CPP
*
*  OS/2 WorkPlace Shell Sample Program - SOM 2.0 / IDL Version
*
*  Copyright (C) 1992-1995 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
*  Entry Points:
*
*     Class Methods:
*
*        carppM_QueryModuleHandle
*
*     Overridden Class Methods:
*
*        carppM_wpclsInitData
*        carppM_wpclsQueryDefaultHelp
*        carppM_wpclsQueryDefaultView
*        carppM_wpclsQueryDetailsInfo
*        carppM_wpclsQueryIconData
*        carppM_wpclsQueryStyle
*        carppM_wpclsQueryTitle
*        carppM_wpclsUnInitData
*
*     Instance Methods:
*
*        carpp_AddDashboardPage
*        carpp_AddHornBeepPage
*
*     Overridden Instance Methods:
*
*        carpp_wpAddSettingsPages
*        carpp_wpFilterPopupMenu
*        carpp_wpInitData
*        carpp_wpMenuItemHelpSelected
*        carpp_wpMenuItemSelected
*        carpp_wpModifyPopupMenu
*        carpp_wpViewObject
*        carpp_wpQueryDetailsData
*        carpp_wpRestoreState
*        carpp_wpSaveState
*        carpp_wpUnInitData
*
*     Non-Method Functions:
*
*        CarInit
*        CarWndProc
*        DashBoardDlgProc
*        HornBeepDlgProc
*
******************************************************************************/

#define Carpp_Class_Source
#define M_Carpp_Class_Source

#include "carpp.xih"          /* implementation header emitted from carpp.idl */

#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <setjmp.h>

/*
 *   Non-Method function prototypes
 */

HWND             CarInit (Carpp*);
MRESULT EXPENTRY DashBoardDlgProc( HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY HornBeepDlgProc( HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY CarWndProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

ULONG _System _Exception(EXCEPTIONREPORTRECORD       *parg,
                 EXCEPTIONREGISTRATIONRECORD *pRegisRecord,
                 PCONTEXTRECORD               pContextRecord,
                 PVOID                        pvSpare);



/***************** GLOBAL/STATIC (NON-INSTANCE) DATA SECTION ******************
*****                                                                     *****
*****    This data shouldn't be changed by instance methods or it will    *****
*****    affect all instances!  Any variables that are specific (unique   *****
*****    values) for each instance of this object should be declared as   *****
*****  instance data or dynamically allocated and stored as window data.  *****
*****                                                                     *****
*****      This global data should be declared as class instance data     *****
*****    if it will change after initialization.  In this case, it will   *****
*****                  be accessed through class methods.                 *****
*****                                                                     *****
******************************************************************************/

   CHAR       szCarWindowClass[] =    "CARSAMPPP";
   CHAR       szCarInstanceFilter[] = "*.CAR";
   CHAR       szHelpLibrary[] =       "carpp.hlp";
   CHAR       szCarClassTitle[CCHMAXPATH] = "";
   HMODULE    hmod = NULLHANDLE;

   PCSZ       pcszDefaultText ="\
                               \n\
     Bill of Sale              \n\
   =========================   \n\
                               \n\
   Make........ Toyota         \n\
                               \n\
   Model....... Camry          \n\
                               \n\
   Color....... Blue           \n\
                               \n\
   Sale Date... 3/31/92        \n\
                               \n\
   Price....... 14,995         \n";

   /*
    *   Globals required for Exception handling
    */

   jmp_buf jmpState;

   PCSZ pcszTrapMessage =                        "\
\
A Memory Access Violation occurred.  The Car \
sample's exception handler has transferred \
control back to the cleanup code in the method \
where the exception occurred.\n";

   /*
    *   Statics required for FIELDINFO structures needed for DETAILS view are
    *   handled in the three functions:
    *
    *   carpp_wpclsInitData, carpp_wpQueryDetailsData, carpp_wpQueryClassDetailsInfo
    */

   #define NUM_CAR_FIELDS    5

   CLASSFIELDINFO fieldinfo[NUM_CAR_FIELDS];      /* structure in wpobject.h */

   PCSZ apszCarColTitles[] =
   {
      "Make",           /* details column 1 */
      "Model",          /* details column 2 */
      "Color",          /* details column 3 */
      "Sale date",      /* details column 4 */
      "Price ($)"       /* details column 5 */
   };


/*************************  INSTANCE METHODS SECTION  *************************
*****                                                                     *****
*****              Do not put any code in this section unless             *****
*****                   it is an object INSTANCE method                   *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMInstance

/*
 *
 * METHOD: AddDashboardPage                               ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   This method adds the dashboard page to the settings notebook.
 *
 * RETURN:
 *
 *   0              Unsuccessful
 *   ulPageId       Identifier for the inserted page
 *
 * HOW TO OVERRIDE:
 *
 *   Method should always be overridden in order to replace or remove
 *   the dashboard page from an object which is a descendant of Car.
 *   In most cases, an override of this method will not call the parent.
 */

SOM_Scope ULONG SOMLINK carpp_AddDashboardPage(Carpp *somSelf, HWND hwndNotebook)
{
    PAGEINFO pageinfo;
    ULONG versionData[2], dlgId;

    DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR, versionData, 8);
    if ( (versionData[0] <= 20 ) && (versionData[1] < 40) )
       dlgId = IDD_DASHBOARD;
    else
       dlgId = IDD_DASHBOARD2;

    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_AddDashboardPage");

    memset((PCH)&pageinfo,0,sizeof(PAGEINFO));
    pageinfo.cb                 = sizeof(PAGEINFO);
    pageinfo.hwndPage           = NULLHANDLE;
    pageinfo.usPageStyleFlags   = BKA_MAJOR;
    pageinfo.usPageInsertFlags  = BKA_FIRST;
    pageinfo.pfnwp              = DashBoardDlgProc;
    pageinfo.resid              = hmod;
    pageinfo.dlgid              = dlgId;
    pageinfo.pszName            = "Dashboard";
    pageinfo.pCreateParams      = somSelf;
    pageinfo.idDefaultHelpPanel = ID_HELP_DASHBOARD;
    pageinfo.pszHelpLibraryName = szHelpLibrary;

    return somSelf->wpInsertSettingsPage( hwndNotebook, &pageinfo );

}   /* end carpp_AddDashboardPage() */


/*
 *
 * METHOD: AddHornBeepPage                                ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   This method adds the horn beep page to the settings
 *   notebook.
 *
 * RETURN:
 *
 *   0              Unsuccessful
 *   ulPageId       Identifier for the inserted page
 *
 * HOW TO OVERRIDE:
 *
 *   Method should always be overridden in order to replace or remove
 *   the horn beep page from an object which is a descendant of Car.
 *   In most cases, an override of this method will not call the parent.
 */

SOM_Scope ULONG SOMLINK carpp_AddHornBeepPage(Carpp *somSelf, HWND hwndNotebook)
{
    PAGEINFO pageinfo;
    ULONG versionData[2], dlgId;

    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_AddHornBeepPage");

    DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR, versionData, 8);
    if ( (versionData[0] <= 20 ) && (versionData[1] < 40) )
       dlgId = IDD_HORNBEEP;
    else
       dlgId = IDD_HORNBEEP2;

    /* Insert the settings pages for a car
     */
    memset((PCH)&pageinfo,0,sizeof(PAGEINFO));
    pageinfo.cb                 = sizeof(PAGEINFO);
    pageinfo.hwndPage           = NULLHANDLE;
    pageinfo.usPageStyleFlags   = BKA_MAJOR;
    pageinfo.usPageInsertFlags  = BKA_FIRST;
    pageinfo.pfnwp              = HornBeepDlgProc;
    pageinfo.resid              = hmod;
    pageinfo.dlgid              = dlgId;
    pageinfo.pszName            = "Horn Beep";
    pageinfo.pCreateParams      = somSelf;
    pageinfo.idDefaultHelpPanel = ID_HELP_HORNBEEP;
    pageinfo.pszHelpLibraryName = szHelpLibrary;

    return somSelf->wpInsertSettingsPage( hwndNotebook, &pageinfo );

}   /* end carpp_AddHornBeepPage() */

/*
 *
 *  OVERRIDE: wpInitData                                   ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Initialize our state variables. Allocate any extra memory that
 *    we might need.
 *
 */

SOM_Scope void  SOMLINK carpp_wpInitData(Carpp *somSelf)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpInitData");

    /*
     *   We can initialize our instance data to 0's by using the somThis
     *   pointer and the size of the CarppData structure created by SOM.
     *
     *   SOM stores instance data in a data structure named by prefixing
     *   the name "Data" with the class name, in this case, "Carpp".
     */

    memset((PVOID)somThis, 0, sizeof(CarppData));

    /*
     *   And/or we can explicitly initialize our instance variables.
     */

    somThis->carBrake   = new Brake;
    somThis->carHorn    = new Horn;
    somThis->carRPM     = new RPM;

    parent_wpInitData(somSelf);

}   /* end carpp_wpInitData() */

/*
 *
 *  OVERRIDE: wpUnInitData                                 ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Clear up memory that was allocated on wpInitData.
 *
 */

SOM_Scope void  SOMLINK carpp_wpUnInitData(Carpp *somSelf)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpUnInitData");

    parent_wpUnInitData(somSelf);

}   /* end carpp_wpUnInitData() */


/*
 *
 *  METHOD: wpSaveState                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Save our state variables (pitch and duration).
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpSaveState(Carpp *somSelf)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpSaveState");

    somThis->carBrake->SaveState(somSelf, szCarClassTitle);
    somThis->carHorn->SaveState(somSelf, szCarClassTitle);
    somThis->carRPM->SaveState(somSelf, szCarClassTitle);

    return (parent_wpSaveState(somSelf));

}   /* end carpp_wpSaveState() */

/*
 *
 *  METHOD: wpRestoreState                                 ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Retrieve our saved state variables (pitch and duration).
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpRestoreState(Carpp *somSelf, ULONG ulReserved)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpRestoreState");

    somThis->carBrake->RestoreState(somSelf, szCarClassTitle);
    somThis->carHorn->RestoreState(somSelf, szCarClassTitle);
    somThis->carRPM->RestoreState(somSelf, szCarClassTitle);

    return (parent_wpRestoreState(somSelf,ulReserved));

}   /* carpp_wpRestoreState() */

/*
 *
 *  METHOD: wpAddSettingsPages                             ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Add our own settings page to let the user alter the pitch
 *    and duration of the car's beep.
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpAddSettingsPages(Carpp *somSelf, HWND hwndNotebook)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpAddSettingsPages");

    if (parent_wpAddSettingsPages(somSelf, hwndNotebook)
             && somSelf->AddHornBeepPage( hwndNotebook)
             && somSelf->AddDashboardPage( hwndNotebook))
    {
        return(TRUE);
    }
    else
    {
        DebugBox("carpp_wpAddSettingsPages", " Failed to add a settings page.");
        return( FALSE );
    }

}   /* carpp_wpAddSettingsPages() */

/*
 *
 *  METHOD: wpFilterPopupMenu                              ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Filter out any options from the context that don't apply.
 *
 *  HOW TO OVERRIDE:
 *
 *    No restrictions.
 *
 */

SOM_Scope ULONG SOMLINK carpp_wpFilterPopupMenu(Carpp *somSelf,
                ULONG ulFlags,
                HWND hwndCnr,
                BOOL fMultiSelect)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpFilterPopupMenu");

    /*
     *   This method allows you to filter which menus to include in the
     *   popup menu.  Note: wpclsQueryStyle is overridden to disallow
     *   linking (creating shadow) as well.
     */
    return( parent_wpFilterPopupMenu(somSelf,ulFlags,hwndCnr,
                    fMultiSelect) & ~CTXT_LINK );

}   /* end carpp_wpFilterPopupMenu() */

/*
 *
 *  METHOD: wpModifyPopupMenu                              ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Add our extra option to the context menu to beep the horn
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpModifyPopupMenu(Carpp *somSelf,
                HWND hwndMenu,
                HWND hwndCnr,
                ULONG iPosition)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpModifyPopupMenu");

    /*
     *   Put in our special "open car" submenu item under the "open" menu
     */
    somSelf->wpInsertPopupMenuItems( hwndMenu, 0,
                               hmod, ID_OPENMENU, WPMENUID_OPEN);
    /*
     *   insert a "beep horn" menu item at the end of the list.
     */
    somSelf->wpInsertPopupMenuItems( hwndMenu, iPosition,
                               hmod, ID_BEEPMENU, 0 );

    /*
     *   insert a "TRAP-D" menu item at the end of the list.
     */
    somSelf->wpInsertPopupMenuItems( hwndMenu, 0,
                               hmod, ID_TRAPMENU, 0 );

    return (parent_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition));

}   /* end carpp_wpModifyPopupMenu() */

/*
 *
 *  METHOD: wpMenuItemSelected                             ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpMenuItemSelected(Carpp *somSelf,
                HWND hwndFrame,
                ULONG MenuId)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpMenuItemSelected");

    /* Which of our menu items was selected ?
     */
    switch( MenuId )
    {
       case IDM_OPENCAR:
          /*
           *   We could call wpOpen here, but, if the object is already opened,
           *   the following API determines whether the object should be
           *   resurfaced, or if multiple views are desired.
           */
          somSelf->wpViewObject( NULLHANDLE, OPEN_CAR, 0);
          break;

       case IDM_BEEPHORN:
          somThis->carHorn->Beep();
          break;

       case IDM_TRAPCAR:
          somSelf->TrapTest();
          break;

       default:
          return parent_wpMenuItemSelected(somSelf, hwndFrame, MenuId);
          break;
    }
    return TRUE;                                          /* we processed it */

}   /* end carpp_wpMenuItemSelected() */

/*
 *
 *  METHOD: wpMenuItemHelpSelected                         ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */

SOM_Scope BOOL SOMLINK carpp_wpMenuItemHelpSelected(Carpp *somSelf,ULONG MenuId)
{
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpMenuItemHelpSelected");

    /* Which of our menu items was selected ?
     */
   switch( MenuId )
   {
      case IDM_BEEPHORN:

         return(somSelf->wpDisplayHelp( ID_HELP_BEEPHORN, szHelpLibrary));
         break;

      case IDM_OPENCAR:

         return(somSelf->wpDisplayHelp( ID_HELP_OPENCAR, szHelpLibrary));
         break;

      case IDM_TRAPCAR:

         return(somSelf->wpDisplayHelp( ID_HELP_TRAPCAR, szHelpLibrary));
         break;

      default:
         break;
   }

   return FALSE;

}   /* end carpp_wpMenuItemHelpSelected() */

/*
 *
 *  METHOD: wpQueryDetailsData                             ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Returns the car specific data for the details view of this object.
 *    Sets the pointer (*ppDetailsData) to the beginning of the buffer
 *    into which the data is written.
 *
 */

SOM_Scope ULONG SOMLINK carpp_wpQueryDetailsData(Carpp *somSelf,
                PVOID *ppDetailsData,
                PULONG pcp)
{
   PCARDETAILS pCarDetails;
   PBYTE       pSize;
    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpQueryDetailsData");

   parent_wpQueryDetailsData(somSelf,ppDetailsData, pcp);

   if (ppDetailsData)                                          /* query data */
   {
      pCarDetails                  = (PCARDETAILS) *ppDetailsData;
      pCarDetails->pszMake         = "Toyota";               /* Manufacturer */
      pCarDetails->pszModel        = "Camry";                  /* Model name */
      pCarDetails->pszColor        = "BLUE";             /* Color of the car */
      pCarDetails->cdateSale.day   = 24;                     /* Date of sale */
      pCarDetails->cdateSale.month = 12;
      pCarDetails->cdateSale.year  = 91;
      pCarDetails->ulPrice         = 14000;              /* Price in dollars */

      /* point to buffer location after our details data */
      *ppDetailsData = ((PBYTE) (*ppDetailsData)) + sizeof(*pCarDetails);

   }   /* end if (ppDetailsData) */
   else                                                /* query size of data */
   {
      *pcp += sizeof(*pCarDetails);     /* caller is querying size of buffer */
   }

   return(TRUE);

}   /* end carpp_wpQueryDetailsData() */

/*
 *
 *   METHOD: wpOpen                                         ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Opens the car window.
 *
 */

SOM_Scope HWND SOMLINK carpp_wpOpen(Carpp *somSelf,
                HWND hwndCnr,
                ULONG ulView,
                ULONG param)
{

 HWND hwnd = NULLHANDLE;
 CarppData *somThis = CarppGetData(somSelf);
 CarppMethodDebug("Carpp","carpp_wpOpen");

   switch (ulView)
   {
      case OPEN_CAR:

         if (!(somSelf->wpSwitchTo( ulView )))
         {
            /*
             *   Create a basic Frame and Client window for this instance.
             */
            hwnd = CarInit(somSelf);
         }
         break;

      default:
         hwnd = parent_wpOpen(somSelf,hwndCnr,ulView,param);
         break;

   }   /* end switch (ulView) */

   return( hwnd );
}   /* end carpp_wpOpen() */


/*
 *
 *  METHOD: wpSetup                                        ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Specify Setup strings and do some initialization.  This method is
 *    invoked once an object is completely created.
 *
 *  Note:  We're overriding this method to write some default data to the
 *         object's real filename.  This will give us something to look at
 *         if the user drag/drops us on an editor or selects the open/editor
 *         view.
 */

SOM_Scope BOOL   SOMLINK carpp_wpSetup(Carpp *somSelf,
                PSZ pszSetupString)
{
 ULONG cbBytesWritten;           /* pointer to variable receiving byte count */
 APIRET rc;
 BOOL fSuccess;
 HFILE hf;                            /* pointer to variable for file handle */
 ULONG ulAction;                     /* pointer to variable for action taken */
 CHAR szObjectFilename[CCHMAXPATH];          /* buffer for wpQueryRealName() */
 ULONG cb  = sizeof(szObjectFilename);


    CarppData *somThis = CarppGetData(somSelf);
    CarppMethodDebug("Carpp","carpp_wpSetup");

   /*
    *   Write an initial bill-of-sale to the object data file
    */

   fSuccess =
   somSelf->wpQueryRealName(                 /* query full-pathname of object's file */
      szObjectFilename,                                     /* return buffer */
      &cb,                                                  /* sizeof buffer */
      TRUE);                            /* request fully qualified pathname? */

   if (fSuccess)
   {
      rc =
      DosOpen(szObjectFilename, &hf, &ulAction,
         0,                             /* file size if created or truncated */
         FILE_NORMAL,                                      /* file attribute */
         FILE_OPEN             /* action taken if file exists/does not exist */
            | FILE_CREATE,
         OPEN_ACCESS_READWRITE                          /* open mode of file */
            | OPEN_SHARE_DENYNONE,
         NULL);              /* pointer to structure for extended attributes */

      if (rc)
      {
       CHAR ach[10];

         _ltoa(rc, ach, 10);
         DebugBox("carpp_wpSetup:DosOpen failed rc =", ach);
      }
      else
      {
         DosWrite(hf,
                  (PVOID)pcszDefaultText,
                  strlen(pcszDefaultText),
                  &cbBytesWritten);
         DosClose(hf);
      }
   }   /* end if (fSuccess) */

   return (parent_wpSetup(somSelf,pszSetupString));

}   /* end carpp_wpSetup() */

/*
 *
 *  METHOD: TrapTest                                        ( ) PRIVATE
 *                                                          (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Specify Setup strings and do some initialization.  This method is
 *    invoked once an object is completely created.
 *
 *  Note:  The variable used to communicate the current state to the
 *         exception handler is global.  If an exception occurs in more than
 *         one instance of this object at the same time, the results will
 *         be unpredictable.
 */

#pragma handler (carpp_TrapTest)
/*
 *
 * METHOD: TrapTest                                       ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   This method tests the exception handling in this sample.
 *
 * RETURN:
 *
 *   nothing
 *
 * HOW TO OVERRIDE:
 *
 *   n/a
 *
 *
 *   Methods from the WPObject class
 */

SOM_Scope void  SOMLINK carpp_TrapTest(Carpp *somSelf)
{
  PSZ      pszTrap = NULL;
  jmp_buf  saveState;
  CarppData *somThis = CarppGetData(somSelf);
  CarppMethodDebug("Carpp","carpp_TrapTest");

   memcpy(saveState, jmpState, sizeof(jmpState));
   if (!setjmp(jmpState))   /* no exception?*/
   {
      /*
       *   Normal code for this method goes here
       */

      *pszTrap = 'a';
   }
   else   /* we just returned from exception handler */
   {
      /*
       *   An exception just occurred in this method.  We should put any
       *   cleanup/recovery code in this code block and then return the
       *   error to the caller.
       */

      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, pcszTrapMessage,
                      "Exception Handler Test",
                      999, MB_MOVEABLE | MB_OK | MB_ERROR);
   }

   memcpy(jmpState, saveState, sizeof(jmpState));

}   /* end carpp_TrapTest() */

/**************************  CLASS METHODS SECTION  ***************************
*****                                                                     *****
*****              Do not put any code in this section unless             *****
*****                     it is an object CLASS method                    *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta


/*
 *
 *  METHOD: clsQueryModuleHandle                           ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    This method returns the module handle of this class.  If this is the
 *    first invocation, DosQueryModuleHandle is called to save the handle
 *    for future invocations.
 *
 *  RETURN:
 *
 *    0              Unsuccessful
 *    non-zero       module handle
 *
 */

/*
 * METHOD: clsQueryModuleHandle                           ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   This method returns the module handle of this class.  If this is the
 *   first invocation, DosQueryModuleHandle is called to save the handle
 *   for future invocations.
 *
 * RETURN:
 *
 *   0              Unsuccessful
 *   non-zero       module handle
 */

SOM_Scope HMODULE   SOMLINK carppM_clsQueryModuleHandle(M_Carpp *somSelf)
{
 APIRET rc;

    /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_clsQueryModuleHandle");

   /*
    *   Make sure we already have module handle
    */


    if (hmod == NULLHANDLE)
    {
     zString zsPathName;
       /*
        *   Retrieve registered pathname of our module (DLL) and query the
        *   module handle.
        */
       zsPathName =
       (SOMClassMgrObject->somLocateClassFile(  SOM_IdFromString("Carpp"),
                                              Carpp_MajorVersion, Carpp_MinorVersion));
       rc =
       DosQueryModuleHandle( zsPathName, &hmod);
       if (rc)
       {
          DebugBox("carppM_wpclsInitData", "Failed to load module");
          return 0L;
       }

    }   /* end if (hmod == NULLHANDLE) */

    return (hmod);

}   /* end carppM_clsQueryModuleHandle() */

/*
 *
 *  METHOD: wpclsQueryStyle                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *     The wpclsQueryStyle method is called to allow the class object
 *     to specify the default object class style for its instances.
 *
 *  REMARKS:
 *
 *     This method can be called at any time in order to determine the
 *     default style for instances of this class.
 *
 *     This method should be overridden in order to modify the default
 *     object style for instances of this class.
 *
 */

SOM_Scope ULONG   SOMLINK carppM_wpclsQueryStyle(M_Carpp *somSelf)
{
    /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryStyle");

    /*
     *   Modify style bits as described in programming reference.  This
     *   particular style (link) is also disabled in wpFilterPopupMenu()
     */
    return (parent_wpclsQueryStyle(somSelf) | CLSSTYLE_NEVERLINK);
}

/*
 *
 *  METHOD: wpclsInitData                                  ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Initialize the class data
 *
 */

SOM_Scope void  SOMLINK carppM_wpclsInitData(M_Carpp *somSelf)
{
 ULONG rc, i;
 PCLASSFIELDINFO pCFI;

    /* M_CarppData *somThis  = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsInitData");

    /*
     *   Call the parent class method first
     */
    parent_wpclsInitData(somSelf);

    /*
     *   Get class title
     */
    if (!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP), somSelf->clsQueryModuleHandle(), ID_TITLE,
                         sizeof(szCarClassTitle), szCarClassTitle))
                              /* Load string failed: use the parent's string */
       strcpy(szCarClassTitle, parent_wpclsQueryTitle(somSelf));

   /*
    *   Initialize everything needed for the CLASSFIELDINFO structures
    *   for the Car object class
    */

   for (i=0, pCFI=fieldinfo; i < NUM_CAR_FIELDS; i++, pCFI++)
   {
      memset((PCH) pCFI, 0, sizeof(CLASSFIELDINFO));               /* zero's */

      pCFI->cb        = sizeof(CLASSFIELDINFO);
      pCFI->flData    = CFA_RIGHT | CFA_SEPARATOR | CFA_FIREADONLY;
      pCFI->flTitle   = CFA_CENTER | CFA_SEPARATOR | CFA_HORZSEPARATOR |
                        CFA_STRING | CFA_FITITLEREADONLY;
      pCFI->pNextFieldInfo = pCFI + 1;       /* point to next CLASSFIELDINFO */
      pCFI->pTitleData = (PVOID) apszCarColTitles[i];
      pCFI->flCompare  = COMPARE_SUPPORTED | SORTBY_SUPPORTED;


/*******************************************************/
/* KLUDGE - w/o the ULONG typecast there is an error */
/******************************************************* */
/* Calculate the byte offset of a field in a structure of type type. */
#define LFIELDOFFSET(type, field)    ((ULONG)&(((type *)0)->field))

      switch (i)
      {
         case INDEX_MAKE:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(LFIELDOFFSET(CARDETAILS,pszMake));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_MODEL:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(LFIELDOFFSET(CARDETAILS,pszModel));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_COLOR:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(LFIELDOFFSET(CARDETAILS,pszColor));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_SALE_DATE:

            pCFI->flData           |= CFA_DATE;
            pCFI->offFieldData      = (ULONG)(LFIELDOFFSET(CARDETAILS,cdateSale));
            pCFI->ulLenFieldData    = sizeof(CDATE);
            pCFI->ulLenCompareValue = sizeof(CDATE);
            pCFI->DefaultComparison = CMP_GREATER;

            break;

         case INDEX_PRICE:

            pCFI->flData           |= CFA_ULONG;
            pCFI->offFieldData      = (ULONG)(LFIELDOFFSET(CARDETAILS,ulPrice));
            pCFI->ulLenFieldData    = sizeof(ULONG);
            pCFI->ulLenCompareValue = sizeof(ULONG);
            pCFI->DefaultComparison = CMP_GREATER;

            break;

      }   /* end switch(i) */

   }   /* end for (i=0, pCFI=fieldinfo; i < NUM_CAR_FIELDS; i++, pCFI++)  */

   fieldinfo[NUM_CAR_FIELDS-1].pNextFieldInfo = NULL;/* terminate linked list */

}   /* end carppM_ wpclsInitData() */

/*
 *
 *  METHOD: wpclsUnInitData                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Free any class data
 *
 */

SOM_Scope void SOMLINK carppM_wpclsUnInitData(M_Carpp *somSelf)
{
    M_CarppMethodDebug("M_Carpp","carppM_wpclsUnInitData");

    parent_wpclsUnInitData(somSelf);

}   /* end carppM_wpclsUnInitData() */

/*
 *
 *  METHOD: wpclsQueryTitle                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Return the string "New car".
 *
 */

SOM_Scope PSZ SOMLINK carppM_wpclsQueryTitle(M_Carpp *somSelf)
{
                              /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryTitle");

    /*
     *   Return the class title for a car
     */

    if (*szCarClassTitle )
       return( szCarClassTitle );
    else
       return( parent_wpclsQueryTitle(somSelf));

}   /* end carppM_wpclsQueryTitle() */

/*
 *
 *  METHOD: wpclsQueryIconDdata                            ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Return the class icon
 *
 */

SOM_Scope ULONG SOMLINK carppM_wpclsQueryIconData(M_Carpp *somSelf,
                  PICONINFO pIconInfo)
{
   APIRET rc;

   M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryIconData");

   if (pIconInfo)
   {
      /*
       *   fill in icon information
       */
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod= somSelf->clsQueryModuleHandle();
      pIconInfo->resid= ID_ICON;
   }

   return (sizeof(ICONINFO));

}   /* end carppM_wpclsQueryIconData() */

/*
 *
 *  METHOD: wpclsQueryDefaultHelp                          ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */

SOM_Scope BOOL SOMLINK carppM_wpclsQueryDefaultHelp(M_Carpp *somSelf,
                PULONG pHelpPanelId,
                PSZ pszHelpLibrary)
{
                              /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryDefaultHelp");

    if (pHelpPanelId)                           /* set default help panel id */
       *pHelpPanelId   = ID_HELP_DEFAULT;

    if (pszHelpLibrary)                                /* copy help filename */
       strcpy(pszHelpLibrary, szHelpLibrary);

    return (TRUE);

}   /* end carppM_wpclsQueryDefaultHelp() */

/*
 *
 *  METHOD: wpclsQueryDefaultView                          ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Returns the default view for a new instance of this object.
 *
 *  REMARKS:
 *
 *    Tell the system what our default open view is...
 *
 */

SOM_Scope ULONG SOMLINK carppM_wpclsQueryDefaultView(M_Carpp *somSelf)
{
                              /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryDefaultView");

    return OPEN_CAR;

}   /* end carppM_wpclsQueryDefaultView() */

/*
 *
 *  METHOD: wpclsQueryDetailsInfo                          ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Appends the car specific chain of FIELDINFO structures describing the
 *    details data of this object to *ppClassFieldInfo (if ppClassFieldInfo
 *    is NON-NULL).  In this case it also sets *ppClassFieldInfo to the
 *    head of the linked list.
 *
 *    Adds the number of bytes required by the details data for car to *pSize
 *    (if pSize is NON-NULL).
 *
 *  REMARKS:
 *
 *    Add details data for this object.
 *
 */

SOM_Scope ULONG SOMLINK carppM_wpclsQueryDetailsInfo(M_Carpp *somSelf,
                PCLASSFIELDINFO *ppClassFieldInfo,
                PULONG pSize)
{
 ULONG           cParentColumns;
 PCLASSFIELDINFO pCFI;
 ULONG           i;

    /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryDetailsInfo");

   /*
    *   Always call the parent method first to retrieve number of details
    *   columns and parent's data already defined in details buffer.
    */

   cParentColumns =
   parent_wpclsQueryDetailsInfo(somSelf, ppClassFieldInfo, pSize);

   /*
    *   If pSize is non-NULL, we must add the size of our deatils column
    *   data structure.
    */

   if (pSize)
      *pSize += sizeof(CARDETAILS);                           /* adjust size */

   /*
    *   If the request was for the chained fieldinfo structures
    *   (ppClassFieldInfo is non-NULL), link them in
    *
    *   eventually the chain will look like
    *
    *   Grandad - Dad - Me - Kid - Grandkid
    *
    *   I will be getting the pointer to the beginning of the chain
    *
    *   If the beginning of the chain is 0, I will assign the address
    *   of my first CLASSFIELDINFO structure to *ppClassFieldInfo.
    *   Otherwise *pp points to the first column description in the
    *   chain.  We need to walk the chain and link our CLASSFIELDINFO
    *   structures at the end.
    */

   if (ppClassFieldInfo)
   {
      /*
       *   Find the last link in the chain;  Then link our CLASSFIELDINFO
       *   structures to the chain.
       */

      if (*ppClassFieldInfo)
      {
         pCFI = *ppClassFieldInfo;
         for (i=0;i<cParentColumns;i++)
            pCFI = (pCFI->pNextFieldInfo) ? pCFI->pNextFieldInfo : pCFI;

         pCFI->pNextFieldInfo = fieldinfo;
      }
      else
         *ppClassFieldInfo = fieldinfo;
   }
   return ((ULONG) (cParentColumns + NUM_CAR_FIELDS));

}   /* end carppM_wpclsQueryDetailsInfo() */

/*
 *
 *  METHOD: wpclsQueryInstanceFilter                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *     The wpclsQueryInstanceFilter method is called to allow the class
 *     object to specify the file title filters for instances of its
 *     class.
 *
 *  REMARKS:
 *
 *     A pointer to a string containing file title filter(s).  This
 *     string can contain several file title filters separated by a
 *     comma.  Example: "*.TXT, *.DOC"
 *
 *  Note:  Overriding this method will cause any data file with the extension
 *         ".CAR" to become a data object of the class "Car."
 *
 */

SOM_Scope PSZ   SOMLINK carppM_wpclsQueryInstanceFilter(M_Carpp *somSelf)
{
    /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryInstanceFilter");

    return (szCarInstanceFilter);
}


/*
 *
 *  METHOD: wpclsQueryDetails                              ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *     Specify default details to display in details view
 *
 *  REMARKS:
 *
 *     A pointer details structure is modified and returned.
 *     Note: this is purely virtual at the moment (parent class doesn't
 *     do anything).
 */

SOM_Scope PCLASSDETAILS   SOMLINK carppM_wpclsQueryDetails(M_Carpp *somSelf)
{
    /* M_CarppData *somThis = M_CarppGetData(somSelf); */
    M_CarppMethodDebug("M_Carpp","carppM_wpclsQueryDetails");

    return (parent_wpclsQueryDetails(somSelf));
}


/**************************  ORDINARY CODE SECTION  ***************************
*****                                                                     *****
*****                  Any non-method code should go here.                *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass


/***************************************************************************
*                                                                          *
*       ROUTINE:    CarInit ( )                                            *
*                                                                          *
*       DESCRIPTION:    Car Initialization                                 *
*                                                                          *
*       RETURNS:    Handle of car frame window, NULL if error              *
*                                                                          *
***************************************************************************/
HWND CarInit (Carpp* somSelf)
{
   HAB  hab;                                       /* PM anchor block handle */
   HWND hwndFrame = NULLHANDLE;                       /* Frame window handle */
   HWND hwndClient = NULLHANDLE;
   PWINDOWDATA pWindowData;
   BOOL fSuccess;
   SWCNTRL    swcEntry;                                      /* Switch Entry */
   FRAMECDATA flFrameCtlData;                              /* Frame Ctl Data */

   hab = WinQueryAnchorBlock(HWND_DESKTOP);
   if (!WinRegisterClass( hab , szCarWindowClass, (PFNWP)CarWndProc ,
                            CS_SIZEREDRAW | CS_SYNCPAINT, sizeof(pWindowData)))
   {
      DebugBox("CarInit", "Failure in WinRegisterClass");
      return NULLHANDLE ;
   }

   /*
    *   Allocate some instance specific data in Window words of Frame window.
    *   This will ensure our window procedure can use this object's methods
    *   (our window proc isn't passed a * somSelf pointer).
    */
   pWindowData = (PWINDOWDATA) somSelf->wpAllocMem( sizeof(*pWindowData), NULL);

   if (!pWindowData)
   {
      DebugBox("CarInit", "wpAllocMem failed to allocate pWindowData");
      return NULLHANDLE;
   }

   memset((PVOID) pWindowData, 0, sizeof(*pWindowData));
   pWindowData->cb = sizeof(*pWindowData);             /* first field = size */
   pWindowData->somSelf = somSelf;

   /* Create a frame window
    */
   flFrameCtlData.cb            = sizeof( flFrameCtlData );
   flFrameCtlData.flCreateFlags = FCF_SIZEBORDER | FCF_TITLEBAR | FCF_SYSMENU |
                                  FCF_MINMAX ;
   flFrameCtlData.hmodResources = hmod;
   flFrameCtlData.idResources   = ID_ICON;

   hwndFrame =                                        /* create frame window */
   WinCreateWindow(
      HWND_DESKTOP,               /* parent-window handle                    */
      WC_FRAME,                   /* pointer to registered class name        */
      somSelf->wpQueryTitle(),     /* pointer to window text                  */
      0,                          /* window style                            */
      0, 0, 0, 0,                 /* position of window                      */
      NULLHANDLE,                 /* owner-window handle                     */
      HWND_TOP,                   /* handle to sibling window                */
      (USHORT) ID_FRAME,          /* window identifier                       */
      (PVOID) &flFrameCtlData,    /* pointer to buffer                       */
      NULL);      ;               /* pointer to structure with pres. params. */

   if (!hwndFrame)
   {
      DebugBox("CarInit", "Failure in WinCreateWindow");
      return NULLHANDLE;
   }
   hwndClient =         /* use WinCreateWindow so we can pass pres params */
   WinCreateWindow(
      hwndFrame,               /* parent-window handle                    */
      szCarWindowClass,        /* pointer to registered class name        */
      NULL,                    /* pointer to window text                  */
      0,                       /* window style                            */
      0, 0, 0, 0,              /* position of window                      */
      hwndFrame,               /* owner-window handle                     */
      HWND_TOP,                /* handle to sibling window                */
      (USHORT)FID_CLIENT,      /* window identifier                       */
      pWindowData,             /* pointer to buffer                       */
      NULL);                   /* pointer to structure with pres. params. */

   if (!hwndClient)
   {
      WinDestroyWindow(hwndFrame);
      return NULLHANDLE;
   }

   WinSendMsg(hwndFrame, WM_SETICON, MPFROMP(somSelf->wpQueryIcon()), NULL);
   WinSetWindowText(WinWindowFromID(hwndFrame,(USHORT)FID_TITLEBAR),
                                                         somSelf->wpQueryTitle());

   /*
    * Restore the Window Position
    */
   fSuccess =
   WinRestoreWindowPos(
      szCarClassTitle,                                        /* class title */
      somSelf->wpQueryTitle(),                                /* object title */
      hwndFrame);

   if (!fSuccess)
   {
    SWP        swp;

      /* Get the dimensions and the shell's suggested
       * location for the window
       */
      WinQueryTaskSizePos(hab,0,&swp);

      /* Set the frame window position
       */
     swp.fl               = SWP_SIZE|SWP_MOVE|SWP_RESTORE|SWP_ZORDER;
     WinSetWindowPos(hwndFrame, HWND_TOP, swp.x, swp.y, swp.cx, swp.cy, swp.fl);
   }

   WinShowWindow(hwndFrame,TRUE);
   WinStartTimer(hab, hwndClient, CAR_TIMER, (ULONG)100);

   return hwndFrame;                                              /* success */

}   /* end CarInit() */


/******************************************************************************
*
*   CarWndProc()
*
*   DESCRIPTION: Car Window Procedure
*
******************************************************************************/
MRESULT EXPENTRY CarWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  ULONG    MenuId;
  PWINDOWDATA pWindowData;
  CarppData *somThis;
  HWND     hwndFrame;
  CHAR     acBuffer[10];
  BOOL     fSuccess;

   hwndFrame = WinQueryWindow(hwnd, QW_PARENT);

   switch( msg )
   {
      case WM_CREATE:

        pWindowData = (PWINDOWDATA) mp1;

        if (pWindowData == NULL)
        {
           DebugBox("CarWndProc:WM_CREATE", "couldn't get window words");
           return FALSE;
        }
        /*
         *   Fill in the class view/usage details and window specific data
         *   for this instance.
         */
        pWindowData->UseItem.type    = USAGE_OPENVIEW;
        pWindowData->ViewItem.view   = OPEN_CAR;
        pWindowData->ViewItem.handle = hwndFrame;
        pWindowData->x               = 10;
        pWindowData->y               = 10;
        pWindowData->xDir            = CAR_RIGHT;
        pWindowData->yDir            = CAR_UP;

        /*
         *   Set window pointer with object pointer and instance view info.
         *   Then add view to the in-use list so wpSwitchTo works.
         */
        WinSetWindowPtr(hwnd, QWL_USER, pWindowData);
        pWindowData->somSelf->wpAddToObjUseList( &pWindowData->UseItem);
        pWindowData->somSelf->wpRegisterView( hwndFrame,
                                             pWindowData->somSelf->wpQueryTitle());
        WinSetFocus( HWND_DESKTOP, hwndFrame);

        break;

      case WM_COMMAND:

        break;

      case WM_TIMER:

         pWindowData = (PWINDOWDATA) WinQueryWindowPtr(hwnd, QWL_USER);

         if (pWindowData == NULL)
         {
            DebugBox("CarWndProc:WM_TIMER", "couldn't get window words");
            return FALSE;
         }
         else
         {
          RECTL  rectl;
          somThis = CarppGetData(pWindowData->somSelf);

            /*
             *   If the car's brakes are off, we move the car by modifying it's
             *   x,y position.  Direction (xDir, yDir) changes when the car's
             *   position reaches a border of the window.  The distance it
             *   moves is based on the speed contained in an instance variable.
             */

            if (somThis->carBrake->IsSet() == FALSE)
            {
               WinQueryWindowRect(hwnd,&rectl);

               if (pWindowData->x <= 0)                   /* at left border? */
                  pWindowData->xDir = CAR_RIGHT;              /* mult. by  1 */
               else
                  if (pWindowData->x >= rectl.xRight - ICON_WIDTH)
                     pWindowData->xDir = CAR_LEFT;            /* mult. by -1 */

               if (pWindowData->y <= 0)                 /* at bottom border? */
                  pWindowData->yDir = CAR_UP;                 /* mult. by  1 */
               else
                  if (pWindowData->y >= rectl.yTop - ICON_HEIGHT)
                     pWindowData->yDir = CAR_DOWN;            /* mult. by -1 */

               pWindowData->x +=
                  pWindowData->xDir * (somThis->carRPM->Query());

               pWindowData->y +=
                  pWindowData->yDir * (somThis->carRPM->Query());

               WinInvalidateRect(hwnd, &rectl, TRUE);  /* invalidate car region */
            }
         }
         break;

      case WM_PAINT:
         pWindowData = (PWINDOWDATA) WinQueryWindowPtr(hwnd, QWL_USER);

         if (pWindowData == NULL)
         {
            DebugBox("CarWndProc:WM_PAINT", "couldn't get window words");
            return FALSE;
         }
         else
         {
          HPS    hps;
          RECTL  rectl;

           hps = WinBeginPaint( hwnd, (HPS)NULLHANDLE, &rectl);
           WinFillRect( hps, &rectl, SYSCLR_WINDOW);
           WinDrawPointer(hps, pWindowData->x, pWindowData->y,
                          pWindowData->somSelf->wpQueryIcon(), DP_NORMAL);
           WinEndPaint( hps );
         }
         break;

      case WM_CLOSE:
         {
          HAB hab;

            hab = WinQueryAnchorBlock(HWND_DESKTOP);
            WinStopTimer(hab,hwnd,CAR_TIMER);

            pWindowData = (PWINDOWDATA) WinQueryWindowPtr(hwnd, QWL_USER);

            if (pWindowData == NULL)
            {
               DebugBox("CarWndProc:WM_CLOSE", "couldn't get window words");
               return FALSE;
            }
            fSuccess =
            WinStoreWindowPos(szCarClassTitle,pWindowData->somSelf->wpQueryTitle(),
                                                                        hwndFrame);
            pWindowData->somSelf->wpDeleteFromObjUseList( &pWindowData->UseItem );
            pWindowData->somSelf->wpFreeMem( (PBYTE)pWindowData );

            WinPostMsg( hwnd, WM_QUIT, 0, 0 );
            WinDestroyWindow ( hwndFrame ) ;
         }
         break;

      default:
         return WinDefWindowProc( hwnd, msg, mp1, mp2 );
   }
   return FALSE;

}   /* end CarWndProc() */

/******************************************************************************
*
*   DashBoardDlgProc()
*
*   DESCRIPTION: Dialog Procedure for Dashboaard settings page
*
******************************************************************************/
MRESULT EXPENTRY DashBoardDlgProc(HWND hwndDlg, ULONG msg,
                                  MPARAM mp1, MPARAM mp2)
{
 DASHDLGDATA * pDashDlgData;
 CarppData *somThis;
 CHAR          acBuffer[10];

   switch (msg)
   {
      case WM_INITDLG:
      {
         USHORT    usCount;
         SLDCDATA  SliderData;
         WNDPARAMS wprm;

         /*
          *   Store some instance specific data in Window words of this dialog.
          *   This will ensure our dialog procedure can access this objects
          *   data (our dialog proc isn't always passed a *somSelf pointer).
          */
         pDashDlgData =
            (PDASHDLGDATA) (((Carpp *)mp2)->wpAllocMem(sizeof(*pDashDlgData), NULL));
         if (pDashDlgData)
         {
            memset((PVOID) pDashDlgData, 0, sizeof(*pDashDlgData));
            pDashDlgData->cb = sizeof(*pDashDlgData);
            pDashDlgData->somSelf = (Carpp *) mp2;   /* pointer to this object */
            somThis = CarppGetData(pDashDlgData->somSelf);
            pDashDlgData->PrevBrakes = somThis->carBrake->IsSet();
            pDashDlgData->PrevSpeed  = somThis->carRPM->Query();
         }
         else
         {
            DebugBox("DashboardDlgProc:WM_INITDLG", "Couldn't allocate window words");
            break;
         }

         WinSetWindowPtr(hwndDlg, QWL_USER, pDashDlgData);

         SliderData.cbSize = sizeof(SLDCDATA);
         SliderData.usScale1Increments = 10;
         SliderData.usScale1Spacing = 20;
         SliderData.usScale2Increments = 10;
         SliderData.usScale2Spacing = 20;

         wprm.fsStatus = WPM_CTLDATA;
         wprm.cchText = 0;
         wprm.cbPresParams = 0;
         wprm.cbCtlData = 0;
         wprm.pCtlData = &SliderData;
         WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,
                          WM_SETWINDOWPARAMS,(MPARAM)&wprm,(MPARAM)NULL ) ;

         for (usCount = 0; usCount < 10; usCount++ )
         {
            WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,
                SLM_SETTICKSIZE,MPFROM2SHORT(usCount,5),NULL);

           _itoa((usCount*10),acBuffer,10);
            WinSendDlgItemMsg(hwndDlg, ID_SPEEDSLIDER, SLM_SETSCALETEXT,
               MPFROMSHORT(usCount), MPFROMP(acBuffer));
         }
         WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
            MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,NULL),
            MPFROM2SHORT(20,40));

         WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
            MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
            MPFROMSHORT((SHORT)(somThis->carRPM->Query())/10));

         WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
            _ltoa(somThis->carRPM->Query(),acBuffer,10));

         if (somThis->carBrake->IsSet() == FALSE)
         {
            WinSendDlgItemMsg(hwndDlg,ID_GO,BM_SETCHECK,(MPARAM)1L, MPVOID);
         }
         else
         {
            WinSendDlgItemMsg(hwndDlg,ID_STOP,BM_SETCHECK,(MPARAM)1L, MPVOID);
         }

         return (MRESULT) TRUE;

      }   /* end case WM_INITDLG: */

      case WM_DESTROY:

         pDashDlgData = (PDASHDLGDATA)WinQueryWindowPtr(hwndDlg, QWL_USER);
         if (pDashDlgData == NULL)
         {
            DebugBox("DashboardDlgProc:WM_DESTROY", "couldn't get window words");
            break;
         }
         pDashDlgData->somSelf->wpFreeMem((PBYTE)pDashDlgData);
         return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

      case WM_COMMAND:

         pDashDlgData = (PDASHDLGDATA)WinQueryWindowPtr(hwndDlg, QWL_USER);
         somThis = CarppGetData(pDashDlgData->somSelf);
         if (pDashDlgData == NULL)
         {
            DebugBox("DashboardDlgProc:WM_COMMAND", "couldn't get window words");
            break;
         }

         switch (SHORT1FROMMP(mp1))
         {
            case ID_UNDO:
               if (pDashDlgData->PrevBrakes == TRUE)
               {
                 somThis->carBrake->Set();
               }
               else
               {
                 somThis->carBrake->Release();
               }

               somThis->carRPM->Set( pDashDlgData->PrevSpeed);

               WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
                  MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                  MPFROMSHORT((SHORT)(somThis->carRPM->Query()/10)));

               WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                  _ltoa(somThis->carRPM->Query(),acBuffer,10));

               if (somThis->carBrake->IsSet() == FALSE)
               {
                  WinSendDlgItemMsg(hwndDlg,ID_GO,BM_SETCHECK,
                     (MPARAM)1L, MPVOID);
               }
               else
               {
                  WinSendDlgItemMsg(hwndDlg,ID_STOP,BM_SETCHECK,
                     (MPARAM)1L, MPVOID);
               }

               break;

            case ID_DEFAULT:
               /*
                *   preserve previous values
                */
               pDashDlgData->PrevBrakes = somThis->carBrake->IsSet();
               pDashDlgData->PrevSpeed  = somThis->carRPM->Query();

               somThis->carBrake->SetToDefault();
               somThis->carRPM->SetToDefault();

               WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
                  MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                  MPFROMSHORT((SHORT)(somThis->carRPM->Query()/10)));

               WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                  _ltoa(somThis->carRPM->Query(), acBuffer,10));

               WinSendDlgItemMsg(hwndDlg,ID_GO,BM_SETCHECK,
                  (MPARAM)1L, MPVOID);

               break;

            case ID_HELP:

               break;

         }   /* end switch (SHORT1FROMMP(mp1)) */

         return (MRESULT) TRUE;

      case WM_CONTROL:
      {
         ULONG temp;

         pDashDlgData = (PDASHDLGDATA)WinQueryWindowPtr(hwndDlg, QWL_USER);
         somThis = CarppGetData(pDashDlgData->somSelf);

         if (pDashDlgData == NULL)
         {
            DebugBox("DashboardDlgProc:WM_CONTROL", "couldn't get window words");
            break;
         }

         switch (SHORT1FROMMP(mp1))
         {
            case ID_GO:
               if ((USHORT) SHORT2FROMMP(mp1) == BN_CLICKED)
               {
                  pDashDlgData->PrevBrakes = somThis->carBrake->IsSet();
                  somThis->carBrake->Release();
               }
               break;

            case ID_STOP:
               if ((USHORT) SHORT2FROMMP(mp1) == BN_CLICKED)
               {
                  pDashDlgData->PrevBrakes = somThis->carBrake->IsSet();
                  somThis->carBrake->Set();
               }
               break;

            case ID_SPEEDSLIDER:
               if ((USHORT) SHORT2FROMMP(mp1) == SLN_CHANGE)
               {
                  pDashDlgData->PrevSpeed  = somThis->carRPM->Query();
                  temp = (ULONG) WinSendDlgItemMsg(hwndDlg, ID_SPEEDSLIDER,
                     SLM_QUERYSLIDERINFO,
                     MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                     NULL);
                  temp = temp * 10;
                  WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                                   _ltoa(temp,acBuffer,10));
                  somThis->carRPM->Set( temp);
               }
               break;

         }   /* end switch (SHORT1FROMMP(mp1)) */

         return (MRESULT) TRUE;
         break;

      }   /* end case WM_CONTROL: */

   }   /* end switch (msg) */

   return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

}   /* end DashBoardDlgProc() */

/******************************************************************************
*
*   HornBeepDlgProc()
*
*   DESCRIPTION:  Dialog Procedure for Horn Beep settings page
*
******************************************************************************/
MRESULT EXPENTRY HornBeepDlgProc(HWND hwndDlg, ULONG msg,
                                  MPARAM mp1, MPARAM mp2)
{
 HORNDLGDATA * pHornDlgData;
 CarppData *somThis;
 CHAR acBuffer[10];

   switch (msg)
   {
      case WM_INITDLG:
      {
         /*
          *   Store some instance specific data in Window words of this dialog.
          *   This will ensure our dialog procedure can access this objects
          *   data (our dialog proc isn't always passed a *somSelf pointer).
          */
         pHornDlgData =
            (PHORNDLGDATA) (((Carpp *)mp2)->wpAllocMem(sizeof(*pHornDlgData), NULL));
         if (pHornDlgData)
         {
            memset((PVOID) pHornDlgData, 0, sizeof(*pHornDlgData));
            pHornDlgData->cb = sizeof(*pHornDlgData);
            pHornDlgData->somSelf = (Carpp *) mp2;  /* pointer to this object */
            somThis = CarppGetData(pHornDlgData->somSelf);
            pHornDlgData->PrevDuration = somThis->carHorn->QueryDuration();
            pHornDlgData->PrevHighTone = somThis->carHorn->QueryHighTone();
            pHornDlgData->PrevLowTone  = somThis->carHorn->QueryLowTone();
         }
         else
         {
            DebugBox("HornBeepDlgProc", "Couldn't allocate window words");
            break;
         }

         WinSetWindowPtr(hwndDlg, QWL_USER, pHornDlgData);

         WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_SETLIMITS,
            MPFROMLONG(1000), MPFROMLONG(0));

         WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_SETCURRENTVALUE,
            MPFROMLONG(pHornDlgData->PrevHighTone), MPFROMLONG(0));

         WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETLIMITS,
            MPFROMLONG(1000), MPFROMLONG(0));

         WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETCURRENTVALUE,
            MPFROMLONG(pHornDlgData->PrevLowTone), MPFROMLONG(0));

         return (MRESULT) TRUE;
         break;

      }   /* end case WM_INITDLG: */

      case WM_DESTROY:

         pHornDlgData = (PHORNDLGDATA) WinQueryWindowPtr(hwndDlg, QWL_USER);

         if (pHornDlgData == NULL)
         {
            DebugBox("HornBeepDlgProc", "Couldn't get window words");
            break;
         }

         pHornDlgData->somSelf->wpFreeMem((PBYTE)pHornDlgData);
         return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

      case WM_COMMAND:

         pHornDlgData = (PHORNDLGDATA) WinQueryWindowPtr(hwndDlg, QWL_USER);
         somThis = CarppGetData(pHornDlgData->somSelf);
         if (pHornDlgData == NULL)
         {
            DebugBox("HornBeepDlgProc", "Couldn't get window words");
            break;
         }

         switch (SHORT1FROMMP(mp1))
         {
            case ID_UNDO:

               WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(pHornDlgData->PrevHighTone),
                  MPFROMLONG(0));

               WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(pHornDlgData->PrevLowTone),
                  MPFROMLONG(0));

               break;

            case ID_DEFAULT:
               /*
                *   preserve previous values
                */
               pHornDlgData->PrevHighTone =
                  somThis->carHorn->QueryHighTone();
               pHornDlgData->PrevLowTone =
                  somThis->carHorn->QueryLowTone();

               somThis->carHorn->SetToDefault();

               WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(somThis->carHorn->QueryHighTone()), MPFROMLONG(0));

               WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(somThis->carHorn->QueryLowTone()), MPFROMLONG(0));
               break;

            case ID_HELP:
               break;

         }   /* end switch (SHORT1FROMMP(mp1)) */

         return (MRESULT) TRUE;
         break;

      case WM_CONTROL:
      {
       ULONG temp;

         pHornDlgData = (PHORNDLGDATA) WinQueryWindowPtr(hwndDlg, QWL_USER);
         somThis = CarppGetData(pHornDlgData->somSelf);

         if (pHornDlgData == NULL)
         {
            DebugBox("HornBeepDlgProc", "Couldn't get window words");
            break;
         }

         switch (SHORT2FROMMP(mp1))
         {
            /************************************************************** */
            /* When the value of either of the tones change, set the new values in the */
            /* car object after saving the current settings for "undo" purposes             */
            /***************************************************************/
            case SPBN_ENDSPIN:

               /*
                *   preserve previous values
                */
               pHornDlgData->PrevHighTone =
                  somThis->carHorn->QueryHighTone();
               pHornDlgData->PrevLowTone =
                  somThis->carHorn->QueryLowTone();

               WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_QUERYVALUE,
                  (MPARAM)&temp, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID));

               somThis->carHorn->SetHighTone( temp);

               WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_QUERYVALUE,
                  (MPARAM)&temp, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID));

               somThis->carHorn->SetLowTone( temp);

               break;

         }   /* end switch (SHORT2FROMMP(mp1)) */

      }   /* end case WM_CONTROL: */

      return (MRESULT) TRUE;
      break;

   }   /* end switch(msg) */

   return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

}   /* end HornBeepDlgProc() */

/******************************************************************************
*
*   _Exception()
*
*   DESCRIPTION:  Exception handler routine for this object.
*
*   PURPOSE: To notify user when an illegal memory access is made
*
*   METHOD:  Whenever a memory protection exception occurs, a message
*            box is put on the screen to inform the user.
*
*   RETURNS: Returns HANDLED if memory exception,
*            otherwise, returns NOT_HANDLED
*
******************************************************************************/
ULONG _Exception(EXCEPTIONREPORTRECORD       *parg,
                 EXCEPTIONREGISTRATIONRECORD *pRegisRecord,
                 PCONTEXTRECORD               pContextRecord,
                 PVOID                        pvSpare)
{
   LONG  rc;
   CHAR  szText[CCHMAXPATH];

   switch (parg->ExceptionNum)
   {
      case XCPT_ACCESS_VIOLATION:

         /*
          *   Notify the user when memory access violation occurs.
          */
         WinAlarm(HWND_DESKTOP, WA_ERROR);
         longjmp(jmpState, ERROR_PROTECTION_VIOLATION);
         break;

      case XCPT_PROCESS_TERMINATE:
      case XCPT_ASYNC_PROCESS_TERMINATE:
      case XCPT_SIGNAL:
      default:
         /*
          *   For more exceptions, see the header files or the programming
          *   reference(s).
          */
         break;

   }   /* end switch (parg->ExceptionNum) */

   return(XCPT_CONTINUE_SEARCH);

}   /* end _Exception() */

/********************************  END carpp.cpp  ********************************/


