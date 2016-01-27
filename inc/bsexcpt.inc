; static char *SCCSID = "@(#)bsexcpt.h  6.5 91/08/22";  
;       SCCSID = @(#)bsexcpt.h  6.5 91/08/22   
;*************************** START OF SPECIFICATION ************************ *                                                                         *
;* Source File Name: BSEXCPT.H                                             *
;*                                                                         *
;* Descriptive Name: Thread Exception Constants and Structure Definitions. *
;*                                                                         *
;* Copyright (c) IBM Corporation 1987,1991                                 *
;* Copyright (c) Microsoft Corporation 1987,1991                           *
;*                                                                         *
;* Function: This file provides constants and data structure definitions   *
;*      required by application programs to use 32-bit thread exceptions   *
;*      management facility.                                               *
;*                                                                         *
;***************************  END OF SPECIFICATION ************************  
; 
;* User Exception Handler Return Codes:
;  
XCPT_CONTINUE_SEARCH	EQU	00000000H       ;  Exception not handled     
XCPT_CONTINUE_EXECUTION	EQU	0FFFFFFFFH       ;  Exception handled         
XCPT_CONTINUE_STOP	EQU	00716668H       ;  Exception handled by      
                                                ;  debugger (via DosDebug)   
; 
;* fHandlerFlags values (see ExceptionReportRecord):
;*
;* The user may only set (but not clear) the EH_NONCONTINUABLE flag.
;* All other flags are set by the system.
;*
;  
EH_NONCONTINUABLE	EQU	1H           ;  Noncontinuable exception           
EH_UNWINDING	EQU	2H                ;  Unwind is in progress              
EH_EXIT_UNWIND	EQU	4H              ;  Exit unwind is in progress         
EH_STACK_INVALID	EQU	8H            ;  Stack out of limits or unaligned   
EH_NESTED_CALL	EQU	10H             ;  Nested exception handler call      
EH_SIGFTERM	EQU	20H                ;  Forced SIGTERM                   @SEC
; 
;* Unwind all exception handlers (see DosUnwindException API)
;  
UNWIND_ALL	EQU	0 
; 
;*   Exception values are 32-bit values laid out as follows:
;*
;*   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
;*   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;*   +---+-+-------------------------+-------------------------------+
;*   |Sev|C|       Facility          |               Code            |
;*   +---+-+-------------------------+-------------------------------+
;*
;*   where
;*
;*       Sev - is the severity code
;*           00 - Success
;*           01 - Informational
;*           10 - Warning
;*           11 - Error
;*
;*       C - is the Customer code flag
;*
;*       Facility - is the facility code
;*
;*       Code - is the facility's status code
;*
;*   Exceptions specific to OS/2 2.0 (e.g. XCPT_SIGNAL) will be marked
;*   with a facility code of 1.
;*
;*   System defined exceptions have a facility code of zero.
;*
;*   Each exception may also have several pieces of additional information.
;*   These are stored in the ExceptionInfo fields of the
;*   ExceptionReportRecord. They are documented here with the exceptions
;*   only for ease of reference.
;  
XCPT_FATAL_EXCEPTION	EQU	0C0000000H 
XCPT_SEVERITY_CODE	EQU	0C0000000H 
XCPT_CUSTOMER_CODE	EQU	20000000H 
XCPT_FACILITY_CODE	EQU	1FFF0000H 
XCPT_EXCEPTION_CODE	EQU	0000FFFFH 
; 
;* Violation flags in ExceptionInfo
;  
XCPT_UNKNOWN_ACCESS	EQU	00000000H       ;  Unknown access            
XCPT_READ_ACCESS	EQU	00000001H       ;  Read access               
XCPT_WRITE_ACCESS	EQU	00000002H       ;  Write access              
XCPT_EXECUTE_ACCESS	EQU	00000004H       ;  Execute access            
XCPT_SPACE_ACCESS	EQU	00000008H       ;  Address space access      
XCPT_LIMIT_ACCESS	EQU	00000010H       ;  Address space limit       
                                                ;  violation                 
XCPT_DATA_UNKNOWN	EQU	0FFFFFFFFH 
; 
;* Signal numbers for XCPT_SIGNAL
;  
XCPT_SIGNAL_INTR	EQU	1 
XCPT_SIGNAL_KILLPROC	EQU	3 
XCPT_SIGNAL_BREAK	EQU	4 
XCPT_SIGNAL_APTERM	EQU	8                                   ; 108449  
; 
;* Portable non-fatal software generated exceptions
;  
XCPT_GUARD_PAGE_VIOLATION	EQU	80000001H 
      ;  ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
;                                          XCPT_WRITE_ACCESS   
      ;  ExceptionInfo[ 1 ] - FaultAddr   
XCPT_UNABLE_TO_GROW_STACK	EQU	80010001H 
; 
;* Portable fatal hardware generated exceptions
;  
XCPT_DATATYPE_MISALIGNMENT	EQU	0C000009EH 
      ;  ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
;                                          XCPT_WRITE_ACCESS   
      ;  ExceptionInfo[ 1 ] - Alignment   
      ;  ExceptionInfo[ 2 ] - FaultAddr   
XCPT_BREAKPOINT	EQU	0C000009FH 
XCPT_SINGLE_STEP	EQU	0C00000A0H 
XCPT_ACCESS_VIOLATION	EQU	0C0000005H 
      ;  ExceptionInfo[ 0 ] - Access Code: XCPT_READ_ACCESS
;                                          XCPT_WRITE_ACCESS
;                                          XCPT_SPACE_ACCESS
;                                          XCPT_LIMIT_ACCESS
;                                          XCPT_UNKNOWN_ACCESS   
      ;  ExceptionInfo[ 1 ] - FaultAddr (XCPT_READ_ACCESS/XCPT_WRITE_ACCESS)
;                             Selector  (XCPT_SPACE_ACCESS)
;                             -1        (XCPT_LIMIT_ACCESS)   
XCPT_ILLEGAL_INSTRUCTION	EQU	0C000001CH 
XCPT_FLOAT_DENORMAL_OPERAND	EQU	0C0000094H 
XCPT_FLOAT_DIVIDE_BY_ZERO	EQU	0C0000095H 
XCPT_FLOAT_INEXACT_RESULT	EQU	0C0000096H 
XCPT_FLOAT_INVALID_OPERATION	EQU	0C0000097H 
XCPT_FLOAT_OVERFLOW	EQU	0C0000098H 
XCPT_FLOAT_STACK_CHECK	EQU	0C0000099H 
XCPT_FLOAT_UNDERFLOW	EQU	0C000009AH 
XCPT_INTEGER_DIVIDE_BY_ZERO	EQU	0C000009BH 
XCPT_INTEGER_OVERFLOW	EQU	0C000009CH 
XCPT_PRIVILEGED_INSTRUCTION	EQU	0C000009DH 
; 
;* Portable fatal software generated exceptions
;  
XCPT_IN_PAGE_ERROR	EQU	0C0000006H 
      ;  ExceptionInfo[ 0 ] - FaultAddr   
XCPT_PROCESS_TERMINATE	EQU	0C0010001H 
XCPT_ASYNC_PROCESS_TERMINATE	EQU	0C0010002H 
      ;  ExceptionInfo[ 0 ] - TID of 'terminator' thread   
XCPT_NONCONTINUABLE_EXCEPTION	EQU	0C0000024H 
XCPT_INVALID_DISPOSITION	EQU	0C0000025H 
; 
;* Non-portable fatal exceptions
;  
XCPT_INVALID_LOCK_SEQUENCE	EQU	0C000001DH 
XCPT_ARRAY_BOUNDS_EXCEEDED	EQU	0C0000093H 
XCPT_B1NPX_ERRATA_02	EQU	0C0010004H 
; 
;* Misc exceptions
;  
XCPT_UNWIND	EQU	0C0000026H 
XCPT_BAD_STACK	EQU	0C0000027H 
XCPT_INVALID_UNWIND_TARGET	EQU	0C0000028H 
; 
;* Signal Exceptions
;  
XCPT_SIGNAL	EQU	0C0010003H 
      ;  ExceptionInfo[ 0 ] - Signal Number   
; 
;* ContextRecord
;*
;* This is the machine specific register contents for the thread
;* at the time of the exception. Note that only the register sets
;* specified by ContextFlags contain valid data. Conversely, only
;* registers specified in ContextFlags will be restored if an exception
;* is handled.
;  
; 
;* The following flags control the contents of the ContextRecord.
;  
CONTEXT_CONTROL	EQU	00000001H   ;  SS:ESP, CS:EIP, EFLAGS,      
                                             ;  EBP                          
CONTEXT_INTEGER	EQU	00000002H   ;  EAX, EBX, ECX, EDX, ESI,     
                                             ;  EDI                          
CONTEXT_SEGMENTS	EQU	00000004H   ;  DS, ES, FS, GS               
CONTEXT_FLOATING_POINT	EQU	00000008H   ;  Numeric coprocessor state    
CONTEXT_FULL	EQU	(CONTEXT_CONTROL  OR  CONTEXT_INTEGER  OR                        CONTEXT_SEGMENTS  OR  CONTEXT_FLOATING_POINT)

_fpreg	STRUC
losig	DD	?
hisig	DD	?
signexp	DW	?
_fpreg	ENDS

FPREG struc
db size _fpreg dup(?)
FPREG ends


_CONTEXT	STRUC
  ; 
;  * The flags values within this flag control the contents of
;  * a ContextRecord.
;  *
;  * If the ContextRecord is used as an input parameter, then
;  * for each portion of the ContextRecord controlled by a flag
;  * whose value is set, it is assumed that that portion of the
;  * ContextRecord contains valid context. If the ContextRecord
;  * is being used to modify a thread's context, then only that
;  * portion of the thread's context will be modified.
;  *
;  * If the ContextRecord is used as an Input/Output parameter to
;  * capture the context of a thread, then only those portions of the
;  * thread's context corresponding to set flags will be returned.
;    
ContextFlags	DD	?
  ; 
;  * This section is specified/returned if the ContextFlags
;  * contains the flag CONTEXT_FLOATING_POINT.
;    
ctx_env	DD	7 DUP (?)
ctx_stack	DB	SIZE _fpreg * 8 DUP (?)
  ; 
;  * This section is specified/returned if the ContextFlags
;  * contains the flag CONTEXT_SEGMENTS.
;    
ctx_SegGs	DD	?
ctx_SegFs	DD	?
ctx_SegEs	DD	?
ctx_SegDs	DD	?
  ; 
;  * This section is specified/returned if the ContextFlags
;  * contains the flag CONTEXT_INTEGER.
;    
ctx_RegEdi	DD	?
ctx_RegEsi	DD	?
ctx_RegEax	DD	?
ctx_RegEbx	DD	?
ctx_RegEcx	DD	?
ctx_RegEdx	DD	?
  ; 
;  * This section is specified/returned if the ContextFlags
;  * contains the flag CONTEXT_CONTROL.
;    
ctx_RegEbp	DD	?
ctx_RegEip	DD	?
ctx_SegCs	DD	?
ctx_EFlags	DD	?
ctx_RegEsp	DD	?
ctx_SegSs	DD	?
_CONTEXT	ENDS

CONTEXTRECORD struc
db size _CONTEXT dup(?)
CONTEXTRECORD ends

PCONTEXTRECORD struc
	dd ?
PCONTEXTRECORD ends

; 
;* ExceptionReportRecord
;*
;* This structure contains machine independant information about an
;* exception/unwind. No system exception will ever have more than
;* EXCEPTION_MAXIMUM_PARAMETERS parameters. User exceptions are not
;* bound to this limit.
;  
EXCEPTION_MAXIMUM_PARAMETERS	EQU	4  ;  Enough for all system exceptions   

_EXCEPTIONREPORTRECORD	STRUC
ExceptionNum	DD	?                ;  Exception number                  
fHandlerFlags	DD	?               ;  Exception attributes              
NestedExceptionReportRecord	DD	?
                                        ;  Preceding exception's report      
                                        ;  record if nested exception        
ExceptionAddress	DD	?            ;  Exception address                 
cParameters	DD	?                 ;  Size of exception specific        
                                        ;  information                       
ExceptionInfo	DD	EXCEPTION_MAXIMUM_PARAMETERS DUP (?)
                                        ;  Exception specfic information     
_EXCEPTIONREPORTRECORD	ENDS

EXCEPTIONREPORTRECORD struc
db size _EXCEPTIONREPORTRECORD dup(?)
EXCEPTIONREPORTRECORD ends

PEXCEPTIONREPORTRECORD struc
	dd ?
PEXCEPTIONREPORTRECORD ends

; 
;* ExceptionRegistrationRecord
;*
;* These are linked together to form a chain of exception handlers that
;* will be dispatched to upon receipt of an exception.
;  

_EXCEPTIONREGISTRATIONRECORD    STRUC
prev_structure      DD      ?
ExceptionHandler    DD      ?
_EXCEPTIONREGISTRATIONRECORD    ENDS

EXCEPTIONREGISTRATIONRECORD struc
        db size _EXCEPTIONREGISTRATIONRECORD dup(?)
EXCEPTIONREGISTRATIONRECORD ends

PEXCEPTIONREGISTRATIONRECORD struc
        dd ?
PEXCEPTIONREGISTRATIONRECORD ends


; 
;* End of exception chain marker.
;  
END_OF_CHAIN    EQU     -1

