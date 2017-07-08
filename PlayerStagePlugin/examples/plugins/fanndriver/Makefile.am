PLAYERC_CFLAGS = -I$(top_srcdir) \
                 -I$(top_srcdir)/client_libs/

PLAYERC_LIB_DIRS=-L${top_builddir}/libplayerxdr \
                 -L${top_builddir}/libplayercore \
                 -L${top_builddir}/client_libs/libplayerc

PLAYERC_DEPS = $(top_builddir)/client_libs/libplayerc/libplayerc.la \
               $(top_builddir)/libplayerxdr/libplayerxdr.la \
               $(top_builddir)/libplayercore/libplayererror.la

AM_CPPFLAGS = $(PLAYERC_CFLAGS) -Wall -g -I$(top_srcdir)

exampledir = $(prefix)/share/player/examples/plugins/fanndriver

example_LTLIBRARIES = libfanndriver.la
example_DATA = fanndriver.cc fanndriver.cfg
EXTRA_DIST = fanndriver.cfg

libfanndriver_la_SOURCES = fanndriver.cc sharedstruct.h
libfanndriver_la_LIBADD = -lfann
