#! /usr/bin/bash

make
install --mode=444 libpolarimetric.so polarimetric.h polarimetric.py ../Test

exit $?