#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_manager.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeManager::ModeManager(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait, box::Switch* box_switch) {
    box::ModeManager::box_mode_awareness = new box::ModeAwareness(box_servomanager, box_wait);
    box::ModeManager::box_mode_reset = new box::ModeReset(box_servomanager, box_wait);
    box::ModeManager::box_mode_normal = new box::ModeNormal(box_servomanager, box_wait);
    box::ModeManager::box_mode_startup = new box::ModeStartup(box_servomanager, box_wait, box_switch);
}

box::ModeManager::~ModeManager() {
}

bool box::ModeManager::run_mode_awareness(int distance, int* box_speed){
    box::ModeManager::box_mode_awareness->run(distance, box_speed);
    return false; // endless mode
}

bool box::ModeManager::run_mode_normal(int* box_speed){
    box::ModeManager::box_mode_normal->run(box_speed);
    return false; // endless mode
}

bool box::ModeManager::run_mode_reset(int* box_speed){
    return box::ModeManager::box_mode_reset->run(box_speed);
}

bool box::ModeManager::run_mode_startup(int* box_speed){
    return box::ModeManager::box_mode_startup->run(box_speed);
}
