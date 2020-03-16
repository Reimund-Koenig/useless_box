#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_servo.hpp"
#include "box_servomotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::NiceMock;

#define TEST_PIN_PWM_SERVOMOTOR 1

struct ServomotorUnderTest : public box::Servomotor {
    ServomotorUnderTest() : Servomotor(TEST_PIN_PWM_SERVOMOTOR) {}
};

class TestServo : public ::testing::Test {
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

TEST_F(TestServo, test_servomotor_init) { EXPECT_TRUE(true); }

TEST_F(TestServo, test_servomotor_defaults) {
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
}

TEST_F(TestServo, test_servomotor_direction_forward) {
    EXPECT_CALL(*servoMock, write(_)).Times(1);
    int expected_angle = servomotorUnderTest->get_angle() + 1;
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_FORWARD);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_FORWARD);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
}

TEST_F(TestServo, test_servomotor_direction_backward) {
    EXPECT_CALL(*servoMock, write(1)).Times(1);
    EXPECT_CALL(*servoMock, write(0)).Times(2);
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_FORWARD);
    int expected_angle = servomotorUnderTest->get_angle() - 1;
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
}

TEST_F(TestServo, test_servomotor_direction_stop) {
    int possitiv_non_direction_value = 42;
    int negative_non_direction_value = -42;
    EXPECT_CALL(*servoMock, write(_)).Times(0);
    servomotorUnderTest->move(SERVOMOTOR_DIRECTION_STOP);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
    servomotorUnderTest->move(possitiv_non_direction_value);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
    servomotorUnderTest->move(negative_non_direction_value);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
}
