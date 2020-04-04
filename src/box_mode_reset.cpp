#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_reset.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeReset::ModeReset(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait,
                                  box::Switch* box_switch) {
    box::ModeReset::box_servomanager = box_servomanager;
    box::ModeReset::box_wait = box_wait;
    box::ModeReset::box_switch = box_switch;
    box::ModeReset::run_mode_reset_step  = 0;
}

box::ModeReset::~ModeReset() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeReset::run() {
    switch (run_mode_reset_step) {
        case 0:  // Get start state of swith
            switch_start_state_is_high = box_switch->is_high();
            // if(box_servomanager->is_vice_versa_mode_activated()) {
            //     switch_start_state_is_high = !switch_start_state_is_high;
            // }
            run_mode_reset_step++;
            return false; // not finished
        case 1:
            if(switch_start_state_is_high) {
                box_servomanager->move_servos_to_percent(0,100);
            } else {
                box_servomanager->move_servos_to_percent(100,0);
            }
            box_wait->milliseconds(400);
            run_mode_reset_step++;
            return false; // not finished
        case 2:
            if(switch_start_state_is_high) {
                box_servomanager->move_copilot_servo_to_percent(0);
            } else {
                box_servomanager->move_pilot_servo_to_percent(0);
            }
            run_mode_reset_step = 0;
            box_wait->milliseconds(400);
            return true; // finished
        default:
            run_mode_reset_step = 0;
            return true; // finished
    }
}
