#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
#include <LowPower.h> // Install Rocket Scream LowPower
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;

#define MODE_RESET 1
#define MODE_AWARENESS 2
#define DEEP_SLEEP_DELAY 15000

box::Controller::Controller(
                box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servomanager,
                box::Wait* box_wait_controller,
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
    box::Controller::box_servomanager = box_servomanager;
    box::Controller::box_wait_controller = box_wait_controller;
    box::Controller::box_wait_deepsleep = box_wait_deepsleep;
    box::Controller::box_mode_manager = box_mode_manager;
    box_mode = MODE_RESET;
    is_mode_finished = false;
    box_wait_deepsleep->milliseconds(DEEP_SLEEP_DELAY);
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
        attachInterrupt(INT0, nullptr, CHANGE);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        box_wait_deepsleep->milliseconds(DEEP_SLEEP_DELAY);
        digitalWrite(pin_power_servos, HIGH);
        digitalWrite(pin_power_sonar, HIGH);
    }
    distance = box_sonar->get_average_distance_cm();
    box_servomanager->move_steps();
    bool user_interrupt = box_switch->has_changed() && box_servomanager->box_servos_not_reached_switch();
    if(user_interrupt) {
        box_mode = MODE_RESET;
        is_mode_finished = false;
        box_wait_deepsleep->milliseconds(DEEP_SLEEP_DELAY);
    }
    if (!box_wait_controller->is_expired()) { return; }
    if (is_mode_finished) { switch_box_mode(); }
    switch (box_mode) {
    case MODE_RESET:        is_mode_finished = box_mode_manager->run_mode_reset(); return;
    case MODE_AWARENESS:    is_mode_finished = box_mode_manager->run_mode_awareness(distance); return;
    default:                box_mode = MODE_RESET; return;
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Controller::switch_box_mode() {
    is_mode_finished = false;
    if(box_mode == MODE_RESET) {
        box_mode = MODE_AWARENESS;
        return;
    }
    box_mode = MODE_RESET;
}
