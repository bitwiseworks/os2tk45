/***************************************************************************/
/*
 *   PROGRAM NAME: FILEEN32
 *   ------------
 *
 *   What this program does:
 *               It gets information on all files within a given
 *               filepath that have been opened by a specific user.
 *               Both the path and the userid may be left unspecified
 *               to get information on all files opened by all users
 *               on the specified server.
 *
 *               The program must be executed by an administrator
 *               and may execute either locally or remotely.
 *
 *   REQUIRED FILES:
 *   --------------
 *   SAMPLE32.MAK    - IBM C SET/2 makefile
 *   FILEEN32.C      - C source code for the main program.
 *   ERRMSG.C        - C source code for the Error_Message function.
 *
 *   REQUIRED LIBRARIES:
 *   ------------------
 *   NETAPI32.LIB   -  32-bit Netapi library for OS/2
 *                     (in the IBMLAN\NETSRC\OS2\LIB directory).
 *
 *   NetAPI32 functions used in this program:
 *   ---------------------------------------
 *   Net32FileEnum2
 *
 ***************************************************************************/

/*------------------ OS/2 Include definitions -----------------------------*/

#define  INCL_BASE

#include <os2.h>
#include <limits.h>
#include <netcons.h>
#include <shares.h>
#include <neterr.h>

/*--------------------- C Include definitions -----------------------------*/

#include <stdio.h>

#ifdef __WATCOMC__
#include <mem.h>
#else
#include <memory.h>
#endif

#include <string.h>

/*--------------------- Function Declarations -----------------------------*/

VOID Error_Message(USHORT, PSZ);
VOID Syntax(VOID);

/***************************************************************************/
/* Main C Function                                                         */
/*-------------------------------------------------------------------------*/

VOID
main(USHORT argc, PCHAR argv[])
{
    USHORT                      usRc = 0,         /* API return code */
                                usEntriesprinted,
                                usMaxDataSize;
    ULONG                       ulBuflen,         /* Size of FileInfo buffer */
                                ulEntriesRead,    /* Number of entries read  */
                                ulEntriesRemaining;/*Total entries available*/
    PCHAR                       pchServername;    /* Server name             */
    PCHAR                       pchBasepath;      /* Root of path for files  */
    PCHAR                       pchUsername;      /* Userid                  */
    struct file_info_3 LSFAR   *EnumBuf;          /* FileInfo buffer         */
    struct file_info_3 LSFAR   *pchBuf;           /* Ptr. to current entry   */
    FRK                         frkResume_key;    /* Resume key: defined in  */
                                                  /* shares.h                */
    /*
     * Check the validity of the input parameters.
     * Up to 4 parameters are allowed.
     */
    if (argc > 4)
        Syntax();

    if (argc == 2 &&
        (!strcmp(argv[1], "?") || !stricmp(argv[1], "/HELP")))
            Syntax();

    /*
     * Command line parameters are positional; the servername, if any,
     * must precede the basepath (if any), which must precede the username
     * (if any).  Servername must begin with two backslashes; basepath
     * must be an absolute path beginning with a drive letter.
     */
    pchServername = pchBasepath = pchUsername = (PCHAR)NULL;
    if (argc == 2)
    {
        if (*argv[1] == '\\')
            pchServername = argv[1];
        else if (*(argv[1]+1) == ':')
            pchBasepath = argv[1];
        else
            pchUsername = argv[1];
    }
    else if (argc == 3)
    {
        /* argv[1] must be either servername or basepath */
        if (*argv[1] == '\\')
        {
            pchServername = argv[1];
            if (*(argv[2]+1) == ':')
                pchBasepath = argv[2];
            else
                pchUsername = argv[2];
        }
        else if (*(argv[1]+1) == ':')
        {
            pchBasepath = argv[1];
            pchUsername = argv[2];
        }
        else
            Syntax();
    }
    else if (argc == 4)
    {
        pchServername = argv[1];
        pchBasepath = argv[2];
        pchUsername = argv[3];
    }

    if (pchServername && (*pchServername != '\\' || *(pchServername+1) != '\\'))
    {
        printf("Invalid server name specified: %s.\n", pchServername);
        Syntax();
    }

    if (pchBasepath &&
        (*(pchBasepath+1) != ':' || *(pchBasepath+2) != '\\'))
    {
        printf("Invalid basepath specified: %s.\n", pchBasepath);
        Syntax();
    }

    if (pchUsername &&
        (*pchUsername == '\\' || *(pchUsername+1) == ':'))
    {
        printf("Invalid username specified: %s.\n", pchUsername);
        Syntax();
    }

    /*
     * Initialize the resume key.
     * (FRK_INIT is a macro that is defined in shares.h.)
     */
    FRK_INIT(frkResume_key);

    /*
     * Call NetFileEnum2 with a NULL buffer pointer and a
     * zero length buffer.  The function can't return any
     * file_info_3 data since the buffer length is 0, but
     * will return the total number of file_info_3 entries
     * (i.e., the total number of open files).  A buffer
     * will then be allocated based on the amount of
     * information available.
     */
    usRc = Net32FileEnum2(pchServername,
                          pchBasepath,
                          pchUsername,
                          3,                    /* Information level 3 */
                          (unsigned char *)EnumBuf,
                          0L,
                          (PULONG)&ulEntriesRead,
                          (PULONG)&ulEntriesRemaining,
                          (PVOID)&frkResume_key);

    /*
     * Exit if the return code is not NERR_Success
     * or ERROR_MORE_DATA.
     */
    if (usRc && usRc != ERROR_MORE_DATA)
    {
        Error_Message(usRc, "Net32FileEnum2");
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }
    else if (usRc == NERR_Success && ulEntriesRemaining == 0)
    {
        printf("There are no open files on %s.\n",
                pchServername ? pchServername : "the local server");
        DosExit(EXIT_PROCESS, 0L);
    }

    /*
     * Calculate the maximum number of bytes of data that
     * could be returned per file_info_3 structure.
     */
    usMaxDataSize = sizeof(struct file_info_3) +
                    UNLEN + 1 +
                    PATHLEN + 1;

    /*
     * Calculate the size of the buffer needed to hold
     * <ulTotalEntries> file_info_3 structures.  Note
     * that Net32FileEnum2 cannot return more than 64KB;
     * if ulTotalEntries indicates that there is more than
     * 64KB of data, a 64KB buffer is allocated and
     * Net32FileEnum2 is called multiple times.
     */
    if ((USHORT)(usMaxDataSize * ulEntriesRemaining) > USHRT_MAX)
        ulBuflen = 64 * 1024;
    else
        ulBuflen = usMaxDataSize * ulEntriesRemaining;

    usRc = DosAllocMem((PPVOID)&EnumBuf,
                        ulBuflen,
                        PAG_READ | PAG_WRITE | PAG_COMMIT);

    if (usRc)
    {
        printf("FILEEN32: Unable to allocate memory.\n");
        Error_Message(usRc, "DosAllocMem");
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    if (ulBuflen == 64 * 1024)
        ulBuflen = USHRT_MAX;

    /* Re-initialize the resume key. */
    FRK_INIT( frkResume_key);

    /*
     * If the buffer supplied to the Net32FileEnum2 function is too
     * small to hold all the available data, ERROR_MORE_DATA will
     * be returned.  In this case, the function can be called
     * repeatedly until all data is obtained (NERR_Success is
     * returned).  The buffer might be too small to hold all
     * the available data if (1) more files have been opened since
     * the first call to Net32FileEnum2, or (2) the maximum buffer
     * size (64K) has been allocated and there is more than
     * 64K of data available.
     */
    do
    {
        /* Set the buffer to zeroes. */
        memset(EnumBuf, '\x00', ulBuflen);

        usRc = Net32FileEnum2(pchServername,
                              pchBasepath,
                              pchUsername,
                              3,
                              (unsigned char *)EnumBuf,
                              ulBuflen,
                              &ulEntriesRead,
                              &ulEntriesRemaining,
                              &frkResume_key);

        switch (usRc)
        {
            case NERR_Success:
            case ERROR_MORE_DATA:
            {
                printf("Servername       : %s\n",
                        pchServername ? pchServername : "NULL");
                printf("Basepath         : %s\n",
                        pchBasepath ? pchBasepath : "NULL");
                printf("Username         : %s\n",
                        pchUsername ? pchUsername : "NULL");
                printf("Level            : 3\n");
                printf("Return code from Net32FileEnum2 : %d\n", usRc);
                printf("Entries read     : %d\n", ulEntriesRead);
                printf("Entries remaining: %d\n", ulEntriesRemaining);

                /* If there is information on open files, print it. */
                if (ulEntriesRead != 0)
                {
                    usEntriesprinted = 1;
                    pchBuf = EnumBuf;

                    do
                    {
                        printf("\n/----------------\\\n");
                        printf("| File ID Number : %ld\n", pchBuf->fi3_id);

                        printf("| App.Permissions: %02X(hex) =>",
                                pchBuf->fi3_permissions);

                        if ((pchBuf->fi3_permissions & PERM_FILE_READ) != 0)
                            printf(" Read");

                        if ((pchBuf->fi3_permissions & PERM_FILE_WRITE) != 0)
                           printf(" Write");

                        if ((pchBuf->fi3_permissions & PERM_FILE_CREATE) != 0)
                            printf(" Create");

                        printf("\n");

                        printf("| # of File locks: %d\n",
                                pchBuf->fi3_num_locks);

                        printf("| Pathname       : %s\n",
                                pchBuf->fi3_pathname);

                        printf("| Username       : %s\n",
                                pchBuf->fi3_username);

                        printf("\\----------------/\n");

                        pchBuf++;
                    } while (++usEntriesprinted <= (USHORT)ulEntriesRead);
                }
                break;
            }
            default:
            {
                /*
                 * If any error besides ERROR_MORE_DATA was returned,
                 * print the error code and drop out of the do loop.
                 */
                Error_Message(usRc,"Net32FileEnum2");
                break;
            }
        }

    } while (usRc == ERROR_MORE_DATA);

    /* Free the buffer and return. */
    (void)DosFreeMem((PVOID)EnumBuf);
    DosExit(EXIT_PROCESS, (USHORT)usRc);

}

VOID
Syntax()
{
    printf("\nUSAGE: FILEEN32 enumerates open files on a server.\n\n");
    printf("Syntax:\n");
    printf("  FILEEN32 \\\\servername basepath username\n");
    printf("  where:\n");
    printf("  - \\\\servername indicates the machine where open files will\n");
    printf("    be enumerated.  If a servername is specified, it must be the\n");
    printf("    first parameter specified.  If no servername is specified,\n");
    printf("    open files at the local server will be enumerated.\n");
    printf("  - basepath is in the form of <drive>:\\pathname.  Only open files\n");
    printf("    matching this basepath will be enumerated.  If no basepath is\n");
    printf("    specified, all open files are enumerated.\n");
    printf("  - username is the name of an existing user.  If username is\n");
    printf("    specified, only files opened by this user are enumerated.\n");
    printf("    Username must be specified as the last parameter.\n\n");
    printf("  If no parameters are specified, all open files at the local\n");
    printf("  server are enumerated.\n");

    DosExit(EXIT_PROCESS, 0);
}
