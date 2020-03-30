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


class TestController : public ::testing::Test {
      protected:
    Controller_under_test* controller_under_test;
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
};

TEST_F(TestController, test_controller_init) { EXPECT_TRUE(true); }

TEST_F(TestController, test_controller_standard_calls) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(1).WillOnce(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(1);
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(1).WillOnce(Return(false));
    controller_under_test->run();
}

TEST_F(TestController, test_controller_startup_no_switch_change) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(1).WillOnce(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(1);
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(0);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).Times(1);
    controller_under_test->run();
}

TEST_F(TestController, test_controller_startup_switch_change_no_user_interrupt) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(1).WillOnce(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(1);
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(1).WillOnce(Return(false));;
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(0);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).Times(1);
    controller_under_test->run();
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(AtLeast(1)).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(1);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(1).WillOnce(Return(true));
    controller_under_test->run();
}

TEST_F(TestController, test_controller_test_is_free) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(AtLeast(1)).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(1);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(AtLeast(1)).WillRepeatedly(Return(false));
    EXPECT_CALL(*arduino_mock, random(_)).Times(0);
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(_)).Times(0);
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).Times(0);
    controller_under_test->run();
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt_switch_mode_to_awareness) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(AtLeast(1)).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(AtLeast(1)).WillOnce(Return(true))
                                                                  .WillOnce(Return(false));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(AtLeast(1)).WillOnce(Return(true))
                                                                             .WillOnce(Return(false));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(1);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed())
                                                            .Times(1).WillOnce(Return(false));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*arduino_mock, random(_)).Times(1).WillOnce(Return(70));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_awareness(70)).Times(1).WillOnce(Return(true));
    controller_under_test->run();
    controller_under_test->run();
}

TEST_F(TestController, test_controller_startup_switch_with_user_interrupt_switch_mode_to_normal) {
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(AtLeast(1)).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(AtLeast(1)).WillOnce(Return(true))
                                                                  .WillOnce(Return(false));
    EXPECT_CALL(*box_servomanager_mock, is_no_box_action()).Times(AtLeast(1)).WillOnce(Return(true))
                                                                             .WillOnce(Return(false));
    EXPECT_CALL(*box_servomanager_mock, random_select_if_vice_versa_mode_should_be_changed()).Times(1);
    EXPECT_CALL(*box_wait_mock, is_free()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_servomanager_mock, change_vise_versa_if_required_and_return_is_changed())
                                                            .Times(1).WillOnce(Return(false));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_reset()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*arduino_mock, random(_)).Times(1).WillOnce(Return(75));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).Times(1).WillOnce(Return(true));
    controller_under_test->run();
    controller_under_test->run();
}

TEST_F(TestController, test_controller_run) {
    // InSequence seq;
    //Startup
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).Times(AtLeast(1)).WillRepeatedly(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_)).Times(AtLeast(1));
    EXPECT_CALL(*box_switch_mock, has_changed()).Times(AtLeast(1)).WillRepeatedly(Return(false));
    EXPECT_CALL(*box_wait_mock, is_free()).Times(AtLeast(1)).WillRepeatedly(Return(true));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).Times(1);
    EXPECT_CALL(*arduino_mock, random(_)).Times(1).WillOnce(Return(75));
    controller_under_test->run();
    controller_under_test->run();
}

TEST_F(TestController, test_controller_run_sequence) {
    InSequence seq;
    //Startup
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_));
    EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(70));
    EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_startup()).WillOnce(Return(true));
    controller_under_test->run();
    EXPECT_CALL(*box_sonar_mock, get_average_distance_cm()).WillOnce(Return(70));
    EXPECT_CALL(*box_servomanager_mock, move_steps(_));
    EXPECT_CALL(*box_switch_mock, has_changed()).WillOnce(Return(70));
    EXPECT_CALL(*box_wait_mock, is_free()).WillOnce(Return(true));
    EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(75));
    EXPECT_CALL(*box_mode_manager_mock, run_mode_normal()).Times(1);
    controller_under_test->run();
}

TEST_F(TestController, test_controller_run_modes_until_user_interrupt) {

}