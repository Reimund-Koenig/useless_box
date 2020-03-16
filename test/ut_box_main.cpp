#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "box_main.hpp"
#include "mocks/mock_arduino.hpp"

class TestMain : public ::testing::Test {
};

/**************************************************************************************************
 * If user gets close to the switch
 *    50% nothing happens
 *    50% useless box self switch
 *    0% useless box deke (antäuschen) self switch --> feature
 */
TEST_F(TestMain, test_main_init) { EXPECT_TRUE(true); }


