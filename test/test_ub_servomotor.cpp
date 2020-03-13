#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_servo.hpp"
#include "ub_servomotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::NiceMock;

#define TEST_PIN_PWM_SERVOMOTOR 1

struct ServomotorUnderTest : public ub::Servomotor {
    ServomotorUnderTest() : Servomotor(TEST_PIN_PWM_SERVOMOTOR) {}
};

class TestMain : public ::testing::Test {
  protected:
    ServomotorUnderTest* servomotorUnderTest;
    virtual void SetUp() {
        servoMock = new NiceMock<ServoMock>;
        servomotorUnderTest = new ServomotorUnderTest();
    }
    virtual void TearDown() {
        delete servomotorUnderTest;
        delete servoMock;
    }
};

TEST_F(TestMain, test_servomotor_direction_forward) {
    EXPECT_CALL(*servoMock, write(_)).Times(1);
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_FORWARD, 200);
}

// TEST_F(TestMain, test_servomotor_init) { EXPECT_TRUE(true); }

// TEST_F(TestMain, test_servomotor_defaults) {
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
//     EXPECT_EQ(servomotorUnderTest->get_speed(), SERVOMOTOR_DEFAULT_SPEED);
// }

// TEST_F(TestMain, test_servomotor_speed) {
//     EXPECT_CALL(*servoMock, write(_)).Times(0);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_STOP, 142);
//     EXPECT_EQ(servomotorUnderTest->get_speed(), 142);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_STOP, -1);
//     EXPECT_EQ(servomotorUnderTest->get_speed(), 0);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_STOP, 256);
//     EXPECT_EQ(servomotorUnderTest->get_speed(), 255);
// }

// TEST_F(TestMain, test_servomotor_direction_forward) {
//     EXPECT_CALL(*servoMock, write(_)).Times(1);
//     int expected_angle = servomotorUnderTest->get_angle() + 1;
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_FORWARD, 200);
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_FORWARD);
//     EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
// }

// TEST_F(TestMain, test_servomotor_direction_backward) {
//     EXPECT_CALL(*servoMock, write(_)).Times(1);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_FORWARD, 200);
//     int expected_angle = servomotorUnderTest->get_angle() - 1;
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_BACKWARD, 200);
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_BACKWARD);
//     EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_BACKWARD, 200);
//     EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
// }

// TEST_F(TestMain, test_servomotor_direction_stop) {
//     int possitiv_non_direction_value = 42;
//     int negative_non_direction_value = -42;
//     EXPECT_CALL(*servoMock, write(_)).Times(0);
//     servomotorUnderTest->move(SERVOMOTOR_DIRECTION_STOP, 200);
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
//     servomotorUnderTest->move(possitiv_non_direction_value,200);
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
//     servomotorUnderTest->move(negative_non_direction_value,200);
//     EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
// }
