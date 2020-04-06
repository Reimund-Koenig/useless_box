
#include "Servo.h"
#include "box_servomotor.hpp"
#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Servomanager::Servomanager(box::Servomotor* box_lower_servo,
                                box::Servomotor* box_upper_servo,
                                box::Switch* box_switch) {
    box::Servomanager::box_lower_servo = box_lower_servo;
    box::Servomanager::box_upper_servo = box_upper_servo;
    box::Servomanager::box_switch = box_switch;
}

box::Servomanager::~Servomanager() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomanager::move_pilot_servo_to_percent(int percentage, int speed) {
    move_servos_to_percent(percentage, speed, -1, -1);
}

void box::Servomanager::move_copilot_servo_to_percent(int percentage, int speed) {
    move_servos_to_percent(-1, -1, percentage, speed);
}

void box::Servomanager::move_servos_to_percent(int percentage_pilot, int speed_pilot,
                            int percentage_copilot, int speed_copilot) {
    if(box_switch->is_high()) {
        if(percentage_pilot != -1) {
            box_upper_servo->move_to_percent(percentage_pilot);
            box_upper_servo->set_speed(speed_pilot);
        }
        if(percentage_copilot != -1) {
            box_lower_servo->move_to_percent(percentage_copilot);
            box_lower_servo->set_speed(speed_copilot);
        }
    } else {
        if(percentage_pilot != -1) {
            box_lower_servo->move_to_percent(percentage_pilot);
            box_lower_servo->set_speed(speed_pilot);
        }
        if(percentage_copilot != -1) {
            box_upper_servo->move_to_percent(percentage_copilot);
            box_upper_servo->set_speed(speed_copilot);
        }
    }
}

void box::Servomanager::move_steps() {
    box_upper_servo->move_step();
    box_lower_servo->move_step();
}

bool box::Servomanager::is_no_box_action() {
    if(box_upper_servo->get_last_percentage() >= 100) {
        return false;
    }
    if(box_lower_servo->get_last_percentage() >= 100) {
        return false;
    }
    return true;
}
