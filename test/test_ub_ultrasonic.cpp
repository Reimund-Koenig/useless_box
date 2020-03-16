#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "ub_ultrasonic.hpp"
#include "mocks/mock_arduino.hpp"

class TestUltrasonic : public ::testing::Test {
};

TEST_F(TestUltrasonic, test_switch_init) { EXPECT_TRUE(true); }
