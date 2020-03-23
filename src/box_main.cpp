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
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12
#define PIN_SWITCH 4

box::Main::Main() {
    box_lower_servo = new box::Servomotor(LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                                      LOWER_SERVO_MIN, LOWER_SERVO_MAX);
    box_upper_servo = new box::Servomotor(UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                                      UPPER_SERVO_MIN, UPPER_SERVO_MAX);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    box_switch = new box::Switch(PIN_SWITCH);
    randomSeed(analogRead(0));
}

box::Main::~Main() {
}

void box::Main::run() {
    if(box_switch->get_state() == HIGH) {
        box_upper_servo->move_to_percent(100);
    } else {
        box_upper_servo->move_to_percent(0);
    }
}