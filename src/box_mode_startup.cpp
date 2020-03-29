#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_startup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeStartup::ModeStartup(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeStartup::box_servomanager = box_servomanager;
    box::ModeStartup::box_wait = box_wait;
}

box::ModeStartup::~ModeStartup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::ModeStartup::run() {
    box_wait->milliseconds(50);
}
