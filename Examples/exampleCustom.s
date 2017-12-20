.global main
.text
main:
    and r0, r1, r2
    swi 0x123456
.data
