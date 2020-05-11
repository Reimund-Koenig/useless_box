#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
#include <avr/sleep.h>
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;

#define MODE_STARTUP 0
#define MODE_RESET 1
#define MODE_AWARENESS 2
#define MODE_WAKEUP 3
#define DEEP_SLEEP_DELAY 60000
#define STANDBY_DELAY 300000

box::Controller::Controller(bool enable_sleep_mode,
                box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servomanager,
                box::Wait* box_wait_controller,
                box::Wait* box_wait_standby,
                box::ModeManager* box_mode_manager) {
    box::Controller::box_switch = box_switch;
    box::Controller::box_sonar = box_sonar;
    box::Controller::box_servomanager = box_servomanager;
    box::Controller::box_wait_controller = box_wait_controller;
    box::Controller::box_wait_standby = box_wait_standby;
    box::Controller::box_mode_manager = box_mode_manager;
    sleep_mode_enabled = enable_sleep_mode;
    if(sleep_mode_enabled) {
        time_till_sleep = DEEP_SLEEP_DELAY;
    } else {
        time_till_sleep = STANDBY_DELAY;
    }
    box_mode = MODE_STARTUP;
    is_mode_finished = false;
    box_wait_standby->milliseconds(time_till_sleep);
    distance = box_sonar->get_average_distance_cm();
}

box::Controller::~Controller() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/
// static void wake_up(){
    // ToDo!
    // detachInterrupt(INT0);
// }

void box::Controller::run() {
    if (box_wait_standby->is_free()) {
        if(sleep_mode_enabled) {
            attachInterrupt(INT0, nullptr, CHANGE);
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            // sleep_mode(); // call sleep_enable() then sleep_cpu() then sleep_disable()
            sleep_enable(); // set sleep enable bit
            sleep_cpu(); //  sleep without SE bit.
            sleep_disable();
        } else {
            box_mode = MODE_WAKEUP;
        }
        box_wait_standby->milliseconds(time_till_sleep);
    }
    distance = box_sonar->get_average_distance_cm();
    box_servomanager->move_steps();
    bool user_interrupt = box_switch->has_changed() && box_servomanager->box_servos_not_reached_switch();
    if(user_interrupt) {
        box_mode = MODE_RESET;
        is_mode_finished = false;
        box_wait_standby->milliseconds(time_till_sleep);
    }
    if (!box_wait_controller->is_free()) { return; }
    if (is_mode_finished) { switch_box_mode(); }
    switch (box_mode) {
    case MODE_RESET:        is_mode_finished = box_mode_manager->run_mode_reset(); return;
    case MODE_AWARENESS:    is_mode_finished = box_mode_manager->run_mode_awareness(distance); return;
    case MODE_WAKEUP:       is_mode_finished = box_mode_manager->run_mode_wakeup(distance); return;
    case MODE_STARTUP:      is_mode_finished = box_mode_manager->run_mode_startup(); return;
    default:                box_mode = MODE_RESET; return;
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Controller::switch_box_mode() {
    is_mode_finished = false;
    if(box_mode == MODE_WAKEUP) {
        box_mode = MODE_STARTUP;
        return;
    }
    if(box_mode == MODE_RESET || box_mode == MODE_STARTUP) {
        box_mode = MODE_AWARENESS;
        return;
    }
    box_mode = MODE_RESET;
}
