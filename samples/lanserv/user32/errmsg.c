/***************************************************************************/
/*
 *    PROGRAM NAME: ERRMSG
 *    ------------
 *
 *    What this program does:
 *                     It provides a common detailed error message
 *                     generation function for programs using the NetAPI
 *                     and NetAPI32 functions and return codes.
 *
 *    REQUIRED FILES:
 *    --------------
 *    ERRMSG.C        - C source code for the Error_Message function.
 *
 *    REQUIRED LIBRARIES:
 *    ------------------
 *    None.
 *
 *    NetAPI functions used in this program :
 *    --------------------------------------
 *    None.
 */
/***************************************************************************/

/*------------------ OS/2 Include definitions -----------------------------*/

#define  INCL_BASE

#include <os2.h>
#include <neterr.h>

/*--------------------- C Include definitions -----------------------------*/

#include <stdio.h>

/***************************************************************************/
/* Error_Message Function                                                  */
/*    Prints a detailed error message (in the case of common errors)       */
/*   and/or an error number.                                               */
/*-------------------------------------------------------------------------*/

VOID
Error_Message(USHORT usRc, PSZ pszNetAPIName)
{

    switch (usRc)
    {
        case ERROR_BAD_NETPATH:
        {
            printf("%s return code : %d - The network path cannot be found\n",
                    pszNetAPIName, usRc);
            printf("Possible cause: The server is not started or does not exist.\n");
            break;
        }

        case NERR_BufTooSmall:
        {
            printf("%s return code : %d - The buffer is too small for",
                    pszNetAPIName, usRc);
            printf(" fixed-length data.\n");
            break;
        }

        case NERR_ResourceNotFound:
        {
           printf("%s return code : %d - The network resource cannot be found\n",
                   pszNetAPIName, usRc);
           break;
       }

        case NERR_RemoteOnly:
        {
            printf("%s return code : %d - This operation is not supported on\n",
                    pszNetAPIName, usRc);
            printf("                      workstations.\n");
            break;
        }

        case NERR_DeviceNotShared:
        {
            printf("%s return code : %d - Device not shared\n",
                    pszNetAPIName, usRc);
            printf("                   or device does not exist.\n");
            break;
        }

        case NERR_WkstaNotStarted:
        {
            printf("%s return code : %d - Requester service not started.\n",
                   pszNetAPIName, usRc);
            break;
        }

        case NERR_NetNameNotFound:
        {
            printf("%s return code : %d - Netname not found.\n",
                    pszNetAPIName, usRc);
            printf("            The Netname is not shared \n");
            printf("         or the Netname does not exist.\n");
            break;
        }

        case ERROR_ACCESS_DENIED:
        {
            printf("%s return code : %d - Access denied.\n",
                    pszNetAPIName, usRc);
            printf("            Administrator privilege is required.\n");
            break;
        }

        case NERR_AliasExists:
        {
            printf("%s return code : %d - The alias has already been created.\n",
                    pszNetAPIName, usRc);
            break;
        }

        case NERR_AliasNotFound:
        {
            printf("%s return code : %d - The alias does not exist.\n",
                    pszNetAPIName, usRc);
            break;
        }

        case NERR_AppExists:
        {
            printf("%s return code : %d - An application with this name "\
                   "has already been created.\n", pszNetAPIName, usRc);
            break;
        }

        case NERR_AppNotFound:
        {
            printf("%s return code : %d - The application does not exist.\n",
                    pszNetAPIName, usRc);
            break;
        }

        case NERR_DASDNotInstalled:
        {
            printf("%s return code : %d - Directory limits are not enabled\n"\
                   "on the specified drive.\n", pszNetAPIName, usRc);
            break;
        }

        case NERR_LimitExists:
        {
            printf("%s return code : %d - The directory limit already exists.\n",
                    pszNetAPIName, usRc);
            break;
        }

        case NERR_NotHPFSVolume:
        {
            printf("%s return code : %d - The directory is not on "\
                   "a 386 HPFS volume.\n", pszNetAPIName, usRc);
            break;
        }

        case NERR_DuplicateShare:
        {
            printf("%s return code : %d - The netname has already been shared.\n",
                    pszNetAPIName, usRc);
            break;
        }

        default:
        {
            printf("Unexpected return code from %s : %d\n",
                    pszNetAPIName, usRc);
            break;
        }

    }

    return;
}
