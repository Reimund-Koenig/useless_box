#include "box_sonar.hpp"
#include "Arduino.h"
#include <stdio.h>

box::Sonar::Sonar(int pin_trigger, int pin_echo) {
    box::Sonar::pin_trigger = pin_trigger;
    box::Sonar::pin_echo = pin_echo;
    pinMode(box::Sonar::pin_trigger, OUTPUT);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    pinMode(box::Sonar::pin_echo, INPUT);
}

box::Sonar::~Sonar(){
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

unsigned int box::Sonar::get_distance_cm(){
    long duration;
    digitalWrite(box::Sonar::pin_trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(box::Sonar::pin_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    duration = pulseIn(box::Sonar::pin_echo, HIGH);
    return (duration/2.0)*0.0343;
}
