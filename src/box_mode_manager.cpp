#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_manager.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeManager::ModeManager(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait_controller, box::Switch* box_switch) {

    box::ModeManager::box_mode_function_jitter = new box::ModeFunctionJitter(box_servomanager);
    box::ModeManager::box_mode_awareness = new box::ModeAwareness(box_servomanager, box_mode_function_jitter, box_wait_controller);
    box::ModeManager::box_mode_reset = new box::ModeReset(box_servomanager, box_wait_controller);
    box::ModeManager::box_mode_startup = new box::ModeStartup(box_servomanager);
}

box::ModeManager::~ModeManager() {
}

bool box::ModeManager::run_mode_awareness(int distance){
    box::ModeManager::box_mode_awareness->run(distance);
    return false; // endless mode
}

bool box::ModeManager::run_mode_reset(){
    return box::ModeManager::box_mode_reset->run();
}

bool box::ModeManager::run_mode_startup(){
    return box::ModeManager::box_mode_startup->run();
}
