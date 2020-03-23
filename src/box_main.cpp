#include "box_main.hpp"
#include "Servo.h"
#include "box_servomotor.hpp"
#include <Arduino.h>
#include <stdio.h>


#define LOWER_SERVO_PWM 9
#define LOWER_SERVO_MIN 0
#define LOWER_SERVO_MAX 133
#define LOWER_SERVO_CLOCKWISE true
#define UPPER_SERVO_PWM 10
#define UPPER_SERVO_MIN 42
#define UPPER_SERVO_MAX 180
#define UPPER_SERVO_CLOCKWISE true

box::Main::Main() {
    lower_servo = new box::Servomotor(LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                                      LOWER_SERVO_MIN, LOWER_SERVO_MAX);
    upper_servo = new box::Servomotor(UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                                      UPPER_SERVO_MIN, UPPER_SERVO_MAX);
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
}