#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_box.hpp"
#include "box_mode_awareness.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeAwareness_under_test : public box::ModeAwareness {
    ModeAwareness_under_test(box::Servomanager* box_servomanager_mock, box::Wait* box_wait_mock) :
    ModeAwareness(box_servomanager_mock, box_wait_mock) {}
};

class TestModeAwareness : public ::testing::Test {
  protected:
    ModeAwareness_under_test* mode_awareness_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        mode_awareness_under_test = new ModeAwareness_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete mode_awareness_under_test;
    }
};

TEST_F(TestModeAwareness, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeAwareness, test_awareness_distance_greater_30) {
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(50)).Times(1);
    mode_awareness_under_test->run(30);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_20_and_30) {
    int rnd_speed_result = 2;
    const int expected_speed = rnd_speed_result+1;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_speed_result))
                                         .WillOnce(Return(12))
                                         .WillOnce(Return(250));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(42,expected_speed)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(500)).Times(1);
    mode_awareness_under_test->run(24);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_10_and_20) {
    int rnd_speed_result = 1;
    const int expected_speed = rnd_speed_result+1;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_speed_result))
                                         .WillOnce(Return(10))
                                         .WillOnce(Return(500));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(60,expected_speed)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(750)).Times(1);
    mode_awareness_under_test->run(11);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_0_and_10) {
    int rnd_speed_result = 3;
    const int expected_speed = rnd_speed_result+1;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_speed_result));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100, expected_speed)).Times(1);
    EXPECT_CALL(*box_wait_mock, milliseconds(250)).Times(1);
    mode_awareness_under_test->run(9);
}