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
    int box_speed_mock = 5;
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
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(5));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 5;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(100,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(6));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 6;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(40));
    EXPECT_TRUE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
}

TEST_F(TestModeReset, test_switchsate_high_change_direction) {
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(100,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_CALL(*arduino_mock, random(100)).WillOnce(Return(51));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(100,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_FALSE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
    EXPECT_CALL(*box_servomanager_mock, move_servos_to_percent(0,0)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3));
    EXPECT_TRUE(mode_reset_under_test->run(&box_speed_mock));
    box_speed_mock = 3;
}