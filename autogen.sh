#!/bin/sh

AM_VER=""
aclocal${AM_VER}
libtoolize --force
autoheader
autoconf
automake${AM_VER} --add-missing --copy
./configure $@
