.global main
.text
main:
    mov r0, #0x5
    and r2,r0,#0x3
    swi 0x123456