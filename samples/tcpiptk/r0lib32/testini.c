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
/************************** TESTINI.C *****************************************/
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOS
#include <os2.h>
#include <stdio.h>
#include <malloc.h>
#include <genio.h>


int main()
{
  HFILE test_handle;
  unsigned long action;
  unsigned long cbFile          = 0L;
  unsigned long ulAttribue      = 0L;
  unsigned long fsOpenFlags     = 1L;
  unsigned long fsOpenModes     = 0x12;
  unsigned long peaop2          = 0L;
  APIRET result;

  /* Open device driver */
  result=DosOpen("TEST$",&test_handle,&action,cbFile,ulAttribue,fsOpenFlags,fsOpenModes,peaop2);
  if (result) {
    printf("Error %d opening TEST$ (test.sys)\n",result);
    return 1;
  }
  else
    printf("test.sys opened with handle %d\n",test_handle);


  /* Initialize device drivers */
//  result = DosDevIOCtl(NULL,NULL, 0x62, DeviceIO, test_handle);
  result = DosDevIOCtl(test_handle,DeviceIO,CALL_INIT,0L,0L,0L,0L,0L,0L);
  if (result) {
    result=DosClose(test_handle);
    printf("Unable to initialize TEST.SYS \n");
  }

}
