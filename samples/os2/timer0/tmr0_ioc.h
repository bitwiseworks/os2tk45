/* TMR0_IOC.H - High Resolution Timer IOCtl interface

   MODIFICATION HISTORY
   DATE       PROGRAMMER   COMMENT
   01-Jul-95  Timur Tabi   Creation
*/

#define HRTERR_BADPARAM       1

#define HRT_IOCTL_CATEGORY    0x80

#define HRT_QUERY_VERSION     0
#define HRT_GET_RESOLUTION    1
#define HRT_SET_RESOLUTION    2
#define HRT_GET_POINTER       3
#define HRT_FREE_POINTER      4
#define HRT_BLOCKUNTIL        5

#define HRTVER_MAJOR(ver)     (((ver) & 0xC000) >> 14)
#define HRTVER_MINOR(ver)     (((ver) & 0x3C00) >> 10)
#define HRTVER_BUGFIX(ver)    (((ver) & 0x03C0) >> 6)
#define HRTVER_PHASE(ver)     (((ver) & 0x0030) >> 4)
#define HRTVER_BUILD(ver)     ((ver) & 0x000F)

#define HRTVER_DEVELOP        0
#define HRTVER_ALPHA          1
#define HRTVER_BETA           2
#define HRTVER_RELEASE        3
