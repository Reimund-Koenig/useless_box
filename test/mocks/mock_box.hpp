#ifndef MOCK_BOX_SWITCH_HPP_
#define MOCK_BOX_SWITCH_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <peripheral/box_switch.hpp>
#include <peripheral/box_servomotor.hpp>
#include <peripheral/box_sonar.hpp>
#include <manager/box_servo_manager.hpp>
#include <manager/box_wait_manager.hpp>
#include <manager/box_mode_manager.hpp>

class BoxServoMock : public box::Servomotor {
  public:
    BoxServoMock() : box::Servomotor(0,false,1,2,NULL) {}
    MOCK_METHOD2(move_to_percent, int(int, int));
    MOCK_METHOD0(move_step, void());
    MOCK_METHOD0(get_last_percentage, int());
    MOCK_METHOD0(get_angle, int());
    MOCK_METHOD0(get_current_angle, int());
    MOCK_METHOD0(current_angle_smaller_than_90_percent, bool());
};

class BoxServoManagerMock : public box::Servomanager {
  public:
    BoxServoManagerMock() : box::Servomanager(NULL,NULL,NULL,NULL) {}
    MOCK_METHOD2(move_pilot_servo_to_percent, void(int,int));
    MOCK_METHOD2(move_copilot_servo_to_percent, void(int,int));
    MOCK_METHOD0(box_servos_not_reached_switch, bool());
    MOCK_METHOD0(move_steps, void());
};

class BoxSonarMock : public box::Sonar{
  public:
    BoxSonarMock() : box::Sonar(0,1) {}
    MOCK_METHOD0(get_average_distance_cm, unsigned int());
    MOCK_METHOD0(get_median_distance_cm, unsigned int());
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
    MOCK_METHOD1(add_milliseconds, void(unsigned long));
    MOCK_METHOD0(is_expired, bool());
};

class BoxModeManagerMock : public box::ModeManager {
  public:
    BoxModeManagerMock() : box::ModeManager(NULL,NULL) {}
    MOCK_METHOD1(run_mode_awareness, bool(int));
    MOCK_METHOD0(run_mode_reset, bool());
};

class BoxModeFunctionJitterMock : public box::SubModeFunctionJitter {
  public:
    BoxModeFunctionJitterMock() : box::SubModeFunctionJitter(NULL) {}
    MOCK_METHOD0(run, bool());
    MOCK_METHOD1(init, void(int));
};

::testing::NiceMock<BoxSwitchMock>* box_switch_mock;
::testing::NiceMock<BoxSonarMock>* box_sonar_mock;
::testing::NiceMock<BoxServoMock>* box_lower_servo_mock;
::testing::NiceMock<BoxServoMock>* box_upper_servo_mock;
::testing::NiceMock<BoxServoManagerMock>* box_servomanager_mock;
::testing::NiceMock<BoxWaitMock>* box_wait_till_servomanager_finished_moving;
::testing::NiceMock<BoxWaitMock>* box_wait_servo_step_speed;
::testing::NiceMock<BoxWaitMock>* box_wait_deep_sleep_mock;
::testing::NiceMock<BoxModeManagerMock>* box_mode_manager_mock;
::testing::NiceMock<BoxModeFunctionJitterMock>* box_submode_function_jitter_mock;

#endif // MOCK_BOX_SWITCH_HPP_
