
.SEQ   ;Use the segments in the order listed
.286p

jmp32   macro   target
                db      066h                    ; operand size prefix
                jmp     flat:target
endm

extrn   _init16_c:far                           ;16bit initialization code (in init16.c)

;group the segments into the correct order
DGROUP  group NULL, _DATA, CONST, FAR_BSS, _BSS, LAST_D

NULL            SEGMENT WORD PUBLIC 'BEGDATA'

;Device driver header
public  dev_header
dev_header      equ     $
ptr_to_next     dd      0ffffffffh      ;loadable
device_attr     dw      0C880h          ;option bits
offst           dw      MAINSEG:STRATEGY
offst1          dw      MAINSEG:IDC
dev_name        db      'TEST$    '     ;device name
res2            dw      4 dup(0)
;--------end of header

public __acrtused
__acrtused      dw      0               ;C .OBJ files want one. They never use it.

public _gdtrtab                         ; will be passed to sgdt instruction
_gdtrtab        db      8 dup(0)

public _myds
_myds           dw      ?

public _devhelp
_devhelp        dd      ?

public mixed                            ;this variable is used to facilitate switching between
mixed           dw      init16          ;the 16bit init code and the 32bit strategy code

NULL            ENDS

_DATA           SEGMENT WORD PUBLIC 'DATA'
_DATA           ENDS

CONST           SEGMENT WORD PUBLIC 'CONST'
CONST           ENDS

FAR_BSS         SEGMENT WORD PUBLIC 'FAR_BSS'
FAR_BSS         ENDS

_BSS            SEGMENT WORD PUBLIC 'BSS'
_BSS            ENDS

LAST_D          SEGMENT WORD PUBLIC 'LAST_DATA'
public          _last_d
_last_d         equ     $
LAST_D          ends

extrn Dos32FlatDS:abs

;==========================================
; Entry point to Strategy routine
;==========================================

MAINSEG         SEGMENT WORD PUBLIC 'CODE'
assume          cs:MAINSEG, ds:DGROUP, es:nothing

STRATEGY        proc    far
                push    es      ; es:bx is a pointer to an lpreq structure
                push    bx
                jmp     mixed
mixed_ret:
                pop     bx
                pop     es
                ret
STRATEGY        endp


public  _setjmp32
_setjmp32       proc    far
                ;(called from init16.c)
                ;this tells the STRATEGY function to jump to strategy1632 next time
                push    bx
                mov     bx, OFFSET mixed
                mov     [bx], OFFSET MAINSEG:strategy1632
                pop     bx
                ret
_setjmp32       endp


init16:         call    _initmyds
                call    _init16_c
                jmp     mixed_ret


strategy1632:
                push    ds
                push    es              ;Save the ES as in 32 bit DS should equal ES
                mov     ax, Dos32FlatDS
                mov     ds,ax
                mov     es,ax           ;Added because this is what the /Gv would have done
                jmp32   strategy32      ;jmp to the 32bit code segment
strategy3216:   pop     es
                pop     ds
                jmp     mixed_ret


;used in .c file
_GetDosVarLinearPtr proc public
                push    bp
                push    bx
                mov     bp, sp
                mov     al, 09h         ; The reserved index=9 to get ptr to the kernel DOSTABLE struct.
                mov     dl, 24h         ; DevHlp_GetDosVar
                call    [_devhelp]
                jc      mygetdosvarerror
                ; Now we have        AX: Selector BX: Offset
                mov     dx, ax
                mov     ax, bx
                pop     bx
                pop     bp
                ret                     ; Return the EAX which is linear pointer to TABLE
mygetdosvarerror:
                mov     ax, 0
                mov     dx, 0
                pop     bx
                pop     bp
                ret
_GetDosVarLinearPtr endp


IDC             proc    far
                ret
IDC             endp


public _initmyds
_initmyds proc far
                mov     ax, ds
                mov     _myds, ax
                ret
_initmyds endp



thunk3216devhelp16:
        call    [_devhelp]              ; 16-bit DevHelp call from 16-bit code
                                        ; callee params are as they were
        jmp     flat:thunk1632devhelp32 ; go back to the 32-bit generic stub

MAINSEG ends

.386
CODE32          SEGMENT DWORD USE32 PUBLIC 'CODE'
assume ds:flat, cs:CODE32

extrn   strategy32_c:near                       ; 32bit C strategy function
extrn   seltoflat:near                          ; in driver.c
extrn   _ptkssbase:dword                        ; in init16.c


public int3break
int3break       proc
                int     3
                ret
int3break       endp


strategy32:     sgdt    byte ptr _gdtrtab       ; find the start of the gdt table
                push    ss:[esp+4]              ; push 4 bytes of 16:16 address
                call    seltoflat
                add     esp,4                   ; clean-up stack
                mov     edx, eax
                or      eax, 0
                jz      sanityfailed

                push    eax
                call    strategy32_c
                add     esp,4
sanityfailed:
                jmp     far ptr strategy3216    ; jump back to 16_bit code



;virt2lin(16:16 virt address) and returns the linear address in eax register
public virt2lin
virt2lin proc
                push    ebp
                mov     ebp,esp

                mov     ax, ss:[ebp+10]                 ; selector
                push    esi
                xor     esi, esi
                mov     si, ss:[ebp+8]                  ; selector
                mov     dl, 5bh                         ; devhlp_virt2lin
                call    devhelp32
                jc      virt2linerror
                pop     esi
                pop     ebp
                ret

virt2linerror:
                xor     eax, eax
                pop     esi
                pop     ebp
                ret
virt2lin endp


retss proc public
                mov     eax, ss
                ret
retss endp


sstods proc public
                mov     eax, dword ptr _ptkssbase
                or      eax, 0
                jz      sstodsexit
                mov     eax, dword ptr [eax]
sstodsexit:
                ret
sstods          endp


devhelp32       proc public
        push    ds
        push    eax
        mov     ax, _myds
        mov     ds, ax
        pop     eax
        jmp     far ptr thunk3216devhelp16      ; jump to  16-bit code

thunk1632devhelp32:
                pop     ds
                ret
devhelp32       endp


CODE32          ENDS


_TEXT           SEGMENT WORD PUBLIC 'CODE'
_TEXT           ENDS


END_TEXT        SEGMENT WORD PUBLIC 'CODE'
END_TEXT        ENDS
END
