#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_startup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeStartup::ModeStartup(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait, box::Switch* box_switch) {
    box::ModeStartup::box_servomanager = box_servomanager;
    box::ModeStartup::box_wait = box_wait;
    box::ModeStartup::box_switch = box_switch;
    box::ModeStartup::box_mode_state = 0;
    box::ModeStartup::counter = 0;
}

box::ModeStartup::~ModeStartup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeStartup::run() {
    switch (box_mode_state) {
        case 0: // Move switch up if it is down
            box_mode_state = 1;
            if(box_switch->is_high()) { return false; }
            box_servomanager->move_servos_to_percent(100,0);
            box_wait->milliseconds(400);
            return false;
        case 1: // Move switch down (should be the endstate)
            box_mode_state = 2;
            box_servomanager->move_servos_to_percent(0,100);
            box_wait->milliseconds(400);
            counter++;
            if(counter == 5) {  box_mode_state = 3; }
            return false;
        case 2: // Move switch up and jump back to one
            box_mode_state = 1;
            box_servomanager->move_servos_to_percent(100,0);
            box_wait->milliseconds(400);
            return false;
        case 3: // Everything done -> reset and exit
            box_mode_state = 0;
            box_servomanager->move_servos_to_percent(0,0);
            box_wait->milliseconds(400);
            return true;
        default:
            box_mode_state = 0;
            return false;
    }
}
