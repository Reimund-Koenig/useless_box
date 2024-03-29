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
                             box::SubModeFunctionJitter* box_submode_function_jitter_mock) :
    ModeAwareness(box_servomanager_mock, box_submode_function_jitter_mock) {}
};

class TestModeAwareness : public ::testing::Test {
  protected:
    ModeAwareness_under_test* mode_awareness_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_submode_function_jitter_mock = new NiceMock<BoxModeFunctionJitterMock>;
        mode_awareness_under_test = new ModeAwareness_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::SubModeFunctionJitter*) box_submode_function_jitter_mock);
    }
    virtual void TearDown() {
        delete mode_awareness_under_test;
        delete box_submode_function_jitter_mock;
        delete box_servomanager_mock;
        delete arduino_mock;
    }

    virtual void ReRunSubmodeJitter(int distance, bool submode_finished) {
        EXPECT_CALL(*box_submode_function_jitter_mock, run()).WillOnce(Return(submode_finished));
        if(submode_finished) {
            EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(_,_));
        }
        EXPECT_FALSE(mode_awareness_under_test->run(distance));
    }

    virtual void RunSubmodeJitter(int speed, int run_submode, int distance) {
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(run_submode)) //jitter activate
                                             .WillOnce(Return(speed)); // speed
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_submode_function_jitter_mock, init(_));
        mode_awareness_under_test->run(distance);
    }
};

TEST_F(TestModeAwareness, test_wait_init) { EXPECT_TRUE(true); }


TEST_F(TestModeAwareness, test_awareness_distance) {
    int distance = 1;
    int speed = 6;
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100, 6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    mode_awareness_under_test->run(distance);
}

TEST_F(TestModeAwareness, test_awareness_jitter) {
    int speed = 3;
    int percent = 70;
    int run_submode_jitter = 99;
    int distance = 13;
    bool submode_finished = false;
    RunSubmodeJitter(speed, run_submode_jitter, distance);
    distance = 27;
    ReRunSubmodeJitter(distance, submode_finished);
    distance = 110;
    ReRunSubmodeJitter(distance, submode_finished);
    distance = 0;
    ReRunSubmodeJitter(distance, submode_finished);
    distance = 25;
    submode_finished = true;
    ReRunSubmodeJitter(distance, submode_finished);
    distance = 21;
    RunSubmodeJitter(speed, run_submode_jitter, distance);
    distance = 50;
    ReRunSubmodeJitter(distance, submode_finished);
}

TEST_F(TestModeAwareness, test_awareness_distance_blocker) {
    int total_runs = 8;
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(_,_)).Times(4);
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).Times(total_runs);
    mode_awareness_under_test->run(40); // 1th call
    mode_awareness_under_test->run(41);
    mode_awareness_under_test->run(47);
    mode_awareness_under_test->run(48); // 2nd call
    mode_awareness_under_test->run(47);
    mode_awareness_under_test->run(41);
    mode_awareness_under_test->run(40); // 3rd call
    mode_awareness_under_test->run(50); // 4rd call
}
