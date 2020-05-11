#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_avr_sleep.hpp"
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
    Controller_under_test(bool enable_sleep_mode,
                    box::Switch* box_switch,
                    box::Sonar* box_sonar,
                    box::Servomanager* box_servomanager,
                    box::Wait* box_wait_controller,
                    box::Wait* box_wait_standby,
                    box::ModeManager* box_mode_manager)
                    : Controller(enable_sleep_mode,
                            box_switch,
                            box_sonar,
                            box_servomanager,
                            box_wait_controller,
                            box_wait_standby,
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
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_switch_mock = new NiceMock<BoxSwitchMock>;
        box_sonar_mock = new NiceMock<BoxSonarMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        box_wait_deep_sleep_mock = new NiceMock<BoxWaitMock>;
        avr_sleep_mock = new NiceMock<AvrSleepMock>;
        box_mode_manager_mock = new NiceMock<BoxModeManagerMock>;
        controller_under_test = new Controller_under_test(false,
                                              (box::Switch*) box_switch_mock,
                                              (box::Sonar*) box_sonar_mock,
                                              (box::Servomanager*) box_servomanager_mock,
                                              (box::Wait*) box_wait_mock,
                                              (box::Wait*) box_wait_deep_sleep_mock,
                                              (box::ModeManager*) box_mode_manager_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_switch_mock;
        delete box_sonar_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete box_wait_deep_sleep_mock;
        delete box_mode_manager_mock;
        delete avr_sleep_mock;
        delete controller_under_test;
    }
};

TEST_F(TestController, test_standby) {
    EXPECT_CALL(*box_wait_deep_sleep_mock, is_free()).WillOnce(Return(true));
    EXPECT_CALL(*arduino_mock, attachInterrupt(_,_,_)).Times(0);
    EXPECT_CALL(*avr_sleep_mock, set_sleep_mode(_)).Times(0);
    EXPECT_CALL(*avr_sleep_mock, sleep_enable()).Times(0);
    EXPECT_CALL(*avr_sleep_mock, sleep_cpu()).Times(0);
    EXPECT_CALL(*avr_sleep_mock, sleep_disable()).Times(0);
    EXPECT_CALL(*box_wait_deep_sleep_mock, milliseconds(300000)).Times(1);
    controller_under_test->run();
    EXPECT_CALL(*box_wait_deep_sleep_mock, is_free()).WillOnce(Return(false));
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(30));
    EXPECT_CALL(*box_servomanager_mock, move_steps());
    EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(false));
    EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_wakeup(_)).Times(1);
    controller_under_test->run();
}