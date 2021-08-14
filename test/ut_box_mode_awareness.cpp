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
                             box::SubModeFunctionJitter* box_submode_function_jitter_mock,
                             box::Wait* box_wait_mock) :
    ModeAwareness(box_servomanager_mock, box_submode_function_jitter_mock) {}
};

class TestModeAwareness : public ::testing::Test {
  protected:
    ModeAwareness_under_test* mode_awareness_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_submode_function_jitter_mock = new NiceMock<BoxModeFunctionJitterMock>;
        mode_awareness_under_test = new ModeAwareness_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::SubModeFunctionJitter*) box_submode_function_jitter_mock,
                                                (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete mode_awareness_under_test;
        delete box_submode_function_jitter_mock;
        delete box_wait_mock;
        delete box_servomanager_mock;
        delete arduino_mock;
    }

    virtual void ReRunSubmodeJitter(int distance, bool submode_finished) {
        EXPECT_CALL(*box_submode_function_jitter_mock, run(true)).WillOnce(Return(submode_finished));
        EXPECT_FALSE(mode_awareness_under_test->run(distance));
    }

    virtual void RunSubmodeJitter(int speed, int percent, int run_submode, int distance) {
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(run_submode)) //jitter activate
                                             .WillOnce(Return(speed)) // speed
                                             .WillOnce(Return(percent)); // percent
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_submode_function_jitter_mock, init(_));
        mode_awareness_under_test->run(distance);
    }
};

TEST_F(TestModeAwareness, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeAwareness, test_awareness_distance_greater_30) {
    int distance = 31;
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(distance);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_20_and_30) {
    int distance = 24;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(1))  // jitter
                                         .WillOnce(Return(0))   // speed
                                         .WillOnce(Return(12));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(42,1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(distance);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_15_and_20) {
    int distance = 17;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(1))  // jitter
                                         .WillOnce(Return(0))   // speed
                                         .WillOnce(Return(12));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(62,1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(distance);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_10_and_15) {
    int distance = 11;
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(3))   // speed
                                         .WillOnce(Return(10));  // percent
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(80,4));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(distance);
}

TEST_F(TestModeAwareness, test_awareness_distance_between_0_and_10) {
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100, 6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(9);
}

TEST_F(TestModeAwareness, test_awareness_jitter) {
    int speed = 3;
    int percent = 10;
    int run_submode_jitter = 99;
    int distance1 = 16;
    int distance2 = 22;
    bool submode_finished = false;
    RunSubmodeJitter(speed, percent, run_submode_jitter, distance1);
    ReRunSubmodeJitter(distance2, submode_finished);
    distance2 = 110;
    ReRunSubmodeJitter(distance2, submode_finished);
    distance2 = 0;
    ReRunSubmodeJitter(distance2, submode_finished);
    submode_finished = true;
    ReRunSubmodeJitter(distance2, submode_finished);
    distance1 = 22;
    RunSubmodeJitter(speed, percent, run_submode_jitter, distance1);
    ReRunSubmodeJitter(distance2, submode_finished);
}

TEST_F(TestModeAwareness, test_awareness_distance_blocker) {
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).Times(3);
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).Times(3);
    mode_awareness_under_test->run(40); // 40a 1th call
    mode_awareness_under_test->run(41);
    mode_awareness_under_test->run(42);
    mode_awareness_under_test->run(43);
    mode_awareness_under_test->run(44);
    mode_awareness_under_test->run(45); // 45  2nd call (5 difference)
    mode_awareness_under_test->run(44);
    mode_awareness_under_test->run(43);
    mode_awareness_under_test->run(42);
    mode_awareness_under_test->run(41);
    mode_awareness_under_test->run(40); // 40b 3rd call (5 difference)
}
