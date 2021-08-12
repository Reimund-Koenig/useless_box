#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "modes/box_mode_startup.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeStartup_under_test : public box::ModeStartup {
    ModeStartup_under_test(box::Servomanager* box_servomanager_mock) :
    ModeStartup(box_servomanager_mock) {}
};

class TestModeStartup : public ::testing::Test {
  protected:
    ModeStartup_under_test* mode_startup_under_test;
    int box_speed_mock = 5;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        mode_startup_under_test = new ModeStartup_under_test(
                                        (box::Servomanager*) box_servomanager_mock);
    }
    virtual void TearDown() {
        delete mode_startup_under_test;
        delete box_servomanager_mock;
        delete arduino_mock;
    }

};

TEST_F(TestModeStartup, test_wait_init) { EXPECT_TRUE(true); }

TEST_F(TestModeStartup, test_startup) {
    // move both motors to 0
    EXPECT_CALL(*box_servomanager_mock, move_pilot_servo_to_percent(0,6));
    EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(0,6));
    EXPECT_FALSE(mode_startup_under_test->run());
}
