#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "box_main.hpp"
#include "mocks/mock_arduino.hpp"

class TestMain : public ::testing::Test {
};

TEST_F(TestMain, test_main_init) { EXPECT_TRUE(true); }
