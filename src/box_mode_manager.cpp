#include "ardunio_namespace.h" // needed for arduino build
#include "box_mode_manager.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;

box::ModeManager::ModeManager(box::Servomanager* box_servomanager,
                                  box::Wait* box_wait) {
    box::ModeManager::box_mode_awareness = new box::ModeAwareness(box_servomanager, box_wait);
    box::ModeManager::box_mode_reset = new box::ModeReset(box_servomanager, box_wait);
    box::ModeManager::box_mode_normal = new box::ModeNormal(box_servomanager, box_wait);
    box::ModeManager::box_mode_startup = new box::ModeStartup(box_servomanager, box_wait);
}

box::ModeManager::~ModeManager() {
}

void box::ModeManager::run_mode_awareness(int distance){
    box::ModeManager::box_mode_awareness->run(distance);
}

void box::ModeManager::run_mode_normal(){
    box::ModeManager::box_mode_normal->run();
}

bool box::ModeManager::run_mode_reset(){
    return box::ModeManager::box_mode_reset->run();
}

void box::ModeManager::run_mode_startup(){
    box::ModeManager::box_mode_startup->run();
}
