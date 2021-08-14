#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
// ToDo Check (<LowPower.h>)
// #include <LowPower.h> // Install Rocket Scream LowPower
// #include <avr/sleep.h>
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;

#define MODE_STARTUP 0
#define MODE_RESET 1
#define MODE_AWARENESS 2
#define ENERGY_SAFE_MODE 60000
#define FULL_POWER_MODE 300000

box::Controller::Controller(bool is_engery_safe_mode,
                box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servo_manager,
                box::Wait* box_wait_servo_speed_control,
                box::Wait* box_wait_deepsleep,
                box::ModeManager* box_mode_manager,
                int pin_power_servos,
                int pin_power_sonar) {
    box::Controller::pin_power_servos = pin_power_servos;
    box::Controller::pin_power_sonar = pin_power_sonar;
    pinMode(pin_power_servos, OUTPUT);
    pinMode(pin_power_sonar, OUTPUT);
    box::Controller::box_switch = box_switch;
    box::Controller::box_sonar = box_sonar;
    box::Controller::box_servo_manager = box_servo_manager;
    box::Controller::box_wait_servo_speed_control = box_wait_servo_speed_control;
    box::Controller::box_wait_deepsleep = box_wait_deepsleep;
    box::Controller::box_mode_manager = box_mode_manager;
    if(is_engery_safe_mode) {
        time_till_sleep = ENERGY_SAFE_MODE;
    } else {
        time_till_sleep = FULL_POWER_MODE;
    }
    box_mode = MODE_STARTUP;
    is_mode_finished = false;
    box_wait_deepsleep->milliseconds(time_till_sleep);
    distance = box_sonar->get_average_distance_cm();
    digitalWrite(pin_power_servos, HIGH);
    digitalWrite(pin_power_sonar, HIGH);
}

box::Controller::~Controller() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Controller::run() {
    if (box_wait_deepsleep->is_expired()) {
        deep_sleep_till_switch_is_toggled();
        box_wait_deepsleep->milliseconds(time_till_sleep);
    }
    distance = box_sonar->get_average_distance_cm();
    box_servo_manager->move_steps();
    bool user_interrupt = box_switch->has_changed() && box_servo_manager->box_servos_not_reached_switch();
    if(user_interrupt) { switch_to_reset_mode(); }
    if(!box_wait_servo_speed_control->is_expired()) { return; }
    if(is_mode_finished) { switch_box_mode(); }
    switch (box_mode) {
    case MODE_RESET:        is_mode_finished = box_mode_manager->run_mode_reset(); return;
    case MODE_AWARENESS:    is_mode_finished = box_mode_manager->run_mode_awareness(distance); return;
    case MODE_STARTUP:      is_mode_finished = box_mode_manager->run_mode_startup(); return;
    default:                switch_box_mode(); return;
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Controller::switch_to_reset_mode() {
    box_mode = MODE_RESET;
    is_mode_finished = false;
    box_wait_deepsleep->milliseconds(time_till_sleep);
}

void box::Controller::switch_box_mode() {
    is_mode_finished = false;
    if(box_mode == MODE_RESET || box_mode == MODE_STARTUP) {
        box_mode = MODE_AWARENESS;
        return;
    }
    box_mode = MODE_RESET;
}

/*****************************************************************************************
 * Box goes into deep_sleep power saving mode until User-Interrupt occurs
*****************************************/
void box::Controller::deep_sleep_till_switch_is_toggled() {
    // attachInterrupt(INT0, nullptr, CHANGE);

    // ToDo Check (<LowPower.h>)
    // LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    // box_wait_deepsleep->milliseconds(DEEP_SLEEP_DELAY);
    // digitalWrite(pin_power_servos, HIGH);
    // digitalWrite(pin_power_sonar, HIGH);

    // set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // // sleep_mode(); // call sleep_enable() then sleep_cpu() then sleep_disable()
    // sleep_enable(); // set sleep enable bit
    // sleep_cpu(); //  sleep without SE bit.
    // sleep_disable();
}
