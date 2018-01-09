.global main
.text

main:
    movs R0, #1
	subs R0, R0, R0 @# apres cette instruction Z doit valoir 1, C doit valoir 1 et R0 doit valoir 0
	movs R0, #1
	sub R0, R0, R0 @# apres cette instruction Z doit valoir 0
	movs R0, #0
	subs R0, R0, #1 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    movs R0, #0x80000000
	subs R0, R0, #1 @# apres cette instruction V doit valoir 1 et R0 doit valoir 0x7FFFFFFF

    movs R0, #0
	mov R0, #0xFFFFFFFF
	mov R1, #F
	subs R0, R0, R1, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xFFFFFF0F
	mov R0, #0xFF
	mov R1, #0xF0
	sub R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFFFFFFFF
	sub R0, R0, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0xFFFFFFFF
    mov R1, #0xFFF
	sub R0, R0, R1, ROR #12 @# apres cette instruction R0 doit valoir 0xFFFFF
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	sub R0, R0, R0, RRX @# apres cette instruction R0 doit valoir 0
end:
    swi 0x123456
.data
