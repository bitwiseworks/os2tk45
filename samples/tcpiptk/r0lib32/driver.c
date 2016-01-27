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
/************************** DRIVER.C ****************************************/
#define INCL_DOS
#define INCL_DOSINFOSEG
#undef  KERNEL

#define INADDR_ANY              (u_long)0x00000000
#define BUFF_SIZE       1024


#include <sys\itypes.h>
#include <driver.h>
#include <genio.h>
#include <sys\socket.h>
#include <sys\uio.h>
#include <sys\ioctl.h>
#include <sys\un.h>
#include <netinet\in.h>
#include <unistd.h>

void devopen(lpRequest);
void GenIOCtrl(lpRequest);
void devclose(lpRequest);
void init(lpRequest);

extern void int3break(void);
void test_server();



void strategy32_c(lpreq)
lpRequest lpreq;
{
   switch(lpreq->reqcommand) {
      case INIT:
           lpreq->reqstatus = GEN_ERR + ERR_UNKNOWNCOMMAND + DONE_BIT;
           break;
      case OPEN:
           devopen(lpreq);
           break;
      case CLOSE:
           devclose(lpreq);
           break;
      case GENIO:
           GenIOCtrl(lpreq);
           break;
      default:
           lpreq->reqstatus = GEN_ERR + ERR_UNKNOWNCOMMAND + DONE_BIT;
           break;
   } /* end switch */
}


typedef unsigned long physaddr;  /* A physical address is 4 bytes */
typedef void  * farptr;

typedef struct _2words_def {
  unsigned low;
  unsigned high;
  } _2words_type;

typedef struct _segaddr_def {
  unsigned offset;
  unsigned segment;
  } _segadr_type;

typedef union _32bits_def {
   physaddr phys;
   void * fptr;
   _2words_type _2words;
   _segadr_type _segadr;
   } _32bits;


void devopen(lpreq)
lpRequest lpreq;
{
   lpreq->reqstatus |= DONE_BIT;
}


void devclose(lpreq)
lpRequest lpreq;
{
   lpreq->reqstatus |= DONE_BIT;
}


void GenIOCtrl(lpreq)
lpRequest lpreq;
{
   switch(lpreq->fcode) {
          case CALL_INIT:
               init(lpreq);
               lpreq->reqstatus |= DONE_BIT;
            break;
          default:
            lpreq->reqstatus = GEN_ERR+ERR_UNKNOWNCOMMAND+DONE_BIT;
            return;
   }
}


void getmem(u_long base, u_long* offset, char* ptr, u_long size)
{
  *ptr = (char*)(base + *offset);
  *offset += size;
}


void init(lpRequest lpreq)
{
       test_server();/* calling the test program */

 /*
  int s1,s2, s3, namelen;
  long result=-1;
  char buf[50];
  char buf2[50];
  struct sockaddr_un name1, name2;

  int3break();

  namelen = 110;


  result = sock_init();
  if ((result>>16)==0xffffffff) goto done;


  result = socket(1,1,0);
  if ((result>>16)==0xffffffff) goto done;
  s1 = (int)result;

  result = socket(1,1,0);
  if ((result>>16)==0xffffffff) goto done;
  s2 = (int)result;


  name1.sun_family=1;
  memcpy(sstods(name1.sun_path), "\\socket\\name1\0", 14);
  result = bind(s1, (struct sockaddr *)sstods(&name1), namelen);
  if ((result>>16)==0xffffffff) goto done;

  name2.sun_family=1;
  memcpy(sstods(name2.sun_path), "\\socket\\name2\0", 14);
  result = bind(s2, (struct sockaddr *)sstods(&name2), namelen);
  if ((result>>16)==0xffffffff) goto done;

  result = listen(s2, 5);
  if ((result>>16)==0xffffffff) goto done;

  result = connect(s1, (struct sockaddr *)sstods(&name2), namelen);
  if ((result>>16)==0xffffffff) goto done;

  result = accept(s2, sstods(&name1), sstods(&namelen));
  if ((result>>16)==0xffffffff) goto done;
  s3 = (int)result;

  memcpy(sstods(buf2),"Hello World\0", 12);
  result = sendto(s3, sstods(buf2), 12, 0, (struct sockaddr *)sstods(&name1), namelen);
  if ((result>>16)==0xffffffff) goto done;

  recvfrom(s1, sstods(buf), 12, 0, (struct sockaddr *)sstods(&name2), sstods(&namelen));
  if ((result>>16)==0xffffffff) goto done;

done:

  shutdown(s1, 2);
  shutdown(s2, 1);

  soclose(s1);
  soclose(s2);

        */
}

u_long seltoflat(u_long seladdr)
{
   u_short sel;
   u_short off;
   u_short stackseg;
   u_long linear = 0L;
   struct GlobalDescReg * gdr;
   struct DescInfo * dinfo;
   extern u_short retss(void);
   extern u_long virt2lin(u_long);
   extern char _gdtrtab[8];

   if(seladdr) {
      sel = seladdr>>16;
      off = seladdr&0x0ffff;
   }

   stackseg = retss(); /* in thunk32.asm, vacpp icks... */

   /* check if it is a ldt */
   if(sel & 0x4) {
      /* ldt */
      linear = (LDT2FLAT(seladdr));
   }
   else if (sel == stackseg) {
      /* ptkssbase */
      if(ptkssbase)
         linear = (u_long)(sstods(off));
      else
         linear = (virt2lin(seladdr));
   }
   else {
      /* gdt */
      sel &= 0xfff8;     /* for safety reasons get the ring 0 gdt sel */
      gdr = (struct GlobalDescReg *) &(_gdtrtab);
      dinfo = (struct DescInfo *)((u_long)gdr->base+(u_long)sel);
      if(dinfo){
         /* optimize it later */
         linear = dinfo->base32;
         linear = linear << 8;
         linear |= dinfo->base24;
         linear = linear << 16;
         linear |= dinfo->base16;

         linear+=(u_long)off;
      }
   }

   return linear;
}

void test_server()
{

        char buf[BUFF_SIZE];/* buffer used for recv() and send() calls */

        struct sockaddr_in client,server;


        int sockd,sockd_c,optval = 1,result;
        int client_len,count;

        /********************************************************
         Initialize with sockets.This call is mandatory before any socket call
        **********************************************************/

        sock_init();

        result =  socket(AF_INET,SOCK_STREAM,0);
        if ((result>>16)==0xffffffff)
                goto done;

        else
                sockd = (int)result;

        /********************************************************
         Bind the socket to the server address.
        *********************************************************/
        server.sin_family = AF_INET;
        server.sin_port   = htons(5000);
        server.sin_addr.s_addr = INADDR_ANY;

        /*******************************************************

If we have a local variable declared as a pointer ,when ever we are assigning
this pointer to any other varible or passing the pointer as a paramater
to a function then we have  to use sstods(is a macro).

        ********************************************************/


        result = setsockopt(sockd,SOL_SOCKET,SO_REUSEADDR,(char *)sstods(&optval),sizeof(int) );
        if ((result>>16)==0xffffffff)
                goto done;


        result = bind(sockd, (struct sockaddr *)sstods(&server),sizeof(server));

        if ((result>>16)==0xffffffff)
                goto done;

        result = listen(sockd, 5);

        if ((result>>16)==0xffffffff)
                goto done;


        /*************************************************
                Accept a connection.
        ***************************************************/
        client_len = sizeof((client));
        result = accept(sockd,(struct sockaddr *)sstods(&client),sstods(&client_len));

        if ((result>>16)==0xffffffff)
                goto done;
        else
                sockd_c = (int)result;

        /****************************************************
         Receive the data on the newly connected socket.
        *******************************************************/
        result = recv(sockd_c,sstods(buf),BUFF_SIZE, 0);
        if ((result>>16)==0xffffffff)
                goto done;


        /****************************************************
                alter the recived data
        ******************************************************/

        for(count = 0;count < BUFF_SIZE;count++)
                buf[count] = (buf[count] + 1);

        /****************************************************
                sending the processd data
        ******************************************************/
        result = send(sockd_c, sstods(buf),BUFF_SIZE, 0);
        if ((result>>16)==0xffffffff)
                goto done;

        /*********************************************
                To test recvmsg() function
        ***********************************************/
        {
                char buff[50];
                struct msghdr   msg;
                struct iovec iov;

                iov.iov_base = sstods(&buff[0]);
                iov.iov_len  = sizeof(buff)-1;

                msg.msg_iov        = sstods(&iov);
                msg.msg_iovlen     = 1;

                result = recvmsg(sockd_c, (struct msghdr *) sstods(&msg),0);
                if ((result>>16)==0xffffffff)
                        goto done;

        }

        /********************************************************
                To test getsockname(),getpeername() and getsocktopt()
        **********************************************************/

        {
                struct sockaddr_in name;
                long hostid;
                int namelen,optval2 = 0,optlen;

                hostid = gethostid();

                result = getsockname(sockd_c,(struct sockaddr *)sstods(&name),(int *)sstods(&namelen));

                if ((result>>16)==0xffffffff)
                        goto done;

                result = getpeername(sockd_c,(struct sockaddr *)sstods(&name),(int *)sstods(&namelen));

                if ((result>>16)==0xffffffff)
                        goto done;

                optlen = sizeof(int);

                result = getsockopt(sockd,SOL_SOCKET,SO_REUSEADDR,(char *)sstods(&optval2),(int *)sstods(&optlen));

                if ((result>>16)==0xffffffff)
                        goto done;
        }


        /*********************************************************
                To test if this IOCTL worked or not U can use
                inetcfg -g synattack and it should not show it as SET
        **********************************************************/
        {
                int on_off =1; /* set flag ON */

                /* Set the SYN attack flag to ON/OFF */
                result = ioctl(sockd_c,SIOCSSYN,(char *)sstods(&on_off));

                if ((result>>16)==0xffffffff)
                        goto done;
        }
        /**********************************
                To test  Sendmsg()
        ***********************************/
        {
                struct msghdr   msg;
                struct iovec iov;
                char buffer[50] = "GANESHA";

                iov.iov_base = sstods(buffer);
                iov.iov_len  = sizeof(buffer);

                msg.msg_name       = (caddr_t) sstods(&server);
                msg.msg_namelen    = sizeof(server);

                msg.msg_iov        = (struct iovec *)sstods(&iov);
                msg.msg_iovlen     = 1;

                result = sendmsg(sockd_c,(struct msghdr *)sstods(&msg),0);
                if ((result>>16)==0xffffffff)
                        goto done;
        }

done:

        shutdown(sockd, 2);
        shutdown(sockd_c, 1);

        soclose(sockd_c);
        soclose(sockd);
}
