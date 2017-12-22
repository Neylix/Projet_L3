CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 test_mov insertion_sort example_and example_eor example_orr

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
test_mov_SOURCES=test_mov.s
insertion_sort_SOURCES=insertion_sort.c
example_and_SOURCES=example_and.s
example_eor_SOURCES=example_eor.s
example_orr_SOURCES=example_orr.s
EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
