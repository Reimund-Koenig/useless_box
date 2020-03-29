#include "ardunio_namespace.h" // needed for arduino build
#include "box_main.hpp"
#include "box_mode_awareness.hpp"
#include "box_mode_reset.hpp"
#include "box_mode_normal.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

#define MODE_RESET 0
#define MODE_AWARENESS 1
#define MODE_NORMAL 2

box::Main::Main(box::Switch* box_switch,
                box::Sonar* box_sonar,
                box::Servomanager* box_servomanager,
                box::Wait* box_wait,
                box::ModeReset* box_mode_reset,
                box::ModeNormal* box_mode_normal,
                box::ModeAwareness* box_mode_awareness) {
    box::Main::box_switch = box_switch;
    box::Main::box_sonar = box_sonar;
    box::Main::box_servomanager = box_servomanager;
    box::Main::box_wait = box_wait;
    box::Main::box_mode_reset = box_mode_reset;
    box::Main::box_mode_awareness = box_mode_awareness;
    box::Main::box_mode_normal = box_mode_normal;
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    is_reset_finished = false;
}

box::Main::~Main() {
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Main::run() {
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
        case MODE_RESET:        is_reset_finished = box_mode_reset->run();  return;
        case MODE_AWARENESS:    box_mode_awareness->run(distance);          return;
        case MODE_NORMAL:       box_mode_normal->run();                     return;
        default:                box_mode = MODE_RESET;                      return;
    }
}

void box::Main::select_new_box_mode() {
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
