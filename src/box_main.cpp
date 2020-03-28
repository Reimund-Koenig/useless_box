#include "ardunio_namespace.h" // needed for arduino build
#include "box_main.hpp"
#include "box_mode_awareness.hpp"
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
            run_mode_reset();
            break;
        case MODE_AWARENESS:
            box_mode_awareness->run(distance);
            break;
        case MODE_NORMAL:
            run_mode_normal();
            break;
        default:
            break;
    }
}

void box::Main::run_mode_normal() {
    box_wait->milliseconds(50);
}

void box::Main::run_mode_reset() {
    switch (run_mode_reset_step) {
        case 0:
            box_servomanager->move_servos_to_percent(0,100);
            box_wait->milliseconds(400);
            run_mode_reset_step = 1;
            return;
        case 1:
            box_servomanager->move_upper_servo_to_percent(0);
            run_mode_reset_step = 0;
            box_wait->milliseconds(400);
            if(!box_servomanager->change_vise_versa_if_required_and_return_is_changed()) {
                if(random(100) < 75) {
                    box_mode = MODE_AWARENESS;
                } else {
                    box_mode = MODE_NORMAL;
                }
            }
            return;
        default:
            run_mode_reset_step = 0;
            break;
    }
}
