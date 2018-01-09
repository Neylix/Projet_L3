.global main
.text

main:
    movs R0, #1
    mov R0, #0x0
	eors R0, R0, #0xF @# apres cette instruction Z doit valoir 1 et R0 doit valoir 0
	mov R0, #0xFFFFFFFF
    movs R1, #0
	eors R0, R0, R1 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    mov R0, #0xFFFFFFFF
    movs R1, #0
	eor R0, R0, R1 @# apres cette instruction N doit valoir 0
    movs R0, #0
	mov R0, #0xFFFFFFFF
	mov R1, #F
	eors R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xFFFFFF0F
	mov R0, #0xFF
	mov R1, #0xF0
	eor R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFFFFFFFF
	eor R0, R0, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xF
	eor R0, R0, R0, ROR #4 @# apres cette instruction R0 doit valoir 0xF000000F
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	eor R0, R0, R0, RRX @# apres cette instruction R0 doit valoir 0xFFFFFFFF
end:
    swi 0x123456
.data
