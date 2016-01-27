/******************************************************************************
*
*  Module Name: HORN.CPP
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
*        QueryDuration
*        SetDuration
*        QueryHighTone
*        SetHighTone
*        QueryLowTone
*        SetLowTone
*        SetToDefault
*        Beep
*        SaveState
*        RestoreState
*
*  Description:
*
*      This file contains the member functions of class Horn.
******************************************************************************/
#include "Carpp.xih"

/*
 * Method: Horn::Horn
 *
 * Description:
 *
 *    This is the constructor for class Horn which initializes
 *     the state data for an object of type Horn.
 */
Horn::Horn(VOID)
{
  lowTone = DEFAULT_LOTONE;
  highTone = DEFAULT_HITONE;
  duration = DEFAULT_DURATION;
}

/*
 * Method: Horn::QueryDuration
 *
 * Description:
 *
 *    Returns a ULONG value indicating the duration of the horn in
 *     milliseconds.
 */
ULONG Horn::QueryDuration(VOID)
{
  return duration;
}

/*
 * Method: Horn::SetDuration
 *
 * Description:
 *
 *    Sets the duration of the horn.  An argument of type ULONG is passed to
 *     the function indicating the horn duration in milliseconds.
 */
VOID Horn::SetDuration(ULONG ulDuration)
{
  duration = ulDuration;
}

/*
 * Method: Horn::QueryHighTone
 *
 * Description:
 *
 *    Returns a ULONG value indicating the current beep frequency of the
 *     horn's high tone.
 */
ULONG Horn::QueryHighTone(VOID)
{
  return highTone;
}

/*
 * Method: Horn::SetHighTone
 *
 * Description:
 *
 *    Sets the frequency of the horn's high tone.  An argument of type ULONG
 *     is passed to the function indicating the new frequency of the horn's
 *     high tone.
 */
VOID Horn::SetHighTone(ULONG ulTone)
{
  highTone = ulTone;
}

/*
 * Method: Horn::QueryLowTone
 *
 * Description:
 *
 *    Returns a ULONG value indicating the current beep frequency of the
 *     horn's low tone.
 */
ULONG Horn::QueryLowTone(VOID)
{
  return lowTone;
}

/*
 * Method: Horn::SetLowTone
 *
 * Description:
 *
 *    Sets the frequency of the horn's low tone.  An argument of type ULONG is
 *     passed to the function indicating the new frequency of the horn's
 *     low tone.
 */
VOID Horn::SetLowTone(ULONG ulTone)
{
  lowTone = ulTone;
}

/*
 * Method: Horn::SetToDefault
 *
 * Description:
 *
 *    Sets the default state of the object.
 */
VOID Horn::SetToDefault(VOID)
{
  lowTone = DEFAULT_LOTONE;
  highTone = DEFAULT_HITONE;
  duration = DEFAULT_DURATION;
}

/*
 * Method: Horn::Beep
 *
 * Description:
 *
 *    Beeps the horn using the data stored in the object.
 */
VOID Horn::Beep(VOID)
{
  DosBeep(QueryHighTone(), QueryDuration());
  DosSleep(100);
  DosBeep(QueryLowTone(), QueryDuration());
}

/*
 * Method: Horn::SaveState
 *
 * Description:
 *
 *    Saves the state of the current object in the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being saved can be stored in the OS2.INI file.
 */
VOID Horn::SaveState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpSaveLong(szCarClassTitle, IDKEY_LOTONE, lowTone);
  clientObject->wpSaveLong(szCarClassTitle, IDKEY_HITONE, highTone);
  clientObject->wpSaveLong(szCarClassTitle, IDKEY_DURATION, duration);
}

/*
 * Method: Horn::RestoreState
 *
 * Description:
 *
 *    Restores the object's saved state from the OS2.INI file.  The input
 *     arguments consist of a pointer to a Carpp object as well as a pointer
 *     to a string.  These two items are required so that the data associated
 *     with the object being restored can be retrieved from the OS2.INI file.
 */
VOID Horn::RestoreState(Carpp *clientObject, PSZ szCarClassTitle)
{
  clientObject->wpRestoreLong(szCarClassTitle, IDKEY_LOTONE, &lowTone);
  clientObject->wpRestoreLong(szCarClassTitle, IDKEY_HITONE, &highTone);
  clientObject->wpRestoreLong(szCarClassTitle, IDKEY_DURATION, &duration);
}
/********************************  END  horn.cpp  ********************************/
