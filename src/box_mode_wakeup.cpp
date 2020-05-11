#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_wakeup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeWakeup::ModeWakeup() {
}

box::ModeWakeup::~ModeWakeup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeWakeup::run(int distance) {
    if(distance < 80) {
        return true;
    }
    return false;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/