/****************************************************************************/
/*
 *    PROGRAM NAME: ALIAS32
 *    ------------
 *
 *    What this program does: Creates an alias for a files resource;
 *    creates an access control profile for the resource; shares it;
 *    attempts to add a directory space limit to the resource (but
 *    only if directory space limits are enabled).
 *
 *    SYNTAX:
 *    ------
 *    ALIAS32 aliasname [\\servername] resourcename
 *       where:
 *        - aliasname is the alias that the resource will be known by
 *          (an alias cannot be longer than 8 bytes)
 *        - \\servername is optional and is the name of the server
 *          where the resource physically resides.  If not specified,
 *          it is assumed that the resource resides on the local server.
 *        - resourcename is a fully qualified path (beginning with a
 *          drive letter).
 *
 *        Note that adding an alias via this sample program will require
 *        some cleanup after the program runs: a administrator will need
 *        to: delete the alias, delete the share, remove read access for
 *        the USERS group from the resource that the alias represents
 *        (if access for USERS did not already exist), and remove the
 *        directory limit, if one was added.  These tasks may be done
 *        with the following NET commands:
 *        - NET ALIAS <aliasname> /DEL
 *        - NET SHARE <aliasname> /DEL
 *          or, if the shared resource is on a remote server,
 *          NET ADMIN \\servername /C NET SHARE <aliasname> /DEL
 *        - NET ACCESS <resourcename>
 *          shows what users and groups have access to the resource; to
 *          delete all access, use
 *          NET ACCESS <resourcename> /DEL
 *          To remove read-only access for the USERS group, use
 *          NET ACCESS <resourcename> /REVOKE USERS
 *          The NET ADMIN command can be used in conjunction with the
 *          NET ACCESS command if the resource resides on a remote server.
 *        - NET DASD <resourcename> /DEL
 *          or
 *          NET ADMIN \\servername /C NET DASD <resourcename> /DEL
 *          if the resource resides on a remote server.
 *
 *    REQUIRED FILES:
 *    --------------
 *    ALIAS32.C        -  Source code for this program
 *
 *    REQUIRED LIBRARIES:
 *    ------------------
 *    NETAPI32.LIB     -  Netapi library (in \IBMLAN\NETSRC\LIB directory)
 *
 *    NetAPI functions used in this program:
 *    -------------------------------------
 *    Net32WkstaGetInfo
 *    Net32ServerEnum2
 *    Net32GetDCName
 *    Net32AliasAdd
 *    Net32AliasDel
 *    Net32ShareAdd
 *    Net32AccessAdd
 *    Net32AccessGetInfo
 *    Net32AccessSetInfo
 *    Net32DASDAdd
 *
 *    HOW TO COMPILE THIS PROGRAM:
 *    ---------------------------
 *    icc /C /Gt+ /DPURE_32 alias32.c
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
#include <dasd.h>
#include <dcdb.h>
#include <server.h>
#include <shares.h>
#include <wksta.h>

/*------- C include files --------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/*------- Function declaration ---------------------------------------------*/
VOID Syntax(VOID);
VOID Error_Message(USHORT, PSZ);


/*------- Main program -----------------------------------------------------*/
VOID
main(int argc, char *argv[])
{
    USHORT                      usRc = 0,
                                usCount;
    ULONG                       ulBytesAvailable,
                                ulEntriesRead;
    CHAR                        ServerName[UNCLEN+1];
    CHAR                        DCName[UNCLEN+1];
    PCHAR                       pBuffer;
    BOOL                        AccessAdded = FALSE;
    struct wksta_info_10       *pWkInfo;
    struct alias_info_2        *pAliasInfo;
    struct access_info_1       *pAccInfo;
    struct access_list         *pAccList;
    struct share_info_2        *pShareInfo;
    struct dasd_info_0         *pDasdInfo;


    /* There must be either 3 or 4 command line arguments */
    if (argc < 3 || argc > 4)
        Syntax();

    /*
     * Verify that the \\servername argument begins with two backslashes,
     * and that it's not too long.
     */
    if (argc == 4)
    {
        if ( *(argv[2]) != '\\' || *(argv[2]+1) != '\\' ||
             strlen(argv[2]) > UNCLEN )
        {
            Syntax();
        }
        else
        {
            strcpy(ServerName, argv[2]);
        }
    }

    /* Make sure that the length of the alias name is acceptable. */
    if (strlen(argv[1]) > ALIAS_LEN)
        Syntax();

    /* allocate a buffer for the API calls */
    usRc = DosAllocMem((PPVOID)&pBuffer,
                       4096,
                       PAG_WRITE | PAG_READ | PAG_COMMIT);

    if (usRc)
    {
        Error_Message(usRc, "DosAllocMem");
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /* Set the buffer to all zeroes. */
    memset(pBuffer, 0, 4096);

    /*
     * Get the name of the domain that we're logged on to so that the
     * name of the domain controller can be determined.
     */
    pWkInfo = (struct wksta_info_10 *)(pBuffer + 3072);

    usRc = Net32WkstaGetInfo(NULL,
                             10,                                /* level */
                             (unsigned char *)pWkInfo,
                             1024,
                             &ulBytesAvailable);

    if (usRc)
    {
        Error_Message(usRc, "NetWkstaGetInfo");
        (void)DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /*
     * If '\\servername' was not specified on the command line,
     * use the name of the local machine as the server where the
     * alias resides.
     */
    if (argc == 3)
    {
        strcpy(ServerName, "\\\\");
        strcat(ServerName, pWkInfo -> wki10_computername);
    }

    /*
     * Get the name of the domain controller.  The return buffer is large
     * enough to hold the name of only one server (since only one domain
     * controller per domain is expected).  The server names returned
     * by NetServerEnum2 do not begin with backslashes.
     */
    usRc = Net32ServerEnum2(NULL,
                            0,
                            DCName + 2,
                            UNCLEN - 1,
                            &ulEntriesRead,
                            &ulBytesAvailable,
                            SV_TYPE_DOMAIN_CTRL,
                            pWkInfo -> wki10_logon_domain);

    if (usRc == NERR_Success && ulEntriesRead == 1)
        *DCName = *(DCName + 1) = '\\';
    else
    {
        /*
         * NetServerEnum2 failed for some reason.  Try a different
         * method for determining the domain controller name.
         */
        usRc = Net32GetDCName(NULL,
                              pWkInfo -> wki10_logon_domain,
                              DCName,
                              UNCLEN+1);

        if (usRc)
        {
            Error_Message(usRc, "NetGetDCName");
            (void)DosFreeMem((PVOID)pBuffer);
            DosExit(EXIT_PROCESS, (ULONG)usRc);
        }
    }

    /*
     * Set up the buffer for the call to Net32AliasAdd.
     * This will be an internal files alias that is shared
     * by administrator action.  The maximum number of
     * simultaneous users of this alias will be 10.
     */
    pAliasInfo = (struct alias_info_2 *)pBuffer;

    /*
     * Note that the ai2_queue, ai2_priority, and ai2_device_pool
     * elements of the alias_info_2 structure do not apply to files
     * aliases, so these elements are not initialized here.  Since
     * the buffer has been memset to 0, these elements all have a
     * value of 0.
     */
    strcpy(pAliasInfo -> ai2_alias, argv[1]);
    pAliasInfo -> ai2_remark = "Alias created by ALIAS sample program.";
    pAliasInfo -> ai2_type = ALIAS_TYPE_FILE;
    pAliasInfo -> ai2_location = ALIAS_LOCATION_INTERNAL;
    strcpy(pAliasInfo -> ai2_server, ServerName+2);
    pAliasInfo -> ai2_mode = ALIAS_MODE_BYADMIN;
    pAliasInfo -> ai2_maxuses = 10;
    strcpy(pAliasInfo -> ai2_netname, argv[1]);
    if (argc == 3)
        pAliasInfo -> ai2_path = argv[2];
    else
        pAliasInfo -> ai2_path = argv[3];

    /* The Net32AliasAdd api must be remoted to the domain controller. */
    usRc = Net32AliasAdd(DCName,
                         2,
                         (unsigned char *)pAliasInfo,
                         sizeof(struct alias_info_2),
                         NULL);

    if (usRc)
    {
        Error_Message(usRc, "Net32AliasAdd");
        (void)DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }
    else
    {
        printf("The alias has been added successfully.\n");
    }

    /*
     * Determine whether the resource currently has an
     * access control profile.  Sometimes this api call
     * takes a while to complete, so we post a message
     * telling the user what's happening.
     */
    printf("Checking access control information for %s...\n",
            argc == 3 ? argv[2] : argv[3]);
    usRc = Net32AccessGetInfo(ServerName,
                              argc == 3 ? argv[2] : argv[3],    /* resource */
                              1,                                /* level    */
                              pBuffer,
                              4096,
                              &ulBytesAvailable);

    /*
     * If the resource has no acp, NERR_ResourceNotFound is returned.
     */
    if (usRc && usRc != NERR_ResourceNotFound)
    {
        Error_Message(usRc, "Net32AccessGetInfo");
        (void)DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /*
     * If the resource has no access control profile, set up a
     * buffer to add read only access for the group "USERS".
     */
    if (usRc == NERR_ResourceNotFound)
    {
        pAccInfo = (struct access_info_1 *)pBuffer;
        pAccList = (struct access_list *)(pBuffer + sizeof(struct access_info_1));

        pAccInfo -> acc1_resource_name = argc == 3 ?
            argv[2] : argv[3];
        pAccInfo -> acc1_attr = 0;
        pAccInfo -> acc1_count = 1;

        strcpy(pAccList -> acl_ugname, "USERS");
        pAccList -> acl_access = ACCESS_READ;

        usRc = Net32AccessAdd(ServerName,
                              1,
                              pBuffer,
                              4096);

        if (usRc)
            Error_Message(usRc, "Net32AccessAdd");
        else
            AccessAdded = TRUE;
    }
    /*
     * The resource has an access control profile.  Loop through the
     * access_list structures to see if there's one for the group "USERS".
     * If not, add read-only access for "USERS"; otherwise, do nothing.
     */
    else
    {
        usCount = ((struct access_info_1 *)pBuffer) -> acc1_count;
        pAccList = (struct access_list *)(pBuffer + sizeof(struct access_info_1));

        while (usCount)
        {
            if (!strcmp(pAccList -> acl_ugname, "USERS"))
            {
                printf("An access control entry for the group USERS already exists.\n");
                break;
            }
            --usCount;
            ++pAccList;
        }

        /*
         * If the group "USERS" wasn't found among the resource's
         * access control entries, add an access_list entry for
         * "USERS".
         */
        if (usCount == 0)
        {
            strcpy(pAccList -> acl_ugname, "USERS");
            pAccList -> acl_access = ACCESS_READ;
            ((struct access_info_1 *)pBuffer) -> acc1_count += 1;

            usRc = Net32AccessSetInfo(ServerName,
                                      argc == 3 ? argv[2] : argv[3],
                                      1,
                                      pBuffer,
                                      4096,
                                      PARMNUM_ALL);

            if (usRc)
                Error_Message(usRc, "Net32AccessSetInfo");
            else
                AccessAdded = TRUE;
        }
    }

    if (usRc == NERR_Success && AccessAdded)
    {
        printf("Read-only access added for \"USERS\" group.\n");
    }
    else if (usRc)
    {
        printf("Deleting alias definition for %s...\n", argv[1]);
        usRc = Net32AliasDel(ServerName, argv[1], 0L, NULL);
        if (usRc)
        {
            printf("Deletion of alias %s failed.\n", argv[1]);
            Error_Message(usRc, "Net32AliasDel");
        }
        else
        {
            printf("Alias %s was deleted successfully.\n", argv[1]);
        }
        (void)DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }

    /*
     * Set up the buffer to get the alias shared.
     */
    pShareInfo = (struct share_info_2 *)pBuffer;

    strcpy(pShareInfo -> shi2_netname, argv[1]);/* Aliasname == netname */
    pShareInfo -> shi2_type = STYPE_DISKTREE;
    pShareInfo -> shi2_remark = "Share added by ALIAS sample program.";
    pShareInfo -> shi2_max_uses = 10;
    if (argc == 3)
        pShareInfo -> shi2_path = argv[2];
    else
        pShareInfo -> shi2_path = argv[3];
    *pShareInfo -> shi2_passwd = '\0';
    pShareInfo -> shi2_permissions = 0;
    pShareInfo -> shi2_current_uses = 0;

    usRc = Net32ShareAdd(ServerName,
                         2,
                         pBuffer,
                         4096);

    if (usRc)
    {
        Error_Message(usRc, "Net32ShareAdd");
        printf("Deleting alias definition for %s...\n", argv[1]);
        usRc = Net32AliasDel(ServerName, argv[1], 0L, NULL);
        if (usRc)
        {
            printf("Deletion of alias %s failed.\n", argv[1]);
            Error_Message(usRc, "Net32AliasDel");
        }
        else
        {
            printf("Alias %s was deleted successfully.\n", argv[1]);
        }
        (void)DosFreeMem((PVOID)pBuffer);
        DosExit(EXIT_PROCESS, (ULONG)usRc);
    }
    else
    {
        printf("The alias was shared successfully.\n");
    }

    /*
     * Finally, attempt to add a directory limit to the resource.
     * We arbitrarily set the limit at 1MB and specify that alerts
     * are to be generated when the directory is 90% full.
     */
    pDasdInfo = (struct dasd_info_0 *)pBuffer;

    pDasdInfo -> d0_resource_name = argc == 3 ? argv[2] : argv[3];
    pDasdInfo -> d0_max = 1024;         /* 1024KB == 1Meg */
    pDasdInfo -> d0_use = 0;
    pDasdInfo -> d0_flag = DASD_VALIDATE_LIMIT_ON;
    pDasdInfo -> d0_thresh = 90;
    pDasdInfo -> d0_delta = 0;

    usRc = Net32DASDAdd(ServerName,
                        0,
                        pBuffer,
                        4096);

    if (usRc == NERR_Success)
    {
        printf("A directory limit of 1MB was set for directory %s.\n",
                argc == 3 ? argv[2] : argv[3]);
    }
    else
    {
        Error_Message(usRc, "Net32DASDAdd");
        (void)DosFreeMem((PVOID)pBuffer);
    }

    DosExit(EXIT_PROCESS, (ULONG)usRc);
}

/*
 * Display syntax information and exit.
 */
VOID
Syntax()
{
    printf("USAGE:\nThe ALIAS32 sample program adds a files alias, shares it, gives\n");
    printf("the USERS group read-only access to the resource, and adds a\n");
    printf("directory limit to the resource.\n\n");
    printf("Syntax:\n");
    printf("   ALIAS32 aliasname resourcename\n");
    printf("      OR\n");
    printf("   ALIAS32 aliasname \\\\servername resourcename\n");
    printf(" where:\n");
    printf("   - aliasname is an 8-byte name that is not currently in use.\n");
    printf("   - \\\\servername specifies the server that the alias resource\n");
    printf("     resides on.  If not specified, it is assumed that the alias\n");
    printf("     resource resides on the local machine.\n");
    printf("   - resourcename is a fully qualified path name.\n");
    DosExit(EXIT_PROCESS, 1);
}
