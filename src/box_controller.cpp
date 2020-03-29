#include "ardunio_namespace.h" // needed for arduino build
#include "box_controller.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

#define MODE_STARTUP 0
#define MODE_RESET 1
#define MODE_AWARENESS 2
#define MODE_NORMAL 3

box::Controller::Controller(box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servomanager,
                box::Wait* box_wait,
                box::ModeManager* box_mode_manager) {
    box::Controller::box_switch = box_switch;
    box::Controller::box_sonar = box_sonar;
    box::Controller::box_servomanager = box_servomanager;
    box::Controller::box_wait = box_wait;
    box::Controller::box_mode_manager = box_mode_manager;
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    is_reset_finished = false;
}

box::Controller::~Controller() {
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Controller::run() {
    int distance = box_sonar->get_average_distance_cm();
    box_servomanager->move_steps(5);
    if(box_switch->has_changed()) {
        box_mode = MODE_RESET;
        if(box_servomanager->is_no_box_action()){
            box_servomanager->random_select_if_vice_versa_mode_should_be_changed();
        }
    }
    if (!box_wait->is_free()) { return; }
    if (is_reset_finished) { select_new_box_mode(); }
    switch (box_mode) {
        case MODE_RESET:        is_reset_finished = box_mode_manager->run_mode_reset();  return;
        case MODE_AWARENESS:    box_mode_manager->run_mode_awareness(distance);          return;
        case MODE_NORMAL:       box_mode_manager->run_mode_normal();                     return;
        case MODE_STARTUP:      box_mode_manager->run_mode_startup();                     return;
        default:                box_mode = MODE_RESET;                      return;
    }
}

void box::Controller::select_new_box_mode() {
    is_reset_finished = false;
    if(box_servomanager->change_vise_versa_if_required_and_return_is_changed()) {
        box_mode = MODE_RESET;
        return;
    }
    if(random(100) < 75) {
        box_mode = MODE_AWARENESS;
        return;
    }
    box_mode = MODE_NORMAL;
}