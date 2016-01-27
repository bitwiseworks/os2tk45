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
#define INCL_32

#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <sys\socket.h>
#include <netinet\in.h>
#include <arpa\inet.h>
#include <netinet\tcp.h>
#include <fcntl.h>


int getfile ();


#define FILENAMESIZE    128

char    serveraddress[FILENAMESIZE];
char    clientaddress[FILENAMESIZE];
char    filename[256];
int     myport ; /* port to fecv init info on */
int  rc;


struct _FileHeader
{
  unsigned long Offset;
  unsigned long Size;
  char  FileName[256];
};


struct fileattrib {
                    char filename[125];
                    long filesize;
                  };

struct fileattrib fa;
 char *temp;
 char message[1000];
 int s1, s2;
FILE *fd;

void main( int argc, char *argv[])
{
 struct sockaddr_in clientname;
 int                clientnamelength;
 struct in_addr     clientaddr;
 struct sockaddr_in server;
   if(argc<3)
    {
     printf(" usage : <ip_addr> <port>");
     exit(0);
    }

   strcpy (serveraddress,argv[1]);
   myport = atoi(argv[2]);

          printf ("Waiting for TestInit message from client\n");

          rc = getfile();

          if(rc)
               printf("file received\n");
          else
               printf(" error");

}


int getfile ()
{
    int rc,i;
    struct sockaddr_in clientname;
    int                clientnamelength;
    struct in_addr     clientaddr;
    struct sockaddr_in servername;
    char *filename;



    s1 = socket (AF_INET, SOCK_STREAM, 0);

    if (s1 != -1)
    {
        printf ("s_send:socket %d opened\n", s1);

      memset (&servername, 0, sizeof(servername));
      servername.sin_len         = sizeof(servername);
      servername.sin_family      = AF_INET;
      servername.sin_addr.s_addr = inet_addr(serveraddress);
      servername.sin_port        = myport;


      /*
       * bind out address.
       */

      rc = bind (s1,
                 (struct sockaddr *) &servername,
                 sizeof (servername));

      clientnamelength = sizeof(clientname);

      if (!rc)
      {

        printf ("s_send:server bound to port %d on %s\n",
                servername.sin_port,
                serveraddress);

        /*
         * set socket to listen for connection requests.
         */
        rc = listen (s1, 5);

      if (!rc)
        {
          s2 = accept (s1,
                       (struct sockaddr *)&clientname,
                       &clientnamelength);


          if (rc != -1)
          {

            printf ("s_send:connection accepted on socket %d from %d:%s\n",
                    s2,
                    clientnamelength,
                    inet_ntoa(clientname.sin_addr));

            rc = recv(s2,message,sizeof(message),MSG_WAITALL);
                printf("\n*************** no of bytes recvd =%d \n",rc);

              puts(message);

            fclose(fd);
            return(rc);
          }
          else
          {
              printf ("s_send:accept() failed rc = %d sock_errno = %d \n", rc, sock_errno());
          }
        }
        else
        {
            printf ("s_send:listen() failed rc = %d sock_errno = %d \n", rc, sock_errno());
        }
      }
      else
      {
          printf ("s_send:bind() failed rc = %d sock_errno = %d \n", rc, sock_errno());
      }
    }
    else
    {
        printf ("s_send:socket() failed rc = %d\n", sock_errno());
    }

    return(rc);
}
