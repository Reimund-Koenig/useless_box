#include "ub_main.hpp"

#include "Arduino.h"

ub::Main::Main() {
    // setup
    // pinMode(GSM1, OUTPUT);
    // pinMode(GSM2, OUTPUT);
    // pinMode(in1, OUTPUT);
    // pinMode(in2, OUTPUT);
    // pinMode(in3, OUTPUT);
    // pinMode(in4, OUTPUT);
}

ub::Main::~Main() {
}

void ub::Main::run() {
}

void ub::Main::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Main::lastResult() {
    return m_lastResult;
}

