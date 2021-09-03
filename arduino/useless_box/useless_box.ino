#include "ardunio_namespace.h"
#include "box_main.hpp"
#include "log.h"

box::Main* box_main;

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
    #endif
    box_main = new box::Main();
}

void loop() {
    box_main->box_controller->run();
    PRINT_LOGS();
}
