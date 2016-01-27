/*ÖÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ·*/
/*º File:         MAIN.C                                          º*/
/*º Date Created: 02/15/95                                        º*/
/*ÓÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ½*/

/*ÖÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ·*/
/*º Description:                                                  º*/
/*º   This is "main" wrapper for a DAPIE-Based app.  It initial-  º*/
/*º izes the AWE environment, call the WinMain function, and upon º*/
/*º completion, calls the WinTerm function to shutdown the AWE    º*/
/*º environment.                                                  º*/
/*º                                                               º*/
/*ÓÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ½*/

/* Include the windows header files ... */
#include <os2win.h>

int main(int argc, char *argv[], char *envp[])
{
   /* Now call WinRun to run the application ... */
   return WinCallWinMain( argc, argv, &WinMain, SW_SHOWNORMAL );
}
