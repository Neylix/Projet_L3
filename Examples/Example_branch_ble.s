.global main
.text

proc:
    bal end
main:
    @#branchement si Z ou (N != V)       (N et Z impossible)
	movs R0, #0xFFFFFFFF
    adds R0, R0, #0xFFFFFFFF
    blt main @# non(Z) et N et V : pas de branchement
    movs R0, #1
    adds R0, R0, #1
    blt main @# non(Z) et non(N) et non(V) : pas de branchement
	movs R0, #0xFFFFFFFF
    adds R0, R0, #0
    blt test2 @# non(Z) et N et non(V) : branchement
    bal main
test2:
    movs R0, #0
    adds R0, R0, #0
    blt test3 @# Z et non(N) et non(V) : branchement
    bal main
test3:
	movs R0, #0x80000000
    adds R0, R0, #0x80000000
    blt test4 @# Z et non(N) et V : branchement
    bal main
test4:
	movs R0, #0x80000000
    adds R0, R0, #0x80000001
    blt proc @# non(Z) et non(N) et V : branchement
    bal main
end:
    swi 0x123456
.data
