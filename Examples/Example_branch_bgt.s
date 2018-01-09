.global main
.text

proc:
    bal end
main:
    @#branchement si non(Z) et (N == V)        (Z et N impossible)
	movs R0, #0xFFFFFFFF
    adds R0, R0, #0
    bge main @# non(Z) et N et non(V) : pas de branchement
	movs R0, #0x80000000
    adds R0, R0, #0x80000000
    bge main @# Z et non(N) et V : pas de branchement
	movs R0, #0x80000001
    adds R0, R0, #0x80000001
    bge main @# non(Z) et non(N) et V : pas de branchement
    movs R0, #0xFFFFFFFF
    adds R0, R0, #0xFFFFFFFF
    bge test2 @# non(Z) et N et V : branchement
	bal main
test2:
    movs R0, #1
    adds R0, R0, #1
    bge proc @#non(Z) et non(N) et non(V) : branchement
    bal main
end:
    swi 0x123456
.data