.global main
.text

proc:
    bal end
main:
    @#branchement si N != V
	movs R0, #0x7FFFFFFF
    adds R0, R0, #0x7FFFFFFF
    blt main @# N et V : pas de branchement
    movs R0, #1
    adds R0, R0, #1
    blt main @#non(N) et non(V) : pas de branchement
	movs R0, #0xFFFFFFFF
    adds R0, R0, #0
    blt test2 @# N et non(V) : branchement
    bal main
test2:
	movs R0, #0x80000000
    adds R0, R0, #0x80000001
    blt proc @# non(N) et V : branchement
    bal main
end:
    swi 0x123456
.data
