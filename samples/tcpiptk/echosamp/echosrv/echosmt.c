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
/*--------------------------------------------------------------------------*/

/*****************************************************************************
*
* Multithreaded ECHO server
* -------------------------
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
* This version of the server will start a new thread for each TCP connection
* it accepts.  The main thread is responsible for handling UDP echo requests
* and for doing the accept() for each new TCP connection and spinning off a
* thread to service it.
*
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>

#include <types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys\time.h>
#include <unistd.h>
#include <arpa\inet.h>
#include <nerrno.h>

#define STACK_SIZE      65536

void ServiceUDP(int sock);
void ServiceTCP(void *arg);


void main(int argc, char **argv) {
   int listen_sock, udp_sock, socks[2], newsock;
   struct servent *echoprot;
   struct sockaddr_in server, client;
   int rc;
   int asize;

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

   // Get the port for the echo protocol out of the etc\services file
   echoprot = getservbyname("echo", "tcp");
   if (echoprot == NULL) {
      printf("The echo/tcp protocol is not listed in the etc/services file\n");
      exit(1);
   }

   // Bind the TCP socket to the echo port
   server.sin_family = AF_INET;
   server.sin_port = echoprot->s_port;
   server.sin_addr.s_addr = INADDR_ANY;

   rc = bind(listen_sock, (struct sockaddr *)&server, sizeof(server));
   if (rc == -1) {
       psock_errno("tcp bind()");
       exit(1);
   }

   // Put the socket into listen mode

   rc = listen(listen_sock, SOMAXCONN);
   if (rc == -1) {
       psock_errno("listen()");
       exit(1);
   }

//------ Create the UDP server socket

   // Create a UDP socket to accept incoming data
   udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (udp_sock == -1) {
       psock_errno("udp socket()");
       exit(1);
   }

   printf("UDP socket assigned is %d\n", udp_sock);

   // Get the port for the echo protocol out of the etc\services file
   echoprot = getservbyname("echo", "udp");
   if (echoprot == NULL) {
      printf("The echo/udp protocol is not listed in the etc/services file\n");
      exit(1);
   }

   // Bind the UDP socket to the echo port
   server.sin_family = AF_INET;
   server.sin_port = echoprot->s_port;
   server.sin_addr.s_addr = INADDR_ANY;

   rc = bind(udp_sock, (struct sockaddr *)&server, sizeof(server));
   if (rc == -1) {
       psock_errno("udp bind()");
       exit(1);
   }

//------ Wait for a connection on the TCP socket or a packet on the UDP socket

   while (1) {
      // Setup the socket array for select()
      socks[0] = listen_sock;
      socks[1] = udp_sock;

      // Wait indefinitely
      rc = os2_select(socks, 2, 0, 0, -1);
      if (rc == -1) {
         // If the select was interrupted, just continue
         if (sock_errno() == SOCEINTR)
            continue;
         psock_errno("main() select()");
         exit(1);
      }

      if (socks[0] != -1) {
         // The TCP listen socket has a connection request ready-- accept() it

         asize = sizeof(client);
         newsock = accept(socks[0], (struct sockaddr *)&client, &asize);
         if (newsock == -1) {
            psock_errno("accept()");
            exit(1);
         }

         printf("Received a TCP connection on socket %d from %s port %d\n",
                newsock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

         // Start a new thread and pass it the socket it should service

         rc = _beginthread(ServiceTCP, 0, STACK_SIZE, (void *)newsock);
         if (rc == -1) {
            printf("_beginthread() failed\n");
            exit(1);
         }
      }

      // If the UDP socket has a packet waiting, service it

      if (socks[1] != -1)
         ServiceUDP(socks[1]);
   }
}


// The UDP socket has a packet waiting to be received-- receive it and echo
// it back to the sender

void ServiceUDP(int sock) {
   static char buf[32767];
   int rc;
   int asize, len;
   struct sockaddr_in client;

   // Receive the packet

   asize = sizeof(client);
   len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client,
                  &asize);
   if (len == -1) {
      psock_errno("recvfrom()");
      exit(1);
   }

   printf("Received a UDP echo request from %s port %d\n",
          inet_ntoa(client.sin_addr), ntohs(client.sin_port));

   // Echo it back

   rc = sendto(sock, buf, len, 0, (struct sockaddr *)&client, sizeof(client));
   if (rc == -1 || rc != len) {
      psock_errno("sendto()");
      exit(1);
   }
}


// A client initiated a TCP connection.  Echo any data he sends us back to him.

void ServiceTCP(void *arg) {
   int rc, len;
   char buf[1024];
   int sock;

   // Convert the parameter to something we can use

   sock = (int)arg;

   // Do this until the client closes the connection

   while (1) {

      // Wait until there's some data available

      len = recv(sock, buf, sizeof(buf), 0);
      if (len == -1) {
         psock_errno("recv()");
         exit(1);
      }

      if (len == 0) {
         printf("TCP socket %d: The client broke the connection\n", sock);
         break;
      }

      printf("TCP socket %d: received %d bytes\n", sock, len);

      // Send the data back

      rc = send(sock, buf, len, 0);
      if (rc == -1) {
         psock_errno("send()");
         exit(1);
      }
   }
}
