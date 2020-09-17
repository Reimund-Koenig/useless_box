#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_servomotor.hpp"
#include "mock_box.hpp"
#include "mock_arduino.hpp"
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
    Servomotor_under_test(bool clockwise, box::Wait* box_wait_controller) :
            Servomotor(TEST_PIN_PWM_SERVOMOTOR, clockwise,
                        TEST_MIN_ANGLE, TEST_MAX_ANGLE, box_wait_controller) {}
};

class TestServo : public ::testing::Test {
  protected:
    Servomotor_under_test* servomotor_under_test_clockwise;
    Servomotor_under_test* servomotor_under_test;

    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        box_wait_mock= new NiceMock<BoxWaitMock>;
        servomotor_under_test_clockwise = new Servomotor_under_test(TEST_CLOCKWISE,
                                            (box::Wait*) box_wait_mock);
        servomotor_under_test = new Servomotor_under_test(TEST_ANTI_CLOCKWISE,
                                            (box::Wait*) box_wait_mock);
    }

    virtual void TearDown() {
        delete servomotor_under_test;
        delete servomotor_under_test_clockwise;
        delete box_wait_mock;
        delete servomotor_mock;
    }

    virtual void test_percentage(Servomotor_under_test* servo, int percentage,
                                    int expected_angle, int expected_percentage) {
        servo->move_to_percent(percentage, 5);
        EXPECT_CALL(*box_wait_mock, is_expired()).WillRepeatedly(Return(true));
        for(int i=0; i<11; i++) {
            servo->move_step();
        }
        EXPECT_EQ(expected_angle, servo->get_angle());
        EXPECT_EQ(expected_percentage, servo->get_last_percentage());
    }

    virtual void test_move_step(const int expected_angle) {
        EXPECT_CALL(*box_wait_mock, is_expired()).WillOnce(Return(true));
        EXPECT_CALL(*servomotor_mock, write(expected_angle)).Times(1);
        EXPECT_CALL(*box_wait_mock, milliseconds(_)).Times(1);
        servomotor_under_test->move_step();
        EXPECT_EQ(expected_angle, servomotor_under_test->get_angle());
    }

    virtual void test_move_step_clockwise(const int expected_angle) {
        EXPECT_CALL(*box_wait_mock, is_expired()).WillOnce(Return(true));
        EXPECT_CALL(*servomotor_mock, write(expected_angle)).Times(1);
        EXPECT_CALL(*box_wait_mock, milliseconds(_)).Times(1);
        servomotor_under_test_clockwise->move_step();
        EXPECT_EQ(expected_angle, servomotor_under_test_clockwise->get_angle());
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

TEST_F(TestServo, test_servomotor_move_step_not_called_if_angle_reached) {
    EXPECT_CALL(*box_wait_mock, is_expired()).Times(0);
    EXPECT_CALL(*servomotor_mock, write(_)).Times(0);
    servomotor_under_test->move_step();
}

TEST_F(TestServo, test_servomotor_move_step_not_called_if_not_free) {
    EXPECT_TRUE(servomotor_under_test->move_to_angle(15,1) > 10);
    EXPECT_CALL(*box_wait_mock, is_expired()).WillOnce(Return(false));
    EXPECT_CALL(*servomotor_mock, write(_)).Times(0);
    servomotor_under_test->move_step();
}

TEST_F(TestServo, test_servomotor_move_to_angle) {
    const int expected_value = 13;
    servomotor_under_test->move_to_angle(13, 4);
    EXPECT_TRUE(servomotor_under_test->move_to_angle(13, 3) > 10); // = 13 Anti Clockwise
    int start_angle = TEST_MIN_ANGLE;
    EXPECT_EQ(servomotor_under_test->get_angle(), start_angle); // 10
    test_move_step(++start_angle); // 11
    test_move_step(++start_angle); // 12
    test_move_step(++start_angle); // 13
    servomotor_under_test->move_step();
    EXPECT_EQ(expected_value, servomotor_under_test->get_angle());
}

TEST_F(TestServo, test_servomotor_move_to_angle_clockwise) {
    const int expected_value_clockwise = 17;
    const int expected_steps = 3;
    EXPECT_TRUE(servomotor_under_test_clockwise->move_to_angle(13, 2) > 10); // = 17 Clockwise
    int start_angle_clockwise = TEST_MAX_ANGLE;
    EXPECT_EQ(servomotor_under_test_clockwise->get_angle(), start_angle_clockwise); // 20
    test_move_step_clockwise(--start_angle_clockwise); // 19
    test_move_step_clockwise(--start_angle_clockwise); // 18
    test_move_step_clockwise(--start_angle_clockwise); // 17
    servomotor_under_test_clockwise->move_step();
    EXPECT_EQ(expected_value_clockwise, servomotor_under_test_clockwise->get_angle());
}

TEST_F(TestServo, test_servomotor_clockwise_move_percentage) {
    int expected_angle_0_percent = 10;
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 17;
    int expected_angle_100_percent = 20;
    test_percentage(servomotor_under_test, 0, expected_angle_0_percent, 0);
    EXPECT_TRUE(servomotor_under_test->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test, 50, expected_angle_50_percent, 50);
    EXPECT_TRUE(servomotor_under_test->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test, 75,expected_angle_75_percent, 75);
    EXPECT_TRUE(servomotor_under_test->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test, 100,expected_angle_100_percent, 100);
    EXPECT_FALSE(servomotor_under_test->current_angle_smaller_than_95_percent());
}

TEST_F(TestServo, test_servomotor_anti_clockwise_move_percentage) {
    int expected_angle_0_percent = 20;
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 13;
    int expected_angle_100_percent = 10;
    test_percentage(servomotor_under_test_clockwise, 0,expected_angle_0_percent, 0);
    EXPECT_TRUE(servomotor_under_test_clockwise->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test_clockwise, 50,expected_angle_50_percent, 50);
    EXPECT_TRUE(servomotor_under_test_clockwise->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test_clockwise, 75,expected_angle_75_percent, 75);
    EXPECT_TRUE(servomotor_under_test_clockwise->current_angle_smaller_than_95_percent());
    test_percentage(servomotor_under_test_clockwise, 100,expected_angle_100_percent, 100);
    EXPECT_FALSE(servomotor_under_test_clockwise->current_angle_smaller_than_95_percent());
}

