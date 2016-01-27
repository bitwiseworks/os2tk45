/****************************************************************************/
/*
 *    PROGRAM NAME: USER32
 *    ------------
 *
 *    What this program does:  Creates a new user; creates
 *    a new alias, gives the user access to it, and makes it a
 *    logon assignment for the user; creates a new group and
 *    adds the user to it; creates a private application definition
 *    and assigns it to the user.  Displays information about the user.
 *    Deletes the user information.
 *
 *    SYNTAX:
 *    ------
 *    USER32 username
 *      displays information about the user 'username'
 *
 *    USER32 username /add
 *      adds a definition for the user 'username', adds username to
 *      a new group, gives username logon and application assignments.
 *
 *    USER32 username /del
 *      deletes the user definition for 'username'.
 *
 *    REQUIRED FILES:
 *    --------------
 *    USER32.C       -  Source code for this program
 *
 *    REQUIRED LIBRARIES:
 *    ------------------
 *    NETAPI32.LIB   -  Netapi library (in \IBMLAN\NETSRC\LIB directory)
 *
 *    NetAPI32 functions used in this program:
 *    ---------------------------------------
 *    Net32AccessAdd
 *    Net32AccessGetInfo
 *    Net32AccessSetInfo
 *    Net32AliasAdd
 *    Net32AliasDel
 *    Net32AliasGetInfo
 *    Net32AppAdd
 *    Net32GetDCName
 *    Net32GroupAdd
 *    Net32GroupAddUser
 *    Net32GroupDel
 *    Net32GroupGetInfo
 *    Net32ServerEnum2
 *    Net32WkstaGetInfo
 *    Net32UserAdd
 *    Net32UserDCDBInit
 *    Net32UserDel
 *    Net32UserGetGroups
 *    Net32UserGetInfo
 *    Net32UserGetLogonAsn
 *    Net32UserSetAppSel
 *    Net32UserSetLogonAsn
 *
 *    HOW TO COMPILE THIS PROGRAM:
 *    ---------------------------
 *    icc /Gt+ /DPURE_32 user32.c
 */
/****************************************************************************/

/**************   Defines   *************************************************/
#define    DISPLAY     0
#define    ADD         1
#define    DELETE      2
#define    BUFFER_LEN  4096
#define    INCL_DOSPROCESS
#define    INCL_DOSMEMMGR
#define    INCL_DOSERRORS

/**************   OS/2 include files ****************************************/
#include <os2.h>

/**************   C-language include files   ********************************/
#include <stdio.h>
#include <string.h>

/**************   LAN Server include files   ********************************/
#include <netcons.h>
#include <neterr.h>
#include <access.h>
#include <dcdb.h>
#include <server.h>
#include <wksta.h>

/**************   Forward declarations of local functions   ******************/
VOID Error_Message(USHORT, PSZ);
VOID Syntax(VOID);
VOID AddUser(char *);
USHORT UserAdd(char *, char *, char *);
USHORT AliasAdd(char *, char *, char *);
USHORT GiveAccessToAlias(char *, char *, char *, char *);
USHORT AddGroup(char *, char *, char *, char *);
USHORT InitDCDBForUser(char *, char *);
USHORT AddLogAsn(char *, char *, char *, char *);
USHORT CreateApplication(char *, char *, char *, char *, char *);
USHORT AssignAppToUser(char *, char *, char *, char *);
VOID DeleteGroup(char *, char *);
VOID DeleteAlias(char *, char *);
VOID DeleteUser(char *, char *);
VOID DisplayUserDetails(char *);
VOID GetDCName(char *);

/**************   Main program   *********************************************/
/*
 * This function simply validates the command line arguments and
 * then calls a worker function, based on command line input.
 */
VOID
main(int argc, char *argv[])
{
    USHORT                 usFunction = DISPLAY;
    CHAR                   pUser[UNLEN+1];

    /* Validate command line input */
    if (argc < 2 || argc > 3)
        Syntax();

    /* Verify that username is not too long. */
    if (strlen(argv[1]) > UNLEN)
        Syntax();

    /* If a command line switch was specified, it must be "/ADD" or "/DELETE" */
    if (argc == 3)
    {
        if (!stricmp(argv[2], "/ADD"))
            usFunction = ADD;
        else if (!strnicmp(argv[2], "/DEL", strlen("/DEL")))
            usFunction = DELETE;
        else
            Syntax();
    }

    strcpy(pUser, argv[1]);

    /* Call the appropriate worker function. */
    if (usFunction == ADD)
        AddUser(pUser);
    else if (usFunction == DELETE)
        DeleteUser(pUser, NULL);
    else
        DisplayUserDetails(pUser);

     DosExit(EXIT_PROCESS, 0);
}

/*
 * This function takes the username specified on the command line as
 * input and attempts to:
 *  - Add a new definition for the username
 *  - Create a new group definition, and add the user to it.
 *  - Add a new files alias, "_DUMMYFA", and give the user access to it.
 *  - Make the new alias a logon assignment for the user.
 *  - Create a private application for the user, and assign it to the user.
 *
 * If any of the attempted actions fail, the previous actions are undone
 * and the program ends.
 */
void
AddUser(char *pUser)
{
    USHORT                     usRc;
    PCHAR                      pBuffer;
    CHAR                       pszDomainController[UNCLEN+1];

    /* Get the name of the domain controller for this domain. */
    GetDCName(pszDomainController);

    usRc = DosAllocMem((PPVOID)&pBuffer,
                       BUFFER_LEN,
                       PAG_WRITE | PAG_READ | PAG_COMMIT);

    if (usRc)
        Error_Message(usRc, "DosAllocMem");
    else
    {
        memset(pBuffer, 0, BUFFER_LEN);

        /* Add a new user definition. */
        if (!usRc)
            usRc = UserAdd(pUser, pszDomainController, pBuffer);

        /* Create a new group, and add the user to it. */
        if (!usRc)
        {
            printf("The user definition for \"%s\" was added successfully.\n",
                    strupr(pUser));

            usRc = AddGroup(pszDomainController, pUser, "NEWGROUP", pBuffer);
            if (usRc)
                DeleteUser(pUser, pszDomainController);
            else
                printf("A new group, \"NEWGROUP\" was added successfully.\n");
        }

        /* Attempt to create an new files alias, "_DUMMYFA". */
        if (!usRc)
        {
            usRc = AliasAdd(pszDomainController, "_DUMMYFA", pBuffer);
            if (usRc)
                DeleteUser(pUser, pszDomainController);
            else
                printf("The alias \"_DUMMYFA\" was added successfully.\n");
        }

        /* Give the user access to the new alias. */
        if (!usRc)
        {
            usRc = GiveAccessToAlias(pUser,
                                     pszDomainController,
                                     "_DUMMYFA",
                                     pBuffer);
            if (usRc)
            {
                DeleteUser(pUser, pszDomainController);
            }
        }

        /*
         * Initialize the new user's domain control database area.
         * This step must be done before private applications can
         * be created for the user, and before any logon or application
         * assignments can be made for the user.
         */
        if (!usRc)
        {
            usRc = InitDCDBForUser(pszDomainController, pUser);
            if (usRc)
            {
                DeleteUser(pUser, pszDomainController);
            }
            else
                printf("The domain control database structures for \"%s\" "\
                       "were successfully created.\n", strupr(pUser));
        }

        /* Add the new alias as a logon assignment. */
        if (!usRc)
        {
            usRc = AddLogAsn(pUser, pszDomainController, "_DUMMYFA", pBuffer);
            if (usRc)
            {
                DeleteUser(pUser, pszDomainController);
            }
            else
                printf("A logon assignment for \"%s\" was added successfully.\n",
                        strupr(pUser));
        }

        /* Create a private application for the user. */
        if (!usRc)
        {
            usRc = CreateApplication(pUser,
                                     "DUMMYAPP",
                                     "_DUMMYFA",
                                     pszDomainController,
                                     pBuffer);
            if (usRc)
            {
                DeleteUser(pUser, pszDomainController);
            }
            else
                printf("A private application for user \"%s\" "\
                       "was created successfully.\n", strupr(pUser));
        }

        /* Assign the new application to the user. */
        if (!usRc)
        {
            usRc = AssignAppToUser(pUser, pszDomainController, "DUMMYAPP", pBuffer);
            if (usRc)
            {
                DeleteUser(pUser, pszDomainController);
            }
            else
                printf("The new private application was successfully "\
                       "assigned to \"%s\".\n", strupr(pUser));
        }

        /* Free the buffer. */
        DosFreeMem((PVOID)pBuffer);
    }

    if (usRc)
        DosExit(EXIT_PROCESS, (ULONG)usRc);

    return;

}

/*
 * DeleteUser() calls the Net32UserDel API to delete a user definition.
 * The Net32UserDel API will delete any domain control database structures
 * associated with the user, including logon assignment information,
 * application assignment information, and definitions of any private
 * applications the user may have defined, as well as deleting the
 * user definition from the network accounts file, NET.ACC.
 *
 * Note that the Net32UserDel() API not only deletes the definition
 * of a user, but also:
 * - deletes any existing domain control database directories and files,
 *   thus deleting all of the user's logon assignments, application
 *   assignments, and private application definitions.
 * - deletes any access control entries for the user.
 * - deletes the user from any groups he was a member of.
 *
 * A new group and a new alias may have been added before this routine is
 * called; to clean up, the group and the alias are deleted if they exist.
 */
void
DeleteUser(char *pszUser, char *pszDCName)
{
    USHORT             usRc;
    CHAR               pszServer[UNCLEN+1];
    CHAR               pBuffer[GNLEN+1];
    ULONG              ulBytesAvailable;

    /*
     * This routine is called in two instances: in one case,
     * the domain controller name is known; in the other case,
     * it is not known.  If it is not known, the caller passes
     * in a NULL pointer so the function knows to determine
     * the dc name before proceeding.
     */
    if (pszDCName == NULL)
    {
        GetDCName(pszServer);
    }

    usRc = Net32UserDel(pszDCName ? pszDCName : pszServer,
                        pszUser,
                        NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserDel");
    else
        printf("The definition of \"%s\" was successfully deleted.\n",
               strupr(pszUser));

    /* See if the group NEWGROUP exists.  If so, delete it. */
    usRc = Net32GroupGetInfo(pszDCName ? pszDCName : pszServer,
                             "NEWGROUP",
                             0L,
                             pBuffer,
                             GNLEN+1,
                             &ulBytesAvailable,
                             0L);

    if (!usRc)
        DeleteGroup("NEWGROUP", pszDCName ? pszDCName : pszServer);

    /* See if the alias _DUMMYFA exists.  If so, delete it. */
    usRc = Net32AliasGetInfo(pszDCName ? pszDCName : pszServer,
                             "_DUMMYFA",
                             0L,
                             pBuffer,
                             GNLEN+1,
                             &ulBytesAvailable,
                             0L);

    if (!usRc)
        DeleteAlias("_DUMMYFA", pszDCName ? pszDCName : pszServer);

    return;
}

void
DisplayUserDetails(char *pszUser)
{
    USHORT                      usRc,
                                usCount;
    ULONG                       ulTotalEntries,
                                ulAvailable;
    PCHAR                       pBuffer;
    struct user_info_11        *pUserInfo;
    struct group_info_0        *pGroups;
    struct logon_asn_list      *pLogList;
    CHAR                        pszDC[UNCLEN + 1];

    /* allocate a buffer for the Net32UserGetInfo call */
    usRc = DosAllocMem((PPVOID)&pBuffer,
                       4096,
                       PAG_WRITE | PAG_READ | PAG_COMMIT);

    if (usRc)
    {
        Error_Message(usRc, "DosAllocMem");
        return;
    }

    /* Get the name of the domain controller. */
    GetDCName(pszDC);

    /*
     * We arbitrarily decide to get the amount of information available
     * in the user_info_11 structure.  More information is available
     * to administrators in the user_info_2 structure.
     */
    usRc = Net32UserGetInfo(pszDC,
                            pszUser,
                            11,
                            pBuffer,
                            4096,
                            &ulAvailable,
                            NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserGetInfo");
    else
    {
        /* Display selected portions of the user_info_11 structure... */
        pUserInfo = (struct user_info_11 *)pBuffer;
        printf("Details for user %s:\n\n", pUserInfo -> usri11_name);
        printf("   Comment: %s\n", pUserInfo -> usri11_comment);
        printf("   User comment: %s\n", pUserInfo -> usri11_usr_comment);
        printf("   Full Name: %s\n", pUserInfo -> usri11_full_name);
        printf("   Privilege level: %s\n",
                pUserInfo -> usri11_priv == USER_PRIV_GUEST ? "Guest" :
                (pUserInfo -> usri11_priv == USER_PRIV_USER ? "User" :
                 "Administrator"));
        printf("   Home directory: %s\n", pUserInfo -> usri11_home_dir);
        printf("   Preferred logon server: ");
        if (!pUserInfo -> usri11_logon_server ||
            *pUserInfo -> usri11_logon_server == '\0')
            printf("Domain controller\n");
        else if (strcmp(pUserInfo -> usri11_logon_server, "\\\\*"))
            printf("Any server\n");
        else
            printf("%s\n", pUserInfo -> usri11_logon_server);

        /* Now, get group membership information for this user. */
        usRc = Net32UserGetGroups(pszDC,
                               pszUser,
                               0,
                               pBuffer,
                               4096,
                               &ulAvailable,
                               &ulTotalEntries,
                               NULL);

        if (usRc)
            Error_Message(usRc, "Net32UserGetGroups");
        else
        {
            /* print group memberships */
            usCount = (USHORT)ulAvailable;
            pGroups = (struct group_info_0 *)pBuffer;
            printf("\nUser \"%s\" belongs to the following groups:\n",
                    strupr(pszUser));
            while (usCount)
            {
                printf("\t%s\n", pGroups -> grpi0_name);
                --usCount;
                ++pGroups;
            }

            /*
             * print logon assignment information...get only
             * assignments to files aliases.
             */
            usRc = Net32UserGetLogonAsn(pszDC,
                                        pszUser,
                                        1,
                                        ALIAS_TYPE_FILE,
                                        pBuffer,
                                        4096,
                                        &ulAvailable,
                                        NULL);

            if (usRc == ERROR_PATH_NOT_FOUND)
                printf("The domain control database directory "\
                       "for \"%s\" was not found.\n", strupr(pszUser));
            else if (usRc)
                Error_Message(usRc, "Net32UserGetLogonAsn");
            else
            {
                usCount = ((struct logon_asn_info_1 *)pBuffer) -> lai1_count;
                if (usCount == 0)
                    printf("\n\"%s\" has no files logon assignments.\n",
                            strupr(pszUser));
                else
                {
                    pLogList =
                     (struct logon_asn_list *)(pBuffer + sizeof(struct logon_asn_info_1));

                    printf("\nLogon assignments for user \"%s\":\n",
                           strupr(pszUser));
                    while (usCount)
                    {
                        printf("\t%-8.8s   %-8.8s\n",
                                pLogList -> lal_alias,
                                pLogList -> lal_device);

                        ++pLogList;
                        --usCount;
                    }
                }
            }
            /*
             * This routine does not display application assignment
             * information.  This information can be obtained by
             * a call to Net32UserGetAppSel().
             */
        }
    }
    DosFreeMem((PVOID)pBuffer);
    return;
}

void
GetDCName(char *pszDCName)
{
    USHORT                 usRc;
    ULONG                  ulEntriesReturned,
                           ulAvailable;
    PCHAR                  pBuffer;
    struct wksta_info_10  *pWkstaInf;

    /* Allocate a buffer to get local requester information. */
    usRc = DosAllocMem((PPVOID)&pBuffer,
                       4096,
                       PAG_READ | PAG_WRITE | PAG_COMMIT);

    if (usRc)
        Error_Message(usRc, "DosAllocMem");

    /*
     * Get the name of the domain this machine is logged onto.
     */
    usRc = Net32WkstaGetInfo(NULL,
                             10,
                             pBuffer,
                             1024,
                             &ulAvailable);

    if (usRc)
    {
        Error_Message(usRc, "Net32WkstaGetInfo");
        DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    pWkstaInf = (struct wksta_info_10 *)pBuffer;

    if (!pWkstaInf -> wki10_logon_domain ||
        !(*pWkstaInf -> wki10_logon_domain))
    {
        printf("Net32WkstaGetInfo: cannot determine logon domain.\n");
        DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /*
     * Call Net32ServerEnum2 to get the name of the domain controller.
     * Note that the buffer is only large enough to hold one servername,
     * since only one domain controller per domain is expected.
     * Net32ServerEnum2 returns a servername without leading backslashes,
     * so the code must explicitly insert the backslashes in the buffer.
     */
    *pszDCName = *(pszDCName+1) = '\\';

    usRc = Net32ServerEnum2(NULL,
                            1,
                            pszDCName,
                            UNCLEN - 1,
                            &ulEntriesReturned,
                            &ulAvailable,
                            SV_TYPE_DOMAIN_CTRL,
                            pWkstaInf -> wki10_logon_domain);

    /*
     * If an error was returned or the number of entries
     * returned is not one, try another method of finding
     * the domain controller name.
     */
    if (usRc || ulEntriesReturned != 1)
    {
        usRc = Net32GetDCName(NULL,
                              pWkstaInf -> wki10_logon_domain,
                              pszDCName,
                              UNCLEN+1);
    }

    DosFreeMem((PVOID)pBuffer);

    if (usRc)
    {
        printf("Unable to determine domain controller name.\n");
        Error_Message(usRc, "Net32GetDCName");
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    return;
}

VOID
DeleteGroup(char *pszGroupName, char *pszDCName)
{
    USHORT             usRc;

    usRc = Net32GroupDel(pszDCName,
                         pszGroupName,
                         NULL);

    if (usRc)
        Error_Message(usRc, "Net32GroupDel");
    else
        printf("The definition of group %s was deleted successfully.\n",
                pszGroupName);

    return;
}

VOID
DeleteAlias(char *pszAliasName, char *pszDCName)
{
    USHORT             usRc;

    usRc = Net32AliasDel(pszDCName,
                         pszAliasName,
                         0L,
                         NULL);

    if (usRc)
        Error_Message(usRc, "Net32AliasDel");
    else
        printf("The definition of alias %s was deleted successfully.\n",
                pszAliasName);

    return;
}

VOID
Syntax()
{
    printf("Usage:\n");
    printf("   USER32 <username> -- displays information about <username>\n");
    printf("   USER32 <username> /ADD -- adds a user definition for <username>\n");
    printf("   USER32 <username> /DELETE -- deletes the defintion of <username>\n");
    DosExit(EXIT_PROCESS, 1);
}

USHORT
UserAdd(char *pszUser, char *pszDCName, char *pBuffer)
{
    USHORT                      usRc;
    struct user_info_2         *pUserInfo = (struct user_info_2 *)pBuffer;
    CHAR                        pszHomeDir[UNCLEN + 1 + 2 + 1 + UNLEN + 1];

    /*
     * Fill in the user_info_2 structure: we're creating
     * a normal user with no password, and a home directory
     * on the domain controller.
     */
    memset(pBuffer, 0, BUFFER_LEN);

    strcpy(pUserInfo -> usri2_name, pszUser);

    /* The user has no password */
    *pUserInfo -> usri2_password = '\0';

    /* The user has 'normal' user privilege. */
    pUserInfo -> usri2_priv = USER_PRIV_USER;

    /* set up the home directory string */
    strcpy(pszHomeDir, pszDCName);
    strcat(pszHomeDir, "\\C$\\");
    strcat(pszHomeDir, pszUser);

    /*
     * The user's home directory is the C:\<username> directory,
     * on the domain controller.
     */
    pUserInfo -> usri2_home_dir = pszHomeDir;

    pUserInfo -> usri2_comment =
    pUserInfo -> usri2_usr_comment = "userid added by USER sample program";

    /* A password is not required for this user. */
    pUserInfo -> usri2_flags = UF_SCRIPT | UF_PASSWD_NOTREQD;
    pUserInfo -> usri2_full_name = "This isn't a real user!";

    /* The user's account never expires. */
    pUserInfo -> usri2_acct_expires = TIMEQ_FOREVER;

    /* Any server can validate this user's logon. */
    pUserInfo -> usri2_logon_server = "\\\\*";

    usRc = Net32UserAdd(pszDCName,
                        2,
                        pBuffer,
                        BUFFER_LEN,
                        NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserAdd");

    return(usRc);
}

USHORT
AddGroup(PCHAR pszDCName, PCHAR pszUserName, PCHAR pszGroupName, PCHAR pBuffer)
{
    USHORT                 usRc;

    memset(pBuffer, 0, sizeof(struct group_info_0));
    strcpy(pBuffer, pszGroupName);

    usRc = Net32GroupAdd(pszDCName,
                         0,
                         pBuffer,
                         sizeof(struct group_info_0),
                         NULL);

    if (usRc)
        Error_Message(usRc, "Net32GroupAdd");

    /* If the group was added successfully, add the user to the new group. */
    if (!usRc)
    {
        usRc = Net32GroupAddUser(pszDCName,
                                 pszGroupName,
                                 pszUserName,
                                 NULL);
        if (usRc)
            Error_Message(usRc, "Net32GroupAddUser");
    }

    return(usRc);
}

USHORT
AliasAdd(char *pszDCName, char *pszAliasName, char *pBuffer)
{
    USHORT                      usRc;
    struct alias_info_2        *pAlias = (struct alias_info_2 *)pBuffer;

    /*
     * Set the alias_info_2 structure to zero.
     */
    memset(pAlias, 0, sizeof(struct alias_info_2));

    strcpy(pAlias -> ai2_alias, pszAliasName);
    pAlias -> ai2_remark = "Alias added by USER sample program.";
    pAlias -> ai2_type = ALIAS_TYPE_FILE;
    pAlias -> ai2_location = ALIAS_LOCATION_INTERNAL;
    strcpy(pAlias -> ai2_server, pszDCName + 2);
    pAlias -> ai2_mode = ALIAS_MODE_BYADMIN;
    pAlias -> ai2_maxuses = 10;
    strcpy(pAlias -> ai2_netname, pszAliasName);
    pAlias -> ai2_path = "C:\\OS2\\APPS";

    usRc = Net32AliasAdd(pszDCName,
                         2,
                         pBuffer,
                         BUFFER_LEN,
                         NULL);

    if (usRc)
        Error_Message(usRc, "Net32AliasAdd");

    return(usRc);
}

USHORT
GiveAccessToAlias(char *pszUser,
                  char *pszDCName,
                  char *pszAliasName,
                  char *pBuffer)
{
    USHORT                      usRc;
    ULONG                       ulBytesAvailable;
    CHAR                        szServer[UNCLEN + 1];
    PCHAR                       pszPath;
    struct alias_info_2        *pAliasInfo;
    struct access_list         *pAccessList;
    struct access_info_1       *pAccInfo;

    /* Determine the server and resource associated with the alias. */
    usRc = Net32AliasGetInfo(pszDCName,
                             pszAliasName,
                             2,
                             pBuffer + (BUFFER_LEN/2),
                             BUFFER_LEN/2,
                             &ulBytesAvailable,
                             NULL);

    if (usRc)
        Error_Message(usRc, "Net32AliasGetInfo");

    if (!usRc)
    {
        pAliasInfo = (struct alias_info_2 *)(pBuffer + (BUFFER_LEN/2));
        pszPath = pAliasInfo -> ai2_path;
        *szServer = *(szServer+1) = '\\';
        strcpy(szServer+2, pAliasInfo -> ai2_server);

        /* Now an access control profile can be created for the resource. */
        /* First, see if access control for the resource already exists.  */
        usRc = Net32AccessGetInfo(szServer,
                                  pszPath,
                                  1,
                                  pBuffer,
                                  BUFFER_LEN,
                                  &ulBytesAvailable);

        if (usRc && usRc != NERR_ResourceNotFound)
            Error_Message(usRc, "Net32AccessGetInfo");
        else
        {
            pAccessList =
                (struct access_list *)(pBuffer + sizeof(struct access_info_1));

            if (usRc == NERR_ResourceNotFound)
            {
                pAccInfo = (struct access_info_1 *)pBuffer;
                pAccInfo -> acc1_resource_name = pszPath;
                pAccInfo -> acc1_attr = 0;
                pAccInfo -> acc1_count = 1;
                strcpy(pAccessList -> acl_ugname, pszUser);
                pAccessList -> acl_access = ACCESS_READ;

                usRc = Net32AccessAdd(szServer,
                                      1,
                                      pBuffer,
                                      BUFFER_LEN);

                if (usRc)
                    Error_Message(usRc, "Net32AccessAdd");
            }
            else
            {
                /*
                 * Add an access_list structure for the user.
                 * We will assume that there is no access control
                 * entry for this user, since he was just added.
                 * Point past the last access_list structure,
                 * fill in values for this user, and update the count
                 * of access control entries.
                 */
                pAccessList += ((struct access_info_1 *)pBuffer) -> acc1_count;
                strcpy(pAccessList -> acl_ugname, pszUser);
                pAccessList -> acl_access = ACCESS_READ;
                ++(((struct access_info_1 *)pBuffer) -> acc1_count);

                usRc = Net32AccessSetInfo(szServer,
                                          pszPath,
                                          1,
                                          pBuffer,
                                          BUFFER_LEN,
                                          PARMNUM_ALL);

                if (usRc)
                    Error_Message(usRc, "Net32AccessSetInfo");
            }
        }
    }
    return(usRc);
}

USHORT
InitDCDBForUser(char *pszDCName, char *pszUser)
{
    USHORT              usRc;

    /*
     * The following function initializes the domain control
     * database directories and structures for the specified user.
     * If any database files already exist, their contents
     * are lost.
     */
    usRc = Net32UserDCDBInit(pszDCName,
                             pszUser,
                             0L,                /* reserved, must be zero */
                             NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserDCDBInit");

    return(usRc);
}

/*
 * This routine shows how to add a single logon assignment for
 * a user.
 */
USHORT
AddLogAsn(char *pszUser, char *pszDCName, char *pszAlias, char *pBuffer)
{
    USHORT                      usRc;
    ULONG                       ulBytesAvailable;
    struct logon_asn_list      *pLogList;
    struct logon_asn_info_1    *pLogInfo;

    /* First, get the user's current assignments, of ALL alias types. */
    usRc = Net32UserGetLogonAsn(pszDCName,
                                pszUser,
                                1,
                                ALIAS_TYPE_FILE | ALIAS_TYPE_PRINTER | ALIAS_TYPE_SERIAL,
                                pBuffer,
                                BUFFER_LEN,
                                &ulBytesAvailable,
                                NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserGetLogonAsn");
    else
    {
        pLogInfo = (struct logon_asn_info_1 *)pBuffer;
        pLogList =
          (struct logon_asn_list *)(pBuffer + sizeof(struct logon_asn_info_1));
        pLogList += pLogInfo -> lai1_count;
        strcpy(pLogList -> lal_alias, pszAlias);
        pLogList -> lal_type = ALIAS_TYPE_FILE;
        strcpy(pLogList -> lal_device, "M:");

        pLogInfo -> lai1_reserved = 0;
        ++(pLogInfo -> lai1_count);

        usRc = Net32UserSetLogonAsn(pszDCName,
                                    pszUser,
                                    1,
                                    pBuffer,
                                    BUFFER_LEN,
                                    NULL);

        if (usRc)
            Error_Message(usRc, "Net32UserSetLogonAsn");
    }
    return(usRc);
}

USHORT
CreateApplication(char *pszUser,
                  char *pszAppName,
                  char *pszAliasName,
                  char *pszDCName,
                  char *pBuffer)
{
    USHORT                      usRc;
    struct app_info_3          *pAppInfo;

    /*
     * Set up the app_info_3 structure.
     * A private application for the user is being created;
     * it resides remotely, on the resource created earlier
     * by the AliasAdd() routine.  It is an OS/2 private PM
     * application.  Note that the application program does
     * not have to be installed at the time that the application
     * definition is created.
     */
    pAppInfo = (struct app_info_3 *)pBuffer;

    strcpy(pAppInfo -> app3_name, pszAppName);
    pAppInfo -> app3_remark = "App created by USER sample program.";
    pAppInfo -> app3_command = "PMSEEK";
    pAppInfo -> app3_command_parms = "";
    strcpy(pAppInfo -> app3_app_alias_or_drv, pszAliasName);
    pAppInfo -> app3_app_drive = '\0';
    pAppInfo -> app3_app_path_to_dir = "\\";
    *pAppInfo -> app3_wrkdir_alias_or_drv = '\0';
    pAppInfo -> app3_wrkdir_drive = '\0';
    pAppInfo -> app3_wrkdir_path_to_dir = "";
    pAppInfo -> app3_prompt = 0;
    pAppInfo -> app3_interface = APP_PM;
    pAppInfo -> app3_apptype = APP_OS2_PRIVATE;
    pAppInfo -> app3_res_count = 0;

    usRc = Net32AppAdd(pszDCName,
                       pszUser,
                       3,
                       pBuffer,
                       BUFFER_LEN,
                       NULL);

    if (usRc)
        Error_Message(usRc, "Net32AppAdd");

    return(usRc);
}

USHORT
AssignAppToUser(char *pszUser, char *pszDCName, char *pszAppName, char *pBuffer)
{
    USHORT                      usRc;
    struct app_sel_info_1      *pAppInfo;
    struct app_sel_list        *pAppList;

    /*
     * In this routine, we assume that the user has no applications
     * assigned to him and just do a SetAppSel operation.  This is
     * a fairly safe assumption, since the user definition was just
     * created and his domain database was just initialized.  However,
     * note that the Net32UserSetAppSel operation is absolute; whatever
     * applications are in the input buffer become the applications
     * assigned to the user.  If the user had any app assignments previous
     * to the Net32UserSetAppSel call, they would be lost.  To preserve
     * existing app assignments, a call to Net32UserGetAppSel() should precede
     * the call to Net32UserSetAppSel().  Refer to AddLogAsn() for an example
     * of how to preserve a user's current assignments.
     */
    pAppInfo = (struct app_sel_info_1 *)pBuffer;
    pAppList = (struct app_sel_list *)(pBuffer + sizeof(struct app_sel_info_1));

    pAppInfo -> asi1_reserved = 0;
    pAppInfo -> asi1_count = 1;

    strcpy(pAppList -> asl_appname, pszAppName);
    pAppList -> asl_apptype = APP_OS2_PRIVATE;
    pAppList -> asl_reserved = 0;

    usRc = Net32UserSetAppSel(pszDCName,
                              pszUser,
                              1,
                              pBuffer,
                              BUFFER_LEN,
                              NULL);

    if (usRc)
        Error_Message(usRc, "Net32UserSetAppSel");

    return(usRc);
}
