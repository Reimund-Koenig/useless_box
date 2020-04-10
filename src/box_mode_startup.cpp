#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_startup.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeStartup::ModeStartup(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait_controller,
                                  box::Switch* box_switch) {
    box::ModeStartup::box_servomanager = box_servomanager;
    box::ModeStartup::box_wait_controller = box_wait_controller;
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
        box_wait_controller->add_milliseconds(1000);
        box_mode_state++;
        return false; // not finished
    } if (box_mode_state == 3) {// Move first one fast back
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_wait_controller->add_milliseconds(750);
        box_mode_state++;
        return false; // finished
    } if (box_mode_state == 4) {// Move second one fast back
        box_servomanager->move_pilot_servo_to_percent(0, 6);
        box_wait_controller->add_milliseconds(750);
        box_mode_state++;
        return false;
    } if (box_mode_state == 5) { // Move upper out
        box_servomanager->move_copilot_servo_to_percent(90, 6);
        box_mode_state++;
        return false;
    } if (box_mode_state == 6) { // Jitter upper motor speed 2(with the eye)
        if(box_servomanager->run_jitter(1,70,85,2)) { box_mode_state++; }
        return false;
    } if (box_mode_state == 7) { // Jitter upper motor speed 3(with the eye)
        if(box_servomanager->run_jitter(1,70,85,3)) { box_mode_state++; }
        return false;
    } if (box_mode_state == 8) { // Jitter upper motor speed 4(with the eye)
        if(box_servomanager->run_jitter(2,70,85,4)) { box_mode_state++; }
        return false;
    } if (box_mode_state == 9) { // Jitter upper motor speed 5(with the eye)
        if(box_servomanager->run_jitter(3,70,85,5)) { box_mode_state++; }
        return false;
    } if (box_mode_state == 10) { // Jitter upper motor speed 6(with the eye)
        if(box_servomanager->run_jitter(4,70,85,6)) { box_mode_state++; }
        return false;
    } if (box_mode_state == 11) { // Press button with lower while upper moving slowly back
        box_servomanager->move_pilot_servo_to_percent(100, 6);
        box_wait_controller->add_milliseconds(1000);
        box_mode_state++;
        return false;
    } if (box_mode_state == 12) { // lower slowly back while upper moving slowly forward
        box_servomanager->move_pilot_servo_to_percent(70, 2);
        box_servomanager->move_copilot_servo_to_percent(0, 2);
        box_wait_controller->add_milliseconds(500);
        box_mode_state++;
        return false;
    } if (box_mode_state == 13) { // press button with upper
        box_servomanager->move_pilot_servo_to_percent(100, 4);
        box_mode_state++;
        return false;
    } if (box_mode_state == 14) { // upper back to 70%
        box_servomanager->move_copilot_servo_to_percent(70, 4);
        box_wait_controller->add_milliseconds(1000);
        box_mode_state++;
        return false;
    } if (box_mode_state == 15) { // Reset both motors
        box_servomanager->move_pilot_servo_to_percent(0, 6);
        box_servomanager->move_copilot_servo_to_percent(0, 6);
        box_mode_state = 0;
        return true; // finished
    }
    box_mode_state = 0;
    return false; // not finished
}
