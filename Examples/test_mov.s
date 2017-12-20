.global main
.text
main:
    mov r0, #0xFF000000
    mov r1, #0xFF000000
    add r0, #2
    add r1, r0
    swi 0x123456
