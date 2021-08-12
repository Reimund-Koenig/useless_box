#include "ardunio_namespace.h" // needed for arduino build
#include "modes/box_submode_function_jitter.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::SubModeFunctionJitter::SubModeFunctionJitter(box::Servomanager* box_servo_manager) {
    box::SubModeFunctionJitter::box_servo_manager = box_servo_manager;
    box_mode_jitter_state = 0;
    jitter_speed = 0;
    jitter_count = 0;
}

box::SubModeFunctionJitter::~SubModeFunctionJitter() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::SubModeFunctionJitter::run(bool pilot, int count, int percent_1, int percent_2, int speed) {
    if(box_mode_jitter_state == 0) {
        jitter_count = count;
        box_mode_jitter_state++;
        return false;
    } if(box_mode_jitter_state == 1) {
        if(pilot) {
            box_servo_manager->move_pilot_servo_to_percent(percent_1, speed);
        } else  {
            box_servo_manager->move_copilot_servo_to_percent(percent_1, speed);
        }
        box_mode_jitter_state++;
        return false;
    } if(box_mode_jitter_state == 2) {
        if(pilot) {
            box_servo_manager->move_pilot_servo_to_percent(percent_2, speed);
        } else  {
            box_servo_manager->move_copilot_servo_to_percent(percent_2, speed);
        }
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
