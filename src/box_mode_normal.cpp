#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_normal.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeNormal::ModeNormal(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeNormal::box_servomanager = box_servomanager;
    box::ModeNormal::box_wait = box_wait;
    box::ModeNormal::run_mode_normal_step = 0;
}

box::ModeNormal::~ModeNormal() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::ModeNormal::run() {
    switch (run_mode_normal_step) {
        case 0:
            box_servomanager->move_pilot_servo_to_percent(0,6);
            box_servomanager->move_copilot_servo_to_percent(0,6);
            box_wait->milliseconds(50);
            return;
        default:
            run_mode_normal_step = 0;
            return;
    }
}
