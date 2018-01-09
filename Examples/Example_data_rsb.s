.global main
.text

main:
    movs R0, #1
	rsbs R0, R0, R0 @# apres cette instruction Z doit valoir 1, C doit valoir 1 et R0 doit valoir 0
	movs R0, #1
	rsb R0, R0, R0 @# apres cette instruction Z doit valoir 0
	movs R0, #1
	rsbs R0, R0, #0 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    movs R0, #0x80000000
	rsbs R0, R0, #0 @# apres cette instruction V doit valoir 1 et R0 doit valoir 0x80000000

    movs R0, #0
	mov R0, #0xF0
	mov R1, #0xFF
	rsbs R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xF00
	mov R0, #0xF
	mov R1, #0xFFF
	rsb R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFFFFFFFF
	rsb R0, R0, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xF0000000
    mov R1, #0xF
	rsb R0, R0, R1, ROR #4 @# apres cette instruction R0 doit valoir 0
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	rsb R0, R0, R0, RRX @# apres cette instruction R0 doit valoir 0
end:
    swi 0x123456
.data
