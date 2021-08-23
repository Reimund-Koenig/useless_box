#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_serial.hpp"
#include "mock_servomotor.hpp"
#include "mock_box.hpp"

#include "manager/box_servo_manager.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct Servomanager_under_test : public box::Servomanager {
    Servomanager_under_test(box::Servomotor* box_lower_servo,
                            box::Servomotor* box_upper_servo,
                            box::Wait* box_wait_till_servomanager_finished_moving,
                            box::Switch* box_switch)
                    : Servomanager(box_lower_servo, box_upper_servo,
                                    box_wait_till_servomanager_finished_moving, box_switch) {}
};


class TestServomanager : public ::testing::Test {
      protected:
    Servomanager_under_test* servomanager_under_test;
    virtual void SetUp() {
        servomotor_mock = new NiceMock<ServomotorMock>;
        arduino_mock = new NiceMock<ArduinoMock>;
        box_lower_servo_mock = new NiceMock<BoxServoMock>;
        box_upper_servo_mock = new NiceMock<BoxServoMock>;
        box_wait_till_servomanager_finished_moving = new NiceMock<BoxWaitMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        servomanager_under_test = new Servomanager_under_test(
                                            (box::Servomotor*) box_lower_servo_mock,
                                            (box::Servomotor*) box_upper_servo_mock,
                                            (box::Wait*) box_wait_till_servomanager_finished_moving,
                                            (box::Switch*) box_switch_mock);
    }
    virtual void TearDown() {
        delete servomanager_under_test;
        delete box_switch_mock;
        delete box_wait_till_servomanager_finished_moving;
        delete box_upper_servo_mock;
        delete box_lower_servo_mock;
        delete arduino_mock;
        delete servomotor_mock;
    }
};

TEST_F(TestServomanager, test_servomanager_init) { EXPECT_TRUE(true); }

TEST_F(TestServomanager, test_servomanager_test_move_steps) {
    EXPECT_CALL(*box_lower_servo_mock, move());
    EXPECT_CALL(*box_upper_servo_mock, move());
    servomanager_under_test->move_motors();
}

TEST_F(TestServomanager, test_servomanager_move_pilot_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result,2)).WillOnce(Return(100));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(100));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_,_)).Times(0);
    servomanager_under_test->move_pilot_servo_to_percent(50,2);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result,4)).WillOnce(Return(200));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(200));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_,_)).Times(0);
    servomanager_under_test->move_pilot_servo_to_percent(50,4);
}

TEST_F(TestServomanager, test_servomanager_move_copilot_servo_percentage) {
    int expected_result = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result,5)).WillOnce(Return(300));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(300));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(_,_)).Times(0);
    servomanager_under_test->move_copilot_servo_to_percent(50,5);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result,6)).WillOnce(Return(1000));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(1000));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(_,_)).Times(0);
    servomanager_under_test->move_copilot_servo_to_percent(50,6);
}

TEST_F(TestServomanager, test_servomanager_move_both_servo_percentage) {
    int expected_result_pilot = 42;
    int expected_result_copilot = 50;
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot,4)).WillOnce(Return(1400));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(1400));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot,5)).WillOnce(Return(2000));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(2000));
    servomanager_under_test->move_pilot_servo_to_percent(42, 4);
    servomanager_under_test->move_copilot_servo_to_percent(50, 5);
     // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_pilot,3)).WillOnce(Return(1));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(1));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_copilot,2)).WillOnce(Return(2));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(2));
    servomanager_under_test->move_pilot_servo_to_percent(42, 3);
    servomanager_under_test->move_copilot_servo_to_percent(50, 2);
    // // otherway arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot,1)).WillOnce(Return(42));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(42));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot,2)).WillOnce(Return(2000));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(2000));
    servomanager_under_test->move_pilot_servo_to_percent(42, 1);
    servomanager_under_test->move_copilot_servo_to_percent(50, 2);
    // // same way arround
    EXPECT_CALL(*box_switch_mock, is_high()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_lower_servo_mock, move_to_percent(expected_result_pilot,5)).WillOnce(Return(555));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(555));
    EXPECT_CALL(*box_upper_servo_mock, move_to_percent(expected_result_copilot,6)).WillOnce(Return(2000));
    EXPECT_CALL(*box_wait_till_servomanager_finished_moving, milliseconds(2000));
    servomanager_under_test->move_pilot_servo_to_percent(42, 5);
    servomanager_under_test->move_copilot_servo_to_percent(50, 6);
}

TEST_F(TestServomanager, test_servomanager_box_servos_not_reached_switch) {
    EXPECT_CALL(*box_upper_servo_mock, current_angle_smaller_than_90_percent()).WillOnce(Return(true));
    EXPECT_CALL(*box_lower_servo_mock, current_angle_smaller_than_90_percent()).WillOnce(Return(true));
    EXPECT_TRUE(servomanager_under_test->box_servos_not_reached_switch());

    EXPECT_CALL(*box_upper_servo_mock, current_angle_smaller_than_90_percent()).WillOnce(Return(false));
    EXPECT_FALSE(servomanager_under_test->box_servos_not_reached_switch());

    EXPECT_CALL(*box_upper_servo_mock, current_angle_smaller_than_90_percent()).WillOnce(Return(true));
    EXPECT_CALL(*box_lower_servo_mock, current_angle_smaller_than_90_percent()).WillOnce(Return(false));
    EXPECT_FALSE(servomanager_under_test->box_servos_not_reached_switch());
}
