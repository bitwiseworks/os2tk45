
/*
 *
 *
 *   Module Name: BROWSE
 *
 *   OS/2 Work Place Shell Sample Program
 *
 *   Copyright (C) 1993 IBM Corporation
 *
 *       DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *       sample code created by IBM Corporation. This sample code is not
 *       part of any standard or IBM product and is provided to you solely
 *       for  the purpose of assisting you in the development of your
 *       applications.  The code is provided "AS IS", without
 *       warranty of any kind.  IBM shall not be liable for any damages
 *       arising out of your use of the sample code, even if they have been
 *       advised of the possibility of such damages.
 *
 */


#define Browse_O_Matic_Class_Source
#include "browse.ih"
#include "pmbrowse.h"

HMODULE hModule = NULLHANDLE;

/*
 *
 *  METHOD: BrowseOMatic                                   ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Displays a file in the specified format.
 *
 *       BOOL bMode   - Specifies the format of the view. Supported values:
 *                      PMBROWSE_HEX_DISPLAY_MODE
 *                      PMBROWSE_TEXT_DISPLAY_MODE
 *
 *       PSZ  pszName - The name of the file to display. Can't be NULL.
 *
 *  RETURN:
 *
 *       N/A
 *
 */


/*
 * SOM_Scope VOID  SOMLINK Browse_O_Maticwps_BrowseOMatic(Browse_O_Matic *somSelf,
 *                 BOOL bMode,
 *                 PSZ pszName)
 */

/*
 * The prototype for Browse_O_Maticwps_BrowseOMatic was replaced by the following prototype:
 */
SOM_Scope void  SOMLINK Browse_O_Maticwps_BrowseOMatic(Browse_O_Matic *somSelf,
                                                       BOOL bMode,
                                                       PSZ pszName)
{
    HAB           hab;
    HWND          hwndClient = NULLHANDLE;
    HWND          hwndFrame  = NULLHANDLE;
    FRAMECDATA    flFrameData;
    PBROWSEDATA   pWindowData;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_BrowseOMatic");

    if( NULLHANDLE == ( hab = WinQueryAnchorBlock( HWND_DESKTOP)))
       _ErrorHandler( somSelf, "Unable to query the anchor block.", 0L);

    if( FALSE == WinRegisterClass( hab, BROWSE_WINDOW_CLASS, BrowseWndProc,
                                                     0L, sizeof( PBROWSEDATA)))
       _ErrorHandler( somSelf, "Unable to register window class", FALSE);

    /***************************************/
    /* Set up the frame window information */
    /***************************************/
    flFrameData.cb            = sizeof( flFrameData );
    flFrameData.hmodResources = hModule;
    flFrameData.idResources   = ID_ICON;

    flFrameData.flCreateFlags = FCF_TITLEBAR | FCF_MINMAX | FCF_SIZEBORDER |
                               FCF_SYSMENU | FCF_TASKLIST | FCF_SHELLPOSITION |
                                               FCF_VERTSCROLL | FCF_HORZSCROLL;

    /***************************/
    /* Create the frame window */
    /***************************/
    if( NULLHANDLE == ( hwndFrame = WinCreateWindow( HWND_DESKTOP, WC_FRAME,
            APPL_NAME, 0, 0, 0, 0, 0, NULLHANDLE, HWND_TOP, (USHORT) ID_WINDOW,
                                                  (PVOID)&flFrameData, NULL)))
       _ErrorHandler( somSelf, "Unable to create frame window", FALSE);

    /***********************************/
    /* Allocate the window data buffer */
    /***********************************/
    pWindowData = (PBROWSEDATA)_wpAllocMem( somSelf, sizeof(BROWSEDATA), NULL);

    /*************************/
    /* Setup the window data */
    /*************************/
    memset( pWindowData, 0, sizeof( BROWSEDATA));

    pWindowData->cb              = sizeof( BROWSEDATA);
    pWindowData->somSelf         = somSelf;
    pWindowData->UseItem.type    = USAGE_OPENVIEW;
    pWindowData->ViewItem.view   = bMode;
    pWindowData->ViewItem.handle = hwndFrame;
    pWindowData->bDisplay        = bMode;

    if( bMode == PMBROWSE_HEX_DISPLAY_MODE)
         pWindowData->pszApplKey  = "HEX Key";
    else
         pWindowData->pszApplKey  = "TEXT Key";

    /*****************************************************************/
    /* Ask the user for the name of the object if we didn't get one. */
    /*****************************************************************/
    if( pszName != NULL){
         pWindowData->pszObjectName = _DuplicateString( somSelf, pszName);
    }
    else{
         if( FALSE == _GetObjectName( somSelf, hwndFrame,
                                                 &pWindowData->pszObjectName)){
             WinDestroyWindow(hwndFrame);
             return;
         }
    }

    /***************************/
    /* Create the child window */
    /***************************/
    if( NULLHANDLE == ( hwndClient = WinCreateWindow( hwndFrame,
            BROWSE_WINDOW_CLASS, NULL,0, 0, 0, 0, 0, hwndFrame, HWND_TOP,
                                       (USHORT)FID_CLIENT, pWindowData, NULL)))
       _ErrorHandler( somSelf, "Unable to create client window", FALSE);

    /******************************************/
    /* Restore the window position and fonts. */
    /******************************************/
    WinRestoreWindowPos( APPL_NAME, pWindowData->pszApplKey, hwndFrame);
    WinSendMsg( hwndClient, WM_PRESPARAMCHANGED, (MPARAM)PP_FONTNAMESIZE,
                                                                    (MPARAM)0);
    WinSendMsg( hwndFrame, WM_SETICON, MPFROMP( _wpQueryIcon( somSelf)), NULL);

    WinShowWindow( hwndFrame, TRUE);   /* Make the window visible */
}

/*
 *
 *  METHOD: DuplicateString                                ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Duplicates a null terminated string. The buffer is allocated using
 *    wpAllocMem() and must be freed using wpFreeMem().
 *
 *       PSZ  pszString - A NULL terminated string.
 *
 *  RETURN:
 *
 *       A pointer to the copy of pszString.
 *
 */

SOM_Scope PSZ   SOMLINK Browse_O_Maticwps_DuplicateString(Browse_O_Matic *somSelf,
                PSZ pszString)
{
    PSZ pszDuplicate;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_DuplicateString");

    pszDuplicate = (PSZ)_wpAllocMem( somSelf, strlen( pszString), NULL);

    if( pszDuplicate)
       strcpy( pszDuplicate, pszString);

    return( pszDuplicate);
}

/*
 *
 *  METHOD: ErrorHandler                                   ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Displays error information in a PM window.
 *
 *       PSZ    pszInfo - A description of the error condition.
 *       APIRET rc      - The error code from the failing method.
 *
 *  RETURN:
 *
 *       N/A
 *
 */


/*
 * SOM_Scope VOID   SOMLINK Browse_O_Maticwps_ErrorHandler(Browse_O_Matic *somSelf,
 *                 PSZ pszInfo,
 *                 APIRET rc)
 */

/*
 * The prototype for Browse_O_Maticwps_ErrorHandler was replaced by the following prototype:
 */
SOM_Scope void  SOMLINK Browse_O_Maticwps_ErrorHandler(Browse_O_Matic *somSelf,
                                                       PSZ pszInfo,
                                                       APIRET rc)
{
    char  pszOutputBuffer[ 100];

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_ErrorHandler");

    sprintf( pszOutputBuffer, "%s - %ld", pszInfo, rc);
    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, pszOutputBuffer,
                                          "ERROR in BROWSE-O-MATIC", 0, MB_OK);
}

/*
 *
 *  METHOD: FileExists                                     ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Determines if a file exists.
 *
 *       PSZ    pszFileName - A fully qualified path and file name.
 *
 *  RETURN:
 *
 *       TRUE  - The file exists.
 *       FALSE - The file does not exist.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_FileExists(Browse_O_Matic *somSelf,
                PSZ pszFileName)
{
    HDIR          FindHandle = 1;
    FILEFINDBUF3  FindBuffer;
    ULONG         FindCount = 1;
    ULONG         ulFileAttribute = 1;     /* Normal Files */
    BOOL          fExists = FALSE;
    APIRET        rc;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_FileExists");

    rc = DosFindFirst( pszFileName, &FindHandle, ulFileAttribute, &FindBuffer,
                                sizeof( FindBuffer), &FindCount, FIL_STANDARD);

    if( rc!=NO_ERROR && rc!=ERROR_FILE_NOT_FOUND && rc!=ERROR_NO_MORE_FILES){
      _ErrorHandler( somSelf, "Opening log file", rc);
      return( fExists);
    }

    DosFindClose( FindHandle);

    if( rc == NO_ERROR)
      fExists = TRUE;

    return( fExists);
}

/*
 *
 *  METHOD: GetHexLine                                     ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Formats 16 bytes of the object for display.
 *
 *       USHORT usLine       - The number of the line to get.
 *       PBYTE  pBuffer      - Pointer to the buffer where the file is loaded in
 *       ULONG  ulFileLength - The size of the file.
 *       PSZ    pszText      - Buffer to store the formated line in.
 *
 *  RETURN:
 *
 *       The length of the formated display line.
 *
 */

SOM_Scope UINT   SOMLINK Browse_O_Maticwps_GetHexLine(Browse_O_Matic *somSelf,
                USHORT usLine,
                PBYTE pBuffer,
                ULONG ulFileLength,
                PSZ pszText)
{
    PSZ    pObj;
    USHORT usCount;
    UINT   nLength = 0;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_GetHexLine");


    /**********************************/
    /* Point to first byte to display */
    /**********************************/
    pObj = &pBuffer[ (usLine - 1) * HEX_BYTES_PER_LINE];

    sprintf( pszText, "³ %08X ³", pObj - pBuffer);

    /**************************************/
    /* Write the hex values to the buffer */
    /**************************************/
    for( usCount=0; usCount<HEX_BYTES_PER_LINE; usCount++){

       /********************************************/
       /* Put a space after each 4 bytes displayed */
       /********************************************/
       if( usCount % sizeof( LONG) == 0)
          strcat( pszText, " ");

       /********************************************************/
       /* Don't display more bytes than there are in the file. */
       /********************************************************/
       if( usCount + (usLine-1)*HEX_BYTES_PER_LINE < ulFileLength)
          sprintf( &pszText[ strlen( pszText)], "%02.2hx", pObj[ usCount]);
       else
          strcat( &pszText[ strlen( pszText)], "  ");
    }

    strcat( pszText, " ³ ");

    /******************************************************/
    /* Copy the ASCII representation to the output buffer */
    /******************************************************/
    nLength = strlen( pszText);
    memset( &pszText[ nLength], 0x20, HEX_BYTES_PER_LINE);
    memcpy( &pszText[ nLength], pObj, min( HEX_BYTES_PER_LINE,
                              ulFileLength - (usLine-1) * HEX_BYTES_PER_LINE));
    nLength += HEX_BYTES_PER_LINE;
    pszText[ nLength] = 0;
    memcpy( &pszText[ nLength], " ³", 3);
    nLength += 2;

    return( nLength);
}

/*
 *
 *  METHOD: GetLinesAndColumns                             ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Determines the maximum number of lines and columns based on the current
 *    view mode.
 *
 *       BOOL   bDisplay       - The display mode. Supported values:
 *                               PMBROWSE_TEXT_DISPLAY_MODE
 *                               PMBROWSE_HEX_DISPLAY_MODE
 *       PSHORT psTotalLines   - Return total number of lines.
 *       PSHORT psTotalColumns - Return maximum number of columns.
 *       USHORT usNumberOfTextLines - The total number of text lines.
 *       USHORT usNumberOfTextColumns - The maximum number of text columns.
 *       ULONG  ulFileLength   - The length of file.
 *
 *  RETURN:
 *
 *       N/A
 *
 */


/*
 * SOM_Scope VOID   SOMLINK Browse_O_Maticwps_GetLinesAndColumns(Browse_O_Matic *somSelf,
 *                 BOOL bDisplay,
 *                 PSHORT psTotalLines,
 *                 PSHORT psTotalColumns,
 *                 USHORT usNumberOfTextLines,
 *                 USHORT usNumberOfTextColumns,
 *                 ULONG ulFileLength)
 */

/*
 * The prototype for Browse_O_Maticwps_GetLinesAndColumns was replaced by the following prototype:
 */
SOM_Scope void  SOMLINK Browse_O_Maticwps_GetLinesAndColumns(Browse_O_Matic *somSelf,
                                                             BOOL bDisplay,
                                                             PSHORT psTotalLines,
                                                             PSHORT psTotalColumns,
                                                             USHORT usNumberOfTextLines,
                                                             USHORT usNumberOfTextColumns,
                                                             ULONG ulFileLength)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_GetLinesAndColumns");

    /*************************************************************************/
    /* Determine the maximum number of columns and the total number of lines */
    /*************************************************************************/
    if( bDisplay == PMBROWSE_TEXT_DISPLAY_MODE){
       *psTotalLines   = usNumberOfTextLines;
       *psTotalColumns = usNumberOfTextColumns;
    }
    else{
       *psTotalLines = (SHORT)( ulFileLength / HEX_BYTES_PER_LINE +
                                    ( ulFileLength % HEX_BYTES_PER_LINE != 0));
       *psTotalColumns = HEX_LINE_LENGTH;
    }
}

/*
 *
 *  METHOD: GetObjectName                                  ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Prompts the user to enter the name of the object.
 *
 *       HWND   hwnd           - Handle to frame window.
 *       PSZ*   ppszObjectName - Address of pointer for object name.
 *
 *  RETURN:
 *
 *       TRUE  - The user selected an valid file.
 *       FALSE - The user selected cancel.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_GetObjectName(Browse_O_Matic *somSelf,
                HWND hwnd,
                PSZ *ppszObjectName)
{
    FILEDLG  fild;
    PSZ      pszDialogTitle = "Object Name";
    PSZ      pszButton      = "Browse";
    PSZ      pszInitialPath = "*.*";
    HWND     hwndDlg;
    BOOL     bOpenFile = FALSE;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_GetObjectName");

    /*****************************************************************/
    /* Initially set all fields to 0                                 */
    /*****************************************************************/
    memset( &fild, 0, sizeof(FILEDLG));

    fild.pszTitle    = pszDialogTitle;
    fild.pszOKButton = pszButton;
    fild.fl          = FDS_CENTER | FDS_OPEN_DIALOG;
    fild.cbSize      = sizeof( FILEDLG);
    fild.pfnDlgProc  = (PFNWP)OpenFilterProc;
    strcpy( fild.szFullFile, pszInitialPath);

    /**********************************************/
    /* Get the name of the log file from the user */
    /**********************************************/
    hwndDlg = WinFileDlg( HWND_DESKTOP, HWND_DESKTOP, &fild);
    if( ( hwndDlg != 0) && ( fild.lReturn == DID_OK)){

       /*********************************/
       /* Determine if the file exists. */
       /*********************************/
       if( _FileExists( somSelf, fild.szFullFile)){

          *ppszObjectName = _DuplicateString( somSelf, fild.szFullFile);
          bOpenFile = TRUE;
       }
       else
          WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, *ppszObjectName,
                              "BROWSE-O-MATIC - Object Not Found", 0, MB_OK);
    }

    return( bOpenFile);
}

/*
 *
 *  METHOD: GetTextLine                                    ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Copies a line of text from the object to a buffer.
 *
 *       USHORT      usLine  - The number of the line to get.
 *       LINERECORD* pLines  - Pointer to the line record array.
 *       PSZ         pszText - Buffer to store the text in.
 *
 *  RETURN:
 *
 *       The length of the line to display.
 *
 */

SOM_Scope UINT   SOMLINK Browse_O_Maticwps_GetTextLine(Browse_O_Matic *somSelf,
                USHORT usLine,
                LINERECORD *pLines,
                PSZ pszText)
{
    UINT uLineLength;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_GetTextLine");

    uLineLength = min( MAX_LINE_LENGTH, pLines[ usLine - 1].unLength);

    /*******************************************/
    /* Copy the text line to the output buffer */
    /*******************************************/
    strncpy( pszText, pLines[ usLine-1].pszLine, uLineLength);

    return( uLineLength);
}

/*
 *
 *  METHOD: LoadObject                                     ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Loads a file into memory and determines the number of text line and
 *    maximum number of text columns.
 *
 *       PSZ     pszObjectName  - Name of the file to load.
 *       PULONG  pulLength      - returns the length of the file.
 *       PBYTE*  ppBuffer       - returns a pointer to the file in memory.
 *       LINERECORD** pLines    - returns a pointer to a line record array.
 *       PUSHORT pusNumberOfTextLines   - returns the number of text lines.
 *       PUSHORT pusNumberOfTextColumns - returns the maximum number of text
 *                                                                      columns.
 *
 *  RETURN:
 *
 *       TRUE  - Loaded object correctly
 *       FALSE - Unable to load object
 */

/*
 *
 * METHOD: LoadObject                                     ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Loads a file into memory and determines the number of text line and
 *   maximum number of text columns.
 *
 *      PSZ     pszObjectName  - Name of the file to load.
 *      PULONG  pulLength      - returns the length of the file.
 *      PBYTE*  ppBuffer       - returns a pointer to the file in memory.
 *      LINERECORD** pLines    - returns a pointer to a line record array.
 *      PUSHORT pusNumberOfTextLines   - returns the number of text lines.
 *      PUSHORT pusNumberOfTextColumns - returns the maximum number of text
 *                                                                     columns.
 *
 * RETURN:
 *
 *      N/A
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_LoadObject(Browse_O_Matic *somSelf,
                PSZ pszObjectName,
                PULONG pulLength,
                PBYTE *ppBuffer,
                LINERECORD **ppLines,
                PUSHORT pusNumberOfTextLines,
                PUSHORT pusNumberOfTextColumns)
{
    UINT        unLength = 0;
    HFILE       hFile;
    USHORT      usCount = 0;
    ULONG       Action;
    ULONG       ulOffset = 0;
    ULONG       ulBytesRead = -1;
    FILESTATUS3 FileInfo;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_LoadObject");

    *pulLength              = 0;
    *pusNumberOfTextLines   = 0;
    *pusNumberOfTextColumns = 0;

    if( NO_ERROR != DosOpen( pszObjectName, &hFile, &Action, 0,
             FILE_NORMAL, OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                                OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY, 0))
      return( FALSE);

    /*************************************************************/
    /* Allocate a buffer large enough for the file system object */
    /*************************************************************/
    DosQueryFileInfo( hFile, FIL_STANDARD, &FileInfo, sizeof( FileInfo));
    *pulLength = FileInfo.cbFile;
    if(NULL == ( *ppBuffer = (PBYTE)_wpAllocMem( somSelf, *pulLength, NULL))){
       DosClose( hFile);
       return( FALSE);
    }

    /**************************************/
    /* Load the entire file system object */
    /**************************************/
    DosRead( hFile, *ppBuffer, *pulLength, &ulBytesRead);
    DosClose( hFile);

    /************************************************/
    /* Count the number of text lines in the object */
    /************************************************/
    for( ulOffset = 0; ulOffset < ulBytesRead; ulOffset++){

       if( (*ppBuffer)[ ulOffset] == '\n')
          (*pusNumberOfTextLines)++;
    }

    /***************************************************************/
    /* Allocate a buffer for an array of character pointers. These */
    /* point to the beginning of each line.                        */
    /***************************************************************/
    if( NULL == ( *ppLines = (LINERECORD*)_wpAllocMem( somSelf,
                        sizeof( LINERECORD) * (*pusNumberOfTextLines), NULL))){
       _wpFreeMem( somSelf, *ppBuffer);
       return( FALSE);
    }

    /****************************************************************/
    /* Store the address of the beginning of each text line and the */
    /* length of each text line in the array.                       */
    /****************************************************************/
    usCount = 0;
    (*ppLines)[ usCount].pszLine = *ppBuffer;
    *pusNumberOfTextColumns = 0;

    for( ulOffset=0; ulOffset<ulBytesRead; ulOffset++){

       if( (*ppBuffer)[ ulOffset] != '\r' && (*ppBuffer)[ ulOffset] != '\n')
          unLength++;

       if( (*ppBuffer)[ ulOffset] == '\n'){

          (*ppLines)[ usCount].unLength = unLength;
          usCount++;

          if( ulOffset+1<ulBytesRead && usCount <= *pusNumberOfTextLines){

             (*ppLines)[ usCount].pszLine = (PSZ)&(*ppBuffer)[ulOffset+1];
             *pusNumberOfTextColumns = max( *pusNumberOfTextColumns, unLength);
          }

          unLength = 0;
       }
    }

    return( TRUE);
}

/*
 *
 * METHOD: ProductInformationView                         ( ) PRIVATE
 *                                                        (X) PUBLIC
 * DESCRIPTION:
 *
 *   Displays the product information view.
 *
 * RETURN:
 *
 *      N/A
 *
 */

SOM_Scope void  SOMLINK Browse_O_Maticwps_ProductInformationView(Browse_O_Matic *somSelf)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_ProductInformationView");

}

/*
 *
 *  METHOD: UnloadObject                                   ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Unloads a file that was loaded. The buffer file and the LINERECORD array
 *    are freed.
 *
 *       PBYTE      ppBuffer - a pointer to the file in memory.
 *       LINERECORD pLines   - a pointer to a line record array.
 *
 *  RETURN:
 *
 *       N/A
 *
 */


/*
 * SOM_Scope VOID   SOMLINK Browse_O_Maticwps_UnloadObject(Browse_O_Matic *somSelf,
 *                 PBYTE pBuffer,
 *                 LINERECORD *pLines)
 */

/*
 * The prototype for Browse_O_Maticwps_UnloadObject was replaced by the following prototype:
 */
SOM_Scope void  SOMLINK Browse_O_Maticwps_UnloadObject(Browse_O_Matic *somSelf,
                                                       PBYTE pBuffer,
                                                       LINERECORD* pLines)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_UnloadObject");

    if( pBuffer)
       _wpFreeMem( somSelf, ( PBYTE)pBuffer);

    if( pLines)
       _wpFreeMem( somSelf, ( PBYTE)pLines);
}

/*
 *
 *  OVERRIDE: wpDragOver                                   ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Rejects objects that are not file system objects from being dropped
 *    on the Browse_O_matic.
 *
 */

SOM_Scope MRESULT   SOMLINK Browse_O_Maticwps_wpDragOver(Browse_O_Matic *somSelf,
                HWND hwndCnr,
                PDRAGINFO pdrgInfo)
{
    MRESULT mResult;
    ULONG   ulCount;
    ULONG   ulNumberOfObjects;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpDragOver");

    /********************************************************************/
    /* Don't call the parent. Initialize mResult to allow the drag over */
    /* to proceed.                                                      */
    /********************************************************************/
    mResult = MRFROM2SHORT( DOR_DROP, DO_MOVE);

    /*******************************************************************/
    /* Determine the number of objects dragged over the Browse-O-Matic */
    /*******************************************************************/
    ulNumberOfObjects = DrgQueryDragitemCount( pdrgInfo);

    /*************************/
    /* Check all the objects */
    /*************************/
    for( ulCount=0; ulCount < ulNumberOfObjects &&
                           SHORT1FROMMR( mResult) != DOR_NEVERDROP; ulCount++){

        /*****************************************/
        /* It must be a file system type object. */
        /*****************************************/
        if( DrgVerifyRMF( DrgQueryDragitemPtr( pdrgInfo, ulCount),
                                                         "DRM_OS2FILE", NULL))
            mResult = MRFROM2SHORT( DOR_DROP, SHORT2FROMMR( mResult));
        else
            mResult = MRFROM2SHORT( DOR_NEVERDROP, SHORT2FROMMR( mResult));
    }

    return( mResult);
}

/*
 *
 *  OVERRIDE: wpDrop                                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Rejects objects that are not file system objects from being dropped
 *    on the Browse_O_matic.
 *
 */

SOM_Scope MRESULT   SOMLINK Browse_O_Maticwps_wpDrop(Browse_O_Matic *somSelf,
                HWND hwndCnr,
                PDRAGINFO pdrgInfo,
                PDRAGITEM pdrgItem)
{
    CHAR    pszBuffer[ 255];
    MRESULT mResult;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpDrop");

    mResult = MRFROM2SHORT( DOR_DROP, 0);

    /***************************************************************/
    /* Don't call the parent. Initialize mResult to allow the drop */
    /* to proceed.                                                 */
    /***************************************************************/
    if( DOR_NEVERDROP != SHORT1FROMMR( mResult) &&
                                 DrgVerifyRMF( pdrgItem, "DRM_OS2FILE", NULL)){

        /* Get the path */
        DrgQueryStrName( pdrgItem->hstrContainerName, sizeof( pszBuffer),
                                                                    pszBuffer);
        /* Append the name of the object to the path */
        DrgQueryStrName( pdrgItem->hstrSourceName,
                                       sizeof( pszBuffer) - strlen( pszBuffer),
                                              &pszBuffer[ strlen( pszBuffer)]);
        _wpViewObject( somSelf, NULLHANDLE, OPEN_DEFAULT, (ULONG)pszBuffer);

        mResult = MRFROM2SHORT( DOR_DROP, SHORT2FROMMR( mResult));
    }
    else
        mResult = MRFROMSHORT( DOR_NEVERDROP);

    return( mResult);
}

/*
 *
 *  OVERRIDE: wpMenuItemHelpSelected                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Provide help for the Hex and Text view popup menu selections.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_wpMenuItemHelpSelected(Browse_O_Matic *somSelf,
                ULONG MenuId)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpMenuItemHelpSelected");

    switch( MenuId){

      case ID_OPEN_TEXT:
         return( _wpDisplayHelp( somSelf, PANEL_HELP_OPEN_TEXT, "browse.hlp"));

      case ID_OPEN_HEX:
         return( _wpDisplayHelp( somSelf, PANEL_HELP_OPEN_HEX, "browse.hlp"));

      default:
         break;
    }

    return( FALSE);
}

/*
 *
 *  OVERRIDE: wpMenuItemSelected                           ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Opens Hex and Text views.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_wpMenuItemSelected(Browse_O_Matic *somSelf,
                HWND hwndFrame,
                ULONG ulMenuId)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpMenuItemSelected");

    switch( ulMenuId){

       case ID_OPEN_TEXT:
          _wpViewObject( somSelf, NULLHANDLE, ulMenuId, 0L);
          break;

       case ID_OPEN_HEX:
          _wpViewObject( somSelf, NULLHANDLE, ulMenuId, 0L);
          break;

       case ID_OPEN_PRODUCTINFO:
       case ID_PRODUCTINFOMENU:
          _wpViewObject( somSelf, NULLHANDLE, ulMenuId, 0L);

       default:
          return( parent_wpMenuItemSelected( somSelf, hwndFrame, ulMenuId));
    }

    return( TRUE);
}

/*
 *
 *  OVERRIDE: wpModifyPopupMenu                            ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Adds selections for Hex and Text view to the popup menu.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_wpModifyPopupMenu(Browse_O_Matic *somSelf,
                HWND hwndMenu,
                HWND hwndCnr,
                ULONG iPosition)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpModifyPopupMenu");

    /* Put open menu with the other views */
    _wpInsertPopupMenuItems( somSelf, hwndMenu, 0,
                                          hModule, ID_OPENMENU, WPMENUID_OPEN);

    /* Add product information */
    _wpInsertPopupMenuItems( somSelf, hwndMenu, 0,
                                   hModule, ID_PRODUCTINFOMENU, WPMENUID_HELP);


    return (parent_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition));
}

/*
 *
 *  OVERRIDE: wpOpen                                       ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Opens the Hex and Text views.
 *
 */

SOM_Scope HWND   SOMLINK Browse_O_Maticwps_wpOpen(Browse_O_Matic *somSelf,
                HWND hwndCnr,
                ULONG ulView,
                ULONG param)
{
    HWND hwnd = NULLHANDLE;

    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpOpen");

    switch( ulView){

        case ID_OPEN_HEX:
          /*************************************************************/
          /* param is the name of the object to open a view for. This  */
          /* parameter is reserved for views supported by the WPObject */
          /* class but can be used for views provided by new classes.  */
          /*************************************************************/
          _BrowseOMatic( somSelf, PMBROWSE_HEX_DISPLAY_MODE, (PSZ)param);
          break;

        case ID_OPEN_TEXT:
          /*************************************************************/
          /* param is the name of the object to open a view for. This  */
          /* parameter is reserved for views supported by the WPObject */
          /* class but can be used for views provided by new classes.  */
          /*************************************************************/
          _BrowseOMatic( somSelf, PMBROWSE_TEXT_DISPLAY_MODE, (PSZ)param);
          break;

        case ID_OPEN_PRODUCTINFO:
          WinDlgBox( HWND_DESKTOP, HWND_DESKTOP, (PFNWP)AboutBoxDlgProc,
                                         hModule, ID_PRODUCTINFO, (PVOID)NULL);

        default:
           hwnd = parent_wpOpen( somSelf, hwndCnr, ulView, param);
           break;
    }

    return( hwnd);
}

/*
 *
 *  OVERRIDE: wpSetup                                      ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Initializes the type of the instance of the Browse_O_Matic object to DLL.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_Maticwps_wpSetup(Browse_O_Matic *somSelf,
                PSZ pszSetupString)
{
    /* Browse_O_MaticData *somThis = Browse_O_MaticGetData(somSelf); */
    Browse_O_MaticMethodDebug("Browse_O_Matic","Browse_O_Maticwps_wpSetup");

    _wpSetType( somSelf, DRT_DLL, NULL);

    /************************************************/
    /* Allow mulitple secondary views of the object */
    /************************************************/
    _wpSetConcurrentView( somSelf, CCVIEW_ON);

    return (parent_wpSetup(somSelf,pszSetupString));
}

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
/*
 *
 *  METHOD: clsQueryModuleHandle                           ( ) PRIVATE
 *                                                         (X) PUBLIC
 *  DESCRIPTION:
 *
 *    Gets the module handle.
 *
 *  RETURN:
 *       NULLHANDLE     - an error occured.
 *       Not NULLHANDLE - The handle for DLL containing the Browse_O_Matic
 *                        class.
 *
 */

SOM_Scope HMODULE   SOMLINK Browse_O_MaticM_clsQueryModuleHandle(M_Browse_O_Matic *somSelf)
{
    PSZ            pszLibraryName;

    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_clsQueryModuleHandle");

    /* Only get the handle once */
    if( hModule == NULLHANDLE){

       /* Get the name of the library so we can get it's handle. */
       if( NULL != ( pszLibraryName = _somLocateClassFile( SOMClassMgrObject,
             SOM_IdFromString( "Browse_O_Matic"), Browse_O_Matic_MajorVersion,
                                                 Browse_O_Matic_MinorVersion)))

           /* Get the handle for the library. */
           DosQueryModuleHandle( pszLibraryName, &hModule);
    }

    return ( hModule);
}

/*
 *
 *  METHOD: wpclsInitData
 *
 *  DESCRIPTION:
 *
 *    Initalize the class data
 *
 */

SOM_Scope void   SOMLINK Browse_O_MaticM_wpclsInitData(M_Browse_O_Matic *somSelf)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsInitData");

    hModule = _clsQueryModuleHandle( somSelf);

    parent_wpclsInitData(somSelf);
}

/*
 *
 *  METHOD: wpclsQueryDefaultHelp
 *
 *  DESCRIPTION:
 *
 *    Provides general help for the Browse_O_Matic instance.
 *
 */

SOM_Scope BOOL   SOMLINK Browse_O_MaticM_wpclsQueryDefaultHelp(M_Browse_O_Matic *somSelf,
                PULONG pHelpPanelId,
                PSZ pszHelpLibrary)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsQueryDefaultHelp");

    if (pHelpPanelId)                           /* set default help panel id */
       *pHelpPanelId = PANEL_MAIN;

    if( pszHelpLibrary)                         /* copy help filename */
       strcpy( pszHelpLibrary, "browse.hlp");

    return( TRUE);
}

/*
 *
 *  METHOD: wpclsQueryDefaultView
 *
 *  DESCRIPTION:
 *
 *    Make the default view the Hex view. The user may change this through
 *    the settings for each instance of the Browse_O_Matic.
 *
 */

SOM_Scope ULONG   SOMLINK Browse_O_MaticM_wpclsQueryDefaultView(M_Browse_O_Matic *somSelf)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsQueryDefaultView");

    return( ID_OPEN_HEX);
}

/*
 *
 *  METHOD: wpclsQueryIconData
 *
 *  DESCRIPTION:
 *
 *    Provide a icon for each instance of the Browse_O_Matic.
 *
 */

SOM_Scope ULONG   SOMLINK Browse_O_MaticM_wpclsQueryIconData(M_Browse_O_Matic *somSelf,
                PICONINFO pIconInfo)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsQueryIconData");

    if( pIconInfo){

       pIconInfo->fFormat = ICON_RESOURCE;
       pIconInfo->resid   = ID_ICON;
       pIconInfo->hmod    = hModule;
    }

    return ( sizeof( ICONINFO));
}

/*
 *
 *  METHOD: wpclsQueryStyle
 *
 *  DESCRIPTION:
 *
 *    Set style to never print so that Print is not available on the menu.
 *
 */

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
SOM_Scope ULONG   SOMLINK Browse_O_MaticM_wpclsQueryStyle(M_Browse_O_Matic *somSelf)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsQueryStyle");

    return (parent_wpclsQueryStyle(somSelf) | CLSSTYLE_NEVERPRINT);
}

/*
 *
 *  METHOD: wpclsQueryTitle
 *
 *  DESCRIPTION:
 *
 *    Provide a default title of "rowse-O-Matic".
 *
 */

SOM_Scope PSZ   SOMLINK Browse_O_MaticM_wpclsQueryTitle(M_Browse_O_Matic *somSelf)
{
    /* M_Browse_O_MaticData *somThis = M_Browse_O_MaticGetData(somSelf); */
    M_Browse_O_MaticMethodDebug("M_Browse_O_Matic","Browse_O_MaticM_wpclsQueryTitle");

    return( "Browse-O-Matic");
}

