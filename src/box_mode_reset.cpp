#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_reset.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeReset::ModeReset(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeReset::box_servomanager = box_servomanager;
    box::ModeReset::box_wait = box_wait;
}

box::ModeReset::~ModeReset() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeReset::run() {
    switch (run_mode_reset_step) {
        case 0:
            box_servomanager->move_servos_to_percent(0,100);
            box_wait->milliseconds(400);
            run_mode_reset_step = 1;
            return false; // not finished
        case 1:
            box_servomanager->move_upper_servo_to_percent(0);
            run_mode_reset_step = 0;
            box_wait->milliseconds(400);
            return true; // finished
        default:
            run_mode_reset_step = 0;
            return true; // finished
    }
}
