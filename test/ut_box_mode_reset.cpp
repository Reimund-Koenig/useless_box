#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "modes/box_mode_reset.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::InSequence;

struct ModeReset_under_test : public box::ModeReset {
    ModeReset_under_test(box::Servomanager* box_servomanager_mock,
                         box::Wait* box_wait_till_servo_finished_moving) :
    ModeReset(box_servomanager_mock) {}
};

class TestModeReset : public ::testing::Test {
  protected:
    ModeReset_under_test* mode_reset_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_till_servo_finished_moving = new NiceMock<BoxWaitMock>;
        mode_reset_under_test = new ModeReset_under_test(
                                    (box::Servomanager*) box_servomanager_mock,
                                    (box::Wait*) box_wait_till_servo_finished_moving);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_till_servo_finished_moving;
        delete mode_reset_under_test;
    }

    virtual void reset_mode_without_changing_switch() {
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,6));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_FALSE(mode_reset_under_test->run());

        int rnd_result = 2;
        int expected_speed = rnd_result+3;
        EXPECT_CALL(*arduino_mock, random(4)).WillOnce(Return(rnd_result));
        EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(49));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0, expected_speed));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0, expected_speed));
        EXPECT_TRUE(mode_reset_under_test->run());
    }

    virtual void reset_mode_with_changing_switch() {
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,6));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_FALSE(mode_reset_under_test->run());

        int rnd_result = 0;
        int expected_speed = rnd_result+3;
        EXPECT_CALL(*arduino_mock, random(4)).WillOnce(Return(rnd_result));
        EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(51));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0, expected_speed));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0, expected_speed));
        EXPECT_FALSE(mode_reset_under_test->run());

        rnd_result = 2;
        expected_speed = rnd_result+3;
        EXPECT_CALL(*arduino_mock, random(4)).WillOnce(Return(rnd_result));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,expected_speed));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,expected_speed));
        EXPECT_FALSE(mode_reset_under_test->run());

        rnd_result = 3;
        expected_speed = rnd_result+3;
        EXPECT_CALL(*arduino_mock, random(4)).WillOnce(Return(rnd_result));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0, expected_speed));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0, expected_speed));
        EXPECT_TRUE(mode_reset_under_test->run());
    }
};

TEST_F(TestModeReset, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeReset, test_reset_mode) {
    reset_mode_without_changing_switch();
    reset_mode_with_changing_switch();
    reset_mode_with_changing_switch();
    reset_mode_without_changing_switch();
    reset_mode_without_changing_switch();
}
