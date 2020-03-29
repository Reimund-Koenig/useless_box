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

struct Controller_under_test : public box::Controller {
    Controller_under_test(box::Switch* box_switch,
                    box::Sonar* box_sonar,
                    box::Servomanager* box_servomanager,
                    box::Wait* box_wait,
                    box::ModeReset* box_mode_reset,
                    box::ModeNormal* box_mode_normal,
                    box::ModeAwareness* box_mode_awareness)
                    : Controller(box_switch,
                            box_sonar,
                            box_servomanager,
                            box_wait,
                            box_mode_reset,
                            box_mode_normal,
                            box_mode_awareness) {}
};


class TestController : public ::testing::Test {
      protected:
    Controller_under_test* controller_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_sonar_mock = new NiceMock<BoxSonarMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_mode_reset_mock = new NiceMock<BoxModeResetMock>;
        box_mode_normal_mock = new NiceMock<BoxModeNormalMock>;
        box_mode_awareness_mock = new NiceMock<BoxModeAwarenessMock>;
        controller_under_test = new Controller_under_test((box::Switch*) box_switch_mock,
                                              (box::Sonar*) box_sonar_mock,
                                              (box::Servomanager*) box_servomanager_mock,
                                              (box::Wait*) box_wait_mock,
                                              (box::ModeReset*) box_mode_reset_mock,
                                              (box::ModeNormal*) box_mode_normal_mock,
                                              (box::ModeAwareness*) box_mode_awareness_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_switch_mock;
        delete box_sonar_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete box_mode_reset_mock;
        delete box_mode_normal_mock;
        delete box_mode_awareness_mock;
        delete controller_under_test;
    }
    virtual void select_new_box_mode_is_called(const int n_times) {
        EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(n_times);
    }
    virtual void startup_switch_high_is_called(const int n_times) {
               EXPECT_CALL(*box_switch_mock, has_changed()).Times(n_times).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(n_times).WillOnce(Return(true));
        EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(n_times);
    }
    virtual void get_average_distance_cm_is_called(const int n_times, const int returns) {
        EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(n_times).WillOnce(Return(returns));
    }
    virtual void move_steps_is_called(const int n_times) {
        EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(n_times);
    }
    virtual void is_free_is_called(const int n_times, const bool returns) {
        EXPECT_CALL(*box_wait_mock, is_free()).Times(n_times).WillOnce(Return(returns));
    }
    virtual void mode_reset_is_called(const int n_times, const bool returns) {
        EXPECT_CALL(*box_mode_reset_mock, run()).Times(n_times).WillOnce(Return(returns));
    }
};

TEST_F(TestController, test_controller_init) { EXPECT_TRUE(true); }

TEST_F(TestController, test_controller_startup_switch_high) {
    get_average_distance_cm_is_called(1,70);
    move_steps_is_called(1);
    startup_switch_high_is_called(1);
    is_free_is_called(1, true);
    select_new_box_mode_is_called(0);
    mode_reset_is_called(1,true);
    controller_under_test->run();
}

