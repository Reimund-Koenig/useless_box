#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "modes/box_mode_awareness.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeAwareness_under_test : public box::ModeAwareness {
    ModeAwareness_under_test(box::Servomanager* box_servomanager_mock,
                             box::ModeFunctionJitter* box_mode_function_jitter_mock,
                             box::Wait* box_wait_mock) :
    ModeAwareness(box_servomanager_mock, box_mode_function_jitter_mock, box_wait_mock) {}
};

class TestModeAwareness : public ::testing::Test {
  protected:
    ModeAwareness_under_test* mode_awareness_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_mode_function_jitter_mock = new NiceMock<BoxModeFunctionJitterMock>;
        mode_awareness_under_test = new ModeAwareness_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::ModeFunctionJitter*) box_mode_function_jitter_mock,
                                                (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete mode_awareness_under_test;
        delete box_mode_function_jitter_mock;
        delete box_wait_mock;
        delete box_servomanager_mock;
        delete arduino_mock;
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
                                         .WillOnce(Return(12))  // percent
                                         .WillOnce(Return(1));  // jitter
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(62,1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(17);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_10_and_15) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3))   // speed
                                         .WillOnce(Return(10))  // percent
                                         .WillOnce(Return(1));  // jitter
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(80,4));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(11);
}

TEST_F(TestModeAwareness, test_awareness_jitter) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3))   // speed
                                         .WillOnce(Return(10))  // percent
                                         .WillOnce(Return(99))  // jitter activate
                                         .WillOnce(Return(2))  // jitter count
                                         .WillOnce(Return(3))  // jitter speed
                                         .WillOnce(Return(5));  // jitter range
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(80,4));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(11);
    EXPECT_CALL(*box_mode_function_jitter_mock, run(true, _,90,80,_)).Times(5).WillRepeatedly(Return(false));
    mode_awareness_under_test->run(11);
    mode_awareness_under_test->run(70);
    mode_awareness_under_test->run(0);
    mode_awareness_under_test->run(110);
    mode_awareness_under_test->run(11);
}

TEST_F(TestModeAwareness, test_awareness_jitter_greater_95) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3))   // speed
                                         .WillOnce(Return(19))  // percent
                                         .WillOnce(Return(99))  // jitter activate
                                         .WillOnce(Return(2))  // jitter count
                                         .WillOnce(Return(3))  // jitter speed
                                         .WillOnce(Return(5));  // jitter range
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(89,4));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(11);
    EXPECT_CALL(*box_mode_function_jitter_mock, run(true, _,79,89,_)).Times(5).WillRepeatedly(Return(false));
    mode_awareness_under_test->run(11);
    mode_awareness_under_test->run(70);
    mode_awareness_under_test->run(0);
    mode_awareness_under_test->run(110);
    mode_awareness_under_test->run(11);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_0_and_10) {
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100, 6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(9);
}

TEST_F(TestModeAwareness, test_awareness_distance_blocker) {
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(0))   // 15a -> speed
                                         .WillOnce(Return(12))  // 15a -> percent
                                         .WillOnce(Return(50))  // 15a -> jitter pecent
                                         .WillOnce(Return(12))  // 20-> percent
                                         .WillOnce(Return(0))   // 15b -> speed
                                         .WillOnce(Return(13))  // 15b -> percent
                                         .WillOnce(Return(50)); // 15b -> jitter pecent
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
