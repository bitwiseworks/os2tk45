/**********************************************************************
*   REXXUTIL.C                                                        *
*                                                                     *
*   This program extends the REXX language by providing many          *
*   REXX external functions.                                          *
*   This sample program contains a subset of the functions            *
*   provided in the version of REXXUTIL.DLL that is included in       *
*   the base OS/2 product.                                            *
*   These functions are:                                              *
*       SysCls              -- Clear the screen in an OS/2 fullscreen *
*                              or windowed command prompt session.    *
*       SysCurPos           -- Set and/or Query the cursor position   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysCurState         -- Make the cursor visible or invisible   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysDriveInfo        -- Returns information about a specific   *
*                              drive.                                 *
*       SysDriveMap         -- Returns a list of the drives on the    *
*                              machine                                *
*       SysDropFuncs        -- Makes all functions in this package    *
*                              unknown to REXX.                       *
*       SysFileDelete       -- Deletes a file                         *
*       SysFileSearch       -- Searches for a file matching a given   *
*                              filespec.                              *
*       SysFileTree         -- Searches for files matching a given    *
*                              filespec, including files in           *
*                              subdirectories.                        *
*       SysGetMessage       -- Retrieves a message text from an OS/2  *
*                              message file, substituting values      *
*                              provided.                              *
*       SysIni              -- Reads and/or updates entries in .INI   *
*                              files.                                 *
*       SysLoadFuncs        -- Makes all functions in this package    *
*                              known to REXX so REXX programs may     *
*                              call them.                             *
*       SysMkDir            -- Creates a directory                    *
*       SysOS2Ver           -- Returns the OS/2 Version number        *
*       SysRmDir            -- Removes a directory                    *
*       SysSearchPath       -- Searches throught a specified path     *
*                              for a file.                            *
*       SysSleep            -- Suspends execution for a number of     *
*                              seconds.                               *
*       SysTempFileName     -- Creates a unique filename              *
*       SysTextScreenRead   -- Reads characters from the screen,      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysTextScreenSize   -- Returns the size of the window in      *
*                              rows and columns,                      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysGetEA            -- Reads an extended attribute            *
*                              for a file.                            *
*       SysPutEA            -- Writes an extended attribute           *
*                              for a file.                            *
*       SysWaitNamedPipe    -- Wait on a named pipe.                  *
*       SysMakeNamedPipe    -- Create a named pipe.                   *
*       SysConnectNamedPipe -- Prepare a named pipe for connection.   *
*       SysDisConnectNamedPipe -- Release a pipe connection.          *
*                                                                     *
*   To compile:    MAKE REXXUTIL                                      *
*                                                                     *
**********************************************************************/
/* Include files */
 
#define  INCL_WINSHELLDATA
#define  INCL_VIO
#define  INCL_DOS
#define  INCL_DOSFILEMGR
#define  INCL_DOSMEMMGR
#define  INCL_DOSMISC
#define  INCL_DOSNMPIPES
#define  INCL_ERRORS
#define  INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include <memory.h>
#include <malloc.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
 
 
/*********************************************************************/
/*  Declare all exported functions as REXX functions.                */
/*********************************************************************/
RexxFunctionHandler SysCls;
RexxFunctionHandler SysCurPos;
RexxFunctionHandler SysCurState;
RexxFunctionHandler SysDriveInfo;
RexxFunctionHandler SysDriveMap;
RexxFunctionHandler SysDropFuncs;
RexxFunctionHandler SysFileDelete;
RexxFunctionHandler SysFileSearch;
RexxFunctionHandler SysFileTree;
RexxFunctionHandler SysGetMessage;
RexxFunctionHandler SysIni;
RexxFunctionHandler SysLoadFuncs;
RexxFunctionHandler SysMkDir;
RexxFunctionHandler SysOS2Ver;
RexxFunctionHandler SysRmDir;
RexxFunctionHandler SysSearchPath;
RexxFunctionHandler SysSleep;
RexxFunctionHandler SysTempFileName;
RexxFunctionHandler SysTextScreenRead;
RexxFunctionHandler SysTextScreenSize;
RexxFunctionHandler SysGetEA;
RexxFunctionHandler SysPutEA;
RexxFunctionHandler SysWaitNamedPipe;
 
 
/*********************************************************************/
/*  Various definitions used by various functions.                   */
/*********************************************************************/
 
#define  MAX_DIGITS     9          /* maximum digits in numeric arg  */
#define  MAX            256        /* temporary buffer length        */
#define  IBUF_LEN       4096       /* Input buffer length            */
#define  AllocFlag      PAG_COMMIT | PAG_WRITE  /* for DosAllocMem   */
 
/*********************************************************************/
/*  Defines used by SysDriveMap                                      */
/*********************************************************************/
 
#define  USED           0
#define  FREE           1
#define  DETACHED       2
#define  REMOTE         3
#define  LOCAL          4
 
/*********************************************************************/
/* Defines uses by SysTree                                           */
/*********************************************************************/
 
#define  FIRST_TIME     0x0001
#define  RECURSE        0x0002
#define  DO_DIRS        0x0004
#define  DO_FILES       0x0008
#define  NAME_ONLY      0x0010
#define  EDITABLE_TIME  0x0020
#define  IGNORE         2              /* Ignore attributes entirely */
#define  AllAtts        FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_DIRECTORY | FILE_ARCHIVED
 
/*********************************************************************/
/* Structures used throughout REXXUTIL.C                              */
/*********************************************************************/
 
/*********************************************************************/
/* RxTree Structure used by SysTree.                                 */
/*********************************************************************/
 
typedef struct RxTreeData {
    ULONG count;                       /* Number of lines processed  */
 
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    ULONG open_action;                 /* Action taken opening the   */
                                       /* file                       */
    ULONG stemlen;                     /* Length of stem             */
    ULONG vlen;                        /* Length of variable value   */
    CHAR TargetSpec[MAX];              /* Filespec RxTree is looking */
                                       /* for                        */
    CHAR truefile[MAX];                /* Used to hold actual file   */
                                       /* name                       */
    CHAR tempfile[MAX];                /* Used to hold temp file name*/
    CHAR Temp[MAX];                    /* Used when building stem    */
                                       /* element                    */
    CHAR ibuf[IBUF_LEN];               /* Input buffer               */
    CHAR varname[MAX];                 /* Buffer for the variable    */
                                       /* name                       */
    ULONG j;                           /* Temp counter               */
    ULONG nattrib;                     /* New attrib, diff for each  */
                                       /* file                       */
} RXTREEDATA;
 
/*********************************************************************/
/* RxStemData                                                        */
/*   Structure which describes as generic                            */
/*   stem variable.                                                  */
/*********************************************************************/
 
typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR ibuf[IBUF_LEN];               /* Input buffer               */
    CHAR varname[MAX];                 /* Buffer for the variable    */
                                       /* name                       */
    CHAR stemname[MAX];                /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
    ULONG vlen;                        /* Length of variable value   */
    ULONG j;                           /* Temp counter               */
    ULONG tlong;                       /* Temp counter               */
    ULONG count;                       /* Number of elements         */
                                       /* processed                  */
} RXSTEMDATA;
 
/*********************************************************************/
/* RxFncTable                                                        */
/*   Array of names of the REXXUTIL functions.                       */
/*   This list is used for registration and deregistration.          */
/*********************************************************************/
 
static PSZ  RxFncTable[] =
   {
      "SysCls",
      "SysCurpos",
      "SysCurState",
      "SysDriveInfo",
      "SysDriveMap",
      "SysDropFuncs",
      "SysFileDelete",
      "SysFileSearch",
      "SysFileTree",
      "SysGetEA",
      "SysGetMessage",
      "SysIni",
      "SysLoadFuncs",
      "SysMkDir",
      "SysOS2Ver",
      "SysRmDir",
      "SysSearchPath",
      "SysSleep",
      "SysTempFileName",
      "SysTextScreenRead",
      "SysTextScreenSize",
      "SysGetEA",
      "SysPutEA",
      "SysWaitNamedPipe",
   };
 
/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/
 
#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  ERROR_NOMEM      "2"          /* Insufficient memory        */
#define  ERROR_FILEOPEN   "3"          /* Error opening text file    */
 
/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/
 
#define  ERROR_RETSTR   "ERROR:"
 
/*********************************************************************/
/* Numeric Return calls                                              */
/*********************************************************************/
 
#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */
 
/*********************************************************************/
/* Some useful macros                                                */
/*********************************************************************/
 
#define BUILDRXSTRING(t, s) { \
  strcpy((t)->strptr,(s));\
  (t)->strlength = strlen((s)); \
}
 
 
/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/
 
/********************************************************************
* Function:  SetFileMode(file, attributes)                          *
*                                                                   *
* Purpose:   Change file attribute bits                             *
*            without PM.                                            *
*                                                                   *
* RC:        0    -  File attributes successfully changed           *
*            1    -  Unable to change attributes                    *
*********************************************************************/
INT  SetFileMode(
  PSZ      file,                       /* file name                  */
  ULONG    attr )                      /* new file attributes        */
{
 
  FILESTATUS3   status;                /* status information         */
  INT           rc;                    /* function return code       */
 
                                       /* get the file status        */
  rc = DosQueryPathInfo(file, FIL_STANDARD, (PSZ)&status,
      sizeof(status));
  if (!rc) {                           /* worked?                    */
    status.attrFile = (USHORT)attr;    /* set new attributes         */
                                       /* set the file info          */
    rc = DosSetPathInfo(file, FIL_STANDARD, (PVOID)&status,
         sizeof(FILESTATUS3), 0);
  }
  return rc;                           /* give back success flag     */
}
 
/********************************************************************
* Function:  string2long(string, number)                            *
*                                                                   *
* Purpose:   Validates and converts an ASCII-Z string from string   *
*            form to an unsigned long.  Returns FALSE if the number *
*            is not valid, TRUE if the number was successfully      *
*            converted.                                             *
*                                                                   *
* RC:        TRUE - Good number converted                           *
*            FALSE - Invalid number supplied.                       *
*********************************************************************/
 
BOOL string2long(PSZ string, LONG *number)
{
  ULONG    accumulator;                /* converted number           */
  INT      length;                     /* length of number           */
  INT      sign;                       /* sign of number             */
 
  sign = 1;                            /* set default sign           */
  if (*string == '-') {                /* negative?                  */
    sign = -1;                         /* change sign                */
    string++;                          /* step past sign             */
  }
 
  length = strlen(string);             /* get length of string       */
  if (length == 0 ||                   /* if null string             */
      length > MAX_DIGITS)             /* or too long                */
    return FALSE;                      /* not valid                  */
 
  accumulator = 0;                     /* start with zero            */
 
  while (length) {                     /* while more digits          */
    if (!isdigit(*string))             /* not a digit?               */
      return FALSE;                    /* tell caller                */
                                       /* add to accumulator         */
    accumulator = accumulator *10 + (*string - '0');
    length--;                          /* reduce length              */
    string++;                          /* step pointer               */
  }
  *number = accumulator * sign;        /* return the value           */
  return TRUE;                         /* good number                */
}
 
/********************************************************************
* Function:  mystrstr(haystack, needle, hlen, nlen, sensitive)      *
*                                                                   *
* Purpose:   Determines if the string 'needle' is in the            *
*            string 'haystack' by returning it's position or        *
*            a NULL if not found.  The length of haystack and       *
*            needle are given by 'hlen' and 'nlen' respectively.    *
*                                                                   *
*            If 'sensitive' is true, then the search is case        *
*            sensitive, else it is case insensitive.                *
*                                                                   *
* RC:        num  - The pos where needle was found.                 *
*            NULL - needle not found.                               *
*                                                                   *
* Used By:   SysFileSearch()                                        *
*********************************************************************/
 
ULONG mystrstr(CHAR *haystack,CHAR *needle,ULONG hlen,ULONG nlen,
                BOOL sensitive)
 
{
  CHAR line[MAX];
  CHAR target[MAX];
  ULONG p;
 
 /* Copy line  - Change nulls to spaces and uppercase if needed      */
 
  for (p = 0; p < hlen; p++) {
 
    if (haystack[p] == '\0')
      line[p] = ' ';
    else if (sensitive)
      line[p] = haystack[p];
    else line[p] = (CHAR)toupper(haystack[p]);
  }
  line[p] = '\0';
 
 /* Copy target  - Change nulls to spaces and uppercase if needed    */
 
  for (p = 0; p < nlen; p++) {
 
    if (needle[p] == '\0')
      target[p] = ' ';
    else if (sensitive)
      target[p] = needle[p];
    else target[p] = (CHAR)toupper(needle[p]);
  }
  target[p] = '\0';
 
  return ((ULONG)strstr(line, target));
}
 
 
/*****************************************************************
* Function:  getpath(string, path, filename)                     *
*                                                                *
* Purpose:  This function gets the PATH and FILENAME of the file *
*           target contained in STRING.  The path will end with  *
*           the '\' char if a path is supplied.                  *
*                                                                *
* Used By:  RecursiveFindFile()                                  *
****************************************************************     */
 
VOID getpath(CHAR *string,CHAR *path,CHAR *filename)
{
  int len;
  int LastSlashPos;
 
  len = strlen(string);                /* Get length of full file    */
                                       /* spec                       */
  LastSlashPos = len;                  /* Get max pos of last '\'    */
 
    /* Step back through string until at begin or at '\' char        */
 
  while (string[LastSlashPos] != '\\' && LastSlashPos >= 0)
    --LastSlashPos;
 
    /* Copy filespec up to and including last '\' to path            */
  memcpy(path, string, LastSlashPos+1);
  path[LastSlashPos+1] = '\0';
 
    /* Get file name from filespec (just after last '\')             */
  strcpy(filename, &string[LastSlashPos+1]);
 
}
 
 
/*********************************************************************/
/* Function: ULONG SameAttr(mask, attr)                              */
/*                                                                   */
/* Purpose:  Returns the value TRUE if the attribute is identical to */
/*           that specified by the mask.  If not the same, then      */
/*           returns the value FALSE.                                */
/*                                                                   */
/* Used By:  RecursiveFindFile()                                     */
/*********************************************************************/
 
ULONG SameAttr(INT *mask,ULONG attr)
{
 
  if (mask[0] == IGNORE)
    return  TRUE;
 
  if (mask[0] < 0 && attr&FILE_ARCHIVED)
    return  FALSE;
 
  if (mask[0] > 0 && !(attr&FILE_ARCHIVED))
    return  FALSE;
 
  if (mask[1] < 0 && attr&FILE_DIRECTORY)
    return  FALSE;
 
  if (mask[1] > 0 && !(attr&FILE_DIRECTORY))
    return  FALSE;
 
  if (mask[2] < 0 && attr&FILE_HIDDEN)
    return  FALSE;
 
  if (mask[2] > 0 && !(attr&FILE_HIDDEN))
    return  FALSE;
 
  if (mask[3] < 0 && attr&FILE_READONLY)
    return  FALSE;
 
  if (mask[3] > 0 && !(attr&FILE_READONLY))
    return  FALSE;
 
  if (mask[4] < 0 && attr&FILE_SYSTEM)
    return  FALSE;
 
  if (mask[4] > 0 && !(attr&FILE_SYSTEM))
    return  FALSE;
  return  TRUE;
}
 
 
/*********************************************************************/
/* Function: ULONG NewAttr(mask, attr)                              */
/*                                                                   */
/* Purpose:  Returns the new file attribute, given the mask of       */
/*           attributes to be cleared/set and the current attribute  */
/*           settings.                                               */
/*                                                                   */
/* Used By:  RecursiveFindFile()                                     */
/*********************************************************************/
 
ULONG NewAttr(INT *mask,ULONG attr)
{
 
  if (mask[0] == IGNORE)
    return  attr;
 
  if (mask[0] < 0)
    attr &= ~FILE_ARCHIVED;            /* Clear                      */
 
  if (mask[0] > 0)
    attr |= FILE_ARCHIVED;             /* Set                        */
 
  if (mask[1] < 0)
    attr &= ~FILE_DIRECTORY;           /* Clear                      */
 
  if (mask[1] > 0)
    attr |= FILE_DIRECTORY;            /* Set                        */
 
  if (mask[2] < 0)
    attr &= ~FILE_HIDDEN;              /* Clear                      */
 
  if (mask[2] > 0)
    attr |= FILE_HIDDEN;               /* Set                        */
 
  if (mask[3] < 0)
    attr &= ~FILE_READONLY;            /* Clear                      */
 
  if (mask[3] > 0)
    attr |= FILE_READONLY;             /* Set                        */
 
  if (mask[4] < 0)
    attr &= ~FILE_SYSTEM;              /* Clear                      */
 
  if (mask[4] > 0)
    attr |= FILE_SYSTEM;               /* Set                        */
  return  attr;
}
 
 
/*****************************************************************************
* Function: RecursiveFindFile( FileSpec, lpd, smask, dmask, options )        *
*                                                                            *
* Purpose:  Finds all files starting with FileSpec, and will look down the   *
*           directory tree if required.                                      *
*                                                                            *
* Params:   FileSpec - ASCIIZ string which designates filespec to search     *
*                       for.                                                 *
*                                                                            *
*           ldp      - Pointer to local data structure.                      *
*                                                                            *
*           smask    - Array of integers which describe the source attribute *
*                       mask.  Only files with attributes matching this mask *
*                       will be found.                                       *
*                                                                            *
*           dmask    - Array of integers which describe the target attribute *
*                       mask.  Attributes of all found files will be set     *
*                       using this mask.                                     *
*                                                                            *
*             Note:  Both source and targets mask are really arrays of       *
*                    integers.  Each index of the mask corresponds           *
*                    to a different file attribute.  Each indexe and         *
*                    its associated attribute follows:                       *
*                                                                            *
*                         mask[0] = FILE_ARCHIVED                            *
*                         mask[1] = FILE_DIRECTORY                           *
*                         mask[2] = FILE_HIDDEN                              *
*                         mask[3] = FILE_READONLY                            *
*                         mask[4] = FILE_SYSTEM                              *
*                                                                            *
*                    A negative value at a given index indicates that        *
*                    the attribute bit of the file is not set.  A positive   *
*                    number indicates that the attribute should be set.      *
*                    A value of 0 indicates a "Don't Care" setting.          *
*                                                                            *
*           options  - The search/output options.  The following options     *
*                       may be ORed together when calling this function:     *
*                                                                            *
*                    FIRST_TIME  - Indicates this is initial call.  This     *
*                                   should always be used.                   *
*                    RECURSE     - Indicates that function should search     *
*                                   all child subdirectories recursively.    *
*                    DO_DIRS     - Indicates that directories should be      *
*                                   included in the search.                  *
*                    DO_FILES    - Indicates that files should be included   *
*                                   in the search.                           *
*                    NAME_ONLY   - Indicates that the output should be       *
*                                   restricted to filespecs only.            *
*                    EDITABLE_TIME - Indicates time and date fields should   *
*                                   be output as one timestamp.              *
*                                                                            *
* Used By:  SysFileTree()                                                    *
*****************************************************************************/
 
LONG RecursiveFindFile(FileSpec,ldp,smask,dmask,options)
  CHAR       *FileSpec;                /* Filespecs to search for    */
  RXTREEDATA *ldp;                     /* Pointer to local data      */
  INT        *smask;                   /* Mask of attributes to      */
                                       /* search for                 */
  INT        *dmask;                   /* Mask of attributes to set  */
  ULONG       options;                 /* Search and output format   */
                                       /* options                    */
{
  FILEFINDBUF3 finfo;                  /* OS2 File Find Utility      */
                                       /* Structure                  */
  ULONG SearchCount = 1;               /* Num things searching for   */
                                       /* per DosFindFirst           */
  HDIR    DirHandle = 0xFFFF;          /* Directory handle used by   */
                                       /* DosFindFirst()             */
  CHAR    path[MAX];                   /* Path to target file(s)     */
  CHAR    filename[MAX];               /* Current file name          */
  BOOL    IsaDotdir = FALSE;           /* Boolean for determining if */
                                       /* file is ./..               */
  BOOL    IsaSubdir = FALSE;           /* Boolean for determining if */
                                       /* file is a dir              */
 
  /********************************************************************
  * Search Strategy Explained:                                        *
  *                                                                   *
  * Get path and filespec to be searched.                             *
  *                                                                   *
  * If this is the initial call (see FIRST_TIME) and                  *
  * searching recursively, make sure to look for the                  *
  * original filespec in the initial directory.                       *
  *                                                                   *
  * From then on, look for a filespec of *.* to expand                *
  * the subdirectories.  Once a subdirectory is expanded,             *
  * search for the initial filespec again.                            *
  ********************************************************************/
 
  getpath(FileSpec, path, filename);
 
  /********************************************************************
  * Initial Recursive case:                                           *
  *                                                                   *
  * This section of code recurses to call itself in the               *
  * non-recursive form.  It will allow the function to                *
  * gather all the files in the initial subdirectory.                 *
  *                                                                   *
  * It also saves the initial filespec to be searched                 *
  * for in all directories which will be expanded (see                *
  * ldp->TargetSpec).  From then on, the recursive case               *
  * will search for directories by examining all files                *
  * matching the spec *.*.                                            *
  ********************************************************************/
 
                                       /* Onye executed first time   */
  if ((options&FIRST_TIME) && (options&RECURSE)) {
    if (RecursiveFindFile(FileSpec, ldp, smask, dmask,
        options&~RECURSE&~FIRST_TIME))
      return INVALID_ROUTINE;
    strcpy(ldp->TargetSpec, filename);
    strcpy(filename, "*.*");
    sprintf(FileSpec, "%s*.*", path);
    options &= ~FIRST_TIME;
  }
 
  /********************************************************************
  * Non-Recursive case:                                               *
  *                                                                   *
  * This section of code finds all files matching the                 *
  * filespec (filtering out . and ..) and stores them                 *
  * in stem elements if they meet the appropriate file                *
  * attribute requirements.  Each found file's attributes             *
  * are set as required if needed.                                    *
  ********************************************************************/
 
  if (!(options&RECURSE)) {
 
    if (!DosFindFirst((PSZ)FileSpec, (PHDIR)&DirHandle, (ULONG)
        AllAtts, (PVOID)&finfo, (ULONG)sizeof(finfo), (PULONG)
        &SearchCount, FIL_STANDARD)) {
 
      IsaSubdir = (BOOL)(finfo.attrFile&FILE_DIRECTORY);
      IsaDotdir = (BOOL)(!strcmp(finfo.achName, ".") ||
          !strcmp(finfo.achName, ".."));
 
      if (!IsaDotdir && SameAttr(smask, finfo.attrFile) &&
          ((IsaSubdir && (options&DO_DIRS)) ||
          (!IsaSubdir && (options&DO_FILES)))) {
        sprintf(ldp->tempfile, "%s%s", path, finfo.achName);
                                       /* Get full path              */
        DosQueryPathInfo(ldp->tempfile, 5, ldp->truefile,
            sizeof(ldp->truefile));
 
        if (options&NAME_ONLY)
          strcpy(ldp->Temp, ldp->truefile);
        else {
          if (options&EDITABLE_TIME)
            sprintf(ldp->Temp, "%02d/%02d/%02d/%02d/%02d  %10lu  ",
                (finfo.fdateLastWrite.year+80)%100,
                finfo.fdateLastWrite.month,
                finfo.fdateLastWrite.day,
                finfo.ftimeLastWrite.hours,
                finfo.ftimeLastWrite.minutes,
                finfo.cbFile);
          else
            sprintf(ldp->Temp, "%2d/%02d/%02d  %2d:%02d%c  %10lu  ",
                finfo.fdateLastWrite.month,
                finfo.fdateLastWrite.day,
                (finfo.fdateLastWrite.year+80)%100,
                (finfo.ftimeLastWrite.hours < 13?
                finfo.ftimeLastWrite.hours:
                (finfo.ftimeLastWrite.hours-(SHORT)12)),
                finfo.ftimeLastWrite.minutes,
                ((finfo.ftimeLastWrite.hours < 12 ||
                finfo.ftimeLastWrite.hours == 24)?'a':'p'),
                finfo.cbFile);
          ldp->nattrib = NewAttr((INT *)dmask, finfo.attrFile);
 
          if (ldp->nattrib != finfo.attrFile) {
            sprintf(ldp->tempfile, "%s%s", path, finfo.achName);
 
            if (SetFileMode(ldp->tempfile, (USHORT)(ldp->nattrib&~
                FILE_DIRECTORY)))      /* Set the new file mode      */
              ldp->nattrib = finfo.attrFile;/* Just in case of       */
                                       /* failure                    */
          }
          sprintf(ldp->Temp, "%s%c%c%c%c%c  %s", ldp->Temp,
              ((ldp->nattrib&FILE_ARCHIVED)?'A':'-'),
              ((ldp->nattrib&FILE_DIRECTORY)?'D':'-'),
              ((ldp->nattrib&FILE_HIDDEN)?'H':'-'),
              ((ldp->nattrib&FILE_READONLY)?'R':'-'),
              ((ldp->nattrib&FILE_SYSTEM)?'S':'-'),
              ldp->truefile);
        }
                                       /* Place new string in Stem   */
        memcpy(ldp->ibuf, ldp->Temp, strlen(ldp->Temp));
        ldp->vlen = strlen(ldp->Temp);
        ldp->count++;
        sprintf(ldp->varname+ldp->stemlen,"%d", ldp->count);
        ldp->shvb.shvnext = NULL;
        ldp->shvb.shvname.strptr = ldp->varname;
        ldp->shvb.shvname.strlength = strlen(ldp->varname);
        ldp->shvb.shvvalue.strptr = ldp->ibuf;
        ldp->shvb.shvvalue.strlength = ldp->vlen;
        ldp->shvb.shvnamelen = ldp->shvb.shvname.strlength;
        ldp->shvb.shvvaluelen = ldp->vlen;
        ldp->shvb.shvcode = RXSHV_SET;
        ldp->shvb.shvret = 0;
        if (RexxVariablePool(&ldp->shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;      /* error on non-zero          */
      }
 
        /* Get the rest of the files ................................*/
 
      while (!DosFindNext((HDIR)DirHandle, (PVOID)&finfo,
          (ULONG)sizeof(finfo), (PULONG)&SearchCount)) {
 
        IsaSubdir = (BOOL)(finfo.attrFile&FILE_DIRECTORY);
        IsaDotdir = (BOOL)(!strcmp(finfo.achName, ".") ||
            !strcmp(finfo.achName, ".."));
 
        if (!IsaDotdir && SameAttr(smask, finfo.attrFile) &&
            ((IsaSubdir && (options&DO_DIRS)) ||
            (!IsaSubdir && (options&DO_FILES)))) {
          sprintf(ldp->tempfile, "%s%s", path, finfo.achName);
                                       /* Get full path              */
          DosQueryPathInfo(ldp->tempfile, 5, ldp->truefile,
              sizeof(ldp->truefile));
 
          if (options&NAME_ONLY)
            strcpy(ldp->Temp, ldp->truefile);
 
          else {
            if (options&EDITABLE_TIME)
              sprintf(ldp->Temp, "%02d/%02d/%02d/%02d/%02d  %10lu  ",
                  (finfo.fdateLastWrite.year+80)%100,
                  finfo.fdateLastWrite.month,
                  finfo.fdateLastWrite.day,
                  finfo.ftimeLastWrite.hours,
                  finfo.ftimeLastWrite.minutes, finfo.cbFile);
            else
              sprintf(ldp->Temp,
                  "%2d/%02d/%02d  %2d:%02d%c  %10lu  ",
                  finfo.fdateLastWrite.month,
                  finfo.fdateLastWrite.day,
                  (finfo.fdateLastWrite.year+80)%100,
                  (finfo.ftimeLastWrite.hours < 13?
                  finfo.ftimeLastWrite.hours:
                  (finfo.ftimeLastWrite.hours - (SHORT)12)),
                  finfo.ftimeLastWrite.minutes,
                  ((finfo.ftimeLastWrite.hours < 12 ||
                  finfo.ftimeLastWrite.hours == 24)?'a':'p'),
                  finfo.cbFile);
            ldp->nattrib = NewAttr((INT *)dmask, finfo.attrFile);
 
            if (ldp->nattrib != finfo.attrFile) {
              sprintf(ldp->tempfile, "%s%s", path, finfo.achName);
 
              if (SetFileMode(ldp->tempfile, (USHORT)(ldp->nattrib&~
                  FILE_DIRECTORY)))    /* Set the new file mode      */
                ldp->nattrib = finfo.attrFile;/* Just in case of     */
                                       /* failure                    */
            }
            sprintf(ldp->Temp, "%s%c%c%c%c%c  %s", ldp->Temp,
                ((ldp->nattrib&FILE_ARCHIVED)?'A':'-'),
                ((ldp->nattrib&FILE_DIRECTORY)?'D':'-'),
                ((ldp->nattrib&FILE_HIDDEN)?'H':'-'),
                ((ldp->nattrib&FILE_READONLY)?'R':'-'),
                ((ldp->nattrib&FILE_SYSTEM)?'S':'-'),
                ldp->truefile);
          }
                                       /* Place new string in Stem   */
          memcpy(ldp->ibuf, ldp->Temp, strlen(ldp->Temp));
          ldp->vlen = strlen(ldp->Temp);
          ldp->count++;
          sprintf(ldp->varname+ldp->stemlen, "%d", ldp->count);
          ldp->shvb.shvnext = NULL;
          ldp->shvb.shvname.strptr = ldp->varname;
          ldp->shvb.shvname.strlength = strlen(ldp->varname);
          ldp->shvb.shvvalue.strptr = ldp->ibuf;
          ldp->shvb.shvvalue.strlength = ldp->vlen;
          ldp->shvb.shvnamelen = ldp->shvb.shvname.strlength;
          ldp->shvb.shvvaluelen = ldp->vlen;
          ldp->shvb.shvcode = RXSHV_SET;
          ldp->shvb.shvret = 0;
          if (RexxVariablePool(&ldp->shvb) == RXSHV_BADN)
            return INVALID_ROUTINE;    /* error on non-zero          */
 
        }
      }
    }
    DosFindClose(DirHandle);
  }
 
  /********************************************************
  * Recursive case:                                       *
  *                                                       *
  * This section of code searches for directories given   *
  * the current FileSpec.  When one is found, the         *
  * function is called in the non-recursive mode to find  *
  * all files matching the initial filespec.  It is also  *
  * called in the recursive mode to expand all subdirect- *
  * ories under the new found directory.                  *
  *********************************************************/
 
  else {
 
    if (!DosFindFirst((PSZ)FileSpec, (PHDIR)&DirHandle,
        (ULONG)AllAtts, (PVOID)&finfo, (ULONG)sizeof(finfo),
        (PULONG)&SearchCount, FIL_STANDARD)) {
 
      IsaSubdir = (BOOL)(finfo.attrFile&FILE_DIRECTORY);
      IsaDotdir = (BOOL)(!strcmp(finfo.achName, ".") ||
          !strcmp(finfo.achName, ".."));
 
      if (!IsaDotdir && IsaSubdir) {
        sprintf(ldp->tempfile, "%s%s\\%s", path, finfo.achName,
            ldp->TargetSpec);
        if (RecursiveFindFile(ldp->tempfile, ldp, smask, dmask,
            options&~RECURSE))
          return INVALID_ROUTINE;
        sprintf(ldp->tempfile, "%s%s\\%s", path, finfo.achName,
            filename);
        if (RecursiveFindFile(ldp->tempfile, ldp, smask, dmask, options))
          return INVALID_ROUTINE;
      }
 
        /* Get the rest of the files ................................*/
 
      while (!DosFindNext((HDIR)DirHandle, (PVOID)&finfo,
          (ULONG)sizeof(finfo), (PULONG)&SearchCount)) {
 
        IsaSubdir = (BOOL)(finfo.attrFile&FILE_DIRECTORY);
        IsaDotdir = (BOOL)(!strcmp(finfo.achName, ".") ||
            !strcmp(finfo.achName, ".."));
 
        if (!IsaDotdir && IsaSubdir) {
          sprintf(ldp->tempfile, "%s%s\\%s", path, finfo.achName,
              ldp->TargetSpec);
          if (RecursiveFindFile(ldp->tempfile, ldp, smask, dmask,
              options&~RECURSE))
            return INVALID_ROUTINE;
          sprintf(ldp->tempfile, "%s%s\\%s", path, finfo.achName,
              filename);
          if (RecursiveFindFile(ldp->tempfile, ldp, smask, dmask,
              options))
            return INVALID_ROUTINE;
        }
      }
    }
    DosFindClose(DirHandle);
  }
  return VALID_ROUTINE;
}
 
 
/****************************************************************
* Function: GetUniqueFileName(Template, Filler, file)           *
*                                                               *
* Purpose:  This function returns a unique temporary file name  *
*           given a template and a filler character.            *
*                                                               *
* Params:   CHAR* Template - The template.  Must contain at     *
*                            least one or more filler chars.    *
*                                                               *
*                            Example:  "C:\TEMP\FILE????.DAT    *
*                                                               *
*           CHAR Filler    - The character in the Template to   *
*                            be replaced with numbers.  For     *
*                            the above example, the filler char *
*                            would be '?'.                      *
*           CHAR* file     - file name produced (output)        *
*                                                               *
* Used By:  RxTempFileName()                                    *
****************************************************************/
 
VOID GetUniqueFileName(CHAR *Template,CHAR Filler,CHAR *file)
{
  CHAR numstr[6];
  BOOL Unique = FALSE;
 
  ULONG rc,                            /* return code for OS/2 call  */
        x,                             /* loop index                 */
        i,                             /*                            */
        j = 0,                         /* number of filler chars     */
                                       /* found                      */
        num,                           /* temporary random number    */
        start,                         /* first random number        */
        max = 1;                       /* maximum random number      */
 
  ULONG SearchCount;                   /* count of files to search   */
                                       /* for.                       */
  HDIR DirHandle;                      /* handle for found file      */
  INT  seed;                           /* to get current time        */
  FILEFINDBUF3 finfo;                  /* OS2 File Find Utility      */
                                       /* Structure                  */
  DATETIME DT;                         /* The date and time structure*/
 
 /** Determine number of filler characters *                         */
 
  for (x = 0; Template[x] != 0; x++)
 
    if (Template[x] == Filler) {
      max = max *10;
      j++;
    }
 
 /** Return NULL string if less than 1 or greater than 4 *           */
 
  if (j == 0 || j > 5) {
    Unique = TRUE;
    strcpy(file, "");
  }
 
 /** Get a random number in the appropriate range                    */
 
  DosGetDateTime((PDATETIME)&DT);      /* Get the time               */
  seed = DT.hours*60 + DT.minutes;     /* convert to hundreths       */
  seed = seed*60 + DT.seconds;
  seed = seed*100 + DT.hundredths;
  srand((INT)seed);                    /* Set random number seed     */
  num = (ULONG)rand()%max;
  start = num;
 
 /** Do until a unique name is found                                 */
 
  while (!Unique) {
 
    /** Generate string which represents the number                  */
 
    switch (j) {
      case 1 :
        sprintf(numstr, "%01u", num);
        break;
      case 2 :
        sprintf(numstr, "%02u", num);
        break;
      case 3 :
        sprintf(numstr, "%03u", num);
        break;
      case 4 :
        sprintf(numstr, "%04u", num);
        break;
      case 5 :
        sprintf(numstr, "%05u", num);
        break;
    }
 
    /** Subsitute filler characters with numeric string              */
 
    i = 0;
 
    for (x = 0; Template[x] != 0; x++)
 
      if (Template[x] == Filler)
        file[x] = numstr[i++];
 
      else
        file[x] = Template[x];
    file[x] = '\0';
 
    /** See if the file exists                                       */
 
    DosError(0);                       /* Disable Hard-Error popups  */
    DirHandle = 0xFFFF;
    SearchCount = 1;
    rc = DosFindFirst(file, &DirHandle, AllAtts, (PVOID)&finfo,
        (ULONG)sizeof(finfo), (PULONG)&SearchCount, FIL_STANDARD);
 
    if (rc == 2 || rc == 18)
      Unique = TRUE;
    DosFindClose(DirHandle);
    DosError(1);                       /* Enable Hard-Error popups   */
 
    /** Make sure we are not wasting our time                        */
 
    num = (num+1)%max;
 
    if (num == start && !Unique) {
      Unique = TRUE;
      strcpy(file, "");
    }
  }
}
 
 
/*************************************************************************
***             <<<<<< REXXUTIL Functions Follow >>>>>>>               ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>               ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>               ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>               ***
*************************************************************************/
/*************************************************************************
* Function:  SysCls                                                      *
*                                                                        *
* Syntax:    call SysCls                                                 *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/
 
ULONG SysCls(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, RXSTRING *retstr)
{
  BYTE bCell[2];                       /* Char/Attribute array       */
 
  if (numargs)                         /* arguments specified?       */
    return INVALID_ROUTINE;            /* raise the error            */
 
  bCell[0] = 0x20;                     /* Space Character            */
  bCell[1] = 0x07;                     /* Default Attrib             */
  VioScrollDn( 0, 0, (USHORT)0xFFFF, (USHORT)0XFFFF,
                     (USHORT)0xFFFF, bCell, (HVIO) 0);/* CLS         */
  VioSetCurPos(0, 0, (HVIO) 0);        /*                Pos cursor  */
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);/* pass back result           */
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysCurPos - positions cursor in OS/2 session                *
*                                                                        *
* Syntax:    call SysCurPos [row, col]                                   *
*                                                                        *
* Params:    row   - row to place cursor on                              *
*            col   - column to place cursor on                           *
*                                                                        *
* Return:    row, col                                                    *
*************************************************************************/
 
ULONG SysCurPos(PUCHAR name, ULONG numargs, RXSTRING args[],
    PSZ queuename, RXSTRING *retstr)
{
  USHORT start_row;                    /* Row at start               */
  USHORT start_col;                    /* Col at start               */
  LONG   new_row;                      /* Row to change to           */
  LONG   new_col;                      /* Col to change to           */
 
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);/* set default result         */
                                       /* check arguments            */
  if ((numargs != 0 && numargs != 2))  /* wrong number?              */
    return INVALID_ROUTINE;            /* raise an error             */
 
  VioGetCurPos(&start_row, &start_col, (HVIO)0); /* Get position     */
  sprintf(retstr->strptr, "%d %d", (int)start_row, (int)start_col);
  retstr->strlength = strlen(retstr->strptr);
 
  if (numargs != 0) {                  /* reset position to given    */
    if (!RXVALIDSTRING(args[0]) ||     /* not real arguments given?  */
        !RXVALIDSTRING(args[1]))
      return INVALID_ROUTINE;          /* raise an error             */
                                       /* convert row to binary      */
    if (!string2long(args[0].strptr, &new_row) || new_row < 0)
      return INVALID_ROUTINE;          /* return error               */
                                       /* convert row to binary      */
    if (!string2long(args[1].strptr, &new_col) || new_col < 0)
      return INVALID_ROUTINE;          /* return error               */
 
    /* Set the cursor position, using the input values.               */
    VioSetCurPos((USHORT) new_row, (USHORT) new_col, (HVIO) 0);
  }
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysCurState                                                 *
*                                                                        *
* Syntax:    call SysCurState state                                      *
*                                                                        *
* Params:    state - Either 'ON' or 'OFF'.                               *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/
 
ULONG SysCurState(PUCHAR name, ULONG numargs, RXSTRING args[],
    PSZ queuename, RXSTRING *retstr)
{
  USHORT state;                        /* State                      */
  VIOCURSORINFO vioci;                 /* Cursor info struct         */
 
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);/* pass back result           */
                                       /* validate the arguments     */
  if (numargs != 1)
    return INVALID_ROUTINE;
                                       /* Get state and validate     */
  if (stricmp(args[0].strptr, "ON") == 0)
    state = (USHORT) 0;
  else if (stricmp(args[0].strptr, "OFF") == 0)
    state = (USHORT) -1;
  else
    return INVALID_ROUTINE;            /* Invalid state              */
 
  VioGetCurType(&vioci, (HVIO) 0);     /* Get current state          */
  vioci.attr= state;                   /* Set state info             */
  VioSetCurType(&vioci, (HVIO) 0);     /* Set new state              */
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysDriveInfo                                                *
*                                                                        *
* Syntax:    call SysDriveInfo drive                                     *
*                                                                        *
* Params:    drive - 'C', 'D', 'E', etc.                                 *
*                                                                        *
* Return:    disk free total label                                       *
*************************************************************************/
 
ULONG SysDriveInfo(PUCHAR name, ULONG numargs, RXSTRING args[],
                           PSZ queuename, RXSTRING *retstr)
{
  PSZ       arg;                       /* Temp var for holding args  */
  UCHAR     buf[256];                  /* Give DosQFSInfo 256 bytes  */
 
  ULONG     drivenum;                  /* Drive number (A=1, B=2,    */
                                       /* etc)                       */
  FSALLOCATE FSInfoBuf;                /* Structure required by      */
                                       /* DosQfsInfo                 */
  LONG      rc;                        /* OS/2 return code           */
 
                                       /* validate arguments         */
  if (numargs != 1 ||
      args[0].strlength > 2 ||
      args[0].strlength == 0)
    return INVALID_ROUTINE;
                                       /* validate the arg           */
  arg = args[0].strptr;                /* get argument pointer       */
                                       /* drive letter?              */
  if ((strlen(arg) == 2 &&
      arg[1] != ':') ||
      !isalpha(arg[0]))
    return INVALID_ROUTINE;
  else                                 /* yep, convert to a number   */
    drivenum = toupper(arg[0])-'A'+1;  /* Get the drive number       */
 
                                       /* query the disk             */
  DosError(0);                         /* Disable hard-error pop-up  */
                                       /* Get the drive info         */
  rc = DosQueryFSInfo(drivenum, 2, buf, sizeof(buf));
  DosQueryFSInfo(drivenum, 1, (char *)&FSInfoBuf, sizeof(FSInfoBuf));
 
  if (rc == 0 || rc == 125) {
    sprintf(retstr->strptr,
            "%c:  %-12lu %-12lu %-13s",
            (drivenum+'A'-1),
            FSInfoBuf.cSectorUnit * FSInfoBuf.cbSector *
            FSInfoBuf.cUnitAvail,
            FSInfoBuf.cSectorUnit * FSInfoBuf.cbSector *
            FSInfoBuf.cUnit, &buf[5]);
                                       /* create return string       */
    retstr->strlength = strlen(retstr->strptr);
  }
  else
    retstr->strlength = 0;             /* return null string         */
 
  DosError(1);                         /* Enable hard-error pop-up   */
  return VALID_ROUTINE;                /* no error on call           */
 
}
 
/*************************************************************************
* Function:  SysDriveMap                                                 *
*                                                                        *
* Syntax:    call SysDriveMap [drive] [,mode]                            *
*                                                                        *
* Params:    drive - 'C', 'D', 'E', etc.  The drive to start the search  *
*                     with.                                              *
*            mode  - Any of the following:  'FREE', 'USED', 'DETACHED',  *
*                                           'LOCAL', 'REMOTE'            *
*                                                                        *
* Return:    'A: B: C: D: ...'                                           *
*************************************************************************/
 
ULONG SysDriveMap(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, RXSTRING *retstr)
{
  CHAR     temp[MAX];                  /* Entire drive map built here*/
 
  CHAR     tmpstr[MAX];                /* Single drive entries built */
                                       /* here                       */
  CHAR     buf[256];                   /* Give DosQFSInfo 256 bytes  */
  CHAR     DeviceName[3];              /* Device name or drive letter*/
                                       /* string                     */
  ULONG    CurDrive;                   /* Current drive              */
  ULONG    DriveMap;                   /* Drive map                  */
  ULONG    Ordinal;                    /* Ordinal of entry in name   */
                                       /* list                       */
  ULONG    FSAInfoLevel;               /* Type of attached FSD data  */
                                       /* required                   */
  ULONG    DataBufferLen;              /* Buffer length              */
  ULONG    dnum;                       /* Disk num variable          */
  ULONG    start = 3;                  /* Initial disk num           */
  ULONG    Mode = USED;                /* Query mode USED, FREE,     */
                                       /* LOCAL, etc                 */
  FSQBUFFER2 DataBuffer;               /* Returned data buffer       */
  LONG     rc;                         /* OS/2 return codes          */
 
  Ordinal = (ULONG )0;
  FSAInfoLevel = (ULONG )1;
 
  temp[0] = '\0';
 
  if (numargs > 2)                     /* validate arguments         */
    return INVALID_ROUTINE;
                                       /* check starting drive letter*/
  if (numargs >= 1 && args[0].strptr) {
 
    if ((strlen(args[0].strptr) == 2 &&
        args[0].strptr[1] != ':') ||
        strlen(args[0].strptr) > 2 ||
        strlen(args[0].strptr) == 0 ||
        !isalpha(args[0].strptr[0]))
      return INVALID_ROUTINE;
    start = toupper(args[0].strptr[0])-'A'+1;
  }
                                       /* check the mode             */
  if (numargs == 2 && args[1].strlength != 0) {
 
    if (!stricmp(args[1].strptr, "FREE"))
      Mode = FREE;
    else if (!stricmp(args[1].strptr, "USED"))
      Mode = USED;
    else if (!stricmp(args[1].strptr, "DETACHED"))
      Mode = DETACHED;
    else if (!stricmp(args[1].strptr, "REMOTE"))
      Mode = REMOTE;
    else if (!stricmp(args[1].strptr, "LOCAL"))
      Mode = LOCAL;
    else
      return INVALID_ROUTINE;
  }
                                       /* perform the query          */
  DosError(0);                         /* Disable Hard-Error Popups  */
  DosQueryCurrentDisk(&CurDrive, &DriveMap);
  DriveMap>>=start-1;                  /* Shift to the first drive   */
  temp[0] = '\0';                      /* Clear temporary buffer     */
 
  for (dnum = start; dnum <= 26; dnum++) {
 
                                       /* Hey, we have a free drive  */
    if (!(DriveMap&(ULONG)1) && Mode == FREE) {
      sprintf(tmpstr, "%c: ", dnum+'A'-1);
      strcat(temp, tmpstr);
    }
                                       /* Hey, we have a used drive  */
    else if ((DriveMap&(ULONG)1) && Mode == USED) {
      sprintf(tmpstr, "%c: ", dnum+'A'-1);
      strcat(temp, tmpstr);
    }
 
    else if (DriveMap&(ULONG)1) {      /* Check specific drive info  */
      sprintf(DeviceName, "%c:", dnum+'A'-1);
      DataBufferLen = sizeof DataBuffer;
      DosQueryFSAttach(DeviceName, Ordinal, FSAInfoLevel,
          &DataBuffer, &DataBufferLen);
      rc = DosQueryFSInfo(dnum, 2, buf, sizeof(buf));
 
      if (rc == 67 && DataBuffer.iType == 4 && Mode == DETACHED) {
                                       /* Hey, we have a detached    */
                                       /* drive                      */
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }
 
      else if (DataBuffer.iType == 4 && Mode == REMOTE) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }
 
      else if (DataBuffer.iType == 3 && Mode == LOCAL) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }
    }
    DriveMap>>=1;                      /* Shift to the next drive    */
  }
 
  BUILDRXSTRING(retstr, temp);         /* pass back result           */
  if (retstr->strlength)               /* if not a null string       */
    retstr->strlength--;               /* Get rid of last space      */
  DosError(1);                         /* Enable Hard-Error Popups   */
  return VALID_ROUTINE;                /* no error on call           */
}
 
 
/*************************************************************************
* Function:  SysDropFuncs                                                *
*                                                                        *
* Syntax:    call SysDropFuncs                                           *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/
 
ULONG SysDropFuncs(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, RXSTRING *retstr)
{
  INT     entries;                     /* Num of entries             */
  INT     j;                           /* Counter                    */
 
  if (numargs != 0)                    /* no arguments for this      */
    return INVALID_ROUTINE;            /* raise an error             */
 
  retstr->strlength = 0;               /* return a null string result*/
 
  entries = sizeof(RxFncTable)/sizeof(PSZ);
 
  for (j = 0; j < entries; j++)
    RexxDeregisterFunction(RxFncTable[j]);
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysFileDelete                                               *
*                                                                        *
* Syntax:    call SysFileDelete file                                     *
*                                                                        *
* Params:    file - file to be deleted.                                  *
*                                                                        *
* Return:    Return code from DosDelete() function.                      *
*************************************************************************/
 
ULONG SysFileDelete(PUCHAR name, ULONG numargs, RXSTRING args[],
                            PSZ queuename, RXSTRING *retstr)
{
  ULONG  rc;                           /* Ret code                   */
 
  if (numargs != 1)                    /* we need one argument       */
    return INVALID_ROUTINE;            /* raise an error             */
 
  rc = DosDelete(args[0].strptr);      /* delete the file            */
                                       /* pass back return code      */
  sprintf(retstr->strptr, "%d", rc);
  retstr->strlength = strlen(retstr->strptr);
 
  return VALID_ROUTINE;                /* finished                   */
}
 
/*************************************************************************
* Function:  SysFileSearch                                               *
*                                                                        *
* Syntax:    call SysFileSearch target, file, stem [, options]           *
*                                                                        *
* Params:    target  - String to search for.                             *
*            file    - Filespec to search.                               *
*            stem    - Stem variable name to place results in.           *
*            options - Any combo of the following:                       *
*                       'C' - Case sensitive search (non-default).       *
*                       'N' - Preceed each found string in result stem   *
*                              with it line number in file (non-default).*
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/
 
ULONG SysFileSearch(PUCHAR name, ULONG numargs, RXSTRING args[],
                            PSZ queuename, RXSTRING *retstr)
{
  PSZ         target;                  /* search string              */
  PSZ         file;                    /* search file                */
  PSZ         opts;                    /* option string              */
  CHAR        line[MAX];               /* Line read from file        */
  ULONG       ptr;                     /* Pointer to char str found  */
  ULONG       num = 0;                 /* Line number                */
  ULONG       len;                     /* Length of string           */
  ULONG       len2;                    /* Length of string           */
  ULONG       rc = 0;                  /* Return code of this func   */
  BOOL        linenums = FALSE;        /* Set TRUE for linenums in   */
                                       /* output                     */
  BOOL        sensitive = FALSE;       /* Set TRUE for case-sens     */
                                       /* search                     */
  FILE       *fp;                      /* Pointer to file to search  */
  RXSTEMDATA  ldp;                     /* stem data                  */
 
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);/* pass back result           */
                                       /* validate arguments         */
  if (numargs < 3 || numargs > 4 ||
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]) ||
      !RXVALIDSTRING(args[2]))
    return INVALID_ROUTINE;            /* raise an error             */
 
  target = args[0].strptr;             /* get target pointer         */
  file = args[1].strptr;               /* get file name              */
 
  if (numargs == 4) {                  /* process options            */
    opts = args[3].strptr;             /* point to the options       */
    if (strstr(opts, "N") || strstr(opts, "n"))
      linenums = TRUE;
 
    if (strstr(opts, "C") || strstr(opts, "c"))
      sensitive = TRUE;
  }
 
                                       /* Initialize data area       */
  ldp.count = 0;
  strcpy(ldp.varname, args[2].strptr);
  ldp.stemlen = args[2].strlength;
  strupr(ldp.varname);                 /* uppercase the name         */
 
  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';
 
  fp = fopen(file, "r");               /* Open the file              */
 
  if (fp == NULL) {                    /* Open error?                */
    BUILDRXSTRING(retstr, ERROR_FILEOPEN);
    return VALID_ROUTINE;              /* finished                   */
  }
                                       /* do the search...found lines*/
                                       /* are saved in stem vars     */
  while (fgets(line, MAX - 1, fp) != NULL) {
    len = strlen(line);
    num++;
    ptr = mystrstr(line, target, len, args[0].strlength, sensitive);
 
    if (ptr != '\0') {
 
      if (linenums) {
        sprintf(ldp.ibuf, "%d ", num);
        len2 = strlen(ldp.ibuf);
        memcpy(ldp.ibuf+len2, line, len);
        ldp.vlen = len+len2;
      }
      else {
        memcpy(ldp.ibuf, line, len);
        ldp.vlen = len;
      }
      ldp.count++;
      sprintf(ldp.varname+ldp.stemlen, "%d", ldp.count);
 
      if (ldp.ibuf[ldp.vlen-1] == '\n')
        ldp.vlen--;
      ldp.shvb.shvnext = NULL;
      ldp.shvb.shvname.strptr = ldp.varname;
      ldp.shvb.shvname.strlength = strlen(ldp.varname);
      ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
      ldp.shvb.shvvalue.strptr = ldp.ibuf;
      ldp.shvb.shvvalue.strlength = ldp.vlen;
      ldp.shvb.shvvaluelen = ldp.vlen;
      ldp.shvb.shvcode = RXSHV_SET;
      ldp.shvb.shvret = 0;
      if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
        return INVALID_ROUTINE;      /* error on non-zero          */
    }
  }
 
  fclose(fp);                        /* Close that file            */
                                     /* set stem.0 to lines read   */
  sprintf(ldp.ibuf, "%d", ldp.count);
  ldp.varname[ldp.stemlen] = '0';
  ldp.varname[ldp.stemlen+1] = 0;
  ldp.shvb.shvnext = NULL;
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = ldp.stemlen+1;
  ldp.shvb.shvnamelen = ldp.stemlen+1;
  ldp.shvb.shvvalue.strptr = ldp.ibuf;
  ldp.shvb.shvvalue.strlength = strlen(ldp.ibuf);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
    return INVALID_ROUTINE;            /* error on non-zero          */
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysFileTree                                                 *
*                                                                        *
* Syntax:    call SysFileTree filespec, stem [, options]                 *
*                                                                        *
* Params:    filespec - Filespec to search for (may include * and ?).    *
*            stem     - Name of stem var to store results in.            *
*            options  - Any combo of the following:                      *
*                        'B' - Search for files and directories.         *
*                        'D' - Search for directories only.              *
*                        'F' - Search for files only.                    *
*                        'O' - Only output file names.                   *
*                        'S' - Recursively scan subdirectories.          *
*                        'T' - Combine time & date fields into one.      *
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/
 
ULONG SysFileTree(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, RXSTRING *retstr)
{
  CHAR        FileSpec[MAX];           /* File spec to look for      */
 
  CHAR        Opts[64];                /* Search options             */
  ULONG       options;                 /* Mask of options            */
  ULONG       x;                       /* Temp counter               */
  ULONG       y;                       /* Temp counter (II)          */
  INT         smask[5];                /* Source attribute mask      */
  INT         dmask[5];                /* Target attribute mask      */
  RXTREEDATA  ldp;                     /* local data                 */
 
  options = FIRST_TIME|DO_FILES|DO_DIRS;/* Clear if we should not    */
                                       /* display files              */
  smask[0] = IGNORE;                   /* No mask unless specified   */
  dmask[0] = IGNORE;                   /* No mask unless specified   */
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);/* pass back result           */
 
                                       /* validate arguments         */
  if (numargs < 2 || numargs > 5 ||
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]) ||
      args[0].strlength > 255)
    return INVALID_ROUTINE;            /* Invalid call to routine    */
                                       /* initialize data area       */
  ldp.count = 0;
  strcpy(ldp.varname, args[1].strptr);
  ldp.stemlen = args[1].strlength;
  strupr(ldp.varname);                 /* uppercase the name         */
 
  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';
 
  strcpy(FileSpec, args[0].strptr);    /* get file spec              */
 
   /** If FileSpec ends in \ then append *.*  *                    */
 
  if (FileSpec[strlen(FileSpec)-1] == '\\')
    strcat(FileSpec, "*.*");
 
  if (numargs >= 3 &&                  /* check third option         */
      !RXNULLSTRING(args[2])) {
    if (!args[2].strlength)            /* a zero length string isn't */
      return INVALID_ROUTINE;          /* valid                      */
    strcpy(Opts, args[2].strptr);
    strupr(Opts);                      /* Uppercase the string       */
 
    if (strchr(Opts, 'S') != NULL)
      options |= RECURSE;              /* Should we recurse          */
 
    if (strchr(Opts, 'O') != NULL)
      options |= NAME_ONLY;            /* Should include names only  */
 
    if (strchr(Opts, 'T') != NULL)
      options |= EDITABLE_TIME;          /* Should create editable     */
                                       /* timestamp                  */
 
    if (strchr(Opts, 'F') != NULL) {
      options &= ~DO_DIRS;             /* Should not include dirs !  */
      options |= DO_FILES;             /* Should include files !     */
    }
 
    if (strchr(Opts, 'D') != NULL) {
      options |= DO_DIRS;              /* Should include dirs !      */
      options &= ~DO_FILES;            /* Should not include files ! */
    }
 
    if (strchr(Opts, 'B') != NULL) {
      options |= DO_DIRS;              /* Should include dirs !      */
      options |= DO_FILES;             /* Should include files !     */
    }
  }
 
  if (numargs >= 4) {                  /* check fourth option        */
    strcpy(Opts, args[3].strptr);
 
    smask[0] = smask[1] = smask[2] = smask[3] = smask[4] = 0;
    x = min(5, strlen(Opts));
 
    for (y = 0; y < x; y++) {
 
      if (Opts[y] == '+')
        smask[y] = 1;
 
      if (Opts[y] == '-')
        smask[y] = -1;
    }
  }
 
  if (numargs == 5) {                  /* check fifth argument       */
    strcpy(Opts, args[4].strptr);      /* point to string            */
    dmask[0] = dmask[1] = dmask[2] = dmask[3] = dmask[4] = 0;
 
    x = min(5, strlen(Opts));
 
    for (y = 0; y < x; y++) {
 
      if (Opts[y] == '+')
        dmask[y] = 1;
 
      if (Opts[y] == '-')
        dmask[y] = -1;
    }
    dmask[1] = 0;                      /* Ignore directory bit of    */
                                       /* destination mask           */
  }
                                       /* recursively search         */
  if (RecursiveFindFile(FileSpec, &ldp, smask, dmask, options))
    return INVALID_ROUTINE;
                                       /* return lines read          */
  sprintf(ldp.ibuf, "%d", ldp.count);
  ldp.varname[ldp.stemlen] = '0';
  ldp.varname[ldp.stemlen+1] = 0;
  ldp.shvb.shvnext = NULL;
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = ldp.stemlen+1;
  ldp.shvb.shvnamelen = ldp.stemlen+1;
  ldp.shvb.shvvalue.strptr = ldp.ibuf;
  ldp.shvb.shvvalue.strlength = strlen(ldp.ibuf);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
    return INVALID_ROUTINE;            /* error on non-zero          */
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
 
/*************************************************************************
* Function:  SysGetMessage                                               *
*                                                                        *
* Syntax:    call SysGetMessage [file], msgnum [,str1] ... [,str9]       *
*                                                                        *
* Params:    file           - Name of message file to get message from.  *
*                              Default is OSO001.MSG.                    *
*            msgnum         - Number of message being queried.           *
*            str1 ... str9  - Insertion strings.  For messages which     *
*                              contain %1, %2, etc, the str1, str2, etc  *
*                              strings will be inserted (if given).      *
*                                                                        *
* Return:    The message with the inserted strings (if given).           *
*************************************************************************/
 
ULONG SysGetMessage(PUCHAR name, ULONG numargs, RXSTRING args[],
                            PSZ queuename, RXSTRING *retstr)
{
  PCHAR   ivtable[9];                  /* Array of pointers to       */
                                       /* variables                  */
  CHAR    dataarea[500];               /* Buffer address to return   */
                                       /* message                    */
  ULONG   msglen;                      /* Length of the returned     */
                                       /* message                    */
  ULONG   datalen = 500;               /* Length of the input buffer */
  ULONG   file_handle = 1;             /* Output device handle for   */
                                       /* the screen                 */
  ULONG   ivcount = 3;                 /* Number of variables to     */
                                       /* insert = 0                 */
  LONG    msgnum;                      /* Message number to get      */
  LONG    x;                           /* Loop counter               */
  PSZ     msgfile;                     /* Message file               */
 
  if (numargs < 1 || numargs > 11 ||   /* validate arguments         */
      !RXVALIDSTRING(args[0]))
    return INVALID_ROUTINE;            /* exit with error            */
                                       /* get message number         */
  if (!string2long(args[0].strptr, &msgnum) || msgnum < 0)
    return INVALID_ROUTINE;            /* exit with error            */
                                       /* Get message file name.     */
                                       /* Use "OSO001.MSG if not     */
                                       /* given                      */
  if (numargs >= 2 && RXVALIDSTRING(args[1]))
    msgfile = args[1].strptr;          /* use provided message file  */
  else
    msgfile = "OSO001.MSG";
 
                                       /* copy insertion string      */
                                       /* pointers                   */
  for (x = 2; x < numargs; x++) {
    if (RXNULLSTRING(args[x]))         /* must be valid string       */
      return INVALID_ROUTINE;          /* leave now if bad           */
    ivtable[x-2] = args[x].strptr;
  }
                                       /* set number of insertions   */
  if (numargs >= 2)
    ivcount = numargs-2;
  else
    ivcount = 0;
                                       /* get the message            */
  DosGetMessage(ivtable, ivcount, dataarea, datalen, msgnum, msgfile,
      &msglen);
                                       /* save the message           */
  if (msglen > retstr->strlength)
    if (DosAllocMem((PPVOID)&retstr->strptr, msglen, AllocFlag)) {
      BUILDRXSTRING(retstr, ERROR_NOMEM);
      return VALID_ROUTINE;
    }
  memcpy(retstr->strptr, dataarea, msglen);
  retstr->strlength = msglen;
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
/*************************************************************************
* Function:  SysIni                                                      *
*                                                                        *
* Syntax:    call SysIni [inifile], app [,key/stem] [,val/stem]          *
*                                                                        *
* Params:    inifile - INI file from which to query or write info.  The  *
*                       default is the current user INI file.            *
*            app     - Application title to work with.  May be either    *
*                       of the following:                                *
*                        'ALL:' - All app titles will be returned in the *
*                                  stem variable specified by the next   *
*                                  parameter.                            *
*                        other  - Specific app to work with.             *
*            key     - Key to work with.  May be any of the following:   *
*                        'ALL:'    - All key titles will be returned in  *
*                                     the stem variable specified by the *
*                                     next parameter.                    *
*                        'DELETE:' - All keys associated with the app    *
*                                     will be deleted.                   *
*                        other     - Specific key to work with.          *
*            val     - Key to work with. May be either of the following: *
*                        'DELETE:' - Delete app/key pair.                *
*                        other     - Set app/key pair info to data spec- *
*                                     ified.                             *
*            stem    - Name of stem variable in which to store results.  *
*                      Stem.0 = Number found (n).                        *
*                      Stem.1 - Stem.n = Entries found.                  *
*                                                                        *
* Return:    other          - Info queried from specific app/key pair.   *
*            ''             - Info queried and placed in stem or data    *
*                              deleted successfully.                     *
*            ERROR_NOMEM    - Out of memory.                             *
*            ERROR_RETSTR   - Error opening INI or querying/writing info.*
*************************************************************************/
 
ULONG SysIni(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, RXSTRING *retstr)
{
  HAB         hab;                     /* Anchor block               */
  HINI        hini;                    /* Ini file handle            */
  ULONG       x;                       /* Temp counter               */
  ULONG       len;                     /* Len var used when creating */
                                       /* stem                       */
  ULONG       lSize;                   /* Size of queried info buffer*/
                                       /* area                       */
  PSZ         IniFile;                 /* Ini file (USER, SYSTEM,    */
                                       /* BOTH, file)                */
  PSZ         App;                     /* Application field          */
  PSZ         Key;                     /* Key field                  */
  CHAR       *Val;                     /* Ptr to data associated w/  */
                                       /* App->Key                   */
  CHAR        Temp[256];               /* Temp string var            */
  CHAR        UserName[256];           /* Filename of User INI       */
                                       /* profile                    */
  CHAR        SysName[256];            /* Filename of System INI     */
                                       /* profile                    */
  PRFPROFILE  PrfInfo;                 /* Profile name structure     */
  LONG        Error = FALSE;           /* Set to true if error       */
                                       /* encountered                */
  BOOL        WildCard = FALSE;        /* Set to true if a wildcard  */
                                       /* operation                  */
  BOOL        QueryApps;               /* Set to true if a query     */
                                       /* operation                  */
  BOOL        terminate = TRUE;        /* perform WinTerminate call  */
  RXSTEMDATA  ldp;                     /* local data                 */
 
  IniFile = "";
  Key = "";
                                       /* validate arguments         */
  if (numargs < 2 ||
      numargs > 4 ||
      !RXVALIDSTRING(args[1]))
    return INVALID_ROUTINE;
                                       /* get pointers to args       */
  IniFile = args[0].strptr;
  App = args[1].strptr;
 
  if (numargs >= 3 && args[2].strptr)
    Key = args[2].strptr;
 
  if (numargs == 4)
    Val = args[3].strptr;
                                       /* Check KEY and APP values   */
                                       /* for "WildCard"             */
  if (!stricmp(App, "ALL:")) {
    App = "";
    QueryApps = TRUE;
    WildCard = TRUE;
 
    if (numargs != 3)
      return INVALID_ROUTINE;          /* Error - Not enough args    */
    else
      x = 2;                           /* Arg number of STEM variable*/
  }
 
  else if (!stricmp(Key, "ALL:")) {
    Key = "";
    Val = "";
    QueryApps = FALSE;
    WildCard = TRUE;
 
    if (numargs != 4)
      return INVALID_ROUTINE;          /* Error - Not enough args    */
 
    else
      x = 3;                           /* Arg number of STEM variable*/
  }
                                       /* If this is a "WildCard     */
                                       /* search, then allocate mem  */
                                       /* for stem struct and get the*/
                                       /* stem name                  */
  if (WildCard == TRUE) {
 
    ldp.count = 0;                     /* get the stem variable name */
    strcpy(ldp.varname, args[x].strptr);
    ldp.stemlen = args[x].strlength;
    strupr(ldp.varname);                 /* uppercase the name         */
 
    if (ldp.varname[ldp.stemlen-1] != '.')
      ldp.varname[ldp.stemlen++] = '.';
  }
 
  hab = WinInitialize((USHORT)0);      /* create anchor block        */
  if (!hab) {                          /* already done?              */
                                       /* get desktop anchor         */
    hab = WinQueryAnchorBlock(HWND_DESKTOP);
    terminate = FALSE;                 /* don't terminate            */
  }
 
      /**************************************************************
      * The following section of code gets the INI file handle      *
      * given the INI file spec (IniFile).                          *
      *                                                             *
      * Possible Ini file specs:                                    *
      *                                                             *
      *   NULL     - Same as USERPROFILE   ( OS2.INI )              *
      *   "BOTH"   - Same as PROFILE       ( OS2.INI & OS2SYS.INI ) *
      *   "USER"   - Same as USERPROFILE   ( OS2.INI )              *
      *   "SYSTEM" - Same as SYSTEMPROFILE ( OS2SYS.INI)            *
      *   other    - Filespec of INI file.                          *
      **************************************************************/
 
  hini = NULLHANDLE;
 
  if (!IniFile)
    hini = HINI_USERPROFILE;
  else if (!stricmp(IniFile, "BOTH"))
    hini = HINI_PROFILE;
  else if (!stricmp(IniFile, "USER"))
    hini = HINI_USERPROFILE;
  else if (!stricmp(IniFile, "SYSTEM"))
    hini = HINI_SYSTEMPROFILE;
 
      /***********************************************************
      * If Ini file spec is 'other' then make sure it does not   *
      * specify the current USER or SYSTEM profiles.             *
      *                                                          *
      * Trying to open the current USER or SYSTEM ini file via   *
      * PrfOpenProfile() will fail.  Therefore, use the function *
      * PrfQueryProfile() to query the current USER and SYSTEM   *
      * ini file specs. If the IniFile string matches either     *
      * the current USER or SYSTEM file specs, then use either   *
      * HINI_USERPROFILE or HINI_SYSTEMPROFILE as appropriate.   *
      *                                                          *
      * If IniFile does not specify the current USER or SYSTEM   *
      * ini file then use PrfOpenProfile() to get the ini file   *
      * handle.                                                  *
      ***********************************************************/
 
  else {
    PrfInfo.pszUserName = UserName;
    PrfInfo.cchUserName = sizeof(UserName);
    PrfInfo.pszSysName = SysName;
    PrfInfo.cchSysName = sizeof(SysName);
 
    if (PrfQueryProfile(hab, &PrfInfo)) {
 
      if (!stricmp(IniFile, PrfInfo.pszUserName))
        hini = HINI_USERPROFILE;
      else if (!stricmp(IniFile, PrfInfo.pszSysName))
        hini = HINI_SYSTEMPROFILE;
      else
        hini = PrfOpenProfile(hab, IniFile);
    }
    else
      hini = PrfOpenProfile(hab, IniFile);
 
      /**************************************************
      * Exit with INI FILE error if the ini file handle *
      * is NULL at this point (and if IniFile != BOTH,  *
      * as that would make the handle NULL also).       *
      **************************************************/
 
    if (hini == NULLHANDLE && stricmp(IniFile, "BOTH")) {
      BUILDRXSTRING(retstr, ERROR_RETSTR);
      if (terminate)
        WinTerminate(hab);
      return VALID_ROUTINE;
    }
  }
                                       /* get value if is a query    */
  if ((numargs == 3 && stricmp(Key, "DELETE:")) ||
      WildCard == TRUE) {
    lSize = 0x0000ffffL;
                                       /* Allocate a large buffer    */
    if (DosAllocMem((PPVOID)&Val, lSize, AllocFlag)) {
      PrfCloseProfile(hini);           /* close the INI file         */
      if (terminate)
        WinTerminate(hab);
      BUILDRXSTRING(retstr, ERROR_NOMEM);
      return VALID_ROUTINE;
    }
 
    if (WildCard && QueryApps)
      Error = !PrfQueryProfileData(hini, NULL, NULL, Val, &lSize);
    else if (WildCard && !QueryApps)
      Error = !PrfQueryProfileData(hini, App, NULL, Val, &lSize);
    else
      Error = !PrfQueryProfileData(hini, App, Key, Val, &lSize);
 
    if (Error) {
      BUILDRXSTRING(retstr, ERROR_RETSTR);
    }
    else if (WildCard == FALSE) {
      if (lSize > retstr->strlength)
        if (DosAllocMem((PPVOID)&retstr->strptr, lSize, AllocFlag)) {
          DosFreeMem(Val);             /* release buffer             */
          PrfCloseProfile(hini);       /* close the INI file         */
          if (terminate)
            WinTerminate(hab);
          BUILDRXSTRING(retstr, ERROR_NOMEM);
          return VALID_ROUTINE;
        }
      memcpy(retstr->strptr, Val, lSize);
      retstr->strlength = lSize;
      DosFreeMem(Val);                 /* release buffer             */
    }
  }
  else {
                                       /* set the var to new value   */
    if (!stricmp(Key, "DELETE:") ||
        !RXVALIDSTRING(args[2]))
      Error = !PrfWriteProfileData(hini, App, NULL, NULL, 0L);
    else if (!stricmp(Val, "DELETE:") ||
        !RXVALIDSTRING(args[3]))
      Error = !PrfWriteProfileData(hini, App, Key, NULL, 0L);
    else {
      lSize = args[3].strlength;
      Error = !PrfWriteProfileData(hini, App, Key, Val, lSize);
    }
 
    if (Error) {
      BUILDRXSTRING(retstr, ERROR_RETSTR);
    }
  }
 
  PrfCloseProfile(hini);               /* close the INI file         */
 
  if (terminate)
    WinTerminate(hab);                 /* destroy anchor block       */
 
      /******************************************
      * If this was a wildcard search, change   *
      * the Val variable from one long string   *
      * of values to a REXX stem variable.      *
      ******************************************/
 
  if (WildCard == TRUE) {              /* fill stem variable         */
 
    if (Error == FALSE) {
      x = 0;
      ldp.count = 0;
 
      do {
  /* Copy string terminated by \0 to Temp.  Last string will end     */
  /* in \0\0 and thus have a length of 0.                            */
        len = 0;
 
        while (Val[x+len] != '\0')
          Temp[len++] = Val[x+len];
        Temp[len] = '\0';
                                       /* if non-zero length, then   */
                                       /* set the stem element       */
        if (len != 0) {
          x += (len+1);                /* Increment pointer past the */
                                       /* new string                 */
          memcpy(ldp.ibuf, Temp, len);
          ldp.vlen = len;
          ldp.count++;
          sprintf(ldp.varname+ldp.stemlen, "%d", ldp.count);
 
          if (ldp.ibuf[ldp.vlen-1] == '\n')
            ldp.vlen--;
          ldp.shvb.shvnext = NULL;
          ldp.shvb.shvname.strptr = ldp.varname;
          ldp.shvb.shvname.strlength = strlen(ldp.varname);
          ldp.shvb.shvvalue.strptr = ldp.ibuf;
          ldp.shvb.shvvalue.strlength = ldp.vlen;
          ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
          ldp.shvb.shvvaluelen = ldp.vlen;
          ldp.shvb.shvcode = RXSHV_SET;
          ldp.shvb.shvret = 0;
          if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
            return INVALID_ROUTINE;    /* error on non-zero          */
        }
      }
 
      while (Val[x] != '\0');
    }
 
    else
      ldp.count = 0;
                                       /* set number returned        */
    sprintf(ldp.ibuf, "%d", ldp.count);
    ldp.varname[ldp.stemlen] = '0';
    ldp.varname[ldp.stemlen+1] = 0;
    ldp.shvb.shvnext = NULL;
    ldp.shvb.shvname.strptr = ldp.varname;
    ldp.shvb.shvname.strlength = ldp.stemlen+1;
    ldp.shvb.shvnamelen = ldp.stemlen+1;
    ldp.shvb.shvvalue.strptr = ldp.ibuf;
    ldp.shvb.shvvalue.strlength = strlen(ldp.ibuf);
    ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
    ldp.shvb.shvcode = RXSHV_SET;
    ldp.shvb.shvret = 0;
    if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
      return INVALID_ROUTINE;          /* error on non-zero          */
 
  }                                    /* * End - IF (Wildcard ... * */
 
  return VALID_ROUTINE;                /* no error on call           */
}
 
 
/*************************************************************************
* Function:  SysLoadFuncs                                                *
*                                                                        *
* Syntax:    call SysLoadFuncs [option]                                  *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    null string                                                 *
*************************************************************************/
 
ULONG SysLoadFuncs(PUCHAR name, ULONG numargs, RXSTRING args[],
                           PSZ queuename, RXSTRING *retstr)
{
  INT    entries;                      /* Num of entries             */
  INT    j;                            /* Counter                    */
 
  retstr->strlength = 0;               /* set return value           */
                                       /* check arguments            */
  if (numargs > 0)
    return INVALID_ROUTINE;
 
  entries = sizeof(RxFncTable)/sizeof(PSZ);
 
  for (j = 0; j < entries; j++) {
    RexxRegisterFunctionDll(RxFncTable[j],
          "REXXUTIL", RxFncTable[j]);
  }
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysMkDir                                                    *
*                                                                        *
* Syntax:    call SysMkDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be created.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from DosMkDir()                                 *
*************************************************************************/
 
ULONG SysMkDir(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  ULONG  rc;                           /* Ret code of func           */
 
  if (numargs != 1)
                                       /* If no args, then its an    */
                                       /* incorrect call             */
    return INVALID_ROUTINE;
  rc = DosMkDir(args[0].strptr, 0L);   /* make the directory         */
  sprintf(retstr->strptr, "%d", rc);   /* result is return code      */
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysOS2Ver                                                   *
*                                                                        *
* Syntax:    call SysOS2Ver                                              *
*                                                                        *
* Return:    OS/2 Version in for 1.1, 1.2, 1.3, 2.0.                     *
*************************************************************************/
 
ULONG SysOS2Ver(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, RXSTRING *retstr)
{
  ULONG  rc = 0;                       /* Return code of this func   */
  ULONG  Versions[2];                  /* Major version number       */
 
  if (numargs != 0)                    /* validate arg count         */
    return INVALID_ROUTINE;
 
  DosQuerySysInfo(QSV_VERSION_MAJOR, QSV_VERSION_MINOR, Versions,
      sizeof(Versions));
  sprintf(retstr->strptr, "%lu.%02lu", Versions[0]/10, Versions[1]);
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
/*************************************************************************
* Function:  SysRmDir                                                    *
*                                                                        *
* Syntax:    call SysRmDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be removed.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from DosRmDir()                                 *
*************************************************************************/
 
ULONG SysRmDir(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  ULONG  rc;                           /* Ret code of func           */
 
  if (numargs != 1)
                                       /* If no args, then its an    */
                                       /* incorrect call             */
    return INVALID_ROUTINE;
                                       /* remove the directory       */
  rc = DosRmDir(args[0].strptr);
  sprintf(retstr->strptr, "%d", rc);
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysSearchPath                                               *
*                                                                        *
* Syntax:    call SysSearchPath path, file                               *
*                                                                        *
* Params:    path - Environment variable name which specifies a path     *
*                    to be searched (ie 'PATH', 'DPATH', etc).           *
*            file - The file to search for.                              *
*                                                                        *
* Return:    other  - Full path and filespec of found file.              *
*            ''     - Specified file not found along path.               *
*************************************************************************/
 
ULONG SysSearchPath(PUCHAR name, ULONG numargs, RXSTRING args[],
                            PSZ queuename, RXSTRING *retstr)
{
  UCHAR    buf[MAX];                   /* returned file name         */
 
                                       /* validate arguments         */
  if (numargs != 2 ||
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]))
    return INVALID_ROUTINE;
 
                                       /* use DosSearchPath          */
  DosSearchPath(7, args[0].strptr, args[1].strptr,
      buf, sizeof(buf));
 
  BUILDRXSTRING(retstr, (const char *)buf);          /* pass back result           */
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysSleep                                                    *
*                                                                        *
* Syntax:    call SysSleep secs                                          *
*                                                                        *
* Params:    secs - Number of seconds to sleep.                          *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*************************************************************************/
 
ULONG SysSleep(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  LONG secs;                           /* Time to sleep in secs      */
 
  if (numargs != 1)                    /* Must have one argument     */
    return INVALID_ROUTINE;
                                       /* get number of seconds      */
  if (!string2long(args[0].strptr, &secs) || secs < 0)
    return INVALID_ROUTINE;            /* raise error if bad         */
 
  DosSleep(secs *1000);
  BUILDRXSTRING(retstr, NO_UTIL_ERROR);
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysTempFileName                                             *
*                                                                        *
* Syntax:    call SysTempFileName template [,filler]                     *
*                                                                        *
* Params:    template - Description of filespec desired.  For example:   *
*                        C:\TEMP\FILE.???                                *
*            filler   - A character which when found in template will be *
*                        replaced with random digits until a unique file *
*                        or directory is found.  The default character   *
*                        is '?'.                                         *
*                                                                        *
* Return:    other - Unique file/directory name.                         *
*            ''    - No more files exist given specified template.       *
*************************************************************************/
 
ULONG SysTempFileName(PUCHAR name, ULONG numargs, RXSTRING args[],
                              PSZ queuename, RXSTRING *retstr)
{
  CHAR   filler;                       /* filler character           */
 
  if (numargs < 1 ||                   /* validate arguments         */
      numargs > 2 ||
      !RXVALIDSTRING(args[0]) ||
      args[0].strlength > 512)
    return INVALID_ROUTINE;
 
  if (numargs == 2 &&                  /* get filler character       */
      !RXNULLSTRING(args[1])) {
    if (args[1].strlength != 1)        /* must be one character      */
      return INVALID_ROUTINE;
    filler = args[1].strptr[0];
  }
  else
    filler = '?';
                                       /* get the file id            */
  GetUniqueFileName(args[0].strptr, filler, retstr->strptr);
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysTextScreenRead                                           *
*                                                                        *
* Syntax:    call SysTextScreenRead row, col [,len]                      *
*                                                                        *
* Params:    row - Horizontal row on the screen to start reading from.   *
*                   The row at the top of the screen is 0.               *
*            col - Vertical column on the screen to start reading from.  *
*                   The column at the left of the screen is 0.           *
*            len - The number of characters to read.  The default is the *
*                   rest of the screen.                                  *
*                                                                        *
* Return:    Characters read from text screen.                           *
*************************************************************************/
 
ULONG SysTextScreenRead(PUCHAR name, ULONG numargs, RXSTRING args[],
                                PSZ queuename, RXSTRING *retstr)
{
  LONG  row;                           /* Row from which to start    */
                                       /* read                       */
  LONG  col;                           /* Column from which to start */
                                       /* read                       */
  LONG len = 8160;                     /* Max length of string,      */
                                       /* default is 8160 (80x102)   */
  CHAR  temp[8160];                    /* Array of CHARs, aka PCH    */
 
  if (numargs < 2 ||                   /* validate arguments         */
      numargs > 3 ||
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]) ||
      !string2long(args[0].strptr, &row) || row < 0 ||
      !string2long(args[1].strptr, &col) || col < 0)
    return INVALID_ROUTINE;
 
  if (numargs == 3) {                  /* check the length           */
    if (!RXVALIDSTRING(args[2]) ||     /* bad string?                */
        !string2long(args[2].strptr, &len) || len < 0)
      return INVALID_ROUTINE;          /* error                      */
  }
                                       /* read the screen            */
  VioReadCharStr( temp, (PUSHORT)&len, row, col, (HVIO) 0);
 
  if (len > retstr->strlength)         /* default too short?         */
                                       /* allocate a new one         */
    if (DosAllocMem((PPVOID)&retstr->strptr, len, AllocFlag)) {
      BUILDRXSTRING(retstr, ERROR_NOMEM);
      return VALID_ROUTINE;
    }
  memcpy(retstr->strptr, temp, len);
  retstr->strlength = len;
 
  return VALID_ROUTINE;
}
 
 
/*************************************************************************
* Function:  SysTextScreenSize                                           *
*                                                                        *
* Syntax:    call SysTextScreenSize                                      *
*                                                                        *
* Return:    Size of screen in row and columns returned as:  row, col    *
*************************************************************************/
 
ULONG SysTextScreenSize(PUCHAR name, ULONG numargs, RXSTRING args[],
                                PSZ queuename, RXSTRING *retstr)
{
  VIOMODEINFO  ModeData;               /* screen info structure      */
 
  if (numargs != 0)                    /* no arguments on this func. */
    return INVALID_ROUTINE;
 
  ModeData.cb = sizeof(ModeData);      /* find out screen size       */
  VioGetMode(&ModeData, (HVIO) 0);
  sprintf(retstr->strptr, "%d %d",(int)ModeData.row,(int)ModeData.col);
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
/*************************************************************************
* Function:  SysGetEA                                                    *
*                                                                        *
* Syntax:    call SysGetEA file, EA_name, variable                       *
*                                                                        *
* Params:    file - file containing EA.                                  *
*            EA_name - name of EA to be retrieved                        *
*            name of variable EA is placed in                            *
*                                                                        *
* Return:    Return code from DosQueryFileInfo.                          *
*************************************************************************/
 
ULONG SysGetEA(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  LONG rc;                             /* Ret code                   */
  UCHAR       geabuff[300];            /* buffer for GEA             */
  PVOID       fealist;                 /* fealist buffer             */
  EAOP2       eaop;                    /* eaop structure             */
  PGEA2       pgea;                    /* pgea structure             */
  PFEA2       pfea;                    /* pfea structure             */
  HFILE       handle;                  /* file handle                */
  ULONG       act;                     /* open action                */
  RXSTEMDATA  ldp;                     /* stem data                  */
 
 
  if (numargs != 3 ||                  /* wrong number of arguments? */
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]) ||
      !RXVALIDSTRING(args[2]))
    return INVALID_ROUTINE;            /* raise error condition      */
 
  ldp.count = 0;                       /* get the stem variable name */
  strcpy(ldp.varname, args[2].strptr);
  ldp.stemlen = args[2].strlength;
  strupr(ldp.varname);                 /* uppercase the name         */
 
  if (rc = DosOpen(args[0].strptr, &handle, &act,
      0L, 0, OPEN_ACTION_OPEN_IF_EXISTS,
      OPEN_ACCESS_READONLY + OPEN_SHARE_DENYREADWRITE +
      OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH,NULL)) {
    sprintf(retstr->strptr, "%d", rc);
    retstr->strlength = strlen(retstr->strptr);
    return VALID_ROUTINE;
  }                                    /* get the file status info   */
 
  if (DosAllocMem((PPVOID)&fealist, 0x00010000L, AllocFlag)) {
    BUILDRXSTRING(retstr, ERROR_NOMEM);
    return VALID_ROUTINE;
  }
                                       /* FEA and GEA lists          */
  eaop.fpGEA2List = (PGEA2LIST)geabuff;
  eaop.fpFEA2List = (PFEA2LIST)fealist;
  eaop.oError = 0;                     /* no error occurred yet      */
  pgea = &eaop.fpGEA2List->list[0];    /* point to first GEA         */
  eaop.fpGEA2List->cbList = sizeof(ULONG) + sizeof(GEA2) +
      args[1].strlength;
  eaop.fpFEA2List->cbList = (ULONG)0xffff;
 
                                       /* fill in the EA name length */
  pgea->cbName = (BYTE)args[1].strlength;
  strcpy(pgea->szName, args[1].strptr);/* fill in the name           */
  pgea->oNextEntryOffset = 0;          /* fill in the next offset    */
                                       /* read the extended attribute*/
  rc = DosQueryFileInfo(handle, 3, (PSZ)&eaop, sizeof(EAOP2));
  DosClose(handle);                    /* close the file             */
  if (eaop.fpFEA2List->cbList <= sizeof(ULONG))
    rc = ERROR_EAS_NOT_SUPPORTED;      /* this is error also         */
 
  sprintf(retstr->strptr, "%d", rc);   /* format return code         */
  retstr->strlength = strlen(retstr->strptr);
 
  if (rc) {                            /* failure?                   */
    DosFreeMem(fealist);               /* error, get out             */
    return VALID_ROUTINE;
  }
 
  pfea = &(eaop.fpFEA2List->list[0]);  /* point to the first FEA     */
  ldp.shvb.shvnext = NULL;
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = ldp.stemlen;
  ldp.shvb.shvnamelen = ldp.stemlen;
  ldp.shvb.shvvalue.strptr = ((PSZ)pfea->szName+(pfea->cbName+1));
  ldp.shvb.shvvalue.strlength = pfea->cbValue;
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    DosFreeMem(fealist);               /* free our buffer            */
    return INVALID_ROUTINE;            /* error on non-zero          */
  }
 
  DosFreeMem(fealist);                 /* free our buffer            */
  return VALID_ROUTINE;
}
 
/*************************************************************************
* Function:  SysPutEA                                                    *
*                                                                        *
* Syntax:    call SysPutEA file, EA_name, value                          *
*                                                                        *
* Params:    file - file containing EA.                                  *
*            EA_name - name of EA to be written                          *
*            new value for the EA                                        *
*                                                                        *
* Return:    Return code from DosQueryFileInfo.                          *
*************************************************************************/
 
ULONG SysPutEA(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  LONG rc;                             /* Ret code                   */
  PVOID       fealist;                 /* fealist buffer             */
  EAOP2       eaop;                    /* eaop structure             */
  PFEA2       pfea;                    /* pfea structure             */
  HFILE       handle;                  /* file handle                */
  ULONG       act;                     /* open action                */
 
 
  if (numargs != 3 ||                  /* wrong number of arguments? */
      !RXVALIDSTRING(args[0]) ||
      !RXVALIDSTRING(args[1]))
    return INVALID_ROUTINE;            /* raise error condition      */
 
  if (rc = DosOpen2(args[0].strptr, &handle, &act,
      0L, 0, OPEN_ACTION_OPEN_IF_EXISTS,
      OPEN_ACCESS_READWRITE + OPEN_SHARE_DENYWRITE +
      OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH, NULL)) {
    sprintf(retstr->strptr, "%d", rc);
    retstr->strlength = strlen(retstr->strptr);
    return VALID_ROUTINE;
  }
 
  if (DosAllocMem((PPVOID)&fealist, 0x00010000L, AllocFlag)) {
    BUILDRXSTRING(retstr, ERROR_NOMEM);
    return VALID_ROUTINE;
  }
 
  eaop.fpFEA2List = (PFEA2LIST)fealist;/* Set memory for the FEA     */
  eaop.fpGEA2List = NULL;              /* GEA is unused              */
  pfea = &eaop.fpFEA2List->list[0];    /* point to first FEA         */
  pfea->fEA = '\0';                    /* set the flags              */
                                       /* Size of FEA name field     */
  pfea->cbName = (BYTE)args[1].strlength;
                                       /* Size of Value for this one */
  pfea->cbValue = (SHORT)args[2].strlength;
                                       /* Set the name of this FEA   */
  strcpy((PSZ)pfea->szName, args[1].strptr);
                                       /* Set the EA value           */
  memcpy((PSZ)pfea->szName+(pfea->cbName+1), args[2].strptr,
      args[2].strlength);
  pfea->oNextEntryOffset = 0;          /* no next entry              */
  eaop.fpFEA2List->cbList =            /* Set the total size var     */
      sizeof(ULONG) + sizeof(FEA2) + pfea->cbName + pfea->cbValue;
 
                                       /* set the file info          */
  rc = DosSetFileInfo(handle, 2, (PSZ)&eaop, sizeof(EAOP2));
  DosClose(handle);                    /* Close the File             */
  DosFreeMem(fealist);                 /* Free the memory            */
  sprintf(retstr->strptr, "%d", rc);   /* format return code         */
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
 
/*************************************************************************
* Function:  SysWaitNamedPipe                                            *
*                                                                        *
* Syntax:    result = SysWaitNamedPipe(name, timeout)                    *
*                                                                        *
* Params:    name - name of the pipe                                     *
*            timeout - amount of time to wait.                           *
*                                                                        *
* Return:    Return code from DosWaitNPipe                               *
*************************************************************************/
 
ULONG SysWaitNamedPipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, RXSTRING *retstr)
{
  LONG        rc;                      /* Ret code                   */
  LONG        timeout;                 /* timeout value              */
 
 
  if (numargs < 1 ||                   /* wrong number of arguments? */
      numargs > 2 ||
      !RXVALIDSTRING(args[0]))
    return INVALID_ROUTINE;            /* raise error condition      */
 
  if (numargs == 2) {                  /* have a timeout value?      */
    if (!string2long(args[1].strptr, &timeout) ||
        (timeout < 0 && timeout != -1))
      return INVALID_ROUTINE;          /* raise error condition      */
  }
 
  rc = DosWaitNPipe(args[0].strptr, timeout);
  sprintf(retstr->strptr, "%d", rc);
  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}
