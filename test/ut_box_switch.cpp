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

struct SwitchUnderTest : public box::Switch {
    SwitchUnderTest() : Switch(TEST_PIN_SWITCH) {}
};

class TestSwitch : public ::testing::Test {
  protected:
    SwitchUnderTest* switchUnderTest;
    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        EXPECT_CALL(*arduinoMock, pinMode(TEST_PIN_SWITCH, INPUT_PULLUP)).Times(1);
        switchUnderTest = new SwitchUnderTest();
    }
    virtual void TearDown() {
        delete switchUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestSwitch, test_switch_init) { EXPECT_TRUE(true); }

TEST_F(TestSwitch, test_switch_state_high) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(HIGH));
    EXPECT_EQ(switchUnderTest->get_state(), HIGH);
}

TEST_F(TestSwitch, test_switch_state_low) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_SWITCH)).Times(1).WillOnce(Return(LOW));
    EXPECT_EQ(switchUnderTest->get_state(), LOW);
}