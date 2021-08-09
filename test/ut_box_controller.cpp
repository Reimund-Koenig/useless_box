#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_low_power.hpp"
#include "mock_serial.hpp"
#include "mock_box.hpp"

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
                    box::Wait* box_wait_controller,
                    box::Wait* box_wait_deepsleep,
                    box::ModeManager* box_mode_manager,
                    int pin_power_servos,
                    int pin_power_sonar)
                    : Controller(box_switch,
                            box_sonar,
                            box_servomanager,
                            box_wait_controller,
                            box_wait_deepsleep,
                            box_mode_manager,
                            pin_power_servos,
                            pin_power_sonar) {}
};

#define SWITCH_TO_NEXT_MODE true
#define STAY_IN_CURRENT_MODE false
#define MODE_AWARENESS 74
#define MODE_FINISHED true
#define MODE_NOT_FINISHED false
#define SWITCH_VICE_VERSA_AND_RERUN_MODE_RESET -1
#define NO_SWITCH_VICE_VERSA_MODE_RESET -2
#define NO_SWITCH_VICE_VERSA_MODE_AWARENESS MODE_AWARENESS


class TestController : public ::testing::Test {
      protected:
    Controller_under_test* controller_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_sonar_mock = new NiceMock<BoxSonarMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_wait_deep_sleep_mock = new NiceMock<BoxWaitMock>;
        low_power_mock = new NiceMock<LowPowerMock>;
        box_mode_manager_mock = new NiceMock<BoxModeManagerMock>;
        int pin_power_servos = 0;
        int pin_power_sonar = 1;
        controller_under_test = new Controller_under_test((box::Switch*) box_switch_mock,
                                              (box::Sonar*) box_sonar_mock,
                                              (box::Servomanager*) box_servomanager_mock,
                                              (box::Wait*) box_wait_mock,
                                              (box::Wait*) box_wait_deep_sleep_mock,
                                              (box::ModeManager*) box_mode_manager_mock,
                                              pin_power_servos,
                                              pin_power_sonar);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_switch_mock;
        delete box_sonar_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete box_wait_deep_sleep_mock;
        delete box_mode_manager_mock;
        delete low_power_mock;
        delete controller_under_test;
    }

    virtual void RunPreSteps(const int ultra_sonar_result) {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(ultra_sonar_result));
        EXPECT_CALL(*box_servomanager_mock, move_steps());
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(false));
        EXPECT_CALL(*box_wait_mock, is_expired()).WillOnce(Return(true));
        EXPECT_CALL(*box_wait_deep_sleep_mock, is_expired()).WillRepeatedly(Return(false));
    }

    virtual void RunResetMode(const bool switch_mode) {
        RunPreSteps(42);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunAwarenessMode(const bool switch_mode, const int ultra_sonar_result) {
        RunPreSteps(ultra_sonar_result);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(ultra_sonar_result)).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunUserInterrupt() {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(5));
        EXPECT_CALL(*box_servomanager_mock, move_steps());
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, box_servos_not_reached_switch()).WillOnce(Return(true));
        EXPECT_CALL(*box_wait_mock, is_expired()).WillOnce(Return(true));
        EXPECT_CALL(*box_wait_deep_sleep_mock, is_expired()).WillRepeatedly(Return(false));
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(true));
        controller_under_test->run();
    }
};

TEST_F(TestController, test_controller_init) { EXPECT_TRUE(true); }

TEST_F(TestController, test_controller_without_user_interrupt) {
    RunResetMode(   MODE_FINISHED);
    RunAwarenessMode(  MODE_NOT_FINISHED, 60);
    RunAwarenessMode(  MODE_NOT_FINISHED, 50);
    RunAwarenessMode(  MODE_FINISHED, 30);
    RunResetMode(   MODE_NOT_FINISHED);
    RunResetMode(   MODE_FINISHED);
    RunAwarenessMode(  MODE_NOT_FINISHED, 60);
    RunAwarenessMode(  MODE_NOT_FINISHED, 50);
    RunAwarenessMode(  MODE_NOT_FINISHED, 40);
    RunAwarenessMode(  MODE_NOT_FINISHED, 30);
    RunAwarenessMode(  MODE_FINISHED, 30);
    RunResetMode(   MODE_FINISHED);
}

TEST_F(TestController, test_controller_run_with_user_interrupt) {
    RunResetMode(   MODE_FINISHED);
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_FINISHED, 50);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_FINISHED, 50);
    RunUserInterrupt();
    RunAwarenessMode(   MODE_NOT_FINISHED, 50);
}

TEST_F(TestController, test_controller_test_is_free) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps()).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, box_servos_not_reached_switch()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_wait_mock, is_expired()).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_wait_deep_sleep_mock, is_expired()).WillRepeatedly(Return(false));
    EXPECT_CALL(*arduino_mock, random(_)).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(_)).Times(0);
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
    controller_under_test->run();
}

TEST_F(TestController, test_deep_sleep) {
    EXPECT_CALL(*box_wait_deep_sleep_mock, is_expired()).WillOnce(Return(true));
    EXPECT_CALL(*arduino_mock, attachInterrupt(_,_,_)).Times(1);
    EXPECT_CALL(*low_power_mock, powerDown(_,_,_)).Times(1);
    controller_under_test->run();
}