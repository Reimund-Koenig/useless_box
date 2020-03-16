#include "box_switch.hpp"
#include "Arduino.h"

void box::Switch::addition(int a, int b) {
    m_lastResult = a + b;
}

int box::Switch::lastResult() {
    return m_lastResult;
}

