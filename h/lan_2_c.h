/*----------------------------------------------------------------*/
/*                   ADAPTER CHECK CODES                          */
/*----------------------------------------------------------------*/

     #define   LLC_ADAPTER_INOPERATIVE                      0x8000
     #define   LLC_ILLEGAL_OP_CODE                          0x1000
     #define   LLC_LOCAL_BUS_PARITY_ERROR                   0x0800
     #define   LLC_EXTERNAL_PARITY_ERROR                    0x0400
     #define   LLC_PH_LOCAL_PARITY_ERROR                    0x0100
     #define   LLC_PH_XMIT_PARITY_ERROR                     0x0080
     #define   LLC_PH_RCV_PARITY_ERROR                      0x0040
     #define   LLC_PH_TRANSMIT_UNDERRUN                     0x0020
     #define   LLC_PH_RECEIVE_OVERRUN                       0x0010
     #define   LLC_INVALID_INTERRUPT                        0x0008


     #define   LLC_INVALID_ERROR_INTERRUPT                  0x0004
     #define   LLC_PC_INOPERATIVE                           0x0003
     #define   LLC_INVALID_SUP_REQUEST                      0x0002
     #define   LLC_PROGRAM_CHECK                            0x0001

     #define   LLC_PROGRAM_DETECTED_ERROR                   0x0100
     #define   LLC_SUP_REQUEST_INVALID                      0x0300
     #define   LLC_INVALID_TASK_ID                          0x0400
     #define   LLC_READY_TASK_REQUEST                       0x0500
     #define   LLC_INVALID_ADAPTER_NUMBER                   0x0600
     #define   LLC_INVALID_ES_VALUE                         0x0700

/*----------------------------------------------------------------*/
/*                   RING STATUS CODES                            */
/*----------------------------------------------------------------*/

     #define   LLC_SIGNAL_LOST                              0x8000
     #define   LLC_HARD_ERROR                               0x4000
     #define   LLC_SOFT_ERROR                               0x2000
     #define   LLC_TRANSMIT_BEACON                          0x1000
     #define   LLC_LOBE_WIRE_FAULT                          0x0800
     #define   LLC_AUTO_REMOVAL_ERROR                       0x0400
     #define   LLC_REMOVE_RECEIVED                          0x0100
     #define   LLC_COUNTER_OVERFLOW                         0x0080
     #define   LLC_SINGLE_STATION                           0x0040
     #define   LLC_RING_RECOVERY                            0x0020

     #define   LLC_NO_CARRIER_DETECTED                      0x8000
     #define   LLC_HOT_CARRIER_DETECTED                     0x1000
     #define   LLC_HOT_CARRIER_GENERATED                    0x0400

/*----------------------------------------------------------------*/
/*                   BRING-UP CODES                               */
/*----------------------------------------------------------------*/

     #define   LLC_TEST_COULD_NOT_RUN                       0x0020
     #define   LLC_ROM_TEST_FAILED                          0x0022
     #define   LLC_SRAM_TEST_FAILED                         0x0024
     #define   LLC_PROC_INST_TEST_FAILED                    0x0026
     #define   LLC_PROC_INTERRUPT_FAILED                    0x0028
     #define   LLC_SRAM_REG_TEST_FAILED                     0x002A
     #define   LLC_PROTOCOL_TEST_FAILED                     0x002C

     #define   LLC_PC_TIMER_FAILED                          0x0040
     #define   LLC_SRAM_WRITE_ERROR                         0x0042
     #define   LLC_SRAM_READONLY_ERROR                      0x0044
     #define   LLC_SRAM_WRITEONLY_ERROR                     0x0046
     #define   LLC_INITIALIZATION_TIMEOUT                   0x0048

/*----------------------------------------------------------------*/
/*                   OPEN ERROR CODES - TOKEN-RING                */


/*                   OPEN ERROR PHASES                            */
/*----------------------------------------------------------------*/

     #define   LLC_LOBE_MEDIA_TEST                          0x10
     #define   LLC_PHYSICAL_INSERTION                       0x20
     #define   LLC_ADDRESS_VERIFICATION                     0x30
     #define   LLC_ROLL_CALL_POLL                           0x40
     #define   LLC_REQUEST_PARAMETERS                       0x50

/*----------------------------------------------------------------*/
/*                   OPEN ERROR CODES - TOKEN-RING                */
/*                   OPEN ERROR CONDITIONS                        */
/*----------------------------------------------------------------*/

     #define   LLC_FUNCTION_FAILURE                         0x01
     #define   LLC_SIGNAL_LOSS                              0x02
     #define   LLC_WIRE_FAULT                               0x03
     #define   LLC_FREQUENCY_ERROR                          0x04
     #define   LLC_TIMEOUT                                  0x05
     #define   LLC_RING_FAILURE                             0x06
     #define   LLC_RING_BEACONING                           0x07
     #define   LLC_DUPLICATE_NODE_ADDRESS                   0x08
     #define   LLC_PARAMETER_REQUEST                        0x09
     #define   LLC_RECEIVED_REMOVE                          0x0A
     #define   LLC_IMPL_FORCE_RECEIVED                      0x0B
     #define   LLC_NO_MONITOR_DETECTED                      0x0D
     #define   LLC_MONITOR_CONTENTION_FAILED                0x0E

/*----------------------------------------------------------------*/
/*                   OPEN ERROR CODES - PC NETWORK                */
/*----------------------------------------------------------------*/

     #define   LLC_NO_CARRIER                               0x0022
     #define   LLC_HOT_CARRIER                              0x0023
     #define   LLC_UNABLE_TO_TRANSMIT                       0x0033
     #define   LLC_DUPLICATE_NODE_ADDR                      0x0038
     #define   LLC_REMOVE_FRAME_RECEIVED                    0x003A

/*----------------------------------------------------------------*/
/*                   PC DETECTED ERROR CODE/DATA                  */
/*----------------------------------------------------------------*/

     #define   LLC_UNEXPECTED_INTERRUPT                     0x0000
     #define   LLC_ACCESS_VIOLATION                         0x0001
     #define   LLC_ARB_COMMAND_ERROR                        0x01
     #define   LLC_ASB_RETURN_CODE_ERROR                    0x02
     #define   LLC_SRB_SSB_COMMAND_ERROR                    0x03
     #define   LLC_ARB_TRANSMIT_ERROR                       0x04
     #define   LLC_UNACCEPTABLE_OS_VALUE                    0x05
     #define   LLC_INTERNAL_ERROR                           0x06



/*----------------------------------------------------------------*/
/*                   SYSTEM ACTION IDS                            */
/*----------------------------------------------------------------*/

     #define   LLC_CLOSED_ADAPTER                           0x01
     #define   LLC_ADAPTER_INITIALIZED                      0x02
     #define   LLC_LOGS_READ                                0x03
     #define   LLC_FUNCTIONAL_ADDR_CHANGE                   0x04
     #define   LLC_GROUP_ADDR_CHANGE                        0x05
     #define   LLC_SAP_RESET                                0x06
     #define   LLC_ALL_SAPS_RESET                           0x07
     #define   LLC_ALL_DIRECT_STATIONS_CLOSED               0x08

/*----------------------------------------------------------------*/
/*                   DLC STATUS BITS                              */
/*----------------------------------------------------------------*/

     #define   LLC_LINK_LOST                                0x8000
     #define   LLC_DM_DISC_RECEIVED                         0x4000
     #define   LLC_FRMR_RECEIVED                            0x2000
     #define   LLC_FRMR_SENT                                0x1000
     #define   LLC_SABME_FOR_OPENED_STN                     0x0800
     #define   LLC_SABME_RCV_STN_OPENED                     0x0400
     #define   LLC_REMOTE_STN_LOCAL_BUSY                    0x0200
     #define   LLC_REMOTE_STN_READY                         0x0100
     #define   LLC_TI_TIMER_EXPIRED                         0x0080
     #define   LLC_DLC_COUNTER_OVERFLOW                     0x0040
     #define   LLC_ACCESS_PRIORITY_LOWERED                  0x0020
     #define   LLC_LOCAL_STN_LOCAL_BUSY                     0x0001

/*----------------------------------------------------------------*/
/*                   READ EVENT BIT CODES                         */
/*----------------------------------------------------------------*/

     #define   LLC_SYSTEM_ACTION_OCCURRED                   0x40
     #define   LLC_RING_NETWORK_STATUS                      0x20
     #define   LLC_CRITICAL_EXCEPTION                       0x10
     #define   LLC_DLC_STATUS_CHANGE                        0x08
     #define   LLC_RECEIVE_DATA                             0x04
     #define   LLC_TRANSMIT_COMPLETION                      0x02
     #define   LLC_COMMAND_COMPLETION                       0x01

/*----------------------------------------------------------------*/
/*                   LINK PRIMARY STATE                           */
/*----------------------------------------------------------------*/

     #define   LLC_PS_LINK_CLOSED                           0x80
     #define   LLC_PS_DISCONNECTED                          0x40
     #define   LLC_PS_DISCONNECTING                         0x20
     #define   LLC_PS_LINK_OPENING                          0x10
     #define   LLC_PS_RESETTING                             0x08
     #define   LLC_PS_FRMR_SENT                             0x04


     #define   LLC_PS_FRMR_RECEIVED                         0x02
     #define   LLC_PS_LINK_OPENED                           0x01

/*----------------------------------------------------------------*/
/*                   LINK SECONDARY STATE                         */
/*----------------------------------------------------------------*/

     #define   LLC_SS_CHECK_POINTING                        0x80
     #define   LLC_SS_LOCAL_BUSY_USER_SET                   0x40
     #define   LLC_SS_LOCAL_BUSY_BUFFER_SET                 0x20
     #define   LLC_SS_REMOTE_BUSY                           0x10
     #define   LLC_SS_REJECTION                             0x08
     #define   LLC_SS_CLEARING                              0x04
     #define   LLC_SS_DYNAMIC_WINDOW_ACTIVE                 0x02

/*----------------------------------------------------------------*/
/*                   OPEN OPTIONS BIT CODE                        */
/*----------------------------------------------------------------*/

     #define   LLC_WRAP_INTERFACE                           0x8000
     #define   LLC_DISABLE_HARD_ERRORS                      0x4000
     #define   LLC_DISABLE_SOFT_ERRORS                      0x2000
     #define   LLC_PASS_ADAPTER_MAC_FRAMES                  0x1000
     #define   LLC_PASS_ATTENTION_FRAMES                    0x0800
     #define   LLC_CONTENDER                                0x0100
     #define   LLC_PASS_BEACON_FRAMES                       0x0080
     #define   LLC_REMOTE_PROGRAM_LOAD                      0x0020
     #define   LLC_TOKEN_RELEASE                            0x0010

