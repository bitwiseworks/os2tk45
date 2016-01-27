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
* Single threaded ECHO server
* ---------------------------
*
* Written by Andre Asselin
*
* Description
* -----------
*
* This program implements the server side of the Echo protocol (RFC 862).  The
* Echo protocol is very simple: whatever the client sends to the server, the
* server will send right back to the client.
*
* This version of the server services UDP echo requests, TCP echo requests, and
* requests for new TCP connections all from one thread using select().
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

*************************************************************************/


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

#define MAX_CONNS    2

void ServiceUDP(int sock);
int ServiceTCP(int sock);


void main(int argc, char **argv) {
   int listen_sock, udp_sock, newsock;
   int connsocks[MAX_CONNS], selsocks[MAX_CONNS + 2];
   struct servent *echoprot;
   struct sockaddr_in server, client;
   int rc, numconns;
   int asize, i;

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

   // No connections yet

   numconns = 0;

//------ Wait for data to echo or a connection on the TCP socket

   while (1) {

      // Setup the socket array for select()

      selsocks[0] = listen_sock;
      selsocks[1] = udp_sock;
      for (i = 0; i < numconns; i++)
         selsocks[i+2] = connsocks[i];

      // Wait indefinitely

      rc = os2_select(selsocks, numconns + 2, 0, 0, -1);
      if (rc == -1) {
         // If the select was interrupted, just continue
         if (sock_errno() == SOCEINTR)
            continue;
         psock_errno("select()");
         exit(1);
      }

      // Check the TCP sockets to see if we got any data on them

      for (i = 0; i < numconns; i++) {
         if (selsocks[i+2] != -1) {
            // We got some data - handle it

            rc = ServiceTCP(selsocks[i+2]);
            if (rc == -1) {

               // The connection was closed - move the last connected socket
               // into the position this socket occupies

               connsocks[i] = connsocks[numconns-1];
               numconns--;
            }
         }
      }

      // If this was a request for a new TCP connection, accept it

      if (selsocks[0] != -1) {

         // accept() the new connection

         asize = sizeof(client);
         newsock = accept(selsocks[0], (struct sockaddr *)&client, &asize);
         if (newsock == -1) {
            psock_errno("accept()");
            exit(1);
         }

         printf("Received a TCP connection on socket %d from %s port %d\n",
                newsock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

         // If there are more concurrent connections than we can handle, just
         // close the socket -- doing the accept() and then a soclose() takes
         // the connection request off our queue, and keeps the client from
         // hanging around forever trying to connect.

         if (numconns >= MAX_CONNS)
            soclose(newsock);
         else
            connsocks[numconns++] = newsock;
      }

      // If we received a UDP packet, handle it

      if (selsocks[1] != -1)
         ServiceUDP(selsocks[1]);
   }
}


// The UDP socket has a packet waiting to be received-- receive it and echo
// it back to the sender

void ServiceUDP(int sock) {
   static char buf[32767];
   int rc;
   int asize, len;
   struct sockaddr_in client;

   // Receive the datagram

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


// A client sent us some data - echo it back to him

int ServiceTCP(int sock) {
   int rc, len;
   int socks[1];
   char buf[1024];

   // Find out how much data he sent us

   rc = ioctl(sock, FIONREAD, (char *)&len, sizeof(len));
   if (rc == -1) {
      psock_errno("ioctl()");
      exit(1);
   }

   // Read the data

   rc = recv(sock, buf, len, 0);
   if (rc == -1) {
      psock_errno("recv()");
      exit(1);
   }

   if (rc == 0) {
      printf("TCP socket %d: The client broke the connection\n", sock);
      soclose(sock);
      return -1;
   }

   printf("TCP socket %d: received %d bytes\n", sock, len);

   // Echo the data back

   rc = send(sock, buf, len, 0);
   if (rc == -1) {
      psock_errno("send()");
      exit(1);
   }

   return 0;
}
