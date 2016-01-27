/*****************************************************************************
 * Copyright:                                                                *
 *   Licensed Materials - Property of IBM                                    *
 *   (C) Copyright IBM Corp. 1995                                            *
 *   All Rights Reserved                                                     *
 *****************************************************************************
 * File: main.c                                                              *
 *****************************************************************************
 * Description:                                                              *
 *   Sample "main" wrapper for applications/executables.                     *
 *****************************************************************************/
#include <os2win.h>

int main(int argc, char *argv[], char *envp[])
{
   /* Call WinCallWinMain to start the application.
    */
   return WinCallWinMain( argc, argv, &WinMain, SW_SHOWNORMAL );
}
