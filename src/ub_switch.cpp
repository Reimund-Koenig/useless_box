#include "ub_switch.hpp"

void ub::Switch::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Switch::lastResult() {
    return m_lastResult;
}

