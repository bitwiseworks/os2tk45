/****************************************************************************/
/*
 *    PROGRAM NAME: ACCESS32
 *    ------------
 *
 *    What this program does: Adds, sets, or displays access control
 *    profiles on the server where it is running.
 *
 *    SYNTAX:
 *    ------
 *    ACCESS32 <resource> -- displays ACLs for the specified resource
 *    ACCESS32 <resource> <user:perm> <user:perm>  -- Adds or sets listed
 *      permissions for the specified resource
 *
 *    REQUIRED FILES:
 *    --------------
 *    ACCESS32.C        -  Source code for this program
 *
 *    REQUIRED LIBRARIES:
 *    ------------------
 *    NETAPI32.LIB     -  Netapi library (in \IBMLAN\NETSRC\LIB directory)
 *
 *    NetAPI functions used in this program:
 *    -------------------------------------
 *    Net32AccessAdd
 *    Net32AccessGetInfo
 *    Net32AccessSetInfo
 *
 *    HOW TO COMPILE THIS PROGRAM:
 *    ---------------------------
 *    icc /C /Gt+ /DPURE_32 access32.c
 */
/****************************************************************************/

/*------- OS/2 include files -----------------------------------------------*/
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#include <os2.h>

/*------- NET APIs include files -------------------------------------------*/
#include <neterr.h>
#include <netcons.h>
#include <access.h>
#include <wksta.h>

/*------- C include files --------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/*------- Function declaration ---------------------------------------------*/
VOID DisplayAccessInfo(struct access_info_1 *);
VOID Syntax(VOID);
VOID ParseAccessInfo(struct access_info_1 *, int, char **);
VOID Error_Message(USHORT, PSZ);

#define         ADD     1
#define         SET     2

/****************************************************************************/
/* MAIN C function                                                          */
/*--------------------------------------------------------------------------*/
VOID
main(int argc, char *argv[])
{
    USHORT                      usRc = 0,
                                usOperation = 0;
    ULONG                       ulTotalAvail = 0,
                                ulBuflen = 4096;
    struct   access_info_1     *pAccessBuf;
    PCHAR                       pszFunction = "Net32AccessGetInfo";

    if (argc == 1)
        Syntax();

    /* Allocate some memory for API calls. */

    usRc = DosAllocMem((PPVOID)&pAccessBuf,
                       ulBuflen,
                       PAG_WRITE | PAG_READ | PAG_COMMIT);

    if (usRc)
    {
        printf("Unable to allocate memory.  DosAllocMem returned %d.\n",
                usRc);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /* Get access information on the specified resource */
    usRc = Net32AccessGetInfo(NULL,
                            argv[1],
                            1L,
                            (unsigned char *)pAccessBuf,
                            ulBuflen,
                            &ulTotalAvail);

    /* If no error and user requested Display, display the access information */
    if (usRc == NERR_Success && argc == 2)      /* Display */
    {
        DisplayAccessInfo(pAccessBuf);
    }
    /* user wants to add or set access control information */
    else if (argc > 2)
    {
        /*
         * If the Get call returned no error, access control exists
         * for the resource, so a Set operation will be done.
         * If NERR_ResourceNotFound was returned, no access control
         * exists for the resource, so an Add operation will be done.
         */

        if (usRc == NERR_Success)
            usOperation = SET;
        else if (usRc == NERR_ResourceNotFound)
            usOperation = ADD;

        if (usOperation == SET)
        {
            /*
             * Parse the command line and add acl info to the buffer
             */
            pszFunction = "Net32AccessSetInfo";
            ParseAccessInfo(pAccessBuf, argc, argv);
            printf("Setting access control for %s...\n", argv[1]);
            usRc = Net32AccessSetInfo(NULL,
                                    argv[1],
                                    1,
                                    (unsigned char *)pAccessBuf,
                                    ulBuflen,
                                    PARMNUM_ALL);
        }
        else if (usRc == NERR_ResourceNotFound)
        {
            /*
             * Set up the access_info_1 structure in the buffer;
             * then parse the command line and add acl info to the buffer.
             */
            pszFunction = "Net32AccessAdd";
            pAccessBuf -> acc1_resource_name = argv[1];
            pAccessBuf -> acc1_attr = 0;
            pAccessBuf -> acc1_count = 0;

            ParseAccessInfo(pAccessBuf, argc, argv);

            printf("Adding access control for %s...\n", argv[1]);
            usRc = Net32AccessAdd(NULL,
                                1,
                                (unsigned char *)pAccessBuf,
                                ulBuflen);
        }
    }

    /* Free the buffer */
    (void)DosFreeMem((PVOID)pAccessBuf);

    /* If an error occurred, display it */
    if (usRc)
        Error_Message(usRc, pszFunction);

    if (usRc == NERR_Success &&
        (usOperation == SET || usOperation == ADD))
        printf("\tSuccess!\n");

    DosExit(EXIT_PROCESS, (ULONG)usRc);

}


/****************************************************************************/
/*  Function  DisplayAccessInfo                                             */
/*--------------------------------------------------------------------------*/
VOID DisplayAccessInfo(struct access_info_1 *buf)
{
    struct access_list *list;
    SHORT               count = buf -> acc1_count;

    /*
     * Print the resource name, its attributes, and the number
     * of access control entries associated with it.
     */
    printf("\n\nResource name         : %s ", buf -> acc1_resource_name);
    printf("\n\tAttributes    : %x ", buf -> acc1_attr);
    printf("\n\tCount         : %d", count);

    /*
     * Print information about each access control entry.
     */
    if (count)
    {
        list = (struct access_list *)(buf + 1);

        while (count--)
        {
            printf("\n\n\tUser/Group    : %s ", list -> acl_ugname);
            printf("\n\tPermissions   : ");
            if (!list -> acl_access)
                printf("NONE");
            else
            {
                if (list -> acl_access & ACCESS_READ)
                    printf("R");

                if (list -> acl_access & ACCESS_WRITE)
                    printf("W");

                if (list -> acl_access & ACCESS_CREATE)
                    printf("C");

                if (list -> acl_access & ACCESS_EXEC)
                    printf("X");

                if (list -> acl_access & ACCESS_DELETE)
                    printf("D");

                if (list -> acl_access & ACCESS_ATRIB)
                    printf("A");

                if (list -> acl_access & ACCESS_PERM)
                    printf("P");
            }
            list++;
        }
        printf("\n");
    }
}


/*
 *  Syntax(): Display syntax information and exit.
 */
void
Syntax()
{
    printf("Usage:\n");
    printf("   ACCESS32 <resource>\n");
    printf("      OR\n");
    printf("   ACCESS32 <resource> <name:permissions> <name:permissions> ...\n\n");
    DosExit(EXIT_PROCESS, 1);
}

/*
 *  ParseAccessInfo(): Parse the <name:permissions> command line input
 *    and add to API buffer if valid.
 */
void
ParseAccessInfo(struct access_info_1 *pBuf,
                int argc,
                char **argv)
{
    USHORT              usCount,        /* Number of access_list structures */
                                        /*   in pBuf                        */
                        i,              /* Index into argv array            */
                        j,              /* Loop counter                     */
                        usCmdLineItems; /* Number of name:permissions items */
                                        /*   specified on the command line  */
    struct access_list *pAccListNew;    /* Pointer to access_list structures*/
    struct access_list *pAccListCurrent;/*     "                            */
    BOOL                found;
    SHORT               sPermissions;   /* Permissions value                */
    PSZ                 pszPerms;       /* Pointer to "permissions" portion */
                                        /*   of name:permissions string     */

    /* Initialize variables */
    usCount = pBuf -> acc1_count;

    /*
     * The access_list structures immediately follow the
     * access_info_1 structure in the buffer.
     */
    pAccListNew = pAccListCurrent = (struct access_list *)(pBuf + 1);

    /* Any new userids/groupids will be added to the end of the
     * existing list of userids/groupids (if any). Point past any
     * current access_list structures
     */
    pAccListNew += usCount;

    /*
     * Loop through the name:permission items specified on the
     * command line, checking the permissions letters for validity.
     */
    for (usCmdLineItems = argc - 2, i = 2;
         usCmdLineItems;
         --usCmdLineItems, ++i)
    {
        /*
         * Find the colon in the command line item and replace it
         * with a null.  Display syntax and exit if no colon is found.
         */
        pszPerms = strchr(argv[i], ':');

        if (pszPerms == (char *)NULL)
        {
            (void)DosFreeMem((PVOID)pBuf);
            Syntax();
        }
        *pszPerms = '\0';

        /*
         * A colon was found...make sure a string follows it.
         */
        ++pszPerms;
        if (pszPerms == (char *)NULL ||
            pszPerms && *pszPerms == '\0')
        {
            (void)DosFreeMem((PVOID)pBuf);
            Syntax();
        }

        /*
         * Now make sure that all the permissions letters are
         * valid.  Display syntax and exit if invalid permissions
         * letters are specified.  The constant values below are
         * defined in ACCESS.H.
         */
        sPermissions = ACCESS_NONE;

        while (*pszPerms)
        {
            switch(*pszPerms)
            {
                case 'R':
                case 'r':
                    sPermissions |= ACCESS_READ;
                    break;

                case 'W':
                case 'w':
                    sPermissions |= ACCESS_WRITE;
                    break;

                case 'C':
                case 'c':
                    sPermissions |= ACCESS_CREATE;
                    break;

                case 'X':
                case 'x':
                    sPermissions |= ACCESS_EXEC;
                    break;

                case 'D':
                case 'd':
                    sPermissions |= ACCESS_DELETE;
                    break;

                case 'A':
                case 'a':
                    sPermissions |= ACCESS_ATRIB;
                    break;

                case 'P':
                case 'p':
                    sPermissions |= ACCESS_PERM;
                    break;

                case 'N':
                case 'n':
                    sPermissions = ACCESS_NONE;
                    break;

                default:
                    (void)DosFreeMem((PVOID)pBuf);
                    Syntax();
                    break;
            } /* end, switch */

            ++pszPerms;

        } /* end, while */

        /*
         * A valid permissions string was specified.  Before adding
         * the userid/groupid to the buffer, check to see if the
         * userid/groupid is already in the buffer.  If the userid/groupid
         * is already in the buffer, change the permissions for the
         * userid/groupid rather than adding another access_list structure
         * for the userid/groupid.  (Only one access_list structure
         * per userid/groupid is allowed).
         *
         * NOTE that before adding a userid/groupid to the buffer, the
         * existence of the userid/groupid could be verified by calling
         * the Net32UserGetInfo and/or Net32GroupGetInfo APIs.
         * However, if the userid/groupid does not exist, the
         * Net32UserAdd/Net32UserSetInfo APIs will return an
         * appropriate error, so existence checking is not done here.
         *
         * NOTE that with this implementation, the same userid/groupid can
         * be specified on the command line multiple times.  If the same
         * userid/groupid is specified multiple times, the last permissions
         * value specified is the one that will be set for the user/group.
         */

         /* point to the beginning of the list of users & groups */
         pAccListCurrent = (struct access_list *)(pBuf + 1);

         /* loop through the list of users and groups, comparing
          * the buffer value (pAccListCurrent -> acl_ugname) with
          * the command line value (Argv[i]).  There are usCount
          * items in the list pointed to by pAccListCurrent.
          */
         for (j = 0, found = FALSE; j < usCount && !found; ++j)
         {
             /* If the command line specifies a user or group that's
              * currently in the buffer, update that user/group's
              * permissions and break out of the for loop.
              */
             if (!stricmp(argv[i], pAccListCurrent -> acl_ugname))
             {
                 pAccListCurrent -> acl_access = sPermissions;
                 found = TRUE;
             }
             /* the names are different, so point to the next buffer item. */
             else
             {
                 ++pAccListCurrent;
             }
         }

         /* If the user/group specified on the command line is not
          * in the current buffer, add the user/group and its permissions
          * to the buffer and update the count of the number of buffer items.
          */
         if (!found)
         {
             strcpy(pAccListNew -> acl_ugname, argv[i]);
             pAccListNew -> acl_access = sPermissions;
             ++usCount;
             ++pAccListNew;
         }
    } /* end, for */

    /* Update the buffer with the number of access_list entries */
    pBuf -> acc1_count = usCount;

    return;

}
