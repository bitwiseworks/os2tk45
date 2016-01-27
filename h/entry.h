/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
#ifndef ENTRY_H
#define ENTRY_H

/* As it is clear from the file name, there are 2 strustures
 * declared in this header file. The first structure is used
 * to fill the entry routines needed by  the R0 applications.
 * The second structure is used to fill the entry routines
 * needed by the protocol device drivers (PDD).
 */

struct R0Buffer {
  /* single entry point for all R0 socket APIS */
  char  * r0entry;

  /* mbuf related calls */
  /* new calls */
  char  * newmget;
  char  * newmfree;
  char  * get_cluster;
  char  * free_cluster;
  /* traditional calls */
  char  * m_get;
  char  * m_getclr;
  char  * m_free;
  char  * m_freem;
  char  * m_copy;
  char  * m_cat;
  char  * m_adj;
  char  * m_pullup;
  /* data for mbuf utilities */
  char  * mbstat;

};


struct PDDBuffer {

  /* mbuf related calls */
  /* new calls */
  void ( * newmget)();
  void ( * newmfree)();
  void ( * get_cluster)();
  void ( * free_cluster)();
  /* traditional calls */
  void ( * m_get)();
  void ( * m_getclr)();
  void ( * m_free)();
  void ( * m_freem)();
  void ( * m_copy)();
  void ( * m_cat)();
  void ( * m_adj)();
  void ( * m_pullup)();
  /* data for mbuf utilities */
  void  * mbstat;

  /* calls from uipc_so1.c */
  void ( * sofree)();
  void ( * soclose)();
  void ( * soabort)();
  void ( * sodisconnect)();
  void ( * soshutdown)();
  void ( * sorflush)();
  void ( * sohasoutofband)();

  /* calls from uipc_so2.c */

  void ( * soisconnecting)();
  void ( * soisconnected)();
  void ( * soisdisconnecting)();
  void ( * soisdisconnected)();
  void ( * sonewconn1)();
  void ( * socantsendmore)();
  void ( * socantrcvmore)();
  void ( * sbwait)();
  void ( * sbwakeup)();
  void ( * sowakeup)();           /* is actually called from sowwakeup() */
  void ( * sowakeup1)();          /* is actually called from sorwakeup() */
  void ( * soreserve)();
  void ( * sbreserve)();
  void ( * sbrelease)();
  void ( * sbappend)();
  void ( * sbappendrecord)();
  void ( * sbappendaddr)();
  void ( * sbappendrights)();
  void ( * sbflush)();
  void ( * sbdrop)();
  void ( * sbdroprecord)();

  /* data */
  struct socket * * socket_bucket;
  long * allocatedSocket;
  struct socket * (* sno_to_socket) ();
  struct domain  * domains;


  /* from dprintf.c */
  void ( * dprintf)();
  void ( * dvprintf)();
  void ( * alloc_debug_mem)();
  void ( * dbgexp)();
  void ( * dbgflags)();

  /* corresponding data pointers */
  char  * dbgbuf;
  char  * dbgbufphys;

  /* set in ifndis */
  unsigned long  * hostid;

#ifdef OFFLOAD
  /* for offload support, used in afinet$ */
  void ( * enq_event)();
#endif

  /* for Anynet - as per Bart Vashaw, sept 30 '94 */
  char  * currentGDT;
  char  * GDTsels;
  void ( * alloccluster)(); /* recommended to use get_cluster() whenever possible */
               /* the difference is that get_cluster() gets a single cluster
                            * of 4Kbytes, but in alloccluster() we can pass the size of
                            * memory needed.
                            */
  /* added for anynet to work properly */
  /* may 25 1995 */
  short  MAXGDTS;
  void ( * socket)();
  void ( * connect)();
  long ( * sosetopt)();
  void ( * sendit)();
  void ( * recvit)();
  void ( * soo_close)();
  void ( * shutdown)();
  short( * sockargs)();
  void ( * sock_cancel)();
  /* jun 22 1995, added for Anynet */
  void ( * listen)();
  void ( * accept)();
  void ( * soo_select)();
  void ( * soo_winsock_select)();


#ifdef SMP
   unsigned long  * grab;  /* passed to grab the lock */
   unsigned long  * grabtimeout; /* passed to set the grab timer */
#endif
   void (* AddDomain)();
   unsigned long * pGInfo;
   unsigned long * pLInfo;
   unsigned long * gseconds;
   unsigned long * gmilseconds;
   unsigned long (* ms_since_midnight)();
   void (* yield)();
   void (* gdv_yield)();
   void (* gdv_tcyield)();
   long(* sleep)();
   long(* sleeptime)();
   void (* wakeup)();
   void (* lock)();
   long (*_CDevHelp)();
   unsigned long * ptkssbase;
   struct mbuf * (* m_prepend)();
   void  (* m_copydata)();
   struct mbuf *  (* m_copym)();
   struct mbuf *  (* m_gethdr)();
   struct mbuf *  (* m_retryhdr)();
   struct mbuf *  (* m_retry)();
   long  (* sb_lock)();
   long max_hdr;
   long max_datalen;
   long max_linkhdr;
   long max_protohdr;
   unsigned long sb_max;
   struct protosw * (* pffindproto)();
   void  (* pfctlinput)();
   void  (* lockcode32)();
   unsigned long * _debug_flag_p;
   int  (* splon)();
   int  (* sploff)();
   long  (*bcmp) ();
   long  (*bzero)();
   long  (*bset) ();
   long  (*bcopy)();
   unsigned long  (* mhouse)();
   short * p_spr;
   long  * p_intr_tr_buf1;
   long  * clean_partial_conns;
  unsigned long (*seltoflat)();
  unsigned long * ip_id;
  struct in_ifaddr ** in_ifaddr;
  struct ifnet ** ifnet;
  void   (*devhelp32)(void);
  int    (*DevHelp_OpenEventSem)();
  int    (*DevHelp_SemHandle)();
  int    (*DevHelp_PostEventSem)();
  unsigned long * (*mclrefcnt)();
  int    (*DevHelp_CloseEventSem)();
  int    (*acceptwakeup)();
  void   (*soqinsque)();
  void   (*sbuck_remque)();
#ifdef PERF_HTTP
  void   (*sonewconn_http)();
#endif
#ifdef KEE
  void   (*attach_cluster)();
  void   (*newmgethdr)();
#endif

};






#endif
