/****************************************************************************************/
/* The following example demonstrates the use of the logging functions to do the        */
/* following operations.                                                                */
/* 1. Opens the default log file                                                        */
/* 2. Reads an entry from the log file using a filter.  The filter is set up so that    */
/*    a record where the DMI_VENDOR_TAG field contains IBM-PSP, or the severity field   */
/*    is greater than 2 will be read.
/* 3. Formats the record                                                                */
/* 4. Closes the log file                                                               */
/****************************************************************************************/
 
/* The following example opens the default error log file (1) */

#define INCL_LOGGING
#include <unidef.h>
#include <os2.h>
#include <stdio.h>
#include <lfdef.h>
#define STRING_BUFFER_LENGTH 4096
#define MAX_FILENAME_LENGTH 255
           
int main()
{

  /* variables used throughout */

  APIRET rc;                      /* return code */
  ULONG service;
  BYTE log_entry_buffer[2048];

  /* variables for open/close */

  UniChar filename[256];
  ULONG filename_length;
  ULONG log_file_ID;
  LOFREQUEST open_file_packet;

  /* variables for LogCloseFile */

  LCFREQUEST close_file_packet;

  /* variables for read */

  ULONG severity = 2;
  LREREQUEST read_entry_packet;
  EVENTKEY EventKey;
  ULONG log_entry_buffer_length;
  SUBBLOCK subblock1, subblock2;
  HEADERBLOCK headerblock1, headerblock2;
  FILTERBLOCK filter;
  UniChar *manufacturer_name = L"IBM-PSP";

  /* variables for format */

  LFEREQUEST format_entry_packet;
  UniChar string_buffer[4096];
  ULONG string_buffer_length;
  LocaleObject lo;
  ULONG number_of_detail_recs;

  service = ERROR_LOGGING_SERVICE;

  /**************************************************************/
  /* The following section of code opens the default log file   */
  /**************************************************************/

  log_file_ID = 1;
  filename_length = MAX_FILENAME_LENGTH;

  /* Construct the LogOpenFile parameter packet */
  open_file_packet.packet_size = sizeof(LOFREQUEST);
  open_file_packet.packet_revision_number = LF_UNI_API;
  open_file_packet.log_file_ID = &log_file_ID;
  open_file_packet.filename_length = &filename_length;
  open_file_packet.filename = &filename;
 
  rc = LogOpenFile(service, &open_file_packet);
 
  if (rc == 0)
    printf("Log File opened successfully\n");
  else {
    printf("LogOpenFile error: return code %d\n",rc);
    return rc;
  }

  /**************************************************************/
  /* The following section of code reads an entry from the file */
  /**************************************************************/

   /*  Construct an event notification filter with 2 header blocks.    */
 
   filter.packet_size = sizeof(FILTERBLOCK);
   filter.packet_revision_number = LF_UNI_API;
   filter.header_block = &headerblock1;
 
   /*-------------construct headerblock1---------------------*/
   headerblock1.pSubblock = &subblock1;
   headerblock1.pNextBlock = &headerblock2;
 
   /*construct first subblock of headerblock1*/
   subblock1.entry_attribute_ID = LOG_ERROR_DMI_VENDOR_TAG;
   subblock1.comparison_operator_ID = LOG_ERROR_EQUAL;
   subblock1.comparison_data_ptr = manufacturer_name;
   subblock1.comparison_data_length = UniStrlen(manufacturer_name)*2;
   subblock1.next_subblock = NULL;
 
   /*------------construct headerblock2----------------------*/
   headerblock2.pSubblock = &subblock2;
   headerblock2.pNextBlock = NULL;
 
   /*construct first subblock of headerblock2*/
   subblock2.entry_attribute_ID = LOG_ERROR_SEVERITY;
   subblock2.comparison_operator_ID = LOG_ERROR_GREATER_THAN;
   subblock2.comparison_data_ptr = &severity;
   subblock2.comparison_data_length = sizeof(ULONG);
   subblock2.next_subblock = NULL;
 
   /* Construct the LogReadEntry parameter packet. */
   read_entry_packet.packet_size = sizeof(LREREQUEST);
   read_entry_packet.packet_revision_number = LF_UNI_API;
   read_entry_packet.log_file_ID = *(open_file_packet.log_file_ID);
   read_entry_packet.flags = START_AT_FIRST_ENTRY;
   read_entry_packet.pEventKey = &EventKey;
   read_entry_packet.pFilter = &filter;
   log_entry_buffer_length = sizeof(log_entry_buffer);
   read_entry_packet.pLogEntryBufferLength = &log_entry_buffer_length;
   read_entry_packet.pLogEntryBuffer = &log_entry_buffer;
 
   rc = LogReadEntry(service, &read_entry_packet);
 
   if (rc == 0)
      printf("LogReadEntry was successful\n");
   else {
      printf("LogReadEntry error: return code = %d",rc);
      return rc;
   }

  /**************************************************************/
  /* The following section of code formats the log entry        */
  /**************************************************************/

   string_buffer_length = STRING_BUFFER_LENGTH;

   rc=UniCreateLocaleObject(UNI_UCS_STRING_POINTER,(UniChar*)L"",&lo);
   if (rc == 0)
      printf("Locale object was created successfully\n");
   else 
      printf("UniCreateLocaleObject error: return code = %d",rc);
 
   /* Construct the Error Log Service format packet  */
   format_entry_packet.packet_size = sizeof(LFEREQUEST);
   format_entry_packet.packet_revision_number = LF_UNI_API;
   format_entry_packet.log_entry_buffer =  &log_entry_buffer;
   format_entry_packet.flags = ERR_FORMAT_DETAIL_DATA;
   format_entry_packet.locale_object = lo;
   format_entry_packet.string_buffer_length = &string_buffer_length;
   format_entry_packet.string_buffer =  &string_buffer;
   format_entry_packet.number_of_detail_records = &number_of_detail_recs;

   rc = LogFormatEntry(service, &format_entry_packet) ;

   if (rc == 0)
     printf("LogFormatEntry was successful\n");
   else {
     printf("LogFormatEntry error: return code = %d",rc);
     return rc;
   }

  /*****************************************************/
  /* The following section of code closes the log file */
  /*****************************************************/
 
  /* Construct the LogCloseFile parameter packet  */
  close_file_packet.packet_size = sizeof(LCFREQUEST);
  close_file_packet.packet_revision_number = LF_UNI_API;
  close_file_packet.log_file_ID = *(open_file_packet.log_file_ID); /* from LogOpenFile call */
 
  rc = LogCloseFile(service, &close_file_packet);

  if (rc == 0) 
    printf("Log File closed successfully\n");
  else 
    printf("LogCloseFile error: return code %d\n",rc);

  return rc;
}
