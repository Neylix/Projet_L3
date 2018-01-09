@# MAJ Z et N, 0^0 0^1 1^1 MAJ C shifter operand, #1, R1, LSL #1, ASR#1, ROR#1, RRX
.global main
.text

main:
    movs R0, #1
    mov R0, #0x0
	teq R0, R0, #0xF @# apres cette instruction Z doit valoir 1 et N doit valoir 0
	mov R0, #0xFFFFFFFF
    movs R1, #0
	teq R0, R0, R1 @# apres cette instruction N doit valoir 1 et Z doit valoir 0
    mov R0, #0x0
	teq R0, R0, #0xFF000000 @# apres cette instruction C doit valoir 1
	movs R0, #0x0
	teq R0, R0, #0 @# apres cette instruction C doit valoir 0
	movs R0, #0
	mov R0, #0x80000000
	teq R0, R0, R0, LSL #1 @# apres cette instruction N doit valoir 1
	movs R0, #0xFFFFFFFF
	teq R0, R0, R0, LSR #1 @# apres cette instruction N doit valoir 1
    movs R0, #0xFFFFFFFF
	teq R0, R0, R0, ASR #1 @# apres cette instruction N doit valoir 0
	movs R0, #0xFFFFFFFF
	teq R0, R0, R0, ROR #1 @# apres cette instruction N doit valoir 0
    mov R2, #0xFFFFFFFF
    adds R2, R2, #1
    mov R0, #0
	teq R0, R0, R0, RRX @# apres cette instruction N doit valoir 1
end:
    swi 0x123456
.data
