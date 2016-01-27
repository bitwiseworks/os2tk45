/*      SCCSID = @(#)bsexcpt.h  6.9 92/03/15 */

/*************************** START OF SPECIFICATION *****************
 *
 * Source File Name: bsexcpt.h
 *
 * Descriptive Name: Thread Exception Constants and Structure Definitions.
 *
 * Copyright: IBM Corp. 1989  - 1992
 *
 * Function: This file provides constants and data structure
 *      definitions required by application programs to use 32 bit
 *      thread exceptions management facility.
 *
 * Notes: None.
 *
 *************************** END OF SPECIFICATION *******************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#ifndef __BSEXCPT__
#define __BSEXCPT__

/*
 * User Exception Handler Return Codes:
 */

#define XCPT_CONTINUE_SEARCH    0x00000000      /* exception not handled   */
#define XCPT_CONTINUE_EXECUTION 0xFFFFFFFF      /* exception handled       */
#define XCPT_CONTINUE_STOP      0x00716668      /* exception handled by    */
                                                /* debugger (VIA DosDebug) */

/*
 * fHandlerFlags values (see ExceptionReportRecord):
 *
 * The user may only set (but not clear) the EH_NONCONTINUABLE flag.
 * All other flags are set by the system.
 *
 */

#define EH_NONCONTINUABLE   0x1          /* Noncontinuable exception */
#define EH_UNWINDING        0x2          /* Unwind is in progress */
#define EH_EXIT_UNWIND      0x4          /* Exit unwind is in progress */
#define EH_STACK_INVALID    0x8          /* Stack out of limits or unaligned */
#define EH_NESTED_CALL      0x10         /* Nested exception handler call */
#define EH_SIGFTERM         0x20         /* Nested exception handler call */


/*
 * Unwind all exception handlers (see DosUnwindException API)
 */
#define UNWIND_ALL              0


/*
 *   Exception values are 32 bit values layed out as follows:
 *
 *   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *   +---+-+-------------------------+-------------------------------+
 *   |Sev|C|       Facility          |               Code            |
 *   +---+-+-------------------------+-------------------------------+
 *
 *   where
 *
 *       Sev - is the severity code
 *           00 - Success
 *           01 - Informational
 *           10 - Warning
 *           11 - Error
 *
 *       C - is the Customer code flag
 *
 *       Facility - is the facility code
 *
 *       Code - is the facility's status code
 *
 *   Exceptions specific to OS/2 2.0 (e.g. XCPT_SIGNAL) will be marked
 *   with a facility code of 1.
 *
 *   System defined exceptions have a facility code of zero.
 *
 *   Each exception may also have several pieces of additional information.
 *   These are stored in the ExceptionInfo fields of the
 *   ExceptionReportRecord. They are documented here with the exceptions
 *   only for ease of reference.
 */

#define XCPT_FATAL_EXCEPTION    0xC0000000
#define XCPT_SEVERITY_CODE      0xC0000000
#define XCPT_CUSTOMER_CODE      0x20000000
#define XCPT_FACILITY_CODE      0x1FFF0000
#define XCPT_EXCEPTION_CODE     0x0000FFFF

/*  Violation flags in ExceptionInfo */

#define XCPT_UNKNOWN_ACCESS     0x00000000      /* Unknown access */
#define XCPT_READ_ACCESS        0x00000001      /* Read access    */
#define XCPT_WRITE_ACCESS       0x00000002      /* Write access   */
#define XCPT_EXECUTE_ACCESS     0x00000004      /* Execute access */
#define XCPT_SPACE_ACCESS       0x00000008      /* Address space access */
#define XCPT_LIMIT_ACCESS       0x00000010      /* Address space limit violation */
#define XCPT_DATA_UNKNOWN       0xFFFFFFFF

/*  Signal numbers for XCPT_SIGNAL */

#define XCPT_SIGNAL_INTR        1
#define XCPT_SIGNAL_KILLPROC    3
#define XCPT_SIGNAL_BREAK       4
#define XCPT_SIGNAL_APTERM      8

/*  Portable non-fatal software generated exceptions */

#define XCPT_GUARD_PAGE_VIOLATION       0x80000001
      /* ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
                                           XCPT_WRITE_ACCESS */
      /* ExceptionInfo[ 1 ] - FaultAddr */

#define XCPT_UNABLE_TO_GROW_STACK       0x80010001

/*  Portable fatal hardware generated exceptions */

#define XCPT_DATATYPE_MISALIGNMENT      0xC000009E
      /* ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
                                           XCPT_WRITE_ACCESS */
      /* ExceptionInfo[ 1 ] - Alignment */
      /* ExceptionInfo[ 2 ] - FaultAddr */

#define XCPT_BREAKPOINT                 0xC000009F
#define XCPT_SINGLE_STEP                0xC00000A0
#define XCPT_ACCESS_VIOLATION           0xC0000005
      /* ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
                                           XCPT_WRITE_ACCESS
                                           XCPT_SPACE_ACCESS
                                           XCPT_LIMIT_ACCESS
                                           XCPT_UNKNOWN_ACCESS */
      /* ExceptionInfo[ 1 ] - FaultAddr (XCPT_READ_ACCESS/XCPT_WRITE_ACCESS)
                              Selector  (XCPT_SPACE_ACCESS)
                              -1        (XCPT_LIMIT_ACCESS) */

#define XCPT_ILLEGAL_INSTRUCTION        0xC000001C
#define XCPT_FLOAT_DENORMAL_OPERAND     0xC0000094
#define XCPT_FLOAT_DIVIDE_BY_ZERO       0xC0000095
#define XCPT_FLOAT_INEXACT_RESULT       0xC0000096
#define XCPT_FLOAT_INVALID_OPERATION    0xC0000097
#define XCPT_FLOAT_OVERFLOW             0xC0000098
#define XCPT_FLOAT_STACK_CHECK          0xC0000099
#define XCPT_FLOAT_UNDERFLOW            0xC000009A
#define XCPT_INTEGER_DIVIDE_BY_ZERO     0xC000009B
#define XCPT_INTEGER_OVERFLOW           0xC000009C
#define XCPT_PRIVILEGED_INSTRUCTION     0xC000009D

/*  Portable fatal software generated exceptions */

#define XCPT_IN_PAGE_ERROR              0xC0000006
      /* ExceptionInfo[ 0 ] - FaultAddr */

#define XCPT_PROCESS_TERMINATE          0xC0010001
#define XCPT_ASYNC_PROCESS_TERMINATE    0xC0010002
      /* ExceptionInfo[ 0 ] - TID of 'terminator' thread */

#define XCPT_NONCONTINUABLE_EXCEPTION   0xC0000024
#define XCPT_INVALID_DISPOSITION        0xC0000025

/*  Non-portable fatal exceptions */

#define XCPT_INVALID_LOCK_SEQUENCE      0xC000001D
#define XCPT_ARRAY_BOUNDS_EXCEEDED      0xC0000093
#define XCPT_B1NPX_ERRATA_02            0xC0010004

/*  Misc exceptions */

#define XCPT_UNWIND                     0xC0000026
#define XCPT_BAD_STACK                  0xC0000027
#define XCPT_INVALID_UNWIND_TARGET      0xC0000028

/*  Signal Exceptions */

#define XCPT_SIGNAL                     0xC0010003
      /* ExceptionInfo[ 0 ] - Signal Number */

/*
 * ContextRecord
 *
 * This is the machine specific register contents for the thread
 * at the time of the exception. Note that only the register sets
 * specified by ContextFlags contain valid data. Conversely, only
 * registers specified in ContextFlags will be restored if an exception
 * is handled.
 */

/*  The following flags control the contents of the CONTEXT structure. */

#define CONTEXT_CONTROL         0x00000001L     /* SS:ESP, CS:EIP, EFLAGS,  */
                                                /* EBP                      */
#define CONTEXT_INTEGER         0x00000002L     /* EAX, EBX, ECX, EDX, ESI, */
                                                /* EDI                      */
#define CONTEXT_SEGMENTS        0x00000004L     /* DS, ES, FS, GS           */
#define CONTEXT_FLOATING_POINT  0x00000008L     /* numeric coprocessor state */

#define CONTEXT_FULL            (CONTEXT_CONTROL | CONTEXT_INTEGER | \
                                 CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT)

/* NOINC */
#pragma pack(1)
/* INC */
typedef struct  _fpreg      /* coprocessor stack register element */
   {
   ULONG losig;
   ULONG hisig;
   USHORT signexp;
   } FPREG , *PFPREG ;

/* NOINC */
#pragma pack()
/* INC */

struct _CONTEXT
   {
  /*
   * The flags values within this flag control the contents of
   * a ContextRecord.
   *
   * If the ContextRecord is used as an input parameter, then
   * for each portion of the ContextRecord controlled by a flag
   * whose value is set, it is assumed that that portion of the
   * ContextRecord contains valid context. If the ContextRecord
   * is being used to modify a thread's context, then only that
   * portion of the thread's context will be modified.
   *
   * If the ContextRecord is used as an Input/Output parameter to
   * capture the context of a thread, then only those portions of the
   * thread's context corresponding to set flags will be returned.
   */

   ULONG ContextFlags;

  /*
   * This section is specified/returned if the ContextFlags
   * contains the flag CONTEXT_FLOATING_POINT.
   */

   ULONG   ctx_env[7];
   FPREG   ctx_stack[8];

  /*
   * This section is specified/returned if the ContextFlags
   * contains the flag CONTEXT_SEGMENTS.
   */

   ULONG ctx_SegGs;
   ULONG ctx_SegFs;
   ULONG ctx_SegEs;
   ULONG ctx_SegDs;

  /*
   * This section is specified/returned if the ContextFlags
   * contains the flag CONTEXT_INTEGER.
   */

   ULONG ctx_RegEdi;
   ULONG ctx_RegEsi;
   ULONG ctx_RegEax;
   ULONG ctx_RegEbx;
   ULONG ctx_RegEcx;
   ULONG ctx_RegEdx;

  /*
   * This section is specified/returned if the ContextFlags
   * contains the flag CONTEXT_CONTROL.
   */

   ULONG ctx_RegEbp;
   ULONG ctx_RegEip;
   ULONG ctx_SegCs;
   ULONG ctx_EFlags;
   ULONG ctx_RegEsp;
   ULONG ctx_SegSs;

   };

typedef struct _CONTEXT CONTEXTRECORD;
typedef struct _CONTEXT *PCONTEXTRECORD;


/*
 * ExceptionReportRecord
 *
 * This structure contains machine independant information about an
 * exception/unwind. No system exception will ever have more than
 * EXCEPTION_MAXIMUM_PARAMETERS parameters. User exceptions are not
 * bound to this limit.
 */


#define EXCEPTION_MAXIMUM_PARAMETERS 4  /* Enough for all system exceptions. */

struct _EXCEPTIONREPORTRECORD
   {
   ULONG   ExceptionNum;                /* exception number */
   ULONG   fHandlerFlags;
   struct  _EXCEPTIONREPORTRECORD    *NestedExceptionReportRecord;
   PVOID   ExceptionAddress;
   ULONG   cParameters;                 /* Size of Exception Specific Info */
   ULONG   ExceptionInfo[EXCEPTION_MAXIMUM_PARAMETERS];
                                        /* Exception Specfic Info */
   };

typedef struct _EXCEPTIONREPORTRECORD EXCEPTIONREPORTRECORD;
typedef struct _EXCEPTIONREPORTRECORD *PEXCEPTIONREPORTRECORD;

/*
 * ExceptionRegistrationRecord
 *
 * These are linked together to form a chain of exception handlers that
 * will be dispatched to upon receipt of an exception.
 */


struct _EXCEPTIONREGISTRATIONRECORD;

/* VISUAL AGE COMPILER - start */
#ifdef KRNL
typedef ULONG _cdecl _ERR(PEXCEPTIONREPORTRECORD,
                          struct _EXCEPTIONREGISTRATIONRECORD *,
                          PCONTEXTRECORD,
                          PVOID);
#else
typedef ULONG APIENTRY _ERR(PEXCEPTIONREPORTRECORD,
                            struct _EXCEPTIONREGISTRATIONRECORD *,
                            PCONTEXTRECORD,
                            PVOID);
#endif
/* VISUAL AGE COMPILER - end   */

typedef _ERR  *ERR;

struct _EXCEPTIONREGISTRATIONRECORD
   {
   struct _EXCEPTIONREGISTRATIONRECORD * volatile prev_structure;
   _ERR * volatile ExceptionHandler;
   };

typedef struct _EXCEPTIONREGISTRATIONRECORD EXCEPTIONREGISTRATIONRECORD;
typedef struct _EXCEPTIONREGISTRATIONRECORD *PEXCEPTIONREGISTRATIONRECORD;

/* End of exception chain marker. */

#define END_OF_CHAIN        ((PEXCEPTIONREGISTRATIONRECORD) -1)
/* NOINC */
#pragma pack()
/* INC */

#endif /* __BSEXCPT__ */

/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
