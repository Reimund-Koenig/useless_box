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
        box_wait_servo_speed_control = new NiceMock<BoxWaitMock>;
        submode_function_jitter_under_test = new SubModeFunctionJitter_under_test(
                                                (box::Servomanager*) box_servomanager_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_servo_speed_control;
        delete submode_function_jitter_under_test;
    }
};

TEST_F(TestSubModeFunctionJitter, test_jitter_compile) { EXPECT_TRUE(1); }

TEST_F(TestSubModeFunctionJitter, test_jitter) {
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(2)) //count
                                        .WillOnce(Return(2)) // speed
                                        .WillOnce(Return(10)); // percent
        submode_function_jitter_under_test->init(70);
        EXPECT_FALSE(submode_function_jitter_under_test->run(false));
        for(int i = 0; i<5; i++) {
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(85, 5));
            EXPECT_FALSE(submode_function_jitter_under_test->run(false)); // move 80 percent
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(70 ,5));
            EXPECT_FALSE(submode_function_jitter_under_test->run(false)); // move 70 percent
        }
        EXPECT_TRUE(submode_function_jitter_under_test->run(false));
}

TEST_F(TestSubModeFunctionJitter, test_jitter_high_init) {
        EXPECT_CALL(*arduino_mock, random(_)).WillOnce(Return(2)) //count
                                        .WillOnce(Return(2)) // speed
                                        .WillOnce(Return(5)); // percent
        submode_function_jitter_under_test->init(92);
        EXPECT_FALSE(submode_function_jitter_under_test->run(false));
        for(int i = 0; i<5; i++) {
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(82, 5));
            EXPECT_FALSE(submode_function_jitter_under_test->run(false)); // move 80 percent
            EXPECT_CALL(*box_servomanager_mock, move_copilot_servo_to_percent(92 ,5));
            EXPECT_FALSE(submode_function_jitter_under_test->run(false)); // move 70 percent
        }
        EXPECT_TRUE(submode_function_jitter_under_test->run(false));
}
