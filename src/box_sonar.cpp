#include "box_sonar.hpp"
#include "Arduino.h"
#include "stdio.h"

static box::Sonar* sonar_instance;

box::Sonar::Sonar(int pin_trigger, int pin_echo, int pin_interrupt, int max_dist) {
    box::Sonar::pin_trigger = pin_trigger;
    box::Sonar::pin_interrupt = pin_interrupt;
    box::Sonar::pin_echo = pin_echo;
    box::Sonar::max_distance = max_distance;
    box::Sonar::finished = false;
    pinMode(box::Sonar::pin_trigger, OUTPUT);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    pinMode(box::Sonar::pin_echo, INPUT);
    // TODO !! its recommented to use digitalPinToInterrupt !!
    // attachInterrupt(digitalPinToInterrupt(box::Sonar::pin_interrupt),
    //             box::Sonar::echo_isr, CHANGE);
    attachInterrupt(box::Sonar::pin_interrupt, box::Sonar::echo_isr, CHANGE);
    sonar_instance = this;
}

box::Sonar::~Sonar(){
    sonar_instance = nullptr;
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Sonar::start(){
  box::Sonar::finished=false;
  digitalWrite(box::Sonar::pin_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(box::Sonar::pin_trigger, LOW);
}

bool box::Sonar::isFinished(){
    return box::Sonar::finished;
}

unsigned int box::Sonar::getRange(bool units){
    return (box::Sonar::endtime - box::Sonar::starttime)/((units)?58:148);
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Sonar::echo_isr(){
    Sonar* instance = sonar_instance;
    switch(digitalRead(instance->pin_echo)){
        case HIGH:
            instance->starttime = micros();
            break;
        case LOW:
            instance->endtime=micros();
            instance->finished=true;
            break;
    }
}
