/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1996.                                   */
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
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*   LIMITATIONS:                                                           */
/*     -Assumes TOTAL directory copy.  ie: no pattern checking              */
/*     -May not work to Servers requiring ACCOUNT info                      */
/*     -The "get" function works with FTPD in TCP/IP for OS/2 Warp V4.0 and */
/*      some Unix FTDs.  Basically the "get" function anticipates return    */
/*      data from FTPD as below:                                            */
/*                                                                          */
/*         file1                                                            */
/*         file2                                                            */
/*         ...                                                              */
/*         subdir1:            <-- subdirectory name ends with a ':'        */
/*         subdir1_file1       <-- file(s) in the subdirectory              */
/*         subdir1_file2                                                    */
/*         ....                                                             */
/*                             <-- a blank line                             */
/*         subdir2:                                                         */
/*         subdir2_file1                                                    */
/*         ....                                                             */
/* Modification History:                                                    */
/* Date:      By:   Tag:    Desc:                                           */
/* 09.16.96   DRC   DRC01   Added check for Borland compiler                */
/* 09.26.96   DRC   DRC02   Added check for Watcom Compiler and!!           */
/*                          put jump around _chdrive - it does not exist    */
/*                          in the Watcom compiler. The options are:        */
/*                          let _chdir change drives also or -              */
/*                          use _dos_setdrive.                              */
/* 09.30.96   DRC   DRC03   Changed UCHAR definition to CHAR for tmp -      */
/*                          error <NOT ANSI COMPATABLE> from Metaware       */
/*                          compiler.                                       */
/* 09.30.96   DRC           removed unreferenced variables                  */
/********************************************************copyrite.xic********/
#define  INCL_DOSFILEMGR
#include <os2.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ftpapi.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_FILEHANDLES  100L
#define MAX_FILENAME_LEN 512

/*   DRC01 && DRC02
** Define some variables to exclude a prefix "_"
*/

#if defined(__BORLANDC__) || (__WATCOMC__)

#define      _chdir      chdir
#define      _mkdir      mkdir
#define      _getcwd      getcwd
#define      _unlink      unlink

#endif


/** Function Prototypes **/
int  sendfiles(char *);
void getfiles(void);
int  mygets(char in[], int, FILE *);
void usage(void);
void ftperror(int);

char local[MAX_FILENAME_LEN+1];
int  transfertype = T_BINARY;
char *host, *user, *passwd;
char *accnt = NULL;
char locpath[MAX_FILENAME_LEN+1] = "";
char destpath[MAX_FILENAME_LEN+1];
char out[MAX_FILENAME_LEN+1];
char host_sys_buf[MAX_FILENAME_LEN+1];            /* Store Host System Type */
BOOL debug = FALSE;


/****************************************************************************/
/** Function: main()                                                       **/
/** Purpose : Parse input args & call proper PUT/GET function              **/
/****************************************************************************/
int main(int argc, char *argv[], char *envp[]) {
   APIRET apirc;
   int i, rc, validarg = 0;
   ULONG curmaxfh;
   char ftptrc[] = "FTPAPI.TRC";
   BOOL willput = TRUE;
   char *curpath;
   LONG reqcount = 0;

   /***  Check input arguments  ***/
   for (i =1; i <argc; i++) {
      if (argv[i][0] == '-') {
       switch (argv[i][1]) {
          case 'a':
            case 'A': /* Account info  */
                     accnt = &argv[i][2];
                     break;

            case 'l':
            case 'L': /* Local Host path */
                     strncpy(locpath, &argv[i][2], MAX_FILENAME_LEN);
                     locpath[0] = toupper((int)locpath[0]);
                     break;

            case 'h':
            case 'H': /* Remote Host */
                     host = &argv[i][2];
                     validarg++;
                     break;

            case 'u':
            case 'U': /* Userid on Remote */
                     user = &argv[i][2];
                     validarg++;
                     break;

            case 'p':
            case 'P': /* Password on Remote Host (may not be required) */
                     passwd = &argv[i][2];
                     break;

            case 'r':
            case 'R': /* Remote directory */
                     strncpy(destpath, &argv[i][2], MAX_FILENAME_LEN);
                     destpath[0] = toupper((int)destpath[0]);
                 break;

            case 'd':
            case 'D': /* Debug & trace info */
                     fprintf(stdout, " Debug ON.  Output to RCOPY.TRC\n");
                     debug = TRUE;
                     ftptrcon("RCOPY.TRC", M_OVERLAY);
                     setbuf(stdout, 0);
                     setbuf(stderr, 0);
                     break;

            case '?':
            default :
                     usage();
                     break;
         } /*end sw*/
      } else /*endif*/
         if (strcmpi(argv[i], "put") == 0)
            willput = TRUE;
         else
          if (strcmpi(argv[i], "get") == 0)
             willput = FALSE;
          else
           if (strcmpi(argv[i], "ascii") == 0)
              transfertype = T_ASCII;
           else
            if (strcmpi(argv[i], "binary") == 0)
               transfertype = T_BINARY;
   } /*endfor*/


   if (validarg < 2) {  /* Make sure we got the min required args */
      usage();          /* host, userid, rest defaults are taken  */
   } /* endif */


   /* Get local host current dir info */
   curpath = _fullpath(NULL, ".", 0);

   /* Setup Local Directory: change to correct disk & path  */
   if (locpath[0] =='\0')
      strncpy(locpath, curpath, MAX_FILENAME_LEN);
   else {


#ifndef __WATCOMC__                  /* DRC02 - let chdir also change drives */

      if (locpath[1] == ':') {                    /* Drive letter specified */
         _chdrive(locpath[0]-'A'+1);              /* Need to change to it   */
      } /* endif */

#endif                              /* DRC02 */


      if (_chdir(locpath)) {             /* Change to the proper local path */
         if (willput) {
            printf("local directory \"%s\" not found\n", locpath);
            exit(1);
         } else {
            if (_mkdir(locpath) == 0) {
               if (_chdir(locpath))
                  printf("Could not CD to newly created dir %s\n", locpath);
            } else {
               printf("could not create local dir %s\n",locpath);
               exit(-1);
            }/*endif*/
         }/*endelse*/
      }/*endif*/
   } /* endelse */


   /* Get Server System Type */
   if (!ftpsys(host,user,passwd, accnt, host_sys_buf, MAX_FILENAME_LEN+1)) {
      printf("Host type: %s\n", host_sys_buf);
   } else
      switch (ftperrno) {           /*  Exit if the RC is bad */
         case FTPSERVICE:
         case FTPHOST:
         case FTPSOCKET:
         case FTPCONNECT:
         case FTPLOGIN:
            ftperror(ftperrno);
            exit(-1);
            break;
      } /* endswitch */


   /* See if the dest path is valid on the remote host */
   if (destpath[0] != '\0') {               /* user wants default directory */
      if ((rc = ftpcd(host, user, passwd, accnt, destpath)) != 0) {
         if (rc == FTPLOGIN) {
            ftperror(ftperrno);
            exit(-1);
         } /* endif */


         if (!willput) {
            printf("Remote directory %s doesn't exist.\n", destpath);
            exit(-1);
         } /*endif*/

         /* Directory didn't exist for PUT, try to create */
         if ((rc = ftpmkd(host, user, passwd, accnt, destpath)) == 0) {
            if ((rc = ftpcd(host, user, passwd, accnt, destpath)) == -1) {
               printf("Unable to change directory to %s.\n", destpath);
               exit(-1);
            }/*endif*/
         } else {
            printf(" Unable to create directory \"%s\".\n", destpath);
            exit(-1);
         }/*endelse*/
      }/*endif*/
   } /* endif */

   /* Increase filehandles for this appl. */
   if (!DosSetRelMaxFH(&reqcount, &curmaxfh)) {  /* Already > what we want? */
      if (curmaxfh <MAX_FILEHANDLES) {
         if ((apirc = DosSetMaxFH(MAX_FILEHANDLES)) != 0)/*No, set it higher*/
            printf("unable to set max of file handles RC:%d\n", apirc);
      } /* endif */
   } /* endif */

   if ((apirc = DosSetMaxFH(MAX_FILEHANDLES)) != 0) {
      printf("unable to set max of file handles RC:%d\n", apirc);
      exit(1);
   }/*endif*/

   if (willput)
      sendfiles(NULL);
   else
      getfiles();

   ftplogoff();
   return(0);
}/*endmain*/


/****************************************************************************/
/** Function: sendfiles()                                                  **/
/** Purpose : FTP files from to remote host from local host                **/
/****************************************************************************/
int sendfiles(char *pat) {
   int rc, i;    /* BSF Removed global i.  Not sure if this is a problem */
   ULONG count;
   char *sp;
   HDIR hdir;
   FILEFINDBUF3 f;
   ULONG fileattrib = FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM |
                      FILE_DIRECTORY | FILE_ARCHIVED;

  if (pat != NULL)
     sp = pat;
   else sp = "*.*";

   hdir = 0xffff;                     /* causes a new handle to be returned */
   count = 1;                         /* find the first file */

   if ((rc = DosFindFirst(sp, &hdir, fileattrib, &f, sizeof(f), &count,
                     FIL_STANDARD)) != 0)
      return(-1);

   _getcwd(out, MAX_FILENAME_LEN+1);
   printf("\nlocal directory %s\n",out);
   i = strlen(out)-1;

   if(out[i] == '\\')
      out[i] = '\0';

   do {
      if (strcmp(f.achName,".") == 0)
       goto getnext;

      if (strcmp(f.achName,"..") == 0)
         goto getnext;

      /* Next directory */
      if (f.attrFile & FILE_DIRECTORY) {
         _chdir(f.achName);

         /* create the directory if it doesn't exist */
         if ((rc = ftpcd(host, user, passwd, accnt, f.achName)) != 0) {
            if (!ftpmkd(host, user, passwd, accnt, f.achName))
               rc = ftpcd(host, user, passwd, accnt, f.achName);
         }/*endif*/

         if (rc == -1) {
            ftperror(rc);
            printf(" Unable to transfer directory %s\n", f.achName);
            goto getnext;
         }/*endif*/

         sendfiles(NULL);
         _chdir("..");
         ftpcd(host, user, passwd, accnt, "..");
         _getcwd(out, MAX_FILENAME_LEN);
      } else {
          sprintf(local, "%s\\%s", out, f.achName);

          /*  while((p=strchr(local,'\\'))!=NULL) *p='/';*/
        printf("sending %s\n", local);

          if (ftpput(host, user, passwd, accnt, local,
                     f.achName, transfertype)) {
           printf("Transfer failed.\n");
             ftperror(ftperrno);
          }
      }/*endelse*/

getnext:
   rc = DosFindNext( hdir, &f, sizeof(f), &count);

   } while (count && (rc == 0));

   DosFindClose(hdir);
   return(0);
}/*endfunction*/


/****************************************************************************/
/** Function: getfiles()                                                   **/
/** Purpose : FTP files from remote host to local host.                    **/
/****************************************************************************/
void getfiles(void) {
   CHAR tmp[MAX_FILENAME_LEN+1];            /* DRC03 UCHAR->CHAR */
   FILE   *fps;
   int    rc;
   char in[MAX_FILENAME_LEN+1];
   char out[MAX_FILENAME_LEN+1];
   ULONG ActionTaken = 1L;
   char *p;

   _getcwd(out, MAX_FILENAME_LEN+1);
   printf("Local directory = %s\n", out);

   strcpy(tmp,"XXXXXX");                       /* Don't want tmp to be NULL */
   if (tmpnam(tmp) == NULL) {
      printf("Unable to create unique local file\n");
      exit(1);
   }/*endif*/


   /*  Non-IBM DOS Server, LS will return files & directories */
   /*  If GET fails, see if it is a directory.                */
   /*   AIX & SUN tested (checked for structure 1/2/95bsf     */
   /*   VM doesn't have DIR's so should be okay               */
   if ((rc = ftpls(host, user, passwd, accnt, tmp, "*")) != 0) {
      printf("Unable to get directory info.\n");
      ftperror(ftperrno);
      exit(1);
   }/*endif*/

   if ((fps = fopen(tmp, "r")) == NULL) {
      printf("Unable to open temp file.  RC: %d\n",rc);
      return;
   }/*endif*/


   while (mygets(in, MAX_FILENAME_LEN+1, fps)) {
      p = &in[0];

      /* Make sure this isn't THIS directory name, need to make sure that   */
      /* this does not cause problems for filenames that begin with ".".    */
      if (*p == '.')
       continue;

      if (ftpget(host, user, passwd, accnt, p, p, "w", transfertype)) {
       if (p[strlen(p)-1] == ':')
          p[strlen(p)-1] = '\0';

       if (!ftpcd(host, user, passwd, accnt, p)) {
          if (_chdir(p) !=0 ) {
             if (_mkdir(p) == 0) {
              if (_chdir(p))
                 printf("Could not CD to newly created dir %s\n", p);
             }
             else
              printf("Could not create local dir %s\n",p);
          }/*endif*/

          if (rc != 0)
             ftpcd(host, user, passwd, accnt, "..");
          else {
             getfiles();
             _chdir("..");
             ftpcd(host, user, passwd, accnt, "..");
          }/*endelse*/
       } else
           printf("Remote %s not a file or a dir\n", p);
      } else
       printf("Received %s\\%s\n",out, p);
   }/*endwhile*/

   fclose(fps);
   _unlink(tmp);
}/*endfunction*/


/****************************************************************************/
/** FUNCTION: mygets()                                                     **/
/** PURPOSE : Read line from file and return valid filename.  Return 1 if  **/
/**           a non-blank line is read, otherwise, return 0.               **/
/**           If a directory is encountered, then read all lines until     **/
/**           hitting another blank line or EOF.                           **/
/****************************************************************************/
int mygets(char in[], int len, FILE *fps) {
   int   i = 0;
   CHAR tmp[MAX_FILENAME_LEN+1];                  /* DRC03 UCHAR->CHAR */

   for (;;) {
      if (fgets(in, len, fps) == NULL) {
       if (!feof(fps))
          printf("Error reading from temp file.\n");
       in[0] = '\0';        /* EOF, return to calling function.           */
       return (0);
      } /*endif*/

      i = strlen(in) - 1;

      if (i != 0)             /* Probably read a blank line, read next line */
       break;
   }

   in[i] = '\0';

   if (in[i-1] == ':') {      /* Found a subdirectory, read thru the lines  */
      for (;;) {              /* after it until hitting a blank line.       */
       if (fgets(tmp, len, fps) == NULL) {
          if (!feof(fps))
             printf("Error reading from temp file.\n");
          break;
       }
         if (strlen(tmp) == 1)                          /* Hit a blank line */
            break;
      } /* endfor */
   }


   if (i > 0)
      return(1);
   else
      return(0);
}/*endfunction*/


/****************************************************************************/
/** FUNCTION: usage()                                                      **/
/** PURPOSE:  Display "HELP" menu and exit when complete.                  **/
/****************************************************************************/
void usage(void) {
   printf("Usage:\n");
   printf("RCOPY put|get ascii|binary [-l<local path>] -h<host> -u<userid> -p<password> [-r<rem path>] [-a<account>]\n\n");
   printf("RCOPY transfers the whole directory tree of files.  RCOPY doesn't read userid\n");
   printf("or password from the NETRC file so they must be specified if required.\n");
   printf("\tput  transfers from local host to remote host (default).\n");
   printf("\tget  transfers from remote host to local host.\n");
   printf("\tbinary  Binary transfer mode (default).\n");
   printf("\tascii   ASCII transfer mode.\n");
   printf("\nExample:\n");
   printf("rcopy put -ld:\\tmp -holeg -uyozzo -pyozzops -rc:\\newtmp\n\n");
   printf("Where:\n");
   printf("      d:\\tmp  is a local path to transfer\n");
   printf("      oleg     is a remote host\n");
   printf("      yozzo    is a remote user id\n");
   printf("      yozzops  is a remote password\n");
   exit(1);
}/*endfunction*/


/****************************************************************************/
/** FUNCTION: ftperror()                                                   **/
/** PURPOSE : translate ftp error rc to text output.                       **/
/****************************************************************************/
void ftperror(int value) {

   switch (value) {

      case FTPSERVICE:
         printf(" unknown service\n");
         break;
      case FTPHOST:
         printf(" unknown host\n");
         break;
      case FTPSOCKET:
         printf(" unable to obtain socket\n");
         break;
      case FTPCONNECT:
         printf(" unable to connect to server \n");
         break;
      case FTPLOGIN:
         printf(" login failed \n");
         break;
      case FTPABORT:
         printf(" transfer aborted \n");
         break;
      case FTPLOCALFILE:
         printf(" problem openning local file \n");
         break;
      case FTPDATACONN:
         printf(" problem initializing data connection \n");
         break;
      case FTPCOMMAND:
         printf(" command failed \n");
         break;
      case FTPPROXYTHIRD:
         printf(" proxy server does not support third party transfers \n");
         break;
      case FTPNOPRIMARY:
         printf(" No primary connection for proxy transfer \n");
         break;
      case FTPNOXLATETBL:
         printf(" No code page translation table was loded \n");
         break;
   } /* endswitch */
}/*endfunction*/

