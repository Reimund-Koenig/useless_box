
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

void box::Servomanager::move_pilot_servo_to_percent(int percentage) {
    move_servos_to_percent(percentage, -1);
}

void box::Servomanager::move_copilot_servo_to_percent(int percentage) {
    move_servos_to_percent(-1, percentage);
}

void box::Servomanager::move_servos_to_percent(int percentage_pilot,
                            int percentage_copilot) {
    if(box_switch->is_high()) {
        if(percentage_pilot != -1) {
            box_upper_servo->move_to_percent(percentage_pilot);
        }
        if(percentage_copilot != -1) {
            box_lower_servo->move_to_percent(percentage_copilot);
        }
    } else {
        if(percentage_pilot != -1) {
            box_lower_servo->move_to_percent(percentage_pilot);
        }
        if(percentage_copilot != -1) {
            box_upper_servo->move_to_percent(percentage_copilot);
        }
    }
}

void box::Servomanager::move_steps(int speed) {
    // ToDo Implement Servomotor speed and
    //      call it here for both motors
    return;
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
