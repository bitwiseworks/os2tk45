/******************************************************************************
*
*  Module Name: BRAKE.CPP
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
*        Set
*        Release
*        IsSet
*        SetToDefault
*        SaveState
*        RestoreState
*
*  Description:
*
*      This file contains the member functions of class Brake.
******************************************************************************/
#include "Carpp.xih"

/*
 * Method: Brake::Brake
 *
 * Description:
 *
 *    This is the constructor for class Brake which initializes
 *     the state data for an object of type Brake.
 */
Brake::Brake(VOID)
{
  depressed = DEFAULT_BRAKEFLAG;
}

/*
 * Method: Brake::Set
 *
 * Description:
 *
 *    Sets the brake.
 */
VOID Brake::Set(VOID)
{
  depressed = TRUE;
}

/*
 * Method: Brake::Release
 *
 * Description:
 *
 *    Releases the brake.
 */
VOID Brake::Release(VOID)
{
  depressed = FALSE;
}

/*
 * Method: Brake::IsSet
 *
 * Description:
 *
 *    Determines whether or not the brake is set.  A value of TRUE is returned
 *     if the brake is set, otherwise, FALSE is returned.
 */
BOOL Brake::IsSet(VOID)
{
  if (depressed)
    return TRUE;
  else
    return FALSE;
}

/*
 * Method: Brake::SetToDefault
 *
 * Description:
 *
 *    Sets the default state of the object.
 */
VOID Brake::SetToDefault(VOID)
{
  depressed = DEFAULT_BRAKEFLAG;
}

/*
 * Method: Brake::SaveState
 *
 * Description:
 *
 *    Saves the state of the current object in the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being saved can be stored in the OS2.INI file.
 */
VOID Brake::SaveState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpSaveLong( szCarClassTitle, IDKEY_BRAKEFLAG, depressed);
}

/*
 * Method: Brake::RestoreState
 *
 * Description:
 *
 *    Restores the object's saved state from the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being restored can be retrieved from the OS2.INI file.
 */
VOID Brake::RestoreState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpRestoreLong(szCarClassTitle, IDKEY_BRAKEFLAG, &depressed);
}
/********************************  END brake.cpp  ********************************/
