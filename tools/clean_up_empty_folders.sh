#!/bin/bash

echo Delete all emtpy folders inside ~/workspace/useless_box

cd ~/workspace/useless_box
find . -type d -empty -delete