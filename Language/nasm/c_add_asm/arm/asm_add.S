.section .text, "x"
.global add
@IMPORT func
add:
    ADD r0, r0, r1
    MOV pc, lr