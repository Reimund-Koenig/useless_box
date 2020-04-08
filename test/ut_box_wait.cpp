#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "box_wait.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Wait_under_test : public box::Wait {
    Wait_under_test() : Wait() {}
};

class TestWait : public ::testing::Test {
  protected:
    Wait_under_test* box_wait_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_wait_under_test = new Wait_under_test();
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_wait_under_test;
    }
};

TEST_F(TestWait, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestWait, test_wait_milliseconds) {
    EXPECT_CALL(*arduino_mock, millis()).WillOnce(Return(5))
                                        .WillOnce(Return(10))
                                        .WillOnce(Return(11))
                                        .WillOnce(Return(11))
                                        .WillOnce(Return(59))
                                        .WillOnce(Return(60))
                                        .WillOnce(Return(60))
                                        .WillOnce(Return(65))
                                        .WillOnce(Return(65))
                                        .WillOnce(Return(75))
                                        .WillOnce(Return(100));
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 5
    box_wait_under_test->milliseconds(50);          // Check 11 -> Wait 10 till 60
    EXPECT_FALSE(box_wait_under_test->is_free());   // Check 11
    EXPECT_FALSE(box_wait_under_test->is_free());   // Check 59
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 60
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 60
    box_wait_under_test->milliseconds(35);          // Check 65 -> Wait 65 till 100
    EXPECT_FALSE(box_wait_under_test->is_free());   // Check 75
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 100
}

TEST_F(TestWait, test_wait_milliseconds_no_smaller_value_possible) {
        EXPECT_CALL(*arduino_mock, millis()).WillOnce(Return(5))
                                        .WillOnce(Return(10))
                                        .WillOnce(Return(11))
                                        .WillOnce(Return(12))
                                        .WillOnce(Return(12))
                                        .WillOnce(Return(59))
                                        .WillOnce(Return(60));
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 5
    box_wait_under_test->milliseconds(50);          // Check 11 -> Wait 10 till 60
    box_wait_under_test->milliseconds(47);          // Check 12 -> NO wait 12 till 59
    EXPECT_FALSE(box_wait_under_test->is_free());   // Check 59
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 60
}

TEST_F(TestWait, test_wait_milliseconds_bigger_value_possible) {
        EXPECT_CALL(*arduino_mock, millis()).WillOnce(Return(5))
                                        .WillOnce(Return(10))
                                        .WillOnce(Return(11))
                                        .WillOnce(Return(12))
                                        .WillOnce(Return(12))
                                        .WillOnce(Return(61))
                                        .WillOnce(Return(62));
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 5
    box_wait_under_test->milliseconds(50);          // Check 11 -> Wait 10 till 60
    box_wait_under_test->milliseconds(50);          // Check 12 -> Wait 12 till 62
    EXPECT_FALSE(box_wait_under_test->is_free());   // Check 61
    EXPECT_TRUE(box_wait_under_test->is_free());    // Check 62
}