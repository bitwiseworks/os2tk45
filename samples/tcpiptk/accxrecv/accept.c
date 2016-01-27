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
/************************** ACCEPT.C **************************************/
/*   TCP Server   */

/* Include Files. */
#define INCL_32
#define INCL_DOS
#include <os2.h>
#include <stdio.h>
#include <types.h>
#include <sys\socket.h>
#include <netinet\in.h>
#include <utils.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys\time.h>

void acceptex_thread();


int sockfd;
int total_threads = 0;

int main(int argc, char *argv[])
{
   int port;
   int accexcnt=0, count;
   int tid=0;
   int rv;
   struct sockaddr_in serv_addr;
   char q;
   struct timeval tv;


if( argc < 3) {
   printf("Usage: %s [server port] [number of acceptex threads]\n", argv[0]);
   return -1;
}

   port = (unsigned int) atoi(argv[1]);
   accexcnt = atoi(argv[2]);

   if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
   {
     printf("server : can't open stream socket");
     exit(0);
   }

   printf("Server: socket opened %d\n",sockfd);
   memset((char *) &serv_addr,0, sizeof(serv_addr));
   serv_addr.sin_family      = AF_INET ;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port        = htons(port);


  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  {
       printf("server : can't bind local address");
       exit (-1);
  }
  printf("Server: socket bound %d\n",sockfd);

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  rv = setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));
  if( rv < 0) psock_errno( " Set SO_RCVTIMEO");


  listen(sockfd, 5);
  printf("now listening\n");

  printf("Creating threads\n");
  printf("ENTER THE VARIABLE Q \n");
  scanf("%c",&q);

  for( count =0; count < accexcnt; count++)
  {
    if( (tid = _beginthread(acceptex_thread, NULL, 8192, NULL)) < 0)
    {
       printf("error\n");
       exit(1);
    }
    printf("Acceptex Thread Id is: %x\n",tid);
  }


  while( total_threads < accexcnt )
     DosSleep( 1 );

    soclose(sockfd);
   return 1;

}

void acceptex_thread()
{
      long sock_accex, recvd = 0;
      long locallen,clilen;
      char *Outbuff;
      int alloc = 1024;
      struct sockaddr_in local;
      struct sockaddr_in cli_addr;
      locallen = sizeof(local);
      clilen = sizeof(cli_addr);

      Outbuff = (char*) malloc( alloc );
      if (Outbuff == NULL) {
        printf("malloc error");
        total_threads++;
        exit(0);
      }
Begin:
      sock_accex = -1;
      recvd = accept_and_recv(sockfd,&sock_accex,(struct sockaddr *)&cli_addr, &clilen,(struct sockaddr *)&local,&locallen,Outbuff,alloc);
      if(recvd < 0)
        {
           printf("error in acceptex recv= %d\n",sock_errno());
           total_threads++;
           goto Begin;
           locallen = sizeof(local);
           clilen = sizeof(cli_addr);
           alloc =1024;
           recvd = accept_and_recv(sockfd,&sock_accex,(struct sockaddr *)&cli_addr, &clilen,(struct sockaddr *)&local,&locallen,Outbuff,alloc);
              if(recvd < 0)
                {
                   printf("error in acceptex recv= %d\n",sock_errno());
        }
        else
        {
              locallen = sizeof(local);
              clilen = sizeof(cli_addr);
              alloc =1024;
              recvd = accept_and_recv(sockfd,&sock_accex,(struct sockaddr *)&cli_addr, &clilen,(struct sockaddr *)&local,&locallen,Outbuff,alloc);
              if(recvd < 0)
                   printf("error in acceptex recv= %d\n",sock_errno());
        }
}

       printf(" Local addr :%x, Local_addr_len :%d\n",local.sin_addr.s_addr,locallen);
       printf(" Bytes recieved with acceptex :%d\n",recvd);
       printf(" Msg recvd with acceptex :%s\n",Outbuff);
       printf(" Recv socket Id :%d\n",sock_accex);
       printf("Before soclose\n");
       soclose(sock_accex);
       printf("after soclose\n");
       total_threads++;
       return ;
}

