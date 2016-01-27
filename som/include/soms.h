/*
 *   COMPONENT_NAME: somu
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.7 src/soms/soms.h, somu, som2.1 12/26/95 18:15:20 [7/30/96 14:49:46] */

/*
 */

#ifndef _soms_h
#define _soms_h

/* Sockets errnos */
#define SOMS_NOERROR		 0	/* no error */
#define SOMS_EWOULDBLOCK	 1	/* operation would block */
#define SOMS_EDEADLK		 SOMS_EWOULDBLOCK
#define SOMS_EAGAIN		 SOMS_EWOULDBLOCK
#define SOMS_EINPROGRESS	 2	/* operation now in progress */
#define SOMS_EALREADY		 3	/* operation already in progress */
#define SOMS_ENOTSOCK		 4	/* socket operation on non-socket */
#define SOMS_EDESTADDRREQ	 5	/* destination address required */
#define SOMS_EMSGSIZE		 6	/* message too long */
#define SOMS_EPROTOTYPE		 7	/* wrong protocol type for socket */
#define SOMS_ENOPROTOOPT	 8	/* protocol not available */
#define SOMS_EPROTONOSUPPORT	 9	/* protocol not supported */
#define SOMS_ESOCKTNOSUPPORT	10	/* socket type not supported */
#define SOMS_EOPNOTSUPP		11	/* operation not supported on socket */
#define SOMS_EPFNOSUPPORT	12	/* protocol family not supported */
#define SOMS_EAFNOSUPPORT	13	/* address family not supported */
#define SOMS_EADDRINUSE		14	/* address already in use */
#define SOMS_EADDRNOTAVAIL	15	/* can't assign requested address */
#define SOMS_ENETDOWN		16	/* network is down */
#define SOMS_ENETUNREACH	17	/* network is unreachable */
#define SOMS_ENETRESET		18	/* network dropped connection on reset*/
#define SOMS_ECONNABORTED	19	/* software caused connection abort */
#define SOMS_ECONNRESET		20	/* connection reset by peer */
#define SOMS_ENOBUFS		21	/* no buffers available */
#define SOMS_EISCONN		22	/* socket is already connected */
#define SOMS_ENOTCONN		23	/* socket is not connected */
#define SOMS_ESHUTDOWN		24	/* can't send after socket shutdown */
#define SOMS_ETOOMANYREFS	25	/* too many references */
#define SOMS_ETIMEDOUT		26	/* connection timed out */
#define SOMS_ECONNREFUSED	27	/* connection refused */
#define SOMS_ELOOP		28	/* too many levels of symbolic links */
#define SOMS_ENAMETOOLONG	29	/* file name too long */
#define SOMS_EHOSTDOWN		30	/* host is down */
#define SOMS_EHOSTUNREACH	31	/* no route to host */
#define SOMS_ENOTEMPTY		32	/* directory not empty */
#define SOMS_EBADF		33	/* bad socket descriptor */
#define SOMS_EINTR		34	/* socket call interrupted */
#define SOMS_EFAULT		35	/* bad address */
#define SOMS_EINVAL		36	/* invalid argument */
#define SOMS_EMFILE		37	/* no more socket descriptors */
#define SOMS_ENODEV		38	/* no such device */
#define SOMS_UNKNOWNERROR	39	/* unknown error */
#define SOMS_HOST_NOT_FOUND	40
#define SOMS_TRY_AGAIN		41
#define SOMS_NO_RECOVERY	42
#define SOMS_NO_ADDRESS		43

#ifndef bzero
#define bzero(p,n) memset((p),0,(n))
#endif

#ifndef bcopy
#define bcopy(s,d,n) memcpy((d),(s),(n))
#endif


/* Sockets data definitions */

/*
 * Definitions from sys/param.h
 */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	32
#endif



/*
 * Definitions from sys/uio.h
 */

struct Sockets_iovec {
	char    *iov_base;	/* base memory address			*/
/* WIN16PORT CHG5 must be an int for psl */
#ifdef _WIN16
	int	iov_len;	/* length of transfer for this area	*/
#else
	long	iov_len;	/* length of transfer for this area	*/
#endif
};


/*
 * Definitions from sys/time.h
 */

struct Sockets_timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
};


/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct Sockets_sockproto {
	unsigned short	sp_family;		/* address family */
	unsigned short	sp_protocol;		/* protocol */
};


/*
 * 4.3 compat sockaddr
 */
struct Sockets_sockaddr {
	unsigned short	sa_family;		/* address family */
	char	sa_data[14];		/* up to 14 bytes of direct address */
};

/*
 * 4.3-compat message header
 */
struct Sockets_msghdr {
	char	*msg_name;		/* optional address */
/* WIN16PORT CHG5 must be an int for psl */
#ifdef _WIN16
	int	msg_namelen;		/* size of address */
#else
	long	msg_namelen;		/* size of address */
#endif
	struct	Sockets_iovec *msg_iov;		/* scatter/gather array */
	long	msg_iovlen;		/* # elements in msg_iov */
	char	*msg_accrights;		/* access rights sent/received */
	long	msg_accrightslen;
};

/*
 * Internet address (a structure for historical reasons)
 */
struct Sockets_in_addr {
	unsigned long s_addr;
};


/*
 * Socket address, internet style.
 */
struct Sockets_sockaddr_in {
	unsigned short	sin_family;
	unsigned short	sin_port;
	struct	Sockets_in_addr sin_addr;
	char	sin_zero[8];
};

/*
 * Structure used to describe IP options.
 * Used to store options internally, to pass them to a process,
 * or to restore options retrieved earlier.
 * The ip_dst is used for the first-hop gateway when using a source route
 * (this gets put into the header proper).
 */
struct Sockets_ip_opts {
	struct	Sockets_in_addr ip_dst;		/* first hop, 0 w/o src rt */
	char	ip_opts[40];		/* actually variable in size */
};

/*
 * Structure used for manipulating linger option.
 */
struct	Sockets_linger {
	long	l_onoff;		/* option on/off */
	long	l_linger;		/* linger time */
};


/*
 * Definitions from netdb.h
 */

/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
struct	Sockets_hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
/* WIN16PORT CHG7 */
#ifdef _WIN16
	int	h_addrtype;	/* host address type */
	int 	h_length;	/* length of address */
#else
	long	h_addrtype;	/* host address type */
	long	h_length;	/* length of address */
#endif
	char	**h_addr_list;	/* list of addresses from name server */
#define	h_addr	h_addr_list[0]	/* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct	Sockets_netent {
	char		*n_name;	/* official name of net */
	char		**n_aliases;	/* alias list */
	long		n_addrtype;	/* net address type */
	unsigned long	n_net;		/* network # */
};

struct	Sockets_servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	long	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};

struct	Sockets_protoent {
	char	*p_name;	/* official protocol name */
	char	**p_aliases;	/* alias list */
	long	p_proto;	/* protocol # */
};

typedef	struct Sockets_fd_set {
	unsigned long	fds_bits[64];
} Sockets_fd_set;


/*
 * Definitions from sys/ioctl.h
 */
/* WIN16PORT CHG1 */
#if (defined(_OS2) || !defined(_H_IOCTL) || defined(_WIN16))

#ifndef _IO
/*
 * Ioctl's have the command encoded in the lower word,
 * and the size of any in or out parameters in the upper
 * word.  The high 2 bits of the upper word are used
 * to encode the in/out status of the parameter; for now
 * we restrict parameters to at most 128 bytes.
 */
#define	IOCPARM_MASK	0x7f		/* parameters must be < 128 bytes */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
/* the 0x20000000 is so we can distinguish new ioctl's from old */
#define	_IO(x,y)	(IOC_VOID|(x<<8)|y)
#define	_IOR(x,y,t)	(IOC_OUT|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
#define	_IOW(x,y,t)	(IOC_IN|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
/* this should be _IORW, but stdio got there first */
#define	_IOWR(x,y,t)	(IOC_INOUT|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
#endif /* _IO */

/* another local */

#define	FIONREAD	_IOR('f', 127, int)	/* get # bytes to read */
#define	FIONBIO		_IOW('f', 126, int)	/* set/clear non-blocking i/o */
#define	FIOASYNC	_IOW('f', 125, int)	/* set/clear async i/o */

#define	FIOSETOWN	_IOW('f', 124, int)	/* set owner */
#define	FIOGETOWN	_IOR('f', 123, int)	/* get owner */

/* socket i/o controls */
#define	SIOCSHIWAT	_IOW('s',  0, int)		/* set high watermark */
#define	SIOCGHIWAT	_IOR('s',  1, int)		/* get high watermark */
#define	SIOCSLOWAT	_IOW('s',  2, int)		/* set low watermark */
#define	SIOCGLOWAT	_IOR('s',  3, int)		/* get low watermark */
#define	SIOCATMARK	_IOR('s',  7, int)		/* at oob mark? */
#define	SIOCSPGRP	_IOW('s',  8, int)		/* set process group */
#define	SIOCGPGRP	_IOR('s',  9, int)		/* get process group */

#define	SIOCADDRT	_IOW('r', 10, struct ortentry)	/* add route */
#define	SIOCDELRT	_IOW('r', 11, struct ortentry)	/* delete route */

#define	SIOCSIFADDR	_IOW('i', 12, struct oifreq)	/* set ifnet address */
#define	OSIOCGIFADDR	_IOWR('i',13, struct oifreq)	/* get ifnet address */
#define	SIOCGIFADDR	_IOWR('i',33, struct oifreq)	/* get ifnet address */
#define	SIOCSIFDSTADDR	_IOW('i', 14, struct oifreq)	/* set p-p address */
#define	OSIOCGIFDSTADDR	_IOWR('i',15, struct oifreq)	/* get p-p address */
#define	SIOCGIFDSTADDR	_IOWR('i',34, struct oifreq)	/* get p-p address */
#define	SIOCSIFFLAGS	_IOW('i', 16, struct oifreq)	/* set ifnet flags */
#define	SIOCGIFFLAGS	_IOWR('i',17, struct oifreq)	/* get ifnet flags */
#define	OSIOCGIFBRDADDR	_IOWR('i',18, struct oifreq)	/* get broadcast addr */
#define	SIOCGIFBRDADDR	_IOWR('i',35, struct oifreq)	/* get broadcast addr */
#define	SIOCSIFBRDADDR	_IOW('i',19, struct oifreq)	/* set broadcast addr */
#define	OSIOCGIFCONF	_IOWR('i',20, struct ifconf)	/* get ifnet list */
#define	SIOCGIFCONF	_IOWR('i',36, struct ifconf)	/* get ifnet list */
#define	OSIOCGIFNETMASK	_IOWR('i',21, struct oifreq)	/* get net addr mask */
#define	SIOCGIFNETMASK	_IOWR('i',37, struct oifreq)	/* get net addr mask */
#define	SIOCSIFNETMASK	_IOW('i',22, struct oifreq)	/* set net addr mask */
#define	SIOCGIFMETRIC	_IOWR('i',23, struct oifreq)	/* get IF metric */
#define	SIOCSIFMETRIC	_IOW('i',24, struct oifreq)	/* set IF metric */
#define	SIOCDIFADDR	_IOW('i',25, struct oifreq)	/* delete IF addr */
#define	SIOCAIFADDR	_IOW('i',26, struct ifaliasreq)	/* add/chg IF alias */
#define	SIOCSIFSUBCHAN	_IOW('i',27, struct oifreq)	/* set subchannel adr.*/
#define	SIOCSIFNETDUMP	_IOW('i',28, struct oifreq)	/* set netdump fastwrt*/

#define	SIOCSARP	_IOW('i', 30, struct arpreq)	/* set arp entry */
#define	OSIOCGARP	_IOWR('i',31, struct arpreq)	/* get arp entry */
#define	SIOCGARP	_IOWR('i',38, struct arpreq)	/* get arp entry */
#define	SIOCDARP	_IOW('i', 32, struct arpreq)	/* delete arp entry */

#define	SIOCADDNETID	_IOW('i',87, struct oifreq)	/* set netids */
#define	SIOCSIFMTU	_IOW('i',88, struct oifreq)	/* set mtu */
#define	SIOCGIFMTU	_IOWR('i',86, struct oifreq)	/* get mtu */
#define	SIOCSIFREMMTU	_IOW('i',89, struct oifreq)	/* set remmtu */

#define SIOCSNETOPT     _IOW('i', 90, struct optreq) /* set network option */
#define SIOCGNETOPT     _IOWR('i', 91, struct optreq) /* get network option */
#define SIOCDNETOPT     _IOWR('i', 92, struct optreq) /* set default */

#define	SIOCSX25XLATE	_IOW('i', 99, struct oifreq)	/* set xlate tab */
#define	SIOCGX25XLATE	_IOWR('i',100, struct oifreq)	/* get xlate tab */
#define	SIOCDX25XLATE	_IOW('i', 101, struct oifreq)	/* delete xlate tab */

#endif /* _H_IOCTL */


/*
 * Definitions from sys/select.h
 */

/* WIN16PORT CHG1 */
#if (defined(_OS2) || !defined(_H_SELECT) || defined(_WIN16)) 

#ifndef NBBY
#define NBBY	8
#endif

/*
 * Timeout values.
 */
#ifndef NO_TIMEOUT
#define	NO_TIMEOUT	0	/* don't wait for a response		*/
#endif

#ifndef INF_TIMEOUT
#define	INF_TIMEOUT	-1	/* wait until a response is received	*/
#endif

/*
 * Number of file descriptors (bits).
 * Apply to nfdsmsgs parameter of SELECT system
 * call &/or return value from SELECT.
 */
#ifndef NFDS
#define NFDS(x)		((x) & 0x0000FFFF)
#endif /* NFDS */

/*
 * Lower half of word.
 * Used in hashing algorithm for devsel_hash chain.
 */
#ifndef LOW_HALF
#define	LOW_HALF(x)	NFDS(x)
#endif /* LOW_HALF */

/*
 * Number of message queues.
 * Apply to nfdsmsgs parameter of SELECT system
 * call &/or return value from SELECT.
 */
#ifndef NMSGS
#define NMSGS(x)	(((x) >> 16) & 0x0000FFFF)
#endif /* NMSGS */

/*
 * Upper half of word.
 * Used in hashing algorithm for devsel_hash chain.
 */
#ifndef HIGH_HALF
#define	HIGH_HALF(x)	NMSGS(x)
#endif /* HIGH_HALF */

/*
 * Macro used to define a sellist structure
 * for the SELECT system call.
 */
#ifndef SELLIST
#define	SELLIST(F,M)		\
struct				\
{				\
	unsigned int   fdsmask[F];	\
	int   msgids[M];	\
}
#endif /* SELLIST */

/*
 * Select uses bit masks of file descriptors.
 * These macros manipulate such bit fields.
 * FD_SETSIZE may be defined by the user to the maximum valued file
 * descriptor to be selected; the default here should be >= OPEN_MAX
 */
#ifndef	FD_SETSIZE
#define	FD_SETSIZE	2048
#endif

#ifndef NFDBITS
#define NFDBITS	(sizeof(unsigned long) * NBBY)	/* bits per mask */
#endif 

#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif

#ifdef NEVER
typedef	struct Sockets_fd_set {
	unsigned long	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} Sockets_fd_set;
#endif


#ifndef FD_SET
/* WIN16MERGE ADD3 Specify long for bitwise operation */
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
#endif

#ifndef FD_CLR
/* WIN16MERGE ADD3 Specify long for bitwise operation */
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
#endif

#ifndef FD_ISSET
/* WIN16MERGE ADD3 Specify long for bitwise operation */
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
#endif

#ifndef FD_ZERO
#define FD_ZERO(p)	bzero((char *)(p), sizeof(*(p)))
#endif

#endif /* _H_SELECT */


/*
 * Definitions from netinet/in.h
 */

/* WIN16PORT CHG1 */
#if (defined(_OS2) || !defined(_H_IN) || defined(_WIN16)) 
/*
 * Protocols
 */
#define	IPPROTO_IP		0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_GGP		3		/* gateway^2 (deprecated) */
#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_EGP		8		/* exterior gateway protocol */
#define	IPPROTO_PUP		12		/* pup */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define	IPPROTO_IDP		22		/* xns idp */
#define	IPPROTO_TP		29 		/* tp-4 w/ class negotiation */
#define	IPPROTO_EON		80		/* ISO cnlp */

#define	IPPROTO_RAW		255		/* raw IP packet */
#define	IPPROTO_MAX		256


/*
 * Local port number conventions:
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define	IPPORT_RESERVED		1024
#define	IPPORT_USERRESERVED	5000
#define	IPPORT_TIMESERVER	37



/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define	IN_CLASSA(i)		(((long)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((long)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(i)		(((long)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	IN_EXPERIMENTAL(i)	(((long)(i) & 0xe0000000) == 0xe0000000)
#define	IN_BADCLASS(i)		(((long)(i) & 0xf0000000) == 0xf0000000)

#define	INADDR_ANY		(unsigned long)0x00000000
#define INADDR_LOOPBACK		(unsigned long)0x7f000001	/* local loopback */
#define	INADDR_BROADCAST	(unsigned long)0xffffffff	/* must be masked */
#ifndef _KERNEL
#define	INADDR_NONE		0xffffffff		/* -1 return */
#endif

#define	IN_LOOPBACKNET		127			/* official! */


/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define	IP_OPTIONS	1	/* buf/ip_opts; set/get IP per-packet options */
#define	IP_HDRINCL	2	/* int; header is included with data (raw) */
#define	IP_TOS		3	/* int; IP type of service and precedence */
#define	IP_TTL		4	/* int; IP time to live */
#define	IP_RECVOPTS	5	/* bool; receive all IP options w/datagram */
#define	IP_RECVRETOPTS	6	/* bool; receive IP options for response */
#define	IP_RECVDSTADDR	7	/* bool; receive IP dst addr w/datagram */
#define	IP_RETOPTS	8	/* ip_opts; set/get IP per-packet options */

#endif /* _H_IN */


/*
 * Definitions from sys/socket.h
 */

/* WIN16PORT CHG1 */
#if (defined(_OS2) || !defined(_H_SOCKET) || defined(_WIN16)) 

/*
 * Types
 */
#define	SOCK_STREAM	1		/* stream socket */
#define	SOCK_DGRAM	2		/* datagram socket */
#define	SOCK_RAW	3		/* raw-protocol interface */
#define	SOCK_RDM	4		/* reliably-delivered message */
#define	SOCK_SEQPACKET	5		/* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG	0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN	0x0002		/* socket has had listen() */
#define	SO_REUSEADDR	0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE	0x0008		/* keep connections alive */
#define	SO_DONTROUTE	0x0010		/* just use interface addresses */
#define	SO_BROADCAST	0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK	0x0040		/* bypass hardware when possible */
#define	SO_LINGER	0x0080		/* linger on close if data present */
#define	SO_OOBINLINE	0x0100		/* leave received OOB data in line */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF	0x1001		/* send buffer size */
#define SO_RCVBUF	0x1002		/* receive buffer size */
#define SO_SNDLOWAT	0x1003		/* send low-water mark */
#define SO_RCVLOWAT	0x1004		/* receive low-water mark */
#define SO_SNDTIMEO	0x1005		/* send timeout */
#define SO_RCVTIMEO	0x1006		/* receive timeout */
#define	SO_ERROR	0x1007		/* get error status and clear */
#define	SO_TYPE		0x1008		/* get socket type */



/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	0xffff		/* options for socket level */

/*
 * Address families.
 */
#define	AF_UNSPEC	0		/* unspecified */
#define	AF_UNIX		1		/* local to host (pipes, portals) */
#define	AF_INET		2		/* internetwork: UDP, TCP, etc. */
#define	AF_IMPLINK	3		/* arpanet imp addresses */
#define	AF_PUP		4		/* pup protocols: e.g. BSP */
#define	AF_CHAOS	5		/* mit CHAOS protocols */
#define	AF_NS		6		/* XEROX NS protocols */
#define	AF_ISO		7		/* ISO protocols */
#define	AF_OSI		AF_ISO
#define	AF_ECMA		8		/* european computer manufacturers */
#define	AF_DATAKIT	9		/* datakit protocols */
#define	AF_CCITT	10		/* CCITT protocols, X.25 etc */
#define	AF_SNA		11		/* IBM SNA */
#define AF_DECnet	12		/* DECnet */
#define AF_DLI		13		/* DEC Direct data link interface */
#define AF_LAT		14		/* LAT */
#define	AF_HYLINK	15		/* NSC Hyperchannel */
#define	AF_APPLETALK	16		/* Apple Talk */
#define	AF_ROUTE	17		/* Internal Routing Protocol */
#define	AF_LINK		18		/* Link layer interface */
#define	pseudo_AF_XTP	19		/* eXpress Transfer Protocol (no AF) */
#define AF_INTF		20		/* Debugging use only */
#define AF_RIF		21		/* raw interface */
#define PF_INTF         AF_INTF		/* Used by sysx/intf */

#define	AF_NETWARE	22
#define	AF_MAX		30


/*
 * Protocol families, same as address families for now.
 */
#define	PF_UNSPEC	AF_UNSPEC
#define	PF_UNIX		AF_UNIX
#define	PF_INET		AF_INET
#define	PF_IMPLINK	AF_IMPLINK
#define	PF_PUP		AF_PUP
#define	PF_CHAOS	AF_CHAOS
#define	PF_NS		AF_NS
#define	PF_ISO		AF_ISO
#define	PF_OSI		AF_ISO
#define	PF_ECMA		AF_ECMA
#define	PF_DATAKIT	AF_DATAKIT
#define	PF_CCITT	AF_CCITT
#define	PF_SNA		AF_SNA
#define PF_DECnet	AF_DECnet
#define PF_DLI		AF_DLI
#define PF_LAT		AF_LAT
#define	PF_HYLINK	AF_HYLINK
#define	PF_APPLETALK	AF_APPLETALK
#define	PF_ROUTE	AF_ROUTE
#define	PF_LINK		AF_LINK
#define	PF_XTP		pseudo_AF_XTP	/* really just proto family, no AF */
#define PF_INTF		AF_INTF
#define PF_RIF		AF_RIF

#define	PF_MAX		AF_MAX

/*
 * Maximum queue length specifiable by listen.
 */
#define	SOMAXCONN	10

#define	MSG_OOB		0x1		/* process out-of-band data */
#define	MSG_PEEK	0x2		/* peek at incoming message */
#define	MSG_DONTROUTE	0x4		/* send without using routing tables */
#define	MSG_EOR		0x8		/* data completes record */
#define	MSG_TRUNC	0x10		/* data discarded before delivery */
#define	MSG_CTRUNC	0x20		/* control data lost before delivery */
#define	MSG_WAITALL	0x40		/* wait for full request or error */

#define MSG_MAXIOVLEN   16              /* may not need this */

#endif /* _H_SOCKET */


/* Sockets typedefs */
typedef struct Sockets_sockaddr Sockets_sockaddr;
typedef struct Sockets_sockaddr_in Sockets_sockaddr_in;
typedef struct Sockets_in_addr  Sockets_in_addr;
typedef struct Sockets_iovec    Sockets_iovec;
typedef struct Sockets_msghdr   Sockets_msghdr;
typedef struct Sockets_timeval  Sockets_timeval;
typedef struct Sockets_hostent  Sockets_hostent;
typedef struct Sockets_servent  Sockets_servent;

#define SOM_DONT_USE_SHORT_NAMES

#define sockaddr Sockets_sockaddr
#define sockaddr_in Sockets_sockaddr_in
#define in_addr  Sockets_in_addr
#define iovec    Sockets_iovec
#define msghdr   Sockets_msghdr
#define timeval  Sockets_timeval
#define hostent  Sockets_hostent
#define servent  Sockets_servent
#define fd_set   Sockets_fd_set

#ifdef __cplusplus
 #include <somssock.xh>
#else
 #include <somssock.h>
#endif

#undef SOM_DONT_USE_SHORT_NAMES

#endif /* _soms_h */
