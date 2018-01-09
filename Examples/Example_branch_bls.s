.global main
.text

proc:
    bal end
main:
    @#branchement si non(C) ou Z
	movs R0, #0xF0000001
    adds R0, R0, #0x10000000
    bls main @#C et non(Z) : pas de branchement
    movs R0, #0
    adds R0, R0, #0
    bls test2 @#non(C) et Z : branchement
	bal main
test2:
	movs R0, #1
    adds R0, R0, #1
    bls test3 @#non(C) et non(Z) : branchement
	bal main
test3:
    movs R0, #0xFFFFFFFF
    adds R0, R0, #1
    bls main @#C et Z : branchement
    bal main
end:
    swi 0x123456
.data
