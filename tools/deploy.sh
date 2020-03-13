#!/bin/bash

screen -X quit
cd ~/workspace/useless_box/arduino
mkdir sketch
cd sketch
cp ../useless_box.ino ./
cp ../Makefile ./
cp ../../include/* ./
cp ../../src/* ./
rm Arduino.h
rm Servo.h
make upload clean
cd ..
rm sketch -rf