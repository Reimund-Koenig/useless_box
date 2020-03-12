#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "ub_switch.hpp"

class TestMain : public ::testing::Test {
};

TEST_F(TestMain, test_switch_addition) {
    ub::Switch ub_switch;
    EXPECT_EQ(ub_switch.lastResult(), -1);
    ub_switch.addition(30,12);
    EXPECT_EQ(ub_switch.lastResult(), 42);
}
