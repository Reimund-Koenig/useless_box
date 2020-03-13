# UselessBox

## How to clone this project
    mkdir ~/workspace
    cd ~/workspace
    git clone https://Reimund_K@bitbucket.org/Reimund_K/useless_box.git

## How to run unit tests
    ~/workspace/useless_box/tools/ut.sh

## Run unit tests on every change
    ~/workspace/useless_box/tools/onChange_ut.sh


## Install Arduino related stuff
    sudo apt-get install gcc-avr avr-libc avrdude
    sudo apt install arduino-mk
    sudo apt install screen

### Compile code
    cd ~/workspace/useless_box/arduino
    make

### Compile and run code
    cd ~/workspace/useless_box/arduino
    make upload clean
#### or
    ~/workspace/useless_box/tools/deploy.sh