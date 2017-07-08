SUBDIRS = include optimized

lib_LTLIBRARIES = libfloatfann2.la libdoublefann2.la libfixedfann2.la libfann2.la

AM_LDFLAGS = -version-info 2:1:0


if X86_64
X86_CFLAGS = -fPIC
endif
AM_CFLAGS = -D_REENTRANT ${X86_CFLAGS}
libfloatfann2_la_SOURCES = floatfann.c
libdoublefann2_la_SOURCES = doublefann.c
libfixedfann2_la_SOURCES = fixedfann.c
libfann2_la_SOURCES = floatfann.c

EXTRA_DIST = fann.c fann_io.c fann_train.c fann_train_data.c fann_error.c fann_cascade.c fann_som.c
