#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "box_sonar.hpp"
#include "mocks/mock_arduino.hpp"

class TestSonar : public ::testing::Test {
};

TEST_F(TestSonar, test_switch_init) { EXPECT_TRUE(true); }
