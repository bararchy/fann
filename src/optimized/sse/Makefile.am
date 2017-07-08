#SUBDIRS = include optimized

lib_LTLIBRARIES = libfloatfann2_sse.la

AM_LDFLAGS = -version-info 2:1:0


AM_CFLAGS = -D_REENTRANT -march=pentium4 -O3 -march=pentium4 -mfpmath=sse -funroll-loops
if X86_64
AM_CFLAGS += -fPIC
endif
libfloatfann2_sse_la_SOURCES = fann_sse_mimo.c
#libdoublefann_la_SOURCES = doublefann.c
#libfixedfann_la_SOURCES = fixedfann.c
#libfann_la_SOURCES = floatfann.c

#EXTRA_DIST = fann.c fann_io.c fann_train.c fann_train_data.c fann_error.c fann_cascade.c fann_som.c
