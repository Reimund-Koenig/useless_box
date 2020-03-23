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
#define TEST_CLOCKWISE true
#define TEST_ANTI_CLOCKWISE false

struct Servomotor_under_test : public box::Servomotor {
    Servomotor_under_test(bool clockwise) : Servomotor(TEST_PIN_PWM_SERVOMOTOR, clockwise,
                                                       TEST_MIN_ANGLE, TEST_MAX_ANGLE) {}
};

class TestServo : public ::testing::Test {
  protected:
    Servomotor_under_test* servomotor_under_test_clockwise;
    Servomotor_under_test* servomotor_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        servomotor_under_test_clockwise = new Servomotor_under_test(TEST_CLOCKWISE);
        servomotor_under_test = new Servomotor_under_test(TEST_ANTI_CLOCKWISE);
    }
    virtual void TearDown() {
        delete servomotor_under_test_clockwise;
        delete servomotor_under_test;
        delete servomotor_mock;
    }
};

TEST_F(TestServo, test_servomotor_init) { EXPECT_TRUE(true); }

TEST_F(TestServo, test_servomotor_defaults) {
    EXPECT_EQ(servomotor_under_test->get_angle(), TEST_MIN_ANGLE);
    EXPECT_EQ(servomotor_under_test_clockwise->get_angle(), TEST_MAX_ANGLE);
}

TEST_F(TestServo, test_servomotor_get_current_angle) {
    int expected_angle = 13;
    EXPECT_CALL(*servomotor_mock, read()).Times(2).WillRepeatedly(Return(expected_angle));
    EXPECT_EQ(expected_angle, servomotor_under_test->get_current_angle());
    EXPECT_EQ(expected_angle, servomotor_under_test_clockwise->get_current_angle());
}

TEST_F(TestServo, test_servomotor_move_to_angle) {
    int expected_value = 13;
    int expected_value_clockwise = 17;
    EXPECT_CALL(*servomotor_mock, write(expected_value)).Times(1);
    EXPECT_CALL(*servomotor_mock, write(expected_value_clockwise)).Times(1);
    servomotor_under_test->move_to_angle(expected_value);
    servomotor_under_test_clockwise->move_to_angle(expected_value);
    EXPECT_EQ(expected_value, servomotor_under_test->get_angle());
    EXPECT_EQ(expected_value_clockwise, servomotor_under_test_clockwise->get_angle());
}

TEST_F(TestServo, test_servomotor_move_percentage) {
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 17;
    EXPECT_CALL(*servomotor_mock, write(expected_angle_50_percent)).Times(1);
    servomotor_under_test->move_to_percent(50);
    EXPECT_EQ(expected_angle_50_percent, servomotor_under_test->get_angle());
    EXPECT_CALL(*servomotor_mock, write(expected_angle_75_percent)).Times(1);
    servomotor_under_test->move_to_percent(75);
    EXPECT_EQ(expected_angle_75_percent, servomotor_under_test->get_angle());
}

TEST_F(TestServo, test_servomotor_move_percentage_clockwise) {
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 13;
    EXPECT_CALL(*servomotor_mock, write(expected_angle_50_percent)).Times(1);
    servomotor_under_test_clockwise->move_to_percent(50);
    EXPECT_EQ(expected_angle_50_percent, servomotor_under_test_clockwise->get_angle());
    EXPECT_CALL(*servomotor_mock, write(expected_angle_75_percent)).Times(1);
    servomotor_under_test_clockwise->move_to_percent(75);
    EXPECT_EQ(expected_angle_75_percent, servomotor_under_test_clockwise->get_angle());
}

