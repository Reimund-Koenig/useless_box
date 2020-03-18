#include "box_main.hpp"
#include "Servo.h"
#include "box_servomotor.hpp"
#include "Arduino.h"
#include <stdio.h>

box::Main::Main() {
    servo1 = new box::Servomotor(pin_servomotor_pwm, 0, 180);
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(0));
}

box::Main::~Main() {
}

void box::Main::run() {
    // print a random number from 1 to 100
    // long randNumber = random(100) + 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
    while(servo1->get_angle() < 180 ) {
        servo1->move_direction(SERVOMOTOR_DIRECTION_FORWARD);
    }
    delay(5000);
    while(servo1->get_angle() > 0 ) {
        servo1->move_direction(SERVOMOTOR_DIRECTION_BACKWARD);
    }
}