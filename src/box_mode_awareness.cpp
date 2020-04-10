#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_awareness.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeAwareness::ModeAwareness(box::Servomanager* box_servomanager,
                box::ModeFunctionJitter* box_mode_function_jitter,
        box::Wait*          box_wait) {
    box::ModeAwareness::box_mode_function_jitter = box_mode_function_jitter;
    box::ModeAwareness::box_servomanager = box_servomanager;
    box::ModeAwareness::box_wait_controller = box_wait;
    box::ModeAwareness::last_distance = 0;
    random_jitter = false;
    jitter_count = 0;
    jitter_percentage_1 = 0;
    jitter_percentage_2 = 0;
    jitter_speed = 0;
}

box::ModeAwareness::~ModeAwareness() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeAwareness::run(int distance) {
    if(random_jitter) {
        random_jitter = !box_mode_function_jitter->run(
                                true,
                                jitter_count,
                                jitter_percentage_1,
                                jitter_percentage_2,
                                jitter_speed);
        return false;
    } else {
        return run_awareness(distance);
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::ModeAwareness::init_jitter(int box_percentage){
    random_jitter = random(100) > 80;
    if(!random_jitter) { return; }
    jitter_count = random(8) + 3;
    jitter_speed = random(4) + 3;
    jitter_percentage_2 = box_percentage;
    int jitter_range = random(10) + 5;
    if(box_percentage+jitter_range >= 95) {
        jitter_percentage_1 = box_percentage-jitter_range;
    } else {
        jitter_percentage_1 = box_percentage+jitter_range;
    }
}

bool box::ModeAwareness::run_awareness(int distance) {
    int diff = last_distance - distance;
    if(5 > diff && diff > -5) { return false; }
    bool return_val = false;
    int box_speed = 6;
    int box_percentage = 0;
    box_servomanager->move_copilot_servo_to_percent(0, 6);
    if(distance >= 30) {
        box_speed = 6;
        box_percentage = 0;
    } else if(distance >= 20) {
        box_speed = 6;
        box_percentage = random(20) + 30; // random move 30-50%
    } else if(distance >= 15) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 50; // random move 50-70%
        init_jitter(box_percentage);
    } else if(distance >= 10) {
        box_speed = random(6) + 1; // Speed = 1-6
        box_percentage = random(20) + 70; // random move 70-90%
        init_jitter(box_percentage);
    } else {
        box_speed = 6;
        box_percentage = 100;
        return_val = true;
    }
    last_distance = distance;
    box_servomanager->move_pilot_servo_to_percent(box_percentage, box_speed);
    return return_val;
}
