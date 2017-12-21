.global main
.text
main:
    mov r0, #32
    mov r1, r0, LSL #26
    mov r2, r0, LSL #26
    adds r1, r2
    swi 0x123456
