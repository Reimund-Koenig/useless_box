#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_box.hpp"
#include "box_mode_reset.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::InSequence;

struct ModeReset_under_test : public box::ModeReset {
    ModeReset_under_test(box::Servomanager* box_servomanager_mock,
                         box::Wait* box_wait_mock) :
    ModeReset(box_servomanager_mock, box_wait_mock) {}
};

class TestModeReset : public ::testing::Test {
  protected:
    ModeReset_under_test* mode_reset_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        mode_reset_under_test = new ModeReset_under_test(
                                    (box::Servomanager*) box_servomanager_mock,
                                    (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete mode_reset_under_test;
    }
};

TEST_F(TestModeReset, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeReset, test_switchsate_high_no_change) {
    int rnd_result = 2;
    const int expected_speed = rnd_result+3;
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_result));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,expected_speed))
                                            .WillOnce(Return(333));
    EXPECT_CALL(*box_wait_mock, milliseconds(333)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6))
                                                            .WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6))
                                                            .WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(40));
    EXPECT_TRUE(mode_reset_under_test->run());
}

TEST_F(TestModeReset, test_switchsate_high_change_direction) {
    int rnd_result = 0;
    const int expected_speed = rnd_result+3;
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,expected_speed))
                                                        .WillOnce(Return(42));
    EXPECT_CALL(*box_wait_mock, milliseconds(42)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_result));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6))
                                                        .WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6))
                                                        .WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(51));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,expected_speed))
                                                        .WillOnce(Return(1000));
    EXPECT_CALL(*box_wait_mock, milliseconds(1000)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_result));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6))
                                                        .WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6))
                                                        .WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_reset_under_test->run());
}