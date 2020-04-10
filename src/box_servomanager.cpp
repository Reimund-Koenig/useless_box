
#include "Servo.h"
#include "box_servomanager.hpp"
#include "box_servomotor.hpp"
#include "ardunio_namespace.h" // needed for arduino build
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Servomanager::Servomanager(box::Servomotor* box_lower_servo,
                                box::Servomotor* box_upper_servo,
                                box::Wait* box_wait_controller,
                                box::Switch* box_switch) {
    box::Servomanager::box_lower_servo = box_lower_servo;
    box::Servomanager::box_upper_servo = box_upper_servo;
    box::Servomanager::box_wait_controller = box_wait_controller;
    box::Servomanager::box_switch = box_switch;
}

box::Servomanager::~Servomanager() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomanager::move_pilot_servo_to_percent(int percentage, int speed) {
    int sleep_time_for_speed = 0;
    if(box_switch->is_high()) {
        sleep_time_for_speed = box_upper_servo->move_to_percent(percentage, speed);
    } else {
        sleep_time_for_speed = box_lower_servo->move_to_percent(percentage, speed);
    }
    box_wait_controller->milliseconds(sleep_time_for_speed);
}

void box::Servomanager::move_copilot_servo_to_percent(int percentage, int speed) {
    int sleep_time_for_speed = 0;
    if(box_switch->is_high()) {
        sleep_time_for_speed = box_lower_servo->move_to_percent(percentage, speed);
    } else {
        sleep_time_for_speed = box_upper_servo->move_to_percent(percentage, speed);
    }
    box_wait_controller->milliseconds(sleep_time_for_speed);
}

void box::Servomanager::move_steps() {
    box_upper_servo->move_step();
    box_lower_servo->move_step();
}

bool box::Servomanager::box_servos_not_reached_switch() {
    return (box_upper_servo->current_angle_smaller_than_95_percent() &&
            box_lower_servo->current_angle_smaller_than_95_percent());
}

bool box::Servomanager::run_jitter(int count, int percent_1, int percent_2, int speed) {
    if(box_mode_jitter_state == 0) {
        jitter_count = count;
        box_mode_jitter_state++;
        return false;
    } if(box_mode_jitter_state == 1) {
        move_copilot_servo_to_percent(percent_1, speed);
        box_mode_jitter_state++;
        return false;
    } if(box_mode_jitter_state == 2) {
        move_copilot_servo_to_percent(percent_2, speed);
        if(--jitter_count == 0) {
            box_mode_jitter_state = 3;
        } else {
            box_mode_jitter_state--;
        }
        return false;
    } if(box_mode_jitter_state == 3) {
        box_mode_jitter_state = 0;
        return true;
    }
    box_mode_jitter_state = 0;
    return false;
}