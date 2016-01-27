
/*
 *
 *
 *   Module Name: TEXTFLDR
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


#define TextFolder_Class_Source
#include "textfldr.ih"

/*
 *
 *  METHOD: IsTextFile                                     (X) PRIVATE
 *                                                         ( ) PUBLIC
 *  DESCRIPTION:
 *
 *    This method checks the first 'n' characters of a file to determine
 *    if it is or is not text. This method is invoked by the
 *    ValidateDragAndDrop method. It is invoked only if no type is specified
 *    for the object being manipulated.  A file is considered to be text if
 *    the first 'n' bytes are in the range of ASCII 32 (20 Hex) to ASCII 126
 *    (7E Hex), or is a carriage return (ASCII 10, Hex 0A), line feed
 *    (ASCII 13, Hex 0D), tab (ASCII 8, HEX 08), or end of file marker
 *    (ASCII 26, Hex 1A).
 *
 *    NOTE: The value used for 'n' is stored in the variable ulSampleSize
 *    which is local to the IsTextFile method.
 *
 *  RETURN:
 *
 *    TRUE  - the object is a text file.
 *    FALSE - the object is not a text file.
 *
 *  HOW TO OVERRIDE:
 *
 *    n/a
 *
 */

SOM_Scope BOOL   SOMLINK TextFolderwps_IsTextFile(TextFolder *somSelf,
                PSZ pszFileName)
{
    ULONG   Action;              /* The action performed on the file */
    HFILE   hFile;               /* The file handle */
    ULONG   ulSampleSize = 500L; /* Number of bytes to test */
    ULONG   ulBytesRead;         /* Number of sample bytes read */
    ULONG   ulCount;             /* Counter variable */
    PSZ     pszSample = NULL;    /* The sample buffer */
    CHAR    cChar;               /* Current character from sample buffer */
    APIRET  aRC;                 /* Return code from API calls */
    BOOL    bRC = FALSE;         /* True - it is a text file */
                                 /* False - it isn't not a text file */

    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_IsTextFile");


    /* Open the file that contains the object */
    if( NO_ERROR != ( aRC=DosOpen( pszFileName,        /* File path name */
                             &hFile,                   /* File handle */
                             &Action,                  /* Action taken */
                             0L,                       /* File size */
                             FILE_NORMAL,              /* File attribute */
                             OPEN_ACTION_FAIL_IF_NEW | /* Open function type */
                             OPEN_ACTION_OPEN_IF_EXISTS,
                             OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYNONE |
                             OPEN_ACCESS_READONLY, 0L)))
       somPrintf( "Unable to open file %s (%ld)\r\n", pszFileName, aRC);

    else{

       /* Allocate buffer for reading a sample of the object into. */
       if( NO_ERROR != (aRC = DosAllocMem( (PPVOID)&pszSample, ulSampleSize,
                                           PAG_COMMIT | PAG_READ | PAG_WRITE)))
          somPrintf( "Unable to allocate buffer of %ld bytes (%ld)\r\n",
                                                            ulSampleSize, aRC);
       else{

          if( NO_ERROR != ( aRC = DosRead( hFile, pszSample, ulSampleSize,
                                                               &ulBytesRead)))
             somPrintf( "Unable to read %ld bytes from file %s (%ld)\r\n",
                                               ulSampleSize, pszFileName, aRC);
          else{

             /* Check all the characters read in the sample buffer */
             for( ulCount=0, bRC=TRUE; (ulCount<ulBytesRead) && (bRC == TRUE);
                                                                    ulCount++){

                cChar = pszSample[ ulCount];

                /* Make sure the characters are text characters,   */
                /* carriage return, line feed, tab, or end of file */

                if( ((cChar < FIRST_TEXT_CHAR) || (cChar > LAST_TEXT_CHAR)) &&
                   (cChar != CHAR_CARRIAGE_RETURN) && (cChar != CHAR_LINE_FEED)
                        && (cChar != CHAR_TAB) && (cChar != CHAR_END_OF_FILE)){
                    bRC = FALSE;
                }
             }
          }

          /* Clean up */
          DosFreeMem( pszSample);
          DosClose( hFile);
       }
    }

    return( bRC);
}

/*
 *
 *  METHOD: ValidateDragAndDrop                            (X) PRIVATE
 *                                                         ( ) PUBLIC
 *  DESCRIPTION:
 *
 *    This method determines if an object can be dragged/dropped on the
 *    TextFolder. It is invoked by the wpDrag and wpDragOver methods.
 *
 *    The ValidateDragAndDrop method first checks the type of the object
 *    being manipulated. If a type is specified, and it is Plain Text,
 *    the operation is considered to be valid. If a type is specified and
 *    it is not Plain Text the operation is considered invalid. If no
 *    type is specified the IsTextFile() method is invoked to determine
 *    if the object is or is not a text object.
 *
 *  RETURN:
 *
 *    TRUE  - The object can be dragged/dropped on the TextFolder.
 *    FALSE - The object can not be dragged/dropped on the TextFolder.
 *
 *  HOW TO OVERRIDE:
 *
 *    n/a
 *
 */

/*
 *
 * METHOD: ValidateDragAndDrop                            (X) PRIVATE
 *                                                        ( ) PUBLIC
 * DESCRIPTION:
 *
 *   This method determines if an object can be dragged/dropped on the
 *   TextFolder. It is invoked by the wpDrag and wpDragOver methods.
 *
 *   The ValidateDragAndDrop method first checks the type of the object
 *   being manipulated. If a type is specified, and it is Plain Text,
 *   the operation is considered to be valid. If a type is specified and
 *   it is not Plain Text the operation is considered invalid. If no
 *   type is specified the IsTextFile() method is invoked to determine
 *   if the object is or is not a text object.
 *
 * RETURN:
 *
 *   TRUE  - The object can be dragged/dropped on the TextFolder.
 *   FALSE - The object can not be dragged/dropped on the TextFolder.
 *
 * HOW TO OVERRIDE:
 *
 *   n/a
 *
 *
 *   Methods from the WPFileSystem class
 *
 */

SOM_Scope BOOL   SOMLINK TextFolderwps_ValidateDragAndDrop(TextFolder *somSelf,
                PDRAGINFO pdrgInfo)
{
    CHAR    pszBuffer[ 255];     // Buffer for file system object name
    ULONG   ulNumberOfObjects;   // Number of object draged over object
    ULONG   ulCurrentObject;     // Used to increment list of objects
    BOOL    bObjectType;         // Type of object

    PDRAGITEM pDragItem;

    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_ValidateDragAndDrop");

    /* Get the number of objects being dragged */
    ulNumberOfObjects = DrgQueryDragitemCount( pdrgInfo);

    /* Test each object */
    for( ulCurrentObject = 0; ulCurrentObject<ulNumberOfObjects;
                                                            ulCurrentObject++){
        pDragItem = DrgQueryDragitemPtr( pdrgInfo, ulCurrentObject);

        /* If the object isn't Plain Text check to see if any type is */
        /* associated with the object. If no type is associated then  */
        /* check the contents of the file to see if it is text only.  */
        if( FALSE == DrgVerifyType( pDragItem, DRT_TEXT)){

            /* If the associated type is known, this isn't a text      */
            /* object. If it isn't known, check the object's contents. */
            bObjectType = DrgVerifyType( pDragItem, "Reserved");

            /* Get the path */
            DrgQueryStrName( pDragItem->hstrContainerName, sizeof( pszBuffer),
                                                                    pszBuffer);
            /* Append the name of the object to the path */
            DrgQueryStrName( pDragItem->hstrSourceName,
                                       sizeof( pszBuffer) - strlen( pszBuffer),
                                              &pszBuffer[ strlen( pszBuffer)]);

            /* If a type was specified, it isn't Plain Text. */
            /* If no type was specified check if it is a text file. */
            if( bObjectType == FALSE || ( bObjectType == TRUE &&
                                 FALSE == _IsTextFile( somSelf, pszBuffer)) ){

                somPrintf( "This is not a Plain Text object. ");
                DrgQueryStrName( pDragItem->hstrType, sizeof( pszBuffer),
                                                                    pszBuffer);
                somPrintf( "The types for this object are: '%s'\n", pszBuffer);

                return( FALSE);
            }
        }
    }

    return( TRUE);
}

/*
 *
 *   METHOD: wpFilterPopupMenu                              ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Removes the Tree view option from the Open popup menu.
 *
 */

SOM_Scope ULONG   SOMLINK TextFolderwps_wpFilterPopupMenu(TextFolder *somSelf,
                ULONG ulFlags,
                HWND hwndCnr,
                BOOL fMultiSelect)
{
    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_wpFilterPopupMenu");

    ulFlags = parent_wpFilterPopupMenu(somSelf,ulFlags,hwndCnr,fMultiSelect);

    /* Remove the tree view menu option and make sure delete is available */
    return ( ( ulFlags | CTXT_DELETE) & ~CTXT_TREE);
}

/*
 *
 *   METHOD: wpAddFolderView2Page                           ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Removes the tree view settings page for the TextFolder.
 *
 */

SOM_Scope ULONG   SOMLINK TextFolderwps_wpAddFolderView2Page(TextFolder *somSelf,
                HWND hwndNotebook)
{
    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_wpAddFolderView2Page");

    return( SETTINGS_PAGE_REMOVED);
}

/*
 *
 *   METHOD: wpAddFolderIncludePage                         ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Removes the Include Page from the TextFolder's settings.
 *
 */

SOM_Scope ULONG   SOMLINK TextFolderwps_wpAddFolderIncludePage(TextFolder *somSelf,
                HWND hwndNotebook)
{
    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_wpAddFolderIncludePage");

    return ( SETTINGS_PAGE_REMOVED);
}

/*
 *
 *   METHOD: wpDragOver                                     ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Allows or disallows an object to be dragged over the TextFolder.
 *
 */

SOM_Scope MRESULT   SOMLINK TextFolderwps_wpDragOver(TextFolder *somSelf,
                HWND hwndCnr,
                PDRAGINFO pdrgInfo)
{
    MRESULT mResult;

    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_wpDragOver");

    mResult = parent_wpDragOver(somSelf,hwndCnr,pdrgInfo);

    /* If the parent said it is okay to drop then we will check it also. */
    if( DOR_NEVERDROP != SHORT1FROMMR( mResult)){

        /* If the object is acceptable, return the mResult from the parent */
        if( FALSE == _ValidateDragAndDrop( somSelf, pdrgInfo))
            mResult = MRFROM2SHORT( DOR_NEVERDROP, DO_UNKNOWN);
    }

    return ( mResult);
}

/*
 *
 *   METHOD: wpDrop                                         ( ) PRIVATE
 *                                                          (X) PUBLIC
 *   DESCRIPTION:
 *
 *     Allows or disallows an object to be dropped on the TextFolder.
 *
 */

SOM_Scope MRESULT   SOMLINK TextFolderwps_wpDrop(TextFolder *somSelf,
                HWND hwndCnr,
                PDRAGINFO pdrgInfo,
                PDRAGITEM pdrgItem)
{
    MRESULT mResult;

    /* TextFolderData *somThis = TextFolderGetData(somSelf); */
    TextFolderMethodDebug("TextFolder","TextFolderwps_wpDrop");

    mResult = parent_wpDrop(somSelf,hwndCnr,pdrgInfo,pdrgItem);

    /* If the parent said it is okay to drop then we will check it also */
    if( DOR_NEVERDROP != SHORT1FROMMR( mResult)){


        /* Validate the objects */
        if( FALSE == _ValidateDragAndDrop( somSelf, pdrgInfo))
            mResult = MRFROM2SHORT( DOR_NEVERDROP, DO_UNKNOWN);
    }

    return ( mResult);
}

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
/*
 *
 *  METHOD: wpclsQueryTitle
 *
 *  DESCRIPTION:
 *
 *    Provides a default name of Text Folder for TextFolder.
 *
 */

SOM_Scope PSZ   SOMLINK TextFolderM_wpclsQueryTitle(M_TextFolder *somSelf)
{
    /* M_TextFolderData *somThis = M_TextFolderGetData(somSelf); */
    M_TextFolderMethodDebug("M_TextFolder","TextFolderM_wpclsQueryTitle");

    return ( "Text Folder");
}

/*
 *
 *  METHOD: wpclsQueryIconData
 *
 *  DESCRIPTION:
 *
 *    Provides an icon for the TextFolder.
 *
 */

SOM_Scope ULONG   SOMLINK TextFolderM_wpclsQueryIconData(M_TextFolder *somSelf,
                PICONINFO pIconInfo)
{
    static HMODULE hModule = NULLHANDLE;
    PSZ            pszLibraryName;

    /* M_TextFolderData *somThis = M_TextFolderGetData(somSelf); */
    M_TextFolderMethodDebug("M_TextFolder","TextFolderM_wpclsQueryIconData");

    /* Only get the handle once */
    if( (hModule == NULLHANDLE) && pIconInfo){

       /* Get the name of the library so we can get it's handle. */
       if( NULL != ( pszLibraryName = _somLocateClassFile( SOMClassMgrObject,
                      SOM_IdFromString( "TextFolder"), TextFolder_MajorVersion,
                                                     TextFolder_MinorVersion)))

           /* Get the handle for the library. */
           DosQueryModuleHandle( pszLibraryName, &hModule);
    }

    if( pIconInfo){

       pIconInfo->fFormat = ICON_RESOURCE;
       pIconInfo->resid   = ID_ICON;
       pIconInfo->hmod    = hModule;
    }

    return ( sizeof( ICONINFO));
}

SOM_Scope PSZ  SOMLINK TextFolderwps_wpclsQueryTitle(M_TextFolder *somSelf)
{
    /* M_TextFolderData *somThis = M_TextFolderGetData(somSelf); */
    M_TextFolderMethodDebug("M_TextFolder","TextFolderwps_wpclsQueryTitle");

    return (M_TextFolder_parent_M_WPFolder_wpclsQueryTitle(somSelf));
}

SOM_Scope ULONG  SOMLINK TextFolderwps_wpclsQueryIconData(M_TextFolder *somSelf, 
                                                          PICONINFO pIconInfo)
{
    /* M_TextFolderData *somThis = M_TextFolderGetData(somSelf); */
    M_TextFolderMethodDebug("M_TextFolder","TextFolderwps_wpclsQueryIconData");

    return (M_TextFolder_parent_M_WPFolder_wpclsQueryIconData(somSelf, 
                                                              pIconInfo));
}

