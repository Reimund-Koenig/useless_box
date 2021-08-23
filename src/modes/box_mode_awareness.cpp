#include "ardunio_namespace.h" // needed for arduino build
#include "modes/box_mode_awareness.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeAwareness::ModeAwareness(box::Servomanager* box_servo_manager,
                box::SubModeFunctionJitter* box_submode_function_jitter) {
    box::ModeAwareness::box_submode_function_jitter = box_submode_function_jitter;
    box::ModeAwareness::box_servo_manager = box_servo_manager;
    box::ModeAwareness::last_distance = 0;
    run_submode_jitter = false;
}

box::ModeAwareness::~ModeAwareness() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::ModeAwareness::run(int distance) {
    if(run_submode_jitter) {
        bool submode_finished = box_submode_function_jitter->run();
        if(submode_finished) {
            box_servo_manager->move_pilot_servo_to_percent(0, 6);
        }
        run_submode_jitter = !submode_finished;
        return false;
    } else {
        return run_awareness(distance);
    }
}

/*************************************************************************************************
 * Private Methods
 *************************************************/
void box::ModeAwareness::decide_for_jitter(int distance) {
    if(distance >= 30) { return; }
    if(distance < 15) { return; }
    run_submode_jitter = random(100) > 80;
    if(!run_submode_jitter) { return; }
    box_submode_function_jitter->init(box_percentage);
}

bool box::ModeAwareness::run_awareness(int distance) {
    box_servo_manager->move_copilot_servo_to_percent(0, 6);
    decide_for_jitter(distance);
    int diff = last_distance - distance;
    if(5 > diff && diff > -5) { return false; }
    box_speed = random(6) + 1;
    if (distance < 9)  { box_percentage = 100; box_speed = 6; } else
    if (distance < 12) { box_percentage = 90;  } else
    if (distance < 15) { box_percentage = 80;  } else
    if (distance < 18) { box_percentage = 70;  } else
    if (distance < 21) { box_percentage = 60;  } else
    if (distance < 24) { box_percentage = 50;  } else
    if (distance < 27) { box_percentage = 40;  } else
    if (distance < 30) { box_percentage = 30;  } else
                       { box_percentage = 0;   }
    last_distance = distance;
    box_servo_manager->move_pilot_servo_to_percent(box_percentage, box_speed);
    return false; // endless mode (ending by controller -> user interrupt)
}
