#SUBDIRS = include optimized

lib_LTLIBRARIES = libfloatfann2_blas.la

AM_LDFLAGS = -version-info 2:1:0


AM_CFLAGS = -D_REENTRANT
if X86_64
AM_CFLAGS += -fPIC
endif
libfloatfann2_blas_la_SOURCES = fann_blas_mimo.c
#libdoublefann_la_SOURCES = doublefann.c
#libfixedfann_la_SOURCES = fixedfann.c
#libfann_la_SOURCES = floatfann.c

#EXTRA_DIST = fann.c fann_io.c fann_train.c fann_train_data.c fann_error.c fann_cascade.c fann_som.c
