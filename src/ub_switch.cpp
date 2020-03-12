#include "ub_switch.hpp"
#include "Arduino.h"

void ub::Switch::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Switch::lastResult() {
    return m_lastResult;
}

