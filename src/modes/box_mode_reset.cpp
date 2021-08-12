#include "ardunio_namespace.h" // needed for arduino build
#include "modes/box_mode_reset.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeReset::ModeReset(box::Servomanager* box_servo_manager,
                                  box::Wait* box_wait_controller) {
    box::ModeReset::box_servo_manager = box_servo_manager;
    box::ModeReset::box_wait_controller = box_wait_controller;
    run_mode_reset_step  = 0;
}

box::ModeReset::~ModeReset() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeReset::run() {
    int speed = 6;
    switch (run_mode_reset_step) {
        case 0:
            box_servo_manager->move_pilot_servo_to_percent(100, speed);
            box_servo_manager->move_copilot_servo_to_percent(0, speed);
            run_mode_reset_step++;
            return false; // not finished
        case 1:
            speed = random(4) + 3; // Speed = 3-6
            box_servo_manager->move_pilot_servo_to_percent(0, speed);
            box_servo_manager->move_copilot_servo_to_percent(0, speed);
            if(random(100) > 50) {
                run_mode_reset_step++;
                return false; // change direction
            }
            run_mode_reset_step = 0;
            return true; // finished
        case 2:
            speed = random(4) + 3; // Speed = 3-6
            box_servo_manager->move_pilot_servo_to_percent(100, speed);
            box_servo_manager->move_copilot_servo_to_percent(0, speed);
            run_mode_reset_step++;
            return false; // not finished
        case 3:
            speed = random(4) + 3; // Speed = 3-6
            box_servo_manager->move_pilot_servo_to_percent(0, speed);
            box_servo_manager->move_copilot_servo_to_percent(0, speed);
            run_mode_reset_step = 0;
            return true; //finished
        default:
            run_mode_reset_step = 0;
            return false; // not finished
    }
    return false; // not finished
}
