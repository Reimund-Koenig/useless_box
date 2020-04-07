#include "ardunio_namespace.h" // needed for arduino build
#include "box_wait.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Wait::Wait() {
    wait_delay = 0;
    last_time = 0;
}

box::Wait::~Wait() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/
void box::Wait::add_milliseconds(unsigned long milliseconds) {
    wait_delay +=  milliseconds;
}

void box::Wait::milliseconds(unsigned long milliseconds) {
    last_time = millis();
    wait_delay =  milliseconds;
}

bool box::Wait::is_free() {
    if ((millis() - last_time) < wait_delay) {
        return false;
    }
    return true;
}
