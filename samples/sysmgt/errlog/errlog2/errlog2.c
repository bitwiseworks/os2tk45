/****************************************************************************************/
/* The following example demonstrates the use of the event notification functions.      */
/* This program does the following things:                                              */
/* 1. Opens event notification without specifying a filter                              */
/* 2. Calls LogWaitEvent which will cause the program to wait until an entry is put     */
/*    in the log.  Since a filter was not specified, any entry put in the log will      */
/*    trigger this event.  You can use the FFSTProbe example program to put an          */
/*    entry in the log.                                                                 */
/* 3. Issues a LogChangeEventFilter to filter on records where vendor tag = IBM-PSP or  */
/*    severity > 2.                                                                     */
/* 4. Issues another LogWaitEvent call to wait for an entry being put in the log based  */
/*    on the new filter criteria.  Again, you can use the FFSTProbe sample program to   */
/*    place an entry in the log which will satisfy the filter criteria.                 */
/* 5. Closes event notification for this program                                        */
/****************************************************************************************/

#define INCL_LOGGING
#include <unidef.h>
#include <os2.h>
#include <stdio.h>
#include <lfdef.h>
#define ERROR_LOG_FILE_ID 1
#define BUFFER_LENGTH 4096
#define SAMPLE_SEVERITY 2
                 
int main()
{

 APIRET rc;
 ULONG service;
 ULONG notification_ID;

 /* LogOpenEventNotification variables */
 ULONG severity = SAMPLE_SEVERITY;
 LOENREQUEST open_event_packet;
 UniChar manufacturer_name[8] = L"IBM-PSP";

 /* variables for LogCloseEventNotification */
 LCENREQUEST close_event_packet;

 /* variables for LogWaitEvent */
 LWEREQUEST log_wait_event_packet;
 EVENTKEY EventKey;
 BYTE  log_entry_buffer[BUFFER_LENGTH];
 ULONG log_entry_buffer_length = BUFFER_LENGTH;
 UniChar pathname[512];
 ULONG pathname_length = sizeof(pathname);

 /* variables for LogChangeEventFilter */
 LCEFREQUEST change_event_filter_packet;
 SUBBLOCK subblock1, subblock2;
 HEADERBLOCK headerblock1, headerblock2;
 FILTERBLOCK filter;

 service = ERROR_LOGGING_SERVICE;

 /**************************************************************/
 /* OpenEventNotification                                      */
 /**************************************************************/
 
 /* Construct the LogOpenEventNotification parameter packet.*/
 open_event_packet.packet_size = sizeof(LOENREQUEST);
 open_event_packet.packet_revision_number = LF_UNI_API;
 open_event_packet.log_file_ID = ERROR_LOG_FILE_ID;
 open_event_packet.pLogNotify = &notification_ID;
 open_event_packet.pFilter = NULL;   /* This will cause a notification on all records */
 open_event_packet.read_flags = 0;
 
 rc = LogOpenEventNotification(service, &open_event_packet);
 
 if (rc == 0)
   printf("LogOpenEventNotification was successful\n");
 else {
   printf("LogOpenEventNotification error: return code = %d",rc);
   return rc;
 }

 /**************************************************************/
 /* LogWaitEvent                                               */
 /**************************************************************/

 /* Construct the LogWaitEvent parameter packet  */
 log_wait_event_packet.packet_size = sizeof(LWEREQUEST);
 log_wait_event_packet.packet_revision_number = LF_UNI_API;
 log_wait_event_packet.LogNotify = notification_ID;
 log_wait_event_packet.pEventKey  = &EventKey;
 log_wait_event_packet.pLogEntryBuffer = &log_entry_buffer;
 log_wait_event_packet.log_entry_buffer_length = &log_entry_buffer_length;
 log_wait_event_packet.timeout = 0;
 log_wait_event_packet.queue_flags = 0;
 log_wait_event_packet.pathname_length = &pathname_length;
 log_wait_event_packet.pathname = pathname;
 
 printf("Waiting for an event now\n");
 
 rc = LogWaitEvent(service, &log_wait_event_packet);

 if (rc == 0)
   printf("LogWaitEvent detected an event\n");
 else {
   printf("LogWaitEvent error: return code = %d",rc);
   return rc;
 }

 /**************************************************************/
 /* LogChangeEventFilter                                       */
 /**************************************************************/

 /*  Construct an event notification filter  */
 
 filter.packet_size = sizeof(FILTERBLOCK);
 filter.packet_revision_number = LF_UNI_API;
 filter.header_block = &headerblock1;
 
 /*-------------construct headerblock1---------------------*/
 headerblock1.pSubblock = &subblock1;
 headerblock1.pNextBlock = &headerblock2;
 
 /*construct subblock1 of headerblock1*/
 subblock1.entry_attribute_ID = LOG_ERROR_DMI_VENDOR_TAG;
 subblock1.comparison_operator_ID = LOG_ERROR_EQUAL;
 subblock1.comparison_data_ptr = &manufacturer_name;
 subblock1.comparison_data_length = UniStrlen(manufacturer_name)*2;
 subblock1.next_subblock = NULL;
 
 /*------------construct headerblock2----------------------*/
 headerblock2.pSubblock = &subblock2;
 headerblock2.pNextBlock = NULL;
 
 /*construct subblock2 of headerblock2*/
 subblock2.entry_attribute_ID = LOG_ERROR_SEVERITY;
 subblock2.comparison_operator_ID = LOG_ERROR_GREATER_THAN;
 subblock2.comparison_data_ptr = &severity;
 subblock2.comparison_data_length = sizeof(ULONG);
 subblock2.next_subblock = NULL;

 /* Construct the LogChangeEventFilter parameter packet  */
 change_event_filter_packet.packet_size = sizeof(LCEFREQUEST);
 change_event_filter_packet.packet_revision_number = LF_UNI_API;
 change_event_filter_packet.purge_flags = PURGE_EVENT_NOTIFICATION;
 change_event_filter_packet.LogNotify = notification_ID;
 change_event_filter_packet.pFilter = &filter;
 
 rc = LogChangeEventFilter(service, &change_event_filter_packet);

 if (rc == 0)
   printf("LogChangeEventFilter completed successfully\n");
 else {
   printf("LogChangeEventFilter error: return code = %d",rc);
   return rc;
 }

 /**************************************************************/
 /* LogWaitEvent using new filter                              */
 /**************************************************************/

 /* Construct the LogWaitEvent parameter packet  */
 log_wait_event_packet.packet_size = sizeof(LWEREQUEST);
 log_wait_event_packet.packet_revision_number = LF_UNI_API;
 log_wait_event_packet.LogNotify = notification_ID;
 log_wait_event_packet.pEventKey  = &EventKey;
 log_wait_event_packet.pLogEntryBuffer = &log_entry_buffer;
 log_wait_event_packet.timeout = 0;         /* wait indefinately */
 log_wait_event_packet.queue_flags = 0;
 log_wait_event_packet.pathname_length = &pathname_length;
 log_wait_event_packet.pathname = pathname;
 
 printf("Waiting for an event now\n");
 
 rc = LogWaitEvent(service, &log_wait_event_packet);

 if (rc == 0)
   printf("LogWaitEvent detected an event\n");
 else {
   printf("LogWaitEvent error: return code = %d",rc);
   return rc;
 }

 /**************************************************************/
 /* LogCloseEventNotification                                  */
 /**************************************************************/

 /* Construct the LogCloseEventNotification parameter packet  */
 close_event_packet.packet_size = sizeof(LCENREQUEST);
 close_event_packet.packet_revision_number = LF_UNI_API;
 close_event_packet.LogNotify = notification_ID;

 rc =  LogCloseEventNotification(service, &close_event_packet);

 if (rc == 0)
   printf("LogCloseEventNotification was successful\n");
 else 
   printf("LogCloseEventNotification error: return code = %d",rc);

 return rc;
}
