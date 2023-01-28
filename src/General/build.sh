#! /usr/bin/bash

make
install --mode=444 libsignalprocessing.so signalprocessing.h signalprocessing.py ../Test

exit $?