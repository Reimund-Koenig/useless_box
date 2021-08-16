
#include "Servo.h"
#include "manager/box_servo_manager.hpp"
#include "peripheral/box_servomotor.hpp"
#include "ardunio_namespace.h" // needed for arduino build
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Servomanager::Servomanager(box::Servomotor* box_lower_servo,
                                box::Servomotor* box_upper_servo,
                                box::Wait* box_wait_till_servomanager_finished_moving,
                                box::Switch* box_switch) {
    box::Servomanager::box_lower_servo = box_lower_servo;
    box::Servomanager::box_upper_servo = box_upper_servo;
    box::Servomanager::box_wait_till_servomanager_finished_moving = box_wait_till_servomanager_finished_moving;
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
    box_wait_till_servomanager_finished_moving->milliseconds(sleep_time_for_speed);
}

void box::Servomanager::move_copilot_servo_to_percent(int percentage, int speed) {
    int sleep_time_for_speed = 0;
    if(box_switch->is_high()) {
        sleep_time_for_speed = box_lower_servo->move_to_percent(percentage, speed);
    } else {
        sleep_time_for_speed = box_upper_servo->move_to_percent(percentage, speed);
    }
    box_wait_till_servomanager_finished_moving->milliseconds(sleep_time_for_speed);
}

void box::Servomanager::move_steps() {
    box_upper_servo->move_step();
    box_lower_servo->move_step();
}

bool box::Servomanager::box_servos_not_reached_switch() {
    return (box_upper_servo->current_angle_smaller_than_95_percent() &&
            box_lower_servo->current_angle_smaller_than_95_percent());
}
