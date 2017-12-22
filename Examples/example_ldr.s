.global main
.text
main:
    mov r0, #0x34
    mov r1, #0x45
    cmp r0,r1
    swi 0x123456