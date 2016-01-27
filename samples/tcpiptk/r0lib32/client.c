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
/************************** CLIENT.C ****************************************/
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys\types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <sys\ioctl.h>

#include <errno.h>
#include <utils.h>

# define BUFF_SIZE      1024

void mysendmsg();
void myrecvmsg();

struct sockaddr_in serv_addr ;
int sockfd;


main(int argc, char *argv[])
{
        char Sendbuff[BUFF_SIZE];
        char Recvbuff[BUFF_SIZE];
        int count,port;

        if (argc != 2)
        {
                fprintf(stderr, "Usage: %s ip address \n", argv[0]);
                exit(1);
        }
        for(count= 0;count < BUFF_SIZE;count++)
                Sendbuff[count] = 'a';

        if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        {
                printf("server : can't open stream socket");
                exit(0);
        }

        memset((char *) &serv_addr,'\0', sizeof(serv_addr));

        serv_addr.sin_family      = AF_INET ;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port        = htons(5000);

        if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        {
                psock_errno("connect()");
                exit(1);
        }

        if( (send(sockfd,Sendbuff,BUFF_SIZE,0)  < 0 )  )
        {
                psock_errno("send()");
                exit(1);
        }

        if( recv(sockfd,Recvbuff,BUFF_SIZE,0) < 0 )
        {
                psock_errno("recv()");
                exit(1);
        }
        for(count= 0;count < BUFF_SIZE;count++)
                printf("%c",Recvbuff[count]);

        mysendmsg();
        myrecvmsg();

}

void myrecvmsg()
{
        struct msghdr msg;
        struct iovec iov;
        char buff[50];
        int i;

        iov.iov_base = &buff[0];
        iov.iov_len = sizeof(buff) -1;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;

        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        if( recvmsg(sockfd,(struct msghdr *)&msg,0) < 0)
        {
                printf("\n recv msg error \n");
                psock_errno("recvmsg()");
                exit(1);
        }
        printf("\n data received by recvmsg \n");
        printf("%s",buff);

}
void mysendmsg()
{
        struct msghdr msg;
        struct iovec iov;
        char buff[50] = "SHIVA";

        iov.iov_base = buff;
        iov.iov_len = sizeof(buff) -1;

        /******* if connect is used *********/
        msg.msg_name = NULL;
        msg.msg_namelen = 0;

        /****************************************
         if connect is not used
        msg.msg_name = (caddr_t)&server;
        msg.msg_namelen = sizeof(server);
        *****************************************/

        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        if( sendmsg(sockfd,&msg,0) < 0)
        {
                printf("\n sendmsg error \n");
                exit(1);
        }

}
