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
/******************************HPS SERVER*************************************/
/*                                                                           */
/*                                                                           */
/*- BUFCNT : Number of 60KByte HPS chunks that you want from the kernel.     */
/*           Defined in cliserv.h. (Note that there is a MAX limit of 64)    */
/*- The VerifyData can be turned ON which will clear all the buffers before  */
/*           each send, and will also validate the incoming data for a known */
/*           pattern. This is time intensive but can be done to check the    */
/*           possible buffer overlap.                                        */
/*- The VerifyMem can be used to optionally check to see if after the        */
/*           semaphore wait if the kernel mem ptrs are really released.      */
/*           None of the ptrs should be NULL at this time. This checking     */
/*           is not a must but can be optionally turned On for ensuring the  */
/*           functionality.                                                  */
/*- Use the -s switch is you explicitly wonna use the semaphores             */
/*- If compiled with DEBUG it will display all the kernel ptrs.              */
/*- I am following a stupid way of waiting for the release of EACH and EVERY */
/*  buffer before proceeding further, just in order to save complex coding in*/
/*  QUERY_MEMMAPIO. This can be written much better for optimised performance*/
/*  But for this test excercise performance is not considered an issue . amol*/
/*****************************************************************************/
#define INCL_DOSSEMAPHORES
#define INCL_DOSERRORS

#include "cliserv.h"

int read_stream(int fd, char *ptr, int maxbytes);

int main(int argc, char *argv[]) {
   struct sockaddr_in   serv, cli;
   u_short port = TCP_SERV_PORT;
   int i, j, k, sockfd,listenfd, clilen, n, cnt, nread, nsent, NumOfBufs,
        VerifyData=0, VerifyMem=0, UseSems=0, wprinted=0;
   int send_data = 0,recv_data = 0;            /*  aarti */

   int mib[4];
   unsigned int needed;
   APIRET rc;
   ULONG dontcare;
   int len = NUMHPSBLOCKS * HPSBLOCKSIZE * BUFCNT;
   int num = ITERATIONS;
   HEV arrayofsem[NUMHPSBLOCKS*BUFCNT];
   long arrayofptrs[NUMHPSBLOCKS*BUFCNT];
   struct msghdr msg;
   struct iovec iov[NUMHPSBLOCKS*BUFCNT];
   char *name;
   HMUX hmux;
   SEMRECORD semrec[NUMHPSBLOCKS*BUFCNT];
   char *cp;

   if (argc < 2) {
      printf("Usage: echos -p#### -n####  -m -d -s\n");
      printf("       p=port, n=number_of_iterations, m=Mem Verification, d=Data validation, s=Use Semaphores\n");
      return(0);
   }

   for (n = 1; n < argc; n++) {
      if (argv[n][0] == '-') {
         if (argv[n][1] == 'p') {
            port = atoi(&argv[n][2]);
         } else if(argv[n][1] == 'n') {
            num = atoi(&argv[n][2]);
         } else if(argv[n][1] == 'm') {
            VerifyMem=TRUE;
         } else if(argv[n][1] == 'd') {
            VerifyData=TRUE;
         } else if(argv[n][1] == 's') {
            UseSems=TRUE;
         } else {
            printf("Usage: echoc -p#### -n####  -m -d -s\n");
            printf("       p=port, n=number_of_iterations, m=Mem Verification, d=Data validation, s=Use Semaphores\n");
            return(0);
         }
      } else {
         name = argv[n];
      }
   }

   printf("HPS Server :(%s)port=%d, Iterations=%d\n", argv[0], port, num);

   cp = malloc(len);
   if (!cp) {
     printf("malloc failed for %d bytes\n",len);
     exit(1);
   }

   if (UseSems) {
      printf("   HPS Server Using Semaphores\n");
      /*
       * Try to get the semaphore handles, and initialize the array of ptrs
       */
      for (i = 0; i < NUMHPSBLOCKS*BUFCNT ; i++) {
        rc = DosCreateEventSem(NULL, &arrayofsem[i], DC_SEM_SHARED, FALSE);
        if (rc != NO_ERROR) {
           printf("DosCreateEventSem=%d\n", rc);
           exit(1);
        }
        arrayofptrs[i]    = arrayofsem[i];
        semrec[i].hsemCur = (HSEM)arrayofsem[i];
        semrec[i].ulUser  = i;
      }

      /* create the hmux sempahore */
      rc = DosCreateMuxWaitSem(NULL, &hmux, NUMHPSBLOCKS*BUFCNT, semrec, DCMW_WAIT_ALL);
      if (rc != NO_ERROR) {
         printf("DosCreateMuxWaitSem=%d\n", rc);
         exit(1);
      }
   } /* UseSems */
   else {
     for (i = 0; i < NUMHPSBLOCKS*BUFCNT ; i++)
       arrayofptrs[i] = 0;
     printf("   HPS Client Without Semaphores\n");
   }

   for (NumOfBufs=0; NumOfBufs < BUFCNT; NumOfBufs++) {
     mib[0] = CTL_OS2;
     mib[1] = AF_INET;
     mib[2] = 0;
     mib[3] = OS2_MEMMAPIO;
     needed = 15*4;    // 60K

     /* buffers return in old, to be free in new */
     if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), (arrayofptrs+(15*NumOfBufs)), &needed, NULL, 0) < 0) {
        printf("sysctl(MEMMAPIO)=%d, errno=%d\n", rc, sock_errno());
        exit(1);
     }
   } /* for */

   if (VerifyMem) {
    for (NumOfBufs=0; NumOfBufs < BUFCNT; NumOfBufs++) {
      /* check to make sure we hav not gotten a NULL ptr from stack */
      mib[0] = CTL_OS2;
      mib[1] = AF_INET;
      mib[2] = 0;
      mib[3] = OS2_QUERY_MEMMAPIO;
      needed = 15*4;   // 60K

      if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), (arrayofptrs+(15*NumOfBufs)), &needed, NULL, 0) < 0) {
         printf("sysctl(MEMMAPIO)=%d, errno=%d\n", rc, sock_errno());
         exit(1);
      }
    } /* for */

    for (i = 0; i < NUMHPSBLOCKS*BUFCNT; i++) {
#ifdef DEBUG
      printf("HPS memory ptr %d:\t%p\n", i, arrayofptrs[i]);
#endif
      if (arrayofptrs[i] == 0) {
         printf("Opsss. Memory allocated  by stack has a NULL ptr !\n");
         exit(1);
      }
    }
   } /* VerifyMem */

   listenfd = socket(PF_INET, SOCK_STREAM, 0);
   if (listenfd < 0) {
      printf("socket=%d (%d)\n", sockfd, sock_errno());
      exit(1);
   }

   memset(&serv, 0, sizeof(serv));
   serv.sin_len         = sizeof(struct sockaddr_in);
   serv.sin_family      = AF_INET;
   serv.sin_addr.s_addr = INADDR_ANY;
   serv.sin_port        = htons(port);

   cnt = len / HPSBLOCKSIZE;

   msg.msg_name   = NULL;
   msg.msg_namelen= 0;
   msg.msg_iov    = iov;
   msg.msg_iovlen = cnt;

   n = len;
   for (i = 0; i < cnt; i++) {
      iov[i].iov_base = (void *)arrayofptrs[i];
      if (n < HPSBLOCKSIZE) {
         iov[i].iov_len = n;
         n = 0;
         break;
      } else {
         iov[i].iov_len = HPSBLOCKSIZE;
         n -= HPSBLOCKSIZE;
      }
   }

   if (bind(listenfd, (SA) &serv, sizeof(serv)) < 0)
       printf("Bind failed",sock_errno());

   if ( listen(listenfd,SOMAXCONN) < 0)
       printf("Listen failed",sock_errno());

   clilen = sizeof (cli);
   memset(&cli, 0, sizeof(cli));
   if ( (sockfd = accept(listenfd, (SA) &cli, &clilen)) < 0)
        printf("Accept error",sock_errno());

   /* Receive the first lot outside the for loop for optimization */
   if ( (nread = read_stream(sockfd, cp, len)) < 0) {
         printf("read error=%d (%d)", nread, sock_errno());
         exit(1);
   }else
         printf("HPS Server : read %d bytes from \t %s \n",nread,inet_ntoa(cli.sin_addr));

   if (!nread)
         exit(1);

   if (VerifyData) {
     /* Verify the first block of data for correctness outside the loop */
     for (j=0; j< (NUMHPSBLOCKS*BUFCNT); j++)
       for (k=0; k< HPSBLOCKSIZE; k++)
          if (cp[j*HPSBLOCKSIZE+k] != CLI_CHAR + j + recv_data)
          {
                 printf("*** ERROR Found in received data. Got %x Expected (%x) at BlockNumber:%d, Iteration:%d, Index in 4K block:%d\n",cp[j*HPSBLOCKSIZE+k], CLI_CHAR+j+i,j,i,k);
                 exit(1);
          }
     printf("Data verification done\n ");
     /* Clear the cp array */
     memset((void *)(cp),0, HPSBLOCKSIZE * NUMHPSBLOCKS * BUFCNT);

     recv_data++;

   } /* VerifyData */

   for (i=1;;i++) {

      /* Fill in some data */
      for (k = 0; k < (NUMHPSBLOCKS*BUFCNT); k++)
        memset((void *)arrayofptrs[k], SERV_CHAR+k+send_data, HPSBLOCKSIZE);

      nsent = sendmsg(sockfd, &msg, MSG_MAPIO);
      if (nsent != len) {
         printf("sendmsg=%d (%d) len=%d\n", nsent, sock_errno(), len);
         exit(1);
      }else
         printf("HPS Server : Sent %d bytes to \t %s \n", nsent, inet_ntoa(cli.sin_addr));

      if (send_data == 9)                 /* aarti */
         send_data = 0;
      else
         send_data++;

      if ( (nread = read_stream(sockfd, cp, len)) < 0) {
         printf("read error=%d (%d)", nread, sock_errno());
         exit(1);
      }else {
         if (nread)
           printf("HPS Server : read %d bytes from \t %s \n",nread, inet_ntoa(cli.sin_addr));
      }

      if (!nread)
         break;

      if (VerifyData) {
        for (j=0; j< (NUMHPSBLOCKS*BUFCNT); j++)
         for (k=0; k< HPSBLOCKSIZE; k++)
          if (cp[j*HPSBLOCKSIZE+k] != CLI_CHAR + j + recv_data)
          {
                 printf("*** ERROR Found in received data. Got %x Expected (%x) at BlockNumber:%d, Iteration:%d, Index in 4K block:%d\n",cp[j*HPSBLOCKSIZE+k], CLI_CHAR+j+i,j,i,k);
                 exit(1);
          }
        printf("Data verification done\n ");
        /* Clear the cp array */
        memset((void *)(cp),0, HPSBLOCKSIZE * NUMHPSBLOCKS * BUFCNT);

        if (recv_data == 9)                       /* aarti */
            recv_data = 0;
        else
            recv_data++;
      } /* VerifyData */

      /*
       *  WAIT till the buffers are released by HP Send
       */
      wprinted=0;
wait:
      if (UseSems) {
        printf("Waiting on the Mutex Sem \n");
        rc = DosWaitMuxWaitSem(hmux, SEM_INDEFINITE_WAIT, &dontcare);
        if (rc != NO_ERROR) {
           printf("DosWaitMuxWaitSem=%d\n", rc);
           exit(1);
        }

        /* AGAIN create the hmux sempahore */
        rc = DosCreateMuxWaitSem(NULL, &hmux, (NUMHPSBLOCKS*BUFCNT), semrec, DCMW_WAIT_ALL);
        if (rc != NO_ERROR) {
           printf("DosCreateMuxWaitSem=%d\n", rc);
           exit(1);
        }
      } /* if Using Semaphores */
      else
      {
        /* I am following a stupid way of waiting for the release of EACH and EVERY
         * buffer before proceeding further, just in order to save complex coding
         * For this test excercise performance is anyway not an issue . amol
         */
         /* As the sysctl query_memmapio modifies the passed in array, save a copy */
         int CopyCnt;
         long arrayofptrsSave[NUMHPSBLOCKS*BUFCNT];

         for (CopyCnt=0;CopyCnt < (sizeof(arrayofptrs)/sizeof(arrayofptrs[0]));CopyCnt++) {
            arrayofptrsSave[CopyCnt] = arrayofptrs[CopyCnt];
         }

         /* Verify that the memory buffers are indeed released by now */
         mib[0] = CTL_OS2;
         mib[1] = AF_INET;
         mib[2] = 0;
         mib[3] = OS2_QUERY_MEMMAPIO;
         needed = 15*4*BUFCNT;

         if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), arrayofptrsSave, &needed, NULL, 0) < 0) {
            printf("sysctl(MEMMAPIO)=%d, errno=%d\n", rc, sock_errno());
            exit(1);
         }
         for (j=0; j< NUMHPSBLOCKS*BUFCNT; j++) {
           if ( !(arrayofptrsSave[j]) ) {
             if (!wprinted) {
               printf("Waiting for kernel to release each and every buffer :");
               wprinted++;
             }
             printf("-");
             goto wait;
           }
         }
         if(wprinted)
           printf("Buffer Released\n");
      } /* if NOT Using Semaphores */

      /* AGAIN Refill the iov_base with the arrayofptrs */
      n = len;
      for (j = 0; j < cnt; j++) {
         iov[j].iov_base = (void *)arrayofptrs[j];
         if (n < HPSBLOCKSIZE) {
            iov[j].iov_len = n;
            n = 0;
            break;
         } else {
            iov[j].iov_len = HPSBLOCKSIZE;
            n -= HPSBLOCKSIZE;
         }
      }

    if (VerifyMem) {
         int CopyCnt;
         long arrayofptrsSave[NUMHPSBLOCKS*BUFCNT];

         for (CopyCnt=0;CopyCnt < (sizeof(arrayofptrs)/sizeof(arrayofptrs[0]));CopyCnt++) {
            arrayofptrsSave[CopyCnt] = arrayofptrs[CopyCnt];
         }

         mib[0] = CTL_OS2;
         mib[1] = AF_INET;
         mib[2] = 0;
         mib[3] = OS2_QUERY_MEMMAPIO;
         needed = 15*4*BUFCNT;

         if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), arrayofptrsSave, &needed, NULL, 0) < 0) {
            printf("sysctl(MEMMAPIO)=%d, errno=%d\n", rc, sock_errno());
            exit(1);
         }

         for (j = 0; j < (NUMHPSBLOCKS*BUFCNT); j++) {
#ifdef DEBUG
            printf("HPS Memory ptr %d:\t%p %p\n", j, arrayofptrs[j], arrayofptrsSave[j]);
#endif
            if ( !arrayofptrsSave[j]) {
              printf("Opsss. Memory not yet freed by stack !!! %d:%p\n",j,arrayofptrs[j]);
              exit(1);
            }
         }
      }   /* VerifyMem  */


   } /* for */

   printf("HPS Server successfully completed %d iterations\n\n",i);

   /* DLLs will close all semaphores and sockets and memmapped */
   return 0;
}


int read_stream(int fd, char *ptr, int maxbytes) {
   int nleft, nread;

   nleft = maxbytes;
   printf("Receiving %d bytes\n", maxbytes);
   while (nleft > 0) {
      nread = recv(fd, ptr, nleft, 0);
      if (nread < 0)
         return(nread);          /* error, return < 0 */
      else if (nread == 0)
         break;                          /* EOF, return #bytes read */
      nleft -= nread;
      ptr += nread;
      printf(".");
   }
   printf("\n");
   return(maxbytes - nleft);               /* return >= 0 */
}
