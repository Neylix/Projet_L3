.global main
.text

main:
    movs R0, #1
    mov R0, #0x0
	tst R0, R0, #0xF @# apres cette instruction Z doit valoir 1 et N doit valoir 0
	movs R0, #0
	mov R0, #0xFFFFFFFF
	tst R0, R0, R0 @# apres cette instruction N doit valoir 1 et Z doit valoir 0
    mov R0, #0x0
	tst R0, R0, #0xFF000000 @# apres cette instruction C doit valoir 1
	movs R0, #0x0
	tst R0, R0, #0 @# apres cette instruction C doit valoir 0
	movs R0, #0
	mov R0, #0xFFFFFFFF
	tst R0, R0, R0, LSL #1 @# apres cette instruction N doit valoir 1
	movs R0, #0xFFFFFFFF
	tst R0, R0, R0, LSR #1 @# apres cette instruction N doit valoir 0
    movs R0, #0xFFFFFFFF
	tst R0, R0, R0, ASR #1 @# apres cette instruction N doit valoir 1
	movs R0, #0xF
	tst R0, R0, R0, ROR #1 @# apres cette instruction N doit valoir 1
    mov R2, #0xFFFFFFFF
    adds R2, R2, #1
    mov R0, #0xFFFFFFFF
	tst R0, R0, R0, RRX @# apres cette instruction N doit valoir 1
end:
    swi 0x123456
.data
