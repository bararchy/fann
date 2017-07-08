# This makefile is on purpose not made with configure, to show how to use the library
# The make file requires that the fann library is installed (see ../README)

GCC=gcc 
TARGETS = xor_train xor_test xor_test_fixed simple_train steepness_train simple_test robot mushroom scaling_test scaling_train simple_test_som  simple_train_som simple_train_gng toy_test regression xor_train_mimo recurrent_basic_test recurrent_hopfield_test recurrent_rtrl_test recurrent_unrolled_test 
# cascade_train

#compiled against installed MIMO fann
DEBUG_TARGETS = $(TARGETS:%=%_debug) # MIMO debug version
DEBUGEX_TARGETS = $(TARGETS:%=%_debugex) # MIMO library debug version
OLD_TARGETS = $(TARGETS:%=%_old) # compiled against old fann
NEW_TARGETS = $(TARGETS:%=%_new) # compiled against source MIMO fann

VERBOSE  ?= 0
OPTIMIZE ?= 4
H_DIM    ?= 2
DATA     ?= xor.data

DEFINES := -DVERBOSE=$(VERBOSE) -DOPTIMIZE=$(OPTIMIZE) -DH_DIM=$(H_DIM)

all: $(TARGETS)

MIMO_PREFIX ?= /usr/local
OLD_CFLAGS = $(DEFINES) -march=pentium4 -funroll-loops
OLD_LDFLAGS = -lm -lfloatfann
MIMO_CFLAGS = $(DEFINES) -I${MIMO_PREFIX}/include/fann/ -funroll-loops 
MIMO_LDFLAGS = -L${MIMO_PREFIX}/lib -lm -ldl -lfloatfann2 -lfloatfann2_scalar -Wl,-rpath=${MIMO_PREFIX}/lib

FANN_C_FILES = ../src/floatfann.c ../src/optimized/scalar/fann_scalar_mimo.c

ifeq ($(OPTIMIZE),4)
	MIMO_CFLAGS += -march=pentium4 
	MIMO_LDFLAGS += -lfloatfann2_sse
	FANN_C_FILES += ../src/optimized/sse/fann_sse_mimo.c
endif

ifeq ($(OPTIMIZE),3)
	MIMO_LDFLAGS += -lfloatfann2_blas -lcblas
	FANN_C_FILES += ../src/optimized/blas/fann_blas_mimo.c
endif

%: %.c Makefile
	$(GCC) $(CFLAGS) $(LDFLAGS) $(MIMO_CFLAGS) $(MIMO_LDFLAGS) -O3 $< -o $@

%_old: %.c Makefile
	$(GCC) $(OLD_CFLAGS) $(OLD_LDFLAGS) -O3 $< -o $@ 

%_fixed: %.c Makefile
	$(GCC) $(CFLAGS) $(LDFLAGS) $(MIMO_CFLAGS) $(MIMO_LDFLAGS) -O3 -DFIXEDFANN $< -o $@

#removed -lcblas here
%_debug: %.c Makefile ../src/*c ../src/include/*h
	$(GCC) -I../src/ -I../src/include/ -DDEBUG  -Wall -gdwarf-2 -g3 $(FANN_C_FILES) $< -o $@ -lm -ldl  $(MIMO_CFLAGS) -rdynamic

%_debug_ex: %.c Makefile
	$(GCC) $(CFLAGS) $(LDFLAGS) $(MIMO_CFLAGS) $(MIMO_LDFLAGS) -DDEBUG -Wall  -gdwarf-2 -g3 -O0 $< -o $@

#removed -lcblas here
%_new: %.c Makefile ../src/*c ../src/include/*h
	$(GCC) -O3 -Wall  -I../src/ -I../src/include/ $(FANN_C_FILES) $< -o $@ -lm -ldl  $(MIMO_CFLAGS) -rdynamic

#removed -lcblas here
%_fixed_debug: %.c Makefile ../src/*c ../src/include/*h
	$(GCC) -ggdb -DDEBUG -Wall  -DFIXEDFANN -I../src/ -I../src/include/ ../src/fixedfann.c $< -o $@ -lm -ldl  $(MIMO_CFLAGS)

clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) $(NEW_TARGETS) xor_fixed.data *.net *~ *.obj *.exe *.tds noscale.txt withscale.txt scale_test_results.txt

runtest: $(TARGETS)
	@echo
	@echo Training network
	./xor_train

	@echo
	@echo Testing network with floats
	./xor_test

	@echo
	@echo Testing network with fixed points
	./xor_test_fixed

#below this line is only for debugging the fann library

rundebugtest: $(DEBUG_TARGETS)
	@echo
	@echo Training network
	./xor_train_debug

	@echo
	@echo Testing network with floats
	./xor_test_debug

	@echo
	@echo Testing network with fixed points
	./xor_test_fixed_debug

runcompliancetest: $(DEBUG_TARGETS)
	./xor_train_debug

	@echo
	@echo Testing network with floats
	@cp xor_float.net xor_float_new.net
	@cp xor_float_backup.net xor_float.net
	@echo RUN
	@./xor_test_debug | grep XOR
	@echo CAT
	@cat xor_float_backup.out

#used to compare result when scale used and not used
runscaletest: scaling_train scaling_test
	echo Without scaling > noscale.txt
	./scaling_train >> noscale.txt
	./scaling_test >> noscale.txt
	echo With scaling > withscale.txt
	./scaling_train scale >> withscale.txt
	./scaling_test scale >> withscale.txt
	pr -m -t -T -w 150 noscale.txt withscale.txt > scale_test_results.txt
	@echo See scale_test_results.txt for results of scaling test

#compiletest is used to test whether the library will compile easily in other compilers
compiletest:
	gcc -O3 -ggdb -DDEBUG -Wall -Wformat-security -Wshadow -Wpointer-arith -Wcast-qual -Wsign-compare -pedantic -I../src/ -I../src/include/ ../src/floatfann.c xor_train.c -o xor_train -lm -ldl 
	gcc -O3 -ggdb -DDEBUG -Wall -Wformat-security -Wshadow -Wpointer-arith -Wcast-qual -Wsign-compare -pedantic -DFIXEDFANN -I../src/ -I../src/include/ ../src/fixedfann.c xor_test.c -o xor_test -lm -ldl
	g++ -O3 -ggdb -DDEBUG -Wall -Wformat-security -Wpointer-arith -Wcast-qual -Wsign-compare -pedantic -I../src/ -I../src/include/ ../src/floatfann.c xor_train.c -o xor_train -lm -ldl

quickcompiletest:
	gcc -O -ggdb -DDEBUG -Wall -Wformat-security -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-qual -Wsign-compare -pedantic  -I../src/ -I../src/include/ ../src/floatfann.c ../examples/xor_train.c -o ../examples/xor_train -lm 

debug: $(DEBUG_TARGETS)

old: $(OLD_TARGETS)

new: $(NEW_TARGETS)


rundebug: $(DEBUG_TARGETS)
	@echo
	@echo Training network
	valgrind --leak-check=yes --show-reachable=yes --leak-resolution=high --db-attach=yes ./xor_train_debug

	@echo
	@echo Testing network with floats
	valgrind --leak-check=yes --show-reachable=yes --leak-resolution=high --db-attach=yes ./xor_test_debug

	@echo
	@echo Testing network with fixed points
	valgrind --leak-check=yes --show-reachable=yes --leak-resolution=high --db-attach=yes ./xor_test_fixed_debug

comparetest: regression_old regression_new
	./regression_new $(DATA) | tee new_output && ./regression_old $(DATA) | tee old_output
	diff old_output new_output
