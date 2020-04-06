#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_reset.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeReset::ModeReset(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeReset::box_servomanager = box_servomanager;
    box::ModeReset::box_wait = box_wait;
    box::ModeReset::run_mode_reset_step  = 0;
}

box::ModeReset::~ModeReset() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeReset::run(int* speed) {
    switch (run_mode_reset_step) {
        case 0:
            do_move(0, 0, speed);
            run_mode_reset_step++;
            return false; // not finished
        case 1:
            do_move(100, 0, speed);
            run_mode_reset_step++;
            return false; // not finished
        case 2:
            do_move(0, 0, speed);
            if(random(100) > 50) {
                run_mode_reset_step++;
                return false; // change direction
            }
            run_mode_reset_step = 0;
            return true; // finished
        case 3:
            do_move(100, 0, speed);
            run_mode_reset_step++;
            return false; // not finished
        case 4:
            do_move(0,0, speed);
            run_mode_reset_step = 0;
            return true; //finished
        default:
            run_mode_reset_step = 0;
            return true; // finished
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::ModeReset::do_move(int percent_pilot, int percent_copilot, int* speed) {
    box_servomanager->move_servos_to_percent(percent_pilot, percent_copilot);
    *speed = random(4) + 3; // Speed = 3-6
    box_wait->milliseconds(400);
}