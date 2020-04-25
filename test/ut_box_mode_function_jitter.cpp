#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "box_mode_function_jitter.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeFunctionJitter_under_test : public box::ModeFunctionJitter {
    ModeFunctionJitter_under_test(box::Servomanager* box_servomanager_mock) :
    ModeFunctionJitter(box_servomanager_mock) {}
};

class TestModeFunctionJitter : public ::testing::Test {
  protected:
    ModeFunctionJitter_under_test* mode_function_jitter_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        mode_function_jitter_under_test = new ModeFunctionJitter_under_test(
                                                (box::Servomanager*) box_servomanager_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete mode_function_jitter_under_test;
    }
};

TEST_F(TestModeFunctionJitter, test_wait_init) { EXPECT_TRUE(true); }


TEST_F(TestModeFunctionJitter, test_servomanager_box_test_jitter) {
        EXPECT_FALSE(mode_function_jitter_under_test->run(false, 5,70,85,2));
        for(int i = 0; i<5; i++) {
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(70,2));
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(85,2));
            EXPECT_FALSE(mode_function_jitter_under_test->run(false, 5,70,85,2)); // move 70 percent
            EXPECT_FALSE(mode_function_jitter_under_test->run(false, 5,70,85,2)); // move 90 percent
        }
        EXPECT_TRUE(mode_function_jitter_under_test->run(false, 5,70,85,2));
}
