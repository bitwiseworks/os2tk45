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
/************************* TCP CLIENT****************************************/

#define MSG_CLOSE 0x800
#define O_RDONLY  0x4

#include <string.h>
#include <stdio.h>
#include <types.h>
#include <sys\socket.h>
#include <netinet\in.h>
#include <netinet\tcp.h>
#include <arpa\inet.h>
#include <io.h>

char  serveraddress[128],filename[256];
int   serverport ;
int   fd,rc,s;

struct sf_parms sfp;

int putfile (void);

int main (int argc, char *argv[])
{
 if(argc<4)
   {
     printf(" usage %s: <ip_addr> <port> <filename> \n",argv[0]);
     exit(0);
   }
 strcpy (serveraddress, argv[1]);   /* argv[1] is server address to which file is to be sent */
 serverport = atoi(argv[2]);        /* argv[2] is server port */
 strcpy (filename, argv[3]);        /* argv[3] is name of the file to be sent */

 printf ("Sending File to server\n");
   if ((rc = putfile()) != 0)
        {
          printf ("Putfile() failed rc = %d sock_errno = %d \n", rc, sock_errno());
            return(rc);
        }
}

int putfile()
{
    struct sockaddr_in servername;

    if( (s = socket (PF_INET, SOCK_STREAM, 0)) != -1 )
    {
      servername.sin_len         = sizeof(servername);
      servername.sin_family      = AF_INET;
      servername.sin_addr.s_addr = inet_addr(serveraddress);
      servername.sin_port        = serverport;

    if((rc = connect(s,(struct sockaddr *)&servername,sizeof(servername))) != -1)
       {
            fd =open(filename,O_RDONLY,0);
            sfp.header_data   = 0;
            sfp.header_length = 0;
            sfp.file_handle   = fd;
            sfp.file_size     = -1;
            sfp.file_offset   = 0;
            sfp.file_bytes    = -1;
            sfp.trailer_data  = 0;
            sfp.trailer_length= 0;
            sfp.bytes_sent    = 0;

          if(( rc = send_file(&s,&sfp,MSG_CLOSE)) != 0)
                printf( " ******  FILE NOT SENT  ****** ");
            close(fd);
       }
      else
        printf ("send_file :connect() failed sock_errno = %d \n",sock_errno());
    }
    else
      printf ("send_file :socket() failed rc = %d\n", sock_errno());
   return(rc);
}
