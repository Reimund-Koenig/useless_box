#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_servomotor.hpp"
#include "mocks/mock_box.hpp"
#include "mocks/mock_arduino.hpp"
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
    Servomotor_under_test(bool clockwise, box::Wait* box_wait) :
            Servomotor(TEST_PIN_PWM_SERVOMOTOR, clockwise,
                        TEST_MIN_ANGLE, TEST_MAX_ANGLE, box_wait) {}
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
        servo->move_to_percent(percentage);
        EXPECT_CALL(*box_wait_mock, is_free()).WillRepeatedly(Return(true));
        for(int i=0; i<11; i++) {
            servo->move_step();
        }
        EXPECT_EQ(expected_angle, servo->get_angle());
        EXPECT_EQ(expected_percentage, servo->get_last_percentage());
    }

    virtual void test_move_step(const int expected_angle, const int speed,
                                          const int expected_wait) {
        servomotor_under_test->set_speed(speed);
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
        EXPECT_CALL(*servomotor_mock, write(expected_angle)).Times(1);
        EXPECT_CALL(*box_wait_mock, milliseconds(expected_wait)).Times(1);
        servomotor_under_test->move_step();
        EXPECT_EQ(expected_angle, servomotor_under_test->get_angle());
    }

    virtual void test_move_step_clockwise(const int expected_angle, const int speed,
                                          const int expected_wait) {
        servomotor_under_test_clockwise->set_speed(speed);
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
        EXPECT_CALL(*servomotor_mock, write(expected_angle)).Times(1);
        EXPECT_CALL(*box_wait_mock, milliseconds(expected_wait)).Times(1);
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
    EXPECT_CALL(*box_wait_mock, is_free()).Times(0);
    EXPECT_CALL(*servomotor_mock, write(_)).Times(0);
    servomotor_under_test->move_step();
}

TEST_F(TestServo, test_servomotor_move_step_not_called_if_not_free) {
    servomotor_under_test->move_to_angle(15);
    EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(false));
    EXPECT_CALL(*servomotor_mock, write(_)).Times(0);
    servomotor_under_test->move_step();
}

TEST_F(TestServo, test_servomotor_move_to_angle) {
    const int expected_value = 13;
    servomotor_under_test->move_to_angle(13);
    int start_angle = TEST_MIN_ANGLE;
    EXPECT_EQ(servomotor_under_test->get_angle(), start_angle); // 10
    test_move_step(++start_angle, 4, 25); // 11
    test_move_step(++start_angle, 5, 10); // 12
    test_move_step(++start_angle, 6, 0); // 13
    servomotor_under_test->move_step();
    EXPECT_EQ(expected_value, servomotor_under_test->get_angle());

}

TEST_F(TestServo, test_servomotor_move_to_angle_clockwise) {
    const int expected_value_clockwise = 17;
    servomotor_under_test_clockwise->move_to_angle(13); // = 17 Clockwise
    int start_angle_clockwise = TEST_MAX_ANGLE;
    EXPECT_EQ(servomotor_under_test_clockwise->get_angle(), start_angle_clockwise); // 20
    test_move_step_clockwise(--start_angle_clockwise, 1, 200); // 19
    test_move_step_clockwise(--start_angle_clockwise, 2, 100); // 18
    test_move_step_clockwise(--start_angle_clockwise, 3, 50); // 17
    servomotor_under_test_clockwise->move_step();
    EXPECT_EQ(expected_value_clockwise, servomotor_under_test_clockwise->get_angle());
}

TEST_F(TestServo, test_servomotor_clockwise_move_percentage) {
    int expected_angle_0_percent = 10;
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 17;
    int expected_angle_100_percent = 20;
    test_percentage(servomotor_under_test, 0, expected_angle_0_percent, 0);
    test_percentage(servomotor_under_test, 50, expected_angle_50_percent, 50);
    test_percentage(servomotor_under_test, 75,expected_angle_75_percent, 75);
    test_percentage(servomotor_under_test, 100,expected_angle_100_percent, 100);
}

TEST_F(TestServo, test_servomotor_anti_clockwise_move_percentage) {
    int expected_angle_0_percent = 20;
    int expected_angle_50_percent = 15;
    int expected_angle_75_percent = 13;
    int expected_angle_100_percent = 10;
    test_percentage(servomotor_under_test_clockwise, 0,expected_angle_0_percent, 0);
    test_percentage(servomotor_under_test_clockwise, 50,expected_angle_50_percent, 50);
    test_percentage(servomotor_under_test_clockwise, 75,expected_angle_75_percent, 75);
    test_percentage(servomotor_under_test_clockwise, 100,expected_angle_100_percent, 100);
}

