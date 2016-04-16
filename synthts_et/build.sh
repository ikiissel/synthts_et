#!/bin/sh
test -x configure || autoreconf -vif
./configure
make
