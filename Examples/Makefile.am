CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
<<<<<<< HEAD
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort exampleCustom Example_branch_bge Example_branch_bgt Example_branch_bhi Example_branch_bls Example_branch_ble
=======
noinst_PROGRAMS=example1 example2 example3 example4 example_ldr test_mov insertion_sort

>>>>>>> c79a5d0494469a00a179b5e55a7aa8535980d6e5

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example_ldr_SOURCES=example_ldr.s
test_mov_SOURCES=test_mov.s

insertion_sort_SOURCES=insertion_sort.c
exampleCustom_SOURCES=exampleCustom.s
Example_branch_bge_SOURCES=Example_branch_bge.s
Example_branch_bgt_SOURCES=Example_branch_bgt.s
Example_branch_bhi_SOURCES=Example_branch_bhi.s
Example_branch_bls_SOURCES=Example_branch_bls.s
Example_branch_ble_SOURCES=Example_branch_ble.s

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
