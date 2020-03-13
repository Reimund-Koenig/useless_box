#!/bin/sh

rm ~/workspace/useless_box/test/build -rf
cd ~/workspace/useless_box/test
mkdir build
cd build
cmake ..
make
make test