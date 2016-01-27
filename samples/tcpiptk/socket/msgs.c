/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1990, 1996.                             */
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
/*   Modification History:                                                  */
/*   Date:     By:   Tag:  Desc:                                            */
/*   09.30.96  DRC         removed unreferenced variables                   */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/uio.h>

main(void)
{
   int byterecv,sockint,s, namelen;
   struct sockaddr_in client, server;
   struct msghdr msg;
   struct iovec iov[5];
   char buf[64],buf1[64],buf2[64],buf3[64],buf4[64];

   /*
    * Initialize with sockets.
    */
   if ((sockint = sock_init()) != 0)
   {
      printf(" INET.SYS probably is not running");
      exit(1);
   }

   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
   {
       psock_errno("socket()");
       exit(1);
   }

   /*
    *
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network interface)
    */
   server.sin_family      = AF_INET;   /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port       */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address    */

   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       psock_errno("bind()");
       exit(2);
   }

   /* Find out what port was really assigned and print it */
   namelen = sizeof(server);
   if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   {
       psock_errno("getsockname()");
       exit(3);
   }

   printf("Port assigned is %d\n", ntohs(server.sin_port));


   iov[0].iov_base = &buf[0];
   iov[0].iov_len = sizeof(buf);
   iov[1].iov_base = &buf1[0];
   iov[1].iov_len = sizeof(buf1);
   iov[2].iov_base = &buf2[0];
   iov[2].iov_len = sizeof(buf2);
   iov[3].iov_base = &buf3[0];
   iov[3].iov_len = sizeof(buf3);
   iov[4].iov_base = &buf4[0];
   iov[4].iov_len = sizeof(buf4);

   msg.msg_name = (caddr_t)&client;
   msg.msg_namelen = sizeof(client);

   msg.msg_iov = &iov[0];
   msg.msg_iovlen = 5;

   if((byterecv=recvmsg(s, &msg, 0))<0)
   {
       psock_errno("recvmsg()");
       exit(4);
   }
   /*
    * Print the message received.
    */
   printf("%s \n", buf);
   printf("%s \n", buf1);
   printf("%s \n", buf2);
   printf("%s \n", buf3);
   printf("%s \n", buf4);

   /*
    * Deallocate the socket.
    */
   soclose(s);
}

