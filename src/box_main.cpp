#include "box_main.hpp"
#include "Servo.h"
#include "box_servomotor.hpp"
#include <Arduino.h>
#include <stdio.h>


#define LOWER_SERVO_PWM 9
#define LOWER_SERVO_MIN 0
#define LOWER_SERVO_MAX 138
#define LOWER_SERVO_CLOCKWISE false
#define UPPER_SERVO_PWM 10
#define UPPER_SERVO_MIN 42
#define UPPER_SERVO_MAX 180
#define UPPER_SERVO_CLOCKWISE true
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12
#define PIN_SWITCH 4

#define MODE_RESET 0
#define MODE_AWARENESS 1
#define MODE_NORMAL 2

box::Main::Main() {
    box_lower_servo = new box::Servomotor(
                        LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                        LOWER_SERVO_MIN, LOWER_SERVO_MAX);
    box_upper_servo = new box::Servomotor(
                        UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                        UPPER_SERVO_MIN, UPPER_SERVO_MAX);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    box_switch = new box::Switch(PIN_SWITCH);
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    run_mode_reset_step = 0;
}

box::Main::~Main() {
}

void box::Main::wait_ms(int wait_ms) {
    last_time = millis();
    wait_delay = wait_ms;
}

void box::Main::run() {
    if(box_switch->is_high()) { box_mode = MODE_RESET; }
    int distance = box_sonar->get_average_distance_cm();
    if ((millis() - last_time) < wait_delay) {
        return;
    }
    switch (box_mode) {
        case MODE_RESET:
            run_mode_reset();
            break;
        case MODE_AWARENESS:
            run_mode_awareness(distance);
            break;
        case MODE_NORMAL:
            run_mode_normal();
            break;
        default:
            break;
    }
}

void box::Main::run_mode_reset() {
    switch (run_mode_reset_step) {
        case 0:
            box_lower_servo->move_to_percent(0);
            box_upper_servo->move_to_percent(100);
            box::Main::wait_ms(400);
            run_mode_reset_step = 1;
            return;
        case 1:
            box_upper_servo->move_to_percent(0);
            if(random(100) < 75) {
                box_mode = MODE_AWARENESS;
            } else {
                box_mode = MODE_NORMAL;
            }
            box::Main::wait_ms(400);
            run_mode_reset_step = 0;
            return;
        default:
            run_mode_reset_step = 0;
            break;
    }
}

void box::Main::run_mode_awareness(int distance) {
    if(distance > 30) {
        box_lower_servo->move_to_percent(0);
        box::Main::wait_ms(50);
        return;
    }
    if(distance > 20) {
        // random move 30-50%
        box_lower_servo->move_to_percent(random(20)+30);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    if(distance > 10) {
        // random move 50-70%
        box_lower_servo->move_to_percent(random(20)+50);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    box_lower_servo->move_to_percent(100);
    box::Main::wait_ms(250);
    return;
}

void box::Main::run_mode_normal() {

    box::Main::wait_ms(50);
}