#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_serial.hpp"
#include "mocks/mock_box.hpp"

#include "box_controller.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::InSequence;

struct Controller_under_test : public box::Controller {
    Controller_under_test(box::Switch* box_switch,
                    box::Sonar* box_sonar,
                    box::Servomanager* box_servomanager,
                    box::Wait* box_wait,
                    box::ModeManager* box_mode_manager)
                    : Controller(box_switch,
                            box_sonar,
                            box_servomanager,
                            box_wait,
                            box_mode_manager) {}
};

#define SWITCH_TO_NEXT_MODE true
#define STAY_IN_CURRENT_MODE false
#define MODE_AWARENESS 74
#define MODE_NORMAL 75
#define MODE_FINISHED true
#define MODE_NOT_FINISHED false
#define SWITCH_VICE_VERSA_AND_RERUN_MODE_RESET -1
#define NO_SWITCH_VICE_VERSA_MODE_RESET -2
#define NO_SWITCH_VICE_VERSA_MODE_AWARENESS MODE_AWARENESS
#define NO_SWITCH_VICE_VERSA_MODE_NORMAL MODE_NORMAL


class TestController : public ::testing::Test {
      protected:
    Controller_under_test* controller_under_test;
    int current_mode;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_sonar_mock = new NiceMock<BoxSonarMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_mode_manager_mock = new NiceMock<BoxModeManagerMock>;
        controller_under_test = new Controller_under_test((box::Switch*) box_switch_mock,
                                              (box::Sonar*) box_sonar_mock,
                                              (box::Servomanager*) box_servomanager_mock,
                                              (box::Wait*) box_wait_mock,
                                              (box::ModeManager*) box_mode_manager_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_switch_mock;
        delete box_sonar_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete box_mode_manager_mock;
        delete controller_under_test;
    }

    virtual void CheckCallSelectNewBoxMode(int next_mode) {
        if(next_mode == current_mode) { return; }
        current_mode = next_mode;
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(next_mode));
    }

    virtual void RunPreSteps(const int ultra_sonar_result) {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(ultra_sonar_result));
        EXPECT_CALL(*box_servomanager_mock, move_steps());
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(false));
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
    }

    virtual void RunStartupMode(bool switch_mode) {
        RunPreSteps(55);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunResetMode(const bool switch_mode) {
        RunPreSteps(42);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunNormalMode(const bool switch_mode) {
        RunPreSteps(42);
        CheckCallSelectNewBoxMode(MODE_NORMAL);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunAwarenessMode(const bool switch_mode, const int ultra_sonar_result) {
        RunPreSteps(ultra_sonar_result);
        CheckCallSelectNewBoxMode(MODE_AWARENESS);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(ultra_sonar_result)).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunUserInterrupt() {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(5));
        EXPECT_CALL(*box_servomanager_mock, move_steps());
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, box_servos_not_reached_switch()).WillOnce(Return(true));
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(true));
        controller_under_test->run();
    }
};

TEST_F(TestController, test_controller_init) { EXPECT_TRUE(true); }

TEST_F(TestController, test_controller_without_user_interrupt) {
    RunStartupMode( MODE_NOT_FINISHED);
    RunStartupMode( MODE_FINISHED);
    RunNormalMode(  MODE_NOT_FINISHED);
    RunNormalMode(  MODE_NOT_FINISHED);
    RunNormalMode(  MODE_FINISHED);
    RunResetMode(   MODE_NOT_FINISHED);
    RunResetMode(   MODE_FINISHED);
    RunAwarenessMode(  MODE_NOT_FINISHED, 60);
    RunAwarenessMode(  MODE_NOT_FINISHED, 50);
    RunAwarenessMode(  MODE_NOT_FINISHED, 40);
    RunAwarenessMode(  MODE_NOT_FINISHED, 30);
    RunAwarenessMode(  MODE_FINISHED, 30);
    RunResetMode(   MODE_FINISHED);
}

TEST_F(TestController, test_controller_startup_to_awareness_no_user_interrupt) {
    RunStartupMode(     MODE_FINISHED);
    RunAwarenessMode(   MODE_FINISHED, 50);
    RunResetMode(       MODE_FINISHED);
    RunNormalMode(      MODE_NOT_FINISHED);
}

TEST_F(TestController, test_controller_run_with_user_interrupt) {
    RunStartupMode(     MODE_NOT_FINISHED);
    RunStartupMode(     MODE_NOT_FINISHED);
    RunUserInterrupt();
    RunNormalMode(      MODE_NOT_FINISHED);
    RunNormalMode(      MODE_NOT_FINISHED);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunUserInterrupt();
    RunNormalMode(      MODE_FINISHED);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_FINISHED, 50);
    RunUserInterrupt();
    RunNormalMode(      MODE_NOT_FINISHED);
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt) {
    RunStartupMode(     MODE_FINISHED);
    RunUserInterrupt();
    RunNormalMode(      MODE_NOT_FINISHED);
}

TEST_F(TestController, test_controller_test_is_free) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps()).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, box_servos_not_reached_switch()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_wait_mock, is_free()).WillRepeatedly(Return(false));
    EXPECT_CALL(*arduino_mock, random(_)).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(_)).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).Times(0);
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
}