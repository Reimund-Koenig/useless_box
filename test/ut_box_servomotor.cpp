#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_servomotor.hpp"
#include "box_servomotor.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

#define TEST_PIN_PWM_SERVOMOTOR 1
#define TEST_MIN_ANGLE 10
#define TEST_MAX_ANGLE 20

struct ServomotorUnderTest : public box::Servomotor {
    ServomotorUnderTest() : Servomotor(TEST_PIN_PWM_SERVOMOTOR, TEST_MIN_ANGLE, TEST_MAX_ANGLE) {}
};

class TestServo : public ::testing::Test {
  protected:
    ServomotorUnderTest* servomotorUnderTest;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        servomotorUnderTest = new ServomotorUnderTest();
    }
    virtual void TearDown() {
        delete servomotorUnderTest;
        delete servomotor_mock;
    }
};

TEST_F(TestServo, test_servomotor_init) { EXPECT_TRUE(true); }

TEST_F(TestServo, test_servomotor_defaults) {
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
    EXPECT_EQ(servomotorUnderTest->get_angle(), TEST_MIN_ANGLE);
}

TEST_F(TestServo, test_servomotor_get_current_angle) {
    int expected_angle = 13;
    EXPECT_CALL(*servomotor_mock, read()).Times(1).WillOnce(Return(expected_angle));
    EXPECT_EQ(servomotorUnderTest->get_current_angle(), expected_angle);
}

TEST_F(TestServo, test_servomotor_move_percentage) {
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 17;
    EXPECT_CALL(*servomotor_mock, write(expected_angle_50_percent)).Times(1);
    servomotorUnderTest->move_to_percent(50);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle_50_percent);
    EXPECT_CALL(*servomotor_mock, write(expected_angle_75_percent)).Times(1);
    servomotorUnderTest->move_to_percent(75);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle_75_percent);
}

TEST_F(TestServo, test_servomotor_direction_forward_with_angle_max_peak) {
    EXPECT_CALL(*servomotor_mock, write(_)).Times(TEST_MAX_ANGLE-TEST_MIN_ANGLE+1);
    int expected_angle = servomotorUnderTest->get_angle() + 1;
    for(int i = TEST_MIN_ANGLE; i <= TEST_MAX_ANGLE; i++) {
        servomotorUnderTest->move_direction(SERVOMOTOR_DIRECTION_FORWARD);
        EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_FORWARD);
        EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
        if(expected_angle < TEST_MAX_ANGLE) { expected_angle++; }
    }
}

TEST_F(TestServo, test_servomotor_direction_backward_with_angle_min_peak) {
    EXPECT_CALL(*servomotor_mock, write(11)).Times(1);
    EXPECT_CALL(*servomotor_mock, write(10)).Times(2);
    servomotorUnderTest->move_direction(SERVOMOTOR_DIRECTION_FORWARD);
    int expected_angle = servomotorUnderTest->get_angle() - 1;
    servomotorUnderTest->move_direction(SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
    servomotorUnderTest->move_direction(SERVOMOTOR_DIRECTION_BACKWARD);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_angle);
}

TEST_F(TestServo, test_servomotor_direction_stop) {
    int possitiv_non_direction_value = 42;
    int negative_non_direction_value = -42;
    EXPECT_CALL(*servomotor_mock, write(_)).Times(0);
    servomotorUnderTest->move_direction(SERVOMOTOR_DIRECTION_STOP);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
    servomotorUnderTest->move_direction(possitiv_non_direction_value);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
    servomotorUnderTest->move_direction(negative_non_direction_value);
    EXPECT_EQ(servomotorUnderTest->get_direction(), SERVOMOTOR_DIRECTION_STOP);
}


TEST_F(TestServo, test_servomotor_move_ange) {
    int expected_value = (int) ((TEST_MIN_ANGLE + TEST_MAX_ANGLE)/2);
    EXPECT_CALL(*servomotor_mock, write(expected_value)).Times(1);
    servomotorUnderTest->move_angle(expected_value);
    EXPECT_EQ(servomotorUnderTest->get_angle(),expected_value);
}
