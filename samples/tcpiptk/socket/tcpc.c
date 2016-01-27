/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1990,1991.                              */
/*                                                                          */
/*   All rights reserved.                                                   */
/*                                                                          */
/*   US Government Users Restricted Rights -                                */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule          */
/*   Contract with IBM Corp.                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/*
 * Include Files.
 */
#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

/*
 * Client Main.
 */
main(int argc, char *argv[])
{
    unsigned short port;       /* port client will connect to              */
    char buf[12];              /* data buffer for sending and receiving    */
    struct hostent *hostnm;    /* server host name information             */
    struct sockaddr_in server; /* server address                           */
    int s;                     /* client socket                            */

    /*
     * Check Arguments Passed. Should be hostname and port.
     */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    /*
     * Initialize with sockets.
     */
    sock_init();

    /*
     * The host name is the first argument. Get the server address.
     */
    hostnm = gethostbyname(argv[1]);
    if (hostnm == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(2);
    }

    /*
     * The port is the second argument.
     */
    port = (unsigned short) atoi(argv[2]);

    /*
     * Put a message into the buffer.
     */
    strcpy(buf, "the message");

    /*
     * Put the server information into the server structure.
     * The port must be put into network byte order.
     */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    /*
     * Get a stream socket.
     */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        psock_errno("Socket()");
        exit(3);
    }

    /*
     * Connect to the server.
     */
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        psock_errno("Connect()");
        exit(4);
    }

    if (send(s, buf, sizeof(buf), 0) < 0)
    {
        psock_errno("Send()");
        exit(5);
    }

    /*
     * The server sends back the same message. Receive it into the buffer.
     */
    if (recv(s, buf, sizeof(buf), 0) < 0)
    {
        psock_errno("Recv()");
        exit(6);
    }

    /*
     * Close the socket.
     */
    soclose(s);

    printf("Client Ended Successfully\n");
    exit(0);

}


