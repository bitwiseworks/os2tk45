/*******************************************************************/
/*                                                                 */
/* Module Name:        DEVINFO.C                                   */
/*                                                                 */
/* Description:        Provide access to system device             */
/*                     device configuration information from REXX  */
/*                     programs.  The information is obtained from */
/*                     DosDevConfig.                               */
/*                                                                 */
/* Function:           Call DosDevConfig and store information     */
/*                     in the REXX program through the REXX        */
/*                     Variable Pool Interface (RexxVariablePool). */
/*                                                                 */
/* Entry point:        DevInfo                                     */
/*                                                                 */
/* Notes:                                                          */
/*                                                                 */
/*   The entry point obeys the calling conventions for REXX        */
/*   subcommand handlers.                                          */
/*                                                                 */
/*******************************************************************/

/*******************************************************************/
/*                                                                 */
/* Include files.  We use the following:                           */
/*                                                                 */
/*    REXXSAA.H         REXX specific type definitions and         */
/*                      function prototypes.                       */
/*                                                                 */
/*    OS2.H             OS/2 specific type definitions and         */
/*                      function prototypes.                       */
/*                                                                 */
/*    stdio.h           Standard C library I/O function prototypes.*/
/*                                                                 */
/*    stdlib.h          Standard C library function prototypes.    */
/*                                                                 */
/*    string.h          Standard C library string function         */
/*                      prototypes.                                */
/*                                                                 */
/*******************************************************************/


#define     INCL_DOSDEVICES
#include    <os2.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#define     INCL_RXSHV
#define     INCL_RXSUBCOM
#include    <rexxsaa.h>



/*******************************************************************/
/*                                                                 */
/* Function prototypes follow.                                     */
/*                                                                 */
/*******************************************************************/
RexxSubcomHandler DevInfo;

static APIRET QueryAndSet(
   PRXSTRING,
   PUCHAR,
   ULONG,
   PBOOL);

/*******************************************************************/
/*                                                                 */
/* Devinfo                                                         */
/*                                                                 */
/* This is the main function and entry point.                      */
/* This is a REXX subcommand handler, sometimes called an          */
/* environment.                                                    */
/*                                                                 */
/* A subroutine is called to check the input command against each  */
/* of the allowed commands, and handle the command if it is OK.    */
/* After all the commands are done, this routine returns to REXX,  */
/* passing back the new value for the REXX special variable RC.    */
/* This routine also sets the REXX error/failure flag.             */
/*                                                                 */
/* The possible exit values are as follows                         */
/*   REXX RC            Error flag          Reason                 */
/*   ------------------------------------------------------------- */
/*   0                  RXSUBCOM_OK         success                */
/*   non-zero number    RXSUBCOM_FAILURE    non-zero return from   */
/*                                          DosDevConfig or        */
/*                                          RexxVariablePool       */
/*   COMMAND TOO LONG   RXSUBCOM_ERROR      command more than nine */
/*                                          characters long        */
/*   UNKNOWN COMMAND    RXSUBCOM_ERROR      command not one of the */
/*                                          following values       */
/*                                          (upper and lower case  */
/*                                          are both OK):          */
/*                                            PARALLEL             */
/*                                            RS232                */
/*                                            DISKETTE             */
/*                                            MATH                 */
/*                                            SUBMODEL             */
/*                                            MODEL                */
/*                                            DISPLAY              */
/*                                            ALL                  */
/*                                                                 */
/*******************************************************************/

ULONG DevInfo(
   PRXSTRING cmd,                      /* command string (input)  */
   PUSHORT err_flag,                   /* ERROR/FAILURE  (output) */
   PRXSTRING rv)                       /* string retcode (output) */
{
   APIRET rc,                          /* rc from function we call*/
          maxrc,                       /* max value received in rc*/
          finalrc;                     /* our 'return' value      */
   BOOL FoundMatch;                    /* did we recognize command*/

   if (cmd->strlength > 9) {
      *err_flag = RXSUBCOM_ERROR;      /* this is REXX ERROR cond.*/
      strcpy(rv->strptr, "COMMAND TOO LONG");
      rv->strlength = strlen(rv->strptr);
      finalrc = 0;                     /* let string be the RC    */

   } else {
      /* Now check for all possible commands, and do the          */
      /* requested operation.                                     */
      maxrc = 0;
      FoundMatch = 0;

      rc = QueryAndSet(cmd, "PARALLEL", DEVINFO_PRINTER,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "RS232", DEVINFO_RS232,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "DISKETTE", DEVINFO_FLOPPY,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "MATH", DEVINFO_COPROCESSOR,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "SUBMODEL", DEVINFO_SUBMODEL,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "MODEL", DEVINFO_MODEL,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      rc = QueryAndSet(cmd, "DISPLAY", DEVINFO_ADAPTER,
         &FoundMatch);
      if(rc > maxrc) maxrc = rc;

      /* We've checked all the valid input commands and have done */
      /* the processing.  Now we set up our return values and exit*/
      if (!FoundMatch) {
         *err_flag = RXSUBCOM_ERROR;   /* Raise ERROR in REXX pgm.*/
         strcpy(rv->strptr, "UNKNOWN COMMAND");
         rv->strlength = strlen(rv->strptr);
         finalrc = 0;                  /* let the string be the RC*/
      } else {
         if (maxrc != 0) {
            *err_flag = RXSUBCOM_FAILURE;/* Raise FAILURE in REXX */
            finalrc = maxrc;           /* use integer as REXX RC  */
         } else {
            /* finally, the successful case is handled.           */
            /* We return with the RXSTRING set to all zeros, and  */
            /* the return value set to 0.  This makes the REXX    */
            /* RC value "0".  We could also make the return value */
            /* RXSTRING into the string "0", and again return 0.  */
            /* That would also set the REXX RC value to "0".      */
            *err_flag = RXSUBCOM_OK;
            finalrc = 0;
            MAKERXSTRING(*rv, (PUCHAR) 0, 0);
         } /* end successful case, return code is zero            */

      } /* end Found a match                                      */

   } /* endif cmd->strlength .....                                */

   return finalrc;
}

/*******************************************************************/
/*                                                                 */
/* QueryAndSet                                                     */
/*                                                                 */
/* This routine will check the data passed to it, and make at most */
/* one call to DosDevConfig.  It that call is OK, it calls         */
/* RexxVariablePool to put the returned data into a REXX variable. */
/*                                                                 */
/*******************************************************************/

static APIRET QueryAndSet(
   PRXSTRING cmd,                   /* User's command              */
   PUCHAR name,                     /* Command name to compare with*/
   ULONG DeviceType,                /* code for DosDevConfig       */
   PBOOL FoundMatch)                /* we indicate if name matches */
{
   SHVBLOCK block;                  /* block to pass to REXX       */
   BYTE DeviceInfoByte;             /* area to hold BYTE size info */
   LONG DeviceInfoDword;            /* area to hold DWORD size info*/
   UCHAR ValueBuffer [12];          /* buffer to hold convered info*/
   PVOID DeviceInfoPointer;         /* pointer to output area      */
   APIRET rc;                       /* return code from calls      */

/* First we compare the user's command with the command name       */
/* we are trying for, and ALL.  If either matches we go on and     */
/* call DosDevConfig.                                              */
   if (!stricmp(cmd->strptr, (const char *)name) || !stricmp(cmd->strptr, "ALL")) {
      *FoundMatch = 1;                 /* record that we got match */
      if (DeviceType <= 6) {
         DeviceInfoPointer = (PVOID) &DeviceInfoByte;
      } else {
         DeviceInfoPointer = (PVOID) &DeviceInfoDword;
      } /* endif */

      rc = DosDevConfig(DeviceInfoPointer, DeviceType);
      if (!rc) {
         /* Now turn the returned value into a string in decimal.  */
         if (DeviceType <= 6) {
            sprintf(ValueBuffer, "%d", (int) DeviceInfoByte);
         } else {
            sprintf(ValueBuffer, "%d", (int) DeviceInfoDword);
         } /* endif */

         /* Build the Shared Variable Block that defines our       */
         /* request.  The shvnext pointer is null because we only  */
         /* are doing one request.                                 */
         /* The shvname RXSTRING is the name passed to this routine*/
         /* The shvvalue RXSTRING is constructed from the string   */
         /* we just created above.                                 */
         /* The shvnamelen and shvvaluelen duplicate the lengths   */
         /* from the RXSTRINGS for simple variable-setting         */
         /* operations such as we are doing here.                  */
         /* The shvcode is set to RXSHV_SET, which means to set    */
         /* the variable, using its name as-is.  This means the    */
         /* name must be a valid REXX symbol, in upper case.       */
         /* The shvret byte is not set--it is an output value.     */

         block.shvnext = (PSHVBLOCK) NULL;
         block.shvname.strptr = name;
         block.shvname.strlength = strlen((const char *)name);
         MAKERXSTRING(block.shvvalue, ValueBuffer,
                      strlen((const char *)ValueBuffer));
         block.shvnamelen = block.shvname.strlength;
         block.shvvaluelen = block.shvvalue.strlength;
         block.shvcode = RXSHV_SET;

         /* Call the REXX variable pool.  We use an AND operation  */
         /* to turn off the low-order bit in the return code,      */
         /* because that bit indicates "new variable created", and */
         /* we do not care about that.  Other return values        */
         /* indicate an error.                                     */
         rc = RexxVariablePool(&block) & 0xFFFFFFFE;

      } /* endif rc from DosDevConfig was zero                     */
   }  /* End if we got a match */
   else
      {
         rc = 0;
      }  /* End we did not get a match */

   return rc; /* rc from DosDeviceConfig or RexxVariablePool, or 0 */
}

