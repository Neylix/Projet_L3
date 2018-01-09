.global main
.text

main:
    movs R0, #0
    mov R0, #0xFFFFFFFF
	cmn R0, R0, #1 @# apres cette instruction Z doit valoir 1, C doit valoir 1
	movs R0, #0xFFFFFFFF
	cmn R0, R0, #0 @# apres cette instruction N doit valoir 1
    movs R0, #0x80000000
	cmn R0, R0, R0 @# apres cette instruction V doit valoir 1

	movs R0, #0
	mov R0, #0x40000000
	cmn R0, R0, R0, LSL #1 @# apres cette instruction N doit valoir 1
	movs R0, #0x80000000
	cmn R0, R0, R0, LSR #1 @# apres cette instruction N doit valoir 1
    movs R0, #0x80000000
	cmn R0, R0, R0, ASR #1 @# apres cette instruction N doit valoir 0
	movs R0, #0xF
	cmn R0, R0, R0, ROR #1 @# apres cette instruction N doit valoir 1
    mov R2, #0xFFFFFFFF
    adds R2, R2, #1
    mov R0, #0x80000000
	cmn R0, R0, R0, RRX @# apres cette instruction N doit valoir 0
end:
    swi 0x123456
.data