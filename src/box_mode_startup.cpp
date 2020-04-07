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
    int speed = 6;
    switch (box_mode_state) {
        case 0: // Move switch up if it is down
            box_mode_state++;
            counter++;
            if(box_switch->is_high()) { return false; }
            box_wait->milliseconds(box_servomanager->move_pilot_servo_to_percent(100, speed));
            return false; // not finished
        case 1: // Move pilot to 100 which changes pilot and copilot
            speed = random(4) + 3; // Speed = 3-6
            box_wait->milliseconds(box_servomanager->move_pilot_servo_to_percent(100, speed));
            box_servomanager->move_copilot_servo_to_percent(0, 6);
            counter++;
            if(counter == 10) {  box_mode_state++; }
            return false; // not finished
        case 2: // Everything done -> reset and exit
            box_servomanager->move_pilot_servo_to_percent(0, speed);
            box_servomanager->move_copilot_servo_to_percent(0, speed);
            box_wait->milliseconds(400);
            box_mode_state = 0;
            counter = 0;
            return true; // finished
        default:
            counter = 0;
            box_mode_state = 0;
            return false; // not finished
    }
    return false; // not finished
}
