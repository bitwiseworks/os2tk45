/****************************************************************************/
/*
 *    PROGRAM NAME: MLOGON32
 *    ------------
 *
 *    What this program does: It logs on a user, waits for the SPACE key
 *    to be pressed, and logs off the user. User name, password (if required)
 *    and domain name are passed on the command line.
 *
 *    REQUIRED FILES:
 *    ---------------
 *    MLOGON.C        -  Source code for this program
 *
 *    REQUIRED LIBRARIES:
 *    -------------------
 *    UPM.LIB         -  UPM library (in IBMLAN\NETSRC\OS2\LIB directory)
 *
 *    HOW TO COMPILE THIS PROGRAM:
 *    ----------------------------
 *    icc /Gt+ /DPURE_32 /C mlogon.c
 *
 ****************************************************************************/

/*------- OS/2 include files -----------------------------------------------*/
#define INCL_BASE
#include <os2.h>

/*------- UPM include files ------------------------------------------------*/
#include <upm.h>

/*------- C include files --------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/****************************************************************************/
/* MAIN C function                                                          */
/*--------------------------------------------------------------------------*/
VOID
main(int argc, char *argv[])
{
    USHORT      usRc,
                usIndex;
    HKBD        KbdHandle;
    KBDKEYINFO  CharData;

    strupr(argv[0]);

    /* Validate command line input. */
    if (argc < 3)
    {
        printf("Usage: %s <username> <password> <domainname>\n",
                strupr(argv[0]));
        printf("                or\n");
        printf("Usage: %s <username> <domainname>\n",
                strupr(argv[0]));
        DosExit(EXIT_PROCESS, 0);
    }

    printf("%s:  logon is in progress...\n", strupr(argv[0]));

    /* Call the api to log the user on. */
    if (argc == 4)
    {
        strupr(argv[2]);                /* password must be UPPERCASE */

        usRc = u32eulgn(argv[1],                        /*  userid      */
                        argv[2],                        /*  password    */
                        argv[3],                        /*  remotename  */
                        (unsigned short)UPM_DOMAIN,     /*  remotetype  */
                        UPM_FL_DOMVER);                 /*  flags       */

        usIndex = 3;
    }
    else
    {
        usRc = u32eulgn(argv[1],                        /*  userid      */
                        NULL,                           /*  password    */
                        argv[2],                        /*  remotename  */
                        (unsigned short)UPM_DOMAIN,     /*  remotetype  */
                        UPM_FL_DOMVER);                 /*  flags       */

        usIndex = 2;
    }

    if (usRc == UPM_OK)
    {
        printf("%s: \"%s\" is now logged on.\n",
                strupr(argv[0]), strupr(argv[1]));
    }
    else if (usRc == UPM_LOGGED)
    {
        printf("Logon could not complete.\n"
               "The specified user may already be logged on to the specified domain.\n");
        DosExit(EXIT_PROCESS, usRc);
    }
    else
    {
        printf("Error: u32eulgn() returned = %X\n",usRc);
        DosExit(EXIT_PROCESS, usRc);
    }

    /* Create a virtual keyboard. */
    usRc = KbdOpen(&KbdHandle);

    if (usRc != 0)
    {
        printf("\nError: KbdOpen() returned = %d\n",usRc);
    }
    else
    {
        /* Set focus to the current session. */
        usRc = KbdGetFocus (0,KbdHandle);
        if (usRc != 0)
        {
            printf("\nError: KbdGetFocus() returned = %d\n",usRc);
        }
    }

    if (!usRc)
    {
        do
        {
            printf("\nTo log off, please press the spacebar.");
            fflush(stdout);

            /* Read from the keyboard until the space bar is pressed. */
            do
            {
                usRc = KbdCharIn(&CharData, 0, KbdHandle);
                if (usRc != 0)
                {
                    printf("\nError: KbdCharLn() returned = %d\n", usRc);
                    break;
                }
            } while (CharData.chChar != 0x20);

            if (!usRc)
            {
                printf("\n\n%s: logoff is in progress...\n", strupr(argv[0]));

                usRc = u32eulgf(argv[1],            /*  userid            */
                                argv[usIndex],      /*  remotename length */
                                UPM_DOMAIN);        /*  remotetype        */

                if (usRc == UPM_OK)
                {
                    printf("%s: \"%s\" was successfully logged off.\n",
                            strupr(argv[0]), strupr(argv[1]));
                    break;
                }
                else if (usRc == UPM_LOGGED)
                {
                    printf("Logoff could not complete.\nMake sure that "\
                           "no other session is using a network device "\
                           "and try again.\n");
                }
                else
                {
                    printf("Error: u32eulgf() returned = %X\n", usRc);
                    break;
                }
            }
        } while (usRc == UPM_LOGGED);
    }

    usRc = KbdClose(KbdHandle);
    if (usRc != 0)
        printf("\nError: KbdClose() returned = %d\n", usRc);

    DosExit(EXIT_PROCESS, usRc);
}
