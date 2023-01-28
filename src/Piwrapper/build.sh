#! /usr/bin/bash

make
install --mode=444 libpiwrapper.so piwrapper.hpp piwrapper.py ../Test

exit $?