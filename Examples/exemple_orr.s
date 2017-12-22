.global main
.text
main:
    mov r0, #0x5
    mov r1, #0x3
    eor r2, r0, r1  
    swi 0x123456