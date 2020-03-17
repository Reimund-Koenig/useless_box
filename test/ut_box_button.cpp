#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "mocks/mock_arduino.hpp"
#include "box_button.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_BUTTON 1
#define TEST_DEBOUNCE_DELAY 10

struct ButtonUnderTest : public box::Button {
    ButtonUnderTest() : Button(TEST_PIN_BUTTON, TEST_DEBOUNCE_DELAY) {}
};

class TestButton : public ::testing::Test {
  protected:
    ButtonUnderTest* buttonUnderTest;
    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        EXPECT_CALL(*arduinoMock, pinMode(TEST_PIN_BUTTON, INPUT)).Times(1);
        buttonUnderTest = new ButtonUnderTest();
    }
    virtual void TearDown() {
        delete buttonUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestButton, test_button_init) { EXPECT_TRUE(true); }

TEST_F(TestButton, test_button_state_change_to_high_no_debounce_block) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(HIGH));
    EXPECT_CALL(*arduinoMock, millis()).Times(2)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1));
    EXPECT_TRUE(buttonUnderTest->pressed());
}

TEST_F(TestButton, test_button_pressed_two_times_in_a_row) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(2).WillRepeatedly(Return(HIGH));
    EXPECT_CALL(*arduinoMock, millis()).Times(3)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 2));
    EXPECT_TRUE(buttonUnderTest->pressed());
    EXPECT_FALSE(buttonUnderTest->pressed());
}

TEST_F(TestButton, test_button_not_pressed_two_times_in_a_row) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(2).WillRepeatedly(Return(LOW));
    EXPECT_CALL(*arduinoMock, millis()).Times(2)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 2));
    EXPECT_FALSE(buttonUnderTest->pressed());
    EXPECT_FALSE(buttonUnderTest->pressed());
}

TEST_F(TestButton, test_button_state_no_change_to_high_because_debounce_blocks) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(HIGH));
    EXPECT_CALL(*arduinoMock, millis()).Times(2)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY));
    EXPECT_FALSE(buttonUnderTest->pressed());
}

TEST_F(TestButton, test_button_state_keep_low_after_debounce_mechanism) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(LOW));
    EXPECT_CALL(*arduinoMock, millis()).Times(1)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1));
    EXPECT_FALSE(buttonUnderTest->pressed());
}

TEST_F(TestButton, test_button_state_keep_low_without_debounce_trigger) {
    EXPECT_CALL(*arduinoMock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(LOW));
    EXPECT_CALL(*arduinoMock, millis()).Times(1)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY));
    EXPECT_FALSE(buttonUnderTest->pressed());
}
