/******************************************************************************
*
*  Module Name: HORN.H
*
*  OS/2 Work Place Shell Sample Program - SOM 2.0 / IDL Version
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
*  Description:
*
*      This file contains the declaration of class Horn.  Horn is a C++ class
*       whose client is Carpp which is a SOM class.  This is one example of
*       the use of C++ constructs in the implementation of a SOM class.
*
*      Horn is the abstraction of a real car's horn.  It has two user settable
*       tones and a user settable duration.  A request to beep the horn will
*       cause the high tone to be sounded for the specified duration followed
*       by the low tone being sounded for the specified duration.
******************************************************************************/
#include "carpp.xih"

class Horn
{
  ULONG lowTone;
  ULONG highTone;
  ULONG duration;

public:
  Horn(VOID);
  ULONG QueryDuration(VOID);
  VOID  SetDuration(ULONG ulDuration);
  ULONG QueryHighTone(VOID);
  VOID  SetHighTone(ULONG ulTone);
  ULONG QueryLowTone(VOID);
  VOID  SetLowTone(ULONG ulTone);
  VOID  SetToDefault(VOID);
  VOID  Beep(VOID);
  VOID  SaveState(Carpp *clientObject, PSZ CarClassTitle);
  VOID  RestoreState(Carpp *clientObject, PSZ CarClassTitle);
};
/********************************  END  horn.h    ********************************/
