/*----------------------------------------------------------------*/
/*                       NETBIOS COMMAND CODES                    */
/*----------------------------------------------------------------*/

     #define   NB_CALL_WAIT                                 0x0010
     #define   NB_LISTEN_WAIT                               0x0011
     #define   NB_HANG_UP_WAIT                              0x0012
     #define   NB_SEND_WAIT                                 0x0014
     #define   NB_RECEIVE_WAIT                              0x0015
     #define   NB_RECEIVE_ANY_WAIT                          0x0016
     #define   NB_CHAIN_SEND_WAIT                           0x0017
     #define   NB_SEND_DATAGRAM_WAIT                        0x0020
     #define   NB_RECEIVE_DATAGRAM_WAIT                     0x0021
     #define   NB_SEND_BROADCAST_DATAGRAM_WAIT              0x0022
     #define   NB_RECEIVE_BROADCAST_DATAGRAM_W              0x0023
     #define   NB_ADD_NAME_WAIT                             0x0030
     #define   NB_DELETE_NAME_WAIT                          0x0031
     #define   NB_RESET_WAIT                                0x0032
     #define   NB_STATUS_WAIT                               0x0033
     #define   NB_SESSION_STATUS_WAIT                       0x0034
     #define   NB_CANCEL_WAIT                               0x0035
     #define   NB_ADD_GROUP_NAME_WAIT                       0x0036
     #define   NB_UNLINK_WAIT                               0x0070
     #define   NB_SEND_NO_ACK_WAIT                          0x0071
     #define   NB_CHAIN_SEND_NO_ACK_WAIT                    0x0072
     #define   NB_FIND_NAMEWAIT                             0x0078

     #define   NB_CALL                                      0x0090
     #define   NB_LISTEN                                    0x0091
     #define   NB_HANG_UP                                   0x0092
     #define   NB_SEND                                      0x0094
     #define   NB_RECEIVE                                   0x0095
     #define   NB_RECEIVE_ANY                               0x0096
     #define   NB_CHAIN_SEND                                0x0097
     #define   NB_SEND_DATAGRAM                             0x00A0
     #define   NB_RECEIVE_DATAGRAM                          0x00A1
     #define   NB_SEND_BROADCAST_DATAGRAM                   0x00A2
     #define   NB_RECEIVE_BROADCAST_DATAGRAM                0x00A3
     #define   NB_ADD_NAME                                  0x00B0
     #define   NB_DELETE_NAME                               0x00B1
     #define   NB_STATUS                                    0x00B3
     #define   NB_SESSION_STATUS                            0x00B4
     #define   NB_ADD_GROUP_NAME                            0x00B6
     #define   NB_SEND_NO_ACK                               0x00F1
     #define   NB_CHAIN_SEND_NO_ACK                         0x00F2
     #define   NB_LAN_STATUS_ALERT                          0x00F3
     #define   NB_FIND_NAME                                 0x00F8

     #define   NB_INVALID_COMMAND_CODE                      0x00FF


/*----------------------------------------------------------------*/
/*                       NETBIOS RETURN CODES                     */
/*----------------------------------------------------------------*/

     #define   NB_COMMAND_SUCCESSFUL                        0x0000
     #define   NB_ILLEGAL_BUFFER_LEN                        0x0001
     #define   NB_INVALID_COMMAND                           0x0003
     #define   NB_COMMAND_TIME_OUT                          0x0005
     #define   NB_MESSAGE_INCOMPLETE                        0x0006
     #define   NB_DATA_NOT_RECEIVED                         0x0007
     #define   NB_ILLEGAL_LOCAL_SESSION                     0x0008
     #define   NB_NO_RES_AVAILABLE                          0x0009
     #define   NB_SESSION_CLOSED                            0x000A
     #define   NB_COMMAND_CANCELLED                         0x000B
     #define   NB_DUPLICATE_NAME                            0x000D
     #define   NB_NAME_TABLE_FULL                           0x000E
     #define   NB_CMND_CMPL_DEREGISTERED                    0x000F
     #define   NB_SESSION_TABLE_FULL                        0x0011
     #define   NB_SES_OPEN_REJECTED                         0x0012
     #define   NB_ILLEGAL_NAME_NUMBER                       0x0013
     #define   NB_REMOTE_NAME_NOT_FOUND                     0x0014
     #define   NB_LOCAL_NAME_NOT_FOUND                      0x0015
     #define   NB_NAME_IN_USE                               0x0016
     #define   NB_NAME_DELETED                              0x0017
     #define   NB_SESSION_ENDED_ABNORMALLY                  0x0018
     #define   NB_NAME_CONFLICT                             0x0019
     #define   NB_INTERFACE_BUSY                            0x0021
     #define   NB_MAX_CMNDS_EXCEEDED                        0x0022
     #define   NB_INVALID_ADAPTER                           0x0023
     #define   NB_CMND_ALREADY_COMPLETED                    0x0024
     #define   NB_CMND_INVALID_TO_CANCEL                    0x0026
     #define   NB_NAME_DEFINED_BY_OTHERS                    0x0030
     #define   NB_ENVIRONMENT_NOT_DEFINED                   0x0034
     #define   NB_NO_OS_RESOURCES                           0x0035
     #define   NB_MAX_APPL_EXCEEDED                         0x0036
     #define   NB_NO_SAP_AVAILABLE                          0x0037
     #define   NB_INADEQUATE_RESOURCES                      0x0038
     #define   NB_INVALID_NCB_ADDRESS                       0x0039
     #define   NB_RESET_INVALID                             0x003A
     #define   NB_INVALID_DD_ID                             0x003B
     #define   NB_SEGMENT_LOCK_UNSUCCESSFUL                 0x003C
     #define   NB_DD_OPEN_ERROR                             0x003F
     #define   NB_OS_ERROR_DETECTED                         0x0040
     #define   NB_PERM_RING_STATUS                          0x004F

     #define   NB_UNEXPECTED_CCB_ERROR                      0x00F6
     #define   NB_ADAPTER_OPEN_ERROR                        0x00F8
     #define   NB_ADAPTER_HANDLER_ERROR                     0x00F9
     #define   NB_ADAPTER_CHECK                             0x00FA
     #define   NB_CODE_NOT_OPERATIONAL                      0x00FB
     #define   NB_OPEN_FAILURES                             0x00FC
     #define   NB_UNEXPECTED_CLOSE                          0x00FD

     #define   NB_COMMAND_IN_PROCESS                        0x00FF
