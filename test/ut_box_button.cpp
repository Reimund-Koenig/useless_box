#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "box_button.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_BUTTON 1
#define TEST_DEBOUNCE_DELAY 10

struct Button_under_test : public box::Button {
    Button_under_test() : Button(TEST_PIN_BUTTON, TEST_DEBOUNCE_DELAY) {}
};

class TestButton : public ::testing::Test {
  protected:
    Button_under_test* button_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        EXPECT_CALL(*arduino_mock, pinMode(TEST_PIN_BUTTON, INPUT)).Times(1);
        button_under_test = new Button_under_test();
    }
    virtual void TearDown() {
        delete button_under_test;
        delete arduino_mock;
    }
};

TEST_F(TestButton, test_button_init) { EXPECT_TRUE(true); }

TEST_F(TestButton, test_button_state_change_to_high_no_debounce_block) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(HIGH));
    EXPECT_CALL(*arduino_mock, millis()).Times(2)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1));
    EXPECT_TRUE(button_under_test->pressed());
}

TEST_F(TestButton, test_button_pressed_two_times_in_a_row) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(2).WillRepeatedly(Return(HIGH));
    EXPECT_CALL(*arduino_mock, millis()).Times(3)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 2));
    EXPECT_TRUE(button_under_test->pressed());
    EXPECT_FALSE(button_under_test->pressed());
}

TEST_F(TestButton, test_button_not_pressed_two_times_in_a_row) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(2).WillRepeatedly(Return(LOW));
    EXPECT_CALL(*arduino_mock, millis()).Times(2)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 2));
    EXPECT_FALSE(button_under_test->pressed());
    EXPECT_FALSE(button_under_test->pressed());
}

TEST_F(TestButton, test_button_state_no_change_to_high_because_debounce_blocks) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(HIGH));
    EXPECT_CALL(*arduino_mock, millis()).Times(2)
                                       .WillOnce(Return(100))
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY));
    EXPECT_FALSE(button_under_test->pressed());
}

TEST_F(TestButton, test_button_state_keep_low_after_debounce_mechanism) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(LOW));
    EXPECT_CALL(*arduino_mock, millis()).Times(1)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY + 1));
    EXPECT_FALSE(button_under_test->pressed());
}

TEST_F(TestButton, test_button_state_keep_low_without_debounce_trigger) {
    EXPECT_CALL(*arduino_mock, digitalRead(TEST_PIN_BUTTON)).Times(1).WillOnce(Return(LOW));
    EXPECT_CALL(*arduino_mock, millis()).Times(1)
                                       .WillOnce(Return(100 + TEST_DEBOUNCE_DELAY));
    EXPECT_FALSE(button_under_test->pressed());
}
