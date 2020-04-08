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
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(30);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_20_and_30) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(12));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(42,6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(24);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_15_and_20) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(0))   // speed
                                         .WillOnce(Return(12));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(62,1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(17);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_10_and_15) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3))   // speed
                                         .WillOnce(Return(10));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(80,4));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(11);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_0_and_10) {
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100, 6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(9);
}

TEST_F(TestModeAwareness, test_awareness_distance_blocker) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(0))   //15a -> speed
                                         .WillOnce(Return(12)) // 15a -> percent
                                         .WillOnce(Return(12))  // 20-> percent
                                         .WillOnce(Return(0))   //15b -> speed
                                         .WillOnce(Return(13)); // 15b -> percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(62,1)); // 15a
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(42,6)); // 20
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(63,1)); // 15b
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).Times(3);
    mode_awareness_under_test->run(15); // 15a
    mode_awareness_under_test->run(16);
    mode_awareness_under_test->run(17);
    mode_awareness_under_test->run(18);
    mode_awareness_under_test->run(19);
    mode_awareness_under_test->run(20); // 20 (5 difference)
    mode_awareness_under_test->run(19);
    mode_awareness_under_test->run(18);
    mode_awareness_under_test->run(17);
    mode_awareness_under_test->run(16);
    mode_awareness_under_test->run(15); // 15b
}
