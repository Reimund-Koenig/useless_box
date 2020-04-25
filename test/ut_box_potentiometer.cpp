#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "box_potentiometer.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_POTENTIOMETER 1
#define TEST_MIN_PEAK_POTENTIOMETER 10
#define TEST_MAX_PEAK_POTENTIOMETER 20

struct Potentiometer_under_test : public box::Potentiometer {
    Potentiometer_under_test() : Potentiometer(TEST_PIN_POTENTIOMETER,
                                             TEST_MIN_PEAK_POTENTIOMETER,
                                             TEST_MAX_PEAK_POTENTIOMETER) {}
};

class TestPotentiometer : public ::testing::Test {
  protected:
    Potentiometer_under_test* potentiometer_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        potentiometer_under_test = new Potentiometer_under_test();
    }
    virtual void TearDown() {
        delete potentiometer_under_test;
        delete arduino_mock;
    }
};

TEST_F(TestPotentiometer, test_potentiometer_init) { EXPECT_TRUE(true); }

TEST_F(TestPotentiometer, test_potentiometer_get_value) {
    EXPECT_CALL(*arduino_mock, analogRead(TEST_PIN_POTENTIOMETER)).Times(1).WillOnce(Return(15));
    EXPECT_CALL(*arduino_mock, map(15,_,_,TEST_MIN_PEAK_POTENTIOMETER, TEST_MAX_PEAK_POTENTIOMETER))
                             .Times(1).WillOnce(Return(15));
    EXPECT_EQ(potentiometer_under_test->get_value(), 15);
}
