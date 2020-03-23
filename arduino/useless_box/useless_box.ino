#include "box_main.hpp"

box::Main* box_main;

void setup() {
    box_main = new box::Main();

}

void loop() {
    box_main->run();
}