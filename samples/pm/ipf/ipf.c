/*static char *SCCSID = "@(#)ipf.c	6.2 91/12/24";*/

/***************** IPF C Sample Program Source Code File (.C) ****************\
 *                                                                           *
 * PROGRAM NAME: IPF.C   PM Sample Application                               *
 * 컴컴컴컴컴컴�                                                             *
 *                                                                           *
 *   (C) Copyright International Business Machines Corporation 1991          *
 *                                                                           *
 * REVISION HISTORY:                                                         *
 * 컴컴컴컴컴컴컴컴�                                                         *
 *                                                                           *
 *   Revision level - 2.0                                                    *
 *                                                                           *
 * WHAT THIS PROGRAM DOES:                                                   *
 * 컴컴컴컴컴컴컴컴컴컴컴�                                                   *
 *                                                                           *
 *   This program creates an IPF Application Controlled Window.              *
 *                                                                           *
 * WHAT THIS PROGRAM DEMONSTRATES:                                           *
 * 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�                                           *
 *                                                                           *
 *   This program illustrates a suggested format for sample Help Manager     *
 *   code using the Information Presentation Facility.                       *
 *                                                                           *
 * API CALLS FEATURED:                                                       *
 * 컴컴컴컴컴컴컴컴컴�                                                       *
 *                                                                           *
 *      DosFreeModule               WinMessageBox                            *
 *      DosLoadModule               WinRegisterClass                         *
 *      GpiDeleteBitmap             WinReleasePS                             *
 *      GpiLoadBitmap               WinSendMsg                               *
 *      WinBeginPaint               WinSetWindowULong                        *
 *      WinDefWindowProc            WinStartTimer                            *
 *      WinDestroyWindow            WinStopTimer                             *
 *      WinDrawBitMap               WinQueryAnchorBlock                      *
 *      WinEndPaint                 WinQueryWindow                           *
 *      WinGetPS                    WinQueryWindowULong                      *
 *      WinInvalidateRect                                                    *
 *                                                                           *
 *                                                                           *
 * WHAT YOU NEED TO COMPILE AND LINK THIS PROGRAM:                           *
 * 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�                           *
 *                                                                           *
 *      REQUIRED FILES:                                                      *
 *      컴컴컴컴컴컴컴�                                                      *
 *                                                                           *
 *           IPF.C         - Source code                                     *
 *           IPF.H         - Include file                                    *
 *           IPF.MAK       - Make file                                       *
 *           IPF.DEF       - Module definition file                          *
 *           IPF.RC        - Resource file                                   *
 *           FRAME1.BMP    - Bitmap 1 used for animation                     *
 *           FRAME2.BMP    - Bitmap 2 used for animation                     *
 *           FRAME3.BMP    - Bitmap 3 used for animation                     *
 *           FRAME4.BMP    - Bitmap 4 used for animation                     *
 *           FRAME5.BMP    - Bitmap 5 used for animation                     *
 *                                                                           *
 *           OS2.H         - Include file for Presentation Manager           *
 *                                                                           *
 *      REQUIRED LIBRARIES:                                                  *
 *      컴컴컴컴컴컴컴컴컴�                                                  *
 *                                                                           *
 *           OS2386.LIB   - Link library for OS/2 functions                  *
 *           DDE4NBS.LIB  - Link library for DLL functions                   *
 *           DDE4NBSO.LIB - Link library for DLL functions                   *
 *                                                                           *
 *      REQUIRED PROGRAMS:                                                   *
 *      컴컴컴컴컴컴컴컴컴                                                   *
 *                                                                           *
 *           IBM C386 Compiler                                               *
 *           IBM LINK386 Linker                                              *
 *           Resource Compiler                                               *
 *                                                                           *
\*****************************************************************************/

/*---------------------------------------------------------------------------*\
 * Include files, macros, defined constants, and externs                     *
\*---------------------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#define INCL_DOSMODULEMGR
#define LINT_ARGS
#define DINCL_32

#include <OS2.H>
#include "IPF.H"

#define COM_HWND             4  /* Used in WinSetWindowULong              */
#define FRAMES               5  /* Number of frames in animation sequence */
#define BEEP_WARN_FREQ      60  /* Frequency of warning beep              */
#define BEEP_WARN_DUR      100  /* Duration of warning beep               */

/*---------------------------------------------------------------------------*\
 * Structure and type definitions                                            *
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
 * Variables used globally in this module                                    *
\*---------------------------------------------------------------------------*/

USHORT   IPFClassRegistered = 0;         /* IPF class registered flag        */
HWND     hwndClient;                     /* Handle to the client window      */
HWND     hwndPrevious;                   /* Handle to the previous active    */
                                         /*  object communication window     */
HWND     hwndLatest;                     /* Handle to the latest active      */
                                         /*  object communication window     */

/*---------------------------------------------------------------------------*\
 * Forward definitions of entry points                                       *
\*---------------------------------------------------------------------------*/

MRESULT EXPENTRY IPFMain (PACVP pACVP, PCH Parameter);
MRESULT EXPENTRY IPFWinProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
VOID Error (PCH str);

/*---------------------------------------------------------------------------*\
 * IPF Entry Point                                                           *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *  Name:     IPFMain ()                                                     *
 *                                                                           *
 *  Purpose:  To register a window class for the AC window, create an        *
 *            instance of the class, and register it with IPF as an          *
 *            object communication window.                                   *
 *                                                                           *
 *  Usage:    N/A                                                            *
 *                                                                           *
 *  Method:                                                                  *
 *            1) Initialize                                                  *
 *              - Register the window class for the main window              *
 *              - Create the main window                                     *
 *                                                                           *
 *            2) Process                                                     *
 *              - Store the window handle                                    *
 *              - Store the parent handle                                    *
 *              - Set the window as an active object communication window    *
 *                                                                           *
 *            3) Finish                                                      *
 *              - Save the returned previous object communication handle     *
 *                                                                           *
 *  Returns:  Returns via return                                             *
 *                                                                           *
 *            0 - if sucessful execution completed                           *
 *            1 - if error in execution                                      *
 *                                                                           *
\*****************************************************************************/


MRESULT EXPENTRY IPFMain (PACVP pACVP, PCH Parameter)
{

    HWND  hwndParent;         /* Handle of parent window in IPF             */
    HWND  hwndFrame;          /* Handle to the frame                        */
    ULONG WinStyle;           /* window style for creating frame            */
    ULONG CtrlData;           /* control data for creating frame            */

    Parameter; /* Warning Level 3 Avoidance */


/** 1) Initialize **/


/*****************************************************************************\
 * Register the window class for the application if not already registered.  *
\*****************************************************************************/

    if (!IPFClassRegistered)
    {
        if (!WinRegisterClass (pACVP->hAB,
                               "CLASS_IPF",
                               (PFNWP) IPFWinProc,
                               CS_SYNCPAINT | CS_SIZEREDRAW | CS_MOVENOTIFY,
                               8))
        {
            DosBeep (BEEP_WARN_FREQ, BEEP_WARN_DUR);
            exit (TRUE);
        }
        IPFClassRegistered = 1;
    }

/*****************************************************************************\
 * Set the frame window style to not visible.                                *
\*****************************************************************************/

    WinStyle = 0L;

/*****************************************************************************\
 * Set the frame window to have no tilte bar or buttons.                     *
\*****************************************************************************/

    CtrlData = 0L;

/*****************************************************************************\
 * Create a window frame and a client window. Return the handle to the       *
 * window frame. Use the ACVP structure for obtaining the handle of the IPF  *
 * parent window. The window frame will become the parent to the client      *
 * window.                                                                   *
\*****************************************************************************/

    if (!(hwndFrame = WinCreateStdWindow (pACVP->hWndParent,
                                          WinStyle,
                                          &CtrlData,
                                          "CLASS_IPF",
                                          "IPF",
                                          0L,
                                          0L,
                                          0L,
                                          &hwndClient
                                          )))
    {
        Error ("Can not create window");
        return (MRESULT) TRUE;
    }


/** 2) Process **/


/*****************************************************************************\
 * Set the handle of the AC window to the handle of the window frame. Use    *
 * the ACVP structure for passing the information.                           *
\*****************************************************************************/

    pACVP->hWndACVP = hwndFrame;

/*****************************************************************************\
 * Set the handle of the parent window to the handle of the AC window. Use   *
 * the ACVP structure for passing the information.                           *
\*****************************************************************************/

    hwndParent = pACVP->hWndParent;

/*****************************************************************************\
 *                                                                           *
 * The following comment includes a sample scenario to provide a broader     *
 * explanation of the object communication process.                          *
 *                                                                           *
 * Send a message to the IPF parent window that this window frame is the     *
 * latest active object communication window (B) in the chain to which       *
 * messages are to be sent to. The handle of the previous active object      *
 * communication window (A) is returned. This window frame remains the       *
 * latest active object communication window (B) until a new active object   *
 * communication window (C) wants to join the communication chain.           *
 *                                                                           *
 *                                 -                                         *
 * Communication chain:       A -> B                                         *
 *                                 -                                         *
\*****************************************************************************/

    hwndPrevious = (HWND) WinSendMsg (pACVP->hWndParent,
                                      HM_SET_OBJCOM_WINDOW,
                                      (MPARAM) hwndFrame,
                                      (MPARAM)NULL);

/*****************************************************************************\
 * Check if the preceeding WinSendMsg was successful. Look for the current   *
 * window as the latest active object communication window in the chain.     *
\*****************************************************************************/

    hwndLatest = (HWND) WinSendMsg (pACVP->hWndParent,
                                    HM_QUERY,
                                    MPFROM2SHORT ((USHORT)0, HMQW_OBJCOM_WINDOW),
                                    (MPARAM)NULL);

    if (hwndFrame != hwndLatest)
    {
        Error ("Can not set object communication window");
        return (MRESULT) TRUE;
    }


/** 3) Finish **/


/*****************************************************************************\
 * Save the handle of the previous active object communciation window into   *
 * reserved memory for the client window.                                    *
\*****************************************************************************/

    if (!WinSetWindowULong (hwndClient, COM_HWND, (ULONG) hwndPrevious))
    {
        Error ("Can not save handle into reserved memory");
        return (MRESULT) TRUE;
    }
    return (MRESULT) FALSE;
}


/****************************** End of IPFMain *******************************/


/*---------------------------------------------------------------------------*\
 * Client Window Procedure                                                   *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *  Name:     IPFWinProc ()                                                  *
 *                                                                           *
 *  Purpose:  To perform animation in the IPF AC window.                     *
 *                                                                           *
 *  Usage:    Called from 'IPFMain' procedure.                               *
 *                                                                           *
 *  Method:                                                                  *
 *            1) Initialize                                                  *
 *              - Check the previous object communication window             *
 *              - Get the handle to the bitmaps in the DLL resource          *
 *              - Load the bitmaps                                           *
 *              - Set and start the timer                                    *
 *                                                                           *
 *            2) Process                                                     *
 *              - Update to the next bitmap frame                            *
 *              - Draw the new bitmap                                        *
 *                                                                           *
 *            3) Finish                                                      *
 *              - Destroy the window                                         *
 *              - Stop the timer                                             *
 *              - Delete the bitmaps                                         *
 *              - Restore the previous object communication window           *
 *              - Free the DLL resource                                      *
 *                                                                           *
 *  Returns:  Returns via return                                             *
 *                                                                           *
 *            0 - if sucessful execution completed                           *
 *            1 - if error in execution                                      *
 *                                                                           *
\*****************************************************************************/


MRESULT EXPENTRY IPFWinProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{

    static HAB      Hhab;         /* anchor block handle                    */
    static HBITMAP  hbm [5];      /* array of bitmap handles                */
    static HPS      hps;          /* presentation space                     */
    static POINTL   ptl;          /* pointl                                 */
    static HMODULE  hModule;      /* to get bitmaps from DLL resource       */
    static SHORT    index;        /* index to current bitmap to display     */
    static LONG     cxClient,
                    cyClient;     /* window size                            */
           BOOL     rValue=TRUE;  /* FALSE if the message was acted         */
                                  /* upon successfully                      */


/** 1) Initialize **/


    switch (msg)
    {
        case HM_UPDATE_OBJCOM_WINDOW_CHAIN:

/*****************************************************************************\
 * Retrieve the handle to the previous object communications window.         *
\*****************************************************************************/

            hwndPrevious = (HWND) WinQueryWindowULong (hwnd, COM_HWND);

/*****************************************************************************\
 *                                                                           *
 * The following comment includes a sample scenario to provide a broader     *
 * explanation of the object communication process.                          *
 *                                                                           *
 * If the previous object communication window (A) is terminating for this   *
 * object communication window (B), then update the saved previous object    *
 * communication window handle (A) to the new previous object communication  *
 * window handle (NULL). Otherwise ignore the message and pass it onto the   *
 * preceeding object communication window (A).                               *
 *                                                                           *
 *                                 -                                         *
 * Communication chain:       A -> B -> C -> D -> E                          *
 *                                 -                                         *
\*****************************************************************************/

            if (hwndPrevious == (HWND)mp2)
            {
                 hwndPrevious = (HWND)mp1;

/*****************************************************************************\
 * Save the handle of the new previous active object communciation window    *
 * into reserved memory for the client window.                               *
\*****************************************************************************/

                if (!WinSetWindowULong (hwndClient,
                                        COM_HWND,
                                        (ULONG) hwndPrevious))
                {
                    Error ("Can not save handle into reserved memory");
                    break;
                }
            }
            else
            {
                if (hwndPrevious != 0L)
                {
                    WinSendMsg (hwndPrevious,
                                HM_UPDATE_OBJCOM_WINDOW_CHAIN,
                                (MPARAM) mp1,
                                (MPARAM) mp2);
                }
            }

            rValue = FALSE;
            break;

        case WM_CREATE:

            if (DosLoadModule (NULL, 0L, "IPF", &hModule))
            {
                Error ("Can not load module");
                break;
            }

/*****************************************************************************\
 * Get the presentation space and return the handle.                         *
\*****************************************************************************/

            if (!(hps = WinGetPS(hwnd)))
            {
                Error ("Can not get presentation space");
                break;
            }

/*****************************************************************************\
 * Update the frame index and load the bitmap resources from the DLL module. *
\*****************************************************************************/

            for (index = 0; index < FRAMES; index++)
            {
                if (!(hbm [index] = GpiLoadBitmap (hps,
                                                   hModule,
                                                   (USHORT)(IDB_FRAME1+index),
                                                   cxClient,
                                                   cyClient)))
                {
                    Error ("Can not load bitmap");
                    return (MRESULT) rValue;
                }
            }

/*****************************************************************************\
 * Release the presentation space.                                           *
\*****************************************************************************/

            WinReleasePS (hps);

            index = 0;

/*****************************************************************************\
 * Start a timer to send timer messages at a desired rate.                   *
\*****************************************************************************/

            if (!(Hhab = WinQueryAnchorBlock (hwnd)))
            {
                Error ("Can not retrieve anchor block handle");
                break;
            }

            if (!WinStartTimer (Hhab, hwnd, ID_TIMER, 150))
            {
                Error ("Can not start timer");
                break;
            }

            rValue = FALSE;
            break;

/*****************************************************************************\
 * Update the index when a timer message is received.                        *
\*****************************************************************************/

        case WM_TIMER:

            if (index++ == FRAMES-1)
            {
                index = 0;
            }

/*****************************************************************************\
 * Invalidate the window to cause repainting.                                *
\*****************************************************************************/

            WinInvalidateRect (hwnd, NULL, FALSE);

            rValue = FALSE;
            break;


/** 2) Process **/


/*****************************************************************************\
 * Get the presentation space whose associated update region is ready for    *
 * drawing. Draw the bitmap and restore the presentation space to the prior  *
 * state.                                                                    *
\*****************************************************************************/

        case WM_PAINT:

            if (!(hps = WinBeginPaint (hwnd, 0L, NULL)))
            {
                Error ("Can not set presentaion space for drawing");
                break;
            }

            if (!WinDrawBitmap (hps,
                                hbm [index],
                                NULL,
                                &ptl,
                                CLR_NEUTRAL,
                                CLR_BACKGROUND,
                                DBM_NORMAL))
            {
                Error ("Can not draw bitmap");
                break;
            }

            WinEndPaint (hps);

            rValue = FALSE;
            break;

        case WM_SIZE:

            cxClient = SHORT1FROMMP (mp2);
            cyClient = SHORT2FROMMP (mp2);

            rValue = FALSE;
            break;


/** 3) Finish **/


/*****************************************************************************\
 * Retrieve the handle of the parent window and destroy the window.          *
\*****************************************************************************/

        case WM_CLOSE:

            WinDestroyWindow (WinQueryWindow (hwnd, QW_PARENT));

            rValue = FALSE;
            break;

/*****************************************************************************\
 * Stop the timer and delete the bitmaps.                                    *
\*****************************************************************************/

        case WM_DESTROY:

            WinStopTimer (Hhab, hwnd, ID_TIMER);

            for (index = 0; index < 8; index++)
            {
                GpiDeleteBitmap (hbm [index]);
            }

/*****************************************************************************\
 * Retrieve the handle to the previous object communications window.         *
\*****************************************************************************/

            hwndPrevious = (HWND) WinQueryWindowULong (hwnd, COM_HWND);

/*****************************************************************************\
 * Query IPF for the handle of the latest active object communication window.*
\*****************************************************************************/

            hwndLatest = (HWND)WinSendMsg (hwnd,
                                    HM_QUERY,
                                    MPFROM2SHORT ((USHORT)0, HMQW_OBJCOM_WINDOW),
                                    (MPARAM)NULL);

/*****************************************************************************\
 *                                                                           *
 * The following comment includes a sample scenario to provide a broader     *
 * explanation of the object communication process.                          *
 *                                                                           *
 * Send a message to the latest active object communication window (E) that  *
 * this object communication window (B) has terminated. The latest active    *
 * object communication window (E) will either use the information or pass   *
 * it to the preceeding object communication window (D). The information     *
 * will be passed along until it reaches the object communication window (C) *
 * created after this object communication window (B). The later object      *
 * communication window (C) must update its saved previous object            *
 * communication window handle (B) to the new previous object communication  *
 * window handle (A) in the communication chain.                             *
 *                                                                           *
 *                                 -                                         *
 * Communication chain:       A -> B -> C -> D -> E                          *
 *                                 -                                         *
\*****************************************************************************/

            WinSendMsg (hwndLatest,
                        HM_UPDATE_OBJCOM_WINDOW_CHAIN,
                        (MPARAM) hwndPrevious,
                        (MPARAM) WinQueryWindow (hwnd, QW_PARENT));

/*****************************************************************************\
 * Free the DLL module and process any window messages that the application  *
 * does not process.                                                         *
\*****************************************************************************/

            DosFreeModule (hModule);

            rValue = FALSE;
            break;

        default:

            rValue = TRUE;
            break;

    }

    return (rValue ? WinDefWindowProc (hwnd, msg, mp1, mp2) : 0L);

}


/***************************** End of IPFWinProc *****************************/


/*---------------------------------------------------------------------------*\
 * Error Message Procedure                                                   *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *  Name:     Error ()                                                       *
 *                                                                           *
 *  Purpose:  To print an error message in a message box.                    *
 *                                                                           *
 *  Usage:    Called from 'IPFMain' and 'IPFWinProc' procedures.             *
 *                                                                           *
 *  Method:   N/A                                                            *
 *                                                                           *
 *  Returns:  N/A                                                            *
 *                                                                           *
\*****************************************************************************/


VOID Error (PCH str)

{

    WinMessageBox (HWND_DESKTOP,
                   HWND_DESKTOP,
                   (PCH)str,
                   (PCH)"IPF Sample Error Message",
                   1,
                   MB_OK | MB_APPLMODAL |
                   MB_MOVEABLE | MB_ICONASTERISK);
}


/************************* End of IPF Sample Program *************************/
