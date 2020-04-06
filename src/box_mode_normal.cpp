#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_normal.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeNormal::ModeNormal(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeNormal::box_servomanager = box_servomanager;
    box::ModeNormal::box_wait = box_wait;
}

box::ModeNormal::~ModeNormal() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::ModeNormal::run() {
    box_wait->milliseconds(50);
}
