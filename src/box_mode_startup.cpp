#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_startup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeStartup::ModeStartup(box::Servomanager* box_servomanager) {
    box::ModeStartup::box_servomanager = box_servomanager;
}

box::ModeStartup::~ModeStartup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeStartup::run() {
    box_servomanager->move_pilot_servo_to_percent(0, 6);
    box_servomanager->move_copilot_servo_to_percent(0, 6);
    return false;
}
