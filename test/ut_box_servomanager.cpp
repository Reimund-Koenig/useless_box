#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_serial.hpp"
#include "mocks/mock_servomotor.hpp"
#include "mocks/mock_box.hpp"

#include "box_servomanager.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Servomanager_under_test : public box::Servomanager {
    Servomanager_under_test(box::Servomotor* box_lower_servo,
                            box::Servomotor* box_upper_servo)
                    : Servomanager(box_lower_servo, box_upper_servo) {}
};


class TestServomanager : public ::testing::Test {
      protected:
    Servomanager_under_test* servomanager_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        box_lower_servo_mock = new NiceMock<BoxServoMock>;
        box_upper_servo_mock = new NiceMock<BoxServoMock>;
        servomanager_under_test = new Servomanager_under_test(
                                            (box::Servomotor*) box_lower_servo_mock,
                                            (box::Servomotor*) box_upper_servo_mock);
    }
    virtual void TearDown() {
        delete servomanager_under_test;
        delete box_upper_servo_mock;
        delete box_lower_servo_mock;
        delete servomotor_mock;
        delete arduino_mock;
    }
};

TEST_F(TestServomanager, test_servomanager_init) { EXPECT_TRUE(true); }

TEST_F(TestServomanager, test_servomanager_move_lower_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_)).Times(0);
    servomanager_under_test->move_lower_servo_to_percent(50);
     // otherway arround
    servomanager_under_test->change_vise_versa_mode();
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_)).Times(0);
    servomanager_under_test->move_lower_servo_to_percent(50);
}

TEST_F(TestServomanager, test_servomanager_move_upper_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_)).Times(0);
    servomanager_under_test->move_upper_servo_to_percent(50);
     // otherway arround
    servomanager_under_test->change_vise_versa_mode();
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_)).Times(0);
    servomanager_under_test->move_upper_servo_to_percent(50);
}

TEST_F(TestServomanager, test_servomanager_move_both_servo_percentage) {
    int expected_result_lower = 42;
    int expected_result_upper = 50;
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_lower)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_upper)).Times(1);
    servomanager_under_test->move_servos_to_percent(42, 50);
     // otherway arround
    servomanager_under_test->change_vise_versa_mode();
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_lower)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_upper)).Times(1);
    servomanager_under_test->move_servos_to_percent(42, 50);
    // otherway arround
    servomanager_under_test->change_vise_versa_mode();
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_lower)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_upper)).Times(1);
    servomanager_under_test->move_servos_to_percent(42, 50);
}

TEST_F(TestServomanager, test_servomanager_is_user_action) {
    int expected_result = 50;
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_FALSE(servomanager_under_test->is_user_action());
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_FALSE(servomanager_under_test->is_user_action());
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_FALSE(servomanager_under_test->is_user_action());
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(99));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(99));
    EXPECT_TRUE(servomanager_under_test->is_user_action());
}