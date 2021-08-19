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

sudo apt-get install gcc-avr avr-libc avrdude &&
sudo apt install arduino-mk &&
sudo apt install screen &&
sudo apt install cmake &&
sudo apt install libgcc-8-dev &&
sudo apt-get install build-essential &&
sudo apt-get install g++

### Download and install Arduino IDE: https://www.arduino.cc/en/Main/Software

### Grant access to Serial_Port

* sudo chmod a+rw /dev/ttyACM0
* sudo chmod a+rw /dev/ttyUSB0
* sudo chmod a+rw /dev/ttyS4  # WSL (Windows Subsystem Linux)

### Compile code

    cd ~/workspace/useless_box/arduino/useless_box
    make

### Debug with print

    * Add following line to *.ino files function "setup()":
    ** Serial.begin(9600);
    * See Makefile for right tty* and execute following command while connected to the box
    ** sudo screen /dev/ttyS4 9600
    * End debugging
    ** CTRL+a then type :quit
    * OR
    ** CTRL+a then CTRL+d   // to detach the session
    ** sudo screen -X quit  // to end it


### Compile and run code

    cd ~/workspace/useless_box/arduino
    make upload clean

#### or

    ~/workspace/useless_box/tools/deploy.sh
