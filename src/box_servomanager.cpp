
#include "Servo.h"
#include "box_servomotor.hpp"
#include "ardunio_namespace.h" // needed for arduino build
#include "box_main.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Servomanager::Servomanager(box::Servomotor* box_lower_servo,
                                box::Servomotor* box_upper_servo) {
    box::Servomanager::box_lower_servo = box_lower_servo;
    box::Servomanager::box_upper_servo = box_upper_servo;
    move_vice_versa = false;
    should_change_to_vice_versa_mode = false;
}

box::Servomanager::~Servomanager() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomanager::move_lower_servo_to_percent(int percentage) {
    move_servos_to_percent(percentage, -1);
}

void box::Servomanager::move_upper_servo_to_percent(int percentage) {
    move_servos_to_percent(-1, percentage);
}

void box::Servomanager::move_servos_to_percent(int percentage_lower,
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

bool box::Servomanager::change_vise_versa_if_required_and_return_is_changed() {
    if(should_change_to_vice_versa_mode) {
        move_vice_versa = !move_vice_versa;
        should_change_to_vice_versa_mode = false;
        return true;
    }
    return false;
}

void box::Servomanager::random_select_if_vice_versa_mode_should_be_changed() {
    if(random(50) > 50) {
        should_change_to_vice_versa_mode = true;
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
