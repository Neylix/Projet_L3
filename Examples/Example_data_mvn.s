.global main
.text

main:
    movs R0, #1
    mov R0, #0xFFFFFFFF
	mvns R0, R0 @# apres cette instruction Z doit valoir 1 et R0 doit valoir 0
    movs R0, #0
	mvns R0, R0 @# apres cette instruction N doit valoir 1 et R0 doit valoir 0xFFFFFFFF
    movs R0, #0
	mvn R0, R0 @# apres cette instruction N doit valoir 0
    mov R0, #0xFFF0FFFF
	mvn R0, R1, R0 @# apres cette instruction R0 doit valoir 0xF0000
	movs R0, #F
	mvns R0, R0, LSL #4 @# apres cette instruction C doit valoir 1 et R0 doit valoir 0xFFFFFF0F
	mov R0, #0xFF
	mov R1, #0xF0
	mvn R0, R0, LSR #4 @# apres cette instruction R0 doit valoir 0xFFFFFFF0
	mov R0, #0xFFFFFFFF
	mvn R0, R0, R0, ASR #4 @# apres cette instruction R0 doit valoir 0
	mov R0, #0x1
	mvn R0, R0, ROR #4 @# apres cette instruction R0 doit valoir 0x7FFFFFFF
	mov R2, #0xFFFFFFFF
	adds R2, R2, #1
	mov R0, #0xFFFFFFFF
	mvn R0, R0, RRX @# apres cette instruction R0 doit valoir 0
end:
    swi 0x123456
.data
