#include "ardunio_namespace.h" // needed for arduino build
#include "modes/box_mode_awareness.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeAwareness::ModeAwareness(box::Servomanager* box_servo_manager,
                box::SubModeFunctionJitter* box_submode_function_jitter) {
    box::ModeAwareness::box_submode_function_jitter = box_submode_function_jitter;
    box::ModeAwareness::box_servo_manager = box_servo_manager;
    box::ModeAwareness::last_distance = 0;
    run_submode_jitter = false;
}

box::ModeAwareness::~ModeAwareness() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeAwareness::run(int distance) {
    if(run_submode_jitter) {
        run_submode_jitter = !box_submode_function_jitter->run(true);
        return false;
    } else {
        return run_awareness(distance);
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

bool box::ModeAwareness::run_awareness(int distance) {
    int diff = last_distance - distance;
    if(5 > diff && diff > -5) { return false; }
    bool return_val = false;
    int box_speed = 6;
    int box_percentage = 0;
    box_servo_manager->move_copilot_servo_to_percent(0, 6);
    if(distance >= 30) {
        box_speed = 6;
        box_percentage = 0;
    } else if(distance >= 20) {
        box_speed = 6;
        box_percentage = random(20) + 30; // random move 30-50%
    } else if(distance >= 15) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 50; // random move 50-70%
        run_submode_jitter = random(100) > 66;
        if(run_submode_jitter) {
            box_submode_function_jitter->init(box_percentage);
        }
    } else if(distance >= 10) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 70; // random move 70-90%
        run_submode_jitter = random(100) > 66;
        if(run_submode_jitter) {
            box_submode_function_jitter->init(box_percentage);
        }
    } else {
        box_speed = 6;
        box_percentage = 100;
        return_val = true;
    }
    last_distance = distance;
    box_servo_manager->move_pilot_servo_to_percent(box_percentage, box_speed);
    return return_val;
}
