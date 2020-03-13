#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "ub_gearmotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::NiceMock;

#define TEST_PIN_PWM_SPEED 1
#define TEST_PIN_FORWARD 2
#define TEST_PIN_BACKWARD 3

struct GearmotorUnderTest : public ub::Gearmotor {
    GearmotorUnderTest() : Gearmotor(TEST_PIN_PWM_SPEED,
                                     TEST_PIN_FORWARD,
                                     TEST_PIN_BACKWARD) {}
};

class TestMain : public ::testing::Test {
  protected:
    GearmotorUnderTest* gearmotorUnderTest;
    virtual void SetUp() {
        arduinoMock = new NiceMock<ArduinoMock>;
        gearmotorUnderTest = new GearmotorUnderTest();
    }
    virtual void TearDown() {
        delete gearmotorUnderTest;
        delete arduinoMock;
    }
};

TEST_F(TestMain, test_gearmotor_init) { EXPECT_TRUE(true); }

TEST_F(TestMain, test_gearmotor_defaults) {
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DEFAULT_DIRECTION);
    EXPECT_EQ(gearmotorUnderTest->get_speed(), GEARMOTOR_DEFAULT_SPEED);
}

TEST_F(TestMain, test_gearmotor_speed) {
    EXPECT_CALL(*arduinoMock, analogWrite(TEST_PIN_PWM_SPEED,_)).Times(3);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_STOP, 142);
    EXPECT_EQ(gearmotorUnderTest->get_speed(), 142);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_STOP, -1);
    EXPECT_EQ(gearmotorUnderTest->get_speed(), 0);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_STOP, 256);
    EXPECT_EQ(gearmotorUnderTest->get_speed(), 255);
}

TEST_F(TestMain, test_gearmotor_direction_forward) {
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_FORWARD,HIGH)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_BACKWARD,LOW)).Times(1);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_FORWARD, 200);
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DIRECTION_FORWARD);
}

TEST_F(TestMain, test_gearmotor_direction_backward) {
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_FORWARD,LOW)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_BACKWARD,HIGH)).Times(1);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_BACKWARD, 200);
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DIRECTION_BACKWARD);
}

TEST_F(TestMain, test_gearmotor_direction_stop) {
    int possitiv_non_direction_value = 42;
    int negative_non_direction_value = -42;
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_FORWARD,LOW)).Times(3);
    EXPECT_CALL(*arduinoMock, digitalWrite(TEST_PIN_BACKWARD,LOW)).Times(3);
    gearmotorUnderTest->move(GEARMOTOR_DIRECTION_STOP, 200);
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DIRECTION_STOP);
    gearmotorUnderTest->move(possitiv_non_direction_value,200);
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DIRECTION_STOP);
    gearmotorUnderTest->move(negative_non_direction_value,200);
    EXPECT_EQ(gearmotorUnderTest->get_direction(), GEARMOTOR_DIRECTION_STOP);
}
