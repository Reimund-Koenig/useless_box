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
    if(distance >= 30) {
        box_servomanager->move_pilot_servo_to_percent(0, box_speed);
        box_wait->milliseconds(50);
        return false;
    }
    if(distance >= 20) {
        // random move 30-50%
        box_speed = random(3) + 1; // Speed = 1-3
        box_servomanager->move_pilot_servo_to_percent(random(20)+30,box_speed);
        box_wait->milliseconds(random(750)+250);
        return false;
    }
    if(distance >= 10) {
        // random move 50-70%
        box_speed = random(3) + 1; // Speed = 1-3
        box_servomanager->move_pilot_servo_to_percent(random(20)+50, box_speed);
        box_wait->milliseconds(random(750)+250);
        return false;
    }
    box_speed = random(6) + 1; // Speed = 1-6
    box_servomanager->move_pilot_servo_to_percent(100, box_speed);
    box_wait->milliseconds(250);
    return true;
}
