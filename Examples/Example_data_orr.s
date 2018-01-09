.global main
.text

main:
    movs R0, #1
    mov R0, #0
	orrs R0, R0, #0 @# apres cette instruction Z doit valoir 1 et R0 doit valoir 0
	mov R0, #0xFFFFFFFF
    movs R1, #0
	orrs R0, R0, R1 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    mov R0, #0xFFFFFFFF
    movs R1, #0
	orr R0, R0, R1 @# apres cette instruction N doit valoir 0
    mov R0, #0
    mov R1, #0xFFFFFFFF
    orrs R0, R1, R1 @# apres cette instruction R0 doit valoir 0xFFFFFFFF
    movs R0, #0
	mov R0, #0
	mov R1, #0xF
	orrs R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xF0
	mov R0, #0
	mov R1, #0xF0
	orr R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF0
    mov R0, #0
	mov R1, #0xFFFFFFFF
	orr R0, R0, R1, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xF
	orr R0, R0, R0, ROR #4 @# apres cette instruction R0 doit valoir 0xF000000F
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
    mov R1, #0
	mov R0, #0xFFFFFFFF
	orr R0, R1, R0, RRX @# apres cette instruction R0 doit valoir 0xFFFFFFFF
end:
    swi 0x123456
.data
