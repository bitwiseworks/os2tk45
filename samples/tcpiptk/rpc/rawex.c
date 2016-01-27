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
/*RAWEX*/
/* AN EXAMPLE OF THE RAW CLIENT/SERVER USAGE */
/* PORTMAPPER MUST BE RUNNING */


#include <rpc/rpctypes.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <sys\socket.h>
#include <string.h>
#include <netdb.h>


#define rawprog ((u_long)150104)
#define rawvers ((u_long)1)
#define rawproc ((u_long)1)

extern enum clnt_stat clntraw_call();
extern void raw2();

main(argc,argv)
int argc;
char *argv[];
{
   SVCXPRT *transp;
   struct hostent *hp;
   struct timeval pertry_timeout, total_timeout;
   struct sockaddr_in server_addr;
   int bout,in;
   register CLIENT *clnt;
   enum clnt_stat cs;
   int addrlen, sock;

   if(argc!=2) {
     printf("usage:  RAWEX integer\n");
     exit(-1);
   }
   in = atoi(argv[1]);
   sock = RPC_ANYSOCK;
   transp = svcraw_create();
   if (transp == NULL) {
      fprintf(stderr, "can't create an RPC server transport\n");
      exit(-1);
   }
   pmap_unset(rawprog, rawvers);
   if (!svc_register(transp,rawprog,rawvers,raw2, 0)) {
      fprintf(stderr, "can't register service\n");
      exit(-1);
   }

   xprt_register(transp);

   if ((clnt = clntraw_create(rawprog, rawvers)) == NULL ) {
      clnt_pcreateerror("clntudp_create");
      exit(-1);
   }
   total_timeout.tv_sec = 60;
   total_timeout.tv_usec = 0;
   printf("Argument:  %d\n",in);
   cs=clnt_call(clnt,rawproc,xdr_int,
                (char *)&in,xdr_int,(char *)&bout,total_timeout);
   printf("Result:  %d",bout);
   if(cs!=0) {
      clnt_perror(clnt,"Client call failed");
      exit(1);
   }
   exit(0);
}

void raw2(rqstp,transp)
   struct svc_req *rqstp;
   SVCXPRT *transp;
{
   int in,out;
   if (rqstp->rq_proc=rawproc) {
   svc_getargs(transp,xdr_int,&in);
   printf("Received:  %d\n",in);
   out=in;
   printf("Sent:  %d\n",out);
   if (!svc_sendreply(transp, xdr_int,(char * )&out)) {
      printf("Can't reply to RPC call.\n");
      exit(1);
   }
   }
}
