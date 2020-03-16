#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "ub_switch.hpp"
#include "mocks/mock_arduino.hpp"

class TestSwitch : public ::testing::Test {
};

TEST_F(TestSwitch, test_switch_init) { EXPECT_TRUE(true); }
