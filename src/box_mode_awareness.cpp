#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_awareness.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeAwareness::ModeAwareness(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeAwareness::box_servomanager = box_servomanager;
    box::ModeAwareness::box_wait = box_wait;
}

box::ModeAwareness::~ModeAwareness() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeAwareness::run(int distance) {
    int box_speed = 6;
    int box_percentage = 0;
    if(distance >= 30) {
        box_servomanager->move_pilot_servo_to_percent(box_percentage, box_speed);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        return false;
    }
    if(distance >= 20) {
        // random move 30-50%
        box_speed = random(4) + 3; // Speed = 3-6
        box_percentage = random(20)+30;
        box_servomanager->move_pilot_servo_to_percent(box_percentage, box_speed);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_wait->add_milliseconds(random(600));
        return false;
    }
    if(distance >= 10) {
        // random move 50-70%
        box_speed = random(4) + 3; // Speed = 3-6
        box_percentage = random(20)+50;
        box_servomanager->move_pilot_servo_to_percent(box_percentage, box_speed);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_wait->add_milliseconds(random(600));
        return false;
    }
    box_speed = random(3) + 4; // Speed = 3-6
    box_servomanager->move_pilot_servo_to_percent(100, box_speed);
    box_servomanager->move_copilot_servo_to_percent(0, 6);
    return true;
}
