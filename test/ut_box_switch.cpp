#include <cstdio>
#include <future>
#include "gtest/gtest.h"

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

TEST_F(TestSwitch, test_switch_state_high) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(HIGH));
    EXPECT_EQ(switch_under_test->get_state(), HIGH);
}

TEST_F(TestSwitch, test_switch_state_low) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(LOW));
    EXPECT_EQ(switch_under_test->get_state(), LOW);
}