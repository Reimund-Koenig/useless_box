#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "ub_motor.hpp"

class TestMain : public ::testing::Test {
};

TEST_F(TestMain, test_main_addition) {
    ub::Motor ub_motor;
    EXPECT_EQ(ub_motor.lastResult(), -1);
    ub_motor.addition(30,12);
    EXPECT_EQ(ub_motor.lastResult(), 42);
}
