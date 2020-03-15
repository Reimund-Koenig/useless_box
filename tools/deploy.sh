#!/bin/bash

screen -X quit
cd ~/workspace/useless_box/arduino
rm sketch -rf
mkdir sketch
cd sketch
cp ../useless_box.ino ./
cp ../Makefile ./
cp ../../include/* ./
cp ../../src/* ./
cp /usr/share/arduino/libraries/Servo/Servo.h ./
cp /usr/share/arduino/libraries/Servo/Servo.cpp ./
rm Arduino.h
rm Servo.h
make upload clean
cd ..
rm sketch -rf