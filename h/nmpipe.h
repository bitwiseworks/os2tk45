/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/

/********************************************************************
 *								    *
 *  About this file ...  NMPIPE.H				    *
 *								    *
 *  This file contains information about the Named Pipe APIs.	    *
 *								    *
 *	Function prototypes.					    *
 *								    *
 *	Data structure templates.				    *
 *								    *
 *	Definition of special values.				    *
 *								    *
 ********************************************************************/


/**************************************************************** 
 *								*
 *	  	Function prototypes 				*
 *								*
 ****************************************************************/

extern API_FUNCTION
  DosMakeNmPipe(char far *, unsigned far *, unsigned short, unsigned short,
				unsigned short, unsigned short, long);

extern API_FUNCTION
  DosQNmPipeInfo(unsigned, unsigned short, char far *, unsigned short);

extern API_FUNCTION
  DosConnectNmPipe(unsigned);

extern API_FUNCTION
  DosDisconnectNmPipe(unsigned);

extern API_FUNCTION
  DosQNmpHandState(unsigned, unsigned short far *);

extern API_FUNCTION
  DosSetNmpHandState(unsigned, unsigned short);

extern API_FUNCTION
  DosPeekNmPipe(unsigned, char far *, unsigned short, unsigned short far *,
				unsigned short far *, unsigned short far *);

extern API_FUNCTION
  DosWaitNmPipe(char far *, long);

extern API_FUNCTION
  DosTransactNmPipe(unsigned, char far *, unsigned short, char far *,
				unsigned short, unsigned short far *);

extern API_FUNCTION
  DosCallNmPipe(char far *, char far *, unsigned short, char far *,
				unsigned short, unsigned short far *, long);

extern API_FUNCTION
  DosSetNmPipeSem(unsigned, long, unsigned short);

extern API_FUNCTION
  DosQNmPipeSemState(long, char far *, unsigned short);


/**************************************************************** 
 *								*
 *	  	Data structure templates			*
 *								*
 ****************************************************************/


struct	npi_data1 {	/* PipeInfo data block (returned, level 1) */
	unsigned short	npi_obuflen;	/* length of outgoing I/O buffer */
	unsigned short	npi_ibuflen;	/* length of incoming I/O buffer */
	unsigned char	npi_maxicnt;	/* maximum number of instances */
	unsigned char	npi_curicnt;	/* current number of instances */
	unsigned char	npi_namlen;	/* length of pipe name */
	char	npi_name[1];		/* start of name */
};	/* npi_data1 */

struct	npss	{	/* QNmPipeSemState information record */
	unsigned char	npss_status;	/* type of record, 0 = EOI, 1 = read ok,
					 *   2 = write ok, 3 = pipe closed */
	unsigned char	npss_flag;	/* additional info, 01 = waiting thread */
	unsigned short	npss_key;	/* user's key value */
	unsigned short	npss_avail;	/* available data/space if status = 1/2 */
};	/* npss */

/* values in npss_status */
#define	NPSS_EOI	0	/* End Of Information */
#define	NPSS_RDATA	1	/* read data available */
#define	NPSS_WSPACE	2	/* write space available */
#define	NPSS_CLOSE	3	/* pipe in CLOSING state */

/* values in npss_flag */
#define	NPSS_WAIT	0x01	/* waiting thread on other end of pipe */


/**************************************************************** 
 *								*
 *	  	Special values and constants			*
 *								*
 ****************************************************************/

/* defined bits in pipe mode */
#define	NP_NBLK		0x8000			/* non-blocking read/write */
#define	NP_SERVER	0x4000			/* set if server end */
#define	NP_WMESG	0x0400			/* write messages */
#define	NP_RMESG	0x0100			/* read as messages */
#define	NP_ICOUNT	0x00FF			/* instance count field */


/*	Named pipes may be in one of several states depending on the actions
 *	that have been taken on it by the server end and client end.  The
 *	following state/action table summarizes the valid state transitions:
 *
 *	Current state		Action			Next state
 *
 *	 <none>		    server DosMakeNmPipe	DISCONNECTED
 *	 DISCONNECTED	    server connect		LISTENING
 *	 LISTENING	    client open			CONNECTED
 *	 CONNECTED	    server disconn		DISCONNECTED
 *	 CONNECTED	    client close		CLOSING
 *	 CLOSING	    server disconn		DISCONNECTED
 *	 CONNECTED	    server close		CLOSING
 *	 <any other>	    server close		<pipe deallocated>
 *
 *	If a server disconnects his end of the pipe, the client end will enter a
 *	special state in which any future operations (except close) on the file
 *	descriptor associated with the pipe will return an error.
 */

/*
 *	Values for named pipe state
 */

#define	NP_DISCONNECTED	1		/* after pipe creation or Disconnect */
#define	NP_LISTENING	2		/* after DosNmPipeConnect */
#define	NP_CONNECTED	3		/* after Client open */
#define	NP_CLOSING	4		/* after Client or Server close */
