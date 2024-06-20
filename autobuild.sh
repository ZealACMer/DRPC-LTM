#!/bin/bash

set -e

rm -rf `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make
cd ..

#lib中的文件可以放在/usr/lib下，或者/usr/local/lib下
#include中的文件可以放在/usr/include，或者/usr/local/include下
cp -r `pwd`/include `pwd`/lib