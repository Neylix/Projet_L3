.global main
.text
main:
    mov r0, #0xF000000F
    mov r1, #0xF000000F
    add r0, #2
    add r1, r0
    swi 0x123456
