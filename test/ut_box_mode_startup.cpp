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
                          box::Wait* box_wait_mock, box::Switch* box_switch_mock) :
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

    virtual void test_switch_move_pilot_to_100_percent() {
        int rnd_result = 3;
        const int expected_speed = rnd_result+3;
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(rnd_result));
        EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,expected_speed))
                                            .WillOnce(Return(200));
        EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6))
                                            .WillOnce(Return(20));
        EXPECT_CALL(*box_wait_mock, milliseconds(200)).Times(1);
        EXPECT_FALSE(mode_startup_under_test->run());
    }
};

TEST_F(TestModeStartup, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeStartup, test_start_with_low_switchstate) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(false));
    // run 1
    EXPECT_FALSE(mode_startup_under_test->run());
    test_switch_move_pilot_to_100_percent();
    // // run 2
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // // run 3
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // // run 4
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // // run 5
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();

    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_startup_under_test->run());
}

TEST_F(TestModeStartup, test_start_with_high_switchstate) {
    EXPECT_CALL(*box_switch_mock, is_high()).WillOnce(Return(true));
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(100,6))
                                    .WillOnce(Return(200));

    EXPECT_CALL(*box_wait_mock, milliseconds(200)).Times(1);

    // run 1
    EXPECT_FALSE(mode_startup_under_test->run());
    test_switch_move_pilot_to_100_percent();
    // run 2
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // run 3
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // run 4
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();
    // run 5
    test_switch_move_pilot_to_100_percent();
    test_switch_move_pilot_to_100_percent();

    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6)).WillOnce(Return(1));
    EXPECT_CALL(*box_wait_mock, milliseconds(400)).Times(1);
    EXPECT_TRUE(mode_startup_under_test->run());
}