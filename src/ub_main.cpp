#include "ub_main.hpp"

void ub::Main::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Main::lastResult() {
    return m_lastResult;
}

