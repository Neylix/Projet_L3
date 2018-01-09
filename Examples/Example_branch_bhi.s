.global main
.text

proc:
    bal end
main:
    @#branchement si C et non(Z)
    movs R0, #0
    adds R0, R0, #0
    bhi main @#non(C) et Z : pas de branchement
	movs R0, #1
    adds R0, R0, #1
    bhi main @#non(C) et non(Z) : pas de branchement
    movs R0, #0xFFFFFFFF
    adds R0, R0, #1
    bhi main @#C et Z : pas de branchement
	movs R0, #0xF0000001
    adds R0, R0, #0x10000000
    bhi proc @#C et non(Z) : branchement
    bal main
end:
    swi 0x123456
.data
