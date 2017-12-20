.global main
.text
main:
    mov r0, #508
    mov r1, #508
    add r0, #2
    add r1, r0
    add r1, r0
    add r1, r0
    add r1, r0
    add r1, r0
    add r1, r0
    swi 0x123456
