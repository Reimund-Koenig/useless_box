#include "ardunio_namespace.h" // needed for arduino build
#include "box_button.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::Button::Button(int pin_button,  unsigned long debounce_delay) {
    box::Button::pin_button = pin_button;
    box::Button::debounce_delay = debounce_delay;
    box::Button::last_state_pressed = false;
    box::Button::last_button_state = 0;
    box::Button::button_state = 0;
    box::Button::last_debounce_time = 0;
    pinMode(pin_button, INPUT);
}

box::Button::~Button() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::Button::pressed() {
    bool is_pressed = get_state();
    if(!is_pressed) {
        last_state_pressed = false;
        return false;
    }
    if(last_state_pressed) { return false; }
    last_state_pressed = true;
    return true;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

bool box::Button::get_state() {
    int reading = digitalRead(box::Button::pin_button);
    if (reading != last_button_state) {
        last_debounce_time = millis();
    }
    if ((millis() - last_debounce_time) > debounce_delay) {
        if (reading != button_state) {
            button_state = reading;
        }
    }
    last_button_state = reading;
    return (bool) button_state;
}