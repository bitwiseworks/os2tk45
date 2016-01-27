
/******************************************************************************
*
*  Module Name: CAR.C
*
*  OS/2 Work Place Shell Sample Program
*
*  Copyright (C) 1992 IBM Corporation
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
*        carM_QueryModuleHandle
*
*     Overridden Class Methods:
*
*        carM_wpclsInitData
*        carM_wpclsQueryDefaultHelp
*        carM_wpclsQueryDefaultView
*        carM_wpclsQueryDetailsInfo
*        carM_wpclsQueryIconData
*        carM_wpclsQueryStyle
*        carM_wpclsQueryTitle
*        carM_wpclsUnInitData
*
*     Instance Methods:
*
*        car_AddDashboardPage
*        car_AddHornBeepPage
*        car_BeepHorn
*        car_QueryBrakes
*        car_QueryDuration
*        car_QueryHighTone
*        car_QueryLowTone
*        car_QuerySpeed
*        car_SetBrakes
*        car_SetDuration
*        car_SetHighTone
*        car_SetLowTone
*        car_SetSpeed
*
*     Overridden Instance Methods:
*
*        car_wpAddSettingsPages
*        car_wpFilterPopupMenu
*        car_wpInitData
*        car_wpMenuItemHelpSelected
*        car_wpMenuItemSelected
*        car_wpModifyPopupMenu
*        car_wpViewObject
*        car_wpQueryDetailsData
*        car_wpRestoreState
*        car_wpSaveState
*        car_wpUnInitData
*
*     Non-Method Functions:
*
*        CarInit
*        CarWndProc
*        DashBoardDlgProc
*        HornBeepDlgProc
*
******************************************************************************/

#define Car_Class_Source
#define M_Car_Class_Source

#include "car.ih"              /* implementation header emitted from car.idl */
#include "car.h"


#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <setjmp.h>

/*
 *   Non-Method function prototypes
 */

HWND             CarInit (Car*);
MRESULT EXPENTRY DashBoardDlgProc( HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY HornBeepDlgProc( HWND hwnd, ULONG msg,
                                   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY CarWndProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

#pragma linkage(_Exception, system)
ULONG _Exception(EXCEPTIONREPORTRECORD       *parg,
                 EXCEPTIONREGISTRATIONRECORD *pRegisRecord,
                 PCONTEXTRECORD               pContextRecord,
                 PVOID                        pvSpare);



/***************** GLOBAL/STATIC (NON-INSTANCE) DATA SECTION ******************
*****                                                                     *****
*****    This data shouldn't be changed by instance methods or it will    *****
*****    effect all instances!  Any variables that are specific (unique   *****
*****    values) for each instance of this object should be declared as   *****
*****  instance data or dynamically allocated and stored as window data.  *****
*****                                                                     *****
*****      This global data should be declared as class instance data     *****
*****    if it will change after initialization.  In this case, it will   *****
*****                  be accessed through class methods.                 *****
*****                                                                     *****
******************************************************************************/


   CHAR       szCarWindowClass[] =    "CARSAMPLE";
   CHAR       szCarInstanceFilter[] = "*.CAR";
   CHAR       szHelpLibrary[] =       "car.hlp";
   UCHAR      szCarClassTitle[CCHMAXPATH] = "";
   HMODULE    hmod = NULLHANDLE;

   PSZ        pszDefaultText ="\
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

   PSZ pszTrapMessage =                        "\
\
A Memory Access Violation occurred.  The Car \
sample's exception handler has transferred \
control back to the cleanup code in the method \
where the exception occurred.\n";

   /*
    *   Statics required for FIELDINFO structures needed for DETAILS view are
    *   handled in the three functions:
    *
    *   car_wpclsInitData, car_wpQueryDetailsData, car_wpQueryClassDetailsInfo
    */

   #define NUM_CAR_FIELDS    5

   CLASSFIELDINFO fieldinfo[NUM_CAR_FIELDS];      /* structure in wpobject.h */

   PSZ apszCarColTitles[] =
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
 *  METHOD: QueryBrakes                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Query state of car's brakes
 *
 *  RETURN:
 *
 *    TRUE           Brake is on
 *    FALSE          Brake is off
 *
 */

/*
 * METHOD: QueryBrakes                                    ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Query state of car's brakes
 *
 * RETURN:
 *
 *   TRUE           Brake is on
 *   FALSE          Brake is off
 */

SOM_Scope BOOL   SOMLINK car_QueryBrakes(Car *somSelf)
{
   CarData *somThis = CarGetData(somSelf);
   CarMethodDebug("Car","car_QueryBrakes");

   return _BrakeFlag;
}

/*
 *
 *  METHOD: SetBrakes                                      ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Put on the brakes
 *
 *  RETURN:
 *
 *    TRUE           state changed
 *    FALSE          state didn't change
 *
 */
/*
 * SOM_Scope VOID  SOMLINK car_SetBrakes(Car *somSelf, BOOL fBrake)
 */

/*
 * The prototype for car_SetBrakes was replaced by the following prototype:
 */
/*
 * METHOD: SetBrakes                                      ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Put on the brakes
 */

SOM_Scope void  SOMLINK car_SetBrakes(Car *somSelf, BOOL fBrake)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_SetBrakes");

    _BrakeFlag = fBrake;
}

/*
 *
 *  METHOD: QueryDuration                                  ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Query Duration of horn beep
 *
 *  RETURN:
 *
 *    ULONG          Duration of beep
 *
 */

/*
 * METHOD: QueryDuration                                  ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Query Duration of horn beep
 *
 * RETURN:
 *
 *   ULONG          Duration of beep
 */

SOM_Scope ULONG  SOMLINK car_QueryDuration(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_QueryDuration");

    return _duration;
}

/*
 *
 *  METHOD: SetDuration                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Set Duration of horn beep
 *
 *  RETURN:
 *
 *    TRUE           Duration changed
 *    FALSE          Duration didn't change
 *
 */


/*
 * SOM_Scope VOID  SOMLINK car_SetDuration(Car *somSelf, ULONG ulDuration)
 */

/*
 * The prototype for car_SetDuration was replaced by the following prototype:
 */
/*
 * METHOD: SetDuration                                    ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Set Duration of horn beep
 */

SOM_Scope void  SOMLINK car_SetDuration(Car *somSelf, ULONG ulDuration)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_SetDuration");

    _duration = ulDuration;
}

/*
 *
 *  METHOD: QueryHighTone                                  ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Query HighTone of horn
 *
 *  RETURN:
 *
 *    ULONG          High frequency of horn
 *
 */

/*
 * METHOD: QueryHighTone                                  ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Query HighTone of horn
 *
 * RETURN:
 *
 *   ULONG          High frequency of horn
 */

SOM_Scope ULONG  SOMLINK car_QueryHighTone(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_QueryHighTone");

    return _HighTone;
}

/*
 *
 *  METHOD: SetHighTone                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Set HighTone of horn
 *
 *  RETURN:
 *
 *    TRUE           Tone changed
 *    FALSE          Tone didn't change
 *
 */


/*
 * SOM_Scope VOID  SOMLINK car_SetHighTone(Car *somSelf, ULONG ulTone)
 */

/*
 * The prototype for car_SetHighTone was replaced by the following prototype:
 */
/*
 * METHOD: SetHighTone                                    ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Set HighTone of horn
 */

SOM_Scope void  SOMLINK car_SetHighTone(Car *somSelf, ULONG ulTone)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_SetHighTone");

    _HighTone = ulTone;
}

/*
 *
 *  METHOD: QueryLowTone                                  ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Query LowTone of horn
 *
 *  RETURN:
 *
 *    ULONG          High frequency of horn
 *
 */

/*
 * METHOD: QueryLowTone                                   ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Query LowTone of horn
 *
 * RETURN:
 *
 *   ULONG          High frequency of horn
 */

SOM_Scope ULONG  SOMLINK car_QueryLowTone(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_QueryLowTone");

    return _LowTone;
}

/*
 *
 *  METHOD: SetLowTone                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Set LowTone of horn
 *
 *  RETURN:
 *
 *    TRUE           Tone changed
 *    FALSE          Tone didn't change
 *
 */


/*
 * SOM_Scope VOID  SOMLINK car_SetLowTone(Car *somSelf, ULONG ulTone)
 */

/*
 * The prototype for car_SetLowTone was replaced by the following prototype:
 */
/*
 * METHOD: SetLowTone                                     ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Set LowTone of horn
 */

SOM_Scope void  SOMLINK car_SetLowTone(Car *somSelf, ULONG ulTone)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_SetLowTone");

    _LowTone = ulTone;
}

/*
 *
 *  METHOD: QuerySpeed                                     ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Query Speed of car
 *
 *  RETURN:
 *
 *    ULONG          car speed
 *
 */

/*
 * METHOD: QuerySpeed                                     ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Query Speed of car
 *
 * RETURN:
 *
 *   ULONG          car speed
 */

SOM_Scope ULONG  SOMLINK car_QuerySpeed(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_QuerySpeed");

    return _speed;
}

/*
 *
 *  METHOD: SetSpeed                                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Set Speed of car
 *
 *  RETURN:
 *
 *    TRUE           speed changed
 *    FALSE          speed didn't change
 *
 */


/*
 * SOM_Scope VOID  SOMLINK car_SetSpeed(Car *somSelf, ULONG ulSpeed)
 */

/*
 * The prototype for car_SetSpeed was replaced by the following prototype:
 */
/*
 * METHOD: SetSpeed                                       ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Set Speed of car
 */

SOM_Scope void  SOMLINK car_SetSpeed(Car *somSelf, ULONG ulSpeed)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_SetSpeed");

    _speed = ulSpeed;
}

/*
 *
 *  METHOD: BeepHorn                                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Beep the car's horn.
 *
 *  RETURN:
 *
 *    TRUE           Successful
 *    FALSE          Unsuccessful
 *
 */

/*
 * METHOD: BeepHorn                                       ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Beep the car's horn.
 *
 * RETURN:
 *
 *   TRUE           Successful
 *   FALSE          Unsuccessful
 */

SOM_Scope BOOL SOMLINK car_BeepHorn(Car *somSelf)
{
    USHORT i;

    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_BeepHorn");

    DosBeep(_QueryHighTone(somSelf), _QueryDuration(somSelf));
    DosSleep(100);
    DosBeep(_QueryLowTone(somSelf), _QueryDuration(somSelf));

    return TRUE;

}   /* end car_BeepHorn() */

/*
 *
 *  METHOD: AddDashboardPage                               ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    This method adds the dashboard page to the settings notebook.
 *
 *  RETURN:
 *
 *    0              Unsuccessful
 *    ulPageId       Identifier for the inserted page
 *
 *  HOW TO OVERRIDE:
 *
 *    Method should always be overridden in order to replace or remove
 *    the dashboard page from an object which is a descendent of Car.
 *    In most cases, an override of this method will not call the parent.
 *
 */

/*
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
 *   the dashboard page from an object which is a descendent of Car.
 *   In most cases, an override of this method will not call the parent.
 */

SOM_Scope ULONG SOMLINK car_AddDashboardPage(Car *somSelf, HWND hwndNotebook)
{
    PAGEINFO pageinfo;
    ULONG versionData[2], dlgId;

    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_AddDashboardPage");

    DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR, versionData, 8);
    if ( (versionData[0] <= 20 ) && (versionData[1] < 40) )
       dlgId = IDD_DASHBOARD;
    else
       dlgId = IDD_DASHBOARD2;

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

    return _wpInsertSettingsPage( somSelf, hwndNotebook, &pageinfo );

}   /* end car_AddDashboardPage() */


/*
 *
 *  METHOD: AddHornBeepPage                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    This method adds the horn beep page to the settings
 *    notebook.
 *
 *  RETURN:
 *
 *    0              Unsuccessful
 *    ulPageId       Identifier for the inserted page
 *
 *  HOW TO OVERRIDE:
 *
 *    Method should always be overridden in order to replace or remove
 *    the horn beep page from an object which is a descendent of Car.
 *    In most cases, an override of this method will not call the parent.
 *
 *
 *    Methods from the WPObject class
 *
 */

/*
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
 *   the horn beep page from an object which is a descendent of Car.
 *   In most cases, an override of this method will not call the parent.
 */

SOM_Scope ULONG SOMLINK car_AddHornBeepPage(Car *somSelf, HWND hwndNotebook)
{
    PAGEINFO pageinfo;
    ULONG versionData[2], dlgId;

    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_AddHornBeepPage");

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

    return _wpInsertSettingsPage( somSelf, hwndNotebook, &pageinfo );

}   /* end car_AddHornBeepPage() */

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

SOM_Scope void SOMLINK car_wpInitData(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpInitData");

    /*
     *   We can initialize our instance data to 0's by using the somThis
     *   pointer and the size of the CarData structure created by SOM.
     *
     *   SOM stores instance data in a data structure named by prefixing
     *   the name "Data" with the class name, in this case, "Car".
     */

    memset((PVOID)somThis, 0, sizeof(CarData));

    /*
     *   And/or we can explicitly initialize our instance variables.
     */

    _HighTone  = DEFAULT_HITONE;
    _LowTone   = DEFAULT_LOTONE;
    _duration  = DEFAULT_DURATION;
    _speed     = DEFAULT_SPEED;
    _BrakeFlag = DEFAULT_BRAKEFLAG;

    parent_wpInitData(somSelf);

}   /* end car_wpInitData() */

/*
 *
 *  OVERRIDE: wpUnInitData                                 ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Clear up memory that was allocated on wpInitData.
 *
 */

SOM_Scope void   SOMLINK car_wpUnInitData(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpUnInitData");

    parent_wpUnInitData(somSelf);

}   /* end car_wpUnInitData() */


/*
 *
 *  METHOD: wpSaveState                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Save our state variables (pitch and duration).
 *
 */

SOM_Scope BOOL SOMLINK car_wpSaveState(Car *somSelf)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpSaveState");

    _wpSaveLong( somSelf, szCarClassTitle, IDKEY_HITONE,          _HighTone  );
    _wpSaveLong( somSelf, szCarClassTitle, IDKEY_LOTONE,          _LowTone   );
    _wpSaveLong( somSelf, szCarClassTitle, IDKEY_DURATION,        _duration  );
    _wpSaveLong( somSelf, szCarClassTitle, IDKEY_SPEED,           _speed     );
    _wpSaveLong( somSelf, szCarClassTitle, IDKEY_BRAKEFLAG, (LONG)_BrakeFlag );

    return (parent_wpSaveState(somSelf));

}   /* end car_wpSaveState() */

/*
 *
 *  METHOD: wpRestoreState                                 ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Retrieve our saved state variables (pitch and duration).
 *
 */

SOM_Scope BOOL SOMLINK car_wpRestoreState(Car *somSelf, ULONG ulReserved)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpRestoreState");

    _wpRestoreLong( somSelf, szCarClassTitle, IDKEY_HITONE,    &_HighTone  );
    _wpRestoreLong( somSelf, szCarClassTitle, IDKEY_LOTONE,    &_LowTone   );
    _wpRestoreLong( somSelf, szCarClassTitle, IDKEY_DURATION,  &_duration  );
    _wpRestoreLong( somSelf, szCarClassTitle, IDKEY_SPEED,     &_speed     );
    _wpRestoreLong( somSelf, szCarClassTitle, IDKEY_BRAKEFLAG, &_BrakeFlag );

    return (parent_wpRestoreState(somSelf,ulReserved));

}   /* car_wpRestoreState() */

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

SOM_Scope BOOL SOMLINK car_wpAddSettingsPages(Car *somSelf, HWND hwndNotebook)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpAddSettingsPages");

    if (parent_wpAddSettingsPages(somSelf, hwndNotebook)
             && _AddHornBeepPage( somSelf, hwndNotebook)
             && _AddDashboardPage(somSelf, hwndNotebook))
    {
        return(TRUE);
    }
    else
    {
        DebugBox("car_wpAddSettingsPages", " Failed to add a settings page.");
        return( FALSE );
    }

}   /* car_wpAddSettingsPages() */

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

SOM_Scope ULONG SOMLINK car_wpFilterPopupMenu(Car *somSelf,
                ULONG ulFlags,
                HWND hwndCnr,
                BOOL fMultiSelect)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpFilterPopupMenu");

    /*
     *   This method allows you to filter which menus to include in the
     *   popup menu.  Note: wpclsQueryStyle is overridden to disallow
     *   linking (creating shadow) as well.
     */
    return( parent_wpFilterPopupMenu(somSelf,ulFlags,hwndCnr,
                    fMultiSelect) & ~CTXT_LINK );

}   /* end car_wpFilterPopupMenu() */

/*
 *
 *  METHOD: wpModifyPopupMenu                              ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Add our extra option to the context menu to beep the horn
 *
 */

SOM_Scope BOOL SOMLINK car_wpModifyPopupMenu(Car *somSelf,
                HWND hwndMenu,
                HWND hwndCnr,
                ULONG iPosition)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpModifyPopupMenu");

    /*
     *   Put in our special "open car" submenu item under the "open" menu
     */
    _wpInsertPopupMenuItems( somSelf, hwndMenu, 0,
                               hmod, ID_OPENMENU, WPMENUID_OPEN);
    /*
     *   insert a "beep horn" menu item at the end of the list.
     */
    _wpInsertPopupMenuItems( somSelf, hwndMenu, iPosition,
                               hmod, ID_BEEPMENU, 0 );

    /*
     *   insert a "TRAP-D" menu item at the end of the list.
     */
    _wpInsertPopupMenuItems( somSelf, hwndMenu, 0,
                               hmod, ID_TRAPMENU, 0 );

    return (parent_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition));

}   /* end car_wpModifyPopupMenu() */

/*
 *
 *  METHOD: wpMenuItemSelected                             ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */

SOM_Scope BOOL SOMLINK car_wpMenuItemSelected(Car *somSelf,
                HWND hwndFrame,
                ULONG MenuId)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpMenuItemSelected");

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
          _wpViewObject(somSelf, NULLHANDLE, OPEN_CAR, 0);
          break;

       case IDM_BEEPHORN:
          _BeepHorn(somSelf);
          break;

       case IDM_TRAPCAR:
          _TrapTest(somSelf);
          break;

       default:
          return parent_wpMenuItemSelected(somSelf, hwndFrame, MenuId);
          break;
    }
    return TRUE;                                          /* we processed it */

}   /* end car_wpMenuItemSelected() */

/*
 *
 *  METHOD: wpMenuItemHelpSelected                         ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */
SOM_Scope BOOL SOMLINK car_wpMenuItemHelpSelected(Car *somSelf,
                ULONG MenuId)
{
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpMenuItemHelpSelected");

    /* Which of our menu items was selected ?
     */
   switch( MenuId )
   {
      case IDM_BEEPHORN:

         return(_wpDisplayHelp(somSelf,ID_HELP_BEEPHORN,szHelpLibrary));
         break;

  /*  case ID_OPENCAR:    no help written at this time
   *
   *     return(_wpDisplayHelp(somSelf,ID_HELP_OPENCAR,szHelpLibrary));
   *     break;
   */
      default:
         break;
   }

   return FALSE;

}   /* end car_wpMenuItemHelpSelected() */

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
SOM_Scope ULONG SOMLINK car_wpQueryDetailsData(Car *somSelf,
                PVOID *ppDetailsData,
                PULONG pcp)
{
   PCARDETAILS pCarDetails;
   PBYTE       pSize;
    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpQueryDetailsData");

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

}   /* end car_wpQueryDetailsData() */

/*
 *
 *   METHOD: wpOpen                                         ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Opens the car window.
 *
 */
SOM_Scope HWND SOMLINK car_wpOpen(Car *somSelf,
                HWND hwndCnr,
                ULONG ulView,
                ULONG param)
{

 HWND hwnd = NULLHANDLE;
 CarData *somThis = CarGetData(somSelf);
 CarMethodDebug("Car","car_wpOpen");

   switch (ulView)
   {
      case OPEN_CAR:

         if (!_wpSwitchTo(somSelf, ulView))
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
   return(hwnd);
}   /* end car_wpOpen() */


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
SOM_Scope BOOL   SOMLINK car_wpSetup(Car *somSelf,
                PSZ pszSetupString)
{
 ULONG cbBytesWritten;           /* pointer to variable receiving byte count */
 APIRET rc;
 BOOL fSuccess;
 HFILE hf;                            /* pointer to variable for file handle */
 ULONG ulAction;                     /* pointer to variable for action taken */
 CHAR szObjectFilename[CCHMAXPATH];          /* buffer for wpQueryRealName() */
 ULONG cb  = sizeof(szObjectFilename);


    CarData *somThis = CarGetData(somSelf);
    CarMethodDebug("Car","car_wpSetup");

   /*
    *   Write an initial bill-of-sale to the object data file
    */

   fSuccess =
   _wpQueryRealName(                 /* query full-pathname of object's file */
      somSelf,                                     /* pointer to this object */
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
         DebugBox("car_wpSetup:DosOpen failed rc =", ach);
      }
      else
      {
         DosWrite(hf, pszDefaultText, strlen(pszDefaultText), &cbBytesWritten);
         DosClose(hf);
      }
   }   /* end if (fSuccess) */

   return (parent_wpSetup(somSelf,pszSetupString));

}   /* end car_wpSetup() */

/*
 *
 *  METHOD: TrapTest                                        ( ) PRIVATE
 *                                                         (X) PUBLIC
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

#pragma handler (car_TrapTest)

/*
 * METHOD: AddHornBeepPage                                ( ) PRIVATE
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


/*
 * SOM_Scope VOID  SOMLINK car_TrapTest(Car *somSelf)
 */

/*
 * The prototype for car_TrapTest was replaced by the following prototype:
 */
SOM_Scope void  SOMLINK car_TrapTest(Car *somSelf)
{
  PSZ      pszTrap = NULL;
  jmp_buf  saveState;
  CarData *somThis = CarGetData(somSelf);
  CarMethodDebug("Car","car_TrapTest");

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

      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, pszTrapMessage,
                      "Exception Handler Test",
                      999, MB_MOVEABLE | MB_OK | MB_ERROR);
   }

   memcpy(jmpState, saveState, sizeof(jmpState));

}   /* end car_TrapTest() */

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

SOM_Scope HMODULE   SOMLINK carM_clsQueryModuleHandle(M_Car *somSelf)
{
 APIRET rc;

    /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_clsQueryModuleHandle");

   /*
    *   Make sure we already have module handle
    */

    if (hmod == NULLHANDLE)
    {
     zString zsPathName;
     somId id;
       /*
        *   Retrieve registered pathname of our module (DLL) and query the
        *   module handle.
        */
       id = SOM_IdFromString("Car");
       zsPathName =
       _somLocateClassFile( SOMClassMgrObject, id ,
                            Car_MajorVersion, Car_MinorVersion);
       /* Free the som id because we don't need it anymore. */
       SOMFree( (VOID*)id);

       rc =
       DosQueryModuleHandle( zsPathName, &hmod);
       if (rc)
       {
          DebugBox("carM_wpclsInitData", "Failed to load module");
          return(hmod);
       }

    }   /* end if (hmod == NULLHANDLE) */

    return (hmod);

}   /* end carM_clsQueryModuleHandle() */

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

SOM_Scope ULONG   SOMLINK carM_wpclsQueryStyle(M_Car *somSelf)
{
    /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryStyle");

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
 *    Initalize the class data
 *
 */

SOM_Scope void SOMLINK carM_wpclsInitData(M_Car *somSelf)
{
 ULONG rc, i;
 PCLASSFIELDINFO pCFI;

    /* M_CarData *somThis  = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsInitData");

    /*
     *   Call the parent class method first
     */
    parent_wpclsInitData(somSelf);

    /*
     *   Get class title
     */
    if (!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP), _clsQueryModuleHandle(somSelf), ID_TITLE,
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

      switch (i)
      {
         case INDEX_MAKE:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(FIELDOFFSET(CARDETAILS,pszMake));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_MODEL:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(FIELDOFFSET(CARDETAILS,pszModel));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_COLOR:

            pCFI->flData           |= CFA_STRING;
            pCFI->offFieldData      = (ULONG)(FIELDOFFSET(CARDETAILS,pszColor));
            pCFI->ulLenFieldData    = sizeof(PSZ);
            pCFI->DefaultComparison = CMP_EQUAL;

            break;

         case INDEX_SALE_DATE:

            pCFI->flData           |= CFA_DATE;
            pCFI->offFieldData      = (ULONG)(FIELDOFFSET(CARDETAILS,cdateSale));
            pCFI->ulLenFieldData    = sizeof(CDATE);
            pCFI->ulLenCompareValue = sizeof(CDATE);
            pCFI->DefaultComparison = CMP_GREATER;

            break;

         case INDEX_PRICE:

            pCFI->flData           |= CFA_ULONG;
            pCFI->offFieldData      = (ULONG)(FIELDOFFSET(CARDETAILS,ulPrice));
            pCFI->ulLenFieldData    = sizeof(ULONG);
            pCFI->ulLenCompareValue = sizeof(ULONG);
            pCFI->DefaultComparison = CMP_GREATER;

            break;

      }   /* end switch(i) */

   }   /* end for (i=0, pCFI=fieldinfo; i < NUM_CAR_FIELDS; i++, pCFI++)  */

   fieldinfo[NUM_CAR_FIELDS-1].pNextFieldInfo = NULL;/* terminate linked list */

}   /* end carM_ wpclsInitData() */

/*
 *
 *  METHOD: wpclsUnInitData                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Free any class data
 *
 */

SOM_Scope void SOMLINK carM_wpclsUnInitData(M_Car *somSelf)
{
    M_CarMethodDebug("M_Car","carM_wpclsUnInitData");

    parent_wpclsUnInitData(somSelf);

}   /* end carM_wpclsUnInitData() */

/*
 *
 *  METHOD: wpclsQueryTitle                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Return the string "New car".
 *
 */

SOM_Scope PSZ SOMLINK carM_wpclsQueryTitle(M_Car *somSelf)
{
                              /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryTitle");

    /*
     *   Return the class title for a car
     */

    if (*szCarClassTitle )
       return( szCarClassTitle );
    else
       return( parent_wpclsQueryTitle(somSelf));

}   /* end carM_wpclsQueryTitle() */

/*
 *
 *  METHOD: wpclsQueryIconDdata                            ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Return the class icon
 *
 */

SOM_Scope ULONG SOMLINK carM_wpclsQueryIconData(M_Car *somSelf,
                  PICONINFO pIconInfo)
{
   APIRET rc;

   M_CarMethodDebug("M_Car","carM_wpclsQueryIconData");

   if (pIconInfo)
   {
      /*
       *   fill in icon information
       */
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod= _clsQueryModuleHandle(somSelf);
      pIconInfo->resid= ID_ICON;
   }

   return (sizeof(ICONINFO));

}   /* end carM_wpclsQueryIconData() */

/*
 *
 *  METHOD: wpclsQueryDefaultHelp                          ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Process input from the extra menu option that we added.
 *
 */

SOM_Scope BOOL SOMLINK carM_wpclsQueryDefaultHelp(M_Car *somSelf,
                PULONG pHelpPanelId,
                PSZ pszHelpLibrary)
{
                              /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryDefaultHelp");

    if (pHelpPanelId)                           /* set default help panel id */
       *pHelpPanelId   = ID_HELP_DEFAULT;

    if (pszHelpLibrary)                                /* copy help filename */
       strcpy(pszHelpLibrary, szHelpLibrary);

    return (TRUE);

}   /* end carM_wpclsQueryDefaultHelp() */

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

SOM_Scope ULONG SOMLINK carM_wpclsQueryDefaultView(M_Car *somSelf)
{
                              /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryDefaultView");

   /*  return (ID_OPENCAR); */
    return (OPEN_CAR);

}   /* end carM_wpclsQueryDefaultView() */

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

SOM_Scope ULONG SOMLINK carM_wpclsQueryDetailsInfo(M_Car *somSelf,
                PCLASSFIELDINFO *ppClassFieldInfo,
                PULONG pSize)
{
 ULONG           cParentColumns;
 PCLASSFIELDINFO pCFI;
 ULONG           i;

    /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryDetailsInfo");

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

}   /* end carM_wpclsQueryDetailsInfo() */

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

SOM_Scope PSZ   SOMLINK carM_wpclsQueryInstanceFilter(M_Car *somSelf)
{
    /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryInstanceFilter");

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

SOM_Scope PCLASSDETAILS   SOMLINK carM_wpclsQueryDetails(M_Car *somSelf)
{
    /* M_CarData *somThis = M_CarGetData(somSelf); */
    M_CarMethodDebug("M_Car","carM_wpclsQueryDetails");

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
HWND CarInit (Car* somSelf)
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
   pWindowData = (PWINDOWDATA) _wpAllocMem(somSelf, sizeof(*pWindowData), NULL);

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
      _wpQueryTitle(somSelf),     /* pointer to window text                  */
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

   WinSendMsg(hwndFrame,WM_SETICON,MPFROMP(_wpQueryIcon(somSelf)),NULL);
   WinSetWindowText(WinWindowFromID(hwndFrame,(USHORT)FID_TITLEBAR),
                                                         _wpQueryTitle(somSelf));

   /*
    * Restore the Window Position
    */
   fSuccess =
   WinRestoreWindowPos(
      szCarClassTitle,                                        /* class title */
      _wpQueryTitle(somSelf),                                /* object title */
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
      WinSetWindowPos(hwndFrame, HWND_TOP, swp.x, swp.y, swp.cx,
                     swp.cy, swp.fl);
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
        _wpAddToObjUseList(pWindowData->somSelf,&pWindowData->UseItem);
        _wpRegisterView(pWindowData->somSelf, hwndFrame,
                                             _wpQueryTitle(pWindowData->somSelf));
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
            /*
             *   If the car's brakes are off, we move the car by modifying it's
             *   x,y position.  Direction (xDir, yDir) changes when the car's
             *   position reaches a border of the window.  The distance it
             *   moves is based on the speed contained in an instance variable.
             */

            if (_QueryBrakes(pWindowData->somSelf) == FALSE)
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
                  pWindowData->xDir * _QuerySpeed(pWindowData->somSelf);

               pWindowData->y +=
                  pWindowData->yDir * _QuerySpeed(pWindowData->somSelf);

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
                          _wpQueryIcon(pWindowData->somSelf), DP_NORMAL);
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
            WinStoreWindowPos(szCarClassTitle,_wpQueryTitle(pWindowData->somSelf),
                                                                        hwndFrame);
            _wpDeleteFromObjUseList(pWindowData->somSelf,&pWindowData->UseItem);
            _wpFreeMem(pWindowData->somSelf,(PBYTE)pWindowData);

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
            (PDASHDLGDATA) _wpAllocMem((Car *)mp2, sizeof(*pDashDlgData), NULL);
         if (pDashDlgData)
         {
            memset((PVOID) pDashDlgData, 0, sizeof(*pDashDlgData));
            pDashDlgData->cb = sizeof(*pDashDlgData);
            pDashDlgData->somSelf = (Car *) mp2;   /* pointer to this object */
            pDashDlgData->PrevBrakes = _QueryBrakes(pDashDlgData->somSelf);
            pDashDlgData->PrevSpeed  = _QuerySpeed(pDashDlgData->somSelf);
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
            MPFROMSHORT((SHORT)_QuerySpeed(pDashDlgData->somSelf)/10));

         WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
            _ltoa(_QuerySpeed(pDashDlgData->somSelf),acBuffer,10));

         if (_QueryBrakes(pDashDlgData->somSelf) == FALSE)
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

         pDashDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);
         if (pDashDlgData == NULL)
         {
            DebugBox("DashboardDlgProc:WM_DESTROY", "couldn't get window words");
            break;
         }
         _wpFreeMem(pDashDlgData->somSelf,(PBYTE)pDashDlgData);
         return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

      case WM_COMMAND:

         pDashDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);
         if (pDashDlgData == NULL)
         {
            DebugBox("DashboardDlgProc:WM_COMMAND", "couldn't get window words");
            break;
         }

         switch (SHORT1FROMMP(mp1))
         {
            case ID_UNDO:
               _SetBrakes(pDashDlgData->somSelf, pDashDlgData->PrevBrakes);
               _SetSpeed(pDashDlgData->somSelf, pDashDlgData->PrevSpeed);

               WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
                  MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                  MPFROMSHORT((SHORT)(_QuerySpeed(pDashDlgData->somSelf)/10)));

               WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                  _ltoa(_QuerySpeed(pDashDlgData->somSelf),acBuffer,10));

               if (_QueryBrakes(pDashDlgData->somSelf) == FALSE)
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
               pDashDlgData->PrevBrakes = _QueryBrakes(pDashDlgData->somSelf);
               pDashDlgData->PrevSpeed  = _QuerySpeed(pDashDlgData->somSelf);

               _SetBrakes(pDashDlgData->somSelf, DEFAULT_BRAKEFLAG);
               _SetSpeed(pDashDlgData->somSelf, DEFAULT_SPEED);

               WinSendDlgItemMsg(hwndDlg,ID_SPEEDSLIDER,SLM_SETSLIDERINFO,
                  MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                  MPFROMSHORT((SHORT)(_QuerySpeed(pDashDlgData->somSelf)/10)));

               WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                  _ltoa(_QuerySpeed(pDashDlgData->somSelf), acBuffer,10));

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

         pDashDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);
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
                  pDashDlgData->PrevBrakes = _QueryBrakes(pDashDlgData->somSelf);
                  _SetBrakes(pDashDlgData->somSelf, FALSE);
               }
               break;

            case ID_STOP:
               if ((USHORT) SHORT2FROMMP(mp1) == BN_CLICKED)
               {
                  pDashDlgData->PrevBrakes = _QueryBrakes(pDashDlgData->somSelf);
                  _SetBrakes(pDashDlgData->somSelf, TRUE);
               }
               break;

            case ID_SPEEDSLIDER:
               if ((USHORT) SHORT2FROMMP(mp1) == SLN_CHANGE)
               {
                  pDashDlgData->PrevSpeed  = _QuerySpeed(pDashDlgData->somSelf);
                  temp = (ULONG) WinSendDlgItemMsg(hwndDlg, ID_SPEEDSLIDER,
                     SLM_QUERYSLIDERINFO,
                     MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                     NULL);
                  temp = temp * 10;
                  WinSetDlgItemText(hwndDlg,ID_SPEEDDATA,
                                   _ltoa(temp,acBuffer,10));
                  _SetSpeed(pDashDlgData->somSelf, temp);
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
            (PHORNDLGDATA) _wpAllocMem((Car *) mp2, sizeof(*pHornDlgData), NULL);
         if (pHornDlgData)
         {
            memset((PVOID) pHornDlgData, 0, sizeof(*pHornDlgData));
            pHornDlgData->cb = sizeof(*pHornDlgData);
            pHornDlgData->somSelf = (Car *) mp2;  /* pointer to this object */
            pHornDlgData->PrevDuration = _QueryDuration(pHornDlgData->somSelf);
            pHornDlgData->PrevHighTone = _QueryHighTone(pHornDlgData->somSelf);
            pHornDlgData->PrevLowTone  = _QueryLowTone(pHornDlgData->somSelf);
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
            MPFROMLONG(_QueryHighTone(pHornDlgData->somSelf)), MPFROMLONG(0));

         WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETLIMITS,
            MPFROMLONG(1000), MPFROMLONG(0));

         WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETCURRENTVALUE,
            MPFROMLONG(_QueryLowTone(pHornDlgData->somSelf)), MPFROMLONG(0));

         return (MRESULT) TRUE;
         break;

      }   /* end case WM_INITDLG: */

      case WM_DESTROY:

         pHornDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);

         if (pHornDlgData == NULL)
         {
            DebugBox("HornBeepDlgProc", "Couldn't get window words");
            break;
         }

         _wpFreeMem(pHornDlgData->somSelf,(PBYTE)pHornDlgData);
         return (WinDefDlgProc(hwndDlg, msg, mp1, mp2) );

      case WM_COMMAND:

         pHornDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);
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
                  _QueryHighTone(pHornDlgData->somSelf);
               pHornDlgData->PrevLowTone =
                  _QueryLowTone(pHornDlgData->somSelf);

               _SetHighTone(pHornDlgData->somSelf, DEFAULT_HITONE);
               _SetLowTone(pHornDlgData->somSelf, DEFAULT_LOTONE);

               WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(DEFAULT_HITONE), MPFROMLONG(0));

               WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_SETCURRENTVALUE,
                  MPFROMLONG(DEFAULT_LOTONE), MPFROMLONG(0));
               break;

            case ID_HELP:
               break;

         }   /* end switch (SHORT1FROMMP(mp1)) */

         return (MRESULT) TRUE;
         break;

      case WM_CONTROL:
      {
       ULONG temp;

         pHornDlgData = WinQueryWindowPtr(hwndDlg, QWL_USER);

         if (pHornDlgData == NULL)
         {
            DebugBox("HornBeepDlgProc", "Couldn't get window words");
            break;
         }

         switch (SHORT2FROMMP(mp1))
         {
            case SPBN_ENDSPIN:

               /*
                *   preserve previous values
                */
               pHornDlgData->PrevHighTone =
                  _QueryHighTone(pHornDlgData->somSelf);
               pHornDlgData->PrevLowTone =
                  _QueryLowTone(pHornDlgData->somSelf);

               WinSendDlgItemMsg( hwndDlg, ID_HITONE, SPBM_QUERYVALUE,
                  (MPARAM)&temp, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID));
               _SetHighTone(pHornDlgData->somSelf, temp);

               WinSendDlgItemMsg( hwndDlg, ID_LOTONE, SPBM_QUERYVALUE,
                  (MPARAM)&temp, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID));
               _SetLowTone(pHornDlgData->somSelf, temp);

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

/********************************  END car.c  ********************************/
