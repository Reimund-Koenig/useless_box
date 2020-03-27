#include "ardunio_namespace.h" // needed for arduino build
#include "box_main.hpp"
#include "Servo.h"
#include "box_servomotor.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;



#define MODE_RESET 0
#define MODE_AWARENESS 1
#define MODE_NORMAL 2

box::Main::Main(box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomotor* box_lower_servo,
                box::Servomotor* box_upper_servo) {
    box::Main::box_switch = box_switch;
    box::Main::box_sonar = box_sonar;
    box::Main::box_lower_servo = box_lower_servo;
    box::Main::box_upper_servo = box_upper_servo;
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    run_mode_reset_step = 0;
    wait_delay = 0;
    last_time = 0;
    move_vice_versa = false;
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

void box::Main::move_servos(int percentage_lower,
                            int percentage_upper) {
    if(move_vice_versa) {
        if(percentage_lower != -1) {
            box_upper_servo->move_to_percent(percentage_lower);
        }
        if(percentage_upper != -1) {
            box_lower_servo->move_to_percent(percentage_upper);
        }
    } else {
        if(percentage_lower != -1) {
            box_lower_servo->move_to_percent(percentage_lower);
        }
        if(percentage_upper != -1) {
            box_upper_servo->move_to_percent(percentage_upper);
        }
    }
}

void box::Main::run_mode_reset() {
    switch (run_mode_reset_step) {
        case 0:
            move_servos(0,100);
            box::Main::wait_ms(400);
            run_mode_reset_step = 1;
            return;
        case 1:
            move_servos(-1,0);
            if(random(100) < 75) {
                box_mode = MODE_AWARENESS;
            } else {
                box_mode = MODE_NORMAL;
            }
            box::Main::wait_ms(400);
            run_mode_reset_step = 2;
            return;
        case 2:
            // 50% of changing the behaviour to "vice versa"
        default:
            run_mode_reset_step = 0;
            break;
    }
}

void box::Main::run_mode_awareness(int distance) {
    if(distance > 30) {
        move_servos(0,-1);
        box::Main::wait_ms(50);
        return;
    }
    if(distance > 20) {
        // random move 30-50%
        move_servos(random(20)+30,-1);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    if(distance > 10) {
        // random move 50-70%
        move_servos(random(20)+50,-1);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    move_servos(100,-1);
    box::Main::wait_ms(250);
    return;
}

void box::Main::run_mode_normal() {

    box::Main::wait_ms(50);
}