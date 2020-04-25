#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "box_mode_startup.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeStartup_under_test : public box::ModeStartup {
    ModeStartup_under_test(box::Servomanager* box_servomanager_mock,
                          box::ModeFunctionJitter* box_mode_function_jitter_mock,
                          box::Wait* box_wait_mock,
                          box::Switch* box_switch_mock) :
    ModeStartup(box_servomanager_mock,box_mode_function_jitter_mock, box_wait_mock, box_switch_mock) {}
};

class TestModeStartup : public ::testing::Test {
  protected:
    ModeStartup_under_test* mode_startup_under_test;
    int box_speed_mock = 5;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_mode_function_jitter_mock = new NiceMock<BoxModeFunctionJitterMock>;
        mode_startup_under_test = new ModeStartup_under_test(
                                        (box::Servomanager*) box_servomanager_mock,
                                        (box::ModeFunctionJitter*) box_mode_function_jitter_mock,
                                        (box::Wait*) box_wait_mock,
                                        (box::Switch*) box_switch_mock);
    }
    virtual void TearDown() {
        delete mode_startup_under_test;
        delete box_mode_function_jitter_mock;
        delete box_switch_mock;
        delete box_wait_mock;
        delete box_servomanager_mock;
        delete arduino_mock;
    }

    virtual void runStartUpSequence() {
        // 1 - Reset both motors
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 2 - Move slowly both motors to 90%
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(90,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(90,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 3 - Move first one fast back
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 4 - Move second one fast back
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 5 - Move upper out
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(90,6));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 6 - Jitter upper motor (with the eye)
        EXPECT_CALL(*box_mode_function_jitter_mock, run(_,_,_,_,_)).WillOnce(Return(false))
                                                         .WillOnce(Return(false))
                                                         .WillOnce(Return(true));
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 2-> false
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 2-> false
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter  speed 2-> true

        EXPECT_CALL(*box_mode_function_jitter_mock, run(_,_,_,_,_)).WillRepeatedly(Return(true));
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 3 -> true
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 4 -> true
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 5 -> true
        EXPECT_FALSE(mode_startup_under_test->run()); // run jitter speed 6 -> true

        // 7 - Press button with lower while upper moving slowly back
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 8 - lower slowly back while upper moving slowly forward
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(70,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,2));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 9 - press button with upper
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 10 - upper back to 70%
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // 11 - Reset both motors
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_TRUE(mode_startup_under_test->run());
    }
};

TEST_F(TestModeStartup, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeStartup, test_start_with_low_switchstate) {
    // move switch to start state -> if low nothing happens
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    EXPECT_FALSE(mode_startup_under_test->run());

    // now run the startup sequence
    runStartUpSequence();
}

TEST_F(TestModeStartup, test_start_with_high_switchstate) {
    // move switch to start state -> if low nothing happens
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,6));
    EXPECT_FALSE(mode_startup_under_test->run());

    // now run the startup sequence
    runStartUpSequence();
}