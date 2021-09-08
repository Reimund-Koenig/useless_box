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

void box::SubModeFunctionJitter::init(int box_percentage){
    jitter_count = random(8) + 3;
    jitter_speed = random(4) + 3;
    int jitter_range = random(10) + 5;
    jitter_percentage_start = box_percentage;
    if(box_percentage+jitter_range >= 95) {
        jitter_percentage_end = box_percentage-jitter_range;
    } else {
        jitter_percentage_end = box_percentage+jitter_range;
    }
}

bool box::SubModeFunctionJitter::run() {
    box_servo_manager->move_copilot_servo_to_percent(0, 6);
    if(box_mode_jitter_state == 0) {
        box_servo_manager->move_pilot_servo_to_percent(jitter_percentage_end, jitter_speed);
        box_mode_jitter_state++;
        return false;
    } if(box_mode_jitter_state == 1) {
        box_servo_manager->move_pilot_servo_to_percent(jitter_percentage_start, jitter_speed);
        if(--jitter_count == 0) {
            box_mode_jitter_state = 2;
        } else {
            box_mode_jitter_state--;
        }
        return false;
    } if(box_mode_jitter_state == 2) {
        box_mode_jitter_state = 0;
        return true;
    }
    box_mode_jitter_state = 0;
    return false;
}
