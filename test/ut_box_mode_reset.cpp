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
                         box::Wait* box_wait_mock,
                         box::Switch* box_switch_mock) :
    ModeReset(box_servomanager_mock, box_wait_mock, box_switch_mock) {}
};

class TestModeReset : public ::testing::Test {
  protected:
    ModeReset_under_test* mode_reset_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        mode_reset_under_test = new ModeReset_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::Wait*) box_wait_mock,
                                                (box::Switch*) box_switch_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete box_switch_mock;
        delete mode_reset_under_test;
    }
};

TEST_F(TestModeReset, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeReset, test_switchsate_high_no_vice_versa) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(HIGH));
    EXPECT_CALL(*box_servomanager_mock, is_vice_versa_mode_activated()).WillOnce(Return(false));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,100)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_upper_servo_to_percent(0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_reset_under_test->run());
}

TEST_F(TestModeReset, test_switchsate_low_no_vice_versa) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(LOW));
    EXPECT_CALL(*box_servomanager_mock, is_vice_versa_mode_activated()).WillOnce(Return(false));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(100,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_lower_servo_to_percent(0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_reset_under_test->run());
}

TEST_F(TestModeReset, test_switchsate_high_with_vice_versa) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(HIGH));
    EXPECT_CALL(*box_servomanager_mock, is_vice_versa_mode_activated()).WillOnce(Return(true));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(100,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_lower_servo_to_percent(0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_reset_under_test->run());
}

TEST_F(TestModeReset, test_switchsate_low_with_vice_versa) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(LOW));
    EXPECT_CALL(*box_servomanager_mock, is_vice_versa_mode_activated()).WillOnce(Return(true));
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,100)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_FALSE(mode_reset_under_test->run());
    EXPECT_CALL(*box_servomanager_mock, move_upper_servo_to_percent(0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_reset_under_test->run());
}