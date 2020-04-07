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
                            box::Servomotor* box_upper_servo,
                            box::Switch* box_switch)
                    : Servomanager(box_lower_servo, box_upper_servo, box_switch) {}
};


class TestServomanager : public ::testing::Test {
      protected:
    Servomanager_under_test* servomanager_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        box_lower_servo_mock = new NiceMock<BoxServoMock>;
        box_upper_servo_mock = new NiceMock<BoxServoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        servomanager_under_test = new Servomanager_under_test(
                                            (box::Servomotor*) box_lower_servo_mock,
                                            (box::Servomotor*) box_upper_servo_mock,
                                            (box::Switch*) box_switch_mock);
    }
    virtual void TearDown() {
        delete servomanager_under_test;
        delete box_switch_mock;
        delete box_upper_servo_mock;
        delete box_lower_servo_mock;
        delete arduino_mock;
        delete servomotor_mock;
    }
};

TEST_F(TestServomanager, test_servomanager_init) { EXPECT_TRUE(true); }

TEST_F(TestServomanager, test_servomanager_test_move_steps) {
    EXPECT_CALL(*box_lower_servo_mock, move_step());
    EXPECT_CALL(*box_upper_servo_mock, move_step());
    servomanager_under_test->move_steps();
}

TEST_F(TestServomanager, test_servomanager_move_pilot_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(2));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_)).Times(0);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(_)).Times(0);
    servomanager_under_test->move_pilot_servo_to_percent(50,2);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(4));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_)).Times(0);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(_)).Times(0);
    servomanager_under_test->move_pilot_servo_to_percent(50,4);
}

TEST_F(TestServomanager, test_servomanager_move_copilot_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(5));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_)).Times(0);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(_)).Times(0);
    servomanager_under_test->move_copilot_servo_to_percent(50,5);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(6));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_)).Times(0);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(_)).Times(0);
    servomanager_under_test->move_copilot_servo_to_percent(50,6);
}

TEST_F(TestServomanager, test_servomanager_move_both_servo_percentage) {
    int expected_result_pilot = 42;
    int expected_result_copilot = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(4));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(5));
    servomanager_under_test->move_pilot_servo_to_percent(42, 4);
    servomanager_under_test->move_copilot_servo_to_percent(50, 5);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_pilot)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(3));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_copilot)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(2));
    servomanager_under_test->move_pilot_servo_to_percent(42, 3);
    servomanager_under_test->move_copilot_servo_to_percent(50, 2);
    // // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(1));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(2));
    servomanager_under_test->move_pilot_servo_to_percent(42, 1);
    servomanager_under_test->move_copilot_servo_to_percent(50, 2);
    // // same way arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot)).Times(1);
    EXPECT_CALL(*box_lower_servo_mock, set_speed_and_get_sleeptime(5));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot)).Times(1);
    EXPECT_CALL(*box_upper_servo_mock, set_speed_and_get_sleeptime(6));
    servomanager_under_test->move_pilot_servo_to_percent(42, 5);
    servomanager_under_test->move_copilot_servo_to_percent(50, 6);
}

TEST_F(TestServomanager, test_servomanager_is_no_box_action) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_FALSE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_TRUE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_TRUE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(0));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_FALSE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_FALSE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(100));
    EXPECT_FALSE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(95));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(95));
    EXPECT_TRUE(servomanager_under_test->is_no_box_action());

    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(95));
    EXPECT_CALL(*box_lower_servo_mock, get_last_percentage()).Times(AtLeast(0)).WillOnce(Return(95));
    EXPECT_TRUE(servomanager_under_test->is_no_box_action());
}
