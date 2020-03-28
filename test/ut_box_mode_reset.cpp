#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>

#include "mocks/mock_arduino.hpp"
#include "mocks/mock_box.hpp"
#include "box_mode_reset.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;

struct ModeReset_under_test : public box::ModeReset {
    ModeReset_under_test(box::Servomanager* box_servomanager_mock, box::Wait* box_wait_mock) :
    ModeReset(box_servomanager_mock, box_wait_mock) {}
};

class TestModeReset : public ::testing::Test {
  protected:
    ModeReset_under_test* mode_reset_under_test;
    virtual void SetUp() {
        arduino_mock = new NiceMock<ArduinoMock>;
        box_servomanager_mock = new NiceMock<BoxServoManagerMock>;
        box_wait_mock = new NiceMock<BoxWaitMock>;
        mode_reset_under_test = new ModeReset_under_test(
                                                (box::Servomanager*) box_servomanager_mock,
                                                (box::Wait*) box_wait_mock);
    }
    virtual void TearDown() {
        delete arduino_mock;
        delete box_servomanager_mock;
        delete box_wait_mock;
        delete mode_reset_under_test;
    }
};

TEST_F(TestModeReset, test_wait_init) { EXPECT_TRUE(true); }
