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
 * Simple TCP and UDP server using BSD select() call.
 */
#ifndef BSD_SELECT
#define BSD_SELECT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <types.h>
#include <netinet\in.h>
#include <sys\select.h>       /* contains FD_*** macros */
#include <sys\socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

main(int argc, char *argv[])
{
    unsigned short port;           /* port server binds to                  */
    char buf[32];                  /* buffer for sending and receiving data */
    struct sockaddr_in tcp_client; /* client address information            */
    struct sockaddr_in tcp_server; /* server address information            */
    struct sockaddr_in udp_client;
    struct sockaddr_in udp_server;
    int tcpsock,udpsock;           /* socket for accepting connections      */
    int ns;                        /* socket connected to client            */
    int namelen;                   /* length of client name                 */
    int sockint, client_address_size;
    fd_set rdfds;                  /* read set mask for select() call */
    int width=0;                   /* # bits to be checked for select() call */
    int readysock=0;
    struct timeval timeout;

/*
 * Check arguments. Should be only one: the port number to bind to.
 */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }

/*
 * Initialize with sockets.
 */
    if ((sockint = sock_init()) != 0)
    {
       printf(" INET.SYS probably is not running");
       exit(2);
    }

/*
 * First argument should be the port.
 */
    port = (unsigned short) atoi(argv[1]);

/*
 *
 * TCP SERVER
 *
 * Get a TCP socket for accepting connections.
 */
    if ((tcpsock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        psock_errno("Socket()");
        exit(3);
    }

/*
 * Bind the TCP socket to the server address.
 */
    tcp_server.sin_family = AF_INET;
    tcp_server.sin_port   = htons(port);
    tcp_server.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcpsock, (struct sockaddr *)&tcp_server, sizeof(tcp_server)) < 0)
    {
        psock_errno("Bind()");
        exit(4);
    }

/*
 * Listen for connections. Specify the backlog as 5.
 */
    if (listen(tcpsock, 5) != 0)
    {
        psock_errno("Listen()");
        exit(5);
    }

/*
 *  UDP SERVER
 *
 * Create a UDP socket in the internet domain and use the
 * default protocol (UDP).
 */
   if ((udpsock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
   {
       psock_errno("socket()");
       exit(6);
   }
/*
 *
 * Bind the UDP socket to the server address.
 *
 */
   udp_server.sin_family      = AF_INET;    /* Server is in Internet Domain */
   udp_server.sin_port        = htons(port);
   udp_server.sin_addr.s_addr = INADDR_ANY; /* Server's Internet Address    */

   if (bind(udpsock, (struct sockaddr *)&udp_server, sizeof(udp_server)) < 0)
   {
       psock_errno("bind()");
       exit(7);
   }

   for(;;)
      {
        /*
         *  polling every 10 seconds instead of blocking indefinitely.
         */
        timeout.tv_sec = 10l;
        timeout.tv_usec = 0l;

        /*
         * read set mask is zeroed before use
         */
        FD_ZERO( &rdfds );

        /*
         * add socket discriptor in the read set mask.
         */
        FD_SET( tcpsock, &rdfds );
        if ( tcpsock > width ) width = tcpsock;
        FD_SET( udpsock, &rdfds );
        if ( udpsock > width ) width = udpsock;

        /*
         * width can be FD_SETSIZE, i.e. 2048.
         */
        width++;

        /*
         * Select() returns the number of socket descriptors selected;
         * if it timed out, then returns 0.
         */
        if ((readysock=select(width, &rdfds, (fd_set *)0, (fd_set *)0, &timeout)) == -1)
         {
           psock_errno("Select()");
           exit(8);
         }

        if (readysock > 0)
         {
           /*
            * Check read readiness on the socket.
            */
           if(FD_ISSET(tcpsock,&rdfds))
            {
             /*
              * Accept a connection.
              */
              namelen = sizeof(tcp_client);
              if ((ns = accept(tcpsock, (struct sockaddr *)&tcp_client, &namelen)) == -1)
               {
                  psock_errno("Accept()");
                  exit(9);
               }

             /*
              * Receive the message on the newly connected socket.
              */
              if (recv(ns, buf, sizeof(buf), 0) == -1)
               {
                  psock_errno("Recv()");
                  exit(10);
               }

             /*
              * Send the message back to the client.
              */
              if (send(ns, buf, sizeof(buf), 0) < 0)
               {
                 psock_errno("Send()");
                 exit(11);
               }
              soclose(ns);
              printf("TCP Server received successfully\n");
            }
           if(FD_ISSET(udpsock,&rdfds))
            {
             /*
              * Receive a message on socket udpsock in buf  of maximum size 32
              * from a client.
              */
              client_address_size = sizeof(udp_client);

              if(recvfrom(udpsock, buf, sizeof(buf), 0, (struct sockaddr *) &udp_client,
                          &client_address_size) <0)
               {
                 psock_errno("recvfrom()");
                 exit(12);
               }
             /*
              * Print the message and the name of the client.
              */
              printf("Received message %s from domain %s port %d internet address %s\n",
              buf,
              (udp_client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
              ntohs(udp_client.sin_port),
              inet_ntoa(udp_client.sin_addr));
           }
         }
      } /* end of for(;;) loop */

}
