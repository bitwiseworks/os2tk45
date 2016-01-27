/***************************************************************************/
/* probe.c: FFSTProbe sample                                              */
/*                                                                        */
/* This test program gives an example of using the FFSTProbe API via a      */
/* 'wrapper' function. The dummy api, My_Dummy_Api returns a return code */
/* which is then used as the basis of firing a FFSTProbe via the wrapper      */
/* function, callFFST. callFFST can be modified to include more or less         */
/* passed in data as needed.                                              */
/**************************************************************************/

#define INCL_DOS
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_FFST
#define NO_ERROR 0

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ffst.h>

/**************************************************************************/
/* Define probe ID for FFSTProbe called when dummy api fails.                           */ 
/* Probe ID is the unique identifier you use later to find the source of the failure   */
/* It should be unique within a DMI triplet (explained later) or within your product */
/**************************************************************************/
#define DUMMY_API_PROBE                  22222

/**************************************************************************/
/* callFFST is the FFSTProbe wrapper function. It allows you to code the           */
/* FFSTProbe API once with data that is static as far as your usage is concerned */
/* and allows you to pass in dynamic data.   It also helps insulate your code if you	*/
/* decide to change your 'static' options                                         */
/**************************************************************************/

void callFFST ( ULONG input_version,        /* FFST 'Wrapper' Function */
                ULONG input_probe_flags,        /* FFSTProbe probe flags */
                ULONG input_severity,             /* FFSTProbe severity */
                ULONG input_probe_id,            /* FFSTProbe ID */
                CHAR* input_module_name,     /* module name passed to probe */
                ULONG input_log_data_length,  /* log data length for the system error log */
                PVOID input_pError_log_data,   /* pointer to the data for system error log */
                int   argc)
{
   APIRET  rc = 0;
   PVOID   pvar_n0;
   ULONG   pvar_n1;

   /***********************************************************************/
   /* FFSTProbe API structures.    Described in the API Guide                                      */
   /***********************************************************************/
   FFSTPARMS     FFSTParms;
   PRODUCTINFO   productInfo;
   PRODUCTDATA   productData;
   DMIDATA       DMIData;
   DUMPUSERDATA  dumpUserData;
   MSGINSDATA    msgInsData;

   /***********************************************************************/
   /* The PRODUCTDATA structure defines the DMI triplet which allows      */
   /* additional product information, including template repository         */
   /* filename, to be retrieved from DMI.   DMI is a industry standare for desktop mgt  */
   /***********************************************************************/
   productData.packet_size            = sizeof ( productData );
   productData.packet_revision_number = PRODUCTDATA_ASCII; /* data can be ASCII or UNI*/
   productData.DMI_tag                = "FFSTProbe Sample";  /*Customize for your program*/
   productData.DMI_vendor_tag         = "IBM";                   /*Customize for your company*/ 
   productData.DMI_revision           = "1.00";                     /* Customize  */

   /**************************************************************************/
   /* The DMIDATA structure below is the information which can either be     */
   /* retrieved by DMI or passed in by the FFSTProbe function. The preferred */
   /* method is to use DMI. In the example below, you can see the use of     */
   /* either depending on whether or not a parm was passed on call to this program */
   /**************************************************************************/

   if ( !(argc>1) )
   {
      /********************************************************************/
      /* Setting this structure to NULL indicates that the information is */
      /* to be retrieved from DMI using the DMI triplet as defined in the */
      /* productData structure.  This is the preferred method.           */
      /* Other files in this example show how to build your own DMI  */
      /********************************************************************/
      productInfo.pDMIData = NULL;

      /*********************************************************************/
      /* Note: This shows the usage of message insert text and is NOT part */
      /* of the information that could or could not be retrieved from DMI  */
      /* This is included as an example of MsgInsTxt and how it can be used to send */
      /* probe specific data to the SYSLOG (System Error Log)  */
      /*********************************************************************/
      msgInsData.MsgInsTxt[0].insert_number = 1;
      msgInsData.MsgInsTxt[0].insert_text   = "We did use a DMI component";
   }
   else
    {
      /********************************************************************/
      /* fill the DMI data structure - useful only in test environments   */
      /********************************************************************/
      DMIData.packet_size              = sizeof ( DMIData );
      DMIData.packet_revision_number   = DMIDATA_ASCII;    /*could be unicode instead */
      DMIData.DMI_product_ID           = "FFST_toolkt_sample"; /*note this is different than tag */
      DMIData.DMI_modification_level   = "000000";
      DMIData.DMI_fix_level            = "010101";
      DMIData.template_filename        = "PROBE.REP";  /* this file must be on the DPATH */
      DMIData.template_filename_length = strlen ( DMIData.template_filename )
                                                * sizeof ( char );  /* since ascii is being used */
      productInfo.pDMIData             = &DMIData;

      /********************************************************************/
      /* Note: This shows the usage of message insert text and is NOT a   */
      /* of the information that could or could not be retrieved from DMI */
      /********************************************************************/
      msgInsData.MsgInsTxt[0].insert_number = 1;
      msgInsData.MsgInsTxt[0].insert_text   = "We did not use a DMI component";
    }

   /***********************************************************************/
   /* set the pointers up for PRODUCTINFO                                 */
   /***********************************************************************/

   productInfo.pProductData = &productData;   /* This points to the DMI related data */

   /***********************************************************************/
   /* set up some DUMPUSERDATA items                                      */
   /***********************************************************************/
   pvar_n0 = "Kilroy was here";        /* Anything can be dumped here, up to 30K bytes */
   pvar_n1 = 2;
   dumpUserData.no_of_variables = 2;
   dumpUserData.DumpDataVar[0].var_n_length = strlen("Kilroy was here");
   dumpUserData.DumpDataVar[0].var_n        = pvar_n0;
   dumpUserData.DumpDataVar[1].var_n_length = sizeof(ULONG);
   dumpUserData.DumpDataVar[1].var_n        = (PVOID)(&pvar_n1);

   /***********************************************************************/
   /* set up a couple of MSGINSDATA messages- just to show it can be done */
   /***********************************************************************/
   msgInsData.no_inserts   = 2;
   msgInsData.MsgInsTxt[1].insert_number = 2;
   msgInsData.MsgInsTxt[1].insert_text   = "Message insert variable 2";

   /***********************************************************************/
   /* set the FFSTPARMS structure, most values from DEFINEs above.  See API GUIDE */
   /* for details on each field and their possible values      */
   /***********************************************************************/
   FFSTParms.packet_size            = sizeof ( FFSTParms );
   FFSTParms.packet_revision_number = FFSTPARMS_OS2_ASCII; /* ASCI vs UNICODE data*/
   FFSTParms.module_name            = input_module_name;
   FFSTParms.probe_ID               = input_probe_id;
   FFSTParms.severity               = input_severity;      
   FFSTParms.template_record_ID     = input_probe_id;
   FFSTParms.pMsgInsData            = &msgInsData;           
   FFSTParms.probe_flags            = input_probe_flags;
   FFSTParms.pDumpUserData          = &dumpUserData;  /* dump data is stored in .DMP files*/
   FFSTParms.log_user_data_length   = input_log_data_length;
   FFSTParms.log_user_data          = input_pError_log_data;  /* log data is stored as part of the SYSLOG entry */

   /***********************************************************************/
   /* Call the FFSProbe API                                               */
   /***********************************************************************/
   if ( input_version == 1)
   {
      rc = FFSTProbe ( &productInfo, &FFSTParms);
   }

   printf("\n----- Fired the FFSTProbe, rc=%d\n",rc); /* for example only, do not do this in customer level code*/

}

/**************************************************************************/
/*  This is the dummy  API for use in the example.  It can easily set non-zero rc's         */
/**************************************************************************/

ULONG My_Dummy_API ( ULONG Mydata )
{
   if ( Mydata != 123456 )
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/**************************************************************************/
/*                                                                        */
/*  Main Application (this uses the callFFST wrapper function).           */
/*                                                                        */
/**************************************************************************/

int main ( int argc, char * argv[], char * envp  )
{
   ULONG  rc          = 0;
   ULONG  Mydata      = 2;
   ULONG  userDataLen = 0;
   PVOID  pUserData   = NULL;

   system("cls");
   printf ( "\n\n\nStarting FFSTProbe Sample\n" );

   /***********************************************************************/
   /* call the 'dummy'  API so it returns a non-zero rc                                               */
   /***********************************************************************/
   rc = My_Dummy_API ( Mydata );
   if ( rc != NO_ERROR )
   {
      /********************************************************************/
      /* The API has failed. Setup the userData to contain the failing rc */
      /********************************************************************/
      pUserData = calloc ( 2, sizeof ( ULONG ) );
      memcpy ( pUserData, &rc, sizeof ( ULONG ) );
      memcpy ( ( PBYTE ) pUserData + sizeof ( ULONG )
             , &Mydata, sizeof ( ULONG ) );


      /********************************************************************/
      /* Call the FFSTProbe wrapper function with a version of 1,       */
      /* Have ffst post the process status and enviroment variables in the syslog, */
      /* a severity of 4, a probe id of DUMMY_API_PROBE which was previously*/
      /* defined as 22222, a the logUserData equal to the failing rc (1) as   */
      /* setup above. Argc is passed in to determine whether or not data  */
      /* should be retrieved from DMI.                                    */
      /********************************************************************/
      callFFST ( 1
               , PSTAT_FLAG | PROC_ENV_FLAG
               , SEVERITY4
               , DUMMY_API_PROBE
               , "my_module_1"
               , 2 * sizeof ( ULONG )
               , pUserData
               , argc );
   }

   if (pUserData != NULL) {free(pUserData); pUserData = NULL;}

   if (argc > 1)
   {
      printf("\nFFSTProbe sample ended not using DMI component:\n\n\n");
   }
   else
   {
      printf("\nFFSTProbe sample ended using DMI component:\n\n\n");
   }

   return 0;
}

