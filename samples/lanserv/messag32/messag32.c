/****************************************************************************/
/*
 *    PROGRAM NAME: MESSG32
 *    ------------
 *
 *    What this program does:  Prompts the user for a servername,
 *      username, and message, and then sends the message by calling
 *      the Net32MessageBufferSend API.
 *
 *    SYNTAX:
 *    ------
 *    MESSG32
 *
 *    REQUIRED FILES:
 *    --------------
 *    MESSG32.C         -  Source code for this program
 *
 *    REQUIRED LIBRARIES:
 *    ------------------
 *    NETAPI32.LIB     -  Netapi library (in \IBMLAN\NETSRC\LIB directory)
 *
 *    NetAPI functions used in this program:
 *    -------------------------------------
 *    Net32MessageBufferSend
 *
 *    HOW TO COMPILE THIS PROGRAM:
 *    ---------------------------
 *    icc /Gt+ /DPURE_32 messg32.c
 */
/****************************************************************************/

/*------- OS/2 include files -----------------------------------------------*/
#include <os2.h>

/*------- NET APIs include files -------------------------------------------*/
#include <netcons.h>
#include <neterr.h>
#include <message.h>

/*------- C include files --------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/*------- Defines ----------------------------------------------------------*/
#define MAXCHAR 256
#define MAXBUF  100

void
main(void)
{
    unsigned char       pszServer[UNCLEN + 1];
    unsigned char       pszRecipient[UNLEN + 1];
    CHAR                pMsgBuf[MAXCHAR];
    USHORT              usRc,
                        usChoice;

    /* Get servername option: */
    printf("\nWhere should the message be sent from?\n");
    printf("\t1 - the local machine\n");
    printf("\t2 - a remote server\n");
    printf("\nResponse: ");
    fflush(stdout);
    scanf("%d", &usChoice);

    /* If remote server was indicated, prompt for it. */
    if (usChoice == 2)
    {
        printf("\n\nEnter the name of the remote server (maximum name length %d):\n",
                UNCLEN);
        fflush(stdout);
        scanf("%s", pszServer);
    }
    /* We'll take any value other than 2 to mean local... */
    else
    {
        printf("\n\nSending from the local machine.\n");
        fflush(stdout);
        *pszServer = '\0';
    }

    /* Prompt for the name of the message recipient. */
    printf("\nWho will the message be sent to?\n");
    printf("\t1 - a user\n");
    printf("\t2 - a domain\n");
    printf("\t3 - all requesters on the LAN\n");
    printf("\nResponse: ");
    fflush(stdout);
    scanf("%d", &usChoice);

    if (usChoice == 3)
    {
        strcpy(pszRecipient, "*");
    }
    else if (usChoice == 2)
    {
        printf("\nEnter the domain name: ");
        fflush(stdout);
        scanf("%s", pszRecipient);
        strcat(pszRecipient, "*");
    }
    /* We'll take any value other than 2 or 3 to mean a userid. */
    else
    {
        printf("\n\nEnter the name of the user: ");
        fflush(stdout);
        scanf("%s", pszRecipient);
    }

    /*
     * Userid's, domain names, etc. are stored as uppercase strings,
     * and case is important to the NetMessageBufferSend API; so,
     * uppercase the name of the recipient.
     */
    strupr(pszRecipient);

    /* Prompt for the message. */
    memset(pMsgBuf, 0, MAXCHAR);
    printf("\nEnter the message to be sent: ");
    fflush(stdout);
    fflush(stdin);
    scanf("%[^\n]", pMsgBuf);

    /* Send the message. */
    printf("Sending a message from %s to %s...\n",
            *pszServer ? pszServer : "the local machine",
            pszRecipient);
    /* debugdebugdebugdebug */
    printf("and the message is...\n\"%s\"\n", pMsgBuf);
    /* debugdebugdebugdebug */
    fflush(stdout);

    usRc = Net32MessageBufferSend(pszServer,
                                  pszRecipient,
                                  pMsgBuf,
                                  strlen(pMsgBuf));

    /* Report the return code. */
    if (usRc == NERR_Success)
        printf("\nThe message was sent successfully.\n");
    else
        printf("Error: NetMessageBufferSend returned %d.\n", usRc);

    fflush(stdout);

    DosExit(EXIT_PROCESS, usRc);
}
