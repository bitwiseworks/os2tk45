/*
 *   Namepipe.h - variables and defines used by both
 *                server and client pipe routines.
 *
 *   Copyright 1991 IBM Corp.
 *
 */

struct _PIPE_MSG
{
   USHORT  usMsgType;
   USHORT  usMsgData;
};

typedef struct _PIPE_MSG  PIPE_MSG;

#define START_PIPE_NAME  "\\pipe\\BUYAPS2"
#define GAME_PIPE_NAME   "\\pipe\\NOWINDOZ"

#define FS_SVR_STARTPIPE_OPEN_MODE  NP_ACCESS_DUPLEX | NP_NOINHERIT | NP_WRITEBEHIND
#define FS_SVR_STARTPIPE_PIPE_MODE  NP_WAIT | NP_READMODE_MESSAGE | NP_TYPE_MESSAGE | 1

#define FS_CLI_STARTPIPE_OPEN_MODE  OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE | OPEN_FLAGS_WRITE_THROUGH

#define FS_SVR_GAMEPIPE_OPEN_MODE   NP_ACCESS_DUPLEX | NP_NOINHERIT | NP_WRITEBEHIND
#define FS_SVR_GAMEPIPE_PIPE_MODE   NP_WAIT | NP_READMODE_MESSAGE | NP_TYPE_MESSAGE | MAX_PLAYERS

#define FS_CLI_GAMEPIPE_OPEN_MODE   OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE | OPEN_FLAGS_WRITE_THROUGH

#define DIVISIONS     3                      /* number of squares per side */
#define SERVER        TRUE
#define CLIENT        FALSE
#define PIPE_TIMEOUT  1000                   /* milliseconds till pipe times out */
#define MAX_PLAYERS   NP_UNLIMITED_INSTANCES
#define MAXSERVERNAME 9                      /* maximum length of server name */
#define MAXPIPENAME   (MAXSERVERNAME+16)     /* maximum length of pipe name */

/*
 *  1 = server, DIVISIONS+1 = client
 */

#define SERVER_NUM  (USHORT)1
#define CLIENT_NUM  (DIVISIONS+(USHORT)1)

#define CLIENT_MOVE       (USHORT)10
#define YOU_FIRST         (USHORT)12
#define ERROR_MSG         (USHORT)13
#define CLIENT_QUIT       (USHORT)14
#define WIN_CLIENT        (USHORT)15
#define WIN_SERVER        (USHORT)16
#define SERVER_MOVE       (USHORT)17
#define START_MSG         (USHORT)18
#define WIN_DRAW          (USHORT)20

#define WM_CLIENT_MOVE    (WM_USER+CLIENT_MOVE)
#define WM_YOU_FIRST      (WM_USER+YOU_FIRST)
#define WM_ERROR_MSG      (WM_USER+ERROR_MSG)
#define WM_CLIENT_QUIT    (WM_USER+CLIENT_QUIT)
#define WM_WIN_CLIENT     (WM_USER+WIN_CLIENT)
#define WM_WIN_SERVER     (WM_USER+WIN_SERVER)
#define WM_SERVER_MOVE    (WM_USER+SERVER_MOVE)
#define WM_START          (WM_USER+START_MSG)
#define WM_DRAW           (WM_USER+WIN_DRAW)

#define CLIENT_MOVE_TEXT    "CLIENT_MOVE"
#define YOU_FIRST_TEXT      "YOU_FIRST"
#define ERROR_MSG_TEXT      "ERROR_MSG"
#define CLIENT_QUIT_TEXT    "CLIENT_QUIT"
#define WIN_CLIENT_TEXT     "WIN_CLIENT"
#define WIN_SERVER_TEXT     "WIN_SERVER"
#define SERVER_MOVE_TEXT    "SERVER_MOVE"
#define START_MSG_TEXT      "START_MSG"
#define WIN_DRAW_TEXT       "WIN_DRAW"
