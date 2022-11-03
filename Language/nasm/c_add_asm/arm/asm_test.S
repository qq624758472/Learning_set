.text

.globl _start

_start:
    mov     r0, $1
    ldr     r1, =message
    mov     r2, $message_len
    mov     r7, $4
    swi     $0

    mov     r0, $0
    mov     r7, $1
    swi     $0
	
.data

message:
    .ascii      "Hello World!\n"
message_len = . - message
