/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1990,1996.                              */
/*                                                                          */
/*   All rights reserved.                                                   */
/*                                                                          */
/*   US Government Users Restricted Rights -                                */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule          */
/*   Contract with IBM Corp.                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/************************** RECV.C *****************************************/
/* TCP client */

/*
 * Include Files.
 */
#define INCL_DOSPROCESS
#define INCL_32.h
#define INCL_DOS.h
#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * Client Main.
 */
void main(int argc, char *argv[])
{
    unsigned short port;       /* port client will connect to              */
    char buf[1024];              /* data buffer for sending and receiving    */
    struct hostent *hostnm;    /* server host name information             */
    struct sockaddr_in server; /* server address                           */
    int s , i,p;                     /* client socket                            */
    int err;
    char tempstr[100];

    /*
     * Check arguments. Should be only one: the port number to bind to.
     */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [server port]\n", argv[0]);
        exit(1);
    }

    sock_init();

    port = (unsigned int  ) atoi(argv[1]);
    printf("port of server is %d\n", port);

    /*
     * Put a message into the buffer.
     */
    strcpy(buf, "messages");

    /*
     * Put the server information into the server structure.
     * The port must be put into network byte order.
     */

    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    /*
     * Get a stream socket.
     */

     if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)

    {
        psock_errno("Socket()");
        exit(3);
    }

    /*
     * Connect to the server.
     */
   printf("connecting\n");
    if ( (err=connect(s, (struct sockaddr *)&server, sizeof(server))) < 0)
    {
        printf("\nThe Socket Error Number is %d\n",sock_errno());
        psock_errno("Err Msg thru psock_errno:");
        exit(4);
    }

    printf("Connected\n:ENTER int VARIAABLE:\n");
    scanf("%d",&p);

          if (send(s, buf, 1024, 0) < 0)
            {
                psock_errno("Send()");
                exit(5);
            }
            printf("sent\n");


    /*
     * Close the socket.
     */
    soclose(s);
    printf("Client Ended Successfully\n");
    exit(0);

}

