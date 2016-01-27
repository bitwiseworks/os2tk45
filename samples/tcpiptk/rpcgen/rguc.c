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
/* RGUC.C */

/* user written */
/* client program */

#include <stdio.h>
#include <rpc/rpc.h>
#include "rg.h"
#include <sys\socket.h>
#include <netdb.h>

main(argc, argv)
   int argc;
   char *argv[];
{
   int *result;
   char *message;
   CLIENT *cl;
   int socket = -1;
   struct sockaddr_in addr;
   struct hostent *hp;     /* Pointer to host info */


   if (argc < 3)
    {
      fprintf(stderr, " Usage: rgc hostname user_input_string\n");
      exit(-1);
    }

   message = argv[2];

   hp = gethostbyname(argv[1]);
   if (hp) {
           addr.sin_family = hp->h_addrtype;
           bcopy(hp->h_addr, (caddr_t)&(addr.sin_addr), hp->h_length);
           addr.sin_port = (unsigned short) 0;
   } else {
           printf("unknown host\n");
           exit(1);
   }

   cl = clnttcp_create(&addr, MESSAGEPROG, MESSAGEVERS, &socket, 50, 500);
   if (cl==NULL) exit(1);
   result = printmessage_1(&message,cl);
}
