CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER

noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort exampleCustom Example_branch_bge Example_branch_bgt Example_branch_bhi Example_branch_bls Example_branch_ble

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
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
