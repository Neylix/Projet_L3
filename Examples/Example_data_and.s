.global main
.text

main:
    movs R0, #1
    mov R0, #0x0
	ands R0, R0, #0xF @# apres cette instruction Z doit valoir 1
	movs R0, #0
	mov R0, #0xFFFFFFFF
	ands R0, R0, R0 @# apres cette instruction N doit valoir 1
    mov R0, #0x0
	and R0, R0, #0xF @# apres cette instruction R0 doit valoir 0
	mov R0, #0xFF
	and R0, R0, #0xF @# apres cette instruction R0 doit valoir 0xF
    mov R0, #0xFF
	mov R1, #0xF
	and R0, R0, R1 @# apres cette instruction R0 doit valoir 0xF
	mov R0, #0xFF
	mov R1, #0xF
	and R0, R0, R1, LSL #4 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFF
	mov R1, #0xF
	mov R2, #4
	and R0, R0, R1, LSL R2 @# apres cette instruction R0 doit valoir 0xF0
	mov R0, #0xFF
	mov R1, #0xF0
	and R0, R0, R1, LSR #4 @# apres cette instruction R0 doit valoir 0xF
	mov R0, #0xFF
	mov R1, #0xF0
	mov R2, #4
	and R0, R0, R1, LSR R2 @# apres cette instruction R0 doit valoir 0xF
	mov R0, #0xFFFFFFFF
	mov R1, #0xFFFFFFFF
	and R0, R0, R1, ASR #4 @# apres cette instruction R0 doit valoir 0xFFFFFFFF
	mov R0, #0xFFFFFFFF
	mov R1, #0xFFFFFFFF
	mov R2, #4
	and R0, R0, R1, ASR R2 @# apres cette instruction R0 doit valoir 0xFFFFFFFF
	mov R0, #0xF0000000
	mov R1, #0x0000000F
	and R0, R0, R1, ROR #4 @# apres cette instruction R0 doit valoir 0xF0000000
	mov R0, #0xF0000000
	mov R1, #0x0000000F
	mov R2, #4
	and R0, R0, R1, ROR R2 @# apres cette instruction R0 doit valoir 0xF0000000
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	mov R1, #0xFFFFFFFF
	and R0, R0, R1, RRX @# apres cette instruction R0 doit valoir 0xFFFFFFFF

    mov R0, #0x0
	ands R0, R0, #0xFF000000 @# apres cette instruction C doit valoir 1
	movs R0, #0x0
	and R0, R0, #0xFF000000 @# apres cette instruction C doit valoir 0
	movs R0, #0
	mov R0, #0xFFFFFFFF
	ands R0, R0, R0, LSL #1 @# apres cette instruction C doit valoir 1
	movs R0, #0
	mov R0, #0xFFFFFFFF
	mov R1, #1
	ands R0, R0, R0, LSL R1 @# apres cette instruction C doit valoir 1
	movs R0, #0xF
	ands R0, R0, R0, LSR #1 @# apres cette instruction C doit valoir 1
	movs R0, #0xF
	mov R1, #1
	ands R0, R0, R0, LSR R1 @# apres cette instruction C doit valoir 1
    movs R0, #0xF
	ands R0, R0, R0, ASR #1 @# apres cette instruction C doit valoir 1
	movs R0, #0xF
	mov R1, #1
	ands R0, R0, R0, ASR R1 @# apres cette instruction C doit valoir 1
	movs R0, #0xF
	ands R0, R0, R0, ROR #1 @# apres cette instruction C doit valoir 1
	movs R0, #0xF
	mov R1, #1
	ands R0, R0, R0, ROR R1 @# apres cette instruction C doit valoir 1
    movs R0, #0xF
	ands R0, R0, R0, RRX @# apres cette instruction C doit valoir 1
end:
    swi 0x123456
.data
