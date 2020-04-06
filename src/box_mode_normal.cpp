#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_normal.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeNormal::ModeNormal(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeNormal::box_servomanager = box_servomanager;
    box::ModeNormal::box_wait = box_wait;
    box::ModeNormal::run_mode_reset_step = 0;
}

box::ModeNormal::~ModeNormal() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::ModeNormal::run() {
    switch (run_mode_reset_step) {
        case 0:
            box_servomanager->move_servos_to_percent(0,3,0,3);
            box_wait->milliseconds(50);
            return;
        default:
            box_servomanager = 0;
            return;
    }
}
