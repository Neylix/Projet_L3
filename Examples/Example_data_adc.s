.global main
.text

main:
    mov R2, #0xFFFFFFFF
	adds R2, R2, #0xF
    mov R0, #0xFFFFFFFF
	adcs R0, R0, #0 @# apres cette instruction Z doit valoir 1, C doit valoir 1 et R0 doit valoir 0
	movs R0, #0xFFFFFFFF
	adc R0, R0, #1 @# apres cette instruction Z doit valoir 0
    mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	movs R0, #0x7FFFFFFF
	adcs R0, R0, #0 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0x80000000
    mov R2, #0
	adds R2, R2, #0
    movs R0, #0x7FFFFFFF
	adcs R0, R0, #1 @# apres cette instruction V doit valoir 1 et R0 doit valoir 0x80000000

    mov R2, #0
	adds R2, R2, #0
	mov R0, #0xFFFFFF0F
	mov R1, #F
	adcs R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    mov R2, #0
	adds R2, R2, #0
	mov R0, #0xF0
	mov R1, #0xF0
	adc R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xFF
    mov R1, #0
	mov R0, #0xFFFFFFFF
	adc R0, R1, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xFFFFF
    mov R1, #0xFFF
	adc R0, R0, R1, ROR #12 @# apres cette instruction R0 doit valoir 0xFFFFFFFF
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
    mov R1, #0
	mov R0, #0xFFFFFFFF
	adc R0, R1, R0, RRX @# apres cette instruction R0 doit valoir 0
end:
    swi 0x123456
.data
