/******************************************************************************
*
*  Module Name: RPM.CPP
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
*        Query
*        Set
*        SetToDefault
*        SaveState
*        RestoreState
*
*  Description:
*
*      This file contains the member functions of class RPM.
******************************************************************************/
#include "Carpp.xih"
/*
 * Method: RPM::RPM
 *
 * Description:
 *
 *    This is the constructor for class RPM which initializes
 *     the state data for an object of type RPM.
 */
RPM::RPM(VOID)
{
  magnitude = DEFAULT_RPM;
}

/*
 * Method: RPM::Query
 *
 * Description:
 *
 *    Returns a ULONG value indicating the speed of the car in RPM's.
 */
ULONG RPM::Query(VOID)
{
  return magnitude;
}

/*
 * Method: RPM::Set
 *
 * Description:
 *
 *    Sets the speed of the car.  An argument of type ULONG is passed to
 *     the function indicating the new speed of the car in RPM's.
 */
VOID RPM::Set(ULONG ulRPM)
{
  magnitude = ulRPM;
}

/*
 * Method: RPM::SetToDefault
 *
 * Description:
 *
 *    Sets the default state of the object.
 */
VOID RPM::SetToDefault(VOID)
{
  magnitude = DEFAULT_RPM;
}

/*
 * Method: RPM::SaveState
 *
 * Description:
 *
 *    Saves the state of the current object in the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being saved can be stored in the OS2.INI file.
 */
VOID RPM::SaveState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpSaveLong( szCarClassTitle, IDKEY_RPM, magnitude);
}

/*
 * Method: RPM::RestoreState
 *
 * Description:
 *
 *    Restores the object's saved state from the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being restored can be retrieved from the OS2.INI file.
 */
VOID RPM::RestoreState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpRestoreLong( szCarClassTitle, IDKEY_RPM,  &magnitude);
}
/********************************  END   rpm.cpp  ********************************/
