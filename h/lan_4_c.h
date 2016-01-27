/*------------------------------------------------------------------*/
/*  EXCEPTION 802.2 DATA DECLARATIONS FOR DYNAMIC LINK ROUTINE      */
/*------------------------------------------------------------------*/
struct     exception_parms_dlr
 {
 dword      notification_flag;          /* User notification flag    */
 word       ccb_count;                  /* Count of CCBs attached    */
 address    event_ccb_pointer;          /* Pointer to CCB chain      */
 word       buffer_count;               /* Count of buffers attached */
 address    first_buffer_addr;          /* Pointer to buffer chain   */
 word       rcv_frame_count;            /* Count of frames attached  */
 address    rcv_frame_addr;             /* pointer to frame chain    */
 };


struct     adapter_check_dlr
 {
 struct     exception_parms_dlr common; /* Exception information     */
 word       reason_code;                /* Adapter check reason code */
 word       parameter_0;                /* Parameter 0 information   */
 word       parameter_1;                /* Parameter 1 information   */
 word       parameter_2;                /* Parameter 2 information   */
 };


struct     ring_network_status_dlr
 {
 struct     exception_parms_dlr common; /* Exception information     */
 word       ring_status_code;           /* Ring/Network status code  */
 };


struct     pc_detected_errors_dlr
 {
 struct     exception_parms_dlr common; /* Exception information     */
 byte       error_code;                 /* PC detected error code    */
 byte       error_data;                 /* PC detected error data    */
 byte       function_code;              /* OS function code data     */
 byte       reserved;                   /* Reserved for applicatoin  */
 word       os_return_code;             /* OS return code            */
 word       process_id;                 /* Process ID                */
 };


struct     system_action_dlr
 {
 struct     exception_parms_dlr common; /* Exception information     */
 byte       system_action_id;           /* System action ID          */
 byte       sap_station_reset;          /* SAP station affected      */
 };


/*------------------------------------------------------------------*/
/*  EXCEPTION 802.2 DATA DECLARATIONS FOR DEVICE DRIVER             */
/*------------------------------------------------------------------*/
struct     adapter_check_dd
 {
 struct     command_completion_info common;
 word       reason_code;                /* Adapter check reason code */
 word       parameter_0;                /* Parameter 0 information   */
 word       parameter_1;                /* Parameter 1 information   */
 word       parameter_2;                /* Parameter 2 information   */
 };


struct     ring_network_status_dd
 {
 struct     command_completion_info common;
 word       ring_status_code;           /* Ring/Network status code  */
 };


struct     pc_detected_errors_dd
 {
 struct     command_completion_info common;
 byte       error_code;                 /* PC detected error code    */
 byte       error_data;                 /* PC detected error data    */
 byte       function_code;              /* OS function code data     */
 byte       reserved[5];                /* RESERVED                  */
 };


struct     system_action_dd
 {
 struct     command_completion_info common;
 byte       system_action_id;           /* System action ID          */
 byte       sap_station_reset;          /* SAP station affected      */
 };


/*------------------------------------------------------------------*/
/*  DLC STATUS 802.2 DATA DECLARATION                               */
/*------------------------------------------------------------------*/
struct     dlc_status
 {
 word       station_id;                 /* Station ID for this event */
 word       dlc_status_code;            /* DLC status code           */
 byte       frmr_data[5];               /* FRaMe Reject data         */
 byte       access_priority;            /* Access priority           */
 byte       remote_node[6];             /* Remote Node               */
 byte       remote_sap;                 /* Remote SAP                */
 byte       reserved;                   /* RESERVED                  */
 word       user_stat_value;            /* User defined value        */
 };
