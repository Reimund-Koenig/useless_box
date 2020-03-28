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

void box::ModeAwareness::run(int distance) {
    if(distance > 30) {
        box_servomanager->move_lower_servo_to_percent(0);
        box_wait->milliseconds(50);
        return;
    }
    if(distance > 20) {
        // random move 30-50%
        box_servomanager->move_lower_servo_to_percent(random(20)+30);
        box_wait->milliseconds(random(750)+250);
        return;
    }
    if(distance > 10) {
        // random move 50-70%
        box_servomanager->move_lower_servo_to_percent(random(20)+50);
        box_wait->milliseconds(random(750)+250);
        return;
    }
    box_servomanager->move_lower_servo_to_percent(100);
    box_wait->milliseconds(250);
    return;
}
