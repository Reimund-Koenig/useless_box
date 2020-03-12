#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "ub_motor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::NiceMock;

struct MotorUnderTest : public ub::Motor {
    MotorUnderTest() : Motor() {}
    MOCK_METHOD0(move_forward, void());
    MOCK_METHOD0(move_backward, void());
};

class TestMain : public ::testing::Test {
  protected:
    MotorUnderTest* motorUnderTest;
    void (*move_forward)();
    void (*move_backward)();
    virtual void SetUp() {
        motorUnderTest = new MotorUnderTest();
    }
    virtual void TearDown() {
        delete motorUnderTest;
    }
};

TEST_F(TestMain, init) { EXPECT_TRUE(true); }

TEST_F(TestMain, test_motor_speed) {
    int default_speed = 100;
    EXPECT_EQ(motorUnderTest->get_speed(), default_speed);
    motorUnderTest->set_speed(142);
    EXPECT_EQ(motorUnderTest->get_speed(), 142);
    motorUnderTest->set_speed(-1);
    EXPECT_EQ(motorUnderTest->get_speed(), 0);
    motorUnderTest->set_speed(256);
    EXPECT_EQ(motorUnderTest->get_speed(), 255);
}

TEST_F(TestMain, test_motor_direction) {
    int default_direction = MOTOR_DIRECTION_STOP;
    EXPECT_EQ(motorUnderTest->get_direction(), default_direction);
    motorUnderTest->set_direction(MOTOR_DIRECTION_FORWARD);
    EXPECT_EQ(motorUnderTest->get_direction(), MOTOR_DIRECTION_FORWARD);
    motorUnderTest->set_direction(MOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(motorUnderTest->get_direction(), MOTOR_DIRECTION_BACKWARD);
    motorUnderTest->set_direction(MOTOR_DIRECTION_STOP);
    EXPECT_EQ(motorUnderTest->get_direction(), MOTOR_DIRECTION_STOP);
    motorUnderTest->set_direction(42);
    EXPECT_EQ(motorUnderTest->get_direction(), MOTOR_DIRECTION_STOP);
    motorUnderTest->set_direction(-42);
    EXPECT_EQ(motorUnderTest->get_direction(), MOTOR_DIRECTION_STOP);
}

TEST_F(TestMain, test_motor_move_when_direction_is_stop) {
    EXPECT_CALL(*motorUnderTest, move_forward()).Times(0);
    EXPECT_CALL(*motorUnderTest, move_backward()).Times(0);
    motorUnderTest->set_direction(MOTOR_DIRECTION_STOP);
    motorUnderTest->move();
}

TEST_F(TestMain, test_motor_move_when_direction_is_forward) {
    EXPECT_CALL(*motorUnderTest, move_forward()).Times(1);
    EXPECT_CALL(*motorUnderTest, move_backward()).Times(0);
    motorUnderTest->set_direction(MOTOR_DIRECTION_FORWARD);
    motorUnderTest->move();
}

TEST_F(TestMain, test_motor_move_when_direction_is_backward) {
    EXPECT_CALL(*motorUnderTest, move_forward()).Times(0);
    EXPECT_CALL(*motorUnderTest, move_backward()).Times(1);
    motorUnderTest->set_direction(MOTOR_DIRECTION_BACKWARD);
    motorUnderTest->move();
}