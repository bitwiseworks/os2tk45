/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1996.                                   */
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
/*                                                                          */
/*****************************************************************************
*
* Single client TCP ECHO server
* -----------------------------
*
* Written by Andre Asselin
*
*
* Description
* -----------
*
* This program implements the server side of the Echo protocol (RFC 862).  The
* Echo protocol is very simple: whatever the client sends to the server, the
* server will send right back to the client.
*
* This version of the server services only 1 TCP echo client at a time.
*
* The complete RFC is reproduced below:
*

Network Working Group                                          J. Postel
Request for Comments: 862                                            ISI
                                                                May 1983

                             Echo Protocol

This RFC specifies a standard for the ARPA Internet community.  Hosts on
the ARPA Internet that choose to implement an Echo Protocol are expected
to adopt and implement this standard.

A very useful debugging and measurement tool is an echo service.  An
echo service simply sends back to the originating source any data it
receives.

TCP Based Echo Service

   One echo service is defined as a connection based application on TCP.
   A server listens for TCP connections on TCP port 7.  Once a
   connection is established any data received is sent back.  This
   continues until the calling user terminates the connection.

UDP Based Echo Service

   Another echo service is defined as a datagram based application on
   UDP.  A server listens for UDP datagrams on UDP port 7.  When a
   datagram is received, the data from it is sent back in an answering
   datagram.

**************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <netdb.h>
#include <sys\ioctl.h>
#include <sys\time.h>
#include <unistd.h>
#include <arpa\inet.h>
#include <nerrno.h>


void main(int argc, char **argv) {
   int listen_sock, newsock;
   struct servent *echoprot;
   struct sockaddr_in server, client;
   int rc, len;
   int asize, i;
   char buf[1024];

   // Initialize TCP/IP

   if (sock_init() != 0) {
      printf("INET.SYS probably is not running");
      exit(1);
   }

//------ Create the TCP server socket

   // Create a TCP socket to accept incoming connections

   listen_sock = socket(AF_INET, SOCK_STREAM, 0);
   if (listen_sock == -1) {
       psock_errno("tcp socket()");
       exit(1);
   }

   printf("TCP socket assigned is %d\n", listen_sock);

//------ Bind the TCP socket to the echo port

   // Get the port for the echo protocol out of the etc\services file

   echoprot = getservbyname("echo", "tcp");
   if (echoprot == NULL) {
      printf("The echo/tcp protocol is not listed in the etc/services file\n");
      exit(1);
   }

   server.sin_family = AF_INET;
   server.sin_port = echoprot->s_port;
   server.sin_addr.s_addr = INADDR_ANY;

   rc = bind(listen_sock, (struct sockaddr *)&server, sizeof(server));
   if (rc == -1) {
       psock_errno("tcp bind()");
       exit(1);
   }

//------ Put the socket into listen mode

   rc = listen(listen_sock, SOMAXCONN);
   if (rc == -1) {
       psock_errno("listen()");
       exit(1);
   }


   while (1) {

//------ Wait for a connection on the TCP socket

      asize = sizeof(client);
      newsock = accept(listen_sock, (struct sockaddr *)&client, &asize);
      if (newsock == -1) {
         psock_errno("accept()");
         exit(1);
      }

      printf("Received a TCP connection on socket %d from %s port %d\n",
             newsock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

      while (1) {

//------ Read the data

         len = recv(newsock, buf, sizeof(buf), 0);
         if (len == -1) {
            psock_errno("recv()");
            exit(1);
         }

         if (len == 0) {
            printf("The client broke the connection\n");
            soclose(newsock);
            break;
         }

         printf("Received %d bytes\n", len);

//------ Echo it back

         rc = send(newsock, buf, len, 0);
         if (rc == -1) {
            psock_errno("send()");
            exit(1);
         }
      }

   }

}
