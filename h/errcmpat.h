#ifndef _ERRCMPAT_H
  #define _ERRCMPAT_H
/*
 * for compatibility with old return codes for ULS
 */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef UNI_SUCCESS
  #define UNI_SUCCESS                         ULS_SUCCESS
#endif

#ifndef NO_ERROR
  #define NO_ERROR                            0
#endif

#ifndef UNI_UNSUPPORTED
  #define UNI_UNSUPPORTED                     ULS_UNSUPPORTED
#endif

#ifndef UNI_NOMEMORY
  #define UNI_NOMEMORY                        ULS_NOMEMORY
#endif

#ifndef UNI_INVALID
  #define UNI_INVALID                         ULS_INVALID
#endif

#ifndef UNI_BADOBJ
  #define UNI_BADOBJ                          ULS_BADOBJECT
#endif

#ifndef UNI_NOTOKEN
  #define UNI_NOTOKEN                         ULS_NOTOKEN
#endif

#ifndef UNI_NOMATCH
  #define UNI_NOMATCH                         ULS_NOMATCH
#endif

#ifndef UNI_BUFFER_FULL
  #define UNI_BUFFER_FULL                     ULS_BUFFERFULL
#endif

#ifndef UNI_RANGE
  #define UNI_RANGE                           ULS_RANGE
#endif

#ifndef ERR_NOOP
  #define ERR_NOOP                            ULS_NOOP
#endif

#ifndef ERR_TOO_MANY_KBD
  #define ERR_TOO_MANY_KBD                    ULS_TOOMANYKBD
#endif

#ifndef ERR_KBD_NOT_FOUND
  #define ERR_KBD_NOT_FOUND                   ULS_KBDNOTFOUND
#endif

#ifndef ERR_KBD_INVALID
  #define ERR_KBD_INVALID                     ULS_INVALID
#endif

#ifndef ERR_BAD_HANDLE
  #define ERR_BAD_HANDLE                      ULS_BADHANDLE
#endif

#ifndef ERR_NO_DEAD
  #define ERR_NO_DEAD                         ULS_NODEAD
#endif

#ifndef ERR_NO_SCAN
  #define ERR_NO_SCAN                         ULS_NOSCAN
#endif

#ifndef ERR_INVALID_SCAN
  #define ERR_INVALID_SCAN                    ULS_INVALIDSCAN
#endif

#ifndef ERR_NOT_IMPLEMENTED
  #define ERR_NOT_IMPLEMENTED                 ULS_NOTIMPLEMENTED
#endif

#ifdef ERR_NO_MEMORY
#undef ERR_NO_MEMORY
#endif
#define ERR_NO_MEMORY                       ULS_NOMEMORY

#ifdef __cplusplus
   }
#endif

#endif
