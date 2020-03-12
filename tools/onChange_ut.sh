#!/bin/sh

cd ~/workspace/useless_box/test
mkdir build
cd build
cmake ..
make test
cd ~/workspace/useless_box/
while inotifywait -e close_write -r ./;do { cd ~/workspace/useless_box/test/build && make && make test && cd ~/workspace/useless_box; } || { cd ~/workspace/useless_box/; } ;done