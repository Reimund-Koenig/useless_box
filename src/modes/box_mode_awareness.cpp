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
        bool submode_finished = box_submode_function_jitter->run(true);
        run_submode_jitter = !submode_finished;
        return false;
    } else {
        return run_awareness(distance);
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/
void box::ModeAwareness::decide_for_jitter(int distance) {
    if(distance >= 30) { return; }
    if(distance < 15) { return; }
    run_submode_jitter = random(100) > 66;
    if(!run_submode_jitter) { return; }
    box_submode_function_jitter->init(box_percentage);
}

bool box::ModeAwareness::run_awareness(int distance) {
    box_servo_manager->move_copilot_servo_to_percent(0, 6);
    decide_for_jitter(distance);
    int diff = last_distance - distance;
    if(3 > diff && diff > -3) { return false; }
    int box_speed = 6;
    if(distance >= 30) {
        box_speed = 6;
        box_percentage = 0;
    } else if(distance >= 20) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 30; // random move 30-50%
    } else if(distance >= 15) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 50; // random move 50-70%
    } else if(distance >= 10) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 70; // random move 70-90%
    } else {
        box_speed = 6;
        box_percentage = 100;
    }
    last_distance = distance;
    box_servo_manager->move_pilot_servo_to_percent(box_percentage, box_speed);
    return false; // endless mode (ending by controller -> user interrupt)
}
