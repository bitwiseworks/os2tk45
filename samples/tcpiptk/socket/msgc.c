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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>

main(int argc, char *argv[])
{


   int s,bytesend;
   unsigned short port;
   struct sockaddr_in server;
   struct msghdr msg;
   struct iovec iov[5];
   char buf[64],buf1[64],buf2[64],buf3[64],buf4[64];

   /* argv[1] is internet address of server argv[2] is port of server.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
   if(argc != 3)
   {
      printf("Usage: %s <host address> <port> \n",argv[0]);
      exit(1);
   }
   port = htons(atoi(argv[2]));

   /* Initialize with sockets */
   sock_init();

   /* Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
   {
       psock_errno("socket()");
       exit(1);
   }

   /* Set up the server name */
   server.sin_family      = AF_INET;            /* Internet Domain    */
   server.sin_port        = port;               /* Server Port        */
   server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */

   strcpy(buf, "Hello, I am Gaurang Shah");
   strcpy(buf1, "Hello *1*, I am William R. SNOW");
   strcpy(buf2, "Hello *2*, I am James Willam");
   strcpy(buf3, "Hello *3*, I am GLENN STUMP");
   strcpy(buf4, "Hello *4*, I am paul seifert");

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

   msg.msg_name = (caddr_t) &server;
   msg.msg_namelen = sizeof(server);

   msg.msg_iov = &iov[0];
   msg.msg_iovlen = 5;

   /* Send the message in buf to the server */

   if ((bytesend=sendmsg(s, &msg, 0)) < 0)
    {
        psock_errno("sendmsg()");
        exit(2);
    }

   /* Deallocate the socket */
   soclose(s);
}
