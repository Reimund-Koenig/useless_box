#!/bin/bash

my_link=/usr/share/arduino/libraries/UselessBox
if [ -L ${my_link} ] ; then
   if [ -e ${my_link} ] ; then
      screen -X quit
      cd ~/workspace/useless_box/arduino/useless_box
      rm sketch -rf
      mkdir sketch
      cd sketch
      cp ../useless_box.ino ./
      cp ../Makefile ./
      make upload clean
      cd ..
      rm sketch -rf
   else
      echo "Broken link"
      echo "Run ./init_uselessbox_build_framework"
   fi
elif [ -e ${my_link} ] ; then
   echo "Not a link"
      echo "Run ./init_uselessbox_build_framework"
else
   echo "Missing"
   echo "Run ./init_uselessbox_build_framework"
fi
