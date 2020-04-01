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
#define NO_MODE_CHANGE -1
#define SWITCH_VICE_VERSA_AND_RERUN_MODE_RESET -1
#define NO_SWITCH_VICE_VERSA_MODE_RESET -2
#define NO_SWITCH_VICE_VERSA_MODE_AWARENESS MODE_AWARENESS
#define NO_SWITCH_VICE_VERSA_MODE_NORMAL MODE_NORMAL


class TestController : public ::testing::Test {
      protected:
    Controller_under_test* controller_under_test;
    bool execute_vice_versa = false;
    int do_change_mode_in_next_interation = -1;
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

    virtual void RunPreSteps(int next_mode, const int ultra_sonar_result) {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(ultra_sonar_result));
        EXPECT_CALL(*box_servomanager_mock, move_steps(_));
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(false));
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
        if(do_change_mode_in_next_interation >= 0) {
            EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).WillOnce(Return(false));
            EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(do_change_mode_in_next_interation));
        }
        do_change_mode_in_next_interation = next_mode;
    }

    /* User interrupt always lead to reset */
    virtual void RunMode_UserInterrupt(int switch_vice_versa) {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(42));
        EXPECT_CALL(*box_servomanager_mock, move_steps(_));
        EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).WillOnce(Return(true));
        EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed());
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(true));
        controller_under_test->run();
        execute_vice_versa = (switch_vice_versa == -1); // Will execute in the next iteration
        do_change_mode_in_next_interation = switch_vice_versa;
    }

    virtual bool RunUserInterrupt() {
        if(!execute_vice_versa) { return false; }
        EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).WillOnce(Return(true));
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(true));
        controller_under_test->run();
        execute_vice_versa = false;
        return true;
    }

    virtual void RunStartupMode(const int next_mode) {
        RunPreSteps(next_mode, 55);
        bool switch_mode = (next_mode>=0);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunResetMode(const int next_mode) {
        RunPreSteps(next_mode, 42);
        EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).WillOnce(Return(true));
        EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).WillOnce(Return(true));
        controller_under_test->run();
        execute_vice_versa = false;
    }

    virtual void RunNormalMode(const int next_mode) {
        RunPreSteps(next_mode, 42);
        bool switch_mode = (next_mode>=0);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }

    virtual void RunAwarenessMode(const int next_mode, const int ultra_sonar_result) {
        RunPreSteps(next_mode, ultra_sonar_result);
        bool switch_mode = (next_mode>=0);
        EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(ultra_sonar_result)).WillOnce(Return(switch_mode));
        controller_under_test->run();
    }
};

TEST_F(TestController, test_controller_init) { EXPECT_TRUE(true); }


TEST_F(TestController, test_controller_startup_switch_with_user_interrupt) {
    RunMode_UserInterrupt(  NO_SWITCH_VICE_VERSA_MODE_RESET);
    RunResetMode(           MODE_NORMAL);
    RunNormalMode(          MODE_NORMAL);
    RunMode_UserInterrupt(  NO_SWITCH_VICE_VERSA_MODE_NORMAL);
    RunNormalMode(          MODE_NORMAL);
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt_switch_mode_to_normal) {
    RunStartupMode( MODE_NORMAL);
    RunNormalMode(  NO_MODE_CHANGE);
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt_switch_mode_to_awareness) {
    RunStartupMode( MODE_AWARENESS);
    RunAwarenessMode(  NO_MODE_CHANGE, 50);
}

TEST_F(TestController, test_controller_run_no_user_interrupts) {
    RunStartupMode(     NO_MODE_CHANGE);
    RunStartupMode(     MODE_NORMAL);
    RunNormalMode(      NO_MODE_CHANGE);
    RunNormalMode(      NO_MODE_CHANGE);
    RunNormalMode(      NO_MODE_CHANGE);
    RunNormalMode(      MODE_AWARENESS);
    RunAwarenessMode(   NO_MODE_CHANGE, 66);
    RunAwarenessMode(   NO_MODE_CHANGE, 45);
    RunAwarenessMode(   NO_MODE_CHANGE, 50);
}

TEST_F(TestController, test_controller_run_with_user_interrupt) {
    RunStartupMode(         NO_MODE_CHANGE);
    RunStartupMode(         MODE_NORMAL);
    RunNormalMode(          NO_MODE_CHANGE);
    RunMode_UserInterrupt(  SWITCH_VICE_VERSA_AND_RERUN_MODE_RESET);
    RunResetMode(           NO_MODE_CHANGE);
    RunMode_UserInterrupt(  NO_SWITCH_VICE_VERSA_MODE_NORMAL);
    RunNormalMode(          NO_MODE_CHANGE);
}

TEST_F(TestController, test_controller_test_is_free) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(AtLeast(1));
    EXPECT_CALL(*box_wait_mock, is_free()).WillRepeatedly(Return(false));
    EXPECT_CALL(*arduino_mock, random(_)).Times(0);
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(0);
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