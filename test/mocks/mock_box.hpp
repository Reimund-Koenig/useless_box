#ifndef MOCK_BOX_SWITCH_HPP_
#define MOCK_BOX_SWITCH_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <box_switch.hpp>
#include <box_servomotor.hpp>
#include <box_servomanager.hpp>
#include <box_sonar.hpp>

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
    MOCK_METHOD0(change_vise_versa_mode, bool());
};

class BoxSonarMock : public box::Sonar{
  public:
    BoxSonarMock() : box::Sonar(0,1) {}
    MOCK_METHOD0(get_average_distance_cm, int());
};

class BoxSwitchMock : public box::Switch {
  public:
    BoxSwitchMock() : box::Switch(1) {}
    MOCK_METHOD0(is_high, bool());
    MOCK_METHOD0(has_changed, bool());
};
::testing::NiceMock<BoxSwitchMock>* box_switch_mock;
::testing::NiceMock<BoxSonarMock>* box_sonar_mock;
::testing::NiceMock<BoxServoMock>* box_lower_servo_mock;
::testing::NiceMock<BoxServoMock>* box_upper_servo_mock;
::testing::NiceMock<BoxServoManagerMock>* box_servomanager_mock;
#endif // MOCK_BOX_SWITCH_HPP_