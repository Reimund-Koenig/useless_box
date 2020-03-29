#ifndef MOCK_BOX_SWITCH_HPP_
#define MOCK_BOX_SWITCH_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <box_switch.hpp>
#include <box_servomotor.hpp>
#include <box_servomanager.hpp>
#include <box_sonar.hpp>
#include <box_wait.hpp>
#include <box_mode_reset.hpp>
#include <box_mode_awareness.hpp>
#include <box_mode_normal.hpp>
#include <box_mode_startup.hpp>

class BoxServoMock : public box::Servomotor {
  public:
    BoxServoMock() : box::Servomotor(0,false,1,2) {}
    MOCK_METHOD1(move_to_percent, void(int));
    MOCK_METHOD0(get_last_percentage, int());
};

class BoxServoManagerMock : public box::Servomanager {
  public:
    BoxServoManagerMock() : box::Servomanager(NULL,NULL) {}
    MOCK_METHOD1(move_lower_servo_to_percent, void(int));
    MOCK_METHOD1(move_upper_servo_to_percent, void(int));
    MOCK_METHOD2(move_servos_to_percent, void(int, int));
    MOCK_METHOD0(is_no_box_action, bool());
    MOCK_METHOD1(move_steps, void(int));
    MOCK_METHOD0(random_select_if_vice_versa_mode_should_be_changed, void());
    MOCK_METHOD0(change_vise_versa_if_required_and_return_is_changed, bool());
};

class BoxSonarMock : public box::Sonar{
  public:
    BoxSonarMock() : box::Sonar(0,1) {}
    MOCK_METHOD0(get_average_distance_cm, unsigned int());
};

class BoxSwitchMock : public box::Switch {
  public:
    BoxSwitchMock() : box::Switch(1) {}
    MOCK_METHOD0(is_high, bool());
    MOCK_METHOD0(has_changed, bool());
};

class BoxWaitMock : public box::Wait {
  public:
    BoxWaitMock() : box::Wait() {}
    MOCK_METHOD1(milliseconds, void(unsigned long));
    MOCK_METHOD0(is_free, bool());
};

class BoxModeResetMock : public box::ModeReset {
  public:
    BoxModeResetMock() : box::ModeReset(NULL,NULL) {}
    MOCK_METHOD0(run, bool());
};

class BoxModeAwarenessMock : public box::ModeAwareness {
  public:
    BoxModeAwarenessMock() : box::ModeAwareness(NULL,NULL) {}
    MOCK_METHOD0(run, void());
};

class BoxModeNormalMock : public box::ModeNormal {
  public:
    BoxModeNormalMock() : box::ModeNormal(NULL,NULL) {}
    MOCK_METHOD0(run, void());
};

class BoxModeStartupMock : public box::ModeStartup {
  public:
    BoxModeStartupMock() : box::ModeStartup(NULL,NULL) {}
    MOCK_METHOD0(run, void());
};

::testing::NiceMock<BoxSwitchMock>* box_switch_mock;
::testing::NiceMock<BoxSonarMock>* box_sonar_mock;
::testing::NiceMock<BoxServoMock>* box_lower_servo_mock;
::testing::NiceMock<BoxServoMock>* box_upper_servo_mock;
::testing::NiceMock<BoxServoManagerMock>* box_servomanager_mock;
::testing::NiceMock<BoxWaitMock>* box_wait_mock;
::testing::NiceMock<BoxModeResetMock>* box_mode_reset_mock;
::testing::NiceMock<BoxModeAwarenessMock>* box_mode_awareness_mock;
::testing::NiceMock<BoxModeNormalMock>* box_mode_normal_mock;
::testing::NiceMock<BoxModeStartupMock>* box_mode_startup_mock;

#endif // MOCK_BOX_SWITCH_HPP_