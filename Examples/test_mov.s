.global main
.text
main:
    mov r0, #0x12
    mov r1, #0x34
    swi 0x123456
