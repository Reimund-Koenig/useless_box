#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "box_gearmotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::NiceMock;

#define TEST_PIN_PWM_SPEED 1
#define TEST_PIN_FORWARD 2
#define TEST_PIN_BACKWARD 3

struct Gearmotor_under_test : public box::Gearmotor {
    Gearmotor_under_test() : Gearmotor(TEST_PIN_PWM_SPEED,
                                     TEST_PIN_FORWARD,
                                     TEST_PIN_BACKWARD) {}
};

class TestController : public ::testing::Test {
  protected:
    Gearmotor_under_test* gearmotor_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        gearmotor_under_test = new Gearmotor_under_test();
    }
    virtual void TearDown() {
        delete gearmotor_under_test;
        delete arduino_mock;
    }
};

TEST_F(TestController, test_gearmotor_init) { EXPECT_TRUE(true); }

TEST_F(TestController, test_gearmotor_defaults) {
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DEFAULT_DIRECTION);
    EXPECT_EQ(gearmotor_under_test->get_speed(), GEARMOTOR_DEFAULT_SPEED);
}

TEST_F(TestController, test_gearmotor_speed) {
    EXPECT_CALL(*arduino_mock, analogWrite(TEST_PIN_PWM_SPEED,_)).Times(3);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_STOP, 142);
    EXPECT_EQ(gearmotor_under_test->get_speed(), 142);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_STOP, -1);
    EXPECT_EQ(gearmotor_under_test->get_speed(), 0);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_STOP, 256);
    EXPECT_EQ(gearmotor_under_test->get_speed(), 255);
}

TEST_F(TestController, test_gearmotor_direction_forward) {
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_FORWARD,HIGH)).Times(1);
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_BACKWARD,LOW)).Times(1);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_FORWARD, 200);
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DIRECTION_FORWARD);
}

TEST_F(TestController, test_gearmotor_direction_backward) {
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_FORWARD,LOW)).Times(1);
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_BACKWARD,HIGH)).Times(1);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_BACKWARD, 200);
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DIRECTION_BACKWARD);
}

TEST_F(TestController, test_gearmotor_direction_stop) {
    int possitiv_non_direction_value = 42;
    int negative_non_direction_value = -42;
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_FORWARD,LOW)).Times(3);
    EXPECT_CALL(*arduino_mock, digitalWrite(TEST_PIN_BACKWARD,LOW)).Times(3);
    gearmotor_under_test->move(GEARMOTOR_DIRECTION_STOP, 200);
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DIRECTION_STOP);
    gearmotor_under_test->move(possitiv_non_direction_value,200);
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DIRECTION_STOP);
    gearmotor_under_test->move(negative_non_direction_value,200);
    EXPECT_EQ(gearmotor_under_test->get_direction(), GEARMOTOR_DIRECTION_STOP);
}
