#!/bin/bash

screen -X quit
cd ~/workspace/useless_box/arduino/useless_box_systemtest
rm sketch -rf
mkdir sketch
cd sketch
cp ../useless_box.ino ./
cp ../Makefile ./
cp ../../../include/* ./
cp ../../../src/* ./
rm Arduino.h
rm Servo.h
rm Serial.h
rm box_gearmotor.cpp
rm box_gearmotor.hpp
rm main.cpp
rm main.hpp
cp /usr/share/arduino/libraries/Servo/Servo.h ./
cp /usr/share/arduino/libraries/Servo/Servo.cpp ./
make upload clean
cd ..
rm sketch -rf