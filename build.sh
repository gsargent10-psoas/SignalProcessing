#! /usr/bin/bash

# Maintain pointer to current directory.
current_d=$(pwd)

# Colors: black=30;red=31;green=32;yellow=33;blue=34;magenta=35;cyan=36;white=37; \e[1;<num>m]

# Start the build process and inform the user of the tests to be performed.
greeting="Building all of SignalProcessing..."
build_list="Build List:"
echo -e $greeting
echo -e $build_list

# Get all nested directories that have a build.sh.
count=1
for d in $(find ./src -maxdepth 1 -type d)
do
    echo -e "\t\e[1;34m$count. $d"
    let count=count+1
done

# Run all build.sh and get results.
count=1
succeed=0
for d in $(find ./src -maxdepth 1 -type d)
do
    echo -e "\e[1;37m\nBuilding \e[1;34m$d\e[1;37m..."
    cd $d
    ./build.sh
    if [ $?==0 ]
    then
        echo -e "\e[1;32m\n$d Build Succeeded!\e[1;37m"
    else
        echo -e "\e[1;31m\n$d Build Failed!\e[1;37m"
        succeed=1
    fi
    cd $current_d
    let count=count+1
done

# Print out results.
if [ $succeed==0 ]
then
    echo -e "\e[1;32m\nAll Builds Succeeded!\e[1;37m"
else
    echo -e "\e[1;31m\nBuild(s) Failed!\e[1;37m"
fi