#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "box_switch.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_SWITCH 1

struct Switch_under_test : public box::Switch {
    Switch_under_test() : Switch(TEST_PIN_SWITCH) {}
};

class TestSwitch : public ::testing::Test {
  protected:
    Switch_under_test* switch_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        EXPECT_CALL(*arduino_mock, pinMode(TEST_PIN_SWITCH, INPUT_PULLUP)).Times(1);
        switch_under_test = new Switch_under_test();
    }
    virtual void TearDown() {
        delete switch_under_test;
        delete arduino_mock;
    }
};

TEST_F(TestSwitch, test_switch_init) { EXPECT_TRUE(true); }

TEST_F(TestSwitch, test_switch_test_startup_high) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(3).WillRepeatedly(Return(HIGH));
    EXPECT_TRUE(switch_under_test->has_changed());
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_TRUE(switch_under_test->is_high());
}

TEST_F(TestSwitch, test_switch_test_startup_low) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(3).WillRepeatedly(Return(LOW));
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_FALSE(switch_under_test->is_high());
}

TEST_F(TestSwitch, test_switch_has_changed) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(LOW));
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(HIGH));
    EXPECT_TRUE(switch_under_test->has_changed());
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(HIGH));
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(LOW));
    EXPECT_TRUE(switch_under_test->has_changed());
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(LOW));
    EXPECT_FALSE(switch_under_test->has_changed());
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(2).WillOnce(Return(HIGH))
                                                                     .WillOnce(Return(LOW));
    switch_under_test->is_high();
    EXPECT_TRUE(switch_under_test->has_changed());
}