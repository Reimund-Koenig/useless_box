#include "Arduino.h"
#include "Servo.h"
#include "box_servomotor.hpp"
#include "box_main.hpp"

box::Main::Main() {
    servo1 = new box::Servomotor(pin_servomotor_pwm);
    pinMode(LED_BUILTIN, OUTPUT);
}

box::Main::~Main() {
}

void box::Main::run() {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
    while(servo1->get_angle() < 180 ) {
        servo1->move(SERVOMOTOR_DIRECTION_FORWARD);
    }
    delay(5000);
    while(servo1->get_angle() > 0 ) {
        servo1->move(SERVOMOTOR_DIRECTION_BACKWARD);
    }
}