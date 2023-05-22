;*****************************************************************************
;*                                                                           *
;*                  C O R O U T I N E                                        *
;*                                                                           *
;*---------------------------------------------------------------------------*
;* Beschreibung:    Assemblerfunktionen zum Starten des ersten Koroutine und *
;*                  zum Umschalten zwischen Koroutinen.                      *
;*                                                                           *
;* Autor:           Michael, Schoettner, HHU, 6.02.2023                      *
;*****************************************************************************


; EXPORTIERTE FUNKTIONEN

[GLOBAL Thread_start]
[GLOBAL Thread_switch]

[EXTERN dbgPrint]
[EXTERN dbgPrint2]
[EXTERN dbgPrintValue]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]
[BITS 64]


; Thread_start: Startet die erste Koroutine
;
; C Prototyp:     void Thread_start  (uint64_t* context);
Thread_start:
	mov rsp, rdi

	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	popfq
	sti
	ret



;
; Thread_switch: Threadnumschaltung. Der aktuelle Registersatz wird
;                   gesichert und der Registersatz der neuen Thread
;                   wird in den Prozessor geladen.
;
; C Prototyp:       Thread_switch (uint64_t** context_now, uint64_t** context_then);
Thread_switch:
	cli
	pushfq
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp




	mov [rdi], rsp

	mov rdi, [rsi]

	call Thread_start
	ret
