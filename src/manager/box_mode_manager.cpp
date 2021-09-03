#include "ardunio_namespace.h" // needed for arduino build
#include "manager/box_mode_manager.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeManager::ModeManager(box::Servomanager* box_servo_manager) {
    box::ModeManager::box_submode_function_jitter = new box::SubModeFunctionJitter(box_servo_manager);
    box::ModeManager::box_mode_awareness = new box::ModeAwareness(box_servo_manager, box_submode_function_jitter);
    box::ModeManager::box_mode_reset = new box::ModeReset(box_servo_manager);
}

box::ModeManager::~ModeManager() {
}

bool box::ModeManager::run_mode_awareness(int distance){
    return box::ModeManager::box_mode_awareness->run(distance);
}

bool box::ModeManager::run_mode_reset(){
    return box::ModeManager::box_mode_reset->run();
}
