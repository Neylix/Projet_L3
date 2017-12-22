CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
<<<<<<< HEAD
noinst_PROGRAMS=example1 example2 example3 example4 example_ldr test_mov insertion_sort
=======
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort exampleCustom
>>>>>>> nils00

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
<<<<<<< HEAD
example_ldr_SOURCES=example_ldr.s
test_mov_SOURCES=test_mov.s
=======
>>>>>>> nils00
insertion_sort_SOURCES=insertion_sort.c
exampleCustom_SOURCES=exampleCustom.s

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
