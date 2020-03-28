#include "ardunio_namespace.h" // needed for arduino build
#include "box_main.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;



#define MODE_RESET 0
#define MODE_AWARENESS 1
#define MODE_NORMAL 2

box::Main::Main(box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servomanager) {
    box::Main::box_switch = box_switch;
    box::Main::box_sonar = box_sonar;
    box::Main::box_servomanager = box_servomanager;
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    run_mode_reset_step = 0;
    wait_delay = 0;
    last_time = 0;
}

box::Main::~Main() {
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Main::run() {
    if(box_switch->has_changed() &&
            box_servomanager->is_no_box_action()) {
        box_mode = MODE_RESET;
        box_servomanager->random_select_if_vice_versa_mode_should_be_changed();
    }
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

void box::Main::run_mode_normal() {
    box::Main::wait_ms(50);
}

void box::Main::run_mode_awareness(int distance) {
    if(distance > 30) {
        box_servomanager->move_lower_servo_to_percent(0);
        box::Main::wait_ms(50);
        return;
    }
    if(distance > 20) {
        // random move 30-50%
        box_servomanager->move_lower_servo_to_percent(random(20)+30);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    if(distance > 10) {
        // random move 50-70%
        box_servomanager->move_lower_servo_to_percent(random(20)+50);
        box::Main::wait_ms(random(750)+250);
        return;
    }
    box_servomanager->move_lower_servo_to_percent(100);
    box::Main::wait_ms(250);
    return;
}

void box::Main::run_mode_reset() {
    switch (run_mode_reset_step) {
        case 0:
            box_servomanager->move_servos_to_percent(0,100);
            box::Main::wait_ms(400);
            run_mode_reset_step = 1;
            return;
        case 1:
            box_servomanager->move_upper_servo_to_percent(0);
            run_mode_reset_step = 0;
            box::Main::wait_ms(400);
            if(!box_servomanager->change_vise_versa_if_required_and_return_is_changed()) {
                if(random(100) < 75) {
                    box_mode = MODE_AWARENESS;
                } else {
                    box_mode = MODE_NORMAL;
                }
            }
            return;
        default:
            run_mode_reset_step = 0;
            break;
    }
}

void box::Main::wait_ms(int wait_ms) {
    last_time = millis();
    wait_delay = wait_ms;
}

