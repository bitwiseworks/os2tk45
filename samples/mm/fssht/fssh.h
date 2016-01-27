/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1991, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME:  FSSH.H                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  File System Stream Handler definitions                */
/*                                                                          */
/* STATUS:  MM Release 1.10                                                 */
/*                                                                          */
/* FUNCTION: This file defines any values and external varaibles specific   */
/*   to the file system stream handler.                                     */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* DEPENDENCIES:                                                            */
/*   SSM.H for RC typedef                                                   */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define FSSH_HANDLER_NAME  "FSSH"
#define FSSH_VERSION       100
#define FSSH_PROCCNT_MTX   "\\SEM32\\FSSH"  /* Name of 32bit Named Mutex */
                                            /* Semaphore used to gaurd access */
                                            /* to the process count variable.*/

extern VOID FsshWrite(void);    /* Write routine for recording to disk */
extern VOID FsshRead(void);     /* Read routine to read from disk */
