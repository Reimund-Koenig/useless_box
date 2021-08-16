#include "ardunio_namespace.h" // needed for arduino build
#include "manager/box_wait_manager.hpp"
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
    unsigned long current = millis();
    bool new_time_bigger_then_last_time = current + milliseconds > last_time + wait_delay;
    if(is_expired() || new_time_bigger_then_last_time) {
        set_milliseconds(current, milliseconds);
    }
}

bool box::Wait::is_expired() {
    if ((millis() - last_time) < wait_delay) {
        return false;
    }
    return true;
}


/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Wait::set_milliseconds(unsigned long current, unsigned long milliseconds) {
    last_time = current;
    wait_delay =  milliseconds;
}
