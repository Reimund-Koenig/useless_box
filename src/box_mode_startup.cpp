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
}

box::ModeStartup::~ModeStartup() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeStartup::run() {
    if (box_mode_state == 0) { // Move switch up if it is down
        box_mode_state++;
        if(!box_switch->is_high()) { return false; }
        box_servomanager->move_pilot_servo_to_percent(100, 6);
        return false; // not finished
    } if (box_mode_state == 1) { // Reset both motors
        box_servomanager->move_pilot_servo_to_percent(0, 6);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_mode_state++;
        return false;
    } if (box_mode_state == 2) { // Move slowly both motors to 90%
        box_servomanager->move_pilot_servo_to_percent(90, 1);
        box_servomanager->move_copilot_servo_to_percent(90, 1);
        box_mode_state++;
        return false; // not finished
    } if (box_mode_state == 3) {// Move first one fast back
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_wait->add_milliseconds(400);
        box_mode_state++;
        return false; // finished
    } if (box_mode_state == 4) {// Move second one fast back
        box_servomanager->move_pilot_servo_to_percent(0, 6);
        box_wait->add_milliseconds(400);
        box_mode_state++;
        return false;
    } if (box_mode_state == 5) { // Move upper out
        box_servomanager->move_copilot_servo_to_percent(80, 6);
        box_mode_state++;
        return false;
    } if (box_mode_state == 6) { // Jitter upper motor (with the eye)
        int delay_time = 250;
        int speed = 2;
        while(delay_time > 0) {
            box_servomanager->move_copilot_servo_to_percent(90, speed);
            delay(delay_time);
            box_servomanager->move_copilot_servo_to_percent(70, speed);
            delay(delay_time);
            delay_time -= 10;
        }
        box_mode_state++;
        return false;
    } if (box_mode_state == 7) { // Press button with lower
        box_servomanager->move_pilot_servo_to_percent(100, 6);
        box_mode_state++;
        return false;
    } if (box_mode_state == 8) { // lower slowly back and press button with upper
        box_servomanager->move_copilot_servo_to_percent(0, 2);
        delay(100);
        box_servomanager->move_pilot_servo_to_percent(100, 4);
        box_mode_state++;
        return false;
    } if (box_mode_state == 9) { // Reset both motors
        box_servomanager->move_pilot_servo_to_percent(0, 6);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_mode_state = 0;
        return true; // finished
    }
    box_mode_state = 0;
    return false; // not finished
}
