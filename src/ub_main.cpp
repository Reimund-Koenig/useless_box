#include "Arduino.h"
#include "Servo.h"
#include "ub_servomotor.hpp"
#include "ub_main.hpp"

ub::Main::Main() {
    servo1 = new ub::Servomotor(pin_servomotor_pwm);
    pinMode(LED_BUILTIN, OUTPUT);
}

ub::Main::~Main() {
}

void ub::Main::run() {
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