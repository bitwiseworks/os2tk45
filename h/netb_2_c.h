/*------------------------------------------------------------------*/
/*               NETBIOS NCB DATA DECLARATIONS                      */
/*------------------------------------------------------------------*/

struct     network_control_block
 {
  byte       ncb_command;               /* Netbios command code      */
  byte       ncb_retcode;               /* Return code               */
  byte       ncb_lsn;                   /* Local session number      */
  byte       ncb_num;                   /* Number of application name*/
  address    ncb_buffer_address;        /* Address of message buffer */
  word       ncb_length;                /* length of message buffer  */
  byte       ncb_callname[16];          /* Destination name          */
  byte       ncb_name[16];              /* Source name               */
  byte       ncb_rto;                   /* Receive timeout           */
  byte       ncb_sto;                   /* Send timeout              */
  union                                 /* Offset 44 parameters      */
   {
   address   ncb_post_address;          /* Address of post routine   */
   struct
    {
    word     ncb_post_addr_offset;      /* Offset of post routine    */
    word     ncb_dd_id;                 /* Device driver ID          */
    } DD;
   } off44;
  byte       ncb_lana_num;              /* Adapter number            */
  byte       ncb_cmd_cplt;              /* Command status            */
  byte       ncb_reserve[14];           /* Reserved (except RESET)   */
 };


struct     ncb_status_information
 {
  byte       burned_in_addr[6];         /* Adapter's burned in addr  */
  byte       reserved1[2];              /* RESERVED always X'0000'   */
  word       software_level_number;     /* X'FFnn' - nn is level num */
  word       reporting_period;          /* reporting period (minutes)*/
  word       frmr_frames_received;      /* Number of FRMR received   */
  word       frmr_frames_sent;          /* Number of FRMR sent       */
  word       bad_iframes_received;      /* # bad Iframes received    */
  word       aborted_transmissions;     /* # aborted transmits       */
  dword      packets_transmitted;       /* # Successfully transmitted*/
  dword      packets_received;          /* # Successfully received   */
  word       bad_iframes_transmitted;   /* # bad Iframes transmitted */
  word       lost_data_count;           /* Lost SAP buffer data cnt  */
  word       t1_expiration_count;       /* Number of T1 expirations  */
  word       ti_expiration_count;       /* Number of Ti expirations  */
  address    extended_status_table;     /* Address of extended status*/
  word       number_of_free_ncbs;       /* Number of NCBs available  */
  word       max_configured_ncbs;       /* Configured NCB maximum    */
  word       max_allowed_ncbs;          /* Maximum NCBs (always 255) */
  word       busy_condition_count;      /* Local station busy count  */
  word       max_datagram_size;         /* Maximum datagram packet   */
  word       pending_sessions;          /* Number of pending sessions*/
  word       max_configured_sessions;   /* Configured session maximum*/
  word       max_allowed_sessions;      /* Maximum sessions (254)    */
  word       max_data_packet_size;      /* Maximum session packet    */
  word       number_of_names_present;   /* Number of names in table  */
 };


struct     ncb_extended_status
 {
  byte       reserved[40];              /* RESERVED                  */
  byte       local_adapter_address[6];  /* Adapter's local address   */
 };


struct     ncb_session_status
 {
  byte       name_number_of_sessions;   /* Name number for sessions  */
  byte       sessions_using_name;       /* # of sessions using name  */
  byte       active_rcv_datagrams;      /* # of receive datagrams out*/
  byte       active_receive_anys;       /* # of RECEIVE.ANY cmnds out*/
  byte       local_session_number;      /* Local session number      */
  byte       session_state;             /* State of session          */
  byte       local_name[16];            /* Local name                */
  byte       remote_name[16];           /* Remote name               */
  byte       active_receives;           /* # of RECEIVE cmnds out    */
  byte       active_sends;              /* # of SEND, CHAIN.SEND out */
 };


struct     ncb_find_name_info
 {
  word       nodes_responding;          /* Number of nodes responding*/
  byte       reserved;                  /* RESERVED                  */
  byte       name_status;               /* Unique/Group name flag    */
 };


struct     ncb_lan_header_entry
 {
  byte       lan_entry_length;          /* Length of entry           */
  byte       lan_pcf0;                  /* Physical control field 0  */
  byte       lan_pcf1;                  /* Physical control field 1  */
  byte       lan_destination_addr[6];   /* Destination address       */
  byte       lan_source_addr[6];        /* Source address            */
  byte       lan_routing_info[18];      /* Routing information       */
 };

struct     ncb_chain_send
 {
  byte       ncb_command;               /* Netbios command code      */
  byte       ncb_retcode;               /* Return code               */
  byte       ncb_lsn;                   /* Local session number      */
  byte       not_used1;                 /* Not used                  */
  address    ncb_buffer_address;        /* Address of message buffer */
  word       ncb_length;                /* Length of message buffer  */
  word       buffer_two_length;         /* Length of second buffer   */
  address    buffer_two_address;        /* Address to second buffer  */
  byte       reserved[10];              /* RESERVED                  */
  byte       not_used2[18];             /* Not used                  */
  union                                 /* Offset 44 parameters      */
   {
   address   ncb_post_address;          /* Address of post routine   */
   struct
    {
    word     ncb_post_addr_offset;      /* Offset of post routine    */
    word     ncb_dd_id;                 /* Device driver ID          */
    } DD;
   } off44 ;
  byte       ncb_lana_num;              /* Adapter number            */
  byte       ncb_cmd_cplt;              /* Command status            */
  byte       ncb_reserve[14];           /* Reserved                  */
 };


struct     ncb_reset
 {
  byte       ncb_command;               /* Netbios command code      */
  byte       ncb_retcode;               /* Return code               */
  byte       ncb_lsn;                   /* Local session number      */
  byte       ncb_num;                   /* Number of application name*/
  address    dd_name_address;           /* Device drive name address */
  byte       not_used1[2];              /* Not used                  */
  byte       req_sessions;              /* # of sessions requested   */
  byte       req_commands;              /* # of commands requested   */
  byte       req_names;                 /* # of names requested      */
  byte       req_name_one;              /* Name number one request   */
  byte       not_used2[12];             /* Not used                  */
  byte       act_sessions;              /* # of sessions obtained    */
  byte       act_commands;              /* # of commands obtained    */
  byte       act_names;                 /* # of names obtained       */
  byte       act_name_one;              /* Name number one response  */
  byte       not_used3[4];              /* Not used                  */
  byte       load_session;              /* Number of sessions at load*/
  byte       load_commands;             /* Number of commands at load*/
  byte       load_names;                /* Number of names at load   */
  byte       load_stations;             /* Number of stations at load*/
  byte       not_used4[2];              /* Not used                  */
  byte       load_remote_names;         /* Number of remote names    */
  byte       not_used5[5];              /* Not used                  */
  word       ncb_dd_id;                 /* NCB device driver ID      */
  byte       ncb_lana_num;              /* Adapter number            */
  byte       not_used6;                 /* Not used                  */
  byte       ncb_reserve[14];           /* NCB error information     */
 };
