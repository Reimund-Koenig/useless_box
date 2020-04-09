#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_box.hpp"
#include "box_mode_startup.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeStartup_under_test : public box::ModeStartup {
    ModeStartup_under_test(box::Servomanager* box_servomanager_mock,
                          box::Wait* box_wait_mock,
                          box::Switch* box_switch_mock) :
    ModeStartup(box_servomanager_mock, box_wait_mock, box_switch_mock) {}
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
        mode_startup_under_test = new ModeStartup_under_test(
                                        (box::Servomanager*) box_servomanager_mock,
                                        (box::Wait*) box_wait_mock,
                                        (box::Switch*) box_switch_mock);
    }
    virtual void TearDown() {
        delete mode_startup_under_test;
        delete box_switch_mock;
        delete box_wait_mock;
        delete box_servomanager_mock;
        delete arduino_mock;
    }

    virtual void runStartUpSequence() {
        // Reset both motors
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Move slowly both motors to 90%
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(90,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(90,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Move first one fast back
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Move second one fast back
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Move upper out
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(90,6));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Jitter upper motor (with the eye)
        EXPECT_FALSE(mode_startup_under_test->run()); // set start values
        for(int i = 0; i<5; i++) {
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(_,_)).Times(2);
            EXPECT_FALSE(mode_startup_under_test->run()); // move 70 percent
            EXPECT_FALSE(mode_startup_under_test->run()); // move 90 percent
        }

        // Press button with lower while upper moving slowly back
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,_));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // lower slowly back while upper moving slowly forward
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(70,_));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,2));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // press button with upper
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // upper back to 70%
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(_,_));
        EXPECT_CALL(*box_wait_mock, add_milliseconds(_));
        EXPECT_FALSE(mode_startup_under_test->run());

        // Reset both motors
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