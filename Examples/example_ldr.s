.global main
.text
main:
    mov r0, #0x34
    mov r1, #0x45
    mov r2, #0x38
    str r0, [r1],r2 ,lsl #2
    ldr r3, [r1],#0
    swi 0x123456