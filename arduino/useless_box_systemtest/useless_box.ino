#include "box_systemtest.hpp"

box::Systemtest* box_systemtest;

void setup() {
    box_systemtest = new box::Systemtest();
}

void loop() {
    box_systemtest->run();
}