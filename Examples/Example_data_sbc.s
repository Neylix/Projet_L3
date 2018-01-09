.global main
.text

main:
    mov R2, #0
	adds R2, R2, #0
    mov R0, #1
	sbcs R0, R0, #0 @# apres cette instruction Z doit valoir 1, C doit valoir 1 et R0 doit valoir 0
	mov R0, #1
	sbcs R0, R0, #0 @# apres cette instruction Z doit valoir 0
    mov R2, #0
	adds R2, R2, #0
	mov R0, #0
	sbcs R0, R0, #0 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    mov R2, #0
	adds R2, R2, #0
    mov R0, #0x80000000
	sbcs R0, R0, #0 @# apres cette instruction V doit valoir 1 et R0 doit valoir 0x7FFFFFFF

    mov R2, #0xFFFFFFFF
    adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	mov R1, #F
	sbcs R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xFFFFFF0F
    mov R2, #0xFFFFFFFF
    adds R2, R2, #1
	mov R0, #0xFF
	mov R1, #0xF0
	sbc R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFFFFFFFF
	sbc R0, R0, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xFFFFFFFF
    mov R1, #0xFFF
	sbc R0, R0, R1, ROR #12 @# apres cette instruction R0 doit valoir 0xFFFFF
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	sbc R0, R0, R0, RRX @# apres cette instruction R0 doit valoir 0
end:
    swi 0x123456
.data