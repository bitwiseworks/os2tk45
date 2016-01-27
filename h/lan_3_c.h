/*------------------------------------------------------------------*/
/*                GENERAL 802.2 DATA DECLARATIONS                   */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*  COMMAND CONTROL BLOCK - DYNAMIC LINK ROUTINE (DLR)              */
/*                                                                  */
/*  THIS STRUCTURE IS FOR USE WITH THE DLR INTERFACE AND MAY        */
/*  BE USED FOR ALL COMMANDS.                                       */
/*------------------------------------------------------------------*/
#ifdef E32TO16
#pragma pack (1)
#endif

struct     command_control_block_dlr
 {
 byte       ccb_adapter;                /* Adapter number            */
 byte       ccb_command;                /* Command code              */
 byte       ccb_retcode;                /* Return code               */
 byte       ccb_work;                   /* Reserved work area        */
 address    ccb_pointer;                /* CCB chain pointer         */
 dword      ccb_cmpl_flag;              /* Completion flag           */
 union                                  /* 4 bytes at offset 12      */
  {
  struct                                /* 2 at off 12,2 at off 14   */
   {
   union                                /* CCB offset 12             */
    {
#ifndef E32TO16
    word    ccb_parm_offset;            /* Parameter table offset    */
#endif
    word    station_id;                 /* Station Id                */
    word    timer_value;                /* Timer value from 1-13107  */
    } off_12;
   union                                /* CCB offset 14             */
    {
    word    ccb_parameter_1;            /* Parameter 1               */
    word    open_station_id;            /* Opened link station id    */
    word    reset_all_saps;             /* Reset all SAPs flag       */
    byte    flow_control;               /* Flow control option byte  */
    } off_14;
   } parm;
#ifdef E32TO16
  address   ccb_parm_offset;            /* Parameter table address   */
#endif
  address   functional_address;         /* Functional address        */
  address   group_address;              /* Group address             */
  address   read_ccb_addr;              /* Read CCB addr to close    */
  address   timer_ccb;                  /* Timer CCB addr to close   */
  dword     timer_cmpl_flag;            /* Timer's CCB_CMPL_FLAG     */
  } parm_1;
#ifndef E32TO16
 dword      ccb_semaphore;              /* System Semaphore handle   */
#else
 HSYSSEM _Seg16  ccb_semaphore;         /* System Semaphore handle   */
#endif
 byte       ccb_appl_id;                /* Application ID            */
 byte       ccb_read_flag;              /* READ attached indicator   */
 word       ccb_appl_key;               /* Application key           */
 union                                  /* Parm 2 field - 2 bytes    */
  {
  word      ccb_parameter_2;            /* Parameter 2               */
  word      system_key;                 /* System key                */
  } parm_2;
 };


/*------------------------------------------------------------------*/
/*  COMMAND CONTROL BLOCK - DEVICE DRIVER (DD)                      */
/*                                                                  */
/*  THIS STRUCTURE IS FOR USE WITH THE DEVICE DRIVER AND MAY        */
/*  BE USED FOR ALL COMMANDS.                                       */
/*------------------------------------------------------------------*/
struct     command_control_block_dd
 {
 byte       ccb_adapter;                /* Adapter number            */
 byte       ccb_command;                /* Command code              */
 byte       ccb_retcode;                /* Return code               */
 byte       ccb_work;                   /* Reserved work area        */
 address    ccb_pointer;                /* CCB chain pointer         */
 word       ccb_cmpl_appndg;            /* Completion appendage      */
 word       reserved1;                  /* Reserved for application  */
 union                                  /* 4 bytes at offset 12      */
  {
  struct                                /* 2 at off 12,2 at off 14   */
   {
   union                                /* CCB offset 12             */
    {
#ifndef E32TO16
    word    ccb_parm_offset;            /* Parameter table offset    */
#endif
    word    station_id;                 /* Station Id                */
    word    timer_cmpl_appndg;          /* Timer ccb cmpl appendage  */
    word    timer_value;                /* Timer value from 1-13107  */
    } off_12;
   union                                /* CCB offset 14             */
    {
    word    ccb_parameter_1;            /* Parameter 1               */
    word    open_station_id;            /* Opened link station id    */
    byte    flow_control;               /* Flow control option byte  */
    } off_14;
   } parm;
#ifdef E32TO16
  address   ccb_parm_offset;            /* Parameter table address   */
#endif
  address   functional_address;         /* Functional address        */
  address   group_address;              /* Group address             */
  address   timer_ccb;                  /* Timer ccb addr to cancel  */
  dword     timer_cmpl_flag;            /* Timer's CCB_CMPL_FLAG     */
  } parm_1;
 word       ccb_resource_id;            /* Resource id for structures*/
 word       reserved2;                  /* Reserved for application  */
 byte       ccb_appl_id;                /* Application ID            */
 byte       reserved3;                  /* Reserved for application  */
 word       ccb_appl_key;               /* Application key           */
 union                                  /* Parm 2 field - 2 bytes    */
  {
  word      ccb_parameter_2;            /* Parameter 2               */
  word      system_key;                 /* System key                */
  word      purge_resource_id;          /* Resource ID to be purged  */
  } parm_2;
 };


/*------------------------------------------------------------------*/
/*  COMMAND COMPLETION INFORMATION TABLE IS RETURNED FOR            */
/*  COMMANDS USING APPENDAGES TO POST COMPLETION OF COMMANDS.       */
/*------------------------------------------------------------------*/
struct     command_completion_info
 {
 word       ccb_count;                  /* Count of CCBs chained     */
 address    event_ccb_pointer;          /* Pointer to chain of CCBs  */
 word       buffer_count;               /* Count of buffers chained  */
 address    first_buffer_addr;          /* Pointer to buffers chained*/
 };


struct     buffer_free_parms
 {
 word       station_id;                 /* Station ID                */
 word       buffer_left;                /* Number of buffers left    */
 byte       reserved[4];                /* RESERVED                  */
 address    first_buffer;               /* Address of first buffer   */
 };


struct     buffer_get_parms
 {
 word       station_id;                 /* Station ID                */
 word       buffer_left;                /* Number of buffers left    */
 byte       buffer_get;                 /* Number of buffers to get  */
 byte       reserved[3];                /* RESERVED                  */
 address    first_buffer;               /* Address of first buffer   */
 };


struct     read_parms
 {
 word       station_id;                 /* Station ID                */
 byte       option_indicator;           /* Read option indicator     */
 byte       event_set;                  /* notification event set    */
 byte       event;                      /* Posting event             */
 byte       critical_subset;            /* Event subset identifier   */
 dword      notification_flag;          /* User notification flag    */
 word       ccb_count;                  /* Count of CCBs attached    */
 address    event_ccb_pointer;          /* Pointer to CCB chain      */
 word       buffer_count;               /* Count of buffers attached */
 address    first_buffer_addr;          /* Pointer to buffer chain   */
 word       rcv_frame_count;            /* Count of frames attached  */
 address    rcv_frame_addr;             /* pointer to frame chain    */
 word       event_error_code;           /* Exception error code      */
 word       event_error_data[3];        /* Exception error data      */
 };


struct     receive_parms
 {
 word       station_id;                 /* Station ID                */
 word       user_length;                /* Length of user space      */
 union
  {
  byte      receive_flag[4];            /* Receive user flag         */
  word      rcv_data_appndg;            /* Receive data appendage    */
  }rcvparm;
 address    first_buffer;               /* Address of first buffer   */
 byte       options;                    /* Receive options           */
 byte       reserved[3];                /* RESERVED                  */
 byte       rcv_read_option;            /* READ posting option       */
 };


struct     receive_not_contiguous
 {
 address    buf_pointer;                /* Pointer to next buffer    */
 word       rcv_len;                    /* length of entire frame    */
 word       length_in_buffer;           /* length of data in buffer  */
 word       user_offset;                /* offset to user space      */
 word       user_length;                /* Length of user space      */
 word       station_id;                 /* Receiving station ID      */
 byte       options;                    /* Receive option            */
 byte       message_type;               /* Type of message received  */
 word       buffers_left;               /* Number of buffers left    */
 byte       rcv_pcfe;                   /* Rcv physical control field*/
 byte       adapter_num;                /* Adapter number            */
 address    next_frame;                 /* Pointer to next rcv frame */
 byte       lan_header_length;          /* Length of LAN header      */
 byte       dlc_header_length;          /* Length of DLC header      */
 byte       lan_header[32];             /* LAN header of rcv frame   */
 byte       dlc_header[4];              /* DLC header of rcv frame   */
 };


struct     receive_contiguous
 {
 address    buf_pointer;                /* Pointer to next buffer    */
 word       rcv_len;                    /* length of entire frame    */
 word       length_in_buffer;           /* length of data in buffer  */
 word       user_offset;                /* offset to user space      */
 word       user_length;                /* Length of user space      */
 word       station_id;                 /* Receiving station ID      */
 byte       options;                    /* Receive option            */
 byte       message_type;               /* Type of message received  */
 word       buffers_left;               /* Number of buffers left    */
 byte       rcv_pcfe;                   /* Rcv physical control field*/
 byte       adapter_num;                /* Adapter number            */
 address    next_frame;                 /* Pointer to next rcv frame */
 };


struct     receive_buffer_two
 {
 address    buf_pointer;                /* Pointer to next buffer    */
 word       rcv_len;                    /* length of entire frame    */
 word       length_in_buffer;           /* length of data in buffer  */
 word       user_offset;                /* offset to user space      */
 word       user_length;                /* Length of user space      */
 };


struct     receive_modify_parms
 {
 word       station_id;                 /* Station ID                */
 word       user_length;                /* Length of user data       */
 word       rcv_data_appndg;            /* Receive data appendage    */
 word       reserved1;                  /* Reserved for application  */
 address    first_buffer;               /* Address of first buffer   */
 word       subroutine_addr;            /* Subroutine offset         */
 word       reserved2;                  /* Reserved for application  */
 };


struct     transmit_parms
 {
 word       station_id;                 /* Station ID                */
 byte       transmit_pcfe;              /* Transmit PCFE information */
 byte       rsap;                       /* Remote SAP value          */
 address    xmit_queue_one;             /* Address of transmit queue */
 address    xmit_queue_two;             /* Address of transmit queue */
 word       buffer_len_one;             /* Length of buffer one      */
 word       buffer_len_two;             /* Length of buffer two      */
 address    buffer_one;                 /* Transmit buffer one       */
 address    buffer_two;                 /* Transmit buffer two       */
 byte       xmit_read_option;           /* READ posting option       */
 };


struct     transmit_buffer
 {
 address    next_buf_pointer;           /* Pointer to next buffer    */
 word       reserved;                   /* Reserved for application  */
 word       length_in_buffer;           /* Length of data in buffer  */
 word       user_data;                  /* User data                 */
 word       user_length;                /* Length of user space      */
 };


struct     dir_initialize_parms
 {
 word       bring_ups;                  /* Bring-up error code       */
 word       sram_address;               /* Configured addr of SRAM   */
 byte       sram_page;                  /* Configured SRAM meg page  */
 byte       reserved[15];               /* RESERVED                  */
 address    shared_ram_address;         /* Virtual address of SRAM   */
 address    mmio_address;               /* Virtual address of MMIO   */
 address    data_seg_address;           /* Virtual address of DS     */
 };

#ifndef E32TO16
struct     dir_open_adapter_parms
 {
 word       adapter_parms_offset;       /* Offset to adapter parms   */
 word       reserved1;                  /* Reserved for application  */
 byte       reserved2[4];               /* RESERVED                  */
 word       dlc_parms_offset;           /* Offset to DLC parms       */
 word       reserved3;                  /* Reserved for application  */
 byte       reserved4[4];               /* RESERVED                  */
 };
#else
struct     dir_open_adapter_parms
 {
 address    adapter_parms_offset;       /* Address of adapter parms  */
 byte       reserved2[4];               /* RESERVED                  */
 address    dlc_parms_offset;           /* Address of DLC parms      */
 byte       reserved4[4];               /* RESERVED                  */
 };
#endif

struct     dir_open_ad_parms
 {
 word       open_error_code;            /* Open adapter errors       */
 word       open_options;               /* Open options              */
 byte       node_address[6];            /* Station's Node address    */
 address    group_address;              /* Station's Group address   */
 byte       functional_address[4];      /* Station's Functional addr */
 word       num_rcv_buffers;            /* Number of receive buffers */
 word       rcv_buffer_len;             /* Length of receive buffers */
 word       dhb_buffer_len;             /* Length of DHB buffers     */
 byte       data_hold_buffers;          /* Number of DHB buffers     */
 byte       reserved1[3];               /* RESERVED                  */
#ifndef E32TO16
 word       product_id_offset;          /* Offset to product ID      */
 word       reserved2;                  /* Reserved for application  */
#else
 address    product_id_offset;          /* Address of product ID     */
#endif
 word       bring_ups;                  /* Bring-up errors           */
 word       init_warnings;              /* Initialization warnings   */
 word       semaphore_count;            /* Number of semaphores      */
 address    sys_semaphore_table;        /* Addr to semaphore table   */
 byte       ddname[8];                  /* Application's DD name     */
 };


struct     dir_open_dlc_parms
 {
 byte       dlc_max_sap;                /* Maximum SAPs              */
 byte       dlc_max_station;            /* Maximum link stations     */
 byte       dlc_max_gsap;               /* Maximum group SAPs        */
 byte       dlc_max_gsap_mem;           /* Maximum Group SAP members */
 byte       dlc_t1_tick_one;            /* T1 timer interval, group1 */
 byte       dlc_t2_tick_one;            /* T2 timer interval, group1 */
 byte       dlc_ti_tick_one;            /* TI timer interval, group1 */
 byte       dlc_t1_tick_two;            /* T1 timer interval, group2 */
 byte       dlc_t2_tick_two;            /* T2 timer interval, group2 */
 byte       dlc_ti_tick_two;            /* Ti timer interval, group2 */
 };


struct     dir_open_direct_parms
 {
 word       dir_buf_size;               /* Size of buffers in pool   */
 word       dir_pool_length;            /* Length of buffer pool     */
 address    dir_pool_address;           /* Address of buffer pool    */
 word       open_options;               /* Open options              */
 };


struct     dir_read_log_parms
 {
 word       log_id;                     /* Identify log to read      */
 word       log_buf_length;             /* Size of log buffer        */
#ifndef E32TO16
 word       log_buf_offset;             /* Offset of log buffer      */
 word       reserved;                   /* Reserved for application  */
#else
 address    log_buf_offset;             /* Address of log buffer     */
#endif
 word       log_act_length;             /* Actual length of log      */
 };


struct     dir_read_log_tokenring
 {
 byte       line_errors;                /* Line error count          */
 byte       internal_errors;            /* Internal error count      */
 byte       burst_errors;               /* Burst error count         */
 byte       ari_fci_delimiter;          /* ARI/FCI delimiter         */
 byte       abort_delimiter;            /* Abort delimiter           */
 byte       reserved1;                  /* RESERVED                  */
 byte       lost_frames;                /* Lost frame count          */
 byte       receive_congestion;         /* Receive congestion count  */
 byte       frame_copied_errors;        /* Frame copied error count  */
 byte       frequency_errors;           /* Frequency error count     */
 byte       token_errors;               /* Token-Ring error count    */
 byte       reserved2[3];               /* RESERVED                  */
 };


struct     dir_read_log_pcnetwork
 {
 byte       crc_errors;                 /* CRC error count           */
 byte       reserved1;                  /* RESERVED                  */
 byte       alignment_errors;           /* Alignment error count     */
 byte       reserved2;                  /* RESERVED                  */
 byte       transmit_errors;            /* Transmit error count      */
 byte       reserved3;                  /* RESERVED                  */
 byte       collision_errors;           /* Collision error count     */
 byte       rcv_congestion_errors;      /* Receive congestion count  */
 byte       reserved4[6];               /* RESERVED                  */
 };


struct     dir_set_exception_parms_dlr
 {
 dword      adapter_check_flag;         /* Adapter check flag        */
 dword      network_status_flag;        /* Ring/Network status flag  */
 dword      pc_error_flag;              /* PC-detected error flag    */
 dword      system_action_flag;         /* System action flag        */
 };


struct     dir_set_exception_parms_dd
 {
 word       adapter_check_appndg;       /* Adapter check appendage   */
 word       reserved1;                  /* Reserved for apllication  */
 word       network_status_appndg;      /* Ring/Network status appndg*/
 word       reserved2;                  /* Reserved for apllication  */
 word       pc_error_appndg;            /* PC-detected error appndg  */
 word       reserved3;                  /* Reserved for apllication  */
 word       system_action_appndg;       /* System action appendage   */
 word       reserved4;                  /* Reserved for apllication  */
 };


struct     dir_status_parms
 {
 byte       burned_in_addr[6];          /* Adapter's burned in addr  */
 byte       node_address[6];            /* Station's Node  address   */
 byte       group_address[4];           /* Station's Group address   */
 byte       function_address[4];        /* Station's Functional addr */
 byte       max_sap;                    /* Maximum SAPs              */
 byte       open_sap;                   /* Number of opened SAPs     */
 byte       max_station;                /* Maximum link stations     */
 byte       open_station;               /* Number of opened link stn */
 byte       avail_station;              /* Available link stations   */
 byte       adapter_config;             /* Adapter configuration     */
 byte       microcode_level[10];        /* Microcode level           */
 address    adapter_parms_addr;         /* Address of adapter parms  */
 address    adapter_mac_addr;           /* Address of adapter MAC    */
 address    tick_counter_addr;          /* Address of tick counter   */
 word       last_ring_status;           /* Last Ring/Network status  */
 word       adapter_type;               /* Adapter type              */
 };


struct     dir_status_tokenring
 {
 byte       phys_addr[4];               /* Adapter physical address  */
 byte       up_node_addr[6];            /* Upstream node address     */
 byte       up_phys_addr[4];            /* Upstream physical address */
 byte       poll_addr[6];               /* Last poll address         */
 byte       auth_env[2];                /* Authorized environment    */
 byte       acc_priority[2];            /* Transmit access priority  */
 byte       source_class[2];            /* Source class authorization*/
 byte       att_code[2];                /* Last attention code       */
 byte       source_addr[6];             /* Last source address       */
 byte       beacon_type[2];             /* Last beacon type          */
 byte       major_vector[2];            /* Last major vector         */
 byte       ring_status[2];             /* Ring status               */
 byte       soft_error[2];              /* Soft error timer value    */
 byte       fe_error[2];                /* Front End error count     */
 byte       local_ring[2];              /* Local ring number         */
 byte       mon_error[2];               /* Monitor error code        */
 byte       beacon_transmit[2];         /* Beacon transmit type      */
 byte       beacon_receive[2];          /* Beacon receive type       */
 byte       frame_correl[2];            /* Frame correlation save    */
 byte       beacon_naun[6];             /* Beaconing station NAUN    */
 byte       reserved[4];                /* RESERVED                  */
 byte       beacon_phys[4];             /* Beaconing stn's phys addr */
 };


struct     dir_status_pcnetwork
 {
 byte       reserved1[28];              /* RESERVED                  */
 byte       source_addr[6];             /* Last source address       */
 byte       reserved2[2];               /* RESERVED                  */
 byte       major_vector[2];            /* Last major vector         */
 byte       network_status[2];          /* Network status            */
 byte       report_error[2];            /* Error report timer value  */
 byte       report_error_tick[2];       /* Error timer tick counter  */
 byte       local_bus_number[2];        /* Local bus number          */
 byte       reserved3[6];               /* RESERVED                  */
 byte       frame_correl[2];            /* Frame correlation save    */
 byte       reserved4[6];               /* RESERVED                  */
 word       network_samples;            /* Number of samples         */
 word       network_busy;               /* Number of busy samples    */
 byte       reserved5[4];               /* RESERVED                  */
 };


struct     dlc_connect_station_parms
 {
 word       station_id;                 /* Station ID                */
 byte       reserved1[2];               /* RESERVED                  */
#ifndef E32TO16
 word       routing_offset;             /* Offset to routing info    */
 word       reserved2;                  /* Reserved for application  */
#else
 address    routing_offset;             /* Address of routing info   */
#endif
 };


struct     dlc_modify_parms
 {
 byte       reserved1[2];               /* RESERVED                  */
 word       station_id;                 /* SAP or Link Station ID    */
 byte       timer_t1;                   /* T1 timer value            */
 byte       timer_t2;                   /* T2 timer value            */
 byte       timer_ti;                   /* Ti timer value            */
 byte       maxout;                     /* Max transmits without ACK */
 byte       maxin;                      /* Max receives without ACK  */
 byte       maxout_incr;                /* Dynamic window increment  */
 byte       max_retry_count;            /* Maximum retry count       */
 byte       reserved2[3];               /* RESERVED                  */
 byte       access_priority;            /* Ring access priority      */
 byte       reserved3[4];               /* RESERVED                  */
 byte       group_count;                /* Length of group list data */
#ifndef E32TO16
 word       group_list_offset;          /* Offset to group list      */
 word       reserved4;                  /* Reserved for application  */
#else
 address    group_list_offset;          /* Address of group list     */
#endif
 };


struct     dlc_open_sap_parms
 {
 word       station_id;                 /* SAP station ID            */
 word       user_stat_value;            /* User value for DLC Status */
 byte       timer_t1;                   /* Ti timer value            */
 byte       timer_t2;                   /* T2 timer value            */
 byte       timer_ti;                   /* Ti timer value            */
 byte       maxout;                     /* Max transmits without ACK */
 byte       maxin;                      /* Max receives without ACK  */
 byte       maxout_incr;                /* Dynamic window increment  */
 byte       max_retry_count;            /* Maximum retry count       */
 byte       max_members;                /* Max SAPs for group list   */
 word       max_i_field;                /* Max rcv data for I frames */
 byte       sap_value;                  /* SAP value to be assigned  */
 byte       options_priority;           /* SAP options/ring priority */
 byte       sap_station_count;          /* Requested number of stns  */
 byte       reserved1[2];               /* RESERVED                  */
 byte       group_count;                /* Length of group list data */
#ifndef E32TO16
 word       group_list_offset;          /* Offset of group list      */
 word       reserved2;                  /* Reserved for application  */
#else
 address    group_list_offset;          /* Address of group list     */
#endif
 union
  {
  address   dlc_status_flag;            /* DLC status user flag      */
  word      dlc_status_appndg;          /* DLC status user appendage */
  } status;
 word       dlc_buf_size;               /* Size of buffers in pool   */
 word       dlc_pool_len;               /* Length of buffer pool     */
 address    dlc_pool_address;           /* Address of buffer pool    */
 byte       adapter_available_stns;     /* Number of stns available  */
 };


struct     dlc_open_station_parms
 {
 word       station_id;                 /* SAP station ID            */
 word       link_station_id;            /* Link station ID assigned  */
 byte       timer_t1;                   /* Ti timer value            */
 byte       timer_t2;                   /* T2 timer value            */
 byte       timer_ti;                   /* Ti timer value            */
 byte       maxout;                     /* Max transmits without ACK */
 byte       maxin;                      /* Max receives without ACK  */
 byte       maxout_incr;                /* Dynamic window increment  */
 byte       max_retry_cnt;              /* Maximum retry count       */
 byte       rsap_value;                 /* Remote SAP value          */
 word       max_i_field;                /* Max rcv data for I frames */
 byte       access_priority;            /* Ring access priority      */
 byte       reserved1;                  /* RESERVED                  */
#ifndef E32TO16
 word       destination_offset;         /* Offset to remote dest add */
 word       reserved2;                  /* Reserved for application  */
#else
 address    destination_offset;         /* Address of remote dest add*/
#endif
 };


struct     dlc_reallocate_stns_parms
 {
 word       station_id;                 /* SAP station ID            */
 byte       option_byte;                /* Increase/decrease flag    */
 byte       station_count;              /* Number of link stations   */
 byte       adapter_available_stns;     /* Adapter: available stns   */
 byte       sap_available_stns;         /* SAP: available stns       */
 byte       adapter_station_count;      /* Adapter: configured stns  */
 byte       sap_station_count;          /* SAP: configured stns      */
 };


struct     dlc_set_threshold_parms
 {
 word       station_id;                 /* SAP station ID            */
 word       buffer_threshold;           /* Buffer threshold number   */
#ifndef E32TO16
 dword      alert_semaphore;            /* System Semaphore handle   */
#else
 HSYSSEM _Seg16   alert_semaphore;      /* System semaphore handle   */
#endif
 };


struct     dlc_statistics_parms
 {
 word       station_id;                 /* Identify log to read      */
 word       log_buf_length;             /* Size of log buffer        */
#ifndef E32TO16
 word       log_buf_offset;             /* Offset to log buffer      */
 word       reserved;                   /* Reserved for application  */
#else
 address    log_buf_offset;             /* Address of log buffer     */
#endif
 word       log_act_length;             /* Actual length of log      */
 byte       options;                    /* Command options           */
 };


struct     sap_station_log
 {
 dword      frames_transmitted;         /* Frames transmitted count  */
 dword      frames_received;            /* Frames received count     */
 dword      frames_discarded;           /* Frames discarded count    */
 dword      lost_data;                  /* Lost data count           */
 word       available_buffers;          /* Number of buffers left    */
 };


struct     link_station_log
 {
 word       iframes_transmitted;        /* Iframes transmit count    */
 word       iframes_received;           /* Iframes received count    */
 byte       iframes_receive_errors;     /* Iframes received error    */
 byte       iframes_transmit_errors;    /* Iframes transmit errors   */
 word       ti_expired_count;           /* Ti expiration counts      */
 byte       last_cmnd_resp_rcved;       /* Last command received     */
 byte       last_cmnd_resp_sent;        /* Last command sent         */
 byte       link_primary_state;         /* Link primary state        */
 byte       link_secondary_state;       /* Link secondary state      */
 byte       send_state_variable;        /* Send state variable       */
 byte       receive_state_variable;     /* Receive state variable    */
 byte       last_received_nr;           /* Last Received NR value    */
 byte       lan_header_length;          /* Length of LAN header      */
 };


struct     dlc_header_iframe
 {
 byte       dsap;                       /* Destination SAP           */
 byte       ssap;                       /* Source SAP                */
 byte       control[2];                 /* Control information       */
 };


struct     dlc_header_non_iframe
 {
 byte       dsap;                       /* Destination SAP           */
 byte       ssap;                       /* Source SAP                */
 byte       control;                    /* Control information       */
 };


struct     lan_header
 {
 byte       pcf0;                       /* Physical control field 0  */
 byte       pcf1;                       /* Physical control field 1  */
 byte       dest_addr[6];               /* Destination address       */
 byte       source_addr[6];             /* Source address            */
 byte       routing_info_header[2];     /* Routing information hdr   */
 };


struct     lan_envelope_start
 {
 byte       s_del;                      /* Starting delimiter        */
 struct    lan_header common;           /* LAN header information    */
 };


struct     lan_envelope_end
 {
 byte       fcs[4];                     /* Frame Check Sequence      */
 byte       e_del;                      /* End delimiter             */
 byte       pcf_e;                      /* PCF extension             */
 };
