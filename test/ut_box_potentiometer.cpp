#include <cstdio>
#include <future>
#include "gtest/gtest.h"

#include "mocks/mock_arduino.hpp"
#include "box_potentiometer.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_POTENTIOMETER 1
#define TEST_MIN_PEAK_POTENTIOMETER 10
#define TEST_MAX_PEAK_POTENTIOMETER 20

struct PotentiometerUnderTest : public box::Potentiometer {
    PotentiometerUnderTest() : Potentiometer(TEST_PIN_POTENTIOMETER,
                                             TEST_MIN_PEAK_POTENTIOMETER,
                                             TEST_MAX_PEAK_POTENTIOMETER) {}
};

class TestPotentiometer : public ::testing::Test {
  protected:
    PotentiometerUnderTest* potentiometerUnderTest;
    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        potentiometerUnderTest = new PotentiometerUnderTest();
    }
    virtual void TearDown() {
        delete potentiometerUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestPotentiometer, test_potentiometer_init) { EXPECT_TRUE(true); }

TEST_F(TestPotentiometer, test_potentiometer_get_value) {
    EXPECT_CALL(*arduinoMock, analogRead(TEST_PIN_POTENTIOMETER)).Times(1).WillOnce(Return(15));
    EXPECT_CALL(*arduinoMock, map(15,_,_,TEST_MIN_PEAK_POTENTIOMETER, TEST_MAX_PEAK_POTENTIOMETER))
                             .Times(1).WillOnce(Return(15));
    EXPECT_EQ(potentiometerUnderTest->get_value(), 15);
}
