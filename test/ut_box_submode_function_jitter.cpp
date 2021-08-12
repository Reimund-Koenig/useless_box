#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mock_arduino.hpp"
#include "mock_box.hpp"
#include "modes/box_submode_function_jitter.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct SubModeFunctionJitter_under_test : public box::SubModeFunctionJitter {
    SubModeFunctionJitter_under_test(box::Servomanager* box_servomanager_mock) :
    SubModeFunctionJitter(box_servomanager_mock) {}
};

class TestSubModeFunctionJitter : public ::testing::Test {
  protected:
    SubModeFunctionJitter_under_test* submode_function_jitter_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        submode_function_jitter_under_test = new SubModeFunctionJitter_under_test(
                                                (box::Servomanager*) box_servomanager_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete submode_function_jitter_under_test;
    }
};

TEST_F(TestModeFunctionJitter, test_wait_init) { EXPECT_TRUE(true); }


TEST_F(TestModeFunctionJitter, test_servomanager_box_test_jitter) {
        EXPECT_FALSE(submode_function_jitter_under_test->run(false, 5,70,85,2));
        for(int i = 0; i<5; i++) {
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(70,2));
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(85,2));
            EXPECT_FALSE(submode_function_jitter_under_test->run(false, 5,70,85,2)); // move 70 percent
            EXPECT_FALSE(submode_function_jitter_under_test->run(false, 5,70,85,2)); // move 90 percent
        }
        EXPECT_TRUE(submode_function_jitter_under_test->run(false, 5,70,85,2));
}
