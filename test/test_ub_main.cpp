#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "ub_main.hpp"
#include "mocks/mock_arduino.hpp"

class TestMain : public ::testing::Test {
};

TEST_F(TestMain, test_main_addition) {
    ub::Main ub_main;
    EXPECT_EQ(ub_main.lastResult(), -1);
    ub_main.addition(30,12);
    EXPECT_EQ(ub_main.lastResult(), 42);
}
