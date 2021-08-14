#include "ardunio_namespace.h" // needed for arduino build
#include "modes/box_mode_startup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeStartup::ModeStartup(box::Servomanager* box_servo_manager) {
    box::ModeStartup::box_servo_manager = box_servo_manager;
}

box::ModeStartup::~ModeStartup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeStartup::run() {
    box_servo_manager->move_pilot_servo_to_percent(0, 6);
    box_servo_manager->move_copilot_servo_to_percent(0, 6);
    return true;
}
