#!/bin/bash

aclocal
autoheader
autoconf
libtoolize --automake
automake --add-missing
