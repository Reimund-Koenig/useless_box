#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
#include <Arduino.h>
#include <stdio.h>
#include <avr/sleep.h>

using namespace arduino;

#define MODE_RESET 1
#define MODE_AWARENESS 2
#define TIME_TILL_DEEP_SLEEP 60000

box::Controller::Controller(bool is_engery_safe_mode,
                box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servo_manager,
                box::Wait* box_wait_till_servomanager_finished_moving,
                box::Wait* box_wait_deepsleep,
                box::ModeManager* box_mode_manager) {
    box::Controller::box_switch = box_switch;
    box::Controller::box_sonar = box_sonar;
    box::Controller::box_servo_manager = box_servo_manager;
    box::Controller::box_wait_till_servomanager_finished_moving = box_wait_till_servomanager_finished_moving;
    box::Controller::box_wait_deepsleep = box_wait_deepsleep;
    box::Controller::box_mode_manager = box_mode_manager;
    reset_servos_blocking();
    box_mode = MODE_AWARENESS;
    is_mode_finished = false;
    distance = box_sonar->get_median_distance_cm();
    box_wait_deepsleep->milliseconds(TIME_TILL_DEEP_SLEEP);
}

box::Controller::~Controller() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Controller::run() {
    if (box_wait_deepsleep->is_expired()) {
        reset_servos_blocking();
        deep_sleep_till_switch_is_toggled();
    }
    box_servo_manager->move_motors();
    distance = box_sonar->get_median_distance_cm();
    bool user_interrupt = box_switch->has_changed() && box_servo_manager->box_servos_not_reached_switch();
    if(user_interrupt) { switch_and_run_reset_mode(); }
    if(!box_wait_till_servomanager_finished_moving->is_expired()) { return; }
    if(is_mode_finished) { switch_box_mode(); }
    switch (box_mode) {
    case MODE_RESET:        is_mode_finished = box_mode_manager->run_mode_reset(); return;
    case MODE_AWARENESS:    is_mode_finished = box_mode_manager->run_mode_awareness(distance); return;
    default:                switch_box_mode(); return;
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Controller::reset_servos_blocking() {
        box_servo_manager->move_pilot_servo_to_percent(0, 6);
        box_servo_manager->move_copilot_servo_to_percent(0, 6);
        while(!box_wait_till_servomanager_finished_moving->is_expired()) {
            box_servo_manager->move_motors(); // blocking servo move
        }
}

void box::Controller::switch_and_run_reset_mode() {
    box_mode = MODE_RESET;
    is_mode_finished = box_mode_manager->run_mode_reset();
    box_wait_deepsleep->milliseconds(TIME_TILL_DEEP_SLEEP);
}

void box::Controller::switch_box_mode() {
    is_mode_finished = false;
    if(box_mode == MODE_RESET) {
        box_mode = MODE_AWARENESS;
        return;
    }
    box_mode = MODE_RESET;
}

/*****************************************************************************************
 * Box goes into deep_sleep power saving mode until User-Interrupt occurs
*****************************************/
void box::Controller::deep_sleep_till_switch_is_toggled() {
    attachInterrupt(INT0, nullptr, CHANGE);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable(); // set sleep enable bit
    sleep_cpu(); //  sleep without SE bit.
    sleep_disable();
    box_wait_deepsleep->milliseconds(TIME_TILL_DEEP_SLEEP);
}
