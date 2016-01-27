
/* Example of placing data into the trace buffer */

#include <os2.h>
#include "trace.h"

#define MAJOR_CODE 123
#define MINOR_CODE 1

int main(int argc, char *argv[])
{
  TCEREQUEST packet;
  APIRET rc;
  char data[8] = "12345678";

  packet.major_event_code = MAJOR_CODE;
  packet.event_data_length = sizeof data;
  packet.minor_event_code = MINOR_CODE;
  packet.event_data = data;

  packet.packet_size = sizeof packet;      /* Size of packet in bytes       */
  packet.packet_revision_number = TRACE_RELEASE; /* Revision level of trace */
  packet.major_event_code = MAJOR_CODE;    /* Major code event to be logged */
  packet.minor_event_code = MINOR_CODE;    /* Minor code event to be logged */
  packet.event_data_length = sizeof data;  /* Length of callers event buffer*/
  packet.event_data = data;                /* Pointer to callers buffer     */
  rc = TraceCreateEntry(&packet);
  return 0;
}

