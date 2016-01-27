/******************************************************************************
*
*  Module Name: BRAKE.H
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
*      This file contains the declaration of class Brake.  Brake is a C++ class
*       whose client is Carpp which is a SOM class.  This is one example of
*       the use of C++ constructs in the implementation of a SOM class.
*
*      Brake is the abstraction of a real car's brake.  The brake can be in
*       one of two states.  It can either be depressed or not depressed.
*       There are public member functions to set the brake, release the
*       brake, and determine whether or not the brake is currently depressed.
******************************************************************************/
#include "carpp.xih"

class Brake
{
  BOOL depressed;

public:
  Brake(VOID);
  VOID Set(VOID);
  VOID Release(VOID);
  BOOL IsSet(VOID);
  VOID SetToDefault(VOID);
  VOID SaveState(Carpp *clientObject, PSZ CarClassTitle);
  VOID RestoreState(Carpp *clientObject, PSZ CarClassTitle);
};
/********************************  END brake.h    ********************************/
