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
/*--------------------------------------------------------------------------*/

/******************************************************************************
*
* An alternative version of the ECHO client
* -----------------------------------------
*
* Written by Andre Asselin
*
* For use with "Introduction to sockets programming with IBM TCP/IP for OS/2"
*
*
* Description
* -----------
*
* This program implements the client side of the Echo protocol (RFC 862).  The
* Echo protocol is very simple: whatever the client sends to the server, the
* server will send right back to the client.  The complete RFC is reproduced
* below:
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

*
******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa\inet.h>

void main(int argc, char **argv) {
   struct sockaddr_in server;
   struct hostent *hp;
   char buf[100];
   struct servent *echoprot;
   int sock, len, rc;

   // Initialize TCP/IP

   if (sock_init() != 0) {
      printf("INET.SYS probably is not running");
      exit(1);
   }

   // Check to make sure the user passed the right number of arguments

   if (argc != 3  ||  argc > 1 && argv[1][0] == '?') {
      printf("Usage:\n"
             "ÄÄechoclntÄÄÂÄ TCP ÄÂÄ<host>ÄÄ\n"
             "              ÀÄ UDP ÄÙ\n"
             "\n"
             "TCP    Specifies to use TCP as the connection protocol.\n"
             "UDP    Specifies to use UDP as the connection protocol.\n"
             "<host> Specifies the remote host on which the command is to be issued.\n");
      exit(1);
   }

   // Open the correct type of socket

   if (stricmp(argv[1], "TCP") == 0) {
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == -1) {
         psock_errno("socket()");
         exit(1);
      }
   } else if (stricmp(argv[1], "UDP") == 0) {
      sock = socket(AF_INET, SOCK_DGRAM, 0);
      if (sock == -1) {
         psock_errno("socket()");
         exit(1);
      }
   } else {
      printf("You must specify either 'tcp' or 'udp' as the connection protocol.\n");
      exit(1);
   }

   // Set the server address from argument 2 -- we accept both domain names
   // and IP addresses

   server.sin_addr.s_addr = inet_addr(argv[2]);
   if (server.sin_addr.s_addr == -1) {
      // It wasn't a dotted decimal address -- assume it's a domain name,
      // and use gethostbyname() to convert it

      hp = gethostbyname(argv[2]);
      if (hp) {
         server.sin_family = hp->h_addrtype;
         memcpy(&server.sin_addr, hp->h_addr, sizeof(server.sin_addr));
      } else {
         printf("Unknown host %s\n", argv[2]);
         exit(1);
      }
   }

   // Get the port for the echo protocol out of the etc\services file

   echoprot = getservbyname("echo", argv[1]);
   if (echoprot == NULL) {
      printf("The echo/%s protocol is not listed in the etc/services file\n", argv[1]);
      exit(1);
   }

   // Set the rest of the server address

   server.sin_family = AF_INET;
   server.sin_port = echoprot->s_port;

   // Connect to the server

   rc = connect(sock, (struct sockaddr *)&server, sizeof(server));
   if (rc == -1) {
      psock_errno("connect()");
      exit(1);
   }

   while (1) {
      if (gets(buf) == NULL) {
         printf("gets() returned NULL\n");
         exit(1);
      }

      rc = send(sock, buf, strlen(buf), 0);
      if (rc == -1) {
         psock_errno("send()");
         exit(1);
      }

      len = recv(sock, buf, sizeof(buf), 0);
      if (len == -1) {
         psock_errno("recv()");
         exit(1);
      }

      buf[len] = '\0';
      printf("%s\n", buf);
   }
}
