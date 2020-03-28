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
                box::Servomanager* box_servomanager) {
    box::Main::box_switch = box_switch;
    box::Main::box_sonar = box_sonar;
    box::Main::box_servomanager = box_servomanager;
    randomSeed(analogRead(0));
    box_mode = MODE_RESET;
    run_mode_reset_step = 0;
    box_wait = new box::Wait();
    box_mode_awareness = new box::ModeAwareness(box_servomanager, box_wait);
    box_mode_reset = new box::ModeReset(box_servomanager, box_wait);
    box_mode_normal = new box::ModeNormal(box_servomanager, box_wait);
}

box::Main::~Main() {
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Main::run() {
    if(box_switch->has_changed() &&
            box_servomanager->is_no_box_action()) {
        box_mode = MODE_RESET;
        box_servomanager->random_select_if_vice_versa_mode_should_be_changed();
    }
    int distance = box_sonar->get_average_distance_cm();
    if (box_wait->is_free()) {
        return;
    }
    switch (box_mode) {
        case MODE_RESET:
            box_mode_reset->run();
            break;
        case MODE_AWARENESS:
            box_mode_awareness->run(distance);
            break;
        case MODE_NORMAL:
            box_mode_normal->run();
            break;
        default:
            break;
    }
}

void box::Main::run_mode_normal() {
}
